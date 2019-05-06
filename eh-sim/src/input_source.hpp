
#ifndef EH_SIM_INPUT_SOURCE_HPP
#define EH_SIM_INPUT_SOURCE_HPP

#include <string>
#include <vector>

namespace ehsim {

/**
 * Models a voltage or current source.
 */
class input_source {
public:
  /**
    * Constructor.
    *
    * Creates a current source based on a trace of voltages measured across a resistor.
    *
    * @param path_to_trace The path to the voltage trace file.
    * @param sample_period The time (seconds) between samples in the trace.
    * @param resistance The resistor (Ohms) the voltage was measured across.
    */
  input_source(std::string const &path_to_trace, double sample_period, double resistance);

  /**
    * Get the time (seconds) between samples in the trace.
    */
  double sample_period() const
  {
    return m_period;
  }

  /**
    * Get the voltage (Volts) at the specified time (seconds).
    */
  double voltage(double time) const;

  /**
    * Get the current (Amperes) at the specified time (seconds).
    */
  double current(double time) const;

private:
  double m_period;

  double m_resistance;

  std::vector<double> m_measurements;

private:
  std::size_t calculate_index(double time) const;
};
} // namespace ehsim

#endif //ENERGY_HARVESTING_INPUT_SOURCE_HPP
