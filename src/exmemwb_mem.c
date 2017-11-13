#include "exmemwb.h"

///--- Load/store multiple operations --------------------------------------------///

// LDM - Load multiple registers from the stack
uint32_t ldm(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("ldm r%u!, {0x%X}\n", decoded.rN, decoded.reg_list);

  uint32_t numLoaded = 0;
  uint32_t rNWritten = (1 << decoded.rN) & decoded.reg_list;
  uint32_t address = cpu_get_gpr(decoded.rN);

  for(int i = 0; i < 8; ++i) {
    int mask = 1 << i;
    if(decoded.reg_list & mask) {
      uint32_t data = 0;
      load(address, &data, 0);
      cpu_set_gpr(i, data);
      address += 4;
      ++numLoaded;
    }
  }

  if(rNWritten == 0)
    cpu_set_gpr(decoded.rN, address);

  return 1 + numLoaded;
}

// STM - Store multiple registers to the stack
uint32_t stm(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("stm r%u!, {0x%X}\n", decoded.rN, decoded.reg_list);

  uint32_t numStored = 0;
  uint32_t address = cpu_get_gpr(decoded.rN);

  for(int i = 0; i < 8; ++i) {
    int mask = 1 << i;
    if(decoded.reg_list & mask) {
      if(i == decoded.rN && numStored == 0) {
        fprintf(stderr, "Error: Malformed instruction!\n");
        terminate_simulation(1);
      }

      uint32_t data = cpu_get_gpr(i);
      store(address, data);
      address += 4;
      ++numStored;
    }
  }

  cpu_set_gpr(decoded.rN, address);

  return 1 + numStored;
}

///--- Stack operations --------------------------------------------///

// Pop multiple reg values from the stack and update SP
uint32_t pop(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("pop {0x%X}\n", decoded.reg_list);

  uint32_t numLoaded = 0;
  uint32_t address = cpu_get_sp();

  for(int i = 0; i < 16; ++i) {
    int mask = 1 << i;
    if(decoded.reg_list & mask) {
      uint32_t data = 0;
      load(address, &data, 0);
      cpu_set_gpr(i, data);
      ++numLoaded;
      if(i == 15)
        takenBranch = 1;
      address += 4;
    }

    // Skip constant 0s
    if(i == 7)
      i = 14;
  }

  cpu_set_sp(address);

  return 1 + numLoaded + takenBranch ? TIMING_PC_UPDATE : 0;
}

// Push multiple reg values to the stack and update SP
uint32_t push(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("push {0x%4.4X}\n", decoded.reg_list);

  uint32_t numStored = 0;
  uint32_t address = cpu_get_sp();

  for(int i = 14; i >= 0; --i) {
    int mask = 1 << i;
    if(decoded.reg_list & mask) {
      address -= 4;
      uint32_t data = cpu_get_gpr(i);
      store(address, data);
      ++numStored;
    }

    // Skip constant 0s
    if(i == 14)
      i = 8;
  }

  cpu_set_sp(address);

  return 1 + numStored;
}

///--- Single load operations --------------------------------------------///

// LDR - Load from offset from register
uint32_t ldr_i(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("ldr r%u, [r%u, #0x%X]\n", decoded.rD, decoded.rN, decoded.imm << 2);

  uint32_t base = cpu_get_gpr(decoded.rN);
  uint32_t offset = zeroExtend32(decoded.imm << 2);
  uint32_t effectiveAddress = base + offset;

  uint32_t result = 0;
  load(effectiveAddress, &result, 0);

  cpu_set_gpr(decoded.rD, result);

  return TIMING_MEM;
}

// LDR - Load from offset from SP
uint32_t ldr_sp(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("ldr r%u, [SP, #0x%X]\n", decoded.rD, decoded.imm << 2);

  uint32_t base = cpu_get_sp();
  uint32_t offset = zeroExtend32(decoded.imm << 2);
  uint32_t effectiveAddress = base + offset;

  uint32_t result = 0;
  load(effectiveAddress, &result, 0);

  cpu_set_gpr(decoded.rD, result);

  return TIMING_MEM;
}

