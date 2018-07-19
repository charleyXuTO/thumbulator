#include "thumbulator/memory.hpp"
#include "cpu_flags.hpp"
#include "trace.hpp"
#include "utils.hpp"

namespace thumbulator {

// RRC - rotate right through carry
uint32_t rrc(decode_result const *decoded)
{
  TRACE_INSTRUCTION("rrc %s%u\n", addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  uint16_t opA = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte);
  uint16_t carry = cpu_get_flag_c();
  uint16_t lsb = opA & 0x1;
  int16_t result = opA >> 1;
  result |= (carry << 15);
  cpu_set_flag_c(lsb);

  // update result & flags
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, result);
  do_nflag(result);
  do_zflag(result);
  cpu_set_flag_v(0);

  ////// update Rs if it's in autoincrement mode
  ////updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getSingleOperandCycleCount(decoded);
}

// SWPB - swap bytes
uint32_t swpb(decode_result const *decoded)
{
  TRACE_INSTRUCTION("swpb %s%u\n", addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  uint16_t opA = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte);
  uint16_t result = (opA >> 8) | ((opA & 0xFF)<<8);

  // update result
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, result);

  ////// update Rs if it's in autoincrement mode
  ////updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getSingleOperandCycleCount(decoded);
}

// RRA - rotate right arithmetically
uint32_t rra(decode_result const *decoded)
{
  TRACE_INSTRUCTION("rra %s%u\n", addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  int16_t opA = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte);
  uint16_t lsb = opA & 0x1;
  int16_t result = (opA >> 1) | (opA & 0x8000);
  cpu_set_flag_c(lsb);

  // update result & flags
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, result);
  do_nflag(result);
  do_zflag(result);
  cpu_set_flag_v(0);

  ////// update Rs if it's in autoincrement mode
  ////updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getSingleOperandCycleCount(decoded);
}

// SXT - extend sign
uint32_t sxt(decode_result const *decoded)
{
  TRACE_INSTRUCTION("sxt %s%u\n", addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  int16_t opA = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte);
  int16_t result = (opA & 0x80)?(0xFF00 | opA):(0xFF & opA);

  // update result
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, result);

  ////// update Rs if it's in autoincrement mode
  ////updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getSingleOperandCycleCount(decoded);
}

// PUSH
uint32_t push(decode_result const *decoded)
{
  TRACE_INSTRUCTION("push %s%u\n", addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  int16_t opA = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte);
  uint32_t sp = cpu_get_sp() -2;
  cpu_set_sp(sp);

  // update result
  if(decoded->isByte) {
    //TODO: this would effect load count?? doublt check false_read
    uint16_t oldVal;
    uint16_t false_read = 1;
    load(sp, &oldVal, false_read);
    opA = (opA & 0xFF) | (oldVal & 0xFF00);
  }
  store(sp, opA);

  ////// update Rs if it's in autoincrement mode
  ////updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getSingleOperandCycleCount(decoded);
}

// CALL
uint32_t call(decode_result const *decoded)
{
  TRACE_INSTRUCTION("call %s%u\n", addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  uint16_t opA = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte);
  uint32_t sp = cpu_get_sp() -2;
  cpu_set_sp(sp);
  uint32_t pc = cpu_get_pc();
  store(sp, pc);

  // update result
  cpu_set_pc(opA);

  ////// update Rs if it's in autoincrement mode
  ////updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getSingleOperandCycleCount(decoded);
}

// RETI - return from interrupt
uint32_t reti(decode_result const *decoded)
{
  TRACE_INSTRUCTION("reti %s%u\n", addrModeString[decoded->Ad].c_str(), decoded->Rd);

  uint32_t sp = cpu_get_sp();
  // restore SR
  uint16_t sr;
  load(sp, &sr, 0);
  cpu_set_sr(sr);
  sp +=2;
  cpu_set_sp(sp);

  // restore PC
  uint16_t pc;
  load(sp, &pc, 0);
  cpu_set_pc(pc);
  sp +=2;
  cpu_set_sp(sp);

  return getSingleOperandCycleCount(decoded);
}


}
