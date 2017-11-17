#ifndef EH_SIM_BACKUP_EVERY_CYCLE_HPP
#define EH_SIM_BACKUP_EVERY_CYCLE_HPP

#include "scheme/eh_scheme.hpp"
#include "scheme/data_sheet.hpp"
#include "capacitor.hpp"
#include "stats.hpp"

namespace ehsim {

/**
 * Based on Architecture Exploration for Ambient Energy Harvesting Nonvolatile Processors.
 *
 * See the data relating to the BEC scheme.
 */
class backup_every_cycle : public eh_scheme {
public:
  backup_every_cycle() : battery(NVP_CAPACITANCE, MEMENTOS_MAX_CAPACITOR_VOLTAGE)
  {
  }

  capacitor &get_battery() override
  {
    return battery;
  }

  uint32_t clock_frequency() const override
  {
    return NVP_CPU_FREQUENCY;
  }

  void execute_instruction(stats_bundle *stats) override
  {
    battery.consume_energy(NVP_INSTRUCTION_ENERGY);

    stats->models.back().instruction_energy += NVP_INSTRUCTION_ENERGY;
  }

  bool is_active(stats_bundle *stats) override
  {
    auto required_energy = NVP_INSTRUCTION_ENERGY + NVP_BEC_BACKUP_ENERGY;

    if(stats->cpu.instruction_count != 0) {
      // we only need to restore if an instruction has been executed
      required_energy += NVP_BEC_RESTORE_ENERGY;
    }

    return battery.energy_stored() > required_energy;
  }

  bool will_backup(stats_bundle *stats) const override
  {
    return true;
  }

  uint64_t backup(stats_bundle *stats) override
  {
    // do not touch arch/app state, assume it is all non-volatile

    battery.consume_energy(NVP_BEC_BACKUP_ENERGY);

    stats->models.back().time_between_backups += stats->cpu.cycle_count - last_cycle_count;
    stats->models.back().num_backups++;

    last_cycle_count = stats->cpu.cycle_count;

    return NVP_BEC_BACKUP_TIME;
  }

  uint64_t restore(stats_bundle *stats) override
  {
    // do not touch arch/app state, assume it is all non-volatile

    battery.consume_energy(NVP_BEC_RESTORE_ENERGY);

    // allocate space for a new active period model
    stats->models.emplace_back();

    return NVP_BEC_RESTORE_TIME;
  }

private:
  capacitor battery;

  uint64_t last_cycle_count = 0u;
};
}

#endif //EH_SIM_BACKUP_EVERY_CYCLE_HPP