// LDR - Load from offset from PC
uint32_t ldr_lit(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("ldr r%u, [PC, #%d]\n", decoded.rD, decoded.imm << 2);

  uint32_t base = cpu_get_pc() & 0xFFFFFFFC;
  uint32_t offset = zeroExtend32(decoded.imm << 2);
  uint32_t effectiveAddress = base + offset;

  uint32_t result = 0;
  load(effectiveAddress, &result, 0);

  cpu_set_gpr(decoded.rD, result);

  return TIMING_MEM;
}

// LDR - Load from an offset from a reg based on another reg value
uint32_t ldr_r(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("ldr r%u, [r%u, r%u]\n", decoded.rD, decoded.rN, decoded.rM);

  uint32_t base = cpu_get_gpr(decoded.rN);
  uint32_t offset = cpu_get_gpr(decoded.rM);
  uint32_t effectiveAddress = base + offset;

  uint32_t result = 0;
  load(effectiveAddress, &result, 0);

  cpu_set_gpr(decoded.rD, result);

  return TIMING_MEM;
}

// LDRB - Load byte from offset from register
uint32_t ldrb_i(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("ldrb r%u, [r%u, #0x%X]\n", decoded.rD, decoded.rN, decoded.imm);

  uint32_t base = cpu_get_gpr(decoded.rN);
  uint32_t offset = zeroExtend32(decoded.imm);
  uint32_t effectiveAddress = base + offset;
  uint32_t effectiveAddressWordAligned = effectiveAddress & ~0x3;

  uint32_t result = 0;
  load(effectiveAddressWordAligned, &result, 0);

  // Select the correct byte
  switch(effectiveAddress & 0x3) {
  case 0:
    break;
  case 1:
    result >>= 8;
    break;
  case 2:
    result >>= 16;
    break;
  case 3:
    result >>= 24;
  }

  result = zeroExtend32(result & 0xFF);

  cpu_set_gpr(decoded.rD, result);

  return TIMING_MEM;
}

// LDRB - Load byte from an offset from a reg based on another reg value
uint32_t ldrb_r(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("ldrb r%u, [r%u, r%u]\n", decoded.rD, decoded.rN, decoded.rM);

  uint32_t base = cpu_get_gpr(decoded.rN);
  uint32_t offset = cpu_get_gpr(decoded.rM);
  uint32_t effectiveAddress = base + offset;
  uint32_t effectiveAddressWordAligned = effectiveAddress & ~0x3;

  uint32_t result = 0;
  load(effectiveAddressWordAligned, &result, 0);

  // Select the correct byte
  switch(effectiveAddress & 0x3) {
  case 0:
    break;
  case 1:
    result >>= 8;
    break;
  case 2:
    result >>= 16;
    break;
  case 3:
    result >>= 24;
  }

  result = zeroExtend32(result & 0xFF);

  cpu_set_gpr(decoded.rD, result);

  return TIMING_MEM;
}

// LDRH - Load halfword from offset from register
uint32_t ldrh_i(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("ldrh r%u, [r%u, #0x%X]\n", decoded.rD, decoded.rN, decoded.imm);

  uint32_t base = cpu_get_gpr(decoded.rN);
  uint32_t offset = zeroExtend32(decoded.imm << 1);
  uint32_t effectiveAddress = base + offset;
  uint32_t effectiveAddressWordAligned = effectiveAddress & ~0x3;

  uint32_t result = 0;
  load(effectiveAddressWordAligned, &result, 0);

  // Select the correct halfword
  switch(effectiveAddress & 0x2) {
  case 0:
    break;
  default:
    result >>= 16;
    break;
  }

  result = zeroExtend32(result & 0xFFFF);

  cpu_set_gpr(decoded.rD, result);

  return TIMING_MEM;
}

