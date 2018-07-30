#!/bin/bash
traces='trace1.txt trace2.txt trace3.txt trace4.txt trace5.txt'

for trace in $traces
do
    printf "%s:" "$trace"
    ./eh-sim --voltage-trace ../voltageTraces/$trace --voltage-rate 1 --scheme clank -b ../../../MiBench2/simple/main.bin --apb 0 --rde 0 --lc 0 --nwfo 0 --wbb 0 

done

