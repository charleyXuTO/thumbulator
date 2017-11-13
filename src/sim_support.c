#include "sim_support.h"

#include "exmemwb.h"
#include <string.h>

uint64_t cycleCount = 0;
uint64_t insnCount = 0;
uint32_t cyclesSinceReset = 0;
uint32_t wdt_seed = 0;
uint32_t wdt_val = 0;
uint32_t ram[RAM_SIZE >> 2];
uint32_t flash[FLASH_SIZE >> 2];
bool takenBranch = 0;
bool simulate = true;

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

  // Reset the systick unit
  systick.control = 0x4;
  systick.reload = 0x0;
  systick.value = 0x0;
  systick.calib = CPU_FREQ / 100 | 0x80000000;

  // Reset counters
  cyclesSinceReset = 0;
  wdt_val = 0;
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

    fromMem = ram[(address & RAM_ADDRESS_MASK) >> 2];
  } else {
    if(address >= (FLASH_START + FLASH_SIZE)) {
      fprintf(
          stderr, "Error: ILF Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      terminate_simulation(1);
    }

    fromMem = flash[(address & FLASH_ADDRESS_MASK) >> 2];
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

      // Check for systick
      if((address >> 4) == 0xE000E01) {
        *value = ((uint32_t *)&systick)[(address >> 2) & 0x3];
        if(address == 0xE000E010)
          systick.control &= 0x00010000;

        return;
      }

      fprintf(
          stderr, "Error: DLR Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      terminate_simulation(1);
    }

    *value = ram[(address & RAM_ADDRESS_MASK) >> 2];
  } else {
    if(address >= (FLASH_START + FLASH_SIZE)) {
      fprintf(
          stderr, "Error: DLF Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      terminate_simulation(1);
    }

    *value = flash[(address & FLASH_ADDRESS_MASK) >> 2];
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

      // Check for systick
      if((address >> 4) == 0xE000E01 && address != 0xE000E01C) {
        if(address == 0xE000E010) {
          systick.control = (value & 0x1FFFD) | 0x4; // No external tick source, no interrupt

          if(value & 0x2) {
            fprintf(stderr, "Warning: SYSTICK interrupts not implemented, ignoring\n");
          }
        } else if(address == 0xE000E014) {
          systick.reload = value & 0xFFFFFF;
        } else if(address == 0xE000E018) {
          // Reads clears current value
          systick.value = 0;
        }

        return;
      }

      fprintf(
          stderr, "Error: DSR Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      terminate_simulation(1);
    }

    ram[(address & RAM_ADDRESS_MASK) >> 2] = value;
  } else {
    if(address >= (FLASH_START + FLASH_SIZE)) {
      fprintf(
          stderr, "Error: DSF Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      terminate_simulation(1);
    }

    flash[(address & FLASH_ADDRESS_MASK) >> 2] = value;
  }
}
