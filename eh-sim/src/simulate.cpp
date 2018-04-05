#include "simulate.hpp"

#include <thumbulator/cpu.hpp>
#include <thumbulator/memory.hpp>

#include "scheme/eh_scheme.hpp"
#include "capacitor.hpp"
#include "stats.hpp"
#include "voltage_trace.hpp"

#include <cstring>
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

std::chrono::milliseconds to_milliseconds(std::chrono::nanoseconds const &time)
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(time);
}

uint64_t time_to_cycles(std::chrono::nanoseconds elapsed_time, uint32_t clock_frequency)
{
  return static_cast<uint64_t>(
      ceil(clock_frequency * std::chrono::duration<double>(elapsed_time).count()));
}

// returns amount of energy per cycles
double calculate_charging_rate(double env_voltage, capacitor &battery, double cpu_freq)
{
  double energy_per_cycle = 0.0;

#ifdef MODEL_VOLTAGE_SOURCE
  // only charge if source voltage higher than current voltage across capacitor
  if(env_voltage > battery.voltage()) {
    // assume always max charging rate
    auto dV_dt = battery.max_current() / battery.capacitance();
    energy_per_cycle = dV_dt / cpu_freq;
  }
#else
  // voltage in the trace is measured across 30kohm resistor
  double current_source = env_voltage / 30000;
  auto dV_dt = current_source / battery.capacitance();
  energy_per_cycle = dV_dt / cpu_freq;

#endif

  return energy_per_cycle;
}

void ensure_forward_progress(int *no_progress_count, int num_backups, int threshold)
{
  if(num_backups == 0) {
    no_progress_count++;

    if(*no_progress_count >= threshold) {
      throw std::runtime_error("No forward progress made across multiple active periods.");
    }
  } else {
    *no_progress_count = 0;
  }
}

