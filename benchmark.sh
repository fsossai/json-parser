#/bin/bash

BENCH_DIR=data/benchmark
PROGRAM=./checker
RUNS=10

if [[ $# > 0 ]]; then
    PROGRAM=$1
fi

for f in $BENCH_DIR/*.json; do
    TSTART=$(date +%s.%N)
    for ((i=0; i<$RUNS; i++)); do
	    cat $f | $PROGRAM 1> /dev/null
    done
    TSTOP=$(date +%s.%N)
    ELAPSED=$(bc -l <<< "scale=3; ($TSTOP-$TSTART)/$RUNS")
    echo "$f, $ELAPSED"
done
