#ifndef EH_SIM_SIMULATE_HPP
#define EH_SIM_SIMULATE_HPP

#include <cstdint>

namespace ehsim {

struct cpu_stats {
  /**
   * Number of executed instructions.
   */
  uint64_t instruction_count;

  /**
   * Number of cycles ticked.
   */
  uint64_t cycle_count;
};

struct stats_bundle {
  cpu_stats cpu;
};

stats_bundle simulate(char const *binary_file, char const *voltage_trace_file);

}

#endif //EH_SIM_SIMULATE_HPP
