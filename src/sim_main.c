#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "except.h"
#include "exmemwb.h"

// Load a program into the simulator's RAM
static void fillState(const char *pFileName)
{
  FILE *fd;

  fd = fopen(pFileName, "r");
  if(fd == NULL) {
    fprintf(stderr, "Error: Could not open file %s\n", pFileName);
    sim_exit(1);
  }

  fread(&flash, sizeof(uint32_t), sizeof(flash) / sizeof(uint32_t), fd);

  fclose(fd);
}

struct CPU cpu;
struct SYSTICK systick;

void sim_exit(int i)
{
  printf("Simulation terminated after\n\t%llu ticks\n\t%llu instructions\n", cycleCount, insnCount);
  exit(i);
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

  // Execute the program
  // Simulation will terminate when it executes insn == 0xBFAA
  bool addToWasted = 0;
  while(simulate) {
    struct CPU lastCPU;

    uint16_t insn;
    takenBranch = 0;

    // Backup CPU state
    //if(PRINT_STATE_DIFF)
    memcpy(&lastCPU, &cpu, sizeof(struct CPU));

#if THUMB_CHECK
    if((cpu_get_pc() & 0x1) == 0) {
      fprintf(stderr, "ERROR: PC moved out of thumb mode: %08X\n", (cpu_get_pc() - 0x4));
      sim_exit(1);
    }
#endif

    simLoadInsn(cpu_get_pc() - 0x4, &insn);
    diss_printf("%04X\n", insn);

    DECODE_RESULT decoded = decode(insn);
    exwbmem(insn, decoded);

    if(cpu_get_except() != 0) {
      lastCPU.exceptmask = cpu.exceptmask;
      memcpy(&cpu, &lastCPU, sizeof(struct CPU));
      check_except();
    }

    // Hacky way to advance PC if no jumps
    if(!takenBranch) {
#if VERIFY_BRANCHES_TAGGED
      if(cpu_get_pc() != lastCPU.gpr[15]) {
        fprintf(stderr, "Error: Break in control flow not accounted for\n");
        sim_exit(1);
      }
#endif
      cpu_set_pc(cpu_get_pc() + 0x2);
    } else
      cpu_set_pc(cpu_get_pc() + 0x4);

    // Increment counters
    if(((cpu_get_pc() - 6) & 0xfffffffe) == addrOfCP)
      cyclesSinceCP = 0;

    unsigned cp_addr = (cpu.gpr[15] - 4) & (~0x1);
    switch(cp_addr) {
    case 0x000000d8:
    case 0x000000f0:
    case 0x00000102:
    case 0x00000116:
    case 0x0000012a:
    case 0x0000013e:
    case 0x00000152:
    case 0x00000168:
    case 0x00000180:
    case 0x0000019c:
      break;
    default:
      break;
    }

    if(addToWasted) {
      addToWasted = 0;
      wastedCycles += cyclesSinceCP;
      cyclesSinceCP = 0;
    }

    if(((cpu_get_pc() - 4) & 0xfffffffe) == addrOfRestoreCP)
      addToWasted = 1;
  }

  printf("Simulation finished in\n\t%llu ticks\n\t%llu instructions\n", cycleCount, insnCount);

  return 0;
}
