//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 2001
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: simDetector.h 2588 2014-10-30 10:31:33Z kohly $
//
// M.Distler 21.5.14 Revision 2559	removed Hadron3 and ToF code
//					class GridCell
//					class simHadron3
//					class simToF

#ifndef __simDetector_h__
#define __simDetector_h__

#include "Cola/include/online/online.h"
#include "simDetectorBase.h"
#include "Model/Model.h"
#include "Colli.h"

class simDummy : public simDetectorBase {
public:
  simDummy(class AquaTree *outtree, Particle *p,
	   double ang, double oopang, double mom,
	   double Dtheta, double Dphi, double Dmomentum)
    : simDetectorBase(outtree, p, ang, oopang, mom,
		      Dtheta, Dphi, Dmomentum) { ; }
  virtual ~simDummy() { ; }
  virtual int energyLossCorrection(class target *, double x[3]) { return 0; }
  virtual int resolution(struct TargetCoord *) { return 0; }
  virtual int check(Particle vf, double x[3],
		    class target *, struct TargetCoord *, struct simFocalCoord *fc, modeltype Modeltype) { return 1; }
};

class simNpol : public simDetectorBase {
public:
  simNpol(class AquaTree *outtree, Particle *p,
	  double ang, double oopang, double mom,
	  double Dtheta, double Dphi, double Dmomentum)
    : simDetectorBase(outtree, p, ang, oopang, mom,
		      Dtheta, Dphi, Dmomentum) { ; }
  virtual ~simNpol() { ; }
  virtual int energyLossCorrection(class target *, double x[3]) { return 0; }
  virtual int resolution(struct TargetCoord *) { return 0; }
  virtual int check(Particle vf, double x[3], class target *,
		    struct TargetCoord *, struct simFocalCoord *, modeltype ModelType) { return 1; }
};

class simNdet : public simDetectorBase {
protected:
  double distance, height, width;
  colliPlane *colli;
public:
  simNdet(class AquaTree *outtree, Particle *p, double ang, double oopang,
	  double dist, double hght, double wdth, const char *filename);
  virtual ~simNdet();
  virtual int energyLossCorrection(class target *, double x[3]) { return 0; }
  virtual int resolution(struct TargetCoord *) { return 0; }
  virtual int check(Particle vf, double x[3], class target *,
		    struct TargetCoord *, struct simFocalCoord *fc, modeltype ModelType);
};

#endif /* __simDetector_h__ */
