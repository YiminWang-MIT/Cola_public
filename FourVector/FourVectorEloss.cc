//								      -*-c++-*-
// Created 1995 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/FourVector/FourVectorEloss.cc,v 3.1 1998-08-30 22:09:58 distler Exp $
//
// Implementation der Vierervektorenklasse fuer Simulationen
//

#include "FourVector.h"
#include "Landau.h"
#include "Multiple.h"

Landau   * FourVectorEloss::dEnergy = NULL;
Multiple * FourVectorEloss::dAngle = NULL;
Uniform  * FourVectorEloss::dPhi = NULL;

FourVectorEloss::FourVectorEloss(RNG *gen)
{
    if (!dEnergy) dEnergy = new Landau(-LandauNormalMean, 1.0, gen);
    if (!dAngle) dAngle = new Multiple(.001, gen);
    if (!dPhi) dPhi = new Uniform(-M_PI, M_PI, gen);
}

void
FourVectorEloss::init(FourVector v, double dA, double dEmp, double dEmean)
{
    double m2	= v.square();
    double E	= v.energy();
    double delta= (dEmean-dEmp > 0.1*dEmean) ? dEmean-dEmp : 0.1*dEmean;

    dAngle->sigma(dA);
    if (delta>0.0) {
	dEnergy->most(dEmean-delta);
	dEnergy->width(invLandauNormalMean*delta);
	delta = (*dEnergy)();
    } else delta = 0.0;

    if (delta > v.Ekin())
	FourVector::initPolar(sqrt(m2),0,0,0);
    else {
	E -= delta;
	FourVector::initPolar(E, sqrt(E*E - m2), (*dAngle)(), (*dPhi)());
    }

    rot_theta(v.theta());
    rot_phi(v.phi());
}

void
FourVectorEloss::initPolar(double E, double P, double theta, double phi,
			   double dA, double dEmp, double dEmean)
{
    double m2	= E*E - P*P;
    double delta= (dEmean-dEmp > 0.1*dEmean) ? dEmean-dEmp : 0.1*dEmean;

    dAngle->sigma(dA);
    if (delta>0.0) {
	dEnergy->most(dEmean-delta);
	dEnergy->width(invLandauNormalMean*delta);
	delta = (*dEnergy)();
    } else delta = 0.0;

    if (delta > (E-sqrt(m2)))
	FourVector::initPolar(sqrt(m2),0,0,0);
    else {
	E -= delta;
	FourVector::initPolar(E, sqrt(E*E - m2), (*dAngle)(), (*dPhi)());
    }

    rot_theta(theta);
    rot_phi(phi);
}

void
FourVectorEloss::scatter(double dA, double dEmp, double dEmean)
{
    double m2	= square();
    double E	= energy();
    double ekin	= Ekin();
    double th	= theta();
    double ph	= phi();
    double delta= (dEmean-dEmp > 0.1*dEmean) ? dEmean-dEmp : 0.1*dEmean;
    dAngle->sigma(dA);
    if (delta>0.0) {
	dEnergy->most(dEmean-delta);
	dEnergy->width(invLandauNormalMean*delta);
	delta = (*dEnergy)();
    } else delta = 0.0;

    if (delta > ekin)
	FourVector::initPolar(sqrt(m2),0,0,0);
    else {
	E -= delta;
	FourVector::initPolar(E, sqrt(E*E - m2), (*dAngle)(), (*dPhi)());
    }

    rot_theta(th);
    rot_phi(ph);
}
