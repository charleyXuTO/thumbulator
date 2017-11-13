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

/**
 * Fetch an instruction from memory.
 *
 * @param address The address to fetch.
 * @param value The data in memory at that address.
 */
void fetch_instruction(uint32_t address, uint16_t *value);

/**
 * Load data from memory.
 *
 * @param address The address to load data from.
 * @param value The data in memory at that address.
 * @param falseRead true if this is a read due to anything other than the program.
 */
void load(uint32_t address, uint32_t *value, uint32_t falseRead);

/**
 * Store data into memory.
 *
 * @param address The address to store the data to.
 * @param value The data to store at that address.
 */
void store(uint32_t address, uint32_t value);

#define ENABLE_INSTRUCTION_TRACE 0
#define TRACE_INSTRUCTION(format, ...)                \
  do {                                                \
    if(ENABLE_INSTRUCTION_TRACE) {                    \
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

extern uint64_t cycleCount;
extern uint64_t insnCount;
extern uint32_t cyclesSinceReset;
extern uint32_t wdt_val;
extern uint32_t wdt_seed;

#endif
