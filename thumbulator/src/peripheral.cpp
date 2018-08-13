//
// Created by d on 8/13/18.
//

#include <thumbulator/peripheral.hpp>

namespace thumbulator {


    uint32_t hardwareMutliplierRegister [23];


    uint16_t hardwareMultiplier(uint32_t value, uint32_t address) { //32 HW multiplier
            uint16_t result = 0;
            address= address - 0x4C0;
            switch(address) {
                case (MPY):
                     hardwareMutliplierRegister[address>>1] = value;
                     break;
                case (OP2):
                     hardwareMutliplierRegister[address>>1] = value;
                     break;
                case (RESLO):
                     result = hardwareMutliplierRegister[MPY>>1]*hardwareMutliplierRegister[OP2>>1];
                     hardwareMutliplierRegister[RESLO>>1] = result;
                     break;
            }
            return result;
    }

}
