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

// Core CPU compenents
extern uint32_t ram[RAM_SIZE >> 2];
extern uint32_t flash[FLASH_SIZE >> 2];
// Informs fetch that previous instruction caused a control flow change
extern bool takenBranch;
/**
 * Whether or not the simulation should continue.
 */
extern bool simulate;
// All sim ends lead through here
extern void sim_exit(int);
// Resets the CPU according to the specification
void cpu_reset();
// All memory accesses one simulation starts should be through these interfaces
char simLoadInsn(uint32_t address, uint16_t *value);
char simLoadData(uint32_t address, uint32_t *value);
// falseRead says whether this is a read due to anything other than the program
char simLoadData_internal(uint32_t address, uint32_t *value, uint32_t falseRead);
char simStoreData(uint32_t address, uint32_t value);

// diss_printf(): disassembly printing?
#define PRINT_INST 0

// Simulator correctness checks: tradeoff speed for safety
#define VERIFY_BRANCHES_TAGGED 1 // Make sure that all control flow changes come from known paths
#define THUMB_CHECK 1            // Verify that the PC stays in thumb mode

#define diss_printf(format, ...)                      \
  do {                                                \
    if(PRINT_INST) {                                  \
      fprintf(stderr, "%08X:\t", cpu_get_pc() - 0x5); \
      fprintf(stderr, format, __VA_ARGS__);           \
    }                                                 \
  } while(0)

#define INCREMENT_CYCLES(x)     \
  {                             \
    cycleCount += (x);          \
    cyclesSinceReset += (x);    \
    if(wdt_seed != 0) {         \
      wdt_val += (x);           \
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
extern uint32_t wdt_val;
extern uint32_t wdt_seed;

#endif
