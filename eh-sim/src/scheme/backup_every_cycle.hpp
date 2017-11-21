#ifndef EH_SIM_BACKUP_EVERY_CYCLE_HPP
#define EH_SIM_BACKUP_EVERY_CYCLE_HPP

#include "scheme/eh_scheme.hpp"
#include "scheme/data_sheet.hpp"
#include "capacitor.hpp"
#include "stats.hpp"

namespace ehsim {

/**
 * Based on Architecture Exploration for Ambient Energy Harvesting Nonvolatile Processors.
 *
 * See the data relating to the BEC scheme.
 */
class backup_every_cycle : public eh_scheme {
public:
  backup_every_cycle() : battery(NVP_CAPACITANCE, MEMENTOS_MAX_CAPACITOR_VOLTAGE)
  {
  }

  capacitor &get_battery() override
  {
    return battery;
  }

  uint32_t clock_frequency() const override
  {
    return NVP_CPU_FREQUENCY;
  }

  void execute_instruction(stats_bundle *stats) override
  {
    battery.consume_energy(NVP_INSTRUCTION_ENERGY);

    stats->models.back().energy_for_instructions += NVP_INSTRUCTION_ENERGY;
  }

  bool is_active(stats_bundle *stats) override
  {
    auto required_energy = NVP_INSTRUCTION_ENERGY + NVP_BEC_BACKUP_ENERGY;

    if(stats->cpu.instruction_count != 0) {
      // we only need to restore if an instruction has been executed
      required_energy += NVP_BEC_RESTORE_ENERGY;
    }

    return battery.energy_stored() > required_energy;
  }

  bool will_backup(stats_bundle *stats) const override
  {
    return true;
  }

  uint64_t backup(stats_bundle *stats) override
  {
    // do not touch arch/app state, assume it is all non-volatile
    auto &active_stats = stats->models.back();
    active_stats.num_backups++;

    active_stats.time_between_backups += stats->cpu.cycle_count - last_backup_cycle;
    last_backup_cycle = stats->cpu.cycle_count;

    active_stats.energy_for_backups += NVP_BEC_BACKUP_ENERGY;
    battery.consume_energy(NVP_BEC_BACKUP_ENERGY);

    return NVP_BEC_BACKUP_TIME;
  }

  uint64_t restore(stats_bundle *stats) override
  {
    last_backup_cycle = stats->cpu.cycle_count;

    // do not touch arch/app state, assume it is all non-volatile

    stats->models.back().energy_for_restore = NVP_BEC_RESTORE_ENERGY;
    battery.consume_energy(NVP_BEC_RESTORE_ENERGY);

    return NVP_BEC_RESTORE_TIME;
  }

  double estimate_progress(eh_model_parameters const &eh) const override
  {
    double const tau_D = 0.0; // BEC has no dead cycles

    // calculate dead energy - Equation 5
    double const e_D = (eh.epsilon - eh.epsilon_C) * tau_D;

    double e_R = 0.0;
    if(eh.do_restore) {
      // calculate restore energy - Equation 8
      e_R = (NVP_BEC_OMEGA_R - eh.epsilon_C / NVP_BEC_SIGMA_R) * (NVP_BEC_A_R + eh.alpha_R * tau_D);
    }

    // numerator of Equation 9
    double const numerator = 1 - (e_D / eh.E) - (e_R / eh.E);

    // calculate backup energy = Equation 9
    double const e_B =
        (NVP_BEC_OMEGA_B - eh.epsilon_C / NVP_BEC_SIGMA_B) * (NVP_BEC_A_B + eh.alpha_B * eh.tau_B);

    // denominator of Equation 9 (assume e_Theta is 0)
    double const denominator =
        (1 + e_B / ((eh.epsilon - eh.epsilon_C) * eh.tau_B)) * (1 - eh.epsilon_C / eh.epsilon);

    return numerator / denominator;
  }

private:
  capacitor battery;

  uint64_t last_backup_cycle = 0u;
};
}

#endif //EH_SIM_BACKUP_EVERY_CYCLE_HPP
