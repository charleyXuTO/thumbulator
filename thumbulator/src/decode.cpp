#include "thumbulator/decode.hpp"

#include "thumbulator/cpu.hpp"
#include "thumbulator/memory.hpp"

#include "cpu_flags.hpp"
#include "exit.hpp"

bool extendedInstruction;

namespace thumbulator {

bool isIndirectAutoIncrement(uint8_t As, uint8_t Rsrc) {
  // indirect autoincrement uses @Rn+, immediate uses @PC+ (both As==3)
  return (As==0x3 && (Rsrc!=GPR_PC));
}

// Various decodings

// Double operand (format I) for regulator cpu instructions
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
  uint16_t opcode = (pInsn & 0xF000); //
  uint8_t Rsrc = (pInsn >> 8) & 0xF;
  uint8_t Rdst = pInsn & 0xF;
  uint8_t As = (pInsn >> 4) & 0x3;
  uint8_t Ad = (pInsn >> 7) & 0x1;
  uint16_t extended;
  uint8_t n =0;

  bool isByte = (pInsn & 0x40)==0x40;
  bool isAddrWord = false;
  uint16_t srcWord150 = 0;
  uint16_t dstWord150 = 0;
  uint16_t srcWord1916 = 0;
  uint16_t dstWord1916 = 0;
  uint32_t srcWord = 0;
  uint32_t dstWord = 0;
  if (extendedInstruction) {
    fetch_instruction(cpu_get_pc()- 0x4, &extended);
    bool al = (extended &0x40) == 0x40;
    n = (extended &0xF) + 1;
    if (al == false && isByte == true) {
      isAddrWord = true;
      isByte = false;
    }
  }

  // [0] check if using CG1 or CG2
  bool useCG = (GPR_CG2 == Rsrc) || ((GPR_CG1 == Rsrc) && (As & 0x2));
  if(GPR_CG1 == Rsrc) {
    if(0x2 == As) srcWord = 0x4;
    else if(0x3 == As) srcWord = 0x8;
  }
  if(GPR_CG2 == Rsrc) {
    if(0x0 == As) srcWord = 0x0;
    else if(0x1 == As) srcWord = 0x1;
    else if(0x2 == As) srcWord = 0x2;
    else if(0x3 == As) srcWord = 0xFFFF;
  }

  // [1] fetch any extra word needed for src
  if(!useCG && (As & 0x1)) { // indexed, symbolic, absolute, indirect autoincrement, immediate
    // indirect autoincrement doesn't need next word
    if(!isIndirectAutoIncrement(As, Rsrc)) {
      if (isAddrWord) {
        srcWord1916 = (extended >> 7) & 0xF;
      }
      fetch_instruction(cpu_get_pc(), &srcWord150);
      cpu_set_pc(cpu_get_pc() + 0x2);
      srcWord = (srcWord1916 & 0xF) <<16 ^ srcWord150;

    }
  }


  // [2] fetch any extra word needed for dst
  if (Ad==0x1) { // indexed, symbolic, absolute
    if (isAddrWord) {
      dstWord1916 = (extended & 0xF);
    }
    fetch_instruction(cpu_get_pc(), &dstWord150);
    cpu_set_pc(cpu_get_pc() + 0x2);
    dstWord = (dstWord1916 & 0xF) <<16 ^ dstWord150;
  }

  // [3] set decode structure
  decoded.opcode = opcode;
  decoded.Rd = Rdst;
  decoded.Rs = Rsrc;
  decoded.As = As;
  decoded.Ad = Ad;
  decoded.isByte = isByte;
  decoded.isAddrWord = isAddrWord;
  decoded.n = n;
  decoded.srcWord = srcWord;
  decoded.dstWord = dstWord;
  decoded.extended = extendedInstruction;
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
  uint16_t opcode;
  uint8_t Rdst;
  uint8_t Ad;
  bool isByte;
  bool isAddrWord;
  uint32_t dstWord = 0;
  uint16_t dstWord1916 = 0;
  uint16_t dstWord150 = 0;
  uint16_t extended;
  uint8_t n = 0;

