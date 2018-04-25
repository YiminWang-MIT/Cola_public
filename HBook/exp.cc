//
// Created 1995 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/HBook/exp.cc,v 1.20 2006-12-07 14:06:45 merkel Exp $
//
// Test Programm fuer scaling Korrektur
//

#include "HBook.h"
#include "Rand.h"
#include <iostream>
#include <ctime>

const double MEAN = 0.02;
const double DEAD = 0.001;
const int PAWCsize = 75000;
struct pawc {
    float h[PAWCsize];
} pawc_;

int
main(int argc, char *argv[])
{
    ACG	gen(time(0),20);
    HBookBase histograms(PAWCsize);
    HBook1D scale1("scale 1", 1000, 0.0, 10.0);
    HBook1D scale2("scale 2", 1000, 0.0, 10.0);
    HBook1D scale3("scale 3", 1000, 0.0, 10.0);
    HBook1D scale4("scale 4", 1000, 0.0, 10.0);
    NegativeExpntl p(MEAN, &gen);
    int	n, times = 1000;
    double x;
    double t1, t2, t3, t4;
    double e1, e2, e3, e4;
    long   n1, n2, n3, n4;

    if (argc == 2)
	times = atoi(argv[1]);
    n = times;

    e1 = e2 = e3 = e4 = 0.0;
    n1 = n2 = n3 = n4 = 0;
    t1 = t2 = t3 = t4 = 0.0;
    while (times--) {
	x =  p(); scale1.inc(x/1);
	t1 += x/1; if (t1 > e1) { n1++; e1 = t1 + DEAD; }
	x += p(); scale2.inc(x/2);
	t2 += x/2; if (t2 > e2) { n2++; e2 = t2 + DEAD; }
	x += p(); scale3.inc(x/3);
	t3 += x/3; if (t3 > e3) { n3++; e3 = t3 + DEAD; }
	x += p(); scale4.inc(x/4);
	t4 += x/4; if (t4 > e4) { n4++; e4 = t4 + DEAD; }
    }

    histograms.write("exp.dat");

    std::cout << n << std::endl;
    std::cout << n1 << '\t' << (double)n1/(double)n
	      << '\t' << (double)n1/(double)t1 << std::endl;
    std::cout << n2 << '\t' << (double)n2/(double)n
	      << '\t' << (double)n2/(double)t2 << std::endl;
    std::cout << n3 << '\t' << (double)n3/(double)n
	      << '\t' << (double)n3/(double)t3 << std::endl;
    std::cout << n4 << '\t' << (double)n4/(double)n
	      << '\t' << (double)n4/(double)t4 << std::endl;

    exit(0);
    return 0;
}
