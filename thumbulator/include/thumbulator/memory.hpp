#ifndef THUMBULATOR_SIM_SUPPORT_H
#define THUMBULATOR_SIM_SUPPORT_H

#include <cstdint>

namespace thumbulator {

#define RAM_START 0x40000000
#define RAM_SIZE_BYTES (1 << 23) // 8 MB
#define RAM_SIZE_ELEMENTS (RAM_SIZE_BYTES >> 2)
#define RAM_ADDRESS_MASK (((~0) << 23) ^ (~0))

/**
 * Random-Access Memory, like SRAM.
 */
extern uint32_t RAM[RAM_SIZE_ELEMENTS];

/**
 * Hook into loads to RAM.
 *
 * The first parameter is the address.
 * The second parameter is the data that would be loaded.
 *
 * The function returns the data that will be loaded, potentially different than the second parameter.
 */
using ram_load_hook_function = uint32_t (*)(uint32_t, uint32_t);
extern ram_load_hook_function ram_load_hook;

/**
 * Hook into stores to RAM.
 *
 * The first parameter is the address.
 * The second parameter is the value at the address before the store.
 * The third parameter is the desired value to store at the address.
 *
 * The function returns the data that will be stored, potentially different from the third parameter.
 */
using ram_store_hook_function = uint32_t (*)(uint32_t, uint32_t, uint32_t);
extern ram_store_hook_function ram_store_hook;

#define FLASH_START 0x0
#define FLASH_SIZE_BYTES (1 << 23) // 8 MB
#define FLASH_SIZE_ELEMENTS (FLASH_SIZE_BYTES >> 2)
#define FLASH_ADDRESS_MASK (((~0) << 23) ^ (~0))

/**
 * Read-Only Memory.
 *
 * Typically used to store the application code.
 */
extern uint32_t FLASH_MEMORY[FLASH_SIZE_ELEMENTS];

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
void load(uint32_t address, uint32_t *value, uint32_t false_read);

/**
 * Store data into memory.
 *
 * @param address The address to store the data to.
 * @param value The data to store at that address.
 */
void store(uint32_t address, uint32_t value);
}

#endif
