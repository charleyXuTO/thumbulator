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

struct active_stats {
  /**
   * The accumulated cycle count of the time between all backups.
   */
  uint64_t time_between_backups = 0;

  /**
   * The accumulated cycle count the spent doing backups.
   */
  uint64_t time_for_backups = 0;

  /**
   * The accumulated energy (nJ) spent doing backups.
   */
  double energy_for_backups = 0.0;

  /**
   * The number of backups performed.
   */
  int num_backups = 0;

  /**
   * The total amount of energy spent on executing instructions.
   */
  double energy_for_instructions = 0.0;

  /**
   * Energy spent on executing instructions that were backed up.
   */
  double energy_forward_progress = 0.0;

  /**
   * The total CPU cycles spent on forward progress.
   */
  uint64_t time_forward_progress = 0u;

  /**
   * The total CPU cycles of this active period.
   */
  uint64_t time_cpu_total = 0u;
};

struct stats_bundle {
  system_stats system;
  cpu_stats cpu;

  /**
   * Model of multiple active periods.
   */
  std::deque<active_stats> models;
};
}

#endif //EH_SIM_STATS_HPP
