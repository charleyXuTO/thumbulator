#ifndef EH_SIM_CAPACITOR_HPP
#define EH_SIM_CAPACITOR_HPP

#include <cmath>
#include <cassert>
#include <stdexcept>

namespace ehsim {

/**
 * Calculate the energy stored in a capacitor.
 *
 * @param voltage The voltage across the capacitor in volts (V).
 * @param capacitance The capacitance in farads (F).
 *
 * @return The energy in nJ.
 */
inline double calculate_energy(double const voltage, double const capacitance)
{
  return 0.5 * capacitance * voltage * voltage * 1e9;
}

class capacitor {
public:
  /**
   * Create a capacitor.
   *
   * @param capacitance The capacitance in farads (F).
   */
  explicit capacitor(double const capacitance) : C(capacitance), energy(0)
  {
  }

  /**
   * @return The capacitance in farads (F).
   */
  double capacitance() const
  {
    return C;
  }

  /**
   * @return The amount of stored energy in nJ.
   */
  double energy_stored() const
  {
    return energy;
  }

  /**
   * Consume energy from the capacitor.
   *
   * @param energy_to_consume The amount of energy to consume in nJ.
   */
  void consume_energy(double const energy_to_consume)
  {
    assert(energy_to_consume >= 0);
    assert(energy - energy_to_consume >= 0);

    energy -= energy_to_consume;
  }

  /**
   * Add energy to the capacitor.
   *
   * @param energy_harvested The amount of energy harvested in nJ.
   */
  void harvest_energy(double const energy_harvested)
  {
    assert(energy_harvested >= 0);

    energy += energy_harvested;
  }

private:
  // capacitance
  double const C;
  // stored energy
  double energy;
};
}

#endif //EH_SIM_CAPACITOR_HPP
