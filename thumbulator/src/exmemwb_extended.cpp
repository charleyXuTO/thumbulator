#include "thumbulator/memory.hpp"
#include "cpu_flags.hpp"
#include "trace.hpp"
#include "utils.hpp"

namespace thumbulator {

uint32_t rrum(decode_result const* decoded) {
    uint32_t opA = getValue(0, decoded->Rd, decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
    uint32_t carry = cpu_get_flag_c();
    uint16_t lsb = opA & 0x1;
    int32_t result = opA >> decoded->dstWord;
    if (!decoded->isAddrWord) {
        result |= (carry << 15);
    }
    else {
        result |= (carry << 19);
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

    return 1;
}
uint32_t pushm (decode_result const* decoded ) {

    int32_t opA;
    uint32_t sp;
    for (int n = 0 ; n<decoded->dstWord; n++) {
        opA = getValue(0, (decoded->Rd-n), decoded->dstWord, decoded->isByte, decoded->isAddrWord, false);
        if (!decoded->isAddrWord) {
            opA = opA & 0xFFFF;
            sp = cpu_get_sp()-2;
            cpu_set_sp(sp);
            store(sp, opA, decoded->isByte);

        }
        else {
            opA = opA & 0xFFFFF;
            sp= cpu_get_sp()-2;
            cpu_set_sp(sp);
            store(sp, ((opA>>16) & 0xF), decoded->isByte);
            sp = cpu_get_sp()-2;
            cpu_set_sp(sp);
            store(sp, (opA & 0xFFFF), decoded->isByte);

        }

    }
    return 1;
}
uint32_t popm (decode_result const* decoded) {

    uint32_t opA150;
    uint32_t opA1916;
    uint32_t opA;
    uint32_t sp;
    for (int n = 0; n< decoded->dstWord;n++) {
        if (decoded->isAddrWord) {
            sp = cpu_get_sp();
            load(sp,&opA1916,0);
            sp= cpu_get_sp()+2;
            cpu_set_sp(sp);
            load(sp, &opA150,0);
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
    return 1;

}


}