#!/bin/bash

if [[ $# < 1 ]]; then
  echo "Usage: $0 <JSON_PARSER>"
  exit 1
fi

CMD=$1

if [ ! $(which $CMD 2> /dev/null) ]; then
  echo "ERROR: '$CMD' is not a valid command"
  exit 1
fi

RUNS=10

echo "file,size[MB],time[s],speed[MB/s]"

for file in *.json; do
    tstart=$(date +%s.%N)
    for ((i=0; i<$RUNS; i++)); do
        cat $file | $CMD 1> /dev/null
    done
    tstop=$(date +%s.%N)
    elapsed=$(bc -l <<< "scale=3; ($tstop-$tstart)/$RUNS")
    fsize=$(du -b $file | awk '{print $1}')
    fsize_mb=$(bc -l <<< "scale=3; $fsize/(1024^2)")
    speed=$(bc -l <<< "scale=3; $fsize_mb/$elapsed")
    echo "$file,$fsize_mb,$elapsed,$speed"
done

exit 0
