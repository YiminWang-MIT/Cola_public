#! /usr/local/bin/bash
DATA=/scratch/a1a3/merkel/
export -n DISPLAY
for file in $DATA/run_*
do
	RUN=`gzcat $file | head -c24 | grep 9`
	OUTFILE=$DATA/coinc_$RUN.gz
	Cola++ -zf $file -o$OUTFILE > /dev/null
	FSIN=`ls -l $file | awk '{print $5;}'`
	FSOUT=`ls -l $OUTFILE | awk '{print $5;}'`
	REDUCTION=`echo $FSIN $FSOUT | awk '{print $1/$2}'`
	echo $file "->" coinc_$RUN.gz "(Reduction: " $REDUCTION ")"
done
