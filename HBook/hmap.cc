//
// Copyright (c) 1994 by   Michael O. Distler,
//                         Institut fuer Kernphysik,
//                         Johannes Gutenberg-Universitaet Mainz
//
// All rights reserved.
//
// $Header: /tmp/cvsroot/Cola/HBook/hmap.cc,v 1.17 1998-08-30 22:10:23 distler Exp $
//

static const char rcsid[] =
 "@(#)$Id: hmap.cc 2216 2008-06-13 21:13:47Z distler $";

#include <time.h>
#include "HBook.h"
#include "Rand.h"

int
main(int argc, char *argv[])
{
    if (argc && argv); // keep compiler happy

    ACG		gen(time(0),20);
    Normal	normal(50.0, 100.0, &gen);
    Erlang	erlang(50.0, 100.0, &gen);
    LogNormal	lognorm(50.0, 100.0, &gen);

    HBookBase	histograms(10000, "TEST");
    HBook1D	histo1("Erlang",  100, 0, 100);
    HBook1D	histo2("LogNorm", 100, 0, 100);
    HBook1D	histo3("Normal",  100, 0, 100);

    for (int i=0; i<1000000; i++) {
	histo1.inc(erlang());
	histo2.inc(lognorm());
	histo3.inc(normal());
    }

    histograms.write("histo.map");

    return 0;
}
