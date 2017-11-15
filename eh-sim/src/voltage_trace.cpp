#include "voltage_trace.hpp"

#include <fstream>

namespace ehsim {
voltage_trace::voltage_trace(std::string const &path_to_trace, std::chrono::microseconds const &sample_rate)
  : maximum_time(0), rate(sample_rate)
{
  std::ifstream trace(path_to_trace);

  uint64_t raw_time;
  double voltage;
  if(trace >> raw_time >> voltage) {
    voltages.emplace_back(voltage);

    while(trace >> raw_time >> voltage) {
      voltages.emplace_back(voltage);
    }

    maximum_time = std::chrono::microseconds(voltages.size());
  }
}

double voltage_trace::get_voltage(std::chrono::microseconds const &time) const
{
  auto const index = time.count() % maximum_time.count();
  return voltages[index];
}
}
