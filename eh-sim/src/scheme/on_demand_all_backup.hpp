#ifndef EH_SIM_ON_DEMAND_ALL_BACKUP_HPP
#define EH_SIM_ON_DEMAND_ALL_BACKUP_HPP

#include "scheme/eh_scheme.hpp"
#include "scheme/data_sheet.hpp"
#include "capacitor.hpp"
#include "stats.hpp"

namespace ehsim {

/**
 * Based on Architecture Exploration for Ambient Energy Harvesting Nonvolatile Processors.
 *
 * See the data relating to the ODAB scheme.
 */
class on_demand_all_backup : public eh_scheme {
public:
  on_demand_all_backup() : battery(NVP_CAPACITANCE, MEMENTOS_MAX_CAPACITOR_VOLTAGE)
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
    if(battery.energy_stored() == battery.maximum_energy_stored()) {
      assert(!active);
      active = true;
    }

    return active;
  }

  bool will_backup(stats_bundle *stats) const override
  {
    // can't backup if the power is off
    assert(active);

    auto const energy_warning = NVP_ODAB_BACKUP_ENERGY + NVP_INSTRUCTION_ENERGY;

    return battery.energy_stored() <= energy_warning;
  }

  uint64_t backup(stats_bundle *stats) override
  {
    // can't backup if the power is off
    assert(active);

    battery.consume_energy(NVP_ODAB_BACKUP_ENERGY);

    // we only backup when moving to power-off mode
    active = false;

    stats->models.back().time_between_backups += stats->cpu.cycle_count - last_cycle_count;
    stats->models.back().num_backups++;

    last_cycle_count = stats->cpu.cycle_count;

    return NVP_ODAB_BACKUP_TIME;
  }

  uint64_t restore(stats_bundle *stats) override
  {
    // is_active should have set this to true before a restore can happen
    assert(active);

    battery.consume_energy(NVP_ODAB_RESTORE_ENERGY);

    // allocate space for a new active period model
    stats->models.emplace_back();

    return NVP_ODAB_RESTORE_TIME;
  }

private:
  capacitor battery;

  uint64_t last_cycle_count = 0u;

  bool active = false;
};
}
#endif //EH_SIM_ON_DEMAND_ALL_BACKUP_HPP
