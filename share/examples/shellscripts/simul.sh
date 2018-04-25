#! /bin/bash
export -n DISPLAY
export ENERGYLOSS=1
START=`date`
nice Simul++ 0-3   100000000 Simulation-0-3.his
echo
nice Simul++ 0.5-3  50000000 Simulation-0.5-3.his
echo
nice Simul++ 1-3    25000000 Simulation-1-3.his
echo
echo    "Started:  "$START
echo -n "Finished: "
date
