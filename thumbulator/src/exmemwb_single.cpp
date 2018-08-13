#include "thumbulator/memory.hpp"
#include "cpu_flags.hpp"
#include "trace.hpp"
#include "utils.hpp"

namespace thumbulator {

// RRC - rotate right through carry
uint32_t rrc(decode_result const *decoded)
{
  TRACE_INSTRUCTION("rrc %s%u\n", addrModeString[decoded->Ad].c_str(), decoded->Rd);
  int result;
  // compute
  uint32_t opA = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
  uint32_t carry = cpu_get_flag_c();
  uint16_t lsb = opA & 0x1;
  if (!decoded->extended) {
      result = opA >> 1;
  }
  else {
      result = opA >> decoded->n;
  }

  if (!decoded->isAddrWord) {
    result |= (carry << 15);
  }
  else {
    result |= (carry << 19);
  }
  cpu_set_flag_c(lsb);

  // update result & flags
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);

  if (!decoded->isAddrWord) {
      do_nflag(result);
      do_zflag(result);
      cpu_set_flag_v(0);
  }
  else {
      do_nflagx(result);
      do_zflagx(result);
      cpu_set_flag_v(0);
  }



  ////// update Rs if it's in autoincrement mode
  ////updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getSingleOperandCycleCount(decoded);
}

// SWPB - swap bytes
uint32_t swpb(decode_result const *decoded)
{
  TRACE_INSTRUCTION("swpb %s%u\n", addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  uint32_t opA = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
  uint32_t result;

  if (!decoded->isAddrWord) {
    result = (opA >> 8) | ((opA & 0xFF) << 8);
  }
  else {
    result = ((opA & 0xFFF) >> 8) |  ((opA & 0xFF) << 8);
  }

  // update resultSWPB
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);

  ////// update Rs if it's in autoincrement mode
  ////updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getSingleOperandCycleCount(decoded);
}

// RRA - rotate right arithmetically
uint32_t rra(decode_result const *decoded)
{
  TRACE_INSTRUCTION("rra %s%u\n", addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  int32_t result;
  int32_t opA = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
  uint16_t lsb = opA & 0x1;
  if (!decoded->extended) {
     result = (opA >> 1) | (opA & 0x8000);
  }
  else {
      for (int i = 0; i<decoded->n; i++ ) {
          opA = getValue (decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
          result = (opA >> 1) | (opA & 0x8000);
          setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);
      }
  }

  cpu_set_flag_c(lsb);

  // update result & flags
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);
  if (!decoded->isAddrWord) {
    do_nflag(result);
    do_zflag(result);
    cpu_set_flag_v(0);
  }
  else {
    do_nflagx(result);
    do_zflagx(result);
    cpu_set_flag_v(0);
  }

  ////// update Rs if it's in autoincrement mode
  ////updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getSingleOperandCycleCount(decoded);
}

// SXT - extend sign
uint32_t sxt(decode_result const *decoded) // TODO: Needs to re-read the document again to figure out what this does NOT DONE
{
  TRACE_INSTRUCTION("sxt %s%u\n", addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  int32_t opA = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
  int32_t result = (opA & 0x80)?(0xFF00 | opA):(0xFF & opA);

  // update result
  setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);

  ////// update Rs if it's in autoincrement mode
  ////updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getSingleOperandCycleCount(decoded);
}

// PUSH
uint32_t push(decode_result const *decoded)
{
  TRACE_INSTRUCTION("push %s%u\n", addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  int32_t opA = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
  uint32_t sp;

  if (!decoded->isAddrWord) {
    sp = cpu_get_sp() - 2;
    cpu_set_sp(sp);
    store(sp, opA, decoded->isByte);
  }
  else {
    sp = cpu_get_sp() -2;
    cpu_set_sp(sp);
    store(sp, (opA >> 16) , decoded->isByte);
    sp = cpu_get_sp() - 2;
    cpu_set_sp(sp);
    store(sp, (opA & 0xFFFF), decoded->isByte);
  }

  // update result
  //if(decoded->isByte) {
  //  uint16_t oldVal;
  //  uint16_t false_read = 1;
  //  load(sp, &oldVal, false_read);
  //  opA = (opA & 0xFF) | (oldVal & 0xFF00);
  //}


  ////// update Rs if it's in autoincrement mode
  ////updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);

  return getSingleOperandCycleCount(decoded);
}

// CALL
uint32_t call(decode_result const *decoded)
{
  TRACE_INSTRUCTION("call %s%u\n", addrModeString[decoded->Ad].c_str(), decoded->Rd);

  // compute
  uint32_t opA = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
  uint32_t sp;
  uint32_t pc;
  if (!decoded->isAddrWord) {
    sp = cpu_get_sp() - 2;
    cpu_set_sp(sp);
    pc = cpu_get_pc();
    store(sp, pc, false);
  }
  else {
    sp = cpu_get_sp() - 2;
    cpu_set_sp(sp);
    pc = cpu_get_pc();
    store(sp, pc >> 16, false);
    sp =  cpu_get_sp() -2;
    cpu_set_sp(sp);
    store(sp, (pc & 0xFFFF), false);
  }

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
  uint32_t sr;
  load(sp, &sr, 0);
  cpu_set_sr(sr);
  sp +=2;
  cpu_set_sp(sp);

  // restore PC
  uint32_t pc;
  load(sp, &pc, 0);
  cpu_set_pc(pc);
  sp +=2;
  cpu_set_sp(sp);

  return getSingleOperandCycleCount(decoded);
}


}
