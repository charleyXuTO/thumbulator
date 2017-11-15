#include "voltage_trace.hpp"

#include <fstream>

namespace ehsim {
voltage_trace::voltage_trace(std::string const &path_to_trace) : maximum_time(0)
{
  std::ifstream trace(path_to_trace);

  uint64_t raw_time;
  uint64_t initial_time_offset;
  double voltage;
  if(trace >> raw_time >> voltage) {
    initial_time_offset = raw_time;
    voltages.emplace_back(voltage);

    while(trace >> raw_time >> voltage) {
      voltages.emplace_back(voltage);
    }

    maximum_time = std::chrono::milliseconds(raw_time - initial_time_offset + 1);
  }
}

double voltage_trace::get_voltage(std::chrono::milliseconds const &time) const
{
  auto const index = time.count() % maximum_time.count();
  return voltages[index];
}
}
