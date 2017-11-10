#include <stdlib.h>
#include "exmemwb.h"
#include "decode.h"

uint32_t breakpoint(void)
{
  return 0;
}

/*
int handle_bkpt(unsigned int bp, unsigned int arg)
{
	int r = 1;
	FILE *f;
	int s, e, n;
	unsigned int sp;

	sp = read_register(13);
	switch (arg) {
		case 0x18:
			fprintf(stderr, "Exiting.\n");
			break;
		case 0x80:
			s = read32(sp + 8);
			e = read32(sp + 4);
			fprintf(stderr, "Dumping from %08X to %08X...\n",
					s, e);
			f = fopen(output_file_name, "wb");
			while (s != e) {
				n = read32(s);
				fwrite(&n, 4, 1, f);
				s += 4;
			}
			fclose(f);
			write_register(13, sp + 12);
			r = 0;
			break;
		default:
			fprintf(stderr, "bkpt 0x%02X %08X\n", bp, arg);
			break;
	}
	return r;
}*/

///--- Move operations -------------------------------------------///

// MOVS - write an immediate to the destination register
uint32_t movs_i()
{
  diss_printf("movs r%u, #0x%02X\n", decoded.rD, decoded.imm);

  uint32_t opA = zeroExtend32(decoded.imm);
  cpu_set_gpr(decoded.rD, opA);

  do_nflag(opA);
  do_zflag(opA);

  return 1;
}

// MOV - copy the source register value to the destination register
uint32_t mov_r()
{
  diss_printf("mov r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t opA = cpu_get_gpr(decoded.rM);

  if(decoded.rD == GPR_PC)
    alu_write_pc(opA);
  else
    cpu_set_gpr(decoded.rD, opA);

  return 1;
}

// MOVS - copy the low source register value to the destination low register
uint32_t movs_r()
{
  diss_printf("movs r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t opA = cpu_get_gpr(decoded.rM);
  cpu_set_gpr(decoded.rD, opA);

  do_nflag(opA);
  do_zflag(opA);

  return 1;
}

///--- Bit twiddling operations -------------------------------------------///

// SXTB - Sign extend a byte to a word
uint32_t sxtb()
{
  diss_printf("sxtb r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t result = 0xFF & cpu_get_gpr(decoded.rM);
  result = (result & 0x80) != 0 ? (result | 0xFFFFFF00) : result;

  cpu_set_gpr(decoded.rD, result);

  return 1;
}

// SXTH - Sign extend a halfword to a word
uint32_t sxth()
{
  diss_printf("sxth r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t result = 0xFFFF & cpu_get_gpr(decoded.rM);
  result = (result & 0x8000) != 0 ? (result | 0xFFFF0000) : result;

  cpu_set_gpr(decoded.rD, result);

  return 1;
}

// UXTB - Extend a byte to a word
uint32_t uxtb()
{
  diss_printf("uxtb r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t result = 0xFF & cpu_get_gpr(decoded.rM);
  cpu_set_gpr(decoded.rD, result);

  return 1;
}

// UXTH - Extend a halfword to a word
uint32_t uxth()
{
  diss_printf("uxth r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t result = 0xFFFF & cpu_get_gpr(decoded.rM);
  cpu_set_gpr(decoded.rD, result);

  return 1;
}

// REV - Reverse ordering of bytes in a word
uint32_t rev()
{
  diss_printf("rev r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t opA = cpu_get_gpr(decoded.rM);
  uint32_t result = opA << 24;
  result |= (opA << 8) & 0xFF0000;
  result |= (opA >> 8) & 0xFF00;
  result |= (opA >> 24);

  cpu_set_gpr(decoded.rD, result);

  return 1;
}

// REV16 - Reverse ordering of bytes in a packed halfword
uint32_t rev16()
{
  diss_printf("rev16 r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t opA = cpu_get_gpr(decoded.rM);
  uint32_t result = (opA << 8) & 0xFF000000;
  result |= (opA >> 8) & 0xFF0000;
  result |= (opA << 8) & 0xFF00;
  result |= (opA >> 8) & 0xFF;

  cpu_set_gpr(decoded.rD, result);

  return 1;
}

// REVSH - Reverse ordering of bytes in a signed halfword
uint32_t revsh()
{
  diss_printf("revsh r%u, r%u\n", decoded.rD, decoded.rM);

  uint32_t opA = cpu_get_gpr(decoded.rM);
  uint32_t result = (opA & 0x8) != 0 ? (0xFFFFFF00 | opA) : (0xFF & opA);
  result <<= 8;
  result |= (opA >> 8) & 0xFF;

  cpu_set_gpr(decoded.rD, result);

  return 1;
}
