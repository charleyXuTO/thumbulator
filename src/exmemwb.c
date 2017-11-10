#include "exmemwb.h"

uint16_t insn;

#if HOOK_GPR_ACCESSES
uint32_t cpu_get_gpr(uint32_t gpr)
{
  gprReadHooks[gpr]();
  return cpu.gpr[gpr];
}

void cpu_set_gpr(uint32_t gpr, uint32_t value)
{
  gprWriteHooks[gpr]();
  cpu.gpr[gpr] = value;
}
#endif

void do_cflag(uint32_t a, uint32_t b, uint32_t carry)
{
  uint32_t result;

  result = (a & 0x7FFFFFFF) + (b & 0x7FFFFFFF) + carry; //carry in
  result = (result >> 31) + (a >> 31) + (b >> 31);      //carry out
  cpu_set_flag_c(result >> 1);
}

uint32_t adcs(DECODE_RESULT);
uint32_t adds_i3(DECODE_RESULT);
uint32_t adds_i8(DECODE_RESULT);
uint32_t adds_r(DECODE_RESULT);
uint32_t add_r(DECODE_RESULT);
uint32_t add_sp(DECODE_RESULT);
uint32_t adr(DECODE_RESULT);
uint32_t subs_i3(DECODE_RESULT);
uint32_t subs_i8(DECODE_RESULT);
uint32_t subs(DECODE_RESULT);
uint32_t sub_sp(DECODE_RESULT);
uint32_t sbcs(DECODE_RESULT);
uint32_t rsbs(DECODE_RESULT);
uint32_t muls(DECODE_RESULT);
uint32_t cmn(DECODE_RESULT);
uint32_t cmp_i(DECODE_RESULT);
uint32_t cmp_r(DECODE_RESULT);
uint32_t tst(DECODE_RESULT);
uint32_t b(DECODE_RESULT);
uint32_t b_c(DECODE_RESULT);
uint32_t blx(DECODE_RESULT);
uint32_t bx(DECODE_RESULT);
uint32_t bl(DECODE_RESULT);
uint32_t ands(DECODE_RESULT);
uint32_t bics(DECODE_RESULT);
uint32_t eors(DECODE_RESULT);
uint32_t orrs(DECODE_RESULT);
uint32_t mvns(DECODE_RESULT);
uint32_t asrs_i(DECODE_RESULT);
uint32_t asrs_r(DECODE_RESULT);
uint32_t lsls_i(DECODE_RESULT);
uint32_t lsrs_i(DECODE_RESULT);
uint32_t lsls_r(DECODE_RESULT);
uint32_t lsrs_r(DECODE_RESULT);
uint32_t rors(DECODE_RESULT);
uint32_t ldm(DECODE_RESULT);
uint32_t stm(DECODE_RESULT);
uint32_t pop(DECODE_RESULT);
uint32_t push(DECODE_RESULT);
uint32_t ldr_i(DECODE_RESULT);
uint32_t ldr_sp(DECODE_RESULT);
uint32_t ldr_lit(DECODE_RESULT);
uint32_t ldr_r(DECODE_RESULT);
uint32_t ldrb_i(DECODE_RESULT);
uint32_t ldrb_r(DECODE_RESULT);
uint32_t ldrh_i(DECODE_RESULT);
uint32_t ldrh_r(DECODE_RESULT);
uint32_t ldrsb_r(DECODE_RESULT);
uint32_t ldrsh_r(DECODE_RESULT);
uint32_t str_i(DECODE_RESULT);
uint32_t str_sp(DECODE_RESULT);
uint32_t str_r(DECODE_RESULT);
uint32_t strb_i(DECODE_RESULT);
uint32_t strb_r(DECODE_RESULT);
uint32_t strh_i(DECODE_RESULT);
uint32_t strh_r(DECODE_RESULT);
uint32_t movs_i(DECODE_RESULT);
uint32_t mov_r(DECODE_RESULT);
uint32_t movs_r(DECODE_RESULT);
uint32_t sxtb(DECODE_RESULT);
uint32_t sxth(DECODE_RESULT);
uint32_t uxtb(DECODE_RESULT);
uint32_t uxth(DECODE_RESULT);
uint32_t rev(DECODE_RESULT);
uint32_t rev16(DECODE_RESULT);
uint32_t revsh(DECODE_RESULT);
uint32_t breakpoint(DECODE_RESULT);

uint32_t exmemwb_error(DECODE_RESULT decoded)
{
  fprintf(stderr, "Error: Unsupported instruction: Unable to execute\n");
  sim_exit(1);
  return 0;
}

// Execute functions that require more opcode bits than the first 6
uint32_t (*executeJumpTable6[2])(DECODE_RESULT) = {
    adds_r, /* 060 - 067 */
    subs    /* 068 - 06F */
};

uint32_t entry6(DECODE_RESULT decoded)
{
  return executeJumpTable6[(insn >> 9) & 0x1](decoded);
}

uint32_t (*executeJumpTable7[2])(DECODE_RESULT) = {
    adds_i3, /* (070 - 077) */
    subs_i3  /* (078 - 07F) */
};

uint32_t entry7(DECODE_RESULT decoded)
{
  return executeJumpTable7[(insn >> 9) & 0x1](decoded);
}

uint32_t (*executeJumpTable16[16])(DECODE_RESULT) = {ands, eors, lsls_r, lsrs_r, asrs_r, adcs, sbcs, rors, tst,
    rsbs, cmp_r, exmemwb_error, orrs, muls, bics, mvns};

uint32_t entry16(DECODE_RESULT decoded)
{
  return executeJumpTable16[(insn >> 6) & 0xF](decoded);
}

