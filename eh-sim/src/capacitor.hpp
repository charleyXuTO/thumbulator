#ifndef EH_SIM_CAPACITOR_HPP
#define EH_SIM_CAPACITOR_HPP

#include <cmath>

namespace ehsim {

class capacitor {
public:
  /**
   * Create a capacitor.
   *
   * @param initial_voltage The initial voltage across the capacitor.
   * @param capacitance The capacitance.
   */
  capacitor(double const initial_voltage, double const capacitance)
      : V(initial_voltage), C(capacitance)
  {
  }

  /**
   * Consume energy from the capacitor.
   *
   * Doing so will decrease the voltage seen across the capacitor.
   *
   * @param energy_to_consume
   */
  void consume_energy(double const energy_to_consume)
  {
    // E = 1/2 * C * V^2
    double const current_energy = 0.5 * C * V * V;
    double const new_energy = current_energy - energy_to_consume;

    V = std::sqrt(2.0 * new_energy / C);
  }

  /**
   * Change the voltage across the capacitor.
   *
   * @param new_voltage The new voltage across the capacitor.
   */
  void change_voltage(double const new_voltage)
  {
    V = new_voltage;
  }

private:
  // voltage
  double V;
  // capacitance
  double const C;
};
}

#endif //EH_SIM_CAPACITOR_HPP
