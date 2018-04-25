BEGIN {
  print "    \\begin{tabular}{|c|r|r|r|r|r|}\\hline";
  printf "      run&Koinzidenzen&Me{\\ss}zeit&Totzeit&";
  print "Strahlstrom&eff. Ladung \\\\ \\hline \\hline";
}{
  if (FNR == 1) { run=$1; }
  if ($1 == "dE/ToF") {
    if ($2 == "A") { coincA=$4 }
    if ($2 == "B") { coincB=$4 }
  }
  if ($1 == "coinc") {
    if ($2 == "AB") { coincAB=$4 }
  }
  if ($1 == "realtime") {
    if ($2 == "A") { realA=$4 }
    if ($2 == "B") { realB=$4 }
  }
  if ($1 == "runtime") {
    if ($2 == "A") { runA=$4 }
    if ($2 == "B") { runB=$4 }
  }
  if ($1 == "deadtime") {
    if ($2 == "A")  { deadA=$4  }
    if ($2 == "B")  { deadB=$4  }
    if ($2 == "AB") { deadAB=$4 }
  }
  if ($1 == "foerster") {
    if ($2 == "A") { foersterA=$4 }
  }
  if ($1 == "photo") {
    if ($2 == "A") { photoA=$4    }
  }
  if ($1 == "faraday") {
    if ($2 == "A") { faradayA=$4  }
  }
  if ($1 == "single") {
    if ($2 == "A")  { singleA=$4  }
    if ($2 == "B")  { singleB=$4  }
  }
  if ($1 == "laser") {
    if ($2 == "AB") { laser=$4 }
  }
  if ($1 == "event") {
    if ($2 == "AB") { coinc=$4 }
  }
  if ($1 == "total") {
    if ($3 >= singleA+singleB+laser+coinc) {
      coincSum += coinc;
      runAsum += runA;
      deadtime = deadA+deadB-deadAB+(coincA+coincB-coincAB)*0.0005;
      realtime = runA-deadtime;
      charge += foersterA*realtime/1000;
      printf "%18s&%7d&%6.1f s&%4.1f \\%&%4.1f $\\mu$A&%4.1f mC\\\\ \\hline\n",
	run,coinc,runA,deadtime*100/realtime,foersterA,foersterA*realtime/1000;
    } else {
      print "event count error";
    }
  }
} END {
  printf "      \\hline      &%7d&%6.0f s&%8s%12s%4.0f mC\\\\ \\hline\n",
	coincSum,runAsum,"&","&",charge;
  print "    \\end{tabular}";
}
