#ifndef EH_SIM_MSP430_ENERGY_HPP
#define EH_SIM_MSP430_ENERGY_HPP

namespace ehsim {

// see: https://www.usenix.org/legacy/event/hotpower08/tech/full_papers/ransford/ransford_html/
// units are nJ

constexpr double MSP430_INSTRUCTION_ENERGY = 2.0;

// MSP430_DESTINATION_SOURCE
constexpr double MSP430_REG_REG = 1.1;
constexpr double MSP430_REG_FLASH = 5.2;
constexpr double MSP430_REG_MEM = 6.3;
constexpr double MSP430_MEM_REG = 8.1;
constexpr double MSP430_MEM_FLASH = 11.8;
constexpr double MSP430_MEM_MEM = 11.7;
constexpr double MSP430_FLASH_REG = 461.0;
constexpr double MSP430_FLASH_FLASH = 350.3;
constexpr double MSP430_FLASH_MEM = 1126.2;
}

#endif //EH_SIM_MSP430_ENERGY_HPP
