#include "simulate.hpp"

#include "capacitor.hpp"
#include "msp430_energy.hpp"
#include "voltage_trace.hpp"

#include <cstring>
#include <stdexcept>

#include <thumbulator/cpu.hpp>
#include <thumbulator/memory.hpp>
#include <iostream>

namespace ehsim {

void load_program(char const *file_name)
{
  std::FILE *fd = std::fopen(file_name, "r");
  if(fd == nullptr) {
    throw std::runtime_error("Could not open binary file.\n");
  }

  std::fread(&thumbulator::FLASH_MEMORY, sizeof(uint32_t),
      sizeof(thumbulator::FLASH_MEMORY) / sizeof(uint32_t), fd);
  std::fclose(fd);
}

void initialize_system(char const *binary_file)
{
  std::fprintf(stderr, "Simulating file %s\n", binary_file);
  std::fprintf(stderr, "Flash start:\t0x%8.8X\n", FLASH_START);
  std::fprintf(stderr, "Flash end:\t0x%8.8X\n", (FLASH_START + FLASH_SIZE));
  std::fprintf(stderr, "Ram start:\t0x%8.8X\n", RAM_START);
  std::fprintf(stderr, "Ram end:\t0x%8.8X\n", (RAM_START + RAM_SIZE));

  // Reset memory, then load program to memory
  std::memset(thumbulator::RAM, 0, sizeof(thumbulator::RAM));
  std::memset(thumbulator::FLASH_MEMORY, 0, sizeof(thumbulator::FLASH_MEMORY));
  load_program(binary_file);

  // Initialize CPU state
  thumbulator::cpu_reset();

  // PC seen is PC + 4
  thumbulator::cpu_set_pc(thumbulator::cpu_get_pc() + 0x4);
}

/**
 * Execute one instruction.
 *
 * @return Number of cycles to execute that instruction.
 */
uint32_t step_cpu()
{
  thumbulator::BRANCH_WAS_TAKEN = false;

  if((thumbulator::cpu_get_pc() & 0x1) == 0) {
    throw std::runtime_error("PC moved out of thumb mode.");
  }

  // fetch
  uint16_t instruction;
  thumbulator::fetch_instruction(thumbulator::cpu_get_pc() - 0x4, &instruction);
  // decode
  auto const decoded = thumbulator::decode(instruction);
  // execute, memory, and write-back
  uint32_t const instruction_ticks = thumbulator::exmemwb(instruction, &decoded);

  // advance to next PC
  if(!thumbulator::BRANCH_WAS_TAKEN) {
    thumbulator::cpu_set_pc(thumbulator::cpu_get_pc() + 0x2);
  } else {
    thumbulator::cpu_set_pc(thumbulator::cpu_get_pc() + 0x4);
  }

  return instruction_ticks;
}

/**
 * Calculate the energy stored in a capacitor.
 *
 * @param voltage The voltage across the capacitor.
 * @param capacitance The capacitance.
 *
 * @return Energy in nJ.
 */
double calculate_energy(double const voltage, double const capacitance)
{
  return 0.5 * capacitance * voltage * voltage * 1e9;
}

std::chrono::nanoseconds get_time(uint64_t const cycle_count)
{
  constexpr double CPU_PERIOD = 1.0 / thumbulator::CPU_FREQ;
  auto const time = static_cast<uint64_t>(CPU_PERIOD * cycle_count * 1e9);

  return std::chrono::nanoseconds(time);
}

std::chrono::milliseconds to_ms(std::chrono::nanoseconds const &time)
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(time);
}

double calculate_charging_rate(double voltage, double capacitance, double cycles_per_sample)
{
  auto const energy = calculate_energy(voltage, capacitance);
  return energy / cycles_per_sample;
}

stats_bundle simulate(char const *binary_file, char const *voltage_trace_file)
{
  using namespace std::chrono_literals;

  // stats tracking
  stats_bundle stats{};
  stats.system.time = 0ns;

  initialize_system(binary_file);

  // epsilon is the energy to execute an instruction as well as the energy to fetch that instruction.
  constexpr double EPSILON = MSP430_INSTRUCTION_ENERGY + MSP430_REG_FLASH;

  // energy harvesting
  voltage_trace power(voltage_trace_file);
  capacitor battery(4.7e-5);

  auto const cycles_per_sample =
      thumbulator::CPU_FREQ * std::chrono::duration<double>(power.sample_rate()).count();
  auto voltage = power.get_voltage(to_ms(stats.system.time));
  auto charging_rate = calculate_charging_rate(voltage, battery.capacitance(), cycles_per_sample);
  auto next_charge_time = power.sample_rate();

  // Execute the program
  // Simulation will terminate when it executes insn == 0xBFAA
  while(!thumbulator::EXIT_INSTRUCTION_ENCOUNTERED) {
    uint64_t elapsed_cycles = 1;

    if(battery.energy_stored() > EPSILON) {
      // active period
      elapsed_cycles = step_cpu();

      stats.cpu.instruction_count++;
      stats.cpu.cycle_count += elapsed_cycles;

      // consume energy for execution
      battery.consume_energy(EPSILON);
    }

    // harvest energy
    auto const harvested_energy = charging_rate * elapsed_cycles;
    battery.harvest_energy(harvested_energy);

    // track stats
    stats.system.time += get_time(elapsed_cycles);
    stats.system.energy_harvested += harvested_energy;

    if(stats.system.time >= next_charge_time) {
      next_charge_time += power.sample_rate();

      voltage = power.get_voltage(to_ms(stats.system.time));
      charging_rate = calculate_charging_rate(voltage, battery.capacitance(), cycles_per_sample);
    }
  }

  stats.system.energy_remaining = battery.energy_stored();

  return stats;
}
}