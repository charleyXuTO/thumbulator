#ifndef EH_SIM_CLANK_HPP
#define EH_SIM_CLANK_HPP

#include "scheme/eh_scheme.hpp"
#include "scheme/data_sheet.hpp"
#include "capacitor.hpp"
#include "stats.hpp"

#include <thumbulator/memory.hpp>
#include <thumbulator/cpu.hpp>
#include <set>
#include <unordered_map>
#include <thumbulator/cpu.hpp>

namespace ehsim {

/**
 * Based on Clank: Architectural Support for Intermittent Computation.
 *
 * Only implements the read- and write-first buffers.
 */
class clank : public eh_scheme {
public:
  /**
   * Construct a default clank configuration.
   */

  clank() : clank(read_buffer_size, 8, 4, 8000)
  {
  }

  clank(size_t rf_entries, size_t wf_entries, size_t ap_entries,int watchdog_period)
      : battery(NVP_CAPACITANCE, MEMENTOS_MAX_CAPACITOR_VOLTAGE, MEMENTOS_MAX_CURRENT)
      , WATCHDOG_PERIOD(watchdog_period)
      , READFIRST_ENTRIES(rf_entries)
      , WRITEFIRST_ENTRIES(wf_entries)
      , WRITEBACK_ENTIRES(wf_entries)
      , ADDRESS_PREFIX_ENTRIES(ap_entries)
      , MAX_BACKUP_ENERGY(CLANK_BACKUP_ARCH_ENERGY)
      , performance_watchdog(WATCHDOG_PERIOD)
      , progress_watchdog(WATCHDOG_PERIOD)
      , progress_check(PROGRESS_CHECK)
      , progress_divider(PROGRESS_DIVIDER)
  {
    assert(READFIRST_ENTRIES >= 1);
    assert(WRITEFIRST_ENTRIES >= 0);

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
    return CORTEX_M0PLUS_FREQUENCY;
  }

  double min_energy_to_power_on(stats_bundle *stats) override
  {
    return battery.maximum_energy_stored();
  }

  void execute_instruction(stats_bundle *stats) override
  {
    auto const elapsed_cycles = stats->cpu.cycle_count - last_tick;
    last_tick = stats->cpu.cycle_count;

    performance_watchdog -= elapsed_cycles;
    if (progress_check == 1) {
      progress_watchdog -= elapsed_cycles;
    }
    // clank's instruction energy is in Energy-per-Cycle
    auto const instruction_energy = CLANK_INSTRUCTION_ENERGY * elapsed_cycles;
    battery.consume_energy(instruction_energy);
    stats->models.back().energy_for_instructions += instruction_energy;
  }

  bool is_active(stats_bundle *stats) override
  {
    if(battery.energy_stored() >= battery.maximum_energy_stored()) {
      power_on();
    } else if(battery.energy_stored() < MAX_BACKUP_ENERGY) {
      power_off();
    }

    return active;
  }

  bool will_backup(stats_bundle *stats) const override
  {
    if(battery.energy_stored() < MAX_BACKUP_ENERGY) {
      return false;
    }
    if (progress_watchdog <= 0 ) {
      return true;
    }
    if(performance_watchdog <= 0) {
      return true;
    }

    return idempotent_violation;
  }

  uint64_t backup(stats_bundle *stats) override
  {
    auto &active_stats = stats->models.back();
    active_stats.num_backups++;

    active_stats.time_between_backups += stats->cpu.cycle_count - last_backup_cycle;
    last_backup_cycle = stats->cpu.cycle_count;

    // save architectural state
    architectural_state = thumbulator::cpu;
    //save SRAM variables
    std::copy(std::begin(thumbulator::RAM), std::end(thumbulator::RAM), std::begin(backup_RAM));
    stats->cpu.program_counter = thumbulator::cpu_get_pc();
    // reset the watchdog
    performance_watchdog = WATCHDOG_PERIOD;
    // clear idempotency-tracking buffers
    clear_buffers();
    // the backup has resolved the idempotancy violation and/or exception
    idempotent_violation = false;
    numberOfBackups++;
    active_stats.energy_for_backups += CLANK_BACKUP_ARCH_ENERGY;
    battery.consume_energy(CLANK_BACKUP_ARCH_ENERGY);
    stats->system.total_energy_backup += CLANK_BACKUP_ARCH_ENERGY;
    backup_check = 0; //disabling the progress watchdog
    progress_check  =0;
    stats->cpu.dead_instruction = 0;
    test_instructions = stats->cpu.instruction_count;
    return CLANK_BACKUP_ARCH_TIME;
  }

