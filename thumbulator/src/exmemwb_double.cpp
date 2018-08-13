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
  int32_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte, decoded->isAddrWord,true);
//  int32_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
  uint32_t result = opA;

  // hack to identify exit
  if(0==decoded->Rd) {
    if(cpu_get_pc()==(result + 4)) {
      EXIT_INSTRUCTION_ENCOUNTERED = true;
    }
  }

  // update result & flags
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);

  if (!decoded->isAddrWord) {
    do_nflag(result);
    do_zflag(result);
    cpu_set_flag_c(!(cpu_get_flag_z()));
    cpu_set_flag_v(0);
  }
  else {
    do_nflagx(result);
    do_zflagx(result);
    cpu_set_flag_c(!(cpu_get_flag_z()));
    cpu_set_flag_v(0);
  }

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
  int32_t opA;
  int32_t opB;
  int32_t result = 0;
  // compute
  if (decoded->extended) {
      for (int n = 0 ; n < decoded->n; n++) {
          opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte, decoded->isAddrWord, true);
          opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
          result = opA + opB;
          setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);
      }
  }
  else {
      opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte, decoded->isAddrWord, true);
      opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
      result = opA + opB;
  }

  // update result & flags
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);
  if (!decoded->isAddrWord) {
    do_nflag(result);
    do_zflag(result);
    do_cflag(opA, opB, 0);
    do_vflag(opA, opB, result);
  }
  else {
    do_nflagx(result);
    do_zflagx(result);
    do_cflagx(opA, opB, 0);
    do_vflagx(opA, opB, result);
  }

  // update Rs if it's in autoincrement mode
  updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getDoubleOperandCycleCount(decoded);
}

uint32_t addc(decode_result const *decoded)
{
  TRACE_INSTRUCTION("addc %s%u, %s%u\n", addrModeString[decoded->As].c_str(), decoded->Rs,
                                        addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  uint32_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte, decoded->isAddrWord, true);
  uint32_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
  uint32_t carry = cpu_get_flag_c();
  uint32_t result = opA + opB + carry;

  // update result & flags
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);

  if (!decoded->isAddrWord) {
    do_nflag(result);
    do_zflag(result);
    do_cflag(opA, opB, 0);
    do_vflag(opA, opB, result);
  }
  else {
    do_nflagx(result);
    do_zflagx(result);
    do_cflagx(opA, opB, 0);
    do_vflagx(opA, opB, result);
  }

  // update Rs if it's in autoincrement mode
  updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getDoubleOperandCycleCount(decoded);
}

uint32_t dadd(decode_result const *decoded)
{
  TRACE_INSTRUCTION("dadd %s%u, %s%u\n", addrModeString[decoded->As].c_str(), decoded->Rs,
                                        addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  uint32_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte, decoded->isAddrWord, true);
  uint32_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);

  uint16_t carry = cpu_get_flag_c();
  uint32_t result = (opA & 0xFF) + ((opA & 0xFF00)>>8)*10 +
                    (opB & 0xFF) + ((opB & 0xFF00)>>8)*10 +
                    carry;

  // update result & flags
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);
  cpu_set_gpr(decoded->Rd, result);

  if (!decoded->isAddrWord) {
    do_nflag(result);
    do_zflag(result);
    // C set if word > 9999h, byte > 99h
    if (result > 0x9999) {
      cpu_set_flag_c(1);
    }
  }
  else {
    do_nflagx(result);
    do_zflagx(result);
    if (result > 0x9999) { // TODO: check if you need to reset the flag if its smaller
      cpu_set_flag_c(1);
    }
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
  int32_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte, decoded->isAddrWord, true);

  int32_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);

  opA = ~(opA);
  int32_t result = opA + opB + 1;

  // update result & flags
  cpu_set_gpr(decoded->Rd, result);
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);
  if (!decoded->isAddrWord) {
    do_nflag(result);
    do_zflag(result);
    do_cflag(opA, opB, 1);
    do_vflag((opA), opB, result);
  }
  else {
    do_nflagx(result);
    do_zflagx(result);
    do_cflagx(opA, opB, 1);
    do_vflagx((opA), opB, result);
  }

  // update Rs if it's in autoincrement mode
  updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getDoubleOperandCycleCount(decoded);
}

