BEGIN {
    fnr=3;
    print "C;c1;r2;K\"run\"" > "coinc.c01";
    print "F;c2;r2;FDR\nC;K\"singles A\"" > "coinc.c02";
    print "F;c3;r2;FDR\nC;K\"singles B\"" > "coinc.c03";
    print "F;c4;r2;FDR\nC;K\"laser events\"" > "coinc.c04";
    print "F;c5;r2;FDR\nC;K\"Coincidences\"" > "coinc.c05";
    print "F;c6;r2;FDC\nC;K\"real time A\"" > "coinc.c06";
    print "F;r3;FDC\nC;K\"[s]\"" >> "coinc.c06";
    print "F;c7;r2;FDC\nC;K\"run time A\"" > "coinc.c07";
    print "F;r3;FDC\nC;K\"[s]\"" >> "coinc.c07";
    print "F;c8;r2;FDC\nC;K\"real time B\"" > "coinc.c08";
    print "F;r3;FDC\nC;K\"[s]\"" >> "coinc.c08";
    print "F;c9;r2;FDC\nC;K\"run time B\"" > "coinc.c09";
    print "F;r3;FDC\nC;K\"[s]\"" >> "coinc.c09";
    print "F;c10;r2;FDC\nC;K\"dead time A\"" > "coinc.c10";
    print "F;r3;FDC\nC;K\"[s]\"" >> "coinc.c10";
    print "F;c11;r2;FDC\nC;K\"dead time B\"" > "coinc.c11";
    print "F;r3;FDC\nC;K\"[s]\"" >> "coinc.c11";
    print "F;c12;r2;FDC\nC;K\"dead time AB\"" > "coinc.c12";
    print "F;r3;FDC\nC;K\"[s]\"" >> "coinc.c12";
    print "F;c13;r2;FDC\nC;K\"foerster A\"" > "coinc.c13";
    print "F;r3;FDC\nC;K\"[uA]\"" >> "coinc.c13";
}
{
    if (FNR == 1) { run=$1; fnr=fnr+1 }
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
	if ($3 == singleA+singleB+laser+coinc)
	{
	    printf "C;r%d;K\"%s\"\n",fnr,run >> "coinc.c01";
	    printf "F;r%d;FDR\nC;K%d\n",fnr,singleA >> "coinc.c02";
	    printf "F;r%d;FDR\nC;K%d\n",fnr,singleB >> "coinc.c03";
	    printf "F;r%d;FDR\nC;K%d\n",fnr,laser >> "coinc.c04";
	    printf "F;r%d;FDR\nC;K%d\n",fnr,coinc >> "coinc.c05";
	    printf "F;r%d;FF2R\nC;K%.2f\n",fnr,realA >> "coinc.c06";
	    printf "F;r%d;FF2R\nC;K%.2f\n",fnr,runA >> "coinc.c07";
	    printf "F;r%d;FF2R\nC;K%.2f\n",fnr,realB >> "coinc.c08";
	    printf "F;r%d;FF2R\nC;K%.2f\n",fnr,runB >> "coinc.c09";
	    printf "F;r%d;FF3R\nC;K%.3f\n",fnr,deadA >> "coinc.c10";
	    printf "F;r%d;FF3R\nC;K%.3f\n",fnr,deadB >> "coinc.c11";
	    printf "F;r%d;FF3R\nC;K%.3f\n",fnr,deadAB >> "coinc.c12";
	    printf "F;r%d;FF4R\nC;K%.4f\n",fnr,foersterA >> "coinc.c13";
	}
	else { print "event count error" }
    }
}
END {
    printf "F;r%d;FDR\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c02";
    printf "F;r%d;FDR\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c03";
    printf "F;r%d;FDR\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c04";
    printf "F;r%d;FDR\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c05";
    printf "F;r%d;FF2R\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c06";
    printf "F;r%d;FF2R\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c07";
    printf "F;r%d;FF2R\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c08";
    printf "F;r%d;FF2R\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c09";
    printf "F;r%d;FF3R\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c10";
    printf "F;r%d;FF3R\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c11";
    printf "F;r%d;FF3R\nC;Esum(r4:%dc)\n",fnr+2,fnr >> "coinc.c12";
    system("cat coinc.c[01][0-9] > coinc.col");
    system("rm  coinc.c[01][0-9]");
}
