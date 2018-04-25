//								      -*-c++-*-
// Created 1994 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/FourVector/SimFourVector.cc,v 3.1 1998-08-30 22:09:59 distler Exp $
//
// Implementation der Vierervektorenklasse fuer Simulationen
//

#include "FourVector.h"
#include "Landau.h"

Uniform * SimFourVector::dPhi = NULL;

SimFourVector::SimFourVector(double dE, double dA, RNG *gen)
{
  dEnergy = new Landau(-dE*LandauNormalMean, dE, gen);
  dAngle  = new Normal(0.0, dA*dA, gen);
  if (!dPhi) dPhi = new Uniform(-M_PI_2, M_PI_2, gen);
}

SimFourVector::~SimFourVector()
{
  if (dEnergy) delete dEnergy;
  if (dAngle)  delete dAngle;
}

void
SimFourVector::init(FourVector v)
{
  double m2	= v.square();
  double phi	= v.phi();
  double theta	= v.theta();
  double E	= v.energy();

  if (dEnergy) E -= (*dEnergy)();
  FourVector::initPolar(E, sqrt(E * E - m2),
			dAngle ? (*dAngle)() : 0.0, dPhi ? (*dPhi)() : 0.0);
  rot_theta(theta);
  rot_phi(phi);
}

void
SimFourVector::initPolar(double E, double P,
			 double theta, double phi)
{
  double m2	= E * E - P * P;

  if (dEnergy) E -= (*dEnergy)();
  FourVector::initPolar(E, sqrt(E * E - m2),
			dAngle ? (*dAngle)() : 0.0, dPhi ? (*dPhi)() : 0.0);
  rot_theta(theta);
  rot_phi(phi);
}

Landau  * SimulFourVector::dEnergy = NULL;
Normal  * SimulFourVector::dAngle = NULL;
Uniform * SimulFourVector::dPhi = NULL;

SimulFourVector::SimulFourVector(RNG *gen)
{
  if (!dEnergy) dEnergy = new Landau(0.0, 1.0, gen);
  if (!dAngle) dAngle = new Normal(0.0, 1.0, gen);
  if (!dPhi) dPhi = new Uniform(-M_PI_2, M_PI_2, gen);
}

void
SimulFourVector::init(FourVector v, double dE, double dA)
{
  double m2	= v.square();
  double E	= v.energy();
  double delta;

  delta = dEnergy ? (dE * (*dEnergy)()) : 0.0;
  if (delta > v.Ekin())
    FourVector::initPolar(sqrt(m2),0,0,0);
  else {
    E -= delta;
    FourVector::initPolar(E, sqrt(E * E - m2),
			  dAngle ? M_SQRT2 * dA * (*dAngle)() : 0.0,
			  dPhi ? (*dPhi)() : 0.0);
  }

  rot_theta(v.theta());
  rot_phi(v.phi());
}

void
SimulFourVector::initPolar(double E, double P, double theta,
			   double phi, double dE, double dA)
{
  double m2	= E * E - P * P;
  double delta;

  delta = dEnergy ? dE * (*dEnergy)() : 0.0;
  if (delta > (E-sqrt(m2)))
    FourVector::initPolar(sqrt(m2),0,0,0);
  else {
    E -= delta;
    FourVector::initPolar(E, sqrt(E * E - m2),
			  dAngle ? M_SQRT2 * dA * (*dAngle)() : 0.0,
			  dPhi ? (*dPhi)() : 0.0);
  }

  rot_theta(theta);
  rot_phi(phi);
}

void
SimulFourVector::scatter(double dE, double dA)
{
  double m2	= square();
  double E	= energy();
  double ekin	= Ekin();
  double th	= theta();
  double ph	= phi();
  double delta;

  delta = dEnergy ? (dE * (*dEnergy)()) : 0.0;
  if (delta > ekin)
    FourVector::initPolar(sqrt(m2),0,0,0);
  else {
    E -= delta;
    FourVector::initPolar(E, sqrt(E * E - m2),
			  dAngle ? M_SQRT2 * dA * (*dAngle)() : 0.0,
			  dPhi ? (*dPhi)() : 0.0);
  }

  rot_theta(th);
  rot_phi(ph);
}
