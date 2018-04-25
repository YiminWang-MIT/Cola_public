/* 
   Changes:

   11.1.94: A. Richter
            deadtimes converted to long (int)
   6.12.93: A. Richter
            struct BeamInfo: faraday, ps_faraday added
   5.1.94:  A. Richter
            struct EventInfo: a_b_time_long added (stop 50 ns earlier)
*/

#ifndef _SPECTROMETER_H_
#define _SPECTROMETER_H_

/**************************************************************************
 *                                                                        *
 *   Struktur von A1-Events                                               *
 *                                                                        *
 **************************************************************************/

/**************************************************************************
 *   A1 - baseset(.h)                                                     *
 **************************************************************************/

#include "detector.h"

struct BeamInfo {
  short foerster;
  short ps_foerster;
  short photo;
  short ps_photo;
  short faraday;
  short ps_faraday;
  short position;           /* Beamwedler */
};

/*
 * Run-Information
 */

struct RunInfo {
  short runtime;	  /* general gate on (100 us)               */
  short realtime;         /* micro not busy (100 us)                */
  short ps_runtime;       /* prescaled runtime (1 s)                */
  short ps_realtime;      /* prescaled realtime (1 s)               */
  short interupts;        /* prescaled interupts (1 k)              */
};

struct EventInfo {
  short a_b_time;
  short a_b_time_long;
  short	a_b_coinc;
  long dead_a;
  long dead_b;
  long dead_c;
  long dead_ab;
  long dead_ac;
  long dead_bc;
  long dead_abc;
};

struct DetA {
  struct VdcA        vdc;
  struct TriggerA    trigger;
  short	eventff;
  short	sync_info;
};

struct DetB {
  struct VdcB        vdc;
  struct TriggerB    trigger;
  short	eventff;
  short	sync_info;
};

#endif