// LDRH - Load halfword from an offset from a reg based on another reg value
uint32_t ldrh_r(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("ldrh r%u, [r%u, r%u]\n", decoded.rD, decoded.rN, decoded.rM);

  uint32_t base = cpu_get_gpr(decoded.rN);
  uint32_t offset = cpu_get_gpr(decoded.rM);
  uint32_t effectiveAddress = base + offset;
  uint32_t effectiveAddressWordAligned = effectiveAddress & ~0x3;

  uint32_t result = 0;
  load(effectiveAddressWordAligned, &result, 0);

  // Select the correct halfword
  switch(effectiveAddress & 0x2) {
  case 0:
    break;
  default:
    result >>= 16;
    break;
  }

  result = zeroExtend32(result & 0xFFFF);

  cpu_set_gpr(decoded.rD, result);

  return TIMING_MEM;
}

// LDRSB - Load signed byte from an offset from a reg based on another reg value
uint32_t ldrsb_r(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("ldrsb r%u, [r%u, r%u]\n", decoded.rD, decoded.rN, decoded.rM);

  uint32_t base = cpu_get_gpr(decoded.rN);
  uint32_t offset = cpu_get_gpr(decoded.rM);
  uint32_t effectiveAddress = base + offset;
  uint32_t effectiveAddressWordAligned = effectiveAddress & ~0x3;

  uint32_t result = 0;
  load(effectiveAddressWordAligned, &result, 0);

  // Select the correct byte
  switch(effectiveAddress & 0x3) {
  case 0:
    break;
  case 1:
    result >>= 8;
    break;
  case 2:
    result >>= 16;
    break;
  case 3:
    result >>= 24;
  }

  result = signExtend32(result & 0xFF, 8);

  cpu_set_gpr(decoded.rD, result);

  return TIMING_MEM;
}

// LDRSH - Load signed halfword from an offset from a reg based on another reg value
uint32_t ldrsh_r(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("ldrsh r%u, [r%u, r%u]\n", decoded.rD, decoded.rN, decoded.rM);

  uint32_t base = cpu_get_gpr(decoded.rN);
  uint32_t offset = cpu_get_gpr(decoded.rM);
  uint32_t effectiveAddress = base + offset;
  uint32_t effectiveAddressWordAligned = effectiveAddress & ~0x3;

  uint32_t result = 0;
  load(effectiveAddressWordAligned, &result, 0);

  // Select the correct halfword
  switch(effectiveAddress & 0x2) {
  case 0:
    break;
  default:
    result >>= 16;
  }

  result = signExtend32(result & 0xFFFF, 16);

  cpu_set_gpr(decoded.rD, result);

  return TIMING_MEM;
}

///--- Single store operations --------------------------------------------///

// STR - Store to offset from register
uint32_t str_i(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("str r%u, [r%u, #%d]\n", decoded.rD, decoded.rN, decoded.imm << 2);

  uint32_t base = cpu_get_gpr(decoded.rN);
  uint32_t offset = zeroExtend32(decoded.imm << 2);
  uint32_t effectiveAddress = base + offset;

  store(effectiveAddress, cpu_get_gpr(decoded.rD));

  return TIMING_MEM;
}

// STR - Store to offset from SP
uint32_t str_sp(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("str r%u, [SP, #%d]\n", decoded.rD, decoded.imm << 2);

  uint32_t base = cpu_get_sp();
  uint32_t offset = zeroExtend32(decoded.imm << 2);
  uint32_t effectiveAddress = base + offset;

  store(effectiveAddress, cpu_get_gpr(decoded.rD));

  return TIMING_MEM;
}

// STR - Store to an offset from a reg based on another reg value
uint32_t str_r(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("str r%u, [r%u, r%u]\n", decoded.rD, decoded.rN, decoded.rM);

  uint32_t base = cpu_get_gpr(decoded.rN);
  uint32_t offset = cpu_get_gpr(decoded.rM);
  uint32_t effectiveAddress = base + offset;

  store(effectiveAddress, cpu_get_gpr(decoded.rD));

  return TIMING_MEM;
}

