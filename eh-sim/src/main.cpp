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

void ensure_file_exists(std::string const &path_to_file)
{
  std::ifstream binary_file(path_to_file);
  if(!binary_file.good()) {
    throw std::runtime_error("File does not exist: " + path_to_file);
  }
}

void validate(argagg::parser_results const &options)
{
  if(options["binary"].count() == 0) {
    throw std::runtime_error("Missing path to application binary.");
  }

  auto const path_to_binary = options["binary"].as<std::string>();
  ensure_file_exists(path_to_binary);

  if(options["voltages"].count() == 0) {
    throw std::runtime_error("Missing path to voltage trace.");
  }

  auto const path_to_voltage_trace = options["voltages"].as<std::string>();
  ensure_file_exists(path_to_voltage_trace);
}

int main(int argc, char *argv[])
{
  argagg::parser arguments{{{"help", {"-h", "--help"}, "display help information", 0},
      {"voltages", {"--voltage-trace"}, "path to voltage trace", 1},
      {"binary", {"-b", "--binary"}, "path to application binary", 1}}};

  try {
    auto const options = arguments.parse(argc, argv);
    if(options["help"]) {
      print_usage(std::cout, arguments);
      return EXIT_SUCCESS;
    }

    validate(options);

    auto const path_to_binary = options["binary"];
    auto const path_to_voltage_trace = options["voltages"];
    auto const stats = ehsim::simulate(path_to_binary, path_to_voltage_trace);

    std::cout << "CPU instructions executed: " << stats.cpu.instruction_count << "\n";
    std::cout << "CPU cycle count: " << stats.cpu.cycle_count << "\n";
    std::cout << "Total time (ns): " << stats.system.time.count() << "\n";
    std::cout << "Energy harvested (J): " << stats.system.energy_harvested * 1e-9 << "\n";

  } catch(std::exception const &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