  uint64_t restore(stats_bundle *stats) override
  {
    performance_watchdog = WATCHDOG_PERIOD;
    if (backup_check == 0) { // resetting and disabling progress watchdog
      progress_check = 0;
      progress_divider = 1;
      progress_watchdog = WATCHDOG_PERIOD;
    }
    else { //enabling and decreasing progress watchdog
      progress_check = 1;
      progress_divider = progress_divider*2;
      progress_watchdog = WATCHDOG_PERIOD/progress_divider;
    }
    backup_check = 1;
    last_backup_cycle = stats->cpu.cycle_count;

    // restore saved architectural state
    thumbulator::cpu_reset();
    thumbulator::cpu = architectural_state;
    //restore SRAM variables from last backup
    std::copy(std::begin(backup_RAM), std::end(backup_RAM), std::begin(thumbulator::RAM));
    /*
    if (stats->cpu.instruction_count-test_instructions != stats->cpu.dead_instruction) {
        printf("wrong");
    }
    
    if (thumbulator::cpu_get_pc() !=stats->cpu.program_counter) {
        printf("wrong");
    }
     */
    stats->models.back().energy_for_restore = CLANK_RESTORE_ENERGY;
    battery.consume_energy(CLANK_RESTORE_ENERGY);
    stats ->system.total_energy_restore +=CLANK_RESTORE_ENERGY;
    numberOfRestores++;
    stats->cpu.dead_instruction_count= stats->cpu.dead_instruction_count + stats->cpu.dead_instruction;
    stats->cpu.dead_instruction = 0;
    // assume memory access latency for reads and writes is the same
    return CLANK_BACKUP_ARCH_TIME;
  }

  double estimate_progress(eh_model_parameters const &eh) const override
  {
    return estimate_eh_progress(eh, dead_cycles::average_case, CLANK_OMEGA_R, CLANK_SIGMA_R, CLANK_A_R,
        CLANK_OMEGA_B, CLANK_SIGMA_B, CLANK_A_B);
  }

private:
  capacitor battery;

  uint64_t last_backup_cycle = 0u;
  uint64_t last_tick = 0u;

  thumbulator::cpu_state architectural_state{};
  uint32_t backup_RAM[RAM_SIZE_BYTES >> 2];
  bool active = false;

  int const WATCHDOG_PERIOD;
  size_t const READFIRST_ENTRIES;
  size_t const WRITEFIRST_ENTRIES;
  size_t const WRITEBACK_ENTIRES;
  size_t const ADDRESS_PREFIX_ENTRIES;

  double const MAX_BACKUP_ENERGY;
  int performance_watchdog;
  int progress_watchdog;
  bool idempotent_violation = false;
  int next_write;
  int progress_check;
  int progress_divider;
  int backup_check;
  int ap_counter;

  int test_instructions;
  std::set<uint32_t> readfirst_buffer;
  std::set<uint32_t> writefirst_buffer;
  std::set<uint32_t> writeback_buffer;
  int address_prefix_buffer[4];

  enum class operation { read, write };

  void clear_buffers()
  {
    readfirst_buffer.clear();
    writefirst_buffer.clear();
    writeback_buffer.clear();
    for (int i = 0 ; i<4; i++) {
        address_prefix_buffer[i] = 0;
    }
    ap_counter = 0; //reset counter
  }

  void power_on()
  {
    active = true;
  }

  void power_off()
  {
    active = false;
    clear_buffers();
  }

  bool try_insert(std::set<uint32_t> *buffer, uint32_t const address, size_t const max_buffer_size)
  {
    if(buffer->size() < max_buffer_size) {
      buffer->insert(address);

      return true;
    }

    return false;
  }

