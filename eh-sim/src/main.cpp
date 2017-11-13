#include <cstdio>
#include <cstring>

#include <iostream>

#include "simulate.hpp"

int main(int argc, char *argv[])
{
  if(argc < 2) {
    std::fprintf(stderr, "Usage: %s [-g] memory_file\n", argv[0]);
    return 1;
  }

  char *file = nullptr;
  if(argc == 2) {
    file = argv[1];
  } else if(0 == strncmp("-g", argv[1], strlen("-g"))) {
    file = argv[2];
  }

  auto const stats = ehsim::simulate(file);

  std::cout << "Cycle count: " << stats.cpu.cycle_count << "\n";
  std::cout << "Instructions executed: " << stats.cpu.instruction_count << "\n";

  return 0;
}
