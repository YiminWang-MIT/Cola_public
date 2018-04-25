//
// Created 1995 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/HBook/landau.cc,v 1.20 2006-12-07 14:06:46 merkel Exp $
//
// Test Programm fuer den Landau Generator
//

#include "Landau.h"
#include "HBook.h"
#include <ctime>

const int PAWCsize = 75000;
struct pawc {
    float h[PAWCsize];
} pawc_;

int
main(int argc, char *argv[])
{
    if (argc && argv); // keep compiler happy

    ACG		gen(time(0),20);
    HBookBase	histograms(PAWCsize);
    HBook1D	histo01("Landau", 5000, -50, 50);
    HBook1D	histo02("Landau", 5000, -50, 50);
    HBook1D	histo03("Landau", 5000, -50, 50);
    HBook1D	histo04("Landau", 5000, -50, 50);
    HBook1D	histo05("Landau", 5000, -50, 50);
    HBook1D	histo06("Landau", 5000, -50, 50);
    HBook1D	histo07("Landau", 5000, -50, 50);
    HBook1D	histo08("Landau", 5000, -50, 50);
    HBook1D	histo09("Landau", 5000, -50, 50);
    HBook1D	histo10("Landau", 5000, -50, 50);
    Landau	landau(-LandauNormalMean, 1.0, &gen);
    int		i, times = 1000;
    double	x;

    if (argc == 2)
	times = atoi(argv[1]);

    while (times--) {
	x = 0.0; i = 0;
	for ( ; i<10 ; i++) x += landau();
	histo01.inc(x);
	for ( ; i<20 ; i++) x += landau();
	histo02.inc(x);
	for ( ; i<30 ; i++) x += landau();
	histo03.inc(x);
	for ( ; i<40 ; i++) x += landau();
	histo04.inc(x);
	for ( ; i<50 ; i++) x += landau();
	histo05.inc(x);
	for ( ; i<60 ; i++) x += landau();
	histo06.inc(x);
	for ( ; i<70 ; i++) x += landau();
	histo07.inc(x);
	for ( ; i<80 ; i++) x += landau();
	histo08.inc(x);
	for ( ; i<90 ; i++) x += landau();
	histo09.inc(x);
	for ( ; i<100 ; i++) x += landau();
	histo10.inc(x);
    }

    histograms.write("landau.dat");

    return 0;
}
