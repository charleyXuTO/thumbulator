#include "voltage_trace.hpp"

#include <fstream>

namespace ehsim {
voltage_trace::voltage_trace(std::string const &path_to_trace) : maximum_time(0)
{
  std::ifstream trace(path_to_trace);

  int time;
  int initial_time_offset;
  double voltage;
  if(trace >> time >> voltage) {
    initial_time_offset = time;
    voltages.emplace_back(voltage);

    while(trace >> time >> voltage) {
      voltages.emplace_back(voltage);
    }

    maximum_time = time - initial_time_offset + 1;
  }
}

double voltage_trace::get_voltage(int time) const
{
  time = time % maximum_time;

  return voltages[time];
}
}
