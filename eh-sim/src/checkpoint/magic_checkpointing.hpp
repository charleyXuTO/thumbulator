#ifndef EH_SIM_MAGIC_HPP
#define EH_SIM_MAGIC_HPP

#include "checkpoint_scheme.hpp"

namespace ehsim {

/**
 * A magical checkpointing scheme.
 *
 * Does not waste any energy, because it is magic.
 */
class magic_checkpointing : public checkpoint_scheme {
public:
  bool will_backup() const override
  {
    // always backup when it is an option
    return true;
  }

  double backup_energy() const override
  {
    // use up no energy for backups
    return 0.0;
  }

  void backup() override
  {
    // do not touch arch/app state
  }

  double restore_energy() const override
  {
    // use up no energy for restores
    return 0.0;
  }

  void restore() override
  {
    // do not touch arch/app state
  }
};
}

#endif //EH_SIM_MAGIC_HPP
