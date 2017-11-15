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

  double energy_threshold() const override
  {
    return energy_instruction();
  }

  double energy_instruction() const override
  {
    // energy to fetch and execute
    return MSP430_INSTRUCTION_ENERGY + MSP430_REG_FLASH;
  }

  bool will_backup(stats_bundle const &stats) const override
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
};
}

#endif //EH_SIM_MAGIC_HPP
