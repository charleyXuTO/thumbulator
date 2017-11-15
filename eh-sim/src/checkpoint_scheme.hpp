#ifndef EH_SIM_CHECKPOINT_SCHEME_HPP
#define EH_SIM_CHECKPOINT_SCHEME_HPP

#include <thumbulator/cpu.hpp>

#include "msp430_energy.hpp"

namespace ehsim {

class checkpoint_scheme {
public:
  double backup_energy() const
  {
    // 16 general purpose registers
    auto const gpr_energy = 16 * MSP430_FLASH_REG;
    // 3 special registers
    auto const special_energy = 3 * MSP430_FLASH_REG;

    return gpr_energy + special_energy;
  }

  bool will_backup() const
  {
    return true;
  }

  void backup()
  {
    architectural_state = thumbulator::cpu;
  }

  double restore_energy() const
  {
    // 16 general purpose registers
    auto const gpr_energy = 16 * MSP430_REG_FLASH;
    // 3 special registers
    auto const special_energy = 3 * MSP430_REG_FLASH;

    return gpr_energy + special_energy;
  }

  void restore()
  {
    thumbulator::cpu = architectural_state;
  }

private:
  thumbulator::cpu_state architectural_state;
};
}

#endif //EH_SIM_CHECKPOINT_SCHEME_HPP
