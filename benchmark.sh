#!/bin/bash

BENCH_DIR=data/benchmark
PROGRAM=./checker
RUNS=10

if [[ $# > 0 ]]; then
    PROGRAM=$1
fi

for file in $BENCH_DIR/*.json; do
    tstart=$(date +%s.%N)
    for ((i=0; i<$RUNS; i++)); do
	    $PROGRAM $file 1> /dev/null
    done
    tstop=$(date +%s.%N)
    elapsed=$(bc -l <<< "scale=3; ($tstop-$tstart)/$RUNS")
    echo "$file,$elapsed"
done
