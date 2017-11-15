#ifndef EH_SIM_SIMULATE_HPP
#define EH_SIM_SIMULATE_HPP

#include <cstdint>

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
   * Number of cycles ticked.
   */
  uint64_t cycle_count = 0u;

  /**
   * Amount of energy harvested (nJ).
   */
  double energy_harvested = 0.0;
};

struct stats_bundle {
  system_stats system;
  cpu_stats cpu;
};

stats_bundle simulate(char const *binary_file, char const *voltage_trace_file);

}

#endif //EH_SIM_SIMULATE_HPP
