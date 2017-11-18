#ifndef EH_SIM_DATA_SHEET_ENERGY_HPP
#define EH_SIM_DATA_SHEET_ENERGY_HPP

namespace ehsim {

// all energy units are in nJ

// Mementos numbers are for an MSP430F1232 (http://www.ti.com/product/msp430f1232)
// see: https://www.usenix.org/legacy/event/hotpower08/tech/full_papers/ransford/ransford_html/
constexpr uint32_t MEMENTOS_CPU_FREQUENCY = 8000000;
constexpr double MEMENTOS_CAPACITANCE = 10e-6;
constexpr double MEMENTOS_MAX_CAPACITOR_VOLTAGE = 4.5;
constexpr double MEMENTOS_INSTRUCTION_ENERGY = 2.0;
// memory naming scheme is MEMENTOS_DESTINATION_SOURCE
constexpr double MEMENTOS_REG_REG = 1.1;
constexpr double MEMENTOS_REG_FLASH = 5.2;
constexpr double MEMENTOS_REG_MEM = 6.3;
constexpr double MEMENTOS_MEM_REG = 8.1;
constexpr double MEMENTOS_MEM_FLASH = 11.8;
constexpr double MEMENTOS_MEM_MEM = 11.7;
constexpr double MEMENTOS_FLASH_REG = 461.0;
constexpr double MEMENTOS_FLASH_FLASH = 350.3;
constexpr double MEMENTOS_FLASH_MEM = 1126.2;

// Batteryless Sim numbers are from: https://github.com/PERSISTLab/BatterylessSim/
constexpr double BATTERYLESS_CAPACITANCE = 10e-6;
constexpr double BATTERYLESS_MAX_CAPACITOR_VOLTAGE = 7.5;

// NVP numbers from Architecture Exploration for Ambient Energy Harvesting Nonvolatile Processors
// see Section 3 from paper - 8 KHz clock frequency
constexpr uint32_t NVP_CPU_FREQUENCY = 8000;
// see Section V from paper - capacitor used in the system is 470 nF
constexpr double NVP_CAPACITANCE = 470e-9;
// see Figure 11 from paper
constexpr auto NVP_INSTRUCTION_ENERGY = 0.03125;
constexpr auto NVP_ODAB_BACKUP_ENERGY = 0.75;
constexpr auto NVP_ODAB_RESTORE_ENERGY = 0.25;
constexpr auto NVP_BEC_BACKUP_ENERGY = 0.125;
constexpr auto NVP_BEC_RESTORE_ENERGY = 0.25;
// see Figure 10 from paper
constexpr uint64_t NVP_ODAB_BACKUP_TIME = 35;
constexpr uint64_t NVP_ODAB_RESTORE_TIME = 35;
constexpr uint64_t NVP_BEC_BACKUP_TIME = 2;
constexpr uint64_t NVP_BEC_RESTORE_TIME = 1;

// based on Clank: Architectural Support for Intermittent Computation
constexpr uint64_t CLANK_BACKUP_ARCH_TIME = 40;
constexpr double CLANK_INSTRUCTION_ENERGY = MEMENTOS_INSTRUCTION_ENERGY + MEMENTOS_REG_FLASH;
constexpr double CLANK_BACKUP_ARCH_ENERGY = MEMENTOS_FLASH_REG * 20;
constexpr double CLANK_RESTORE_ENERGY = MEMENTOS_REG_FLASH * 20;
}

#endif //EH_SIM_DATA_SHEET_ENERGY_HPP
