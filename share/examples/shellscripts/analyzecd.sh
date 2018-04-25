#! /bin/bash

SETUP=$1
RUNLIST=runlist.txt
HIST=$SETUP/full_histograms

export -n DISPLAY

mount /cdrom 2>/dev/null
mkdir -p $HIST
clear
echo "Anlyze script for setup "$1
echo "-------------------------------"
for f in `grep run_ runlist.txt | grep data | grep $SETUP | awk '{print $1}'`
do
    export ONLINE_HIS=$HIST/$f.his
    if [ -f $ONLINE_HIS ]
    then
	echo $f" already done."
    else
	FILE=`find /cdrom -name "*$f*"`
	if [ -f ""$FILE ]
	then
	    echo "Analyzing "$f" ..."
	    nice Cola++ -zf $FILE
	    echo "done."
	else
	    echo $f" not found on this CD"
	fi
    fi
done
