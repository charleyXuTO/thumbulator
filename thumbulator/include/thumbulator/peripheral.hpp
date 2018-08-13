//
// Created by d on 8/13/18.
//

#ifndef ENERGY_HARVESTING_PERIPHERAL_HPP
#define ENERGY_HARVESTING_PERIPHERAL_HPP

#endif //ENERGY_HARVESTING_PERIPHERAL_HPP


#include <cstdint>
#include <functional>
namespace thumbulator {

    enum HWregister {
        MPY = 0x00,
        MPYS = 0x02,
        MAC = 0x04,
        MACS = 0x06,
        OP2 = 0x08,
        RESLO = 0x0A,
        RESHI = 0x0C,
        SUMEXT = 0x0E,
        MPY32L = 0x10,
        MPY32H = 0x12,
        MPYS32L = 0x14,
        MPYS32H = 0x16,
        MAC32L = 0x18,
        MAC32H = 0x1A,
        MACS32L = 0x1C,
        MACS32H = 0x1E,
        OP2L = 0x20,
        OP2H = 0x22,
        RES0 = 0x24,
        RES1 = 0x26,
        RES2 = 0x28,
        RES3 = 0x2A,
        MPY32CTl0 = 0x2C,
    };

    enum HWoperations {
        unsignedmultiply = 0,
        signedmultiply = 1,
        unsignedmultiplyaccummulate=2,
        signedmultiplyaccumulate = 3,
    };


uint16_t hardwareMultiplier(uint32_t value, uint32_t address);

}