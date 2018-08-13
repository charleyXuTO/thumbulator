//
// Created by d on 8/13/18.
//

#include "thumbulator/peripheral.hpp"
#include <cstdint>
#include <functional>
#include "exit.hpp"

namespace thumbulator {


    uint16_t hardwareMultiplierRegister [23];
    int HWoperation;
    int bit;
    void hardwareMultiplierError() {
        fprintf(stderr, "Error: Unsupported HW register");
        terminate_simulation(1);
    }
    void multiplication(int size1, int size2) {
        uint64_t temp = 0;
        uint64_t op1 = 0;
        uint64_t op2 = 0;
        if (HWoperation == unsignedmultiply) {
            if (size1 == 16 && size2 == 16) {
                temp = hardwareMultiplierRegister[MPY >> 1] * hardwareMultiplierRegister[OP2 >> 1];
                hardwareMultiplierRegister[RESLO>>1] = temp & 0xFFFF;
                hardwareMultiplierRegister[RESHI>>1] = (temp >> 16) & 0xFFFF;
            }
            else if (size1 == 32 && size2 == 32) {
                op1 = (hardwareMultiplierRegister[MPY32H >> 1] << 16) ^ (hardwareMultiplierRegister[MPY32L >> 1]);
                op2 = (hardwareMultiplierRegister[OP2H >> 1] << 16) ^ (hardwareMultiplierRegister[OP2L >> 1]);
                temp = op1*op2;

                hardwareMultiplierRegister[RES0 >> 1] = temp & 0xFFFF;
                hardwareMultiplierRegister[RES1 >> 1] = (temp >> 16) & 0xFFFF;
                hardwareMultiplierRegister[RES2 >> 1] = (temp >> 32) & 0xFFFF;
                hardwareMultiplierRegister[RES3 >> 1] = (temp >> 48) & 0xFFFF;
            }
            else {
                hardwareMultiplierError();
            }
            //TODO: check for overflow and signed business
        }
        else if (HWoperation == signedmultiply)  {
            if (size1 == 16 && size2 == 16) {
                temp = (int16_t)hardwareMultiplierRegister[MPYS >>1]*(int16_t)hardwareMultiplierRegister[OP2>>1];
                hardwareMultiplierRegister[RESLO>>1] = temp & 0xFFFF;
                hardwareMultiplierRegister[RESHI>>1] = (temp >> 16) & 0xFFFF;
            }
            else {

                hardwareMultiplierError();
            }
            //TODO: check for overflow and signed business
        }
        else if (HWoperation == unsignedmultiplyaccummulate) {
            hardwareMultiplierError();
        }
        else if (HWoperation == signedmultiplyaccumulate) {
            hardwareMultiplierError();
        }





    }
    uint16_t hardwareMultiplier(uint32_t value, uint32_t address) { //32 HW multiplier

            uint16_t result = 0;
            address= address - 0x4C0;

            switch(address) {
                case MPY: //16 bit multiply operand 1
                     hardwareMultiplierRegister[MPY>>1] = value;
                     HWoperation = unsignedmultiply;
                     bit = 16;
                     break;
                case MPYS: //16 bit signed multiply operand 1
                     hardwareMultiplierError();
                     HWoperation = signedmultiply;
                     bit = 16;
                     break;
                case MAC: //16 bit multiply accumulate operand 1
                     hardwareMultiplierError();
                     HWoperation = unsignedmultiplyaccummulate;
                     bit = 16;
                     break;
                case MACS: //16 bit signed multiply accumulate operand 1
                     hardwareMultiplierError();
                     HWoperation = signedmultiplyaccumulate;
                     bit= 16;
                     break;
                case OP2: //16 bit operand 2
                     hardwareMultiplierRegister[OP2>>1] = value;
                     multiplication(bit, 16);
                     break;
                case RESLO: //16 bit result low word
                     result = hardwareMultiplierRegister[RESLO>>1];
                     break;
                case RESHI: //16 bit result high word
                     result = hardwareMultiplierRegister[RESHI>>1];
                     break;
                case SUMEXT: //16 bit sum extension
                     hardwareMultiplierError();
                     break;
                case MPY32L: //32 bit multiply low word operand 1
                     HWoperation = unsignedmultiply;
                     hardwareMultiplierRegister[MPY32L >> 1] = value;
                     bit = 32;
                     break;
                case MPY32H: //32 bit multiply high word operand 1
                     HWoperation =  unsignedmultiply;
                     hardwareMultiplierRegister[MPY32H >> 1] = value;
                     bit = 32;
                     break;
                case MPYS32L: //32 bit signed multiply low word operand 1
                     hardwareMultiplierError();
                     HWoperation = signedmultiply;
                     break;
                case MPYS32H: //32 bit signed multiply high word operand 1
                     hardwareMultiplierError();
                     HWoperation = signedmultiply;
                     break;
                case MAC32L: //32 bit multiply accumulate low word operand 1
                     hardwareMultiplierError();
                     HWoperation = unsignedmultiplyaccummulate;
                     break;
                case MAC32H: //32 bit multiply accumulate high word operand 1
                     hardwareMultiplierError();
                     HWoperation = unsignedmultiplyaccummulate;
                     break;
                case MACS32L: //32 bit signed multiply accumulate low word operand 1
                     hardwareMultiplierError();
                     HWoperation = signedmultiplyaccumulate;
                     break;
                case MACS32H: //32 bit signed multiply accumulate high word operand 1
                     hardwareMultiplierError();
                     HWoperation = signedmultiplyaccumulate;
                     break;
                case OP2L: //32 bit low word operand 2
                     hardwareMultiplierRegister[OP2L >> 1] = value;
                     break;
                case OP2H: //32 bit high word operand 2
                     hardwareMultiplierRegister[OP2H >> 1] = value;
                     multiplication(bit, 32);
                     break;
                case RES0: //32 bit result least significant word
                     result = hardwareMultiplierRegister[RES0 >> 1];
                     break;
                case RES1: //32 bit result 1
                     result = hardwareMultiplierRegister[RES1 >> 1];
                     break;
                case RES2: //32 bit result 2
                     result = hardwareMultiplierRegister[RES2 >> 1];
                     break;
                case RES3: //32 bit result most significant word
                     result = hardwareMultiplierRegister[RES3 >> 1];
                     break;
                case MPY32CTl0: //MPY32 control 0
                     hardwareMultiplierError();
                     break;
                default:
                     hardwareMultiplierError();
                     break;
            }
            return result;
    }



}
