#include "exmemwb.h"
#include "decode.h"

// Holds the result of decoding
// Passed to exmemwb stage
DECODE_RESULT decoded;

// Various decodings
DECODE_RESULT decode_3lo(const uint16_t pInsn)
{
  DECODE_RESULT decoded;

  decoded.rD = pInsn & 0x7;
  decoded.rN = (pInsn >> 3) & 0x7;
  decoded.rM = (pInsn >> 6) & 0x7;

  return decoded;
}

DECODE_RESULT decode_2loimm5(const uint16_t pInsn)
{
  DECODE_RESULT decoded;

  decoded.rD = pInsn & 0x7;
#if DECODE_SAFE
  decoded.rM = (pInsn >> 3) & 0x7; // Just to be safe
#endif
  decoded.rN = (pInsn >> 3) & 0x7;
  decoded.imm = (pInsn >> 6) & 0x1F;

  return decoded;
}

DECODE_RESULT decode_2loimm3(const uint16_t pInsn)
{
  DECODE_RESULT decoded;

  decoded.rD = pInsn & 0x7;
#if DECODE_SAFE
  decoded.rM = (pInsn >> 3) & 0x7; // Just to be safe
#endif
  decoded.rN = (pInsn >> 3) & 0x7;
  decoded.imm = (pInsn >> 6) & 0x7;

  return decoded;
}

DECODE_RESULT decode_2lo(const uint16_t pInsn)
{
  DECODE_RESULT decoded;

  decoded.rD = pInsn & 0x7;
  decoded.rM = (pInsn >> 3) & 0x7;
  decoded.rN = (pInsn >> 3) & 0x7;

  return decoded;
}

DECODE_RESULT decode_imm8lo(const uint16_t pInsn)
{
  DECODE_RESULT decoded;

  decoded.rD = (pInsn >> 8) & 0x7;
#if DECODE_SAFE
  decoded.rM = decoded.rD; // Just to be safe
  decoded.rN = decoded.rD; // Just to be safe
#endif
  decoded.imm = pInsn & 0xFF;

  return decoded;
}

DECODE_RESULT decode_imm8(const uint16_t pInsn)
{
  DECODE_RESULT decoded;

  decoded.imm = pInsn & 0xFF;

  return decoded;
}

DECODE_RESULT decode_imm8c(const uint16_t pInsn)
{
  DECODE_RESULT decoded;

  decoded.imm = pInsn & 0xFF;
  decoded.cond = (pInsn >> 8) & 0xF;

  return decoded;
}

DECODE_RESULT decode_imm7(const uint16_t pInsn)
{
  DECODE_RESULT decoded;

  decoded.rD = GPR_SP;
  decoded.imm = pInsn & 0x7F;

  return decoded;
}

DECODE_RESULT decode_imm11(const uint16_t pInsn)
{
  DECODE_RESULT decoded;

  decoded.imm = pInsn & 0x7FF;
  return decoded;
}

DECODE_RESULT decode_reglistlo(const uint16_t pInsn)
{
  DECODE_RESULT decoded;

  decoded.rN = (pInsn >> 8) & 0x7;
  decoded.reg_list = pInsn & 0xFF;
  return decoded;
}

DECODE_RESULT decode_pop(const uint16_t pInsn)
{
  DECODE_RESULT decoded;

  decoded.reg_list = ((pInsn & 0x100) << 7) | (pInsn & 0xFF);
  return decoded;
}

DECODE_RESULT decode_push(const uint16_t pInsn)
{
  DECODE_RESULT decoded;

  decoded.reg_list = (pInsn & 0xFF) | ((pInsn & 0x100) << 6);
  return decoded;
}

DECODE_RESULT decode_bl(const uint16_t pInsn)
{
  DECODE_RESULT decoded;

  uint16_t secondHalf;
  simLoadInsn(cpu_get_pc() - 0x2, &secondHalf);

  uint32_t S = (pInsn >> 10) & 0x1;
  uint32_t J1 = (secondHalf >> 13) & 0x1;
  uint32_t J2 = (secondHalf >> 11) & 0x1;
  uint32_t I1 = ~(J1 ^ S) & 0x1;
  uint32_t I2 = ~(J2 ^ S) & 0x1;
  uint32_t imm10 = pInsn & 0x3FF;
  uint32_t imm11 = secondHalf & 0x7FF;

  decoded.imm = (S << 23) | (I1 << 22) | (I2 << 21) | (imm10 << 11) | imm11;
  return decoded;
}

