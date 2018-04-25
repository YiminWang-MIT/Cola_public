//                                                                    -*-c++-*-
// $Id: simSpectrometer.h 2588 2014-10-30 10:31:33Z kohly $

#ifndef __simSpectrometer_h__
#define __simSpectrometer_h__

#include "simDetectorBase.h"
#include "DSpectrometer.h"
#include "simChamber.h" 
#include "Model/Model.h"

class simSpectrometer : public simDetectorBase {
protected:
  int idecay;               // flag for particle decay
  int        vacuum;      // Vacuum connection to scattering chamber
  Collimator *collimator; // Collimator
  Chamber *chamber;       // Chamber acceptance simulation
  int        steps;       // steps in energyloss correction
  double     *onlElossCorr;  // ptr to online.Spec struct 
  double     *onlElossSim;   // ptr to online.Spec struct 
  DSpectrometer *decayspectrometer; // ptr to Spectrometer which contains the decaying particle
public:
  double momentumRes;       // Spec Momentum Resolution (relative)
  double angularRes;        // Spec Angular Resolution in mrad

  int ndpcuts;              // Number of regions in dp
  double *dpcuts;           // Regions in which the mratio/aratio are valid 
  double momentumRes2;      // 2nd Spec Momentum Resolution (relative)
  double *mratio;           // Probability momRes2/momRes 
  double angularRes2;       // 2nd Spec Angular Resolution in mrad
  double *aratio;           // Probability angRes2/angRes
  double verti;             // Ratio of errors for vertical/horizontal angle 

  double refmomentum;       // Reference momentum correction factor
  double acceptance;        // Nominal momentum acceptance
  simSpectrometer(class AquaTree *outtree, 
		  double ang, double angRes, double angRes2, double verti,
		  double mom, double momRes, double momRes2,
		  int ndpcuts, double *dpcuts, double *aratio, double *mratio, 
		  double refmom, double akz, 
		  const char *colli, Collimator* ColliList[], 
		  int vac, Particle *p, double *Eloss_corr, double *Eloss_sim,
		  double Dtheta, double Dphi, double Dmomentum, 
		  double oopang, int decay, char * chamberfile=NULL);
  virtual int energyLossCorrection(class target *, double x[3]);
  virtual int resolution(struct TargetCoord *tc);
  virtual int check(Particle vf, double targetpos[3],
		    class target *, struct TargetCoord *tc,
		    struct simFocalCoord *fc, modeltype ModelType);
};

#endif /* __simSpectrometer_h__ */
