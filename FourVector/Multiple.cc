//								      -*-c++-*-
// Created 1995 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/FourVector/Multiple.cc,v 3.0 1997-07-09 10:14:33 merkel Exp $
//
// Header File fuer den Multiple Scattering RandomNumberGenerator
//

#include <Multiple.h>
#include <math.h>

Multiple::Multiple(double sigma, RNG *gen)
: Normal(fabs(sigma), sigma*sigma, gen)
{
    pNorm	= sin(pStdDev) * exp(-0.5);
}

double
Multiple::sigma(double x)
{
    double t	= pStdDev;

    pStdDev	= fabs(x);
    pVariance	= x * x;
    pMean	= pStdDev;
    pNorm	= sin(pStdDev) * exp(-0.5);

    return t;
}

double
Multiple::operator()()
{
    for (;;) {
	double u = this->Normal::operator()();
	if ((u > 0.0) && (pGenerator->asDouble()*gauss(u) < function(u)))
	  return u;
    }
}

double
Multiple::function(double x)
{
    return sin(x) * exp(-0.5*x*x/pVariance);
}

double
Multiple::gauss(double x)
{
    double e = (x - pMean) / pStdDev;
    return pNorm * exp(-0.5*e*e);
}
