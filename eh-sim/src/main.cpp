#include <argagg/argagg.hpp>

#include <fstream>
#include <iostream>
#include <memory>

#include "scheme/backup_every_cycle.hpp"
#include "scheme/magical_scheme.hpp"
#include "simulate.hpp"
#include "voltage_trace.hpp"

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

  if(options["rate"].count() == 0) {
    throw std::runtime_error("No sampling rate provided for the voltage trace.");
  }
}

int main(int argc, char *argv[])
{
  argagg::parser arguments{{{"help", {"-h", "--help"}, "display help information", 0},
      {"voltages", {"--voltage-trace"}, "path to voltage trace", 1},
      {"rate", {"--voltage-rate"}, "sampling rate of voltage trace (microseconds)", 1},
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
    std::chrono::microseconds sampling_rate(options["rate"]);

    auto scheme = std::make_unique<ehsim::backup_every_cycle>();
    ehsim::voltage_trace power(path_to_voltage_trace, sampling_rate);

    auto const stats = ehsim::simulate(path_to_binary, power, scheme.get(), false);

    std::cout << "CPU instructions executed: " << stats.cpu.instruction_count << "\n";
    std::cout << "CPU time (cycles): " << stats.cpu.cycle_count << "\n";
    std::cout << "Total time (ns): " << stats.system.time.count() << "\n";
    std::cout << "Energy harvested (J): " << stats.system.energy_harvested * 1e-9 << "\n";
    std::cout << "Energy remaining (J): " << stats.system.energy_remaining * 1e-9 << "\n";

    std::ofstream eh_file("eh_model.csv");
    eh_file << "active.id,total.instruction.energy,tau.b\n";
    int id = 0;
    for(auto const &model : stats.models) {
      auto const tau_sum =
          std::accumulate(model.backup_times.begin(), model.backup_times.end(), 0ul);
      auto const tau_b = static_cast<double>(tau_sum) / model.backup_times.size();
      eh_file << id++ << "," << model.instruction_energy << "," << tau_b << "\n";
    }

  } catch(std::exception const &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
