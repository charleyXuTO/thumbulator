#include "capacitor.hpp"

#include <fstream>
#include <cmath>

namespace ehsim {

  capacitor::capacitor(double const capacitance, double const maximum_voltage, double const maximum_current)
      : m_capacitance(capacitance)
      , m_max_voltage(maximum_voltage)
      , m_max_current(maximum_current)
      , m_voltage(0)
      , m_max_energy(calculate_energy(maximum_voltage, m_capacitance))
      , m_energy(0)
  {

  }

  void capacitor::charge(double current, double time) {
    assert(current >= 0);

    m_voltage += (current * time) / m_capacitance;

    if (m_voltage > m_max_voltage) {
      throw std::runtime_error("The voltage across the capacitor has exceeded its voltage rating.");
    }
  }

  void capacitor::drain(double current, double time) {
    assert(current <= 0);

    // Voltage across the capacitor cannot be negative.
    m_voltage = std::min(0.0, m_voltage + ((current * time) / m_capacitance));
  }

  void capacitor::consume_energy(double const energy_to_consume) {
    assert(energy_to_consume >= 0);
    assert(m_energy - energy_to_consume >= 0);

    m_energy -= energy_to_consume;
    update_voltage();
  }

  double capacitor::harvest_energy(double const energy_harvested) {
    assert(energy_harvested >= 0);

    double can_harvest = energy_harvested;
    if (m_energy + energy_harvested > m_max_energy) {
      can_harvest = m_max_energy- m_energy;
    }

    // clamp to maximum energy, avoiding floating point precision errors
    m_energy = std::min(m_energy + can_harvest, m_max_energy);
    update_voltage();

    return can_harvest;
  }
}