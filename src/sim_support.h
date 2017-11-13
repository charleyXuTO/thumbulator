#ifndef SIMSUPPORT_HEADER
#define SIMSUPPORT_HEADER

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define RAM_START 0x40000000
#define RAM_SIZE (1 << 23) // 8 MB
#define RAM_ADDRESS_MASK (((~0) << 23) ^ (~0))
#define FLASH_START 0x0
#define FLASH_SIZE (1 << 23) // 8 MB
#define FLASH_ADDRESS_MASK (((~0) << 23) ^ (~0))
#define CPU_FREQ 24000000
#define MEMMAPIO_START 0x80000000
#define MEMMAPIO_SIZE (4 * 19)

// Core CPU compenents
extern uint32_t ram[RAM_SIZE >> 2];
extern uint32_t flash[FLASH_SIZE >> 2];
extern bool takenBranch;   // Informs fetch that previous instruction caused a control flow change
extern void sim_exit(int); // All sim ends lead through here
void cpu_reset();          // Resets the CPU according to the specification
char simLoadInsn(uint32_t address,
    uint16_t *value); // All memory accesses one simulation starts should be through these interfaces
char simLoadData(uint32_t address, uint32_t *value);
char simLoadData_internal(uint32_t address,
    uint32_t *value,
    uint32_t falseRead); // falseRead says whether this is a read due to anything other than the program
char simStoreData(uint32_t address, uint32_t value);

// Simulator debugging
#define PRINT_INST 0 // diss_printf(): disassembly printing?
#define PRINT_ALL_STATE \
  0 // Print all registers after each instruction? Used for comparing to original Thumbulator.
#define PRINT_STATE_DIFF_INIT \
  (0 & (PRINT_ALL_STATE)) // Print changed registers after each instruction?
#define PRINT_STORES_WITH_STATE (0 & (PRINT_ALL_STATE)) // Print memory written with state updates?

// Simulator correctness checks: tradeoff speed for safety
#define VERIFY_BRANCHES_TAGGED 1 // Make sure that all control flow changes come from known paths
#define THUMB_CHECK 1            // Verify that the PC stays in thumb mode

#define diff_printf(format, ...)                    \
  do {                                              \
    fprintf(stderr, "%08X:\t", cpu_get_pc() - 0x5); \
    fprintf(stderr, format, __VA_ARGS__);           \
  } while(0)
#define diss_printf(format, ...)                      \
  do {                                                \
    if(PRINT_INST) {                                  \
      fprintf(stderr, "%08X:\t", cpu_get_pc() - 0x5); \
      fprintf(stderr, format, __VA_ARGS__);           \
    }                                                 \
  } while(0)

#define INCREMENT_CYCLES(x)     \
  {                             \
    cycleCount += (x);            \
    cyclesSinceReset += (x);      \
    cyclesSinceCP += (x);         \
    if(wdt_seed != 0) {         \
      wdt_val += (x);             \
      if(wdt_val >= wdt_seed) { \
        wdt_val = 0;            \
        cpu_set_except(16);     \
      }                         \
    }                           \
  }

struct ADDRESS_LIST {
  uint32_t address;
  struct ADDRESS_LIST *next;
};
typedef struct ADDRESS_LIST ADDRESS_LIST;

extern uint64_t cycleCount;
extern uint64_t insnCount;
extern uint32_t cyclesSinceReset;
extern uint32_t resetAfterCycles;
extern uint64_t wastedCycles;
extern uint32_t cyclesSinceCP;
extern uint32_t addrOfCP;
extern uint32_t addrOfRestoreCP;
extern uint32_t do_reset;
extern uint32_t wdt_val;
extern uint32_t wdt_seed;
extern uint32_t PRINT_STATE_DIFF;

#endif
