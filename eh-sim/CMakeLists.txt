cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(
  eh-sim
  VERSION 0.0.1
  LANGUAGES CXX
)

add_executable(
  ${PROJECT_NAME}
  src/main.cpp
  src/simulate.cpp
  src/simulate.hpp
)

target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE thumbulator
)

set_target_properties(
  ${PROJECT_NAME} PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
)
