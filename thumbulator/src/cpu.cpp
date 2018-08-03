#include "thumbulator/cpu.hpp"

#include "thumbulator/memory.hpp"
#include "cpu_flags.hpp"
#include "exit.hpp"

#include <cstring>

namespace thumbulator {

uint16_t insn;

bool BRANCH_WAS_TAKEN = false;
bool EXIT_INSTRUCTION_ENCOUNTERED = false;

// Reset CPU state in accordance with B1.5.5 and B3.2.2
void cpu_reset()
{
  // Clear the general purpose registers
  memset(cpu.gpr, 0, sizeof(cpu.gpr));

  // For MSP430, user sets SP at beginning of their program

  // Set the program counter to the address at the reset vector location (0FFFEh).
  uint32_t startAddr;
  // TODO: any other way to do this??
  //RESET_VECTOR = FLASH_MEMORY[12]; // observed from hexdump
  load(0xFFFE, &startAddr, 0);
  cpu_set_pc(startAddr);
}

cpu_state cpu;

uint32_t mov(decode_result const *);
uint32_t add(decode_result const *);
uint32_t addc(decode_result const *);
uint32_t subc(decode_result const *);
uint32_t sub(decode_result const *);
uint32_t cmp(decode_result const *);
uint32_t dadd(decode_result const *);
uint32_t bit(decode_result const *);
uint32_t bic(decode_result const *);
uint32_t bis(decode_result const *);
uint32_t xorOp(decode_result const *);
uint32_t andOp(decode_result const *);

uint32_t rrc(decode_result const *);
uint32_t swpb(decode_result const *);
uint32_t rra(decode_result const *);
uint32_t sxt(decode_result const *);
uint32_t push(decode_result const *);
uint32_t call(decode_result const *);
uint32_t reti(decode_result const *);

uint32_t jne(decode_result const *);
uint32_t jeq(decode_result const *);
uint32_t jnc(decode_result const *);
uint32_t jc(decode_result const *);
uint32_t jn(decode_result const *);
uint32_t jge(decode_result const *);
uint32_t jl(decode_result const *);
uint32_t jmp(decode_result const *);

uint32_t rrum(decode_result const *);
uint32_t pushm(decode_result const *);
uint32_t popm(decode_result const *);


uint32_t exmemwb_error(decode_result const *decoded)
{
  fprintf(stderr, "Error: Unsupported instruction: Unable to execute\n");
  terminate_simulation(1);
  return 0;
}

uint32_t exmemwb_exit_simulation(decode_result const *decoded)
{
  EXIT_INSTRUCTION_ENCOUNTERED = true;

  return 0;
}

uint32_t (*executeJumpTableSingleOp[8])(decode_result const *) = {
    rrc,
    swpb,
    rra,
    sxt,
    push,
    call,
    reti,
    exmemwb_error
};

uint32_t singleOpEntry2(decode_result const *decoded)
{
    return executeJumpTableSingleOp[(insn >> 7) & 0x7](decoded);
}

uint32_t (*executeJumpTableSingleOp2[2])(decode_result const *) = {
      pushm, // pushm
      popm // popm
};

uint32_t singleOpEntry3(decode_result const *decoded) {
   return executeJumpTableSingleOp2[(insn >> 9) & 0x1](decoded);
}

uint32_t (*executeJumpTableSingleOpFirst[2])(decode_result const *) = {
    singleOpEntry2, // regular single op entires
    singleOpEntry3 // popm, pushm
};


uint32_t singleOpEntry(decode_result const *decoded)
{
  return executeJumpTableSingleOpFirst[(insn >> 10) & 0x1](decoded);
}

uint32_t (*executeJumpTableJumpOp[8])(decode_result const *) = {
    jne,
    jeq,
    jnc,
    jc,
    jn,
    jge,
    jl,
    jmp
};

uint32_t jumpEntry(decode_result const *decoded)
{
  return executeJumpTableJumpOp[(insn >> 10) & 0x7](decoded);
}
uint32_t (*executeExtendedTableOp2[4]) (decode_result const *) = {
    exmemwb_error, //rrcm 0
    exmemwb_error, //rram 1
    exmemwb_error, //rlam 2
    rrum  //rrum 3
};
uint32_t extended2(decode_result const *decoded)
{
    return executeExtendedTableOp2[(insn >> 8) & 0x3](decoded);
}


uint32_t (*executeExtendedTableOp[16])(decode_result const *) = {
    exmemwb_error, // mova 0
    exmemwb_error, // mova 1
    exmemwb_error, // mova 2
    exmemwb_error, // mova 3
    extended2,     // RR.. A 4
    extended2,     // RR.. W 5
    exmemwb_error, // mova 6
    exmemwb_error, // mova 7
    exmemwb_error, // mova 8
    exmemwb_error, // cmpa 9
    exmemwb_error, // adda 10
    exmemwb_error, // suba 11
    exmemwb_error, // mova 12
    exmemwb_error, // cmpa 13
    exmemwb_error, // adda 14
    exmemwb_error // suba 15
};

uint32_t extended(decode_result const* decoded)
{
    return executeExtendedTableOp[(insn>>4) & 0xF](decoded);
}

uint32_t (*executeJumpTable[16])(decode_result const *) = {
    extended,
    singleOpEntry,
    jumpEntry,
    jumpEntry,
    mov,
    add,
    addc,
    subc,
    sub,
    cmp,
    dadd,
    bit,
    bic,
    bis,
    xorOp,
    andOp
};

uint32_t exmemwb(uint16_t instruction, decode_result const *decoded)
{
  insn = instruction;
#if 1
  uint32_t insnTicks = executeJumpTable[instruction >> 12](decoded);
#else
  uint32_t insnTicks = 0;
  if(decoded.opcode > 0x3000) { // double operand
    DoubleOpInsn op = decoded.opcode;
    switch(op){
      case MOV:  insnTicks = mov(decoded); break;
      case ADD:  insnTicks = add(decoded); break;
      case ADDC: insnTicks = addc(decoded); break;
      case SUBC: insnTicks = subc(decoded); break;
      case SUB:  insnTicks = sub(decoded); break;
      case CMP:  insnTicks = cmp(decoded); break;
      case DADD: insnTicks = dadd(decoded); break;
      case BIT:  insnTicks = bit(decoded); break;
      case BIC:  insnTicks = bic(decoded); break;
      case BIS:  insnTicks = bis(decoded); break;
      case XOR:  insnTicks = xorOp(decoded); break;
      case AND:  insnTicks = andOp(decoded); break;
      default:
        assert(0 && "unknown double operand instruction!");
    }
  }
  else if ((insn>>12) == 0x1) { // single operand
    SingleOpInsn op = decoded.opcode;
    switch(op){
      case RRC:  insnTicks = rrc(decoded); break;
      case SWPB: insnTicks = swpb(decoded); break;
      case RRA:  insnTicks = rra(decoded); break;
      case SXT:  insnTicks = sxt(decoded); break;
      case PUSH: insnTicks = push(decoded); break;
      case CALL: insnTicks = call(decoded); break;
      case RETI: insnTicks = reti(decoded); break;
      default:
        assert(0 && "unknown single operand instruction!");
    }
  }
  else if ((insn>>12) > 0x1) { // jump
    JumpInsn op = decoded.opcode;
    switch(op){
      case JNE: insnTicks = jne(decoded); break;
      case JEQ: insnTicks = jeq(decoded); break;
      case JNC: insnTicks = jnc(decoded); break;
      case JC:  insnTicks = jc(decoded); break;
      case JN:  insnTicks = jn(decoded); break;
      case JGE: insnTicks = jge(decoded); break;
      case JL:  insnTicks = jl(decoded); break;
      case JMP: insnTicks = jmp(decoded); break;
      default:
        assert(0 && "unknown jump instruction!");
    }
  }
  else {
      assert(0 && "unknown instruction?");
  }
#endif

  return insnTicks;
}

}
