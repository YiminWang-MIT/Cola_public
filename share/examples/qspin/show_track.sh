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
./qspinSpecA -showbound
./qspinSpecA $DP $THETA $Y0 $PHI $PREF $SX $SY $SZ
awk '{printf("%8.2f %8.2f %8.2f %8.2f %8.2f %8.2f\n",$5, -1.0*$3, 1.0*$4, 5.0*$11, -5.0*$9, 5.0*$10);}' qspin.dat > ps/qspin.tmp

# --- RAYTRACE
[ -e ps/ray.tmp ] && rm ps/ray.tmp
[ -e ps/ray.dat ] && rm ps/ray.dat
[ -e ps/cvb.tmp ] && rm ps/cvb.tmp
if [ -x $HOME/bin/run_ray.sh ]
then
    cd ps
    pwd
    echo $DP $THETA $Y0 $PHI
    $HOME/bin/run_ray.sh $DP $THETA $Y0 $PHI
    test -e ray.dat && tac ray.dat | \
	awk 'BEGIN {a = 0;} {a += 1; if ( a > 1 ) print}' > cvb.tmp
    test -e cvb.tmp && tac cvb.tmp | \
	awk 'BEGIN {a = 0;}
	     {a += 1; if ( a > 1 ) printf("%11.3f %11.3f %11.3f\n",
		(10.0*$3), (10.0*$2), (-10.0*$1))}' > ray.tmp
    cd ..
fi
# --- Inv. TMA
#Test_InvTMA $DP $THETA $Y0 $PHI -abscoord | \
#  awk '{printf("%8.2f %8.2f %8.2f\n",$3, -1.0*$1, 1.0*$2);}' > ps/tma_pos.dat

SPINCOLOR="00cc00"
TRACKCOLOR="000000"
INVTMACOLOR="00ffff"
TRACKFILE="qspin.dps"
RAYTC="ff00aa"
SHAPECOLOR="ffaa00"
ENSHAPECOLOR="ffee00"
EXSHAPECOLOR="ddbb00"


data2ps -noframe \
-portrait \
-x -300 8500 \
-y -2200 11000 \
-addarrows dat/vdc_origin.dat 4 1 2 3 4 1 0000ff 0.7 0  \
-o ps/vdc_coord.ps

SPECA_PS=`dirname $0`/A_grey.ps
echo $SPECA_PS
test -f $SPECA_PS || SPECA_PS=$HOME/Bilder/A_grey.ps

data2ps -f ps/qspin.tmp \
-portrait \
-arrows 1 $SPINCOLOR 2 10 \
-title "" \
-columns 6 1 2 4 5 \
-x -300 8500 \
-y -2200 11000 \
-ytics -3000 11000 14 9 \
-xticscale  0.001 \
-yticscale -0.001 \
-import $SPECA_PS 1.7 2.03 0.853 0.853 0.0 \
-import "ps/vdc_coord.ps" 0 0 1 1 0 \
-addsymbols "ps/tma_pos.dat" 3 1 2 -1.5 -1 $INVTMACOLOR \
-addcline ps/qspin.tmp 6 1 2 "[] 0" "000000" 0.3 \
-addcline dat/quad_En.dat 2 1 2 "[] 0" $ENSHAPECOLOR 1.4 \
-addcline dat/quad_Ex.dat 2 1 2 "[] 0" $EXSHAPECOLOR 1.4 \
-addcline dat/quad.dat 2 1 2 "[] 0" $SHAPECOLOR 1.4 \
-addcline dat/sext_En.dat 2 1 2 "[] 0" $ENSHAPECOLOR 1.4 \
-addcline dat/sext_Ex.dat 2 1 2 "[] 0" $EXSHAPECOLOR 1.4 \
-addcline dat/sext.dat 2 1 2 "[] 0" $SHAPECOLOR 1.4 \
-addcline dat/d1_En.dat 2 1 2 "[] 0" $ENSHAPECOLOR 1.4 \
-addcline dat/d1_Ex.dat 2 1 2 "[] 0" $EXSHAPECOLOR 1.4 \
-addcline dat/d1.dat 2 1 2 "[] 0" $SHAPECOLOR 1.4 \
-addcline dat/d2_En.dat 2 1 2 "[] 0" $ENSHAPECOLOR 1.4 \
-addcline dat/d2_Ex.dat 2 1 2 "[] 0" $EXSHAPECOLOR 1.4 \
-addcline dat/d2.dat 2 1 2 "[] 0" $SHAPECOLOR 1.4 \
-addcline "ps/ray.tmp" 3 1 3 "[2 2] 0" $RAYTC 0.3 \
-text "`head -1 $TRACKFILE |tail -1`" 14 4.2 20.5 l \
-text "`head -2 $TRACKFILE |tail -1`" 12 4.2 19.8 l \
-text "`head -3 $TRACKFILE |tail -1`" 12 4.2 19.2 l \
-text "`head -4 $TRACKFILE |tail -1`" 12 4.2 18.6 l \
-text "`head -5 $TRACKFILE |tail -1`" 12 4.2 18 l \
-text "`head -6 $TRACKFILE |tail -1`" 12 4.2 17.2 l \
-text "`head -7 $TRACKFILE |tail -1`" 12 4.2 16.6 l \
-text "`head -8 $TRACKFILE |tail -1`" 14 6.2 11.3 c \
-text "`head -9 $TRACKFILE |tail -1`" 12 6.2 10.7 c \
-fkttext "`head -10 $TRACKFILE |tail -1`" 14 1200 1200 c $TRACKCOLOR \
-fkttext "`head -12 $TRACKFILE |tail -1`" 14 4500 10500 c 000000 \
-fkttext "`head -13 $TRACKFILE |tail -1`" 12 4650 10150 c 000000 \
-fkttext "`head -14 $TRACKFILE |tail -1`" 14 4500 9700 c $TRACKCOLOR \
-o ps/qspin_side.ps
# -fkttext "B{_L} switched off !!" 14 6500 870 c ff0000 \