uint32_t subc(decode_result const *decoded)
{
  TRACE_INSTRUCTION("subc %s%u, %s%u\n", addrModeString[decoded->As].c_str(), decoded->Rs,
                                        addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  int32_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte, decoded->isAddrWord, true);
  int32_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);

  opA = ~(opA);
  uint16_t carry = cpu_get_flag_c();
  int32_t result = opA + opB + carry;

  // update result & flags
  cpu_set_gpr(decoded->Rd, result);
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);

  if (!decoded->isAddrWord) {
    do_nflag(result);
    do_zflag(result);
    do_cflag(opA, opB, carry);
    do_vflag(opA, opB, result);
  }
  else {
    do_nflagx(result);
    do_zflagx(result);
    do_cflagx(opA, opB, carry);
    do_vflagx(opA, opB, result);
  }

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
  int32_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte, decoded->isAddrWord, true);
  int32_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);

  if (!decoded->isAddrWord) {
    if (opA >> 15 > 0) {
      opA = (int16_t)opA;
    }
    if (opB >> 15 >0 ) {
      opB = (int16_t)opB;
    }
  }
  else {
    if (opA >> 19 >0 ) {
        opA = (opA & 0xFFFF) - (opA & 0x1FFFFF);
    }
    if (opB >> 19 >0) {
        opB = (opB & 0xFFFF) - (opB & 0x1FFFFF);
    }

    printf("needs to finish"); //TODO: needs to do the negative version of that
  }

  opA = ~(opA);
  int32_t result = opA + opB + 1;

  // update flags
  if (!decoded->isAddrWord) {
    do_nflag(result);
    do_zflag(result);
    do_cflag(opA, opB, 1);
    do_vflag(opA, opB, result);
  }
  else {
    do_nflagx(result);
    do_zflagx(result);
    do_cflag(opA, opB, 1);
    do_vflagx(opA, opB, result);
  }

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
  int32_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte, decoded->isAddrWord, true);
  int32_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
  int32_t result = opA & opB;

  // update result & flags
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);

  if (!decoded->isAddrWord) {
    do_nflag(result);
    do_zflag(result);
    cpu_set_flag_c(!(cpu_get_flag_z()));
    cpu_set_flag_v(0);
  }
  else {
    do_nflagx(result);
    do_zflagx(result);
    cpu_set_flag_c(!(cpu_get_flag_z()));
    cpu_set_flag_v(0);
  }

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
  int32_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte, decoded->isAddrWord, true);
  int32_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
  int32_t result = ~opA & opB;

  // update result & flags not affected
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);

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
  int32_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte, decoded->isAddrWord, true);
  int32_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
  int32_t result = opA | opB;

  // update result & flags not affected
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);

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
  int32_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte, decoded->isAddrWord, true);
  int32_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
  int32_t result = opA ^ opB;

  // update result & flags
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);
  if (!decoded->isAddrWord) {

    do_nflag(result);
    do_zflag(result);
    cpu_set_flag_c(!(cpu_get_flag_z()));
    cpu_set_flag_v((opA >> 15) & (opB >> 15));
  }
  else {
    do_nflagx(result);
    do_zflagx(result);
    cpu_set_flag_c(!(cpu_get_flag_z()));
    cpu_set_flag_v((opA >> 19) & (opB >> 19));
  }

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
  int32_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte, decoded->isAddrWord, true);
  int32_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
  int32_t result = opA & opB;

  // update result & flags
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);
  if (!decoded->isAddrWord) {
    do_nflag(result);
    do_zflag(result);
    cpu_set_flag_c(!(cpu_get_flag_z()));
    cpu_set_flag_v(0);
  }
  else {
    do_nflagx(result);
    do_zflagx(result);
    cpu_set_flag_c(!(cpu_get_flag_z()));
    cpu_set_flag_v(0);
  }

  // update Rs if it's in autoincrement mode
  updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getDoubleOperandCycleCount(decoded);
}

}
