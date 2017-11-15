#ifndef EH_SIM_VOLTAGE_TRACE_HPP
#define EH_SIM_VOLTAGE_TRACE_HPP

#include <chrono>
#include <string>
#include <vector>

namespace ehsim {
class voltage_trace {
public:
  /**
   * Constructor.
   *
   * @param path_to_trace Path to an existing and valid trace file.
   * @param sample_rate The time between samples in the trace.
   */
  voltage_trace(std::string const &path_to_trace, std::chrono::microseconds const &sample_rate);

  /**
   * Get the voltage at the specified time.
   *
   * @param time The time in microseconds.
   *
   * @return The voltage reading at the specified time.
   */
  double get_voltage(std::chrono::microseconds const &time) const;

  std::chrono::microseconds sample_rate() const
  {
    return rate;
  }

private:
  std::chrono::microseconds rate;

  std::chrono::microseconds maximum_time;

  std::vector<double> voltages;
};
}

#endif //EH_SIM_VOLTAGE_TRACE_HPP
