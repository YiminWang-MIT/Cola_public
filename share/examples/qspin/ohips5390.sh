#!/bin/sh

tmp=ohips-2m.tmp
out=ohips-2m.out

for dp in -5 -4 -3 -2 -1 0 1 2 3 4 5
do
    for ph in -21 -14 -7 0 7 14 21
    do
	for th in -55 -44 -33 -22 -11 0 11 22 33 44 55
	do
	    for y0 in -12 -8 -4 0 4 8 12
	    do
		echo dpthy0ph $dp $th $y0 $ph >>$tmp
		./qspinOHIPS $dp $th $y0 $ph 269.1 0 0 0 >>$tmp
		echo >>$tmp
	    done
	done
    done
done

awk '/dpthy0ph/ { dp=$2; x0=0.0; th0=$3; y0=0.1*$4; ph0=$5; }
/th.mrad,/ { x=0.1*$7; th=$8; y=$9; ph=$10; }
/Pathlength/ {
    printf "%4.1f %3.1f %5.1f %4.1f %5.1f %7.2f %5.1f %6.2f %5.1f %7.2f\n",
	dp, x0, th0, y0, ph0, x, th, y, ph, 0.1*$9;
}' <$tmp >$out
