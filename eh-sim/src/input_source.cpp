#include "input_source.hpp"

#include <fstream>
#include <cmath>

namespace ehsim {

struct linear_interpolator {
  double x0;
  double x1;
  double y0;
  double y1;

  double operator()(double x) const
  {
    return ((y0 * (x1 - x) + y1 * (x - x0)) / (x1 - x0));
  }
};

input_source::input_source(std::string const &path_to_trace,
    double sample_period,
    double resistance)
    : m_period(sample_period), m_resistance(resistance)
{
  if(m_period <= 0) {
    throw std::runtime_error("The source's sample period must be greater than 0.");
  }

  if(m_resistance <= 0) {
    throw std::runtime_error("The source's resistance must be greater than 0.");
  }

  std::ifstream trace(path_to_trace);
  if(!trace.good()) {
    throw std::runtime_error("Could not load " + path_to_trace + ".");
  }

  // We ignore the trace's timestamps. This means we assume a start time of 0 and that all elements of the trace are
  // spaced evenly apart.
  std::uint64_t time;
  double voltage;
  while(trace >> time >> voltage) {
    if(voltage < 0.0) {
      throw std::runtime_error("A voltage reading in trace was negative.");
    }

    double const current = voltage / m_resistance;
    m_measurements.emplace_back(current);
  }
}

double input_source::voltage(double time) const
{
  return current(time) * m_resistance;
}

double input_source::current(double time) const
{
  // if requested time is past trace length, loop back
  if (time >= m_period * m_measurements.size()) {
    time = std::fmod(time, m_period * m_measurements.size());
  }
  linear_interpolator interpolator{};

  std::size_t index = calculate_index(time);
  interpolator.x0 = static_cast<double>(index) * m_period;
  interpolator.y0 = m_measurements[index];

  index = calculate_index(time + m_period);
  interpolator.x1 = interpolator.x0 + m_period;
  interpolator.y1 = m_measurements[index];

  return interpolator(time);
}

// takes in any timestamp and matches it to the last voltage read from trace before this time
std::size_t input_source::calculate_index(double time) const
{
  auto const index = static_cast<std::size_t>(time / m_period) % m_measurements.size();
  return index;
}
} // namespace ehsim