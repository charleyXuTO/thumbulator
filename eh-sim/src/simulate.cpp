#include "simulate.hpp"

#include "voltage_trace.hpp"

#include <cstdio>
#include <cstring>
#include <stdexcept>

extern "C" {
#include <thumbulator/cpu.h>
#include <thumbulator/exception.h>
#include <thumbulator/sim_support.h>
}

namespace ehsim {

void load_program(char const *file_name)
{
  std::FILE *fd = std::fopen(file_name, "r");
  if(fd == nullptr) {
    throw std::runtime_error("Could not open binary file.\n");
  }

  std::fread(&FLASH_MEMORY, sizeof(uint32_t), sizeof(FLASH_MEMORY) / sizeof(uint32_t), fd);
  std::fclose(fd);
}

stats_bundle simulate(char const *binary_file, char const *voltage_trace_file)
{
  std::fprintf(stderr, "Simulating file %s\n", binary_file);
  std::fprintf(stderr, "Flash start:\t0x%8.8X\n", FLASH_START);
  std::fprintf(stderr, "Flash end:\t0x%8.8X\n", (FLASH_START + FLASH_SIZE));
  std::fprintf(stderr, "Ram start:\t0x%8.8X\n", RAM_START);
  std::fprintf(stderr, "Ram end:\t0x%8.8X\n", (RAM_START + RAM_SIZE));

  // Reset memory, then load program to memory
  std::memset(RAM, 0, sizeof(RAM));
  std::memset(FLASH_MEMORY, 0, sizeof(FLASH_MEMORY));
  load_program(binary_file);

  // Initialize CPU state
  cpu_reset();

  // PC seen is PC + 4
  cpu_set_pc(cpu_get_pc() + 0x4);

  // stats tracking
  stats_bundle stats{};

  voltage_trace voltages(voltage_trace_file);

  // Execute the program
  // Simulation will terminate when it executes insn == 0xBFAA
  while(!EXIT_INSTRUCTION_ENCOUNTERED) {
    BRANCH_WAS_TAKEN = false;

    // Backup CPU state
    cpu_state lastCPU{};
    std::memcpy(&lastCPU, &cpu, sizeof(cpu_state));

    if((cpu_get_pc() & 0x1) == 0) {
      throw std::runtime_error("PC moved out of thumb mode.");
    }

    // fetch
    uint16_t instruction;
    fetch_instruction(cpu_get_pc() - 0x4, &instruction);
    // decode
    decode_result const decoded = decode(instruction);
    // execute, memory, and write-back
    uint32_t const instruction_ticks = exmemwb(instruction, &decoded);

    // update statistics
    stats.cpu.instruction_count++;
    stats.cpu.cycle_count += instruction_ticks;

    if(cpu_get_exception() != 0) {
      lastCPU.exceptmask = cpu.exceptmask;
      std::memcpy(&cpu, &lastCPU, sizeof(cpu_state));
      check_except();
    }

    if(!BRANCH_WAS_TAKEN) {
      if(cpu_get_pc() != lastCPU.gpr[15]) {
        throw std::runtime_error("Unexpected control flow change.");
      }

      cpu_set_pc(cpu_get_pc() + 0x2);
    } else {
      cpu_set_pc(cpu_get_pc() + 0x4);
    }
  }

  return stats;
}
}