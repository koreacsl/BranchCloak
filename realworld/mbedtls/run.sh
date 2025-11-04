#!/bin/bash
argc=$#

if [ 1 -eq $argc ]
then
    args_1=$1
    echo "sudo passwd : "$args_1

    echo $args_1| sudo -S perf stat -o result_perf_aligned.txt -e page-faults,branch-misses,branches,cache-misses,cache-references,L1-icache-load-misses,L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses ./align/test_suite_mpi >/dev/null

    echo $args_1 | sudo -S perf stat -o result_perf_raw.txt -e page-faults,branch-misses,branches,cache-misses,cache-references,L1-icache-load-misses,L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses ./raw/test_suite_mpi >/dev/null

else
    echo "usage: ./run.sh {sudo passwd}"
fi
