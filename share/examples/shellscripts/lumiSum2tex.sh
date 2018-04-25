#! /bin/bash
#! /bin/bash

cat <<EOF
\NeedsTeXFormat{LaTeX2e}
\documentclass[12pt]{article}
\usepackage{isolatin1}
\usepackage{german,a4}
\pagestyle{empty}
\setlength\textwidth{      18cm}
\setlength\textheight{     26cm}
\setlength\topmargin{     -25mm}
\setlength\evensidemargin{ -5mm}
\setlength\oddsidemargin{  -5mm}
\begin{document}
EOF
echo "\\centerline{\bf\Large Setting "$1"}" 
cat <<EOF
\vspace{2mm}
\begin{tabular}{|l|r|r|r|c|c|r|}
\hline
Run & 
Events & 
$\int L\cdot dt$ &
$\int L_{cor}\cdot dt$ 
& $\bar{I}$ & Deadtime & t \\\\ 
 &       & $ [pb^{-1}]$ &$ [pb^{-1}]$ & $ [\mu A]$ & $ [\%]$ & s \\\\ 
\hline
\hline 
EOF
SUMEVENTS=0
SUMLUMINO=0
SUMLUMCOR=0
SUMCURRENT=0
SUMDEAD=0
SUMTIME=0
N=0
for f in `grep run_ runlist.txt | awk '{print $1}'`
do  
    if grep  $f runlist.txt | grep $1 | grep -q data
    then 
      if  ls */luminosity/$f.lumi &> /dev/null 
      then 
        LUMI=*/luminosity/$f.lumi
        EVENTS=`awk '$3==":" && $2=="events" {print $4}' <$LUMI`
        LUMINO=`awk  '$2=="AB" || $2=="AC" {print $7}' <$LUMI`
        CURRENT=`awk '$2=="AB" || $2=="AC" {print $6}' <$LUMI`
        DEAD=`awk    '$2=="AB" || $2=="AC" {print $4}' <$LUMI`
        TIME=`awk    '$2=="AB" || $2=="AC" {print $3}' <$LUMI`
        LUMCOR=`awk  '$2=="AB" || $2=="AC" {printf "%f",$7*(1-$4/100)}' <$LUMI`
        printf "%s & %7d & %7.0f & %7.0f & %5.3f & %4.1f & %7.2f \\\\\\\\\n" \
	       "`echo $f | sed 's/run_/Run /'`" \
		$EVENTS $LUMINO $LUMCOR $CURRENT $DEAD $TIME
    SUMEVENTS=$[SUMEVENTS+EVENTS]
    SUMLUMINO=`echo  | awk '{printf "%f", '$SUMLUMINO'  + '$LUMINO'}'`
    SUMLUMCOR=`echo  | awk '{printf "%f", '$SUMLUMCOR'  + '$LUMCOR'}'`
    SUMCURRENT=`echo | awk '{printf "%f", '$SUMCURRENT' + '$CURRENT'}'`
    SUMDEAD=`echo    | awk '{printf "%f", '$SUMDEAD'    + '$DEAD'}'`
    SUMTIME=`echo    | awk '{printf "%f", '$SUMTIME'    + '$TIME'}'`
    N=$[N+1]
      else
        echo $f" missing "
#        grep  $f runlist.txt
      fi
    fi
done
SUMCURRENT=`echo | awk '{print '$SUMCURRENT' / '$N'}'`
SUMDEAD=`echo    | awk '{print '$SUMDEAD'    / '$N'}'`
SUMTIME=`echo    | awk '{printf "%dh %dm %ds",\
                                ('$SUMTIME'/3600), \
                                ('$SUMTIME'/60)%60, \
                                '$SUMTIME'%60}'`
echo "\\hline"
echo -n "$\\Sigma:$"
printf "%s & %d & %.0f & %.0f & %.3f & %.1f & %s \\\\\\\\\n" \
  $N" Runs" $SUMEVENTS $SUMLUMINO $SUMLUMCOR $SUMCURRENT $SUMDEAD "$SUMTIME"
cat <<EOF
\hline
\end{tabular}
\end{document}
EOF







