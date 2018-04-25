#! /usr/local/bin/bash

# DATA=/scratch/a1a3/merkel/Setting-2-4
# DATA=offline2/Setting-2-2
# DATA=/data/kpha1/offline4/merkel/Setting-2-1
DATA=/data/kpha1/online0/filtered

export -n DISPLAY
for file in `ls -t $DATA/coinc*.gz $DATA/backg*.gz`
do
	nice Cola++ -zf $file > /dev/null
	BASE=`basename $file`
	NEWFILE=`echo $BASE | awk 'FS="." {print $1}'`.his
	cp Online.his $DATA/$NEWFILE
	echo $BASE "->" $NEWFILE
done
