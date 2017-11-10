cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(
  thumbulator
  VERSION 0.0.1
  LANGUAGES C
)

add_executable(
  ${PROJECT_NAME}
  decode.c
  decode.h
  except.c
  except.h
  exmemwb.c
  exmemwb.h
  exmemwb_arith.c
  exmemwb_branch.c
  exmemwb_logic.c
  exmemwb_mem.c
  exmemwb_misc.c
  sim_main.c
  sim_support.c
  sim_support.h
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
