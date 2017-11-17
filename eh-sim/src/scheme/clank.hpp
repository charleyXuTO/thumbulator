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
      : battery(BATTERYLESS_CAPACITANCE, BATTERYLESS_MAX_CAPACITOR_VOLTAGE)
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
    return idempotent_violation;
  }

  uint64_t backup(stats_bundle *stats) override
  {
    stats->models.back().backup_times += stats->cpu.cycle_count - last_cycle_count;
    stats->models.back().num_backups++;

    last_cycle_count = stats->cpu.cycle_count;

    battery.consume_energy(CLANK_BACKUP_ARCH_ENERGY);
    battery.consume_energy(writeback_backups * MEMENTOS_FLASH_REG);

    // save architectural state
    architectural_state = thumbulator::cpu;

    // the backup has resolved the idempotancy violation
    idempotent_violation = false;

    return backup_time;
  }

  uint64_t restore(stats_bundle *stats) override
  {
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

  void process_address(uint32_t address, operation op, bool value_changed, uint32_t value)
  {
    if(ADDRESS_PREFIX_ENTRIES > 0) {
      // the address prefix buffer exists
      auto const prefix = address & PREFIX_MASK;

      if(addressprefix_buffer.find(prefix) == addressprefix_buffer.end()) {
        // the prefix was not in the buffer
        if(addressprefix_buffer.size() == ADDRESS_PREFIX_ENTRIES) {
          // the address prefix buffer is full

          if(op == operation::write) {
            if(!value_changed) {
              // false write
              return;
            } else if(checkpoint_on_next_addressprefix_write) {
              // idempotent violation - addressprefix buffer was full earlier
              checkpoint();
              process_address(address, op, value_changed, value);

              return;
            }

            // idempotent violation - addressprefix buffer is full
            checkpoint();
            process_address(address, op, value_changed, value);

            return;
          } else {
            // addressprefix buffer is full but this is a read, can delay checkpoint
            checkpoint_on_next_addressprefix_write = true;

            return;
          }
        } else {
          // the buffer is not full
          addressprefix_buffer.insert(prefix);
        }
      }
    }

    if(op == operation::read) {
      if(writefirst_buffer.find(address) == writefirst_buffer.end()) {
        // read dominated

        if(writeback_buffer.find(address) == writeback_buffer.end()) {
          if(readfirst_buffer.find(address) == readfirst_buffer.end()) {
            if(readfirst_buffer.size() < READFIRST_ENTRIES) {
              // the read buffer has room
              readfirst_buffer.insert(address);
            } else {
              // the readfirst buffer is full, delay checkpoint till next write
              checkpoint_on_next_write = true;
            }
          }
        }
      }
    } else if(op == operation::write) {
      if(readfirst_buffer.find(address) != readfirst_buffer.end()) {
        // read dominated

        if(value_changed) {
          if(writeback_buffer.find(address) != writeback_buffer.end()) {
            // the address is already in the writeback buffer

            // update the writeback buffer with the new value
            writeback_buffer[address] = value;
            return;
          } else if(writeback_buffer.size() < WRITEBACK_ENTRIES) {
            writeback_buffer[address] = value;

            // buffer optimization
            readfirst_buffer.erase(address);

            return;
          } else {
            // idempotent violation - writeback buffer is full
            checkpoint();
            process_address(address, op, value_changed, value);

            return;
          }
        }
      } else if(writefirst_buffer.find(address) == writefirst_buffer.end()) {
        // write dominated

        if(writeback_buffer.find(address) == writeback_buffer.end()) {
          if(!value_changed) {
            // false write
            return;
          }

          if(checkpoint_on_next_write) {
            // idempotent violation - readfirst buffer was full earlier
            checkpoint();
            process_address(address, op, value_changed, value);

            return;
          }

          if(writefirst_buffer.size() < WRITEFIRST_ENTRIES) {
            writefirst_buffer.insert(address);
          } else {
            // idempotent violation - writefirst buffer is full
            checkpoint();
            process_address(address, op, value_changed, value);

            return;
          }
        } else {
          // the address is already in the writeback buffer

          // update the writeback buffer with the new value
          writeback_buffer[address] = value;
        }
      }
    }
  }

  uint32_t process_read(uint32_t address, uint32_t data)
  {
    process_address(address, operation::read, false, data);

    auto const it = writeback_buffer.find(address);
    if(it != writeback_buffer.end()) {
      // data is in the writeback buffer, use that value instead
      data = it->second;
    }

    return data;
  }

  uint32_t process_store(uint32_t address, uint32_t old_value, uint32_t value)
  {
    // this may call checkpoint, which would: set idempotent violation to true, clear all buffers
    process_address(address, operation::write, old_value != value, value);

    auto const it = writeback_buffer.find(address);
    if(it != writeback_buffer.end()) {
      // data is in the writeback buffer, do not put the new value in memory until checkpoint
      value = old_value;
    }

    return value;
  }
};
}

#endif //EH_SIM_CLANK_HPP
