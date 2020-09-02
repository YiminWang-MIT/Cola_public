//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 2001
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: simDetectorBase.h 2588 2014-10-30 10:31:33Z kohly $
//

#ifndef __simDetectorBase_h__
#define __simDetectorBase_h__

#include "Cola/Masses.h"             // Definition of Particles
#include "Model/Model.h"
class simDetectorBase {
protected:
  class AquaTree *atree;
  /// Particle after energy loss/radiation
  Particle particle;
  /// Spectrometer angle, side of A is neg (rad)
  double angle;
  /// Out of plane angle (rad)
  double oop;
  /// Central momentum
  double momentum;
  /// +/- theta (rad)   range for generation > acceptance
  double dtheta;
  /// +/- phi (rad)     range for generation > acceptance
  double dphi;
  /// +/- momentum range for generation > acceptance
  double dmomentum;
public:
  simDetectorBase(class AquaTree *outtree, Particle *p,
		  double ang, double oopang, double mom,
		  double Dtheta, double Dphi, double Dmomentum) {
    atree     = outtree;
    particle  = (p ? *p : Particle());
    angle     = ang;
    oop       = oopang; 
    momentum  = mom; 
    dtheta    = Dtheta;
    dphi      = Dphi;
    dmomentum = Dmomentum;
  }
  virtual ~simDetectorBase() { ; }
  virtual int energyLossCorrection(class target *, double x[3]) = 0;
  virtual int resolution(struct TargetCoord *tc) = 0;
  virtual int check(Particle vf, double targetpos[3],
		    class target *, struct TargetCoord *tc,
		    struct simFocalCoord *fc, class Focus *focus, modeltype ModelType)
  { std::cerr<< "check not implemented! This just doesn't happen!"<<std::endl;
    return 0;};
  virtual double checkwithtrace(Particle vf, double targetpos[3],
				class target *, struct Spectrometer *onlS, modeltype ModelType) 
  { std::cerr<< "checkwithtrace not implemented! This just doesn't happen!"<<std::endl;
    return 0;};
  virtual const Particle & getParticle() { return particle; }
  virtual double getAngle()     { return angle; }
  virtual double getOop()       { return oop; }
  virtual double getMomentum()  { return momentum; }
  virtual double getDtheta()    { return dtheta; }
  virtual double getDphi()      { return dphi; }
  virtual double getDmomentum() { return dmomentum; }
};

#endif /* __simDetectorBase_h__ */
