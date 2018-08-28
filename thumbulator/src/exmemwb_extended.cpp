#include "thumbulator/memory.hpp"
#include "cpu_flags.hpp"
#include "trace.hpp"
#include "utils.hpp"

namespace thumbulator {

uint32_t rrum(decode_result const* decoded) {
    TRACE_INSTRUCTION("rrum %s%u %d\n", addrModeString[0].c_str(), decoded->Rd, decoded->n );
    uint32_t opA = getValue(0, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
    //uint32_t carry = cpu_get_flag_c();
    int32_t result = opA;
    uint16_t lsb;
    for (int n = 0; n<decoded->n; n++ ) {
        lsb = result & 0x01; // getting the least significant bit
        result = result >> 1; // shifting to the right
        if (!decoded->isAddrWord) {
            result |= (0 << 15); //setting the most significiant bit to zero
        }
        else {
            result |= (0 << 19);
        }
        cpu_set_flag_c(lsb); //setting the carry to the least significant bit
    }
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
uint32_t rrcm (decode_result const* decoded) {
    TRACE_INSTRUCTION("rrcm %s%u %d\n", addrModeString[0].c_str(), decoded->Rd, decoded->n );
    uint32_t opA = getValue(0, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
    uint32_t carry;
    uint16_t lsb;
    int32_t result = opA;
    for (int n = 0; n < decoded->n; n++ ) {
        carry = cpu_get_flag_c(); // getting the carry bit
        lsb = result & 0x01; //getting the least significant bit
        result = result >> 1; // shifting to the right
        if (!decoded->isAddrWord) {
            result |= (carry << 15); // shift carry into most significant bit
        } else {
            result |= (carry << 19); // shift carry into most significant bit
        }
        cpu_set_flag_c(lsb); //setting the carry
    }
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
uint32_t rram (decode_result  const* decoded) {

    TRACE_INSTRUCTION("rram %s%u %d\n", addrModeString[0].c_str(), decoded->Rd, decoded->n );
    uint32_t opA = getValue(0, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
    uint16_t lsb;
    uint16_t msb;
    int32_t result = opA;


    for (int n = 0 ; n < decoded->n; n++) {
        if (!decoded->isAddrWord) { // if it is not an address-word
            lsb = result & 0x1; //getting the least significant bit
            msb = (result >> 15) & 0x1; // getting the most significant bit
            result = result >> 1;
            result |= (msb << 15); // most significant bit stays the most significant bit
        } else {
            lsb = result & 0x1; //getting the least significant bit
            msb = (result >> 19) & 0x1; //getting the most significant bit
            result = result >> 1;
            result |= (msb << 19); // most significant bit stays the most significant bit

        }
        cpu_set_flag_c(lsb); //setting the carry flag
    }
    setValue (0, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);
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
    fprintf(stderr, "Result: 0x%4.4x (%u)\n", result, result);
    for (int i =0; i<16; i++) {
        fprintf(stderr, "  Register %d : 0x%4.4x (%u)\n", i, cpu.gpr[i], cpu.gpr[i]);
    }
    return 1;
}
uint32_t rlam (decode_result const* decoded ) {

    TRACE_INSTRUCTION("rlam %s%u %d\n", addrModeString[0].c_str(), decoded->Rd, decoded->n );
    uint32_t opA = getValue(0, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
    //uint16_t lsb;
    uint16_t msb;
    int32_t result = opA;
    for (int n = 0; n<decoded->n; n++) {
        if (!decoded->isAddrWord) { //if it is not an address-word
            msb = (result >>15) & 0x1; //getting the most significant bit
            result = result << 1; //shifting the result to the left
            cpu_set_flag_c(msb); //setting the carry as the msb
        }
        else {
            msb = (result >> 19) & 0x1;
            result = result <<1;
            cpu_set_flag_c(msb);
        }
    }
    setValue (0, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);
    if (!decoded->isAddrWord) {
        do_nflag(result);
        do_zflag(result);
        // the documentation said that the v flag is undefined here -_-
    }
    else {
        do_nflagx(result);
        do_zflagx(result);
        // same thing here
    }
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
    for (int n = 0 ; n<decoded->n; n++) { // pushing multiple registers
        opA = getValue(0, (decoded->Rd-n), decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
        if (!decoded->isAddrWord) {
            // Force to 16-bit value.
            opA = opA & 0xFFFF;
            sp = cpu_get_sp()-2; // decreasing the stack pointer
            cpu_set_sp(sp);
            store(sp, opA, decoded->isByte); //storing the value into address location
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
    for (int n = 0; n< decoded->n;n++) { //popping multiple registers
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
            load(sp, &opA, 0); //getting the "old" values of the register from the address location
            opA = opA & 0xFFFF ;
            setValue(0, (decoded->Rd+n), decoded->dstWord, decoded->isByte, decoded->isAddrWord, opA);
            cpu_set_sp(sp+2); //increasing the stack pointer
        }

    }
    for (int i =0; i<16; i++) {
        fprintf(stderr, "  Register %d : 0x%4.4x (%u)\n", i, cpu.gpr[i], cpu.gpr[i]);
    }
    return 1;

}
uint32_t mova(decode_result const* decoded) {

    int32_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte, decoded->isAddrWord,true);
    uint32_t result = opA;

    // hack to identify exit
    if(0==decoded->Rd) { // personally i have never seen this being used its from julie
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
    // All flags are left untouched
    // update Rs if it's in autoincrement mode
    updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);
    return 1;
}
uint32_t cmpa (decode_result const* decoded) {
    TRACE_INSTRUCTION("cmpa %s%u %d, %s%u %d\n", addrModeString[decoded->As].c_str(), decoded->Rs, decoded->srcWord,
                      addrModeString[decoded->Ad].c_str(), decoded->Rd, decoded->dstWord);

    // compute
    int32_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte, decoded->isAddrWord, true);
    int32_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);

    if (!decoded->isAddrWord) {
        if ((opA >> 15) > 0) {
            opA = (int16_t)opA; //getting the 16 bit signed integer value
        }
        if ((opB >> 15) >0 ) {
            opB = (int16_t)opB;
        }
    }
    else {
        if ((opA >> 19) >0 ) {
            opA = (opA & 0xFFFF) - (opA & 0x1FFFFF); //getting the 20 bit signed integer value
        }
        if ((opB >> 19) >0) {
            opB = (opB & 0xFFFF) - (opB & 0x1FFFFF);
        }
    }

    opA = ~(opA); //using 1's compliment ~
    int32_t result = opA + opB + 1; //using the formula subtraction = ~(opA) + opB + 1 =  opA - opB

    // update flags
    if (!decoded->isAddrWord) {
        do_nflag(result);
        do_zflag(result);
        do_cflag(opA, opB, 1);
        do_vflag(opA, opB, result);
    }
    else {
        do_nflagx(result);
        do_zflagx(result);
        do_cflag(opA, opB, 1);
        do_vflagx(opA, opB, result);
    }
    fprintf(stderr, "Result: 0x%4.4x (%u) C flag: %u N flag: %u, V flag: %u, Z flag: %u\n", result, result, cpu_get_flag_c(), cpu_get_flag_n(), cpu_get_flag_v(), cpu_get_flag_z());
    for (int i =0; i<16; i++) {
        fprintf(stderr, "  Register %d : 0x%4.4x (%u)\n", i, cpu.gpr[i], cpu.gpr[i]);
    }
    // update Rs if it's in autoincrement mode
    updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte); //cmpa should never be in autoincrement mode

    return 1;
}
uint32_t suba(decode_result const*decoded) {
    TRACE_INSTRUCTION("suba %s%u %d, %s%u %d\n", addrModeString[decoded->As].c_str(), decoded->Rs, decoded->srcWord,
                      addrModeString[decoded->Ad].c_str(), decoded->Rd, decoded->dstWord);

    // compute
    int32_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte, decoded->isAddrWord, true);
    int32_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);

    fprintf(stderr, "sub opA: %d, opB: %d\n", opA, opB);

    opA = ~(opA); //using 1's compliment
    int32_t result = opA + opB + 1; //using the formula for subtraction = ~(opA) + opB + 1 = opA - opB

    // update result & flags
    cpu_set_gpr(decoded->Rd, result);
    setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);
    if (!decoded->isAddrWord) {
        do_nflag(result);
        do_zflag(result);
        do_cflag(opA, opB, 1);
        do_vflag((opA), opB, result);
    }
    else {
        do_nflagx(result);
        do_zflagx(result);
        do_cflagx(opA, opB, 1);
        do_vflagx((opA), opB, result);
    }

    fprintf(stderr, "Result: 0x%4.4x (%u) C flag: %u N flag: %u, V flag: %u, Z flag: %u\n", result, result, cpu_get_flag_c(), cpu_get_flag_n(), cpu_get_flag_v(), cpu_get_flag_z());
    for (int i =0; i<16; i++) {
        fprintf(stderr, "  Register %d : 0x%4.4x (%u)\n", i, cpu.gpr[i], cpu.gpr[i]);
    }

    // update Rs if it's in autoincrement mode
    updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte); //suba should never be in autoincrement mode

    return 1;
}
uint32_t adda (decode_result const* decoded) {
    TRACE_INSTRUCTION("adda %s%u %d, %s%u %d\n", addrModeString[decoded->As].c_str(), decoded->Rs, decoded->srcWord,
                      addrModeString[decoded->Ad].c_str(), decoded->Rd, decoded->dstWord);
    // compute
    int32_t opA = getValue(decoded->As, decoded->Rs, decoded->srcWord, decoded->isByte, decoded->isAddrWord, true);
    int32_t opB = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
    int32_t result = opA + opB;

    // update result & flags
    setValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, result);
    if (!decoded->isAddrWord) {
        do_nflag(result);
        do_zflag(result);
        do_cflag(opA, opB, 0);
        do_vflag(opA, opB, result);
    }
    else {
        do_nflagx(result);
        do_zflagx(result);
        do_cflagx(opA, opB, 0);
        do_vflagx(opA, opB, result);
    }

