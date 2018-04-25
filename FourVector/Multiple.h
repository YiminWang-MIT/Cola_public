//								      -*-c++-*-
// Created 1995 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/FourVector/Multiple.h,v 3.1 1998-08-30 22:09:58 distler Exp $
//
// Header File fuer den Multiple Scattering RandomNumberGenerator
//

#ifndef __Multiple_h__
#define __Multiple_h__

#include "Rand.h"

class Multiple : public Normal {
    double pNorm;
    double gauss(double x);
public:
    Multiple(double sigma, RNG *gen);
    double sigma() { return pStdDev; }
    double sigma(double x);
    double function(double x);
    virtual double operator()();
};

#endif /* __Multiple_h__ */
