#include <stdio.h>
#include <string.h>

#include "thumbulator/exception.h"
#include "thumbulator/exmemwb.h"

// Load a program into the simulator's RAM
static void fillState(const char *pFileName)
{
  FILE *fd;

  fd = fopen(pFileName, "r");
  if(fd == NULL) {
    fprintf(stderr, "Error: Could not open file %s\n", pFileName);
    terminate_simulation(1);
  }

  fread(&flash, sizeof(uint32_t), sizeof(flash) / sizeof(uint32_t), fd);

  fclose(fd);
}

int main(int argc, char *argv[])
{
  char *file = 0;

  if(argc < 2) {
    fprintf(stderr, "Usage: %s [-g] memory_file\n", argv[0]);
    return 1;
  }

  if(argc == 2)
    file = argv[1];
  else if(0 == strncmp("-g", argv[1], strlen("-g"))) {
    file = argv[2];
  }

  fprintf(stderr, "Simulating file %s\n", file);
  fprintf(stderr, "Flash start:\t0x%8.8X\n", FLASH_START);
  fprintf(stderr, "Flash end:\t0x%8.8X\n", (FLASH_START + FLASH_SIZE));
  fprintf(stderr, "Ram start:\t0x%8.8X\n", RAM_START);
  fprintf(stderr, "Ram end:\t0x%8.8X\n", (RAM_START + RAM_SIZE));

  // Reset memory, then load program to memory
  memset(ram, 0, sizeof(ram));
  memset(flash, 0, sizeof(flash));
  fillState(file);

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
    takenBranch = 0;

    // Backup CPU state
    struct CPU lastCPU;
    memcpy(&lastCPU, &cpu, sizeof(struct CPU));

    if((cpu_get_pc() & 0x1) == 0) {
      fprintf(stderr, "ERROR: PC moved out of thumb mode: %08X\n", (cpu_get_pc() - 0x4));
      terminate_simulation(1);
    }

    // fetch
    uint16_t insn;
    fetch_instruction(cpu_get_pc() - 0x4, &insn);
    // decode
    decode_result const decoded = decode(insn);
    // execute
    uint32_t const insnTicks = exwbmem(insn, decoded);

    // update statistics
    ++insnCount;
    cycleCount += insnTicks;

    if(cpu_get_except() != 0) {
      lastCPU.exceptmask = cpu.exceptmask;
      memcpy(&cpu, &lastCPU, sizeof(struct CPU));
      check_except();
    }

    // Hacky way to advance PC if no jumps
    if(!takenBranch) {
      if(cpu_get_pc() != lastCPU.gpr[15]) {
        fprintf(stderr, "Error: Break in control flow not accounted for\n");
        terminate_simulation(1);
      }

      cpu_set_pc(cpu_get_pc() + 0x2);
    } else {
      cpu_set_pc(cpu_get_pc() + 0x4);
    }
  }

  printf("Simulation finished in\n\t%u ticks\n\t%u instructions\n", cycleCount, insnCount);

  return 0;
}