    fprintf(stderr, "Result: 0x%4.4x (%u) C flag: %u N flag: %u, V flag: %u, Z flag: %u\n", result, result, cpu_get_flag_c(), cpu_get_flag_n(), cpu_get_flag_v(), cpu_get_flag_z());
    for (int i =0; i<16; i++) {
        fprintf(stderr, "  Register %d : 0x%4.4x (%u)\n", i, cpu.gpr[i], cpu.gpr[i]);
    }

    // update Rs if it's in autoincrement mode
    updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte); //adda should never be in autoincrement mode

    return 1;
}
uint32_t calla(decode_result const*decoded) {

    uint32_t opA = getValue(decoded->Ad, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
    uint32_t sp;
    uint32_t pc;

    sp = cpu_get_sp() - 2;
    cpu_set_sp(sp);

    pc = cpu_get_pc();
    store(sp, ((pc>>16)&0xF), false); //store the upper bits of the pc

    sp = cpu_get_sp()-2;
    cpu_set_sp(sp);

    store(sp, (pc&0xFFFF), false);//store the low bits of the pc

    // update result
    cpu_set_pc(opA);

    ////// update Rs if it's in autoincrement mode NEEDS TO DO THIS PART
    ////updateAutoIncrementReg(decoded->As, decoded->Rs, decoded->isAddrWord, decoded->isByte);


    return 1;
}


}