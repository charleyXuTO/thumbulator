#include <stdlib.h>
#include <string.h>

#include "sim_support.h"
#include "exmemwb.h"

uint64_t cycleCount = 0;
uint64_t insnCount = 0;
uint64_t wastedCycles = 0;
uint32_t cyclesSinceReset = 0;
uint32_t cyclesSinceCP = 0;
uint32_t resetAfterCycles = 0;
uint32_t addrOfCP = 0;
uint32_t addrOfRestoreCP = 0;
uint32_t do_reset = 0;
uint32_t wdt_seed = 0;
uint32_t wdt_val = 0;
uint32_t md5[4] = {0, 0, 0, 0, 0};
uint32_t PRINT_STATE_DIFF = PRINT_STATE_DIFF_INIT;
uint32_t ram[RAM_SIZE >> 2];
uint32_t flash[FLASH_SIZE >> 2];
bool takenBranch = 0;

// Reserve a space inside the simulator for variables that GDB and python can use to control the simulator
// Essentially creates a new block of addresses on the bus of the processor that only the debug read and write commands can access
//MEMMAPIO mmio = {.cycleCountLSB = &cycleCount, .cycleCountMSB = &cycleCount+4,
//                 .cyclesSince = &cyclesSinceReset, .resetAfter = &resetAfterCycles};
uint32_t *mmio[] = {&cycleCount, ((uint32_t *)&cycleCount) + 1, &wastedCycles,
    ((uint32_t *)&wastedCycles) + 1, &cyclesSinceReset, &cyclesSinceCP, &addrOfCP, &addrOfRestoreCP,
    &resetAfterCycles, &do_reset, &PRINT_STATE_DIFF, &wdt_seed, &wdt_val, &(md5[0]), &(md5[1]),
    &(md5[2]), &(md5[3]), &(md5[4])};

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
  simLoadData(0, &cpu.sp_main);
  cpu.sp_main &= 0xFFFFFFFC;
  cpu.sp_process = 0;
  cpu_set_sp(cpu.sp_main);

  // Set the program counter to the address of the reset exception vector
  uint32_t startAddr;
  simLoadData(0x4, &startAddr);
  cpu_set_pc(startAddr);

  // No pending exceptions
  cpu.exceptmask = 0;

  // Check for attempts to go to ARM mode
  if((cpu_get_pc() & 0x1) == 0) {
    printf("Error: Reset PC to an ARM address 0x%08X\n", cpu_get_pc());
    sim_exit(1);
  }

  // Reset the systick unit
  systick.control = 0x4;
  systick.reload = 0x0;
  systick.value = 0x0;
  systick.calib = CPU_FREQ / 100 | 0x80000000;

  // Reset counters
  wastedCycles += cyclesSinceCP;
  cyclesSinceReset = 0;
  cyclesSinceCP = 0;
  wdt_val = 0;
}

// Returns 1 if the passed list of addresses contains the passed
// address, returns 0 otherwise
char containsAddress(const ADDRESS_LIST *pList, const uint32_t pAddress)
{
  if(pList->address == pAddress)
    return 1;

  if(pList->next == NULL)
    return 0;

  // Tail recursion FTW
  return containsAddress(pList->next, pAddress);
}

// Adds the passed address to the end of the list
// Returns 0 if already present, 1 if added to end
char addAddress(const ADDRESS_LIST *pList, const uint32_t pAddress)
{
  ADDRESS_LIST *temp = pList;
  ADDRESS_LIST *temp_prev;

  // Go to the end of the list
  do {
    if(temp->address == pAddress)
      return 0;
    temp_prev = temp;
    temp = temp->next;
  } while(temp != NULL);

  // Create a new entry and link to it
  temp_prev->next = malloc(sizeof(ADDRESS_LIST));
  temp_prev->next->address = pAddress;
  temp_prev->next->next = NULL;

  return 1;
}

// Clears the list
void clearList(ADDRESS_LIST *pList)
{
  if(pList->next == NULL)
    return;

  clearList(pList->next);

  free(pList->next);

  pList->next = NULL;
}

