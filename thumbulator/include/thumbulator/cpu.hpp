#ifndef THUMBULATOR_CPU_H
#define THUMBULATOR_CPU_H

#include <cstdint>

#include "thumbulator/decode.hpp"

namespace thumbulator {

// TODO: what to use here??
constexpr auto CPU_FREQ = 1000000; // use 1 MHz default for MSP430AFE253 for now

/**
 * The state of an MSP430 CPU.
 */
struct cpu_state {
  /**
   * General-purpose register including PC, SP, SR, and GC.
   */
  uint32_t gpr[16];

  /**
   * Application program status register.
   */
  //uint32_t apsr;

  /**
   * Interrupt program status register.
   *
   * Exception number.
   */
  //uint32_t ipsr;

  // TODO: figure out interrupt, interrupt vectors??
  /**
   * Exception program status register.
   *
   * Not software readable.
   */
  //uint32_t espr;

  //uint32_t primask;
  //uint32_t control;
  //uint32_t sp_main;
  //uint32_t sp_process;
  //uint32_t mode;

  /**
   * Bit mask of pending exceptions.
   */
  //uint32_t exceptmask;
};

/**
 * Informs fetch that previous instruction caused a control flow change
 */
extern bool BRANCH_WAS_TAKEN; //TODO

/**
 * Whether or not the exit instruction has been executed.
 */
extern bool EXIT_INSTRUCTION_ENCOUNTERED; //TODO

/**
 * Resets the CPU according to the specification.
 */
void cpu_reset();

extern cpu_state cpu;

/*******************************************************************
 * Register macros
 *******************************************************************/
/**
 * Get a general-purpose register.
 */
#define cpu_get_gpr(x) cpu.gpr[x]

/**
 * Set a general-purpose register.
 */
#define cpu_set_gpr(x, y) cpu.gpr[x] = y

/*******************************************************************
 * PC, SP, SR, CG1, CG2
 *******************************************************************/
/**
 * The register-index of special registers.
 */
#define GPR_PC 0
#define GPR_SP 1
#define GPR_SR 2
#define GPR_CG1 2
#define GPR_CG2 3

/**
 * Get the value currently stored in the special registers.
 */
#define cpu_get_pc() cpu_get_gpr(GPR_PC)
#define cpu_get_sp() cpu_get_gpr(GPR_SP)
#define cpu_get_sr() cpu_get_gpr(GPR_SR)
#define cpu_get_cg1() cpu_get_gpr(GPR_CG1)
#define cpu_get_cg2() cpu_get_gpr(GPR_CG2)

/**
 * Change the value stored in the special registers.
 */
#define cpu_set_pc(x) cpu_set_gpr(GPR_PC, (x))
#define cpu_set_sp(x) cpu_set_gpr(GPR_SP, (x))
#define cpu_set_sr(x) cpu_set_gpr(GPR_SR, (x))
#define cpu_set_cg1(x) cpu_set_gpr(GPR_CG1, (x))
#define cpu_set_cg2(x) cpu_set_gpr(GPR_CG2, (x))


//TODO: what is this?!?!? system timer?!?!
//struct system_tick {
//  uint32_t control;
//  uint32_t reload;
//  uint32_t value;
//  uint32_t calib;
//};
//
//extern system_tick SYSTICK;

/**
 * Cycles taken for branch instructions.
 */
//#define TIMING_BRANCH 2

/**
 * Cycles taken for the branch with link instruction.
 */
//#define TIMING_BRANCH_LINK 3

/**
 * Cycles taken for instructions that update the program counter.
 */
//#define TIMING_PC_UPDATE 2

/**
 * Cycles taken for memory instructions.
 */
//#define TIMING_MEM 2

/**
 * Perform the execute, mem, and write-back stages.
 *
 * @param instruction The instruction to execute.
 * @param decoded The result from the decode stage.
 *
 * @return The number of cycles taken.
 */
uint32_t exmemwb(uint16_t instruction, decode_result const *decoded);
}

#endif //THUMBULATOR_CPU_H
