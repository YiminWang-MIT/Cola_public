#! /bin/bash

PROC=`basename $0`
if [ $# -lt 8 ]; then
        echo
        echo "  usage:   $PROC <dp/%c> <th/mrad> <y0/mm> <ph/mrad> <p_ref/MeV/c>" >&2
        echo "                 <Sx_tg> <Sy_tg> <Sz_tg>" >&2
        echo
        exit 1
fi

DP=$1
THETA=$2
Y0=$3
PHI=$4
PREF=$5
SX=$6
SY=$7
SZ=$8

[ -d ps ] || mkdir ps
[ -d dat ] || mkdir dat

# --- QSPIN
qspinOHIPS -showbound
qspinOHIPS $DP $THETA $Y0 $PHI $PREF $SX $SY $SZ
awk '{printf("%8.2f %8.2f %8.2f %8.2f %8.2f %8.2f\n",$5, -1.0*$3, 1.0*$4, 5.0*$11, -5.0*$9, 5.0*$10);}' qspinOHIPS.dat > ps/qspinOHIPS.tmp

SPINCOLOR="00cc00"
TRACKCOLOR="0000FF"
INVTMACOLOR="00ffff"
TRACKFILE="qspinOHIPS.dps"
RAYTC="ff00aa"
SHAPECOLOR="ffaa00"
ENSHAPECOLOR="ffee00"
EXSHAPECOLOR="ddbb00"

OHIPS_PS=`dirname $0`/ohips.eps
echo $OHIPS_PS
test -f $OHIPS_PS || OHIPS_PS=$HOME/Bilder/ohips.eps

data2ps -f ps/qspinOHIPS.tmp \
-portrait \
-arrows 1 $SPINCOLOR 2 10 \
-title "" \
-columns 6 1 2 4 5 \
-x -300 7800 \
-y -4500 7620 \
-ytics -5000 8000 13 9 \
-xticscale  0.001 \
-yticscale -0.001 \
-import $OHIPS_PS -0.2 -2.5 1.2 1.2 0.0 \
-addcline ps/qspinOHIPS.tmp 6 1 2 "[] 0" $TRACKCOLOR 0.3 \
-addcline dat/OHIPS_Q1_En.dat 2 1 2 "[] 0" $ENSHAPECOLOR 1.4 \
-addcline dat/OHIPS_Q1_Ex.dat 2 1 2 "[] 0" $EXSHAPECOLOR 1.4 \
-addcline dat/OHIPS_Q1.dat 2 1 2 "[] 0" $SHAPECOLOR 1.4 \
-addcline dat/OHIPS_Q2_En.dat 2 1 2 "[] 0" $ENSHAPECOLOR 1.4 \
-addcline dat/OHIPS_Q2_Ex.dat 2 1 2 "[] 0" $EXSHAPECOLOR 1.4 \
-addcline dat/OHIPS_Q2.dat 2 1 2 "[] 0" $SHAPECOLOR 1.4 \
-addcline dat/OHIPS_D1_En.dat 2 1 2 "[] 0" $ENSHAPECOLOR 1.4 \
-addcline dat/OHIPS_D1_Ex.dat 2 1 2 "[] 0" $EXSHAPECOLOR 1.4 \
-addcline dat/OHIPS_D1.dat 2 1 2 "[] 0" $SHAPECOLOR 1.4 \
-text "`head -1 $TRACKFILE |tail -1`" 14 4.2 23.0 l \
-text "`head -2 $TRACKFILE |tail -1`" 12 4.2 22.3 l \
-text "`head -3 $TRACKFILE |tail -1`" 12 4.2 21.7 l \
-text "`head -4 $TRACKFILE |tail -1`" 12 4.2 21.1 l \
-text "`head -5 $TRACKFILE |tail -1`" 12 4.2 20.5 l \
-text "`head -6 $TRACKFILE |tail -1`" 12 4.2 19.7 l \
-text "`head -7 $TRACKFILE |tail -1`" 12 4.2 19.1 l \
-fkttext "`head -8 $TRACKFILE |tail -1`" 14 2200 -3000 c 000000 \
-fkttext "`head -9 $TRACKFILE |tail -1`" 12 2400 -3300 c 000000 \
-fkttext "`head -10 $TRACKFILE |tail -1`" 14 2200 -3700 c $TRACKCOLOR \
-fkttext "`head -12 $TRACKFILE |tail -1`" 14 5500 -3000 c 000000 \
-fkttext "`head -13 $TRACKFILE |tail -1`" 12 5700 -3300 c 000000 \
-fkttext "`head -14 $TRACKFILE |tail -1`" 14 5500 -3700 c $TRACKCOLOR \
-o ps/qspinOHIPS_side.ps
# -fkttext "B{_L} switched off !!" 14 6500 870 c ff0000 \

data2ps -f ps/qspinOHIPS.tmp \
-portrait \
-height 2.3 \
-arrows 1 $SPINCOLOR 2 10 \
-title "" \
-columns 6 1 3 4 6 \
-x -300 7800 \
-xtics -1000 8000 -9 9 \
-y -360 360 \
-ytics -600 600 4 1 \
-xticscale  0.001 \
-yticscale  0.001 \
-addcline ps/qspinOHIPS.tmp 6 1 3 "[] 0" $TRACKCOLOR 0.3 \
-o ps/qspinOHIPS_top.ps

data2ps -f ps/qspinOHIPS.tmp \
-portrait \
-width 2.3 \
-arrows 1 $SPINCOLOR 2 10 \
-title "" \
-columns 6 3 2 6 5 \
-x -360 360 \
-xtics -600 600 4 1 \
-y -4500 7620 \
-ytics -5000 8000 -13 9 \
-xticscale  0.001 \
-yticscale -0.001 \
-addcline ps/qspinOHIPS.tmp 6 3 2 "[] 0" $TRACKCOLOR 0.3 \
-o ps/qspinOHIPS_front.ps

SCALE=0.94
data2ps -noframe \
-portrait \
-box 1.1 2.1 19.88 27.45 ffffff 1.3 000000 \
-BoundingBox 1.0 2.0 19.98 27.55 \
-drawline 17.3 4.0 17.6 4.0 $TRACKCOLOR 2 \
-ctext "QSPIN track" "Helvetica-Narrow-Bold" 7 17.7 3.93 l "000000" \
-drawline 17.3 3.7 17.5 3.7 $SPINCOLOR 2 \
-triang  17.5 3.6 17.6 3.7 17.5 3.8 $SPINCOLOR 0 $SPINCOLOR \
-ctext "QSPIN spin" "Helvetica-Narrow-Bold" 7 17.7 3.63 l "000000" \
-ctext "show_trackOHIPS.sh" "Helvetica-Narrow-Oblique" 9 19.4 2.8 r "000000" \
-ctext "`date`" "Helvetica-Narrow-Oblique" 5 19.4 2.6 r "000000" \
-import "ps/qspinOHIPS_side.ps" -0.6 2 $SCALE $SCALE 0 \
-import "ps/qspinOHIPS_front.ps" 8.0 2 $SCALE $SCALE 0 \
-import "ps/qspinOHIPS_top.ps" -0.6 -10.7 $SCALE $SCALE 0 \
-o ps/qspinOHIPS.ps

[ -e ps/qspinOHIPS.ps ] && ghostview -magstep -1 ps/qspinOHIPS.ps &
