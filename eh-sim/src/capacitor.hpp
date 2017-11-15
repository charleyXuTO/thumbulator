#ifndef EH_SIM_CAPACITOR_HPP
#define EH_SIM_CAPACITOR_HPP

#include <cmath>
#include <cassert>
#include <stdexcept>

namespace ehsim {

class capacitor {
public:
  /**
   * Create a capacitor.
   *
   * @param capacitance The capacitance.
   */
  explicit capacitor(double const capacitance) : C(capacitance), energy(0)
  {
  }

  double capacitance() const
  {
    return C;
  }

  double energy_stored() const
  {
    return energy;
  }

  /**
   * Consume energy from the capacitor.
   *
   * @param energy_to_consume The amount of energy to consume.
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
   * @param energy_harvested The amount of energy harvested.
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
