cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(
  thumbulator
  VERSION 0.0.1
  LANGUAGES CXX
)

add_library(
  ${PROJECT_NAME}
  include/thumbulator/cpu.hpp
  include/thumbulator/decode.hpp
  include/thumbulator/memory.hpp
  src/cpu_flags.hpp
  src/decode.cpp
  src/exit.hpp
  src/cpu.cpp
  src/exmemwb_double.cpp
  src/exmemwb_single.cpp
  src/exmemwb_branch.cpp
  src/memory.cpp
  src/trace.hpp
  src/utils.hpp
  src/utils.cpp
        src/exmemwb_extended.cpp)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(
  ${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
)
