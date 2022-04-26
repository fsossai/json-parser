#!/bin/bash

BENCH_DIR=data/benchmark
PROGRAM=checker
RUNS=10

if [[ $# > 0 ]]; then
    PROGRAM=$1
fi

if [ ! -e $PROGRAM ]; then
	echo "Program not found: $PROGRAM"
	exit 1
fi

echo "file,size[MB],time[s],speed[MB/s]"

for file in $BENCH_DIR/*.json; do
    tstart=$(date +%s.%N)
    for ((i=0; i<$RUNS; i++)); do
        cat $file | ./$PROGRAM 1> /dev/null
	    # $PROGRAM $file 1> /dev/null
    done
    tstop=$(date +%s.%N)
    elapsed=$(bc -l <<< "scale=3; ($tstop-$tstart)/$RUNS")
    fsize=$(du -b $file | awk '{print $1}')
    fsize_mb=$(bc -l <<< "scale=3; $fsize/(1024^2)")
    speed=$(bc -l <<< "scale=3; $fsize_mb/$elapsed")
    echo "$file,$fsize_mb,$elapsed,$speed"
done