data2ps -f ps/qspin.tmp \
-portrait \
-height 2.3 \
-arrows 1 $SPINCOLOR 2 10 \
-title "" \
-columns 6 1 3 4 6 \
-x -300 8500 \
-xtics -1000 10000 -11 9 \
-y -410 410 \
-ytics -600 600 4 2 \
-xticscale  0.001 \
-yticscale  0.001 \
-addsymbols "ps/tma_pos.dat" 3 1 3 -1.5 -1 $INVTMACOLOR \
-addcline ps/qspin.tmp 6 1 3 "[] 0" "000000" 0.3 \
-addcline "ps/ray.tmp" 3 1 2 "[2 2] 0" $RAYTC 0.3 \
-o ps/qspin_top.ps

data2ps -f ps/qspin.tmp \
-portrait \
-width 2.3 \
-arrows 1 $SPINCOLOR 2 10 \
-title "" \
-columns 6 3 2 6 5 \
-x -410 410 \
-xtics -600 600 4 2 \
-y -2200 11000 \
-ytics -3000 11000 -14 9 \
-xticscale  0.001 \
-yticscale -0.001 \
-addsymbols "ps/tma_pos.dat" 3 3 2 -1.5 -1 $INVTMACOLOR \
-addcline ps/qspin.tmp 6 3 2 "[] 0" "000000" 0.3 \
-addcline "ps/ray.tmp" 3 2 3 "[2 2] 0" $RAYTC 0.3 \
-o ps/qspin_front.ps

SCALE=0.94
# -text "QSPIN in Spek A" 18 10 27.4 c \

data2ps -noframe \
-portrait \
-box 1.1 2.1 19.88 27.45 ffffff 1.3 000000 \
-BoundingBox 1.0 2.0 19.98 27.55 \
-drawline 17.3 4.3 17.4 4.3 $RAYTC 2 \
-drawline 17.5 4.3 17.6 4.3 $RAYTC 2 \
-ctext "RAYTRACE track" "Helvetica-Narrow-Bold" 7 17.7 4.23 l "000000" \
-drawline 17.3 4.0 17.6 4.0 $TRACKCOLOR 2 \
-ctext "QSPIN track" "Helvetica-Narrow-Bold" 7 17.7 3.93 l "000000" \
-drawline 17.3 3.7 17.5 3.7 $SPINCOLOR 2 \
-triang  17.5 3.6 17.6 3.7 17.5 3.8 $SPINCOLOR 0 $SPINCOLOR \
-ctext "QSPIN spin" "Helvetica-Narrow-Bold" 7 17.7 3.63 l "000000" \
-circle 17.45 3.4 0.1 $INVTMACOLOR 0 "000000" \
-ctext "Inv. TMA" "Helvetica-Narrow-Bold" 7 17.7 3.33 l "000000" \
-ctext "show_track.sh" "Helvetica-Narrow-Oblique" 9 19.4 2.8 r "000000" \
-ctext "`date`" "Helvetica-Narrow-Oblique" 5 19.4 2.6 r "000000" \
-import "ps/qspin_side.ps" -0.6 2 $SCALE $SCALE 0 \
-import "ps/qspin_front.ps" 8.0 2 $SCALE $SCALE 0 \
-import "ps/qspin_top.ps" -0.6 -10.7 $SCALE $SCALE 0 \
-o ps/qspin.ps

[ -e ps/qspin.ps ] && ghostview -magstep -1 ps/qspin.ps &
# ghostview ps/qspin_side.ps &
