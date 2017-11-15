cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(
  eh-sim
  VERSION 0.0.1
  LANGUAGES CXX
)

add_executable(
  ${PROJECT_NAME}
  src/capacitor.hpp
  src/checkpoint_scheme.hpp
  src/main.cpp
  src/msp430_energy.hpp
  src/simulate.cpp
  src/simulate.hpp
  src/voltage_trace.cpp
  src/voltage_trace.hpp
)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE argagg
  PRIVATE thumbulator
)

set_target_properties(
  ${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
)