  if ((pInsn >> 10 & 0x1) == 0x1) { // for pushm and popm
      isByte = false;
      isAddrWord = (pInsn>>8 & 0x1) != 0x1;
      dstWord = pInsn >> 4 & 0xF;
      if ((pInsn >> 9 & 0x1) == 0x1 ) {
          Rdst = (pInsn & 0xF);
      }
      else {
          Rdst = (pInsn & 0xF);
      }
      dstWord = dstWord + 0x1;
  }
  else {
      opcode = (pInsn & 0xFF80);
      Rdst = pInsn & 0xF;
      Ad = (pInsn >> 4) & 0x3;
      isByte = (pInsn & 0x40) == 0x40;
      isAddrWord = false;
      dstWord = 0;
      dstWord1916 = 0;
      dstWord150 = 0;

      if (extendedInstruction) {
          fetch_instruction(cpu_get_pc() - 0x4, &extended);
          bool al = (extended & 0x40) == 0x40;
          n = (extended & 0xF) +1;
          if (al == false && isByte == true) {
              isAddrWord = true;
              isByte = false;
          }
      }
      // [1] fetch any extra word needed (indexed, symbolic, absolute, immediate)
      if (Ad & 0x1) { // indexed, symbolic, absolute, indirect autoincrement, immediate
          // indirect autoincrement doesn't need next word

          if (!isIndirectAutoIncrement(Ad, Rdst)) {
              if (isAddrWord) {
                  dstWord1916 = extended & 0xF;

              }
              fetch_instruction(cpu_get_pc(), &dstWord150);
              cpu_set_pc(cpu_get_pc() + 0x2);
              dstWord = (dstWord1916 & 0xF) << 16 ^ dstWord150;

          }
      }
  }
  // [3] set decode structure
  decoded.opcode = opcode;
  decoded.Rd = Rdst;
  decoded.Ad = Ad;
  decoded.isByte = isByte;
  decoded.isAddrWord = isAddrWord;
  decoded.dstWord = dstWord;
  decoded.extended = extendedInstruction;
  decoded.n = n;
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
  int16_t offset = (pInsn & 0x3FF);
  bool isNeg = (pInsn & 0x200);

  decoded.opcode = opcode;
  decoded.offset = (isNeg)?(0xFC00 | offset):(offset); // sign-extend

  return decoded;
}
decode_result decode_extended(const uint16_t pInsn)
{
    decode_result decoded;

    uint16_t opcode = (pInsn>>4 & 0x3F);
    bool isAddrWord = (opcode & 0x01 != 0x01);
    uint8_t Rdst = (pInsn & 0xF);
    uint8_t n = ((pInsn >> 10) & 0x3)+1;


    decoded.opcode = opcode;
    decoded.isAddrWord = isAddrWord;
    decoded.Rd = Rdst;
    decoded.dstWord = n;

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
  decode_extended,     // 0
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

// Decoding is a matter of indexing the decode jump table
// using the first 4 instruction opcode bits and then
// executing the function pointed to
// The decode functions update the global decode structure
decode_result decode(uint16_t* instruction)
{

  extendedInstruction = false;

  if (*instruction >= 0x1800 && *instruction <= 0x2000) {

    // | 15  9 |  8  |  7  |  6  | 5  4 | 3    0 |
    // |-----------------------------------------|
    // |       | ZC  |  #  | A/L |      |(n-1)/Rn|
    // |-----------------------------------------|

    // |15  11 | 10      7 |  6  | 5  4 | 3       0 |
    // |--------------------------------------------|
    // |       | Src 19:16 | A/L |      | Dst 19:16 |
    // |--------------------------------------------|

    // | 15  7 |  6  | 5  4 | 3       0 |
    // |--------------------------------|
    // |       | A/L |      | Dst 19:16 |
    // |--------------------------------|

    // |15  11 | 10      7 |  6  | 5  0 |
    // |--------------------------------|
    // |       | Src 19:16 | A/L |      |
    // |--------------------------------|

    extendedInstruction = true;

    fetch_instruction(cpu_get_pc(), instruction); //fetch new instruction

    cpu_set_pc(cpu_get_pc() + 0x2);

  }
   // if (cpu_get_pc() == 0x41c4) {
     // printf("Hello");
 // }
  return decodeJumpTable[*instruction >> 12](*instruction);
}
}
;