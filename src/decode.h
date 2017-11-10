#ifndef DECODE_HEADER
#define DECODE_HEADER

#include "sim_support.h"

// Registers that hold the results of instruction decoding
// Passed to exectue, memory access, and write-back stage in decoded variable
typedef struct {
  unsigned char rD;
  unsigned char rM;
  unsigned char rN;
  uint32_t imm;
  uint32_t cond;
  uint32_t reg_list;
} DECODE_RESULT;

// Interface to the decode stage
// Sets the decode stage registers based upon the passed instruction
// Prints a message and exits the simulator upon decoding error
DECODE_RESULT decode(uint16_t pInsn);

#endif
