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
   */
  explicit voltage_trace(std::string const &path_to_trace);

  /**
   * Get the voltage at the specified time.
   *
   * @param time The time in milliseconds.
   *
   * @return The voltage reading at the specified time.
   */
  double get_voltage(std::chrono::milliseconds const &time) const;

  std::chrono::milliseconds sample_rate() const
  {
    return std::chrono::milliseconds(1);
  }

private:
  std::chrono::milliseconds maximum_time;

  std::vector<double> voltages;
};
}

#endif //EH_SIM_VOLTAGE_TRACE_HPP
