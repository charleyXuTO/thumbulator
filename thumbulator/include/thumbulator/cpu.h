#ifndef THUMBULATOR_CPU_H
#define THUMBULATOR_CPU_H

#include <stdint.h>

#include "thumbulator/decode.h"

/**
 * The state of an armv6m CPU.
 */
typedef struct {
  /**
   * General-purpose register including FP, SP, LR, and PC.
   */
  uint32_t gpr[16];

  /**
   * Application program status register.
   */
  uint32_t apsr;

  /**
   * Interrupt program status register.
   *
   * Exception number.
   */
  uint32_t ipsr;

  /**
   * Exception program status register.
   *
   * Not software readable.
   */
  uint32_t espr;

  uint32_t primask;
  uint32_t control;
  uint32_t sp_main;
  uint32_t sp_process;
  uint32_t mode;

  /**
   * Bit mask of pending exceptions.
   */
  uint32_t exceptmask;
} cpu_state;

extern cpu_state cpu;

/**
 * Get a general-purpose register.
 */
#define cpu_get_gpr(x) cpu.gpr[x]

/**
 * Set a general-purpose register.
 */
#define cpu_set_gpr(x, y) cpu.gpr[x] = y

/**
 * The register-index of the program counter.
 */
#define GPR_PC 15

/**
 * Get the value currently stored in the program counter.
 */
#define cpu_get_pc() cpu_get_gpr(GPR_PC)

/**
 * Change the value stored in the program counter.
 */
#define cpu_set_pc(x) cpu_set_gpr(GPR_PC, (x))

typedef struct {
  uint32_t control;
  uint32_t reload;
  uint32_t value;
  uint32_t calib;
} system_tick;

extern system_tick systick;

/**
 * Cycles taken for branch instructions.
 */
#define TIMING_BRANCH 2

/**
 * Cycles taken for the branch with link instruction.
 */
#define TIMING_BRANCH_LINK 3

/**
 * Cycles taken for instructions that update the program counter.
 */
#define TIMING_PC_UPDATE 2

/**
 * Cycles taken for memory instructions.
 */
#define TIMING_MEM 2

/**
 * Perform the execute, mem, and write-back stages.
 *
 * @param pInsn The instruction to execute.
 * @param decoded The result from the decode stage.
 *
 * @return The number of cycles taken.
 */
uint32_t exmemwb(uint16_t pInsn, decode_result decoded);

#endif //THUMBULATOR_CPU_H
