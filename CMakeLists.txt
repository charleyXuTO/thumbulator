cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(
  thumbulator
  VERSION 0.0.1
  LANGUAGES C
)

add_executable(
  ${PROJECT_NAME}
  src/decode.c
  src/decode.h
  src/except.c
  src/except.h
  src/exmemwb.c
  src/exmemwb.h
  src/exmemwb_arith.c
  src/exmemwb_branch.c
  src/exmemwb_logic.c
  src/exmemwb_mem.c
  src/exmemwb_misc.c
  src/sim_main.c
  src/sim_support.c
  src/sim_support.h
)

set_target_properties(
  ${PROJECT_NAME} PROPERTIES
  C_STANDARD 99
  C_STANDARD_REQUIRED ON
)

#find_package(OpenSSL REQUIRED)
#target_link_libraries(
#  ${PROJECT_NAME}
#  PRIVATE ${OPENSSL_LIBRARIES}
#)
