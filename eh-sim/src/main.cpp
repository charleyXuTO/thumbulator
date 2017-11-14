#include <argagg/argagg.hpp>

#include <fstream>
#include <iostream>

#include "simulate.hpp"

void print_usage(std::ostream &stream, argagg::parser const &arguments)
{
  argagg::fmt_ostream help(stream);

  help << "Simulate an energy harvesting environment.\n\n";
  help << "simulate [options] ARG [ARG...]\n\n";
  help << arguments;
}

void validate(argagg::parser_results const &options)
{
  if(options["binary"].count() == 0) {
    throw std::runtime_error("Missing path to application binary.");
  }

  auto const path_to_binary = options["binary"].as<std::string>();
  std::ifstream binary_file(path_to_binary, std::ios::binary);
  if(!binary_file.good()) {
    throw std::runtime_error("File does not exist: " + path_to_binary);
  }
}

int main(int argc, char *argv[])
{
  argagg::parser arguments{{{"help", {"-h", "--help"}, "display help information", 0},
      {"binary", {"-b", "--binary"}, "path to application binary", 1}}};

  try {
    auto const options = arguments.parse(argc, argv);
    if(options["help"]) {
      print_usage(std::cout, arguments);
      return EXIT_SUCCESS;
    }

    validate(options);

    auto const path_to_binary = options["binary"];
    auto const stats = ehsim::simulate(path_to_binary);

    std::cout << "Cycle count: " << stats.cpu.cycle_count << "\n";
    std::cout << "Instructions executed: " << stats.cpu.instruction_count << "\n";

  } catch(std::exception const &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
