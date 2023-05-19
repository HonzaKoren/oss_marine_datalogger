#!/bin/sh

while true
do
    python3 /opt/windvane.perftest.py -d $DESTINATION  -c $COUNT
    sleep 1
done