// Memory access functions assume that RAM has a higher address than Flash
char simLoadInsn(uint32_t address, uint16_t *value)
{
  uint32_t fromMem;

  if(address >= RAM_START) {
    if(address >= (RAM_START + RAM_SIZE)) {
      fprintf(
          stderr, "Error: ILR Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      sim_exit(1);
    }

    fromMem = ram[(address & RAM_ADDRESS_MASK) >> 2];
  } else {
    if(address >= (FLASH_START + FLASH_SIZE)) {
      fprintf(
          stderr, "Error: ILF Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      sim_exit(1);
    }

    fromMem = flash[(address & FLASH_ADDRESS_MASK) >> 2];
  }

  // Data 32-bits, but instruction 16-bits
  *value = ((address & 0x2) != 0) ? (uint16_t)(fromMem >> 16) : (uint16_t)fromMem;

  return 0;
}

// Normal interface for a program to load from memory
// Increments load counter
char simLoadData(uint32_t address, uint32_t *value)
{
  return simLoadData_internal(address, value, 0);
}

char simLoadData_internal(uint32_t address, uint32_t *value, uint32_t falseRead)
{
  if(address >= RAM_START) {
    if(address >= (RAM_START + RAM_SIZE)) {
      // Check for UART
      if(address == 0xE0000000) {
        *value = 0;
        return 0;
      }

      // Check for systick
      if((address >> 4) == 0xE000E01) {
        *value = ((uint32_t *)&systick)[(address >> 2) & 0x3];
        if(address == 0xE000E010)
          systick.control &= 0x00010000;

        return 0;
      }

      fprintf(
          stderr, "Error: DLR Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      sim_exit(1);
    }

    *value = ram[(address & RAM_ADDRESS_MASK) >> 2];
  } else {
    if(address >= (FLASH_START + FLASH_SIZE)) {
      fprintf(
          stderr, "Error: DLF Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      sim_exit(1);
    }

    *value = flash[(address & FLASH_ADDRESS_MASK) >> 2];
  }

  return 0;
}

char simStoreData(uint32_t address, uint32_t value)
{
  unsigned int word;

  if(address >= RAM_START) {
    if(address >= (RAM_START + RAM_SIZE)) {
      // Check for UART
      if(address == 0xE0000000) {
        return 0;
      }

      // Check for systick
      if((address >> 4) == 0xE000E01 && address != 0xE000E01C) {
        if(address == 0xE000E010) {
          systick.control = (value & 0x1FFFD) | 0x4; // No external tick source, no interrupt
          if(value & 0x2)
            fprintf(stderr, "ERROR: SYSTICK interrupts not implemented...ignoring\n");
        } else if(address == 0xE000E014)
          systick.reload = value & 0xFFFFFF;
        else if(address == 0xE000E018)
          systick.value = 0; // Reads clears current value

        return 0;
      }

      // Check for cycle count
      if(address >= MEMMAPIO_START && address <= (MEMMAPIO_START + MEMMAPIO_SIZE)) {
        word = *(mmio[((address & 0xfffffffc) - MEMMAPIO_START >> 2)]);
        word &= ~(0xff << (8 * (address % 4)));
        word |= (value << (8 * (address % 4)));
        *(mmio[((address & 0xfffffffc) - MEMMAPIO_START >> 2)]) = word;

        // If the variable updated is a request to reset, then do it
        if(do_reset != 0) {
          cpu_reset();
          cpu_set_pc(cpu_get_pc() + 0x4);
          do_reset = 0;
        }

        return 0;
      }

      fprintf(
          stderr, "Error: DSR Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      sim_exit(1);
    }

    ram[(address & RAM_ADDRESS_MASK) >> 2] = value;
  } else {
    if(address >= (FLASH_START + FLASH_SIZE)) {
      fprintf(
          stderr, "Error: DSF Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      sim_exit(1);
    }

    flash[(address & FLASH_ADDRESS_MASK) >> 2] = value;
  }

  return 0;
}