// STRB - Store byte to offset from register
uint32_t strb_i(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("strb r%u, [r%u, #0x%X]\n", decoded.rD, decoded.rN, decoded.imm);

  uint32_t base = cpu_get_gpr(decoded.rN);
  uint32_t offset = zeroExtend32(decoded.imm);
  uint32_t effectiveAddress = base + offset;
  uint32_t effectiveAddressWordAligned = effectiveAddress & ~0x3;
  uint32_t data = cpu_get_gpr(decoded.rD) & 0xFF;

  uint32_t orig;
  load(effectiveAddressWordAligned, &orig, 1);

  // Select the correct byte
  switch(effectiveAddress & 0x3) {
  case 0:
    orig = (orig & 0xFFFFFF00) | (data << 0);
    break;
  case 1:
    orig = (orig & 0xFFFF00FF) | (data << 8);
    break;
  case 2:
    orig = (orig & 0xFF00FFFF) | (data << 16);
    break;
  case 3:
    orig = (orig & 0x00FFFFFF) | (data << 24);
  }

  store(effectiveAddressWordAligned, orig);

  return TIMING_MEM;
}

// STRB - Store byte to an offset from a reg based on another reg value
uint32_t strb_r(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("strb r%u, [r%u, r%u]\n", decoded.rD, decoded.rN, decoded.rM);

  uint32_t base = cpu_get_gpr(decoded.rN);
  uint32_t offset = cpu_get_gpr(decoded.rM);
  uint32_t effectiveAddress = base + offset;
  uint32_t effectiveAddressWordAligned = effectiveAddress & ~0x3;
  uint32_t data = cpu_get_gpr(decoded.rD) & 0xFF;

  uint32_t orig;
  load(effectiveAddressWordAligned, &orig, 1);

  // Select the correct byte
  switch(effectiveAddress & 0x3) {
  case 0:
    orig = (orig & 0xFFFFFF00) | (data << 0);
    break;
  case 1:
    orig = (orig & 0xFFFF00FF) | (data << 8);
    break;
  case 2:
    orig = (orig & 0xFF00FFFF) | (data << 16);
    break;
  case 3:
    orig = (orig & 0x00FFFFFF) | (data << 24);
  }

  store(effectiveAddressWordAligned, orig);

  return TIMING_MEM;
}

// STRH - Store halfword to offset from register
uint32_t strh_i(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("strh r%u, [r%u, #0x%X]\n", decoded.rD, decoded.rN, decoded.imm);

  uint32_t base = cpu_get_gpr(decoded.rN);
  uint32_t offset = zeroExtend32(decoded.imm << 1);
  uint32_t effectiveAddress = base + offset;
  uint32_t effectiveAddressWordAligned = effectiveAddress & ~0x3;
  uint32_t data = cpu_get_gpr(decoded.rD) & 0xFFFF;

  uint32_t orig;
  load(effectiveAddressWordAligned, &orig, 1);

  // Select the correct byte
  switch(effectiveAddress & 0x2) {
  case 0:
    orig = (orig & 0xFFFF0000) | (data << 0);
    break;
  default:
    orig = (orig & 0x0000FFFF) | (data << 16);
  }

  store(effectiveAddressWordAligned, orig);

  return TIMING_MEM;
}

// STRH - Store halfword to an offset from a reg based on another reg value
uint32_t strh_r(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("strh r%u, [r%u, r%u]\n", decoded.rD, decoded.rN, decoded.rM);

  uint32_t base = cpu_get_gpr(decoded.rN);
  uint32_t offset = cpu_get_gpr(decoded.rM);
  uint32_t effectiveAddress = base + offset;
  uint32_t effectiveAddressWordAligned = effectiveAddress & ~0x3;
  uint32_t data = cpu_get_gpr(decoded.rD) & 0xFFFF;

  uint32_t orig;
  load(effectiveAddressWordAligned, &orig, 1);

  // Select the correct byte
  switch(effectiveAddress & 0x2) {
  case 0:
    orig = (orig & 0xFFFF0000) | (data << 0);
    break;
  default:
    orig = (orig & 0x0000FFFF) | (data << 16);
  }

  store(effectiveAddressWordAligned, orig);

  return TIMING_MEM;
}
