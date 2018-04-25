/*								        -*-c-*-
 * AQUA: data AcQUisition for A1 experiments
 *
 * Copyright (c) 2003
 *
 * Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
 * 55099 Mainz, Germany				fax  +49 6131 39-22964
 *
 * $Id: silicon.h 2216 2008-06-13 21:13:47Z distler $
 */

#ifndef __SILICON_H__
#define __SILICON_H__

struct Silicon_fadc_t {
  unsigned short hitime;
  unsigned   int lotime;
  short          energy;
  unsigned short status;
};

struct Silicon_V1724_t {
  unsigned long timetag_lo;
  unsigned short timetag_hi;
  struct {
    double maximum;
    unsigned short sample[400];
  } chan[6];
};

struct Silicon_t {
  unsigned short label;
  unsigned short adc[64];
  unsigned int clockticks;
  unsigned short runtime;
  unsigned short realtime;
  unsigned int scaler[8];
  unsigned short msx[12];
  unsigned short misc[32];
  struct Silicon_fadc_t fadc[8];
  unsigned int fadc_event;
  unsigned short trigger;
  unsigned int microticks;
  struct Silicon_V1724_t V1724;
};

#endif /* __SILICON_H__ */
