#include "exmemwb.h"

///--- Add operations --------------------------------------------///

// ADCS - add with carry and update flags
uint32_t adcs(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("adcs r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t opA = cpu_get_gpr(decoded.rD);
  uint32_t opB = cpu_get_gpr(decoded.rM);
  uint32_t result = opA + opB + cpu_get_flag_c();

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);
  do_cflag(opA, opB, cpu_get_flag_c());
  do_vflag(opA, opB, result);

  return 1;
}

// ADD - add small immediate to a register and update flags
uint32_t adds_i3(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("adds r%u, r%u, #0x%X\n", decoded.rD, decoded.rN, decoded.imm);

  uint32_t opA = cpu_get_gpr(decoded.rN);
  uint32_t opB = zeroExtend32(decoded.imm);
  uint32_t result = opA + opB;

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);
  do_cflag(opA, opB, 0);
  do_vflag(opA, opB, result);

  return 1;
}

// ADD - add large immediate to a register and update flags
uint32_t adds_i8(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("adds r%u, #0x%X\n", decoded.rD, decoded.imm);

  uint32_t opA = cpu_get_gpr(decoded.rD);
  uint32_t opB = zeroExtend32(decoded.imm);
  uint32_t result = opA + opB;

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);
  do_cflag(opA, opB, 0);
  do_vflag(opA, opB, result);

  return 1;
}

// ADD - add two registers and update flags
uint32_t adds_r(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("adds r%u, r%u, r%u\n", decoded.rD, decoded.rN, decoded.rM);

  uint32_t opA = cpu_get_gpr(decoded.rN);
  uint32_t opB = cpu_get_gpr(decoded.rM);
  uint32_t result = opA + opB;

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);
  do_cflag(opA, opB, 0);
  do_vflag(opA, opB, result);

  return 1;
}

// ADD - add two registers, one or both high no flags
uint32_t add_r(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("add r%u, r%u\n", decoded.rD, decoded.rM);

  // Check for malformed instruction
  if(decoded.rD == 15 && decoded.rM == 15) {
    //UNPREDICTABLE
    fprintf(stderr, "Error: Instruction format error.\n");
    sim_exit(1);
  }

  uint32_t opA = cpu_get_gpr(decoded.rD);
  uint32_t opB = cpu_get_gpr(decoded.rM);
  uint32_t result = opA + opB;

  // If changing the PC, check that thumb mode maintained
  if(decoded.rD == GPR_PC)
    alu_write_pc(result);
  else
    cpu_set_gpr(decoded.rD, result);

  // Instruction takes two cycles when PC is the destination
  return (decoded.rD == GPR_PC) ? 2 : 1;
}

// ADD - add an immpediate to SP
uint32_t add_sp(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("add r%u, SP, #0x%02X\n", decoded.rD, decoded.imm);

  uint32_t opA = cpu_get_sp();
  uint32_t opB = zeroExtend32(decoded.imm << 2);
  uint32_t result = opA + opB;

  cpu_set_gpr(decoded.rD, result);

  return 1;
}

// ADR - add an immpediate to PC
uint32_t adr(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("adr r%u, PC, #0x%02X\n", decoded.rD, decoded.imm);

  uint32_t opA = cpu_get_pc();
  // Align PC to 4 bytes
  opA = opA & 0xFFFFFFFC;
  uint32_t opB = zeroExtend32(decoded.imm << 2);
  uint32_t result = opA + opB;

  cpu_set_gpr(decoded.rD, result);

  return 1;
}

///--- Subtract operations --------------------------------------------///

uint32_t subs_i3(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("subs r%u, r%u, #0x%X\n", decoded.rD, decoded.rN, decoded.imm);

  uint32_t opA = cpu_get_gpr(decoded.rN);
  uint32_t opB = ~zeroExtend32(decoded.imm);
  uint32_t result = opA + opB + 1;

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);
  do_cflag(opA, opB, 1);
  do_vflag(opA, opB, result);

  return 1;
}

uint32_t subs_i8(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("subs r%u, #0x%02X\n", decoded.rD, decoded.imm);

  uint32_t opA = cpu_get_gpr(decoded.rD);
  uint32_t opB = ~zeroExtend32(decoded.imm);
  uint32_t result = opA + opB + 1;

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);
  do_cflag(opA, opB, 1);
  do_vflag(opA, opB, result);

  return 1;
}

uint32_t subs(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("subs r%u, r%u, r%u\n", decoded.rD, decoded.rN, decoded.rM);

  uint32_t opA = cpu_get_gpr(decoded.rN);
  uint32_t opB = ~cpu_get_gpr(decoded.rM);
  uint32_t result = opA + opB + 1;

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);
  do_cflag(opA, opB, 1);
  do_vflag(opA, opB, result);

  return 1;
}

uint32_t sub_sp(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("sub SP, #0x%02X\n", decoded.imm);

  uint32_t opA = cpu_get_sp();
  uint32_t opB = ~zeroExtend32(decoded.imm << 2);
  uint32_t result = opA + opB + 1;

  cpu_set_sp(result);

  return 1;
}

uint32_t sbcs(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("sbcs r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t opA = cpu_get_gpr(decoded.rD);
  uint32_t opB = ~cpu_get_gpr(decoded.rM);
  uint32_t result = opA + opB + cpu_get_flag_c();

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);
  do_cflag(opA, opB, cpu_get_flag_c());
  do_vflag(opA, opB, result);

  return 1;
}

uint32_t rsbs(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("rsbs r%u, r%u\n, #0", decoded.rD, decoded.rN);

  uint32_t opA = 0;
  uint32_t opB = ~(cpu_get_gpr(decoded.rN));
  uint32_t result = opA + opB + 1;

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);
  do_cflag(opA, opB, 1);
  do_vflag(opA, opB, result);

  return 1;
}

///--- Multiply operations --------------------------------------------///

// MULS - multiply the source and destination and store 32-bits in dest
// Does not update carry or overflow: simple mult
uint32_t muls(DECODE_RESULT decoded)
{
  TRACE_INSTRUCTION("muls r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t opA = cpu_get_gpr(decoded.rD);
  uint32_t opB = cpu_get_gpr(decoded.rM);
  uint32_t result = opA * opB;

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);

  return 32;
}
