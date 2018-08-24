#include "thumbulator/memory.hpp"
#include "cpu_flags.hpp"
#include "trace.hpp"
#include "utils.hpp"

namespace thumbulator {

uint32_t rrum(decode_result const* decoded) {
    uint32_t opA = getValue(0, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
    //uint32_t carry = cpu_get_flag_c();
    uint16_t lsb = opA & 0x1;
    int32_t result = opA >> (decoded->n);
    if (!decoded->isAddrWord) {
        result |= (0 << 15);
    }
    else {
        result |= (0 << 19);
    }
    cpu_set_flag_c(lsb);

    // update result & flags
    setValue(0, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);

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

    // update Rs if it's in autoincrement mode
    fprintf(stderr, "Result: 0x%4.4x (%u)\n", result, result);
    for (int i =0; i<16; i++) {
        fprintf(stderr, "  Register %d : 0x%4.4x (%u)\n", i, cpu.gpr[i], cpu.gpr[i]);
    }
    return 1;
}
uint32_t pushm (decode_result const* decoded ) {
    TRACE_INSTRUCTION("pushm %s%u %d\n", addrModeString[decoded->Ad].c_str(), decoded->Rd, decoded->n );

    int32_t opA;
    uint32_t sp;
    for (int n = 0 ; n<decoded->n; n++) {
        opA = getValue(0, (decoded->Rd-n), decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
        if (!decoded->isAddrWord) {
            // Force to 16-bit value.
            opA = opA & 0xFFFF;

            sp = cpu_get_sp()-2;
            cpu_set_sp(sp);
            store(sp, opA, decoded->isByte);
        }
        else {
            // Force to 20-bit value, this is MSP430X.
            opA = opA & 0xFFFFF;

            // Store the first 16 bits.
            sp= cpu_get_sp()-2;
            cpu_set_sp(sp);
            store(sp, ((opA>>16) & 0xF), decoded->isByte);

            // Store the next 4 bits.
            sp = cpu_get_sp()-2;
            cpu_set_sp(sp);
            store(sp, (opA & 0xFFFF), decoded->isByte);

        }

    }

    for (int i =0; i<16; i++) {
        fprintf(stderr, "  Register %d : 0x%4.4x (%u)\n", i, cpu.gpr[i], cpu.gpr[i]);
    }
    return 1;
}
uint32_t popm (decode_result const* decoded) {
    TRACE_INSTRUCTION("popm %s%u %d\n", addrModeString[decoded->Ad].c_str(), decoded->Rd, decoded->n );

    uint32_t opA150;
    uint32_t opA1916;
    uint32_t opA;
    uint32_t sp;
    for (int n = 0; n< decoded->n;n++) {
        if (decoded->isAddrWord) {
            // Load the first 4 bits (reverse order to push).
            sp = cpu_get_sp();
            load(sp,&opA1916,0);

            // Load the next 16 bits.
            sp= cpu_get_sp()+2;
            cpu_set_sp(sp);
            load(sp, &opA150,0);

            // Create and set the 20-bit value
            opA = ((opA1916 & 0xF) << 16 ) ^ opA150;
            setValue (0, (decoded->Rd+n), decoded->dstWord, decoded->isByte, decoded->isAddrWord, opA);

            sp = cpu_get_sp()+2;
            cpu_set_sp(sp);

        }
        else {
            sp = cpu_get_sp();
            load(sp, &opA, 0);
            opA = opA & 0xFFFF;
            setValue(0, (decoded->Rd+n), decoded->dstWord, decoded->isByte, decoded->isAddrWord, opA);
            cpu_set_sp(sp+2);
        }

    }
    for (int i =0; i<16; i++) {
        fprintf(stderr, "  Register %d : 0x%4.4x (%u)\n", i, cpu.gpr[i], cpu.gpr[i]);
    }
    return 1;

}
uint32_t mova(decode_result const* decoded) {

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
    fprintf(stderr, "Result: 0x%4.4x (%u) C flag: %u N flag: %u, V flag: %u, Z flag: %u\n", result, result, cpu_get_flag_c(), cpu_get_flag_n(), cpu_get_flag_v(), cpu_get_flag_z());
    for (int i =0; i<16; i++) {
        fprintf(stderr, "  Register %d : 0x%4.4x (%u)\n", i, cpu.gpr[i], cpu.gpr[i]);
    }
/*
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
*/
    // update Rs if it's in autoincrement mode
    updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);
    return 1;
}


}