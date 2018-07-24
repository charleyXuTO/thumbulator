#ifndef THUMBULATOR_SIM_SUPPORT_H
#define THUMBULATOR_SIM_SUPPORT_H

#include <cstdint>
#include <functional>

namespace thumbulator {

// TODO: for now only support 16-bit address, extend to 20-bits later

// reset vector location
#define RESET_VECTOR_START 0xFFFE
extern uint16_t RESET_VECTOR;

// interrupt vector table, not including the reset vector
#define INTERRUPT_VECTOR_START 0xFFE0
#define INTERRUPT_VECTOR_SIZE_ELEMENTS 15
#define INTERRUPT_VECTOR_ADDRESS_MASK(x) ((x - INTERRUPT_VECTOR_START)>>1)
extern uint16_t INTERRUPT_VECTOR_TABLE[INTERRUPT_VECTOR_SIZE_ELEMENTS];

// RAM starts from 0x00200
// use 0.5 KB for MSP430AFE253 for now
#define RAM_START 0x00200
#define RAM_SIZE_BYTES (1 << 9)
#define RAM_SIZE_ELEMENTS (RAM_SIZE_BYTES >> 1)
//#define RAM_ADDRESS_MASK (((~0) << 9) ^ (~0))
#define RAM_ADDRESS_MASK(x) ((x - RAM_START)>>1)

/**
 * Random-Access Memory, like SRAM.
 */
extern uint16_t RAM[RAM_SIZE_ELEMENTS];

/**
 * Hook into loads to RAM.
 *
 * The first parameter is the address.
 * The second parameter is the data that would be loaded.
 *
 * The function returns the data that will be loaded, potentially different than the second parameter.
 */
extern std::function<uint16_t(uint32_t, uint16_t)> ram_load_hook;

/**
 * Hook into stores to RAM.
 *
 * The first parameter is the address.
 * The second parameter is the value at the address before the store.
 * The third parameter is the desired value to store at the address.
 *
 * The function returns the data that will be stored, potentially different from the third parameter.
 */
extern std::function<uint16_t(uint32_t, uint16_t, uint16_t)> ram_store_hook;

// FLASH/ROM ends at 0x0FFDF
// use 16 KB for MSP430AFE253 for now
#define FLASH_END 0x0FFDF
#define FLASH_START 0x0BFE0
#define FLASH_SIZE_BYTES (1 << 14)
#define FLASH_SIZE_ELEMENTS (FLASH_SIZE_BYTES >> 1)
#define FLASH_ADDRESS_MASK(x) ((x - FLASH_START)>>1)
//(((~0) << 14) ^ (~0))

/**
 * Read-Only Memory.
 *
 * Typically used to store the application code.
 */
extern uint16_t FLASH_MEMORY[FLASH_SIZE_ELEMENTS];

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
 * @param false_read true if this is a read due to anything other than the program.
 */
void load(uint32_t address, uint16_t *value, uint16_t false_read);

/**
 * Store data into memory.
 *
 * @param address The address to store the data to.
 * @param value The data to store at that address.
 */
void store(uint32_t address, uint16_t value, bool isByte);
}

#endif
