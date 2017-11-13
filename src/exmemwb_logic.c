#include "exmemwb.h"
#include "decode.h"

///--- Logical operations ----------------------------------------///

// AND - logical AND two registers and update flags
uint32_t ands(decode_result decoded)
{
  TRACE_INSTRUCTION("ands r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t opA = cpu_get_gpr(decoded.rD);
  uint32_t opB = cpu_get_gpr(decoded.rM);
  uint32_t result = opA & opB;

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);

  return 1;
}

// BIC - clears the bits in the destination register that are set in
// the source register
uint32_t bics(decode_result decoded)
{
  TRACE_INSTRUCTION("bics r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t opA = cpu_get_gpr(decoded.rD);
  uint32_t opB = cpu_get_gpr(decoded.rM);
  uint32_t result = opA & ~opB;

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);

  return 1;
}

// EOR - exclusive OR two registers and update the flags
uint32_t eors(decode_result decoded)
{
  TRACE_INSTRUCTION("eors r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t opA = cpu_get_gpr(decoded.rD);
  uint32_t opB = cpu_get_gpr(decoded.rM);
  uint32_t result = opA ^ opB;

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);

  return 1;
}

// ORR - logical OR two registers and update the flags
uint32_t orrs(decode_result decoded)
{
  TRACE_INSTRUCTION("orrs r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t opA = cpu_get_gpr(decoded.rD);
  uint32_t opB = cpu_get_gpr(decoded.rM);
  uint32_t result = opA | opB;

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);

  return 1;
}

// MVN - Move while negating
uint32_t mvns(decode_result decoded)
{
  TRACE_INSTRUCTION("mvns r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t opA = cpu_get_gpr(decoded.rM);
  uint32_t result = ~opA;

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);

  return 1;
}

///--- Shift and rotate operations --------------------------------------------///

uint32_t asrs_i(decode_result decoded)
{
  TRACE_INSTRUCTION("asrs r%u, r%u, #%d\n", decoded.rD, decoded.rM, decoded.imm);

  uint32_t opA = cpu_get_gpr(decoded.rM);
  uint32_t opB = decoded.imm;
  uint32_t result;

  // 0 really means 32 (A6.4.1)
  if(opB == 0) {
    opB = 32;
    // Special handling of negative numbers
    result = ((opA & 0x80000000) != 0) ? ~0 : 0;
  } else {
    // Special handling of negative numbers
    if((opA & 0x80000000) != 0) {
      result = (opA >> opB) | (0xFFFFFFFF << (32 - opB));
    } else {
      result = opA >> opB;
    }

    cpu_set_flag_c((opA >> (opB - 1)) & 0x1);
  }

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);

  return 1;
}

uint32_t asrs_r(decode_result decoded)
{
  TRACE_INSTRUCTION("asrs r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t opA = cpu_get_gpr(decoded.rD);
  uint32_t opB = cpu_get_gpr(decoded.rM) & 0xFF;
  uint32_t result = 0;

  if(opB == 0) {
    result = opA;
  } else {
    if(opB < 32) {
      if((opA & 0x80000000) != 0) {
        result = (opA >> opB) | (0xFFFFFFFF << (32 - opB));
      } else {
        result = opA >> opB;
      }
    } else {
      result = ((opA & 0x80000000) != 0) ? ~0 : 0;
    }

    cpu_set_flag_c((opB >= 32) ? (opA >> 31) : (opA >> (opB - 1)) & 0x1);
  }

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);

  return 1;
}

uint32_t lsls_i(decode_result decoded)
{
  if(decoded.imm == 0)
    TRACE_INSTRUCTION("mov r%u, r%u\n", decoded.rD, decoded.rM);
  else
    TRACE_INSTRUCTION("lsls r%u, r%u, #%d\n", decoded.rD, decoded.rM, decoded.imm);

  uint32_t opA = cpu_get_gpr(decoded.rM);
  uint32_t opB = decoded.imm;
  uint32_t result = opA << opB;

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);
  cpu_set_flag_c((opB == 0) ? cpu_get_flag_c() : (opA << (opB - 1)) >> 31);

  return 1;
}

uint32_t lsrs_i(decode_result decoded)
{
  TRACE_INSTRUCTION("lsrs r%u, r%u, #%d\n", decoded.rD, decoded.rM, decoded.imm);

  uint32_t opA = cpu_get_gpr(decoded.rM);
  uint32_t opB = decoded.imm;
  // 0 really means 32 (A6.4.1)
  uint32_t result = opB ? opA >> opB : 0;

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);
  cpu_set_flag_c((opB == 0) ? 0 : (opA >> (opB - 1)) & 0x1);

  return 1;
}

uint32_t lsls_r(decode_result decoded)
{
  TRACE_INSTRUCTION("lsls r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t opA = cpu_get_gpr(decoded.rD);
  uint32_t opB = cpu_get_gpr(decoded.rM) & 0xFF;
  uint32_t result = (opB >= 32) ? 0 : opA << opB;

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);
  cpu_set_flag_c((opB == 0) ? cpu_get_flag_c() : (opB > 32) ? 0 : (opA << (opB - 1)) >> 31);

  return 1;
}

uint32_t lsrs_r(decode_result decoded)
{
  TRACE_INSTRUCTION("lsrs r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t opA = cpu_get_gpr(decoded.rD);
  uint32_t opB = cpu_get_gpr(decoded.rM) & 0xFF;
  uint32_t result = (opB >= 32) ? 0 : opA >> opB;

  cpu_set_gpr(decoded.rD, result);

  do_nflag(result);
  do_zflag(result);
  cpu_set_flag_c((opB == 0) ? cpu_get_flag_c() : (opB > 32) ? 0 : (opA >> (opB - 1)) & 0x1);

  return 1;
}

uint32_t rors(decode_result decoded)
{
  TRACE_INSTRUCTION("rors r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t opA = cpu_get_gpr(decoded.rD);
  uint32_t opB = cpu_get_gpr(decoded.rM) & 0xFF;

  uint32_t result = opA;
  if(opB != 0) {
    opB &= 0x1F; // Everything above 32 is a multiple of 32
    result = (opB == 0) ? opA : opA >> opB | opA << (32 - opB);
    cpu_set_flag_c((result >> 31) & 0x1);
    cpu_set_gpr(decoded.rD, result);
  }

  do_nflag(result);
  do_zflag(result);

  return 1;
}
