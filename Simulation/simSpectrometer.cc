//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 1999-2001
//
// Institut fr Kernphysik, Universit� Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: simSpectrometer.cc 2621 2015-01-22 14:06:36Z schlimme $
//
// Deluje 21.06.2007
//
//

#include <iostream> 
#include "FourVector/FourVector.h"   // relativistic four vector arithmetics
#include "Colli.h"                   // Definition of Collimators
#include "Cola/QuasiRandom.h"             // Random Generator
#include "Cola/include/online/online.h"        // 1st level reconstructed data
#include "Cola/Targetlength.h"       // Targetcells and Energyloss
#include "Cola/Luminosity.h"
#include "spinTransfer.h"            // spin transfer target -> focal plane
#include "simSpectrometer.h"
#include "SimParticle.h"
#include "DSpectrometer.h"
#include "ran1.h"
#include "Model/Model.h"

long int seed = -1; // za zrebanje

extern int ergloss;
modeltype ModelType;
simSpectrometer::simSpectrometer(AquaTree *outtree, 
				 double ang, double angRes, double angRes2,
				 double vertiAngRatio,
				 double mom, double momRes, double momRes2, 
				 int ndp, double *dpc, 
				 double *ara, double *mra,
				 double refmom, double akz, 
				 const char *colli, Collimator* ColliList[],
				 int vac, Particle *p,
				 double *Eloss_corr, double *Eloss_sim,
				 double Dtheta, double Dphi, double Dmomentum,
				 double oopang, int decay,char *chamberfile)
  : simDetectorBase(outtree, p, ang, oopang, mom, Dtheta, Dphi, Dmomentum)
{
  idecay = decay;
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

  // assignments for particle decay simulation
  // only 28 msr for spec A at the moment
  if (!strcmp(collimator->getName(),"15msr") ||
      !strcmp(collimator->getName(), "21msr") ||
      !strcmp(collimator->getName(), "28msr")) { // three valid Spec A collis
    DSpectrometerA *dspectrometer = new DSpectrometerA(collimator, lkA,1);
    decayspectrometer = dspectrometer;
  } else if (!strcmp(collimator->getName(),"5.6msr")) { // Spec B with 5.6 msr
    DSpectrometerB *dspectrometer = new DSpectrometerB(collimator, lkB,-1);
    decayspectrometer = dspectrometer;
  }

  chamber=new Chamber(chamberfile);

}

int simSpectrometer::resolution(struct TargetCoord *tc)
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

  // vertical spectrometers
  tc->th = particle.specTheta(angle);
  tc->ph = -particle.specPhi(angle); // different sign in lib4vec!

  // horizontal spectrometers (e.g. HES)
  if (!strcmp(collimator->getName(), "HES")) {
  tc->ph = particle.specTheta(angle);
  tc->th = -particle.specPhi(angle); // different sign in lib4vec!
  }

  tc->dp = particle.specDelta(momentum/refmomentum);
  
  atree->packEventData(&(tc->th), 1);
  atree->packEventData(&(tc->ph), 1);
  atree->packEventData(&(tc->dp), 1);

  return 0;
}

int simSpectrometer::energyLossCorrection(target *target, double x[3]) 
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
  
int simSpectrometer::check(Particle vf, double x[3],
			   target *target, struct TargetCoord *tc,
			   struct simFocalCoord *fc, Focus *focus, modeltype ModelType)
{
  double EnergyBefore = 0;
  
  // handle particle decay: first decide for spectrometer according
  // to collimator name, then for particle type
  
  particle = vf;

  static PseudoRandom psrandom;
  static normal norm(&psrandom, &psrandom);

