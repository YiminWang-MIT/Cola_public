//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 2004-2008
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: SiliconDetector.h 2299 2009-05-10 19:24:10Z makek $
//

#ifndef __SILICON_DETECTOR_H__
#define __SILICON_DETECTOR_H__

#include "detectorBase.h"
#include "Cola/Masses.h"
#include "Cola/Material.h"

const int siliconDetectorAdcMax = 48; //Can go up to 64, but we use 48
const int AquaUnit08A8channels = 6;
const int fadcChannelsMax = 6;

class SiliconDetector : public detectorBase {
protected:
  AquaNodeUShort *adc[siliconDetectorAdcMax];
  AquaNodeUShort *msx[6];
  AquaNodeUShort *tdc[5]; 
  AquaNodeUShort *misc[1];
  AquaNodeDouble *maximum[AquaUnit08A8channels];
  AquaNodeDouble *fadc_v1724[AquaUnit08A8channels];
  AquaNodeShort *fadc_n1728[4];
  AquaNodeDouble *si_theta; 
  AquaNodeDouble *si_phi; 
  AquaNodeDouble *E_scint;
  AquaNodeDouble *E_MSX[5];
  AquaNodeDouble *E_fADC[fadcChannelsMax];
  AquaNodeDouble *E_VETO;
  AquaNodeDouble *E_bb2a, *E_bb2b;
  AquaNodeDouble *E_A[24];
  AquaNodeDouble *E_B[24];
  AquaNodeDouble *E_rec[3];
  
  AquaNodeDouble *EnergyParticle;
  AquaNodeDouble *MomentumParticle;
  AquaNodeDouble *ThetaParticle;
  AquaNodeDouble *PhiParticle;
 
  AquaNodeDouble *EnergyP_fadc;
  AquaNodeDouble *MomentumP_fadc;

  AquaNodeUShort *OK;
 
  double a_A[24];
  double b_A[24];
  double a_B[24];
  double b_B[24];
  double a_MSX[6];
  double b_MSX[6];
  double a_fADC[fadcChannelsMax];
  double b_fADC[fadcChannelsMax];
  double a_scint,b_scint;
  
  int threshold[54]; //from file specified in run.db by Si.config (or default) //Only 48 are needed, but 54 is left for backwards compatibility
  double thick[8];//from file specified in run.db by Si.config (or default)
  double angle; //from Si.angle in run.db (or default)
  double distance; //from Si.distance in run.db (or default)
  int USE_FADC; //from use_fADC in run.db (default = 0)
  int fadcChannels;
  
  //static const int d = 89; //distance from the target [mm]

public:
  SiliconDetector(AquaTree *aquatree, AquaTree *outtree, int use_fadc);
  virtual ~SiliconDetector();
  /** initialize various parameters for analysis or use save values */
  virtual int init(const char *filename=NULL, double angle_=90.0, double distance_=8.0);
  virtual int handle();
  virtual int print();
  virtual void setFADC(int use_fadc_ = 0);
 };

class Layer{
private:
    double LayerSteps;
    double LayerThickness;
    Material *material;
    double path;
public:
  Layer(Material *mat, double thickness, double steps);
  double steps(void) {return LayerSteps;}
  double thickness(void) {return LayerThickness;}
  double pathlength(double th, double ph);
  double Eloss(Particle part);
  double Egain(Particle part);
  double initial;
};

#endif /* __SILICON_DETECTOR_H__ */
