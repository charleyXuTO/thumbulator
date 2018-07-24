#ifndef THUMBULATOR_UTILS_H
#define THUMBULATOR_UTILS_H

#include "thumbulator/cpu.hpp"
#include "cpu_flags.hpp"

namespace thumbulator {

//----- addressing modes
AddrMode getAddrMode(uint8_t addrMode, uint8_t reg, bool isSource);
uint32_t getAddressBaseOnMode(uint8_t addrMode, uint8_t reg, uint16_t nextWord);
uint16_t getValue(uint8_t addrMode, uint8_t reg, uint16_t nextWord, bool isByte, bool isSource);
void setValue(uint8_t addrMode, uint8_t reg, uint16_t nextWord, bool isByte, uint16_t val);
void updateAutoIncrementReg(uint8_t addrMode, uint8_t reg, bool isAddrWord, bool isByte);

//----- cycle time

// implements table 4-10 in SLAU367O
// Table[src_addr_mode][dst_addr_mode/PC]
const uint32_t doubleOpCycleCountTable[7][5] = {
  {2, 4, 5, 5, 5}, // Rn
  {4, 6, 7, 7, 7}, // x(Rn)
  {4, 6, 7, 7, 7}, // EDE
  {4, 6, 7, 7, 7}, // &EDE
  {3, 5, 6, 6, 6}, // @Rn
  {3, 5, 6, 6, 6}, // @Rn+
  {3, 4, 6, 6, 6}  // #N
};
uint32_t footnote2(uint32_t cycles);
uint32_t footnote4(uint32_t cycles);
uint32_t getDoubleOpColumnIdx(AddrMode dstMode, uint16_t Rd);
uint32_t getDoubleOperandCycleCount(decode_result const *decoded);

// implements table 4-9 in SLAU367O
// Table[addr_mode][opcode]
const uint32_t singleOpCycleCountTable[7][3] = {
  {1, 3, 4}, // Rn
  {4, 4, 5}, // x(Rn)
  {4, 4, 5}, // EDE
  {4, 4, 6}, // &EDE
  {3, 3, 4}, // @Rn
  {3, 3, 4}, // @Rn+
  {0, 3, 4}  // #N
};
uint32_t getSingleOpColumnIdx(uint16_t opcode);
uint32_t getSingleOperandCycleCount(decode_result const *decoded);
uint32_t getJumpCycleCount(decode_result const *decoded);

}

#endif //THUMBULATOR_UTILS_H
