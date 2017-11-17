#ifndef EH_SIM_CLANK_HPP
#define EH_SIM_CLANK_HPP

#include "scheme/eh_scheme.hpp"
#include "scheme/data_sheet.hpp"
#include "capacitor.hpp"
#include "stats.hpp"

#include <thumbulator/memory.hpp>

#include <set>
#include <unordered_map>
#include <thumbulator/cpu.hpp>

namespace ehsim {

/**
 * Based on Clank: Architectural Support for Intermittent Computation.
 *
 * All optimizations, except text segments, are enabled.
 *
 * Clank claims that backups take 40 cycles to save architectural state. Based on thumbulator, we see that
 * are 20 registers, so that's 2 cycles/register.
 *
 * Clank does not provide energy numbers, so we assume Mementos' energy numbers. The source of error here is in
 * the different ISA (Thumb vs RISC) and that the MSP430 is a 16-bit processor while Clank's M0+ is 32-bit.
 */
class clank : public eh_scheme {
public:
  /**
   * Construct the pareto-optimal clank.
   *
   * See Table 2 in the paper.
   */
  clank() : clank(14, 16, 8, 4, 4)
  {
  }

  clank(size_t entry_bits,
      size_t rf_entries,
      size_t wf_entries,
      size_t wb_entries,
      size_t ap_entries)
      : battery(10e-3, BATTERYLESS_MAX_CAPACITOR_VOLTAGE)
      , BITS_PER_ENTRY(entry_bits)
      , READFIRST_ENTRIES(rf_entries)
      , WRITEFIRST_ENTRIES(wf_entries)
      , WRITEBACK_ENTRIES(wb_entries)
      , ADDRESS_PREFIX_ENTRIES(ap_entries)
      , PREFIX_MASK(entry_bits > 29 ? 0 : 0xFFFFFFFF >> (entry_bits + 2) << (entry_bits + 2))
      , MAX_BACKUP_ENERGY(CLANK_BACKUP_ARCH_ENERGY + WRITEBACK_ENTRIES * MEMENTOS_FLASH_REG)
  {
    assert(BITS_PER_ENTRY >= 0);
    assert(READFIRST_ENTRIES >= 1);
    assert(WRITEFIRST_ENTRIES >= 0);
    assert(WRITEBACK_ENTRIES >= 0);
    assert(ADDRESS_PREFIX_ENTRIES >= 0);

    if(ADDRESS_PREFIX_ENTRIES == 0) {
      assert(BITS_PER_ENTRY == 30);
    }

    thumbulator::ram_load_hook = [this](
        uint32_t address, uint32_t data) -> uint32_t { return this->process_read(address, data); };

    thumbulator::ram_store_hook = [this](uint32_t address, uint32_t last_value,
        uint32_t value) -> uint32_t { return this->process_store(address, last_value, value); };
  }

  capacitor &get_battery() override
  {
    return battery;
  }

  uint32_t clock_frequency() const override
  {
    return MEMENTOS_CPU_FREQUENCY;
  }

  void execute_instruction(stats_bundle *stats) override
  {
    battery.consume_energy(MEMENTOS_INSTRUCTION_ENERGY);

    stats->models.back().instruction_energy += MEMENTOS_INSTRUCTION_ENERGY;
  }

  bool is_active(stats_bundle *stats) const override
  {
    if(battery.energy_stored() == battery.maximum_energy_stored()) {
      assert(!active);
      active = true;
    } else if(battery.energy_stored() <= MAX_BACKUP_ENERGY) {
      active = false;
    }

    return active;
  }

  bool will_backup(stats_bundle *stats) const override
  {
    // TODO: watchdog timer
    auto const energy_warning = CLANK_BACKUP_ARCH_ENERGY + MEMENTOS_INSTRUCTION_ENERGY;

    return idempotent_violation || (battery.energy_stored() <= energy_warning);
  }

  uint64_t backup(stats_bundle *stats) override
  {
    stats->models.back().backup_times += stats->cpu.cycle_count - last_cycle_count;
    stats->models.back().num_backups++;

    last_cycle_count = stats->cpu.cycle_count;

    battery.consume_energy(CLANK_BACKUP_ARCH_ENERGY);
    //battery.consume_energy(writeback_backups * MEMENTOS_FLASH_REG);

    // save architectural state
    architectural_state = thumbulator::cpu;

    // the backup has resolved the idempotancy violation
    idempotent_violation = false;

    return backup_time;
  }

