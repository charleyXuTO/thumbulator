#ifndef EH_SIM_SCHEME_HPP
#define EH_SIM_SCHEME_HPP

namespace ehsim {

class capacitor;
struct stats_bundle;

/**
 * An abstract checkpointing scheme.
 */
class eh_scheme {
public:
  virtual capacitor &get_battery() = 0;

  virtual uint32_t clock_frequency() const = 0;

  virtual double energy_threshold() const = 0;

  virtual double energy_instruction() const = 0;

  virtual bool will_backup(stats_bundle const &stats) const = 0;

  virtual void backup(stats_bundle *stats) = 0;

  virtual void restore(stats_bundle *stats) = 0;
};
}

#endif //EH_SIM_SCHEME_HPP
