//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 1999-2001
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: simSpecEventSim.cc 2588 2014-10-30 10:31:33Z kohly $
//

#include "FourVector/FourVector.h"   // relativistic four vector arithmetics
#include "Colli.h"                   // Definition of Collimators
#include "Cola/QuasiRandom.h"             // Random Generator
#include "Cola/include/online/online.h"        // 1st level reconstructed data
#include "Cola/Targetlength.h"       // Targetcells and Energyloss
#include "Cola/Luminosity.h"
#include "spinTransfer.h"            // spin transfer target -> focal plane
#include "simSpecEventSim.h"

extern int ergloss;
extern modeltype ModelType;
	
simSpecEventSim::simSpecEventSim(double ang, double angRes, double angRes2,
				 double vertiAngRatio,
				 double mom, double momRes, double momRes2, 
				 int ndp, double *dpc, 
				 double *ara, double *mra,
				 double refmom, double akz, 
				 const char *colli, Collimator* ColliList[],
				 int vac, Particle *p,
				 double *Eloss_corr, double *Eloss_sim,
				 double Dtheta, double Dphi, double Dmomentum,
				 double oopang)
  : simDetectorBase(NULL, p, ang, oopang, mom, Dtheta, Dphi, Dmomentum)
{
  vacuum = vac; 
  onlElossCorr = Eloss_corr;
  onlElossSim = Eloss_sim;
  steps = 1;
  if (ergloss > 1) {
    steps = 5;
    if (particle == P_electron) steps=  1;
    if (particle == P_proton)   steps= 10;
    if (particle == P_deuteron) steps=100;
  }
  momentumRes = momRes; 
  refmomentum = refmom;
  angularRes = angRes;

  momentumRes2 = momRes2; 
  angularRes2  = angRes2;
  ndpcuts = ndp;
  if (ndp) {
    mratio = new double[ndp];
    aratio = new double[ndp]; 
    dpcuts = new double[ndp];
    for (int i=0; i<ndp; i++) 
      {mratio[i]=mra[i]; aratio[i]=ara[i]; dpcuts[i]=dpc[i];}
  } 
  else {mratio=NULL; aratio=NULL; dpcuts=NULL;}
  
  verti = vertiAngRatio;

  acceptance = akz; 

  if (!colli[0]) 
    collimator = ColliList[0]; 
  else 
    for(int i=0; ColliList[i]; i++) 
      if (!strcmp(colli, ColliList[i]->getName()))
	collimator = ColliList[i];
  if (!collimator) {
    collimator = ColliList[0]; 
    std::cout << "Collimator \"" << colli << "\" unknown, using \""
	      << collimator->getName() << "\"." << std::endl;
  }
  if (!strcmp(collimator->getName(), "4pi"))     acceptance = 1000.0;
  if (!strcmp(collimator->getName(), "inPlane")) acceptance = 1000.0;
}

int simSpecEventSim::resolution(struct TargetCoord *tc)
{
  if (particle.getID() == 0) return -1;
  static PseudoRandom psrandom;
  static normal norm(&psrandom, &psrandom);

  double p  = particle.momentum(); 
  double E  = particle.energy(); 
  double theta = particle.theta();
  double phi   = particle.phi();
  double mRes = momentumRes;
  double aRes = angularRes;

  // dp-dependent resolutions
  if (mratio) {
    double dp = (p/(momentum/refmomentum)-1)*100.0;
    int i=0;
    while (dpcuts[i]<dp && i<ndpcuts-1) i++;
    if (psrandom()<mratio[i]) mRes = momentumRes2;
    if (psrandom()<aratio[i]) aRes = angularRes2;
  }
  double pn = p * (1 + norm() * mRes);

  double phires = (2 * psrandom() - 1) * M_PI;
  double thetares = norm() * aRes;

  // If vertical angle resolution is better than horizontal (Spec. B)
  if (verti < 1) {
    double sp = sin(phires)*verti;
    double cp = cos(phires);
    phires   = atan2(sp, cp);
    thetares *= sqrt(sp*sp+cp*cp);
  }  

  particle.initPolar(sqrt(E*E - p*p + pn*pn), pn, thetares, phires);
  particle.rot_theta(theta);
  particle.rot_phi(phi);

  tc->th = particle.specTheta(angle);
  tc->ph = -particle.specPhi(angle); // different sign in lib4vec!
  tc->dp = particle.specDelta(momentum/refmomentum);
  

  return 0;
}

int simSpecEventSim::energyLossCorrection(target *target, double x[3]) 
{
  double EnergyBefore = 0;
  if (!particle.getCharge()) return -1;
  if (!ergloss) return -1;

  EnergyBefore = particle.energy();
  if (!vacuum) target->EnergyLossCorrChamber(particle);
  target->EnergyLossCorr(particle, x[0], x[1], x[2], steps);
  if (onlElossCorr) *onlElossCorr = (particle.energy() - EnergyBefore);

  return 0;
}
  
int simSpecEventSim::check(Particle vf, double x[3],
			   target *target, struct TargetCoord *tc, struct simFocalCoord *fc, modeltype ModelType)
{
  double EnergyBefore = 0;
  
  if (!collimator->accepted(x, vf, angle, oop, refmomentum))  return 0; 

  particle = vf;

  if (ergloss) {
    EnergyBefore = vf.energy();
    target->EnergyLossSim(particle, x[0], x[1], x[2], steps, ModelType);
    if (!vacuum) target->EnergyLossSimChamber(particle); 
    if (onlElossSim) *onlElossSim = (EnergyBefore - particle.energy());
  }
  if (fabs(particle.specDelta(momentum)) > acceptance) return 0;   

  tc->th = particle.specTheta(angle);
  tc->ph = -particle.specPhi(angle); // different sign in lib4vec!
  tc->dp = particle.specDelta(momentum/refmomentum);
  tc->y0 = (- x[0] * cos(angle) + x[2] * sin(angle))/10.0; 
  // this is only an approximation (i.e. ph==0) to initialize the variable
  // proper vertex coordinate y0 has to go here...


  return 1;
}                                                                   
