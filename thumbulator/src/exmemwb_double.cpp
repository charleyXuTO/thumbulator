#include "thumbulator/memory.hpp"
#include "cpu_flags.hpp"
#include "trace.hpp"
#include "utils.hpp"

namespace thumbulator {

///--- Move operations -------------------------------------------///
// MOV
uint32_t mov(decode_result const *decoded)
{
  TRACE_INSTRUCTION("mov %s%u, %s%u\n", addrModeString[decoded->As].c_str(), decoded->Rs,
                                        addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  int16_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte);
  int16_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte);
  int16_t result = opA;

  // update result & flags
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, result);
  do_nflag(result);
  do_zflag(result);
  cpu_set_flag_c(!(cpu_get_flag_z()));
  cpu_set_flag_v(0);

  // update Rs if it's in autoincrement mode
  updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getDoubleOperandCycleCount(decoded);
}

///--- Add operations --------------------------------------------///

// ADD
uint32_t add(decode_result const *decoded)
{
  TRACE_INSTRUCTION("add %s%u, %s%u\n", addrModeString[decoded->As].c_str(), decoded->Rs,
                                        addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  int16_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte);
  int16_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte);
  int16_t result = opA + opB;

  // update result & flags
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, result);
  do_nflag(result);
  do_zflag(result);
  do_cflag(opA, opB, 0);
  do_vflag(opA, opB, result);

  // update Rs if it's in autoincrement mode
  updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getDoubleOperandCycleCount(decoded);
}

uint32_t addc(decode_result const *decoded)
{
  TRACE_INSTRUCTION("addc %s%u, %s%u\n", addrModeString[decoded->As].c_str(), decoded->Rs,
                                        addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  uint16_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte);
  uint16_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte);
  uint16_t carry = cpu_get_flag_c();
  uint16_t result = opA + opB + carry;

  // update result & flags
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, result);
  do_nflag(result);
  do_zflag(result);
  do_cflag(opA, opB, 0);
  do_vflag(opA, opB, result);

  // update Rs if it's in autoincrement mode
  updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getDoubleOperandCycleCount(decoded);
}

uint32_t dadd(decode_result const *decoded)
{
  TRACE_INSTRUCTION("dadd %s%u, %s%u\n", addrModeString[decoded->As].c_str(), decoded->Rs,
                                        addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  uint16_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte);
  uint16_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte);
  uint16_t carry = cpu_get_flag_c();
  uint16_t result = (opA & 0xFF) + ((opA & 0xFF00)>>8)*10 +
                    (opB & 0xFF) + ((opB & 0xFF00)>>8)*10 +
                    carry;

  // update result & flags
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, result);
  cpu_set_gpr(decoded->Rd, result);
  do_nflag(result);
  do_zflag(result);
  // C set if word > 9999h, byte > 99h
  if(result > 0x9999) {
    cpu_set_flag_c(1);
  }
  // V undefined
  //do_vflag(opA, opB, result);

  // update Rs if it's in autoincrement mode
  updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getDoubleOperandCycleCount(decoded);
}

///--- Subtract operations --------------------------------------------///

uint32_t sub(decode_result const *decoded)
{
  TRACE_INSTRUCTION("sub %s%u, %s%u\n", addrModeString[decoded->As].c_str(), decoded->Rs,
                                        addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  int16_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte);
  opA = ~(opA);
  int16_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte);
  int16_t result = opA + opB + 1;

  // update result & flags
  cpu_set_gpr(decoded->Rd, result);
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, result);
  do_nflag(result);
  do_zflag(result);
  do_cflag(opA, opB, 1);
  do_vflag(opA, opB, result);

  // update Rs if it's in autoincrement mode
  updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getDoubleOperandCycleCount(decoded);
}

