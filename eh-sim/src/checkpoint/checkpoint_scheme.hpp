#ifndef EH_SIM_SCHEME_HPP
#define EH_SIM_SCHEME_HPP

namespace ehsim {

/**
 * An abstract checkpointing scheme.
 */
class checkpoint_scheme {
public:
  virtual bool will_backup() const = 0;

  virtual double backup_energy() const = 0;

  virtual void backup() = 0;

  virtual double restore_energy() const = 0;

  virtual void restore() = 0;
};
}

#endif //EH_SIM_SCHEME_HPP