  if (idecay) { // particle decay (*** new ***)
    
    // rotate from LAB to SPEC coordinate system
    double theta = oop;
    double phi = angle;
    
    Momentum ex(sin(theta)*sin(phi), -cos(theta), sin(theta)*cos(phi));
    Momentum ey(cos(phi), 0.0, -sin(phi));
    Momentum ez(cos(theta)*sin(phi), sin(theta), cos(theta)*cos(phi));
    
    FourVector mom(vf[0], vf[1], vf[2], vf[3]);
    Momentum pos(x[0], x[1], x[2]);
    
    Vector3D posSpec(pos*ex, pos*ey, pos*ez); // has to be vector3D
    Vector3D spinSpec(0, 0, 0);  // trace spin (work in progress)
    
    Momentum vecp(mom[1], mom[2], mom[3]);
    
    FourVector momSpec(mom[0], vecp*ex, vecp*ey, vecp*ez);

    posSpec = posSpec;  // unit is [mm]
    momSpec = momSpec*1000.0; // unit is now [MeV]

    if (vf == P_piplus) {
     
      SimParticle simParticle(vf.getID(), posSpec, momSpec, spinSpec);
      // create particle GROUP in order to accommodate many decay products
      SimParticleGroup simParticleGroup;
    
      // initial particle added into group
      simParticleGroup.add(simParticle);
           
      // start simulation with this particle (real stuff called here)
      int stat = decayspectrometer->simulate(&simParticleGroup, momentum*1000);

      // check return status of main decay routine
      if ( stat==1 ) {
        // successful completion 

        int group_status = 0; 
        for (int i = 0; i<simParticleGroup.getNumber(); i++) {
           
	  if (simParticleGroup.simParticle[i].status==1) {
            // at least one of the decay products (or the initial particle
	    // without the decay) made it to the focal plane
            group_status = 1; 
	    
            Vector3D VDCposition;
            double theta, phi;
            Vector3D Velocity(
		 simParticleGroup.simParticle[i].fVmomentum.beta()[0],
                 simParticleGroup.simParticle[i].fVmomentum.beta()[1],
                 simParticleGroup.simParticle[i].fVmomentum.beta()[2]);
            Velocity = Velocity * SpeedOfLight;
            decayspectrometer->position_at_VDC(
                 simParticleGroup.simParticle[i].position,
		 Velocity, &VDCposition, &theta, &phi);

	    if ((int)simParticleGroup.simParticle[i].ID==P_piplus.getID()) {
	      fc->x  = VDCposition[1];
	      fc->th = theta;
	      fc->y  = VDCposition[2];
	      fc->ph = phi;
	      
	      atree->packEventData(&(fc->x), 1);
	      atree->packEventData(&(fc->th), 1);
	      atree->packEventData(&(fc->y), 1);
	      atree->packEventData(&(fc->ph), 1);
	      
	    } else if ((int)simParticleGroup.simParticle[i].ID==P_muplus.getID()) {
	      
	      fc->x_decay  = VDCposition[1];
	      fc->th_decay = theta;
	      fc->y_decay  = VDCposition[2];
	      fc->ph_decay = phi;
	      
	      atree->packEventData(&(fc->x_decay), 1);
	      atree->packEventData(&(fc->th_decay), 1);
	      atree->packEventData(&(fc->y_decay), 1);
	      atree->packEventData(&(fc->ph_decay), 1);
	    }
	    else {
	      std::cout << "This one did not make it!" << std::endl;
	    }
	  }
	}

        // If none of the particles made it to the focal plane, return zero
	// i.e. the initially generated particle was NOT accepted
        if (group_status == 0) return 0; 
      }
      else {
	// return 0 if decay mode unknown
        return 0;
      }
    }
    else {
      std::cerr << "Only pi+ decay enabled now" << std::endl;
    }

  } else { 
    
    // no particle decay (standard Simul)

    if (ergloss) {
      EnergyBefore = vf.energy();
      target->EnergyLossSim(particle, x[0], x[1], x[2], steps, ModelType);
      if (!vacuum) target->EnergyLossSimChamber(particle); 
      if (onlElossSim) *onlElossSim = (EnergyBefore - particle.energy());

      double norm_multiplescattering[2] = {norm(), norm()};
      if (ModelType == ElasticRadiative) 
        target->MultipleScattering(particle, norm_multiplescattering, x[0], x[1], x[2], steps, ModelType);
    }
    if (!collimator->accepted(x, vf, angle, oop, refmomentum)) return 0; 
    
    if (!chamber->ok(angle, particle.specDelta(momentum/refmomentum),x[2],particle.specTheta(angle),-particle.specPhi(angle))) return 0;
 
    if (fabs(particle.specDelta(momentum)) > acceptance) return 0;  
  }
    
  // vertical spectrometers
  tc->th = particle.specTheta(angle);
  tc->ph = -particle.specPhi(angle); 

  // horizontal spectrometers (e.g. HES)
  if (!strcmp(collimator->getName(), "HES")) {
  tc->ph = particle.specTheta(angle);
  tc->th = -particle.specPhi(angle); // different sign in lib4vec!
  }

  tc->dp = particle.specDelta(momentum/refmomentum); //momentum == central momentum [simDetectorBase.h]
  tc->y0 = (- x[0] * cos(angle) + x[2] * sin(angle))/10.0; // result is in [cm]
  // this is only an approximation (i.e. ph==0) to initialize the variable
  // proper vertex coordinate y0 has to go here...
  /*
  if (focus){
    TargetCo result = {tc->dp, tc->th, tc->y0, tc->ph, tc->len};
    //std::cout << tc->dp << " " << tc->th << " " << tc->y0 << " " << tc->ph << std::endl;
    //std::cout << result.dp << " " << result.th << " " << result.y0 << " " << result.ph << std::endl;
    focus->focusanticorrB(x[0], x[1], &result);

    tc->dp = result.dp;
    tc->th = result.th;
    tc->y0 = result.y0;
    tc->ph = result.ph;
    tc->len = result.len;
    //std::cout << tc->dp << " " << tc->th << " " << tc->y0 << " " << tc->ph << std::endl;
    //std::cout << result.dp << " " << result.th << " " << result.y0 << " " << result.ph << std::endl;
    //focus->focuscorrB(x[0], x[1], &result);
    //std::cout << result.dp << " " << result.th << " " << result.y0 << " " << result.ph << std::endl;
  }*/
  
  //std::cout << tc->dp << " " << tc->th << " " << tc->y0 << " " << tc->ph << std::endl;
  atree->packEventData(&(tc->th), 1);
  atree->packEventData(&(tc->ph), 1);
  atree->packEventData(&(tc->dp), 1);
  atree->packEventData(&(tc->y0), 1);

  return 1;

}                                                                   