uint32_t subc(decode_result const *decoded)
{
  TRACE_INSTRUCTION("subc %s%u, %s%u\n", addrModeString[decoded->As].c_str(), decoded->Rs,
                                        addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  int16_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte);
  opA = ~(opA);
  int16_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte);
  uint16_t carry = cpu_get_flag_c();
  int16_t result = opA + opB + carry;

  // update result & flags
  cpu_set_gpr(decoded->Rd, result);
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, result);
  do_nflag(result);
  do_zflag(result);
  do_cflag(opA, opB, carry);
  do_vflag(opA, opB, result);

  // update Rs if it's in autoincrement mode
  updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getDoubleOperandCycleCount(decoded);
}

///--- Compare operations --------------------------------------------///

uint32_t cmp(decode_result const *decoded)
{
  TRACE_INSTRUCTION("cmp %s%u, %s%u\n", addrModeString[decoded->As].c_str(), decoded->Rs,
                                        addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  int16_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte);
  opA = ~(opA);
  int16_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte);
  int16_t result = opA + opB + 1;

  // update flags
  do_nflag(result);
  do_zflag(result);
  do_cflag(opA, opB, 1);
  do_vflag(opA, opB, result);

  // update Rs if it's in autoincrement mode
  updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getDoubleOperandCycleCount(decoded);
}


///--- Bitwise operations --------------------------------------------///
// BIT - test the bits in the destination register that are set in
// the source register
uint32_t bit(decode_result const *decoded)
{
  TRACE_INSTRUCTION("bit %s%u, %s%u\n", addrModeString[decoded->As].c_str(), decoded->Rs,
                                        addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  int16_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte);
  int16_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte);
  int16_t result = opA & opB;

  // update result & flags
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, result);
  do_nflag(result);
  do_zflag(result);
  cpu_set_flag_c(!(cpu_get_flag_z()));
  cpu_set_flag_v(0);

  // update Rs if it's in autoincrement mode
  updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getDoubleOperandCycleCount(decoded);
}

// BIC - clears the bits in the destination register that are set in
// the source register
uint32_t bic(decode_result const *decoded)
{
  TRACE_INSTRUCTION("bic %s%u, %s%u\n", addrModeString[decoded->As].c_str(), decoded->Rs,
                                        addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  int16_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte);
  int16_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte);
  int16_t result = ~opA & opB;

  // update result & flags not affected
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, result);

  // update Rs if it's in autoincrement mode
  updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getDoubleOperandCycleCount(decoded);
}

// BIS - set the bits in the destination register that are set in
// the source register
uint32_t bis(decode_result const *decoded)
{
  TRACE_INSTRUCTION("bis %s%u, %s%u\n", addrModeString[decoded->As].c_str(), decoded->Rs,
                                        addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  int16_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte);
  int16_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte);
  int16_t result = opA | opB;

  // update result & flags not affected
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, result);

  // update Rs if it's in autoincrement mode
  updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getDoubleOperandCycleCount(decoded);
}

// XOR - XOR two registers and update flags
uint32_t xorOp(decode_result const *decoded)
{
  TRACE_INSTRUCTION("xor %s%u, %s%u\n", addrModeString[decoded->As].c_str(), decoded->Rs,
                                        addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  int16_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte);
  int16_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte);
  int16_t result = opA ^ opB;

  // update result & flags
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, result);
  do_nflag(result);
  do_zflag(result);
  cpu_set_flag_c(!(cpu_get_flag_z()));
  cpu_set_flag_v((opA>>15) & (opB>>15));

  // update Rs if it's in autoincrement mode
  updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getDoubleOperandCycleCount(decoded);
}

// AND - logical AND two registers and update flags
uint32_t andOp(decode_result const *decoded)
{
  TRACE_INSTRUCTION("and %s%u, %s%u\n", addrModeString[decoded->As].c_str(), decoded->Rs,
                                        addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  int16_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte);
  int16_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte);
  int16_t result = opA & opB;

  // update result & flags
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, result);
  do_nflag(result);
  do_zflag(result);
  cpu_set_flag_c(!(cpu_get_flag_z()));
  cpu_set_flag_v(0);

  // update Rs if it's in autoincrement mode
  updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getDoubleOperandCycleCount(decoded);
}

}
