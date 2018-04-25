
#ifndef __simSpecwithTrace_h__
#define __simSpecwithTrace_h__

#include "Cola/include/online/online.h"
#include "simDetectorBase.h"
#include "TMA/transandlook.h"
#include "Model/Model.h" 

#define maxwirecount 1000

struct layerinfo
{
  double poswireone;
  double slope;
  int maxwire;
  double probability[maxwirecount];
};


class simSpecwithTrace : public simDetectorBase {
protected:
  int        vacuum;      // Vacuum connection to scattering chamber
  Collimator *collimator; // Collimator
  int        steps;       // steps in energyloss correction
  double     *onlElossCorr;  // ptr to online.Spec struct 
  double     *onlElossSim;   // ptr to online.Spec struct 
  TL         transform;
  struct layerinfo li[4];
  double sin_x_s,sec_x_s, cos_x_s, csc_x_s, tan_x_s,cot_x_s;
  double     SimulVDC(struct Spectrometer *onlS);
  int pos2wire(int layer,double pos); 

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
  simSpecwithTrace(class AquaTree *outtree, 
		  double ang, double angRes, double angRes2, double verti,
		  double mom, double momRes, double momRes2,
		  int ndpcuts, double *dpcuts, double *aratio, double *mratio, 
		  double refmom, double akz, 
		  const char *colli, Collimator* ColliList[], 
		  int vac, Particle *p, double *Eloss_corr, double *Eloss_sim,
		  double Dtheta, double Dphi, double Dmomentum, 
		  double oopang);
  virtual int energyLossCorrection(class target *, double x[3]);
  virtual int resolution(struct TargetCoord *tc);
  virtual int check (Particle vf, double targetpos[3],
		    class target *, struct TargetCoord * tc);
  virtual double checkwithtrace(Particle vf, double targetpos[3],
				class target *,struct Spectrometer *onlS, modeltype ModelType);
};

#endif
