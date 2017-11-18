#include "simulate.hpp"

#include <thumbulator/cpu.hpp>
#include <thumbulator/memory.hpp>

#include "scheme/eh_scheme.hpp"
#include "capacitor.hpp"
#include "stats.hpp"
#include "voltage_trace.hpp"

#include <cstring>

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
    printf("Oh no! Current PC: 0x%08X\n", thumbulator::cpu.gpr[15]);
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

std::chrono::nanoseconds get_time(uint64_t const cycle_count, uint32_t const frequency)
{
  double const CPU_PERIOD = 1.0 / frequency;
  auto const time = static_cast<uint64_t>(CPU_PERIOD * cycle_count * 1e9);

  return std::chrono::nanoseconds(time);
}

std::chrono::microseconds to_microseconds(std::chrono::nanoseconds const &time)
{
  return std::chrono::duration_cast<std::chrono::microseconds>(time);
}

double calculate_charging_rate(double voltage, double capacitance, double cycles_per_sample)
{
  auto const energy = calculate_energy(voltage, capacitance);
  return energy / cycles_per_sample;
}

stats_bundle simulate(char const *binary_file,
    ehsim::voltage_trace const &power,
    eh_scheme *scheme,
    bool always_harvest)
{
  using namespace std::chrono_literals;

  // stats tracking
  stats_bundle stats{};
  stats.system.time = 0ns;
  stats.models.emplace_back();

  initialize_system(binary_file);

  // energy harvesting
  auto &battery = scheme->get_battery();
  // start in power-off mode
  auto was_active = false;

  auto const cycles_per_sample =
      scheme->clock_frequency() * std::chrono::duration<double>(power.sample_rate()).count();
  auto voltage = power.get_voltage(to_microseconds(stats.system.time));
  auto charging_rate = calculate_charging_rate(voltage, battery.capacitance(), cycles_per_sample);
  auto next_charge_time = power.sample_rate();

  // Execute the program
  // Simulation will terminate when it executes insn == 0xBFAA
  while(!thumbulator::EXIT_INSTRUCTION_ENCOUNTERED) {
    uint64_t elapsed_cycles = 0;

    if(scheme->is_active(&stats)) {
      if(!was_active && stats.cpu.instruction_count != 0) {
        // we have just transitioned to an active mode
        elapsed_cycles += scheme->restore(&stats);
      }

      was_active = true;

      auto const instruction_ticks = step_cpu();

      stats.cpu.instruction_count++;
      stats.cpu.cycle_count += instruction_ticks;
      elapsed_cycles += instruction_ticks;

      // consume energy for execution
      scheme->execute_instruction(&stats);

      if(scheme->will_backup(&stats)) {
        elapsed_cycles += scheme->backup(&stats);

        stats.models.back().energy_forward_progress = stats.models.back().energy_for_instructions;
      }
    } else {
      was_active = false;
      elapsed_cycles = 1;
    }

    stats.system.time += get_time(elapsed_cycles, scheme->clock_frequency());

    if(always_harvest || !was_active) {
      // harvest energy
      auto const harvested_energy = charging_rate * elapsed_cycles;
      battery.harvest_energy(harvested_energy);

      stats.system.energy_harvested += harvested_energy;
    }

    if(stats.system.time >= next_charge_time) {
      next_charge_time += power.sample_rate();

      voltage = power.get_voltage(to_microseconds(stats.system.time));
      charging_rate = calculate_charging_rate(voltage, battery.capacitance(), cycles_per_sample);
    }
  }

  stats.system.energy_remaining = battery.energy_stored();

  return stats;
}
}