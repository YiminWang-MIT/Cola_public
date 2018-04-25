//
// Copyright (c) 1993/94 by	Michael O. Distler,
//				Institut fuer Kernphysik,
//				Johannes Gutenberg-Universitaet Mainz
//
// All rights reserved.
//
// $Header: /tmp/cvsroot/Cola/HBook/htest.cc,v 1.17 1998-08-30 22:10:24 distler Exp $
//

static const char rcsid[] =
 "@(#)$Id: htest.cc 2354 2009-11-13 13:58:04Z merkel $";

#include <time.h>
#include "HBook.h"
#include "Rand.h"

const int PAWCsize = 100000;
struct pawccc {
    float h[PAWCsize];
} pawc_;

int
main(int argc, char *argv[])
{
    if (argc && argv); // keep compiler happy

    ACG		gen(time(0),20);
    Normal	normal(50.0, 100.0, &gen);
    Normal	normal2(50.0, 1000.0, &gen);
    Erlang	erlang(50.0, 100.0, &gen);
    LogNormal	lognorm(50.0, 100.0, &gen);

//    HBookBase	histograms(PAWCsize);
    HBookBase	histograms("histo.dat", PAWCsize);
    HBook1D	histo1("Erlang",  100, 0, 100);
    HBook1D	histo2("LogNorm", 100, 0, 100);
    HBook1D	histo3("Normal",  100, 0, 100);
    HBook2D	histo4("Normal2D",  100, 0, 100, 100, 0, 200);

    for (int i=0; i<10000; i++) {
	histo1.inc(erlang());
	histo2.inc(lognorm());
	histo3.inc(normal());
	histo4.inc(normal(),normal2());
    }

    HBook1D histo5("Normal (scaled)", histo3);
    histo5 *= 5.0;

    histograms.printTitle(4);
    histograms.printContents(4,0," POST");
    histograms.write("histo.dat");

    return 0;
}
