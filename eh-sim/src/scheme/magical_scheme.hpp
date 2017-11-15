#ifndef EH_SIM_MAGIC_HPP
#define EH_SIM_MAGIC_HPP

#include "scheme/eh_scheme.hpp"
#include "capacitor.hpp"
#include "msp430_energy.hpp"

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

private:
  capacitor battery;
};
}

#endif //EH_SIM_MAGIC_HPP
