//								      -*-c++-*-
// AQUA: data AcQUisition for A1 experiments
//
// Copyright (c) 2003
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: ndet.h 2347 2009-10-08 10:34:37Z schlimme $
//

#ifndef __BaselNeutronDetector__
#define __BaselNeutronDetector__

#include "Aqua/AquaTree.h"
struct NDET;

class NDetdEBar {
protected:
public:
  NDetdEBar(AquaTree *atree, AquaTree *out, int index);
  virtual ~NDetdEBar();
  int m_index;
  AquaNodeShort *mANSa_Adc, *mANSa_Tdc;
  AquaNodeInt *mANIo_Veto;
  int handle();//bss tempdouble TimeOfTrigger);
};


class NDetdE {
protected:
public:
  NDetdE(AquaTree *atree, AquaTree *out);
  virtual ~NDetdE();
  NDetdEBar *dEBar[9];
  AquaNodeShort *mANS_Adc[9], *mANS_Tdc[9];
  //  AquaNodeDouble *mAND_ECorr[9], *mAND_TCorr[9];
  //  AquaNodeInt *mANI_Veto[9];

  //  int ok() { return okFlag; }
  //  int id() { return index; }
  int handle();//bss tempdouble TimeOfTrigger);
  int Veto(int bar);
  //  int init(double *ped);
};

class NDetBar {
 protected:
  int index, okFlag, offsUp, offsDown, offsPos;
  double pedUp, pedDown;
  double flasherUp, flasherDown,  flash2energyUp, flash2energyDown, flasherstartingUp, flasherstartingDown;
  double AdcParticlePeakUp, AdcParticlePeakDown;
  double TdcMulti1GaugeX0, TdcMulti1GaugeY0, TdcMulti1GaugeR; 
  double offsTime;
  int la_yer;
  int ba_r;
  int gmeanOKFlag;
  AquaNodeShort *mAN_AdcUp, *mAN_AdcDown, *mIn_TdcUp, *mIn_TdcDown;
  AquaNodeDouble *mAN_AdcUpCorr, *mAN_AdcDownCorr, *mAN_Gmean, *mAN_Position, *mAN_EUp, *mAN_EDown, *mAN_tUp, *mAN_tDown, *mAN_AdcParticlePeakUp, *mAN_AdcParticlePeakDown, *mAN_GmeanParticlePeak, *mAN_AdcPedestalUp, *mAN_AdcPedestalDown;
  AquaNodeDouble *mOut_TdcUp, *mOut_TdcDown;
 public:
  NDetBar(AquaTree *atree, AquaTree *out, int layer, int bar);
  virtual ~NDetBar();
  int ok() { return okFlag; }
  int id() { return index; }
  int handle();//bss tempdouble TimeOfTrigger);
  double timing();
  double get_gmean();
  double TdcDistanceToMulti1Gauge(double x0, double y0, double R, double Tdc1, double Tdc2);
  double timeAtTarget();
  AquaNodeDouble *mAN_TdcMulti1Distance;
  //bssold: int init(double *offs, double *ped, double *pos, double *tim);
  int init(double *offs, double *flped, double *pos, double *tim, double *flpeak, double *adcped, double *adcpeak, double *TdcMulti1Gauge);
  int TDCsignalFlag(int BestPaddle, int BestPaddleLayer); //0: n/a 1:OK 2:overflow
  int gmeanOK();
};

const int NDETbars = 5;
class NDetLayer {
 protected:
  double bestTiming;
  int okFlags, bestPaddle;
  NDetBar *bar[NDETbars];
  double highest_gmean;
  AquaNodeDouble *highest_layer_gmean;
  double sum_gmean;
  AquaNodeDouble *sum_layer_gmean;
 public:
  NDetLayer(AquaTree *atree, AquaTree *out, int layer);
  AquaNodeDouble *mAN_BestTdcMulti1Distance;
  virtual ~NDetLayer();
  int ok() { return okFlags; }
  int handle();//bss tempint TimeOfTrigger);
  //  double return_BestTdcMulti1Distance();
  double timing() { return bestTiming; }
  double get_highest_gmean();
  double get_sum_gmean();
  int paddle() { return bestPaddle; }
  double timeAtTarget() { return (bestPaddle<0 ? 0.0 :
				  bar[bestPaddle%NDETbars]->timeAtTarget()); };
  //bssold: int init(double *offs, double *ped, double *pos, double *tim);
  int init(double *offs, double *flped, double *pos, double *tim, double *flpeak, double *adcped, double *adcpeak, double *TdcMulti1Gauge);
  int number_TDCs;
  int gmeanOK(int BarToCheck);
  int TDCsignalFlag(int BarToCheck, int BestPaddle);
};

const int NDETlayers = 6;
class NDetector {
 protected:
  int okFlags, bestPaddle;
  AquaNodeInt *mAND_TdcAN;
  AquaNodeDouble *mAND_ATimeAtTarget;
  AquaNodeDouble *mANDo_TimeOfTrigger;
  double bestTiming;
  AquaNodeDouble *mAN_BestTdcMulti1Distance;
  unsigned int bsstools;
  NDetdE *dE;
  NDetLayer *layer[NDETlayers];
  AquaNodeInt *multi, *pattern, *charged, *sync_info_a, *sync_info_ndet;
  AquaNodeShort *paddle;
  AquaNodeDouble *timeAtTarget;
  double highest_gmean; //highest gmean of all 30 bars
  AquaNodeDouble *highest_ndet_gmean;
  double highest_26gmean; //highest gmean of layers 2, 3, 4, 5 and 6
  AquaNodeDouble *highest_ndet_26gmean;
  double sum_gmean; //sum of all 30 gmeans
  AquaNodeDouble *sum_ndet_gmean;
  int number_TDCs;
  AquaNodeInt *temp_check;
  int particleType;
  int identify(unsigned int multiplicity);
  int PrintTdcPattern(long int TdcUpPattern, long int TdcDnPattern);
  void easyPatternPlot(int easypattern[NDETlayers][NDETbars], unsigned int multiplicity);
  int gmeanOK(int LayerToCheck, int BarToCheck);
  int TDCsignalFlag(int LayerToCheck, int BarToCheck);
  //  int Veto(int bar);
 public:
  NDetector(AquaTree *atree, AquaTree *out);
  virtual ~NDetector();
  int ok() { return okFlags; }
  int handle();
  int init(struct NDET *);
};

#endif /* __BaselNeutronDetector__ */



//bss old:
//void ndetFill(AquaTree *atree, AquaTree *out, 
//	      struct MpsDataN *ndet, struct Ndet *N);

