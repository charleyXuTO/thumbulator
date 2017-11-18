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
  magical_scheme() : battery(MEMENTOS_CAPACITANCE, MEMENTOS_MAX_CAPACITOR_VOLTAGE)
  {
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
    // energy to fetch and execute
    battery.consume_energy(energy_per_instruction);
  }

  bool is_active(stats_bundle *stats) override
  {
    return battery.energy_stored() > energy_per_instruction;
  }

  bool will_backup(stats_bundle *stats) const override
  {
    return false;
  }

  uint64_t backup(stats_bundle *stats) override
  {
    // do not touch arch/app state
    return 0;
  }

  uint64_t restore(stats_bundle *stats) override
  {
    // do not touch arch/app state
    return 0;
  }

private:
  capacitor battery;

  // based on Mementos numbers for an MSP430
  static constexpr auto energy_per_instruction = MEMENTOS_INSTRUCTION_ENERGY + MEMENTOS_REG_FLASH;
};
}

#endif //EH_SIM_MAGIC_HPP
