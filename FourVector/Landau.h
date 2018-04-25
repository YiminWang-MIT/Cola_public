//								      -*-c++-*-
// Created 1994 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/FourVector/Landau.h,v 3.3 2006-06-27 12:33:55 distler Exp $
//
// Header File fuer den Landau RandomNumberGenerator
//

#ifndef __Landau_h__
#define __Landau_h__

#include "Rand.h"

// Die Landauverteilung ist asymetrisch und kann durch ihren
// wahrscheinlichsten Wert und ihren Mittelwert beschrieben werden.
// Hier wird der Zufallszahlengenerator mit dem wahrscheinlichsten Wert
// (most probable) und durch eine Breite beschrieben.
// Fuer eine Breite von 1.0 und einem wahrscheinlichsten Wert von 0.0
// liegt der Mittelwert bei LandauNormalMean

const double LandauNormalMean	 = 1.27035973851;
const double invLandauNormalMean = 1.0/LandauNormalMean;

class Landau : public Random {

protected:
    double pMost;
    double pWidth;

public:
    Landau(double xmost, double xwidth, RNG *gen);
    virtual ~Landau() { ; }
    double most()		{ return pMost; }
    double most(double x)	{ double t=pMost; pMost = x; return t; }
    double mean()		{ return pMost+LandauNormalMean*pWidth; }
    double width()		{ return pWidth; }
    double width(double x)	{ double t=pWidth; pWidth = x; return t; }
    double function(double x);
    virtual double operator()();
};

#endif
