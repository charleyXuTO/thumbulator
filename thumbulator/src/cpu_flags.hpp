#ifndef THUMBULATOR_CPU_FLAGS_H
#define THUMBULATOR_CPU_FLAGS_H

#include "thumbulator/cpu.hpp"
#include <string>

namespace thumbulator {

// Define bit fields of SR
#define FLAG_N_INDEX 2
#define FLAG_Z_INDEX 1
#define FLAG_C_INDEX 0
#define FLAG_V_INDEX 8
#define FLAG_GIE_INDEX 3
#define FLAG_CPU_OFF_INDEX 4
#define FLAG_OSC_OFF_INDEX 5
#define FLAG_SCG0_INDEX 6
#define FLAG_SCG1_INDEX 7

#define FLAG_N_MASK (1 << FLAG_N_INDEX)
#define FLAG_Z_MASK (1 << FLAG_Z_INDEX)
#define FLAG_C_MASK (1 << FLAG_C_INDEX)
#define FLAG_V_MASK (1 << FLAG_V_INDEX)

// Get, set, and compute the CPU flags
#define cpu_get_flag_z() ((cpu_get_sr() & FLAG_Z_MASK) >> FLAG_Z_INDEX)
#define cpu_get_flag_n() ((cpu_get_sr() & FLAG_N_MASK) >> FLAG_N_INDEX)
#define cpu_get_flag_c() ((cpu_get_sr() & FLAG_C_MASK) >> FLAG_C_INDEX)
#define cpu_get_flag_v() ((cpu_get_sr() & FLAG_V_MASK) >> FLAG_V_INDEX)
#define cpu_set_flag_z(x) cpu_set_sr((((x)&0x1) << FLAG_Z_INDEX) | (cpu_get_sr() & ~FLAG_Z_MASK))
#define cpu_set_flag_n(x) cpu_set_sr((((x)&0x1) << FLAG_N_INDEX) | (cpu_get_sr() & ~FLAG_N_MASK))
#define cpu_set_flag_c(x) cpu_set_sr((((x)&0x1) << FLAG_C_INDEX) | (cpu_get_sr() & ~FLAG_C_MASK))
#define cpu_set_flag_v(x) cpu_set_sr((((x)&0x1) << FLAG_V_INDEX) | (cpu_get_sr() & ~FLAG_V_MASK))

#define do_zflag(x) cpu_set_flag_z(((x) == 0) ? 1 : 0)
#define do_nflag(x) cpu_set_flag_n((x) >> 15)
#define do_vflag(a, b, r) \
  cpu_set_flag_v(         \
      (((a) >> 15) & ((b) >> 15) & ~((r) >> 15)) | (~((a) >> 15) & ~((b) >> 15) & ((r) >> 15)))

static void do_cflag(uint16_t a, uint16_t b, uint16_t carry)
{
  uint16_t result;

  result = (a & 0x7FFF) + (b & 0x7FFF) + carry; //carry in
  result = (result >> 15) + (a >> 15) + (b >> 15);      //carry out
  cpu_set_flag_c(result >> 1);
}

// TODO: do we need this??
// Other SPR
#define CPU_MODE_HANDLER 0
#define CPU_MODE_THREAD 1
#define cpu_mode_is_handler() (cpu.mode == 0x0)
#define cpu_mode_is_thread() (cpu.mode == 0x1)
#define cpu_mode_handler() cpu.mode = (0x0)
#define cpu_mode_thread() cpu.mode = (0x1)
#define cpu_get_ipsr() (cpu.ipsr)
#define cpu_set_ipsr(x) cpu.ipsr = (x & 0x1F)
#define CPU_STACK_MAIN 0
#define CPU_STACK_PROCESS 1
#define cpu_stack_is_main() ((cpu.control & 0x2) == 0x0)
#define cpu_stack_is_process() (~cpu_stack_is_main())
#define cpu_stack_use_main() cpu.control = (cpu.control & ~0x2)
#define cpu_stack_use_process() cpu.control = (cpu.control | 0x2)

// Sign extension
#define zeroExtend32(x) (x)
#define signExtend32(x, n) \
  (((((x) >> ((n)-1)) & 0x1) != 0) ? (~((unsigned int)0) << (n)) | (x) : (x))

// Special write to PC
#define alu_write_pc(x)    \
  do {                     \
    BRANCH_WAS_TAKEN = 1;  \
    cpu_set_pc((x) | 0x1); \
  } while(0)
}

enum AddrMode {
  REGISTER = 0,                 // Rn
  INDEXED = 1,                  // X(Rn)
  SYMBOLIC = 2,                 // ADDR
  ABSOLUTE = 3,                 // &ADDR
  INDIRECT_REGISTER = 4,        // @Rn
  INDIRECT_AUTOINCREMENT = 5,   // @Rn+
  IMMEDIATE = 6                 // #N
};

//TODO: better print
extern std::string addrModeString[7];

#endif //THUMBULATOR_CPU_FLAGS_H
