#ifndef EH_SIM_SIMULATE_HPP
#define EH_SIM_SIMULATE_HPP

#include <chrono>
#include <cstdint>

namespace ehsim {

class eh_scheme;
struct stats_bundle;
class voltage_trace;

stats_bundle simulate(char const *binary_file,
    ehsim::voltage_trace const &power,
    eh_scheme *scheme,
    bool always_harvest);
}

#endif //EH_SIM_SIMULATE_HPP
