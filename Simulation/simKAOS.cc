//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 1999-2001
//
// Institut fr Kernphysik, Universit� Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
//

#include <iostream> 
#include <fstream> 
#include "FourVector/FourVector.h"      // relativistic four vector arithmetics
#include "Colli.h"                   // Definition of Collimators
#include "Cola/QuasiRandom.h"           // Random Generator
#include "Cola/include/online/online.h" // 1st level reconstructed data
#include "Cola/Targetlength.h"       // Targetcells and Energyloss
#include "Cola/Luminosity.h"
#include "simKAOS.h"
#include "SimParticle.h"
#include "ran1.h"
#include "Cola/rundatabase.h"

extern int ergloss;
extern modeltype ModelType;
using namespace std;

simKAOS::simKAOS(AquaTree *outtree, 
		 double ang, double angRes, double angRes2,
		 double mom, double momRes,
		 const char *colli, Collimator* ColliList[],
		 int vac, Particle *p,
		 double *Eloss_corr, double *Eloss_sim,
		 double Dtheta, double Dphi, double Dmomentum)
  : simDetectorBase(outtree, p, ang, 0, mom, Dtheta, Dphi, Dmomentum)
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
  refmomentum = mom;
  momentumRes = momRes;
  angularRes  = angRes;
  angularRes2 = angRes2;

  if (rundb.kaos.acceptance)
    {
      ifstream acceptfile(rundb.kaos.acceptance);

      if (acceptfile)
	{     
	  if (!(acceptfile >> ThBinCount>> PhiBinCount>>dpBinCount))
	    cerr << "Error reading bin count" << endl;

	  double TempTh, TempPhi, Tempdp, TempAcceptance;
	  if (acceptfile >> TempTh >> TempPhi >> Tempdp)
	    {
	      ThBinMin  = TempTh*M_PI/180*1000;  // min theta (mrad)
	      PhiBinMin = TempPhi*M_PI/180*1000; // min phi  (mrad)
	      dpBinMin  = Tempdp;                // min dp
	    }
	  else cerr << "Error reading bin minimum" << endl;
	  if (acceptfile >> TempTh >> TempPhi >> Tempdp)
	    {
	      ThBinWidth  = TempTh*M_PI/180*1000;
	      PhiBinWidth = TempPhi*M_PI/180*1000;
	      dpBinWidth  = Tempdp;
	    }
	  else cerr << "Error reading bin width" << endl;
	  
	  accprob=(double *)malloc(sizeof(double) *
				   ThBinCount * PhiBinCount * dpBinCount);
	  /*	  
	  cout << "Kaos acceptance file no of bins (dp, theta, phi): " 
	       << dpBinCount << " " << ThBinCount << " " << PhiBinCount << endl;
	  cout << "Kaos acceptance file first bins (dp, theta, phi): " 
	       << dpBinMin << " " << ThBinMin << " " << PhiBinMin << endl;
	  cout << "Kaos acceptance file bin widths (dp, theta, phi): " 
	       << dpBinWidth << " " << ThBinWidth << " " << PhiBinWidth << endl;
	  */
	  for (int th=0;th<ThBinCount;th++)
	    for (int phi=0;phi<PhiBinCount;phi++)
	      for (int dp=0;dp<dpBinCount;dp++)
		{
		  if (acceptfile >> TempTh >> TempPhi >> Tempdp 
		      >> TempAcceptance)
		    {
		      accprob[th*dpBinCount*PhiBinCount+phi*dpBinCount+dp]=
			TempAcceptance;
		    }
		  else
		    {
		      cerr << "Error reading bin content: " 
			   << th*dpBinCount*PhiBinCount+phi*dpBinCount+dp 
			   << endl;
		    }
		}
	  acceptfile.close();
	}
      else  cerr << "no acceptance file found: "  
		 << rundb.kaos.acceptance << endl;
    }
  
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
  //  std::cout << "Init Complete" << std::endl;
  
}

