BEGIN {
    fnr=3;
    print "# This file was created by GNU Oleo" > "coinc.c00";
    print "# format 1.0"                       >> "coinc.c00";
    print "F;DGFL8"                            >> "coinc.c00";
    print "O;auto;background;noa0;ticks 1"     >> "coinc.c00";
    print "C;c1;r2;K\"run\""                    > "coinc.c01";
    print "F;c2;r2;FDR\nC;K\"scint A\""         > "coinc.c02";
    print "F;r3;FDC\nC;K\"* 1000\""            >> "coinc.c02";
    print "F;c3;r2;FDR\nC;K\"scint B\""         > "coinc.c03";
    print "F;r3;FDC\nC;K\"* 1000\""            >> "coinc.c03";
    print "F;c4;r2;FDR\nC;K\"coinc A/B\""       > "coinc.c04";
    print "F;r3;FDC\nC;K\"* 1000\""            >> "coinc.c04";
    print "F;c5;r2;FDR\nC;K\"deadtime\""        > "coinc.c05";
    print "F;r3;FDC\nC;K\"* 1000\""            >> "coinc.c05";
    print "F;c6;r2;FDR\nC;K\"singles A\""       > "coinc.c06";
    print "F;c7;r2;FDR\nC;K\"singles B\""       > "coinc.c07";
    print "F;c8;r2;FDR\nC;K\"laser events\""    > "coinc.c08";
    print "F;c9;r2;FDR\nC;K\"Coincidences\""    > "coinc.c09";
    print "F;c10;r2;FDC\nC;K\"real time A\""    > "coinc.c10";
    print "F;r3;FDC\nC;K\"[s]\""               >> "coinc.c10";
    print "F;c11;r2;FDC\nC;K\"run time A\""     > "coinc.c11";
    print "F;r3;FDC\nC;K\"[s]\""               >> "coinc.c11";
    print "F;c12;r2;FDC\nC;K\"real time B\""    > "coinc.c12";
    print "F;r3;FDC\nC;K\"[s]\""               >> "coinc.c12";
    print "F;c13;r2;FDC\nC;K\"run time B\""     > "coinc.c13";
    print "F;r3;FDC\nC;K\"[s]\""               >> "coinc.c13";
    print "F;c14;r2;FDC\nC;K\"dead time A\""    > "coinc.c14";
    print "F;r3;FDC\nC;K\"[s]\""               >> "coinc.c14";
    print "F;c15;r2;FDC\nC;K\"dead time B\""    > "coinc.c15";
    print "F;r3;FDC\nC;K\"[s]\""               >> "coinc.c15";
    print "F;c16;r2;FDC\nC;K\"dead time AB\""   > "coinc.c16";
    print "F;r3;FDC\nC;K\"[s]\""               >> "coinc.c16";
    print "F;c17;r2;FDC\nC;K\"foerster A\""     > "coinc.c17";
    print "F;r3;FDC\nC;K\"[uA]\""              >> "coinc.c17";
    print "F;c19;r2;FDC\nC;K\"Totzeit A\""      > "coinc.c19";
    print "F;c20;r2;FDC\nC;K\"Totzeit B\""      > "coinc.c20";
    print "F;c21;r2;FDC\nC;K\"runtime\""        > "coinc.c21";
    print "F;c22;r2;FDC\nC;K\"Ladung\""         > "coinc.c22";
    print "F;r3;FDC\nC;K\"[mC]\""              >> "coinc.c22";
}
{
    if (FNR == 1) { run=$1; fnr=fnr+1 }
    if ($1 == "dE/ToF")
    {
	if ($2 == "A") { coincA=$4 }
	if ($2 == "B") { coincB=$4 }
    }
    if ($1 == "coinc") { if ($2 == "AB") { coincAB=$4 } }
    if ($1 == "realtime")
    {
	if ($2 == "A") { realA=$4 }
	if ($2 == "B") { realB=$4 }
    }
    if ($1 == "runtime")
    {
	if ($2 == "A") { runA=$4 }
	if ($2 == "B") { runB=$4 }
    }
    if ($1 == "deadtime")
    {
	if ($2 == "A")  { deadA=$4  }
	if ($2 == "B")  { deadB=$4  }
	if ($2 == "AB") { deadAB=$4 }
    }
    if ($1 == "foerster") { if ($2 == "A") { foersterA=$4 } }
    if ($1 == "photo")    { if ($2 == "A") { photoA=$4    } }
    if ($1 == "faraday")  { if ($2 == "A") { faradayA=$4  } }
    if ($1 == "single")
    {
	if ($2 == "A")  { singleA=$4  }
	if ($2 == "B")  { singleB=$4  }
    }
    if ($1 == "laser") { if ($2 == "AB") { laser=$4 } }
    if ($1 == "event") { if ($2 == "AB") { coinc=$4 } }
    if ($1 == "total")
    {
	if ($3 >= singleA+singleB+laser+coinc)
	{
	    printf "C;r%d;K\"%s\"\n",fnr,run              >> "coinc.c01";
	    printf "F;r%d;FDR\nC;K%d\n",fnr,coincA        >> "coinc.c02";
	    printf "F;r%d;FDR\nC;K%d\n",fnr,coincB        >> "coinc.c03";
	    printf "F;r%d;FDR\nC;K%d\n",fnr,coincAB       >> "coinc.c04";
	    printf "F;r%d;FF1R\n",fnr                     >> "coinc.c05";
	    deadtime=(coincA+coincB-coincAB)*0.0005;
	    printf "C;E(rc2 + rc3 - rc4) * 0.0005;K%f\n",
			deadtime                          >> "coinc.c05";
	    printf "F;r%d;FDR\nC;K%d\n",fnr,singleA       >> "coinc.c06";
	    printf "F;r%d;FDR\nC;K%d\n",fnr,singleB       >> "coinc.c07";
	    printf "F;r%d;FDR\nC;K%d\n",fnr,laser         >> "coinc.c08";
	    printf "F;r%d;FDR\nC;K%d\n",fnr,coinc         >> "coinc.c09";
	    printf "F;r%d;FF2R\nC;K%.2f\n",fnr,realA      >> "coinc.c10";
	    printf "F;r%d;FF2R\nC;K%.2f\n",fnr,runA       >> "coinc.c11";
	    printf "F;r%d;FF2R\nC;K%.2f\n",fnr,realB      >> "coinc.c12";
	    printf "F;r%d;FF2R\nC;K%.2f\n",fnr,runB       >> "coinc.c13";
	    printf "F;r%d;FF3R\nC;K%.3f\n",fnr,deadA      >> "coinc.c14";
	    printf "F;r%d;FF3R\nC;K%.3f\n",fnr,deadB      >> "coinc.c15";
	    printf "F;r%d;FF3R\nC;K%.3f\n",fnr,deadAB     >> "coinc.c16";
	    printf "F;r%d;FF4R\nC;K%.4f\n",fnr,foersterA  >> "coinc.c17";
	    printf "F;r%d;F%1R\n",fnr                     >> "coinc.c19";
	    printf "C;E(rc11 - rc10) / rc11;K%f\n",
			(runA-realA)/runA                 >> "coinc.c19";
	    printf "F;r%d;F%1R\n",fnr                     >> "coinc.c20";
	    printf "C;E(rc13 - rc12) / rc13;K%f\n",
			(runB-realB)/runB                 >> "coinc.c20";
	    printf "F;r%d;FF1R\n",fnr                     >> "coinc.c21";
	    realtime=runA+deadAB-deadB-deadA-deadtime;
	    printf "C;Erc11 + rc16 - rc15 - rc14 - rc5;K%f\n",
		realtime                                  >> "coinc.c21";
	    printf "F;r%d;FF1R\n",fnr                     >> "coinc.c22";
	    printf "C;Erc17 * 0.001 * rc21;K%f\n",
		foersterA*0.001*realtime                  >> "coinc.c22";
	}
	else { print "event count error" }
    }
}
END {
    printf "F;r%d;FDR\nC;Esum(r4:%dc)\n",fnr+2,fnr  >> "coinc.c02";
    printf "F;r%d;FDR\nC;Esum(r4:%dc)\n",fnr+2,fnr  >> "coinc.c03";
    printf "F;r%d;FDR\nC;Esum(r4:%dc)\n",fnr+2,fnr  >> "coinc.c04";
    printf "F;r%d;FF1R\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c05";
    printf "F;r%d;FDR\nC;Esum(r4:%dc)\n",fnr+2,fnr  >> "coinc.c06";
    printf "F;r%d;FDR\nC;Esum(r4:%dc)\n",fnr+2,fnr  >> "coinc.c07";
    printf "F;r%d;FDR\nC;Esum(r4:%dc)\n",fnr+2,fnr  >> "coinc.c08";
    printf "F;r%d;FDR\nC;Esum(r4:%dc)\n",fnr+2,fnr  >> "coinc.c09";
    printf "F;r%d;FF2R\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c10";
    printf "F;r%d;FF2R\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c11";
    printf "F;r%d;FF2R\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c12";
    printf "F;r%d;FF2R\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c13";
    printf "F;r%d;FF3R\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c14";
    printf "F;r%d;FF3R\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c15";
    printf "F;r%d;FF3R\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c16";
    printf "F;r%d;FF1R\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c21";
    printf "F;r%d;FF1R\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c22";
    printf "O;status 2\nW;N1;A18 1;C7 0 7;Ostandout\nE\n" >> "coinc.c22";
    system("cat coinc.c[012][0-9] > coinc.col");
    system("rm  coinc.c[012][0-9]");
}
