#include "utils.hpp"
#include "thumbulator/memory.hpp"
#include "cpu_flags.hpp"
#include <assert.h>

namespace thumbulator {

//TODO: better print
std::string addrModeString[7] = {
  "REG", "IDX", "SYM", "ABS", "IDREG", "IDAUTO+", "IMM"
};

AddrMode getAddrMode(uint8_t addrMode, uint8_t reg) {
  AddrMode mode;
  switch(addrMode) {
    case 0: { // register
      mode = REGISTER;
      break;
    }
    case 1: {   // indexed, symbolic, absolute
      if(reg==GPR_SR)
        mode = ABSOLUTE;
      else if(reg==GPR_PC)
        mode = SYMBOLIC;
      else
        mode = INDEXED;
      break;
    }
    case 2: {   // indirect register
      mode = INDIRECT_REGISTER;
      break;
    }
    case 3: {   // indirect autoincrement, immediate
      if(reg==GPR_PC)
        mode = IMMEDIATE;
      else
        mode = INDIRECT_AUTOINCREMENT;
      break;
    }
    default:
      assert(0 && "getAddrMode: unknown addressing mode!");
  }
  return mode;
}

uint32_t getAddressBaseOnMode(uint8_t addrMode, uint8_t reg, uint32_t nextWord) {
  uint32_t retVal = 0;
  AddrMode mode = getAddrMode(addrMode, reg);
  switch(mode) {
    case REGISTER: {
      assert(0 && "should not call getAddressBaseOnMode in register mode");
      //val = cpu_get_gpr(reg);
      break;
    }
    case INDEXED: {
      int16_t x = nextWord;
      uint32_t Rn = cpu_get_gpr(reg);
      int32_t addr = Rn + x;
      retVal = addr;
      break;
    }
    case SYMBOLIC: {
      int16_t x = nextWord;
      uint32_t Rn = cpu_get_gpr(reg); // this is PC
      int32_t addr = Rn + x;
      retVal = addr;
      break;
    }
    case ABSOLUTE: {
      int16_t x = nextWord;
      retVal = x;
      break;
    }
    case INDIRECT_REGISTER: {
      retVal = cpu_get_gpr(reg);
      break;
    }
    case INDIRECT_AUTOINCREMENT: {
      retVal = cpu_get_gpr(reg);
      break;
    }
    case IMMEDIATE: {
      assert(0 && "should not call getAddressBaseOnMode in immediate mode");
      //uint32_t addr = nextword;
      //retVal = addr;
      break;
    }
    default:
      assert(0 && "unknown addressing mode!");
  }
  return retVal;
}

uint32_t getValue(uint8_t addrMode, uint8_t reg, uint32_t nextWord, bool isByte) {
  uint32_t val = 0;
  AddrMode mode = getAddrMode(addrMode, reg);
  if(mode==REGISTER) {
    val = cpu_get_gpr(reg);
  }
  else if(mode==IMMEDIATE) {
    val = nextWord;
  }
  else {
    uint32_t addr = getAddressBaseOnMode(addrMode, reg, nextWord);
    load(addr, &val, 0);
  }
  // adjust value based on type
  if(isByte)
      val &= 0xFF;
  return val;
}

void setValue(uint8_t addrMode, uint8_t reg, uint32_t nextWord, bool isByte, uint32_t val) {
  AddrMode mode = getAddrMode(addrMode, reg);
  if(mode==REGISTER) {
    if(isByte) {
      val &= 0xFF;
    }
    cpu_set_gpr(reg, val);
  }
  //else if(mode==IMMEDIATE) {
  //  val = nextWord;
  //}
  else {
    uint32_t addr = getAddressBaseOnMode(addrMode, reg, nextWord);
    if(isByte) {
      //TODO: this would effect load count?? double check false_read
      uint32_t oldVal;
      load(addr, &oldVal, 1);
      val = (val & 0xFF) | (oldVal & 0xFF00);
    }
    store(addr, val);
  }
  return;
}

void updateAutoIncrementReg(uint8_t addrMode, uint8_t reg, bool isAddrWord, bool isByte) {
  AddrMode mode = getAddrMode(addrMode, reg);
  if(mode==INDIRECT_AUTOINCREMENT) {
    uint32_t addr = cpu_get_gpr(reg);
    uint32_t newAddr = (isAddrWord)?(addr+4):((isByte)?(addr+1):(addr+2));
    //if(reg==GPR_PC) newAddr = addr; // PC already incremented in decode
    cpu_set_gpr(reg, newAddr);
  }
}

uint32_t footnote2(decode_result const *decoded, uint32_t cycles) {
  if(decoded->opcode == MOV ||
     decoded->opcode == BIT ||
     decoded->opcode == CMP) {
    cycles--;
  }
  return cycles;
}

uint32_t footnote4(decode_result const *decoded, uint32_t cycles) {
  if(decoded->opcode == MOV ||
     decoded->opcode == ADD ||
     decoded->opcode == SUB) {
    cycles--;
  }
  return cycles;
}

uint32_t getDoubleOpColumnIdx(AddrMode dstMode, uint16_t Rd) {
  uint32_t idx = 0;
  if(REGISTER==dstMode) {
    idx = 0;
  }
  else if(GPR_PC==Rd) {
    idx = 1;
  }
  else if(INDEXED==dstMode) {
    idx = 2;
  }
  else if(SYMBOLIC==dstMode) {
    idx = 3;
  }
  else if(ABSOLUTE==dstMode) {
    idx = 4;
  }
  else {
    assert(0);
  }
  return idx;
}

// implements table 4-10 in SLAU367O
uint32_t getDoubleOperandCycleCount(decode_result const *decoded) {
  uint32_t cycles = 0;
  AddrMode srcMode = getAddrMode(decoded->As, decoded->Rs);
  AddrMode dstMode = getAddrMode(decoded->Ad, decoded->Rd);
  uint32_t srcIdx = srcMode;
  uint32_t dstIdx = getDoubleOpColumnIdx(dstMode, decoded->Rd);
  cycles = doubleOpCycleCountTable[srcIdx][dstIdx];

  // special cases
  if(dstIdx>=2) {
    cycles = footnote2(decoded, cycles);
  }
  if(dstIdx==1 &&
     (srcIdx==6 || srcIdx==1 || srcIdx==2 || srcIdx==3)) {
    cycles = footnote4(decoded, cycles);
  }
  return cycles;
}

uint32_t getSingleOpColumnIdx(uint16_t opcode) {
  uint32_t idx = 0;
  if(RRA==opcode || RRC==opcode || SWPB==opcode || SXT==opcode) {
    idx = 0;
  }
  else if(PUSH==opcode) {
    idx = 1;
  }
  else if(CALL==opcode) {
    idx = 2;
  }
  else {
    assert(0);
  }
  return idx;
}

// implements table 4-9 in SLAU367O
uint32_t getSingleOperandCycleCount(decode_result const *decoded) {
  uint32_t cycles = 0;
  AddrMode dstMode = getAddrMode(decoded->Ad, decoded->Rd);
  uint32_t rowIdx = dstMode;
  uint32_t columnIdx = getSingleOpColumnIdx(decoded->opcode);
  cycles = singleOpCycleCountTable[rowIdx][columnIdx];

  return cycles;
}

uint32_t getJumpCycleCount(decode_result const *decoded) {
  return 2;
}


}
