#ifndef EH_SIM_MAGIC_HPP
#define EH_SIM_MAGIC_HPP

#include "checkpoint/checkpoint_scheme.hpp"
#include "msp430_energy.hpp"

namespace ehsim {

/**
 * A magical checkpointing scheme.
 *
 * Does not waste any energy, because it is magic.
 */
class magic_checkpointing : public checkpoint_scheme {
public:
  double energy_threshold() const override
  {
    return energy_instruction();
  }

  double energy_instruction() const override
  {
    // energy to fetch and execute
    return MSP430_INSTRUCTION_ENERGY + MSP430_REG_FLASH;
  }

  bool will_backup() const override
  {
    // always backup when it is an option
    return true;
  }

  void backup() override
  {
    // do not touch arch/app state
  }

  void restore() override
  {
    // do not touch arch/app state
  }
};
}

#endif //EH_SIM_MAGIC_HPP
