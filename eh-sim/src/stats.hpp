#ifndef EH_SIM_STATS_HPP
#define EH_SIM_STATS_HPP

#include <chrono>
#include <deque>

namespace ehsim {
struct cpu_stats {
  /**
   * Number of executed instructions.
   */
  uint64_t instruction_count = 0u;

  /**
   * Number of cycles ticked.
   */
  uint64_t cycle_count = 0u;
};

struct system_stats {
  /**
   * Total execution time, including active and charging periods.
   */
  std::chrono::nanoseconds time{0};

  /**
   * Amount of energy harvested (nJ).
   */
  double energy_harvested = 0.0;

  /**
   * Remaining energy in battery.
   */
  double energy_remaining = 0.0;
};

/**
 * A model of one active period.
 */
struct eh_model {
  /**
   * The accumulated cycle count of all backups.
   */
  uint64_t backup_times;

  /**
   * The number of backups performed.
   */
  uint32_t num_backups;

  /**
   * The total amount of energy spent on executing instructions.
   */
  double instruction_energy = 0.0;
};

struct stats_bundle {
  system_stats system;
  cpu_stats cpu;

  /**
   * Model of multiple active periods.
   */
  std::deque<eh_model> models;
};
}

#endif //EH_SIM_STATS_HPP