  /**
   * Detection logic for idempotency violations.
   */
  void detect_violation(uint32_t address, operation op, uint32_t old_value, uint32_t value)
  {

    int buffer_address = address;
    bool ap_buffer_check = true;
    if (apb_clank_selected) {
        int ap_address = (address - (address % 1000)) / 1000; //creating 24 bit address for buffer
        buffer_address = address % 1000; //simple 4 bit info for actual buffers
        //auto const ap_it = address_prefix_buffer.find(ap_address);
        //auto const ap_hit = ap_it != address_prefix_buffer.end();
        ap_buffer_check = false;
        for (int i = 0; i < ap_counter; i++) {
            if (address_prefix_buffer[i] == ap_address) { //checking for existing 24 bit address
                ap_buffer_check = true;
                buffer_address = buffer_address + (i + 1) * 10000;
                break;
            }
        }
        if (!ap_buffer_check && ap_counter <= 3) { //adding new 24 bit address
            address_prefix_buffer[ap_counter] = ap_address;
            buffer_address = buffer_address + (ap_counter + 1) * 10000;
            ap_counter++;
            ap_buffer_check = true;
        } else if (!ap_buffer_check && ap_counter > 3) {
            idempotent_violation = true; //address pre-buffer overflow
            bufferOverflowViolations++;
        }
    }
    if (ap_buffer_check == true) {
    auto const readfirst_it = readfirst_buffer.find(buffer_address);
    auto const readfirst_hit = readfirst_it != readfirst_buffer.end();

    auto const writefirst_it = writefirst_buffer.find(buffer_address);
    auto const writefirst_hit = writefirst_it != writefirst_buffer.end();

    auto const writeback_it = writeback_buffer.find(buffer_address);
    auto const writeback_hit = writeback_it != writeback_buffer.end();

    if (op==operation::write && next_write == 1) { // delay checkpoint until next write
        idempotent_violation = true;
        next_write = 0;
    }
    else if(!readfirst_hit && !writefirst_hit) {
      // the memory access is in neither buffer

      // add the memory access to the appropriate buffer
      bool was_added = false;
      if (rde_clank_selected) {
        if (op == operation::read && writeback_hit) { //if the address is already contained in the writeback_buffer
          was_added = true;
        }
      }
      if(op == operation::read && was_added == false) {
        was_added = try_insert(&readfirst_buffer, buffer_address, READFIRST_ENTRIES);
      } else if(op == operation::write && was_added == false) {
        was_added = try_insert(&writefirst_buffer, buffer_address, WRITEFIRST_ENTRIES);
      }
      if (nwfo_clank_selected == true ) {
        if(!was_added && op == operation::read && next_write!=1) { //only if the read buffer is full
          // idempotent violation - read buffer was full
          //idempotent_violation = true;
          if (lc_clank_selected == true) {
            next_write = 1; //delay checkpoint until next write so ignore all read overflows
          } else {
            idempotent_violation = true;
          }
          bufferOverflowViolations++;
        }
      }
      else {
        if (!was_added && next_write!=1) {
          if (lc_clank_selected == true && op == operation::read ) {
            next_write = 1;
          }
          else {
            idempotent_violation = true;
          }
          bufferOverflowViolations++;
        }
      }
    } else if(op == operation::write && readfirst_hit) {
      // idempotent violation - write to read-dominated address
        if (wbb_clank_selected == true) {
            bool was_added = false;
            was_added = try_insert(&writeback_buffer, buffer_address, WRITEBACK_ENTIRES);
            if (!was_added) {
                idempotent_violation = true;
                bufferWriteViolations++;
            }
            else if (rde_clank_selected) {
                readfirst_buffer.erase(buffer_address); //erasing old read address
            }
        }
        else {
            idempotent_violation = true;
            bufferWriteViolations++;
        }

    }
    }
  }

  uint32_t process_read(uint32_t address, uint32_t value)
  {
    detect_violation(address, operation::read, value, value);

    if(idempotent_violation && battery.energy_stored() < MAX_BACKUP_ENERGY) {
      power_off();
    }

    return value;
  }

  uint32_t process_store(uint32_t address, uint32_t old_value, uint32_t value)
  {
    detect_violation(address, operation::write, old_value, value);

    if(idempotent_violation && battery.energy_stored() < MAX_BACKUP_ENERGY) {
      power_off();

      return old_value;
    }

    return value;
  }
};
}

#endif //EH_SIM_CLANK_HPP
