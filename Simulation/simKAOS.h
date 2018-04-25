//                                                                    -*-c++-*-
#ifndef __simKAOS_h__
#define __simKAOS_h__

#include "simDetectorBase.h"
#include "Model/Model.h"

class simKAOS : public simDetectorBase {
protected:
  int        vacuum;      // Vacuum connection to scattering chamber
  Collimator *collimator; // Collimator
  int        steps;       // steps in energyloss correction
  double     *onlElossCorr;  // ptr to online.Spec struct 
  double     *onlElossSim;   // ptr to online.Spec struct 

  double ThBinMin;
  double PhiBinMin;
  double dpBinMin;
  double ThBinWidth;
  double PhiBinWidth;
  double dpBinWidth;
  int ThBinCount;
  int PhiBinCount;
  int dpBinCount;

  double *accprob;

public:
  double refmomentum;           // Reference momentum correction factor
  double momentumRes;           // Spec Momentum Resolution (relative)
  double angularRes;            // Spec Angular Resolution in mrad
  double angularRes2;           // Spec Angular Resolution in mrad

  double acceptance;        // Nominal momentum acceptance
  simKAOS(class AquaTree *outtree, 
	  double ang, double angRes, double angRes2,
	  double mom, double momRes,
	  const char *colli, Collimator* ColliList[],
	  int vac, Particle *p, double *Eloss_corr, double *Eloss_sim,
	  double Dtheta, double Dphi, double Dmomentum);
  virtual int energyLossCorrection(class target *, double x[3]);
  virtual int resolution(struct TargetCoord *tc);
  virtual int check(Particle vf, double targetpos[3],
		    class target *, struct TargetCoord *tc,
		    struct simFocalCoord *fc, modeltype ModelType);
};

#endif /* __simKAOS_h__ */
