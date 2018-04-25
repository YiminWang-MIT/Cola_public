//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 2004
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
//

#ifndef __NDET_H__
#define __NDET_H__

#include "detectorBase.h"
#include "Aqua/AquaTRB.h"
#include <vector>


#define NDET_NUMBER_TDC_CHANNELS 28
#define NDET_CHANNELS_PER_TDC    64


struct channelHit
{
  int bar;
  int pmt;
  int edge;
};


class Ndet2016 : public detectorBase {

private:
  static const int leading = 1;
  static const int trailing = 0;
  static const int top = 1;
  static const int bottom = 0;
 // std::vector <double[1024]> timeCalibrationData;

protected:
  AquaTRBsubevent *aquaTrbSubevent;
  AquaTRBsubevent **trbSubevent;
  trbTDCdata *dataPtr = NULL;
  channelHit tdc2pmtMapping(int trb, int tdc, int channel);
  int setStandardCalibrationData();
  int loadCalibrationData();
  int channelNumber(int trb, int tdc, int channel);
  
public:
  Ndet2016(AquaTree *aquatree, AquaTree *outtree, AquaTRBsubevent **trbSubevent);
  virtual ~Ndet2016();
  virtual int init(char *mappingFile, char *calibrationFile);
  virtual int handle();
};

#endif // __NDET_H__
  
