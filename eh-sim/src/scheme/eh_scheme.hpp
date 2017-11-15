#ifndef EH_SIM_SCHEME_HPP
#define EH_SIM_SCHEME_HPP

namespace ehsim {

class capacitor;

/**
 * An abstract checkpointing scheme.
 */
class eh_scheme {
public:
  virtual double energy_threshold() const = 0;

  virtual double energy_instruction() const = 0;

  virtual bool will_backup() const = 0;

  virtual void backup() = 0;

  virtual void restore() = 0;
};
}

#endif //EH_SIM_SCHEME_HPP
