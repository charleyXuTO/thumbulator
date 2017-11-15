#ifndef EH_SIM_STATS_HPP
#define EH_SIM_STATS_HPP

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

struct stats_bundle {
  system_stats system;
  cpu_stats cpu;
};
}

#endif //EH_SIM_STATS_HPP
