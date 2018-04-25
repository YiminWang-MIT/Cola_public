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
qspinOOPS -showbound
qspinOOPS $DP $THETA $Y0 $PHI $PREF $SX $SY $SZ
awk '{printf("%8.2f %8.2f %8.2f %8.2f %8.2f %8.2f\n",$5, -1.0*$3, 1.0*$4, 5.0*$11, -5.0*$9, 5.0*$10);}' qspinOOPS.dat > ps/qspinOOPS.tmp

SPINCOLOR="00cc00"
TRACKCOLOR="0000FF"
INVTMACOLOR="00ffff"
TRACKFILE="qspinOOPS.dps"
RAYTC="ff00aa"
SHAPECOLOR="ffaa00"
ENSHAPECOLOR="ffee00"
EXSHAPECOLOR="ddbb00"

OOPS_PS=`dirname $0`/oops_cut.eps
echo $OOPS_PS
test -f $OOPS_PS || OOPS_PS=$HOME/Bilder/oops_cut.eps

# 6600 8800 -4.0 14 
data2ps -f ps/qspinOOPS.tmp \
-portrait \
-arrows 1 $SPINCOLOR 2 10 \
-title "" \
-columns 6 1 2 4 5 \
-x -200 5200 \
-y -3000 5080 \
-ytics -3000 5000 8 9 \
-xticscale  0.001 \
-yticscale -0.001 \
-import $OOPS_PS 1.30 -2.84 0.925 0.925 0.0 \
-addcline ps/qspinOOPS.tmp 6 1 2 "[] 0" $TRACKCOLOR 0.3 \
-addcline dat/OOPS_Qn.dat 2 1 2 "[] 0" $ENSHAPECOLOR 1.4 \
-addcline dat/OOPS_Qx.dat 2 1 2 "[] 0" $EXSHAPECOLOR 1.4 \
-addcline dat/OOPS_Q.dat 2 1 2 "[] 0" $SHAPECOLOR 1.4 \
-addcline dat/OOPS_Dn.dat 2 1 2 "[] 0" $ENSHAPECOLOR 1.4 \
-addcline dat/OOPS_Dx.dat 2 1 2 "[] 0" $EXSHAPECOLOR 1.4 \
-addcline dat/OOPS_D.dat 2 1 2 "[] 0" $SHAPECOLOR 1.4 \
-text "`head -1 $TRACKFILE |tail -1`" 14 4.2 23.0 l \
-text "`head -2 $TRACKFILE |tail -1`" 12 4.2 22.3 l \
-text "`head -3 $TRACKFILE |tail -1`" 12 4.2 21.7 l \
-text "`head -4 $TRACKFILE |tail -1`" 12 4.2 21.1 l \
-text "`head -5 $TRACKFILE |tail -1`" 12 4.2 20.5 l \
-text "`head -6 $TRACKFILE |tail -1`" 12 4.2 19.7 l \
-text "`head -7 $TRACKFILE |tail -1`" 12 4.2 19.1 l \
-fkttext "`head -8 $TRACKFILE |tail -1`" 14 1200 -2000 c 000000 \
-fkttext "`head -9 $TRACKFILE |tail -1`" 12 1400 -2200 c 000000 \
-fkttext "`head -10 $TRACKFILE |tail -1`" 14 1200 -2500 c $TRACKCOLOR \
-fkttext "`head -12 $TRACKFILE |tail -1`" 14 4000 -2000 c 000000 \
-fkttext "`head -13 $TRACKFILE |tail -1`" 12 4200 -2200 c 000000 \
-fkttext "`head -14 $TRACKFILE |tail -1`" 14 4000 -2500 c $TRACKCOLOR \
-o ps/qspinOOPS_side.ps

data2ps -f ps/qspinOOPS.tmp \
-portrait \
-height 2.3 \
-arrows 1 $SPINCOLOR 2 10 \
-title "" \
-columns 6 1 3 4 6 \
-x -200 5200 \
-xtics -1000 6000 -7 9 \
-y -250 250 \
-ytics -400 400 4 1 \
-xticscale  0.001 \
-yticscale  0.001 \
-addcline ps/qspinOOPS.tmp 6 1 3 "[] 0" $TRACKCOLOR 0.3 \
-o ps/qspinOOPS_top.ps

data2ps -f ps/qspinOOPS.tmp \
-portrait \
-width 2.3 \
-arrows 1 $SPINCOLOR 2 10 \
-title "" \
-columns 6 3 2 6 5 \
-x -250 250 \
-xtics -400 400 4 1 \
-y -3000 5080 \
-ytics -3000 5000 -8 9 \
-xticscale  0.001 \
-yticscale -0.001 \
-addcline ps/qspinOOPS.tmp 6 3 2 "[] 0" $TRACKCOLOR 0.3 \
-o ps/qspinOOPS_front.ps

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
-ctext "show_trackOOPS.sh" "Helvetica-Narrow-Oblique" 9 19.4 2.8 r "000000" \
-ctext "`date`" "Helvetica-Narrow-Oblique" 5 19.4 2.6 r "000000" \
-import "ps/qspinOOPS_side.ps" -0.6 2 $SCALE $SCALE 0 \
-import "ps/qspinOOPS_front.ps" 8.0 2 $SCALE $SCALE 0 \
-import "ps/qspinOOPS_top.ps" -0.6 -10.7 $SCALE $SCALE 0 \
-o ps/qspinOOPS.ps

[ -e ps/qspinOOPS.ps ] && ghostview -magstep -1 ps/qspinOOPS.ps &
