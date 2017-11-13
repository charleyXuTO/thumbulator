cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(
  eh-sim
  VERSION 0.0.1
  LANGUAGES C
)

add_executable(
  ${PROJECT_NAME}
  src/sim_main.c
)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE thumbulator
)

set_target_properties(
  ${PROJECT_NAME} PROPERTIES
  C_STANDARD 99
  C_STANDARD_REQUIRED ON
)
