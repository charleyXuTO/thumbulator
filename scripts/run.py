import argparse
import sys
import subprocess
import os


def run(eh_sim, app, trace, rate, s, harvest, out_dir):
    base_name = s + "-" + str(harvest)
    path_to_output = out_dir + "/" + base_name + ".csv"

    to_run = "{} -b{} --voltage-trace={} --voltage-rate={} --scheme={} -o{}".format(eh_sim, app, trace, rate, s,
                                                                                    path_to_output)
    to_run = to_run.split()

    if harvest is True:
        to_run.append('--always-harvest=1')
    else:
        to_run.append('--always-harvest=0')

    stdout_file = open(out_dir + "/" + base_name + ".stdout", "w")
    stderr_file = open(out_dir + "/" + base_name + ".stderr", "w")
    subprocess.run(to_run, stdout=stdout_file, stderr=stderr_file)


if __name__ == "__main__":
    p = argparse.ArgumentParser(description='Run eh-sim.')
    p.add_argument('-x', '--exe', dest='eh_sim', default=None)
    p.add_argument('--benchmark-dir', dest='benchmark_dir', default=None)
    p.add_argument('--voltage-trace-dir', dest="vtrace_dir", default=None)
    p.add_argument('-d', '--destination', dest='output_dir', default=None)

    (args) = p.parse_args()

    if args.eh_sim is None:
        sys.exit("Error: need path to eh-sim executable.")
    if args.benchmark_dir is None:
        sys.exit("Error: no path to benchmarks.")
    if args.vtrace_dir is None:
        sys.exit("Error: no path to voltage traces.")
    if args.output_dir is None:
        sys.exit("Error: no path given to output destination.")

    # the schemes to run
    schemes = ['bec', 'odab', 'clank']

    # the benchmarks to run, from Matthew Hicks' version of MiBench
    benchmark_whitelist = ['adpcm_decode', 'adpcm_encode', 'aes', 'crc', 'limits', 'lzfx', 'overflow', 'picojpeg',
                           'randmath', 'rc4', 'regress', 'rsa', 'susan', 'vcflags']

    # the voltage traces to use, from BatterylessSim
    vtrace_whitelist = ['1', '3', '4', '5', '6', '7', '8', '9']
    # the time between samples of the voltage trace, in microseconds. The longer the time, the smaller the rate
    # of charge per cycle
    vtrace_rates = {'bec': 10000, 'odab': 10000, 'clank': 1000}

    for benchmark in benchmark_whitelist:
        for vtrace in vtrace_whitelist:
            for scheme in schemes:
                print("Running {}.bin with {}.txt voltage trace in {} scheme".format(benchmark, vtrace, scheme))

                path_to_benchmark = args.benchmark_dir + "/" + benchmark + ".bin"
                path_to_vtrace = args.vtrace_dir + "/" + vtrace + ".txt"

                path_to_destination = args.output_dir + "/" + benchmark + "/" + vtrace
                os.makedirs(path_to_destination, exist_ok=True)

                run(args.eh_sim, path_to_benchmark, path_to_vtrace, vtrace_rates[scheme], scheme, True,
                    path_to_destination)
