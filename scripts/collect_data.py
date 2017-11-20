import argparse
import sys
import os
import csv
import random


def read_data(data_file, num_samples):
    with open(data_file) as csvin:
        reader = csv.reader(csvin, skipinitialspace=True)

        row_count = len(list(reader))
        if row_count < num_samples:
            num_samples = row_count

        probability = float(num_samples) / row_count

        sample_count = 0
        samples = []

        # back to the beginning of the file
        csvin.seek(0)
        next(reader)
        for row in reader:
            if random.random() < probability:
                row.append(benchmark)
                row.append(scheme)
                row.append(trace)

                samples.append(row)
                sample_count = sample_count + 1

                if sample_count >= num_samples:
                    break
        writer.writerows(samples)


if __name__ == "__main__":
    p = argparse.ArgumentParser(description='Run eh-sim.')
    p.add_argument('-r', '--root-dir', dest='root_dir', default=None)
    p.add_argument('-n', '--num-samples', dest='num_samples', default=1000)
    p.add_argument('-s', '--scheme', dest='scheme', default='clank')
    p.add_argument('-o', '--output-file', dest='output_file', default='sampled-data.csv')

    (args) = p.parse_args()

    if args.root_dir is None:
        sys.exit("Error: no path given to data's root directory.")

    benchmark_whitelist = ['adpcm_decode', 'adpcm_encode', 'aes', 'crc', 'limits', 'lzfx', 'overflow', 'picojpeg',
                           'randmath', 'rc4', 'regress', 'rsa', 'susan', 'vcflags']

    header = ['id', 'E', 'n_B', 'tau_B', 'e_B', 'alpha_B', 'e_R', 'e_P', 'tau_P', 'tau_D', 'p', 'benchmark', 'vtrace',
              'scheme']

    with open(args.output_file, 'w', newline='') as csvout:
        writer = csv.writer(csvout, delimiter=',')
        writer.writerow(header)

        for benchmark in os.listdir(args.root_dir):
            benchmark_dir = os.path.join(args.root_dir, benchmark)

            for trace in os.listdir(benchmark_dir):
                trace_dir = os.path.join(benchmark_dir, trace)

                for file in os.listdir(trace_dir):
                    scheme = ""
                    if file.endswith('.csv'):
                        scheme = file.replace('-True.csv', '')
                    else:
                        continue

                    if scheme != args.scheme:
                        continue

                    print("Collecting data for (benchmark, voltage trace, scheme): {}, {}, {}".format(benchmark, trace,
                                                                                                      scheme))

                    file = os.path.join(trace_dir, file)
                    read_data(file, args.num_samples)
