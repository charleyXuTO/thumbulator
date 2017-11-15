#ifndef EH_SIM_BACKUP_EVERY_CYCLE_HPP
#define EH_SIM_BACKUP_EVERY_CYCLE_HPP

#include "scheme/eh_scheme.hpp"
#include "capacitor.hpp"
#include "stats.hpp"

namespace ehsim {

class backup_every_cycle : public eh_scheme {
public:
  backup_every_cycle() : battery(4.7e-5)
  {
  }

  capacitor &get_battery() override
  {
    return battery;
  }

  uint32_t clock_frequency() const override
  {
    return 8000;
  }

  double energy_threshold() const override
  {
    return energy_instruction();
  }

  double energy_instruction() const override
  {
    // based on Architecture Exploration for Ambient Energy Harvesting Nonvolatile Processors, Figure 11
    return 0.03125;
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

#endif //EH_SIM_BACKUP_EVERY_CYCLE_HPP
