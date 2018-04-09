#ifndef THUMBULATOR_DECODE_H
#define THUMBULATOR_DECODE_H

#include <cstdint>

namespace thumbulator {



/**
 * Enums for instruction type
 * TODO: decide whether to use rel or abs
 */
enum DoubleOpInsn {
  MOV = 0x4,
  ADD = 0x5,
  ADDC = 0x6,
  SUBC = 0x7,
  SUB = 0x8,
  CMP = 0x9,
  DADD = 0xA,
  BIT = 0xB,
  BIC = 0xC,
  BIS = 0xD,
  XOR = 0xE,
  AND = 0xF
};
enum SingleOpInsn { // rel
  RRC  = 0x1000,    // 0x020,
  SWPB = 0x1080,    // 0x021,
  RRA  = 0x1100,    // 0x022,
  SXT  = 0x1180,    // 0x023,
  PUSH = 0x1200,    // 0x024,
  CALL = 0x1280,    // 0x025,
  RETI = 0x1300     // 0x026
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
 * Passed to exectue, memory access, and write-back stage in decoded variable.
 */
struct decode_result {
  /**
   * Destination register-index.
   */
  uint8_t Rd;

  /**
   * Source register-index.
   */
  uint8_t Rs;


  uint8_t As;
  uint8_t Ad;
  uint16_t srcWord;
  uint16_t dstWord;
  bool isByte;
  uint16_t opcode;
  uint16_t offset;

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
