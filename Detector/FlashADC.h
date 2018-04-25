//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 2008
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: FlashADC.h 2249 2008-10-09 15:31:37Z makek $
//

#ifndef __FLASHADC_H__
#define __FLASHADC_H__

#include "detectorBase.h"

const int fADC_channels = 6;
const int fADC_samples  = 400;
const int fADC_words  = fADC_samples/2;

const int fADC_osc = getenv("FADC_OSC") != NULL;

const int BL_points=2;
const int BL_position=-2;

class FlashADC : public detectorBase {
protected:
  AquaNodeUShort *samples_ch[fADC_channels][fADC_samples];
  AquaNodeDouble *maximum[fADC_channels];
  AquaNodeDouble *E_ch[fADC_channels];
 
  int M,RT,FT_min,FT_max;
  int L[fADC_channels],Epts[fADC_channels];
  int Lmin, Lmax, Smin, Smax, Estart, Estop, Sstart;
  int d[fADC_channels],s[fADC_channels],pprime[fADC_channels],trapezoid[fADC_channels][fADC_samples*2];
  unsigned short smpl[fADC_channels][fADC_samples*2];
  int baseline_s[fADC_channels], baseline_t[fADC_channels];  
  double Etmp[fADC_channels], Ltmp[fADC_channels];
  bool threshold[fADC_channels];
  int trigger_mark[fADC_channels];
  double pileup_ctrl, pileup_thr;

public:
  FlashADC(AquaTree *aquatree, AquaTree *outtree);
  virtual ~FlashADC();
  virtual int init(double M_=940, double RT_=14, double FT_min_=100, double FT_max_=100, \
		     double Sstart_=50, double pileup_ctrl_=0, double pileup_thr_=0);
  virtual int handle();
  virtual int printSamples();
};

#endif /* __FLASHADC__ */
