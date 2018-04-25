//								      -*-c++-*-
// Created 1994 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/FourVector/Landau.cc,v 3.2 1999-12-01 09:47:58 merkel Exp $
//
// Implementation des Landau RandomNumberGenerator
//

#define _DEFAULT_SOURCE 1
#include <Landau.h>
#include <math.h>

Landau::Landau(double xmost, double xwidth, RNG *gen) : Random(gen)
{
    pMost = xmost;
    pWidth = xwidth;
}

const double LPAR1 = 0.60653066;
const double LPAR2 = 2.287946;

#ifdef DBL_MAX
const double   CUT = -log(DBL_MAX)/LPAR2;
#else
const double   CUT = -709.78271289/LPAR2;
#endif

double
Landau::operator()()
{
    for (;;) {
	double u = tan(M_PI * pGenerator->asDouble() - M_PI_2);

	if (CUT < u)
	  if ( LPAR1*pGenerator->asDouble()
	      < (1+u*u)*exp(-.5*(LPAR2*u+exp(-LPAR2*u))) )
	    return u * pWidth * LPAR2 + pMost;
    }
}

double
Landau::function(double x)
{
    double tmp = (x-pMost)/pWidth;

    return exp(-0.5*(tmp+exp(-tmp)));
}
