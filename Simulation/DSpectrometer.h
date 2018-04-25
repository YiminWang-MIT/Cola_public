//
// $Id: DSpectrometer.h 2216 2008-06-13 21:13:47Z distler $
//

#ifndef __DSPECTROMETER_H__
#define __DSPECTROMETER_H__

#include <math.h>

#include "Matrix3D.h"
#include "CK_RungeKutta.h"
#include "Magnets.h"
#include "DipolB.h"
#include "FourVector/FourVector.h"
#include "Colli.h"
#include "SimParticle.h"





//Constants:
const double SpeedOfLight = 299.792458; // Speed of light 'c' in [mm/nsec]
const double lkA = 565.0 + 80.0; // distance from target center 
                                 // to just behind the collimator in spec A
const double lkB = 3400.0;         // distance target center to ??? in spec B

class DSpectrometer{
protected: 
  Collimator *collimator;
  double Lk; // distance from target center to just behind the collimator.
  double phi;
  double theta;
public:		
  virtual int position_at_VDC(Vector3D Position, Vector3D Velocity, Vector3D *VDCPosition, double *theta, double *phi) = 0;
  virtual int simulate(SimParticleGroup *simParticleGroup1, double refp) = 0;
  virtual int showbound() = 0;
  virtual int checkbound() = 0;
  virtual int checkInsideSpectrometer() = 0;
  virtual ~DSpectrometer() { };
};

class DSpectrometerA : public DSpectrometer{
private:
  int fromTargetToSpectrometer(SimParticleGroup *simParticleGroup1);

public:
  int simulate(SimParticleGroup *simParticleGroup1, double refp);
  int position_at_VDC(Vector3D Position, Vector3D Velocity, Vector3D *VDCPosition,double *theta, double *phi);
  int showbound();
  int checkbound(); 
  int checkInsideSpectrometer();

  DSpectrometerA(Collimator* colli, double lk);
  DSpectrometerA(Collimator* colli, double lk, int fieldDirection);

  virtual ~DSpectrometerA() { };
};

class DSpectrometerB : public DSpectrometer{
private:
  int fromTargetToSpectrometer(SimParticleGroup *simParticleGroup1);
  
public:
  int simulate(SimParticleGroup *simParticleGroup1, double refp);
  int position_at_VDC(Vector3D Position, Vector3D Velocity, Vector3D *VDCPosition,double *theta, double *phi);
  int showbound();
  int checkbound();
  int checkInsideSpectrometer();
  int fieldMagnitude();
  
  DSpectrometerB(Collimator* colli, double lk);
  DSpectrometerB(Collimator* colli, double lk, int fieldDirection);
  virtual ~DSpectrometerB() { };
};

class DSpectrometerKAOS : public DSpectrometer{
  private:
    int fromTargetToSpectrometer(SimParticleGroup *simParticleGroup1);
      
  public:
    int simulate(SimParticleGroup *simParticleGroup1, double refp);
    int position_at_VDC(Vector3D Position, Vector3D Velocity, Vector3D *VDCPosition,double *theta, double *phi);
    int showbound();
    int checkbound();
    int checkInsideSpectrometer();
    int fieldMagnitude();
    
    DSpectrometerKAOS(Collimator* colli, double lk, int fieldDirection);
    virtual ~DSpectrometerKAOS() { };
};




// TO-DO: Here shoud be definitions of other spectrometers

#endif
