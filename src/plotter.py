#!/usr/bin/env python3

from subprocess import run as sp_run, CompletedProcess as sp_CompletedProcess 
import csv
from matplotlib import pyplot as plt

CC = "gcc"

LINKEDSRCS = ["common.c"]  # Add source files to be linked with your client if needed, 
                            # otherwise clear this list

BINARY_DIR = "./"  # The binary will be stored inside the current directory

def compile(filename: str) -> str:
    """ Function to compile the provided file in gcc """

    binary_filename = filename.split(".")[0]  # Omit `.c`

    print(f"Creating binary: {binary_filename} From source file: {filename}\n")

    sp_run([CC, *LINKEDSRCS, "-o", binary_filename, filename]) # Compile

    return BINARY_DIR + binary_filename

def run_binary(binary_filename: str) -> sp_CompletedProcess:
    """ Runs the provided binary """

    print(f"Running binary: {binary_filename}\n")

    return sp_run([binary_filename], capture_output=True, text=True)


if __name__ == "__main__":
    import plotly.express as px

    binary_path: str = compile("client.c")  # Name of the client source file

    running_benchmark: sp_CompletedProcess = run_binary(binary_path)

    ##print(f"Captured STDOUT: \n{running_benchmark.stdout}\n")
    ##print(f"Captured STDERR: \n{running_benchmark.stderr}\n")

    plots = csv.reader(running_benchmark.stdout.splitlines(), delimiter='\t')

    x = []
    y = []

    for row in plots:
        print(row[0],row[1])
        x.append(row[0])
        y.append(row[1])
  
    fig = px.line(x = x, y = y, title='Throughput as function of data size over TCP')
    fig.write_image("matplotlib.png")
    fig.show()

    plt.bar(x, y, color = 'g', width = 0.72, label = "Mbps")
    plt.xlabel('Data size')
    plt.ylabel('Throughput')
    plt.title('Throughput as function of data size over TCP')
    plt.legend()
    plt.savefig("matplotlib.png")  #savefig, don't show
    plt.show()
