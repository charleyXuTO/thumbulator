#ifndef EH_SIM_MAGIC_HPP
#define EH_SIM_MAGIC_HPP

#include <thumbulator/cpu.hpp>

#include "scheme/eh_scheme.hpp"
#include "scheme/data_sheet.hpp"
#include "capacitor.hpp"
#include "stats.hpp"

namespace ehsim {

/**
 * A magical checkpointing scheme.
 *
 * Does not waste any energy, because it is magic.
 */
class magical_scheme : public eh_scheme {
public:
  magical_scheme() : battery(BATTERYLESS_CAPACITANCE, BATTERYLESS_MAX_CAPACITOR_VOLTAGE)
  {
  }

  capacitor &get_battery() override
  {
    return battery;
  }

  uint32_t clock_frequency() const override
  {
    return thumbulator::CPU_FREQ;
  }

  void execute_instruction(stats_bundle *stats) override
  {
    // energy to fetch and execute
    battery.consume_energy(energy_per_instruction);
  }

  bool is_active(stats_bundle *stats) const override
  {
    return battery.energy_stored() > energy_per_instruction;
  }

  bool will_backup(stats_bundle *stats) const override
  {
    // always backup when it is an option
    return true;
  }

  uint64_t backup(stats_bundle *stats) override
  {
    // do not touch arch/app state

    stats->models.back().backup_times += stats->cpu.cycle_count - last_cycle_count;
    stats->models.back().num_backups++;

    last_cycle_count = stats->cpu.cycle_count;

    return 0;
  }

  uint64_t restore(stats_bundle *stats) override
  {
    // do not touch arch/app state

    // allocate space for a new active period model
    stats->models.emplace_back();

    return 0;
  }

private:
  capacitor battery;

  uint64_t last_cycle_count = 0u;

  // based on Mementos numbers for an MSP430
  static constexpr auto energy_per_instruction = MEMENTOS_INSTRUCTION_ENERGY + MEMENTOS_REG_FLASH;
};
}

#endif //EH_SIM_MAGIC_HPP
