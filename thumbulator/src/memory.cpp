#include "thumbulator/memory.hpp"

#include <cstdio>

#include "cpu_flags.hpp"
#include "exit.hpp"

namespace thumbulator {

uint16_t RESET_VECTOR;
uint16_t INTERRUPT_VECTOR_TABLE[INTERRUPT_VECTOR_SIZE_ELEMENTS];

uint16_t RAM[RAM_SIZE_BYTES >> 1];

std::function<uint16_t(uint32_t, uint16_t)> ram_load_hook;
std::function<uint16_t(uint32_t, uint16_t, uint16_t)> ram_store_hook;

uint16_t FLASH_MEMORY[FLASH_SIZE_BYTES >> 1];

uint16_t ram_load(uint32_t address, bool false_read)
{
  auto data = RAM[RAM_ADDRESS_MASK(address)];

  if(!false_read && ram_load_hook != nullptr) {
    data = ram_load_hook(address, data);
  }

  return data;
}

void ram_store(uint32_t address, uint16_t value, bool isByte)
{
  if(ram_store_hook != nullptr) {
    auto const old_value = ram_load(address, true);

    value = ram_store_hook(address, old_value, value);
  }

  uint32_t idx = RAM_ADDRESS_MASK(address);
  if(isByte) {
    if(address & 0x1) { //upper
      RAM[idx] = ((value & 0xFF)<<8) | (RAM[idx] & 0xFF);
    }
    else { //lower
      RAM[idx] = (value & 0xFF) | (RAM[idx] & 0xFF00);
    }
  }
  else {
    RAM[idx] = value;
  }
}

void fetch_instruction(uint32_t address, uint16_t *value)
{
  uint32_t fromMem;
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

void load(uint32_t address, uint32_t *value, uint16_t false_read)
{
  //TODO: support byte level load
    // TODO :: support 20 bit loads
  if(address >= FLASH_START) {
    if(address >= (FLASH_START + FLASH_SIZE_BYTES)) {
      if(address == RESET_VECTOR_START) {
        *value = RESET_VECTOR;
        return;
      }
      if(address >= INTERRUPT_VECTOR_START) {
        uint32_t idx = INTERRUPT_VECTOR_ADDRESS_MASK(address);
        *value = INTERRUPT_VECTOR_TABLE[idx];
        return;
      }
      fprintf(
        stderr, "Error: DLF Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      terminate_simulation(1);
    }


    if (address==RESET_VECTOR_START) {
        *value = RESET_VECTOR;
    }
    else {


        //*value = FLASH_MEMORY[(address & FLASH_ADDRESS_MASK) >> 1];
        *value = FLASH_MEMORY[FLASH_ADDRESS_MASK(address)];
    }
  }
  else if (address >= RAM_START) {
    if(address >= RAM_START) {
      if(address >= (RAM_START + RAM_SIZE_BYTES)) {
        fprintf(
          stderr, "Error: DLR Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
        terminate_simulation(1);
      }

      *value = ram_load(address, false_read == 1);
    }
  }
  else {
    // accessing either peripherals or special function registers
  }
}

void store(uint32_t address, uint16_t value, bool isByte)
{
  if(address >= FLASH_START) {
    if(address >= (FLASH_START + FLASH_SIZE_BYTES)) {
      if(address == RESET_VECTOR_START) {
        RESET_VECTOR = value;
        return;
      }
      if(address >= INTERRUPT_VECTOR_START) {
        uint32_t idx = INTERRUPT_VECTOR_ADDRESS_MASK(address);
        INTERRUPT_VECTOR_TABLE[idx] = value;
        return;
      }
      fprintf(
        stderr, "Error: DSF Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      terminate_simulation(1);
    }

    uint32_t idx = FLASH_ADDRESS_MASK(address);
    if(isByte) {
      if(address & 0x1) { //upper
        FLASH_MEMORY[idx] = ((value & 0xFF)<<8) | (FLASH_MEMORY[idx] & 0xFF);
      }
      else { //lower
        FLASH_MEMORY[idx] = (value & 0xFF) | (FLASH_MEMORY[idx] & 0xFF00);
      }
    }
    else {
      FLASH_MEMORY[FLASH_ADDRESS_MASK(address)] = value;
    }
  }
  else if (address >= RAM_START) {
    if(address >= (RAM_START + RAM_SIZE_BYTES)) {
      fprintf(
          stderr, "Error: DSR Memory access out of range: 0x%8.8X, pc=%x\n", address, cpu_get_pc());
      terminate_simulation(1);
    }

    ram_store(address, value, isByte);
  }
  else {
    // accessing either peripherals or special function registers
  }
}
}