  uint64_t restore(stats_bundle *stats) override
  {
    // allocate space for a new active period model
    stats->models.emplace_back();

    // restore saved architectural state
    thumbulator::cpu_reset();
    thumbulator::cpu = architectural_state;

    battery.consume_energy(CLANK_RESTORE_ENERGY);

    // assume memory access latency for reads and writes is the same
    return CLANK_BACKUP_ARCH_TIME;
  }

private:
  capacitor battery;

  uint64_t last_cycle_count = 0u;

  thumbulator::cpu_state architectural_state{};
  mutable bool active = false;

  size_t const BITS_PER_ENTRY;
  size_t const READFIRST_ENTRIES;
  size_t const WRITEFIRST_ENTRIES;
  size_t const WRITEBACK_ENTRIES;
  size_t const ADDRESS_PREFIX_ENTRIES;
  int const PREFIX_MASK;
  double const MAX_BACKUP_ENERGY;

  bool idempotent_violation = false;

  std::set<uint32_t> readfirst_buffer;
  std::set<uint32_t> writefirst_buffer;
  std::set<uint32_t> addressprefix_buffer;
  std::unordered_map<uint32_t, uint32_t> writeback_buffer;

  bool checkpoint_on_next_write = false;
  bool checkpoint_on_next_addressprefix_write = false;

  uint64_t backup_time = 0;
  size_t writeback_backups = 0;

  void checkpoint()
  {
    writeback_backups = writeback_buffer.size();
    backup_time = CLANK_BACKUP_ARCH_TIME;
    if(!writeback_buffer.empty()) {
      // time to access writeback buffer
      backup_time += CLANK_BACKUP_WBB_ACCESS_TIME;
      // time to write values in writeback buffer
      backup_time += (CLANK_BACKUP_WBB_ENTRY_TIME * writeback_buffer.size());
    }

    for(auto const &memory_access : writeback_buffer) {
      auto const address = memory_access.first;
      auto const value = memory_access.second;

      // actually save the data into non-volatile memory
      thumbulator::RAM[(address & RAM_ADDRESS_MASK) >> 2] = value;
    }

    // set flags
    idempotent_violation = true;
    checkpoint_on_next_write = false;
    checkpoint_on_next_addressprefix_write = false;

    // clear all buffers
    readfirst_buffer.clear();
    writefirst_buffer.clear();
    addressprefix_buffer.clear();
    writeback_buffer.clear();
  }

  enum class operation { read, write };

  bool try_insert(std::set<uint32_t> *buffer, uint32_t address, size_t max_buffer_size)
  {
    if(buffer->size() < max_buffer_size) {
      buffer->insert(address);

      return true;
    }

    return false;
  }

  void process_address(uint32_t address, operation op, bool value_changed, uint32_t value)
  {
    auto const readfirst_it = readfirst_buffer.find(address);
    auto const readfirst_hit = readfirst_it != readfirst_buffer.end();

    auto const writefirst_it = writefirst_buffer.find(address);
    auto const writefirst_hit = writefirst_it != writefirst_buffer.end();

    if(!readfirst_hit && !writefirst_hit) {
      // the memory access is in neither buffer

      // add the memory access to the appropriate buffer
      bool was_added = false;
      if(op == operation::read) {
        was_added = try_insert(&readfirst_buffer, address, READFIRST_ENTRIES);
      } else if(op == operation::write) {
        was_added = try_insert(&writefirst_buffer, address, WRITEFIRST_ENTRIES);
      }

      if(!was_added) {
        // idempotent violation - a buffer was full
        checkpoint();
      }
    } else if(op == operation::write && readfirst_hit) {
      // idempotent violation - write to read-dominated address
      checkpoint();
    }
  }

  uint32_t process_read(uint32_t address, uint32_t data)
  {
    process_address(address, operation::read, false, data);

    return data;
  }

  uint32_t process_store(uint32_t address, uint32_t old_value, uint32_t value)
  {
    // this may call checkpoint, which would: set idempotent violation to true, clear all buffers
    process_address(address, operation::write, old_value != value, value);

    return value;
  }
};
}

#endif //EH_SIM_CLANK_HPP