int simKAOS::resolution(struct TargetCoord *tc)
{
  if (particle.getID() == 0) return -1;

  static PseudoRandom psrandom;
  static normal norm(&psrandom, &psrandom);

  double p  = particle.momentum(); 
  double E  = particle.energy(); 
  double theta = particle.theta();
  double phi   = particle.phi();

  double th    = -particle.specPhi(-angle)/1000;
  double ph    = particle.specTheta(-angle)/1000;
  double dp    = particle.momentum()/refmomentum-1;

  //    cout << "mRes: " << momentumRes 
  //         << " thRes: " << angularRes << " phiRes: " 
  //         << angularRes2 << endl;

  tc->th =  th + norm() * angularRes/1000;
  tc->ph =  ph + norm() * angularRes2/1000;
  tc->dp =  dp + norm() * momentumRes;

  double phires = norm() * angularRes2/1000;
  double thetares = norm() * angularRes/1000;
  double pn = p * (1 + norm() * momentumRes);

  particle.initPolar(sqrt(E*E - p*p + pn*pn), pn, thetares, phires);
  particle.rot_theta(theta);
  particle.rot_phi(phi);

  atree->packEventData(&(tc->th), 1);
  atree->packEventData(&(tc->ph), 1);
  atree->packEventData(&(tc->dp), 1);

  //  cout << "after resolution folding: theta: " << tc->th 
  //       << " phi: " << tc->ph 
  //       << " dp: " << tc->dp << endl << endl;

  return 0;
}

int simKAOS::energyLossCorrection(target *target, double x[3]) 
{
  double EnergyBefore = 0;
  if (!particle.getCharge()) return -1;
  if (!ergloss) return -1;

  EnergyBefore = particle.energy();
  if (!vacuum) target->EnergyLossCorrKaos(particle);
  target->EnergyLossCorr(particle, x[0], x[1], x[2], steps);
  if (onlElossCorr) *onlElossCorr = (particle.energy() - EnergyBefore);

  return 0;
}
  
int simKAOS::check(Particle vf, double x[3], target *target, 
		   struct TargetCoord *tc, struct simFocalCoord *fc, modeltype ModelType)
{
  static PseudoRandom psrandom;

  double EnergyBefore = 0;

  particle = vf;

  if (ergloss) {
    EnergyBefore = vf.energy();
    target->EnergyLossSim(particle, x[0], x[1], x[2], steps, ModelType);
    if (!vacuum) target->EnergyLossSimKaos(particle); 
    if (onlElossSim) *onlElossSim = (EnergyBefore - particle.energy());
  }

  if (!collimator->accepted(x, vf, -angle, oop, refmomentum)) return 0; 

  double deltap  = particle.momentum()/refmomentum;

  if (rundb.kaos.acceptance)
    {
      int dpBin      = (int)((deltap-dpBinMin)/dpBinWidth);
      int ThetaBin   = (int)((-particle.specPhi(-angle) -  ThBinMin)/ThBinWidth);
      int PhiBin     = (int)((particle.specTheta(-angle) - PhiBinMin)/PhiBinWidth);

      /*
	cout << "acceptance bin: theta phi dp: " 
	<< ThetaBin << " " 
	<< PhiBin   << " " 
	<< dpBin << endl;
	cout << "acceptance bin count theta phi dp: " 
	<< ThBinCount  << " " 
	<< PhiBinCount << " " 
	<< dpBinCount << endl;
      */
      
      if ((dpBin<0)    || (dpBin>=dpBinCount)) return 0;
      if ((ThetaBin<0) || (ThetaBin>=ThBinCount)) return 0;
      if ((PhiBin<0)   || (PhiBin>=PhiBinCount)) return 0;
      
      double rnd = psrandom();
      //  cout << "Bin Th Phi p: " << ThetaBin << " " << PhiBin << " " 
      //       << dpBin << endl;
      //  std::cout << "Probability to accept: " 
      //    << accprob[ThetaBin*dpBinCount*PhiBinCount+PhiBin*dpBinCount+dpBin] 
      //    << " < " << rnd << std::endl;

      if (accprob[ThetaBin*dpBinCount*PhiBinCount+PhiBin*dpBinCount+dpBin] < rnd)
    return 0;

      /*
	cout  << "angle: "   <<  angle/3.14*180 << " "
	<< "  theta: " <<  -particle.specPhi(-angle)/3.14*180/1000
	<< "  phi: "   <<  particle.specTheta(-angle)/3.14*180/1000
	<< endl;
      */
    }
  
  
  tc->th = -particle.specPhi(-angle)/1000;
  tc->ph =  particle.specTheta(-angle)/1000;
  tc->dp = (deltap-1);
  tc->y0 = (- x[0] * cos(angle) + x[2] * sin(angle))/10.0;//[cm]
  
  atree->packEventData(&(tc->th), 1);
  atree->packEventData(&(tc->ph), 1);
  atree->packEventData(&(tc->dp), 1);
  atree->packEventData(&(tc->y0), 1);
  
  //  cout << "generated: theta: " << tc->th 
  //       << " phi: " << tc->ph 
  //       << " dp: " << tc->dp << endl;

 return 1;

}                                                                   

