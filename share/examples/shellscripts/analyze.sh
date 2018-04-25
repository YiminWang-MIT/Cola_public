#! /bin/bash
#! /bin/bash

SETUP=$1
RUNLIST=runlist.txt

mount /cdrom 2>/dev/null
mkdir -p $SETUP/histograms

for f in `grep run_ runlist.txt | grep data | grep $SETUP | awk '{print $1}'`
do

    export ONLINE_HIS=$SETUP/histograms/$f.his
    export -n DISPLAY
    echo "Analyzing "$f" ..."
    nice Cola++ -zf $SETUP/filtered/$f.filtered
    echo "done."
done






