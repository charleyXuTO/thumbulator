#include "simulate.hpp"

#include <cstdio>
#include <cstring>

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
    std::fprintf(stderr, "Error: Could not open file %s\n", file_name);
    terminate_simulation(1);
  }

  std::fread(&FLASH_MEMORY, sizeof(uint32_t), sizeof(FLASH_MEMORY) / sizeof(uint32_t), fd);

  std::fclose(fd);
}

void simulate(char const *binary_file)
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
  uint32_t insnCount = 0;
  uint32_t cycleCount = 0;

  // Execute the program
  // Simulation will terminate when it executes insn == 0xBFAA
  while(simulate) {
    BRANCH_WAS_TAKEN = false;

    // Backup CPU state
    cpu_state lastCPU{};
    std::memcpy(&lastCPU, &cpu, sizeof(cpu_state));

    if((cpu_get_pc() & 0x1) == 0) {
      std::fprintf(stderr, "ERROR: PC moved out of thumb mode: %08X\n", (cpu_get_pc() - 0x4));
      terminate_simulation(1);
    }

    // fetch
    uint16_t insn;
    fetch_instruction(cpu_get_pc() - 0x4, &insn);
    // decode
    decode_result const decoded = decode(insn);
    // execute
    uint32_t const insnTicks = exmemwb(insn, &decoded);

    // update statistics
    ++insnCount;
    cycleCount += insnTicks;

    if(cpu_get_exception() != 0) {
      lastCPU.exceptmask = cpu.exceptmask;
      std::memcpy(&cpu, &lastCPU, sizeof(cpu_state));
      check_except();
    }

    // Hacky way to advance PC if no jumps
    if(!BRANCH_WAS_TAKEN) {
      if(cpu_get_pc() != lastCPU.gpr[15]) {
        std::fprintf(stderr, "Error: Break in control flow not accounted for\n");
        terminate_simulation(1);
      }

      cpu_set_pc(cpu_get_pc() + 0x2);
    } else {
      cpu_set_pc(cpu_get_pc() + 0x4);
    }
  }

  std::printf("Simulation finished in\n\t%u ticks\n\t%u instructions\n", cycleCount, insnCount);
}
}