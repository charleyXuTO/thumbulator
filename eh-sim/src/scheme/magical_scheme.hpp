#ifndef EH_SIM_MAGIC_HPP
#define EH_SIM_MAGIC_HPP

#include <thumbulator/cpu.hpp>

#include "scheme/eh_scheme.hpp"
#include "capacitor.hpp"
#include "msp430_energy.hpp"
#include "stats.hpp"

namespace ehsim {

/**
 * A magical checkpointing scheme.
 *
 * Does not waste any energy, because it is magic.
 */
class magical_scheme : public eh_scheme {
public:
  magical_scheme() : battery(4.7e-5)
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

  bool is_active() const override
  {
    return battery.energy_stored() > energy_per_instruction;
  }

  bool will_backup(stats_bundle *stats) const override
  {
    // always backup when it is an option
    return true;
  }

  void backup(stats_bundle *stats) override
  {
    // do not touch arch/app state
    stats->models.back().backup_times.push_back(stats->cpu.cycle_count);
  }

  void restore(stats_bundle *stats) override
  {
    // do not touch arch/app state

    // allocate space for a new active period model
    stats->models.emplace_back();
  }

private:
  capacitor battery;
  // based on Mementos numbers for an MSP430
  static constexpr auto energy_per_instruction = MSP430_INSTRUCTION_ENERGY + MSP430_REG_FLASH;
};
}

#endif //EH_SIM_MAGIC_HPP
