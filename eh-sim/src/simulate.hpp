#ifndef EH_SIM_SIMULATE_HPP
#define EH_SIM_SIMULATE_HPP

#include <chrono>
#include <cstdint>

namespace ehsim {

class eh_scheme;
struct stats_bundle;

stats_bundle
simulate(char const *binary_file, char const *voltage_trace_file, eh_scheme *scheme);
}

#endif //EH_SIM_SIMULATE_HPP
