#ifndef THUMBULATOR_DECODE_H
#define THUMBULATOR_DECODE_H

#include <cstdint>

namespace thumbulator {

/**
 * Enums for instruction type
 */
enum DoubleOpInsn {
  MOV  = 0x4000,
  ADD  = 0x5000,
  ADDC = 0x6000,
  SUBC = 0x7000,
  SUB  = 0x8000,
  CMP  = 0x9000,
  DADD = 0xA000,
  BIT  = 0xB000,
  BIC  = 0xC000,
  BIS  = 0xD000,
  XOR  = 0xE000,
  AND  = 0xF000
};
enum SingleOpInsn {
  RRC  = 0x1000,
  SWPB = 0x1080,
  RRA  = 0x1100,
  SXT  = 0x1180,
  PUSH = 0x1200,
  CALL = 0x1280,
  RETI = 0x1300
};
enum JumpInsn {
  JNE = 0x2000,
  JEQ = 0x2400,
  JNC = 0x2800,
  JC  = 0x2C00,
  JN  = 0x3000,
  JGE = 0x3400,
  JL  = 0x3800,
  JMP = 0x3C00
};

/**
 * Registers that hold the results of instruction decoding.
 *
 * Passed to execute, memory access, and write-back stage in decoded variable.
 */
struct decode_result {
  /**
   * Destination register-index.
   */
  uint8_t Rd; // rdst

  /**
   * Source register-index.
   */
  uint8_t Rs; //rsrc

  // addressing modes
  uint8_t As;
  uint8_t Ad;

  // immediate or offset
  uint32_t srcWord;
  uint32_t dstWord;

  // distinguish between word and byte instructions
  bool isByte;
  bool isAddrWord;

  uint16_t opcode;
  int16_t offset;


};

/**
 * Interface to the decode stage.
 *
 * @param instruction The instruction to decode.
 * @return The decode stage registers based upon the passed instruction.
 */
decode_result decode(uint16_t instruction);
}

#endif
