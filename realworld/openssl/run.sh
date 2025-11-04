#!/bin/bash
argc=$#

if [ 1 -eq $argc ]
then
    args_1=$1
    echo "sudo passwd : "$args_1

    chrpath -r ./align bntest
    chrpath bntest
    echo $args_1| sudo -S perf stat -o result_perf_aligned.txt -e page-faults,branch-misses,branches,cache-misses,cache-references,L1-icache-load-misses,L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses ./bntest >/dev/null

    chrpath -r ./raw bntest
    chrpath bntest
    echo $args_1 | sudo -S perf stat -o result_perf_raw.txt -e page-faults,branch-misses,branches,cache-misses,cache-references,L1-icache-load-misses,L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses ./bntest >/dev/null
else
    echo "usage: ./run.sh {sudo passwd}"
fi
