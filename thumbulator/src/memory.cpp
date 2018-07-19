#include "thumbulator/memory.hpp"

#include <cstdio>

#include "cpu_flags.hpp"
#include "exit.hpp"

namespace thumbulator {

uint16_t RESET_VECTOR;

uint16_t RAM[RAM_SIZE_BYTES >> 1];

std::function<uint16_t(uint32_t, uint16_t)> ram_load_hook;
std::function<uint16_t(uint32_t, uint16_t, uint16_t)> ram_store_hook;

uint16_t FLASH_MEMORY[FLASH_SIZE_BYTES >> 1];

uint16_t ram_load(uint32_t address, bool false_read)
{
  auto data = RAM[(address & RAM_ADDRESS_MASK) >> 1];

  if(!false_read && ram_load_hook != nullptr) {
    data = ram_load_hook(address, data);
  }

  return data;
}

void ram_store(uint32_t address, uint16_t value)
{
  if(ram_store_hook != nullptr) {
    auto const old_value = ram_load(address, true);

    value = ram_store_hook(address, old_value, value);
  }

  RAM[(address & RAM_ADDRESS_MASK) >> 1] = value;
}

//TODO: is the below true for MSP430??
// Memory access functions assume that RAM has a higher address than Flash
void fetch_instruction(uint32_t address, uint16_t *value)
{
  uint16_t fromMem;
  uint32_t idx;

  if(address >= FLASH_START) {
    if(address >= (FLASH_START + FLASH_SIZE_BYTES)) {
      fprintf(
        stderr, "Error: ILF Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      terminate_simulation(1);
    }

    idx = FLASH_ADDRESS_MASK(address);
    fromMem = FLASH_MEMORY[idx];
  }
  else{
    if(address >= RAM_START) {
      if(address >= (RAM_START + RAM_SIZE_BYTES)) {
        fprintf(
          stderr, "Error: ILR Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
        terminate_simulation(1);
      }

      fromMem = ram_load(address, false);
    }
  }

  *value = fromMem;

}

void load(uint32_t address, uint16_t *value, uint16_t false_read)
{
  //TODO: support byte level load
  if(address >= FLASH_START) {
    if(address >= (FLASH_START + FLASH_SIZE_BYTES)) {
      if(address == RESET_VECTOR_START) {
        *value = RESET_VECTOR;
        return;
      }
      fprintf(
        stderr, "Error: DLF Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      terminate_simulation(1);
    }

    //*value = FLASH_MEMORY[(address & FLASH_ADDRESS_MASK) >> 1];
    *value = FLASH_MEMORY[FLASH_ADDRESS_MASK(address)];
  }
  else{
    if(address >= RAM_START) {
      if(address >= (RAM_START + RAM_SIZE_BYTES)) {
        fprintf(
          stderr, "Error: DLR Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
        terminate_simulation(1);
      }

      *value = ram_load(address, false_read == 1);
    }
  }
}

void store(uint32_t address, uint16_t value)
  //TODO: support byte level store
{
  if(address >= FLASH_START) {
    if(address >= (FLASH_START + FLASH_SIZE_BYTES)) {
      if(address == RESET_VECTOR_START) {
        RESET_VECTOR = value;
        return;
      }
      fprintf(
        stderr, "Error: DSF Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      terminate_simulation(1);
    }

    FLASH_MEMORY[FLASH_ADDRESS_MASK(address)] = value;
  } else {
    if(address >= (RAM_START + RAM_SIZE_BYTES)) {
      fprintf(
          stderr, "Error: DSR Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      terminate_simulation(1);
    }

    ram_store(address, value);
  }
}
}
