#ifndef DECODE_HEADER
#define DECODE_HEADER

#include "sim_support.h"

/**
 * Registers that hold the results of instruction decoding.
 *
 * Passed to exectue, memory access, and write-back stage in decoded variable.
 */
typedef struct {
  uint8_t rD;
  uint8_t rM;
  uint8_t rN;
  uint32_t imm;
  uint32_t cond;
  uint32_t reg_list;
} decode_result;

/**
 * Interface to the decode stage.
 *
 * @param instruction The instruction to decode.
 * @return The decode stage registers based upon the passed instruction.
 */
decode_result decode(uint16_t instruction);

#endif