uint32_t (*executeJumpTable17[8])(DECODE_RESULT) = {
    add_r,        /* (110 - 113) */
    add_r, cmp_r, /* (114 - 117) */
    cmp_r, mov_r, /* (118 - 11B) */
    mov_r, bx,    /* (11C - 11D) */
    blx           /* (11E - 11F) */
};

uint32_t entry17(DECODE_RESULT decoded)
{
  return executeJumpTable17[(insn >> 7) & 0x7](decoded);
}

uint32_t (*executeJumpTable20[2])(DECODE_RESULT) = {
    str_r, /* (140 - 147) */
    strh_r /* (148 - 14F) */
};

uint32_t entry20(DECODE_RESULT decoded)
{
  return executeJumpTable20[(insn >> 9) & 0x1](decoded);
}

uint32_t (*executeJumpTable21[2])(DECODE_RESULT) = {
    strb_r, /* (150 - 157) */
    ldrsb_r /* (158 - 15F) */
};

uint32_t entry21(DECODE_RESULT decoded)
{
  return executeJumpTable21[(insn >> 9) & 0x1](decoded);
}

uint32_t (*executeJumpTable22[2])(DECODE_RESULT) = {
    ldr_r, /* (160 - 167) */
    ldrh_r /* (168 - 16F) */
};

uint32_t entry22(DECODE_RESULT decoded)
{
  return executeJumpTable22[(insn >> 9) & 0x1](decoded);
}

uint32_t (*executeJumpTable23[2])(DECODE_RESULT) = {
    ldrb_r, /* (170 - 177) */
    ldrsh_r /* (178 - 17F) */
};

uint32_t entry23(DECODE_RESULT decoded)
{
  return executeJumpTable23[(insn >> 9) & 0x1](decoded);
}

uint32_t (*executeJumpTable44[16])(DECODE_RESULT) = {add_sp, /* (2C0 - 2C1) */
    add_sp, sub_sp,                            /* (2C2 - 2C3) */
    sub_sp, exmemwb_error, exmemwb_error, exmemwb_error, exmemwb_error, sxth, sxtb, uxth, uxtb,
    exmemwb_error, exmemwb_error, exmemwb_error, exmemwb_error};

uint32_t entry44(DECODE_RESULT decoded)
{
  return executeJumpTable44[(insn >> 6) & 0xF](decoded);
}

uint32_t (*executeJumpTable46[16])(DECODE_RESULT) = {exmemwb_error, exmemwb_error, exmemwb_error, exmemwb_error,
    exmemwb_error, exmemwb_error, exmemwb_error, exmemwb_error, rev, rev16, exmemwb_error,
    exmemwb_error, exmemwb_error, exmemwb_error, exmemwb_error, exmemwb_error};

uint32_t entry46(DECODE_RESULT decoded)
{
  return executeJumpTable46[(insn >> 6) & 0xF](decoded);
}

uint32_t (*executeJumpTable47[2])(DECODE_RESULT) = {
    pop,       /* (2F0 - 2F7) */
    breakpoint /* (2F8 - 2FB) */
};

uint32_t entry47(DECODE_RESULT decoded)
{
  return executeJumpTable47[(insn >> 9) & 0x1](decoded);
}

uint32_t entry55(DECODE_RESULT decoded)
{
  if((insn & 0x0300) != 0x0300)
    return b_c(decoded);

  if(insn == 0xDF01) {
    printf("Program exit after\n\t%llu ticks\n\t%llu instructions\n", cycleCount, insnCount);
#if MEM_COUNT_INST
    printf("Loads: %u\nStores: %u\nCheckpoints: %u\n", load_count, store_count, cp_count);
#endif
    sim_exit(0);
  }

  return exmemwb_error(decoded);
}

uint32_t (*executeJumpTable[64])(DECODE_RESULT) = {lsls_i, lsls_i, lsrs_i, lsrs_i, asrs_i, asrs_i, entry6, /* 6 */
    entry7,                                                                              /* 7 */
    movs_i, movs_i, cmp_i, cmp_i, adds_i8, adds_i8, subs_i8, subs_i8, entry16,           /* 16 */
    entry17,                                                                             /* 17 */
    ldr_lit, ldr_lit, entry20,                                                           /* 20 */
    entry21,                                                                             /* 21 */
    entry22,                                                                             /* 22 */
    entry23,                                                                             /* 23 */
    str_i, str_i, ldr_i, ldr_i, strb_i, strb_i, ldrb_i, ldrb_i, strh_i, strh_i, ldrh_i, ldrh_i,
    str_sp, str_sp, ldr_sp, ldr_sp, adr, adr, add_sp, add_sp, entry44, /* 44 */
    push, entry46,                                                     /* 46 */
    entry47,                                                           /* 47 */
    stm, stm, ldm, ldm, b_c, b_c, b_c, entry55,                        /* 55 */
    b, b, exmemwb_error, exmemwb_error, bl,                            /* 60 ignore mrs */
    bl,                                                                /* 61 ignore udef */
    exmemwb_error, exmemwb_error};

void exwbmem(uint16_t pInsn, DECODE_RESULT decoded)
{
  ++insnCount;
  insn = pInsn;

  unsigned int insnTicks = executeJumpTable[pInsn >> 10](decoded);
  INCREMENT_CYCLES(insnTicks);

  // Update the systick unit and look for resets
  if(systick.control & 0x1) {
    if(insnTicks >= systick.value) {
      // Ignore resets due to reads
      if(systick.value > 0)
        systick.control |= 0x00010000;

      systick.value = systick.reload - insnTicks + systick.value;
    } else
      systick.value -= insnTicks;
  }
}