DECODE_RESULT decode_1all(const uint16_t pInsn)
{
  DECODE_RESULT decoded;

  decoded.rM = (pInsn >> 3) & 0xF;
  return decoded;
}

DECODE_RESULT decode_mov_r(const uint16_t pInsn)
{
  DECODE_RESULT decoded;

  decoded.rD = (pInsn & 0x7) | ((pInsn & 0x80) >> 4);
  decoded.rN = decoded.rD;
  decoded.rM = (pInsn >> 3) & 0xF;
  return decoded;
}

// Stop simulation if we cannot decode the instruction
DECODE_RESULT decode_error(const uint16_t pInsn)
{
  fprintf(stderr, "Error: Malformed instruction: Unable to decode: 0x%4.4X at 0x%08X\n", pInsn,
      cpu_get_pc() - 4);
  sim_exit(1);
}

// Decode functions that require more opcode bits than the first 6
DECODE_RESULT (*decodeJumpTable17[4])
(const uint16_t pInsn) = {
    decode_mov_r,               /* 01_0001_0XXX (110 - 117) */
    decode_mov_r, decode_mov_r, /* 01_0001_10XX (118 - 11B) */
    decode_1all                 /* 01_0001_11XX (11C - 11F) */
};

DECODE_RESULT (*decodeJumpTable44[4])
(const uint16_t pInsn) = {decode_imm7, /* 10_1100_00XX (2C0 - 2C3) */
    decode_error, decode_2lo,          /* 10_1100_10XX (2C8 - 2CB) */
    decode_error};

DECODE_RESULT (*decodeJumpTable47[4])
(const uint16_t pInsn) = {decode_pop, /* 10_1111_0XXX (2F0 - 2F7) */
    decode_pop, decode_imm8,          /* 10_1111_10XX (2F8 - 2FB) */
    decode_error};

DECODE_RESULT decode_17(const uint16_t pInsn)
{
  decodeJumpTable17[(pInsn >> 8) & 0x3](pInsn);
}
DECODE_RESULT decode_44(const uint16_t pInsn)
{
  decodeJumpTable44[(pInsn >> 8) & 0x3](pInsn);
}
DECODE_RESULT decode_47(const uint16_t pInsn)
{
  decodeJumpTable47[(pInsn >> 8) & 0x3](pInsn);
}

// Use a table of function pointers indexed by the instruction
// to make decoding fast
// Indices 16, 17, 44, 47, 60, and 62 have multiple conflicting
// decodings that need to be resolved outside the jump table
DECODE_RESULT (*decodeJumpTable[64])
(const uint16_t pInsn) = {decode_2loimm5, decode_2loimm5, decode_2loimm5, decode_2loimm5,
    decode_2loimm5, decode_2loimm5, decode_3lo, decode_2loimm3, decode_imm8lo, decode_imm8lo,
    decode_imm8lo, decode_imm8lo, decode_imm8lo, decode_imm8lo, decode_imm8lo, decode_imm8lo,
    decode_2lo, /* A5.2.2 - these are all decoded the same */
    decode_17,  /* 17 */
    decode_imm8lo, decode_imm8lo, decode_3lo, decode_3lo, decode_3lo, decode_3lo, decode_2loimm5,
    decode_2loimm5, decode_2loimm5, decode_2loimm5, decode_2loimm5, decode_2loimm5, decode_2loimm5,
    decode_2loimm5, decode_2loimm5, decode_2loimm5, decode_2loimm5, decode_2loimm5, decode_imm8lo,
    decode_imm8lo, decode_imm8lo, decode_imm8lo, decode_imm8lo, decode_imm8lo, decode_imm8lo,
    decode_imm8lo, decode_44,           /* 44 */
    decode_push, decode_2lo, decode_47, /* 47 */
    decode_reglistlo, decode_reglistlo, decode_reglistlo, decode_reglistlo, decode_imm8c,
    decode_imm8c, decode_imm8c, decode_imm8c, decode_imm11, decode_imm11, decode_error, /* 58 */
    decode_error,                                                                       /* 59 */
    decode_bl, /* Ignore other possible decodings */
    decode_bl, /* Ignore other possible decodings */
    decode_error, decode_error};

// Decoding is a matter of indexing the decode jump tabel
// using the first 6 instruction opcode bits and then
// executing the function pointed to
// The decode functions update the global decode structure
DECODE_RESULT decode(const uint16_t pInsn)
{
  return decodeJumpTable[pInsn >> 10](pInsn);
}
