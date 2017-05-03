import numpy as np
from sys import argv
import os
# Read sampler takes 3 command line arguments
# module load anaconda/3.latest
# python read_sampler.py <fasta/fastq> <read_file_to_sample> <total_samples_to_take>
if argv[1].lower() == "fasta":
    scalar = 2
else:
    scalar = 4

samples = int(argv[3])
with open(argv[2], "rb") as in_file:
    reads = in_file.readlines()

tot_reads = len(reads)//scalar

samples = np.random.choice(tot_reads,int(argv[3]), False) * scalar
samples.sort()
argv[2] = os.path.basename(argv[2])
with open("_{0}_sampled.".format(argv[3]).join(argv[2].split(".")), "wb" ) as out_file:
    for ind in samples:
        out_file.write(reads[ind])
        for i in range(1, scalar ):
            out_file.write(reads[ind+i])