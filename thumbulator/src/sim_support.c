#include "thumbulator/sim_support.h"

#include "cpu_flags.h"

#include <string.h>
#include <stdlib.h>

#define ESPR_T (1 << 24)

uint32_t RAM[RAM_SIZE >> 2];
uint32_t FLASH_MEMORY[FLASH_SIZE >> 2];
bool BRANCH_WAS_TAKEN = 0;
bool EXIT_INSTRUCTION_ENCOUNTERED = false;

void terminate_simulation(int exit_code)
{
  printf("Simulation was terminated prematurely.\n");
  exit(exit_code);
}

// Reset CPU state in accordance with B1.5.5 and B3.2.2
void cpu_reset(void)
{
  // Initialize the special-purpose registers
  cpu.apsr = 0;       // No flags set
  cpu.ipsr = 0;       // No exception number
  cpu.espr = ESPR_T;  // Thumb mode
  cpu.primask = 0;    // No except priority boosting
  cpu.control = 0;    // Priv mode and main stack
  cpu.sp_main = 0;    // Stack pointer for exception handling
  cpu.sp_process = 0; // Stack pointer for process

  // Clear the general purpose registers
  memset(cpu.gpr, 0, sizeof(cpu.gpr));

  // Set the reserved GPRs
  cpu.gpr[GPR_LR] = 0;

  // May need to add logic to send writes and reads to the
  // correct stack pointer
  // Set the stack pointers
  load(0, &cpu.sp_main, 0);
  cpu.sp_main &= 0xFFFFFFFC;
  cpu.sp_process = 0;
  cpu_set_sp(cpu.sp_main);

  // Set the program counter to the address of the reset exception vector
  uint32_t startAddr;
  load(0x4, &startAddr, 0);
  cpu_set_pc(startAddr);

  // No pending exceptions
  cpu.exceptmask = 0;

  // Check for attempts to go to ARM mode
  if((cpu_get_pc() & 0x1) == 0) {
    printf("Error: Reset PC to an ARM address 0x%08X\n", cpu_get_pc());
    terminate_simulation(1);
  }

  // Reset the SYSTICK unit
  SYSTICK.control = 0x4;
  SYSTICK.reload = 0x0;
  SYSTICK.value = 0x0;
  SYSTICK.calib = CPU_FREQ / 100 | 0x80000000;
}

// Memory access functions assume that RAM has a higher address than Flash
void fetch_instruction(uint32_t address, uint16_t *value)
{
  uint32_t fromMem;

  if(address >= RAM_START) {
    if(address >= (RAM_START + RAM_SIZE)) {
      fprintf(
          stderr, "Error: ILR Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      terminate_simulation(1);
    }

    fromMem = RAM[(address & RAM_ADDRESS_MASK) >> 2];
  } else {
    if(address >= (FLASH_START + FLASH_SIZE)) {
      fprintf(
          stderr, "Error: ILF Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      terminate_simulation(1);
    }

    fromMem = FLASH_MEMORY[(address & FLASH_ADDRESS_MASK) >> 2];
  }

  // Data 32-bits, but instruction 16-bits
  *value = ((address & 0x2) != 0) ? (uint16_t)(fromMem >> 16) : (uint16_t)fromMem;
}

void load(uint32_t address, uint32_t *value, uint32_t falseRead)
{
  if(address >= RAM_START) {
    if(address >= (RAM_START + RAM_SIZE)) {
      // Check for UART
      if(address == 0xE0000000) {
        *value = 0;
        return;
      }

      // Check for SYSTICK
      if((address >> 4) == 0xE000E01) {
        *value = ((uint32_t *)&SYSTICK)[(address >> 2) & 0x3];
        if(address == 0xE000E010)
          SYSTICK.control &= 0x00010000;

        return;
      }

      fprintf(
          stderr, "Error: DLR Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      terminate_simulation(1);
    }

    *value = RAM[(address & RAM_ADDRESS_MASK) >> 2];
  } else {
    if(address >= (FLASH_START + FLASH_SIZE)) {
      fprintf(
          stderr, "Error: DLF Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      terminate_simulation(1);
    }

    *value = FLASH_MEMORY[(address & FLASH_ADDRESS_MASK) >> 2];
  }
}

void store(uint32_t address, uint32_t value)
{
  if(address >= RAM_START) {
    if(address >= (RAM_START + RAM_SIZE)) {
      // Check for UART
      if(address == 0xE0000000) {
        return;
      }

      // Check for SYSTICK
      if((address >> 4) == 0xE000E01 && address != 0xE000E01C) {
        if(address == 0xE000E010) {
          SYSTICK.control = (value & 0x1FFFD) | 0x4; // No external tick source, no interrupt

          if(value & 0x2) {
            fprintf(stderr, "Warning: SYSTICK interrupts not implemented, ignoring\n");
          }
        } else if(address == 0xE000E014) {
          SYSTICK.reload = value & 0xFFFFFF;
        } else if(address == 0xE000E018) {
          // Reads clears current value
          SYSTICK.value = 0;
        }

        return;
      }

      fprintf(
          stderr, "Error: DSR Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      terminate_simulation(1);
    }

    RAM[(address & RAM_ADDRESS_MASK) >> 2] = value;
  } else {
    if(address >= (FLASH_START + FLASH_SIZE)) {
      fprintf(
          stderr, "Error: DSF Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      terminate_simulation(1);
    }

    FLASH_MEMORY[(address & FLASH_ADDRESS_MASK) >> 2] = value;
  }
}