double update_energy_harvested(uint64_t elapsed_cycles,
    std::chrono::nanoseconds exec_end_time,
    double &charging_rate,
    double &env_voltage,
    std::chrono::nanoseconds &next_charge_time,
    uint32_t clock_freq,
    ehsim::voltage_trace const &power,
    capacitor &battery)
{
  // execution can span over more than 1 voltage trace sample period
  // accumulate charge of each periods separately
  auto potential_harvested_energy = 0.0;
  auto cycles_accounted = 0;
  while(exec_end_time >= next_charge_time) {
    uint64_t cycles_in_cur_charge_rate =
        elapsed_cycles - time_to_cycles(exec_end_time - next_charge_time, clock_freq);
    potential_harvested_energy += cycles_in_cur_charge_rate * charging_rate;
    cycles_accounted += cycles_in_cur_charge_rate;

    // move to next voltage sample
    next_charge_time += power.sample_period();
    env_voltage = power.get_voltage(to_milliseconds(next_charge_time));
    charging_rate = calculate_charging_rate(env_voltage, battery, clock_freq);
  }
  potential_harvested_energy += (elapsed_cycles - cycles_accounted) * charging_rate;

  // update battery -- battery may be full so ahe<=phe
  auto actual_harvested_energy = battery.harvest_energy(potential_harvested_energy);

  return actual_harvested_energy;
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

  initialize_system(binary_file);

  // energy harvesting
  auto &battery = scheme->get_battery();
  // start in power-off mode
  auto was_active = false;

  // frequency in Hz, sample period in ms
  auto cycles_per_sample = static_cast<uint64_t>(
      scheme->clock_frequency() * std::chrono::duration<double>(power.sample_period()).count());

  std::cout.setf(std::ios::unitbuf);
  std::cout << "cycles per sample: " << cycles_per_sample << "\n";

  // get voltage based current time (includes active+sleep) -- this should be @ time 0
  auto env_voltage = power.get_voltage(to_milliseconds(stats.system.time));
  auto charging_rate = calculate_charging_rate(env_voltage, battery, scheme->clock_frequency());
  auto next_charge_time = std::chrono::nanoseconds(power.sample_period());
  std::cout << "next_charge_time: " << next_charge_time.count() << "ns\n";

  uint64_t active_start = 0u;
  int no_progress_counter = 0;

  // Execute the program
  // Simulation will terminate when it executes insn == 0xBFAA
  while(!thumbulator::EXIT_INSTRUCTION_ENCOUNTERED) {
    uint64_t elapsed_cycles = 0;

    if(scheme->is_active(&stats)) {
      if(!was_active) {
        std::cout << "["
                  << std::chrono::duration_cast<std::chrono::nanoseconds>(stats.system.time).count()
                  << "ns - ";
        // allocate space for a new active period model
        stats.models.emplace_back();
        // track the time this active mode started
        active_start = stats.cpu.cycle_count;
        stats.models.back().energy_start = battery.energy_stored();

        if(stats.cpu.instruction_count != 0) {

          // restore state
          auto const restore_time = scheme->restore(&stats);
          elapsed_cycles += restore_time;

          stats.models.back().time_for_restores += restore_time;
        }
      }

      was_active = true;

      auto const instruction_ticks = step_cpu();

      stats.cpu.instruction_count++;
      stats.cpu.cycle_count += instruction_ticks;
      stats.models.back().time_for_instructions += instruction_ticks;
      elapsed_cycles += instruction_ticks;

      // consume energy for execution
      scheme->execute_instruction(&stats);

      if(scheme->will_backup(&stats)) {
        auto const backup_time = scheme->backup(&stats);
        elapsed_cycles += backup_time;

        auto &active_stats = stats.models.back();
        active_stats.time_for_backups += backup_time;
        active_stats.energy_forward_progress = active_stats.energy_for_instructions;
        active_stats.time_forward_progress = stats.cpu.cycle_count - active_start;
      }

      stats.system.time += get_time(elapsed_cycles, scheme->clock_frequency());

      if(always_harvest) {
        // update energy harvested & voltage sample corresponding to current time
        auto harvested_energy =
            update_energy_harvested(elapsed_cycles, stats.system.time, charging_rate, env_voltage,
                next_charge_time, scheme->clock_frequency(), power, battery);
        stats.system.energy_harvested += harvested_energy;
        stats.models.back().energy_charged += harvested_energy;
      } else {
        // just update voltage sample value
        if(stats.system.time >= next_charge_time) {
          while(stats.system.time >= next_charge_time) {
            next_charge_time += power.sample_period();
          }

          env_voltage = power.get_voltage(to_milliseconds(stats.system.time));
          charging_rate = calculate_charging_rate(env_voltage, battery, scheme->clock_frequency());
        }
      }
    } else { // powered off
      if(was_active) {
        std::cout << std::chrono::duration_cast<std::chrono::nanoseconds>(stats.system.time).count()
                  << "ns]\n";
        // we just powered off
        auto &active_period = stats.models.back();

        // ensure forward progress is being made, otherwise throw
        //ensure_forward_progress(&no_progress_counter, active_period.num_backups, 5);

        active_period.time_total = active_period.time_for_instructions +
                                   active_period.time_for_backups + active_period.time_for_restores;

        active_period.energy_consumed = active_period.energy_for_instructions +
                                        active_period.energy_for_backups +
                                        active_period.energy_for_restore;

        active_period.progress =
            active_period.energy_forward_progress / active_period.energy_consumed;
        active_period.eh_progress = scheme->estimate_progress(eh_model_parameters(active_period));
      }

      was_active = false;

      // figure out how long to be off for
      // move in steps of voltage sample (1ms)
      double min_energy = scheme->min_energy_to_power_on(&stats);
      double min_voltage = sqrt(2 * min_energy / battery.capacitance());

      // assume linear max dV/dt for now
      double max_dV_dt = battery.max_current() / battery.capacitance();
      double dV_dt_per_cycle = max_dV_dt / scheme->clock_frequency();
      uint64_t min_cycles =
          static_cast<uint64_t>(ceil((min_voltage - battery.voltage()) / dV_dt_per_cycle));

      auto time_until_next_charge = next_charge_time - stats.system.time;
      uint64_t cycles_until_next_charge =
          time_to_cycles(time_until_next_charge, scheme->clock_frequency());

      if(min_cycles > cycles_until_next_charge) {
        stats.system.time = next_charge_time;
        elapsed_cycles = cycles_until_next_charge;
      } else {
        elapsed_cycles = min_cycles;
        auto elapsed_time = std::chrono::nanoseconds(
            static_cast<uint64_t>(elapsed_cycles * scheme->clock_frequency() * 1e9));
        stats.system.time += elapsed_time;
      }

      // update energy harvested & voltage sample corresponding to current time
      auto harvested_energy = update_energy_harvested(elapsed_cycles, stats.system.time,
          charging_rate, env_voltage, next_charge_time, scheme->clock_frequency(), power, battery);
      stats.system.energy_harvested += harvested_energy;
    }
  }
  std::cout << "done\n";

  auto &active_period = stats.models.back();
  active_period.time_total = active_period.time_for_instructions + active_period.time_for_backups +
                             active_period.time_for_restores;

  active_period.energy_consumed = active_period.energy_for_instructions +
                                  active_period.energy_for_backups +
                                  active_period.energy_for_restore;

  active_period.progress = active_period.energy_forward_progress / active_period.energy_consumed;
  active_period.eh_progress = scheme->estimate_progress(eh_model_parameters(active_period));

  stats.system.energy_remaining = battery.energy_stored();

  return stats;
}
}
