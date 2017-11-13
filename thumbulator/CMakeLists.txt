cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(
  thumbulator
  VERSION 0.0.1
  LANGUAGES C
)

add_library(
  ${PROJECT_NAME}
  include/thumbulator/cpu.h
  include/thumbulator/decode.h
  include/thumbulator/except.h
  include/thumbulator/exmemwb.h
  include/thumbulator/sim_support.h
  src/decode.c
  src/except.c
  src/exmemwb.c
  src/exmemwb_arith.c
  src/exmemwb_branch.c
  src/exmemwb_logic.c
  src/exmemwb_mem.c
  src/exmemwb_misc.c
  src/sim_support.c
)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(
  ${PROJECT_NAME} PROPERTIES
  C_STANDARD 99
  C_STANDARD_REQUIRED ON
)