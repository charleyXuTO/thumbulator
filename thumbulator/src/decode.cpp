#include "thumbulator/decode.hpp"

#include "thumbulator/cpu.hpp"
#include "thumbulator/memory.hpp"

#include "cpu_flags.hpp"
#include "exit.hpp"

namespace thumbulator {

bool isIndirectAutoIncrement(uint8_t As, uint8_t Rsrc) {
  // indirect autoincrement uses @Rn+, immediate uses @PC+ (both As==3)
  return (As==0x3 && (Rsrc!=GPR_PC));
}

// Various decodings

// Double operand (format I)
// |15    12|11    8| 7  |  6  |5  4|3     0|
// |----------------------------------------|
// | opcode | R_src | Ad | B/W | As | R_dst |
// |----------------------------------------|
// | src or dst                             |
// |----------------------------------------|
// | dst                                    |
// |----------------------------------------|
decode_result decode_double(const uint16_t pInsn)
{
  decode_result decoded;

  uint16_t opcode = (pInsn & 0xF000);
  uint8_t Rsrc = (pInsn >> 8) & 0xF;
  uint8_t Rdst = pInsn & 0xF;
  uint8_t As = (pInsn >> 4) & 0x3;
  uint8_t Ad = (pInsn >> 7) & 0x1;
  bool isByte = (pInsn & 0x40)==1;
  bool isAddrWord = false;
  uint16_t srcWord = 0;
  uint16_t dstWord = 0;

  // [1] fetch any extra word needed for src
  if(As & 0x1) { // indexed, symbolic, absolute, indirect autoincrement, immediate
    // indirect autoincrement doesn't need next word
    if(!isIndirectAutoIncrement(As, Rsrc)) {
        fetch_instruction(cpu_get_pc(), &srcWord);
        cpu_set_pc(cpu_get_pc() + 0x2);
    }
  }

  // [2] fetch any extra word needed for dst
  if (Ad==0x1) { // indexed, symbolic, absolute
    fetch_instruction(cpu_get_pc(), &dstWord);
    cpu_set_pc(cpu_get_pc() + 0x2);
  }

  // [3] set decode structure
  decoded.opcode = opcode;
  decoded.Rd = Rdst;
  decoded.Rs = Rsrc;
  decoded.As = As;
  decoded.Ad = Ad;
  decoded.isByte = isByte;
  decoded.isAddrWord = isAddrWord;
  decoded.srcWord = srcWord;
  decoded.dstWord = dstWord;

  return decoded;
}


// Single operand (format II)
// |15     7|  6  |5  4|3     0|
// |---------------------------|
// | opcode | B/W | Ad | R_dst |
// |---------------------------|
// | dst                       |
// |---------------------------|
decode_result decode_single(const uint16_t pInsn)
{
  decode_result decoded;

  uint16_t opcode = (pInsn & 0xFF80);
  uint8_t Rdst = pInsn & 0xF;
  uint8_t Ad = (pInsn >> 4) & 0x3;
  bool isByte = (pInsn & 0x40)==0x40;
  bool isAddrWord = false;
  uint16_t dstWord = 0;

  // [1] fetch any extra word needed (indexed, symbolic, absolute, immediate)
  if(Ad & 0x1) { // indexed, symbolic, absolute, indirect autoincrement, immediate
    // indirect autoincrement doesn't need next word
    if(!isIndirectAutoIncrement(Ad, Rdst)) {
        fetch_instruction(cpu_get_pc(), &dstWord);
        cpu_set_pc(cpu_get_pc() + 0x2);
    }
  }

  // [3] set decode structure
  decoded.opcode = opcode;
  decoded.Rd = Rdst;
  decoded.Ad = Ad;
  decoded.isByte = isByte;
  decoded.isAddrWord = isAddrWord;
  decoded.dstWord = dstWord;

  return decoded;
}

// Jump
// |15    13|12   10| 9 |8                0|
// |---------------------------------------|
// | opcode | Cond  | S | signed PC offset |
// |---------------------------------------|
decode_result decode_jump(const uint16_t pInsn)
{
  decode_result decoded;

  uint16_t opcode = (pInsn & 0xFC00);
  int16_t offset = (pInsn & 0x03FF);
  bool isNeg = (pInsn & 0x200);

  decoded.opcode = opcode;
  decoded.offset = (isNeg)?(0xFC00 | offset):(offset); // sign-extend

  return decoded;
}

// Stop simulation if we cannot decode the instruction
decode_result decode_error(const uint16_t pInsn)
{
  fprintf(stderr, "Error: Malformed instruction: Unable to decode: 0x%4.4X at 0x%08X\n", pInsn,
      cpu_get_pc() - 2);
  terminate_simulation(1);
}

// Use a table of function pointers indexed by the instruction
// to make decoding fast
decode_result (*decodeJumpTable[16])(const uint16_t pInsn) = {
  decode_error,     // 0
  decode_single,    // 1
  decode_jump,      // 2
  decode_jump,      // 3
  decode_double,    // 4
  decode_double,    // 5
  decode_double,    // 6
  decode_double,    // 7
  decode_double,    // 8
  decode_double,    // 9
  decode_double,    // A
  decode_double,    // B
  decode_double,    // C
  decode_double,    // D
  decode_double,    // E
  decode_double     // F
  };

// Decoding is a matter of indexing the decode jump tabel
// using the first 4 instruction opcode bits and then
// executing the function pointed to
// The decode functions update the global decode structure
decode_result decode(const uint16_t instruction)
{
  return decodeJumpTable[instruction >> 12](instruction);
}
}
