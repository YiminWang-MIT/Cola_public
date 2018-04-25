/*								        -*-c-*-
 * AQUA: data AcQUisition for A1 experiments
 *
 * Copyright (c) 1999-2015
 *
 * Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-5802
 * 55099 Mainz, Germany				fax  +49 6131 39-2964
 *
 * $Id: parity.h 1887 2012-10-26 14:22:58Z boehmr $
 */

// WARNING: Do not omit the keyword 'struct' below, e.g. in
//    struct MpsKAOS_Paddle fdet[30];
// although it is allowed (or even favored) by C++, as in the equivalent
//    MpsKAOS_Paddle fdet[30];
// The Cindy-Parser seems to need that 'struct' keyword!

#ifndef __PARITY_H__
#define __PARITY_H__

#include "spectrometer.h"


// old tr4ee for 2014 beamtime can be erased by 2016
#undef PARITY_BEAMTIME_2014

struct ParityAdc {
  unsigned short  linear;
  unsigned short  squared;
};

struct ParityXymo {
  unsigned short  x;
  unsigned short  y;
};

struct ParityBeam {
  struct ParityXymo  xymo[2];
  unsigned short     pimo[2];
  unsigned short     enmo[2];
};

#ifdef PARITY_BEAMTIME_2014
struct ParitySpectrometer {
  struct ParityAdc pmt[3];
};

struct ParityTimeslot {
  unsigned short             parity;
  struct ParitySpectrometer  a;
  struct ParitySpectrometer  b;
  struct ParityBeam          beam;
  struct ParityAdc           extra[2];
  unsigned short             spare[2];
  unsigned short             status;
};

#else
struct ParitySpectrometerB {
  struct ParityAdc pmt[3];
};

struct ParitySpectrometerA {
  struct ParityAdc pmt[9];
};


struct ParityTimeslot {
  unsigned short             parity;
  struct ParitySpectrometerA a;
  struct ParitySpectrometerB b;
  struct ParityBeam          beam;
  struct ParityAdc           extra[2];
  unsigned short             spare[2];
  unsigned short             status;
};
#endif



struct ParityGateHisto {
  unsigned short positive[2000];
  unsigned short negative[2000];
};

struct MpsDataParity {
  unsigned short  sync_info;
  unsigned int    clockticks;

  struct ParityTimeslot time[4];
  struct ParityGateHisto gatehisto;
  short polainverted;
};

#endif /* __PARITY_H__ */
