/* 
   Changes:

   7.7.94:  Wagner/Wilhelm
            position_sem added
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

struct MpsBeamInfo {
  short foerster;
  short ps_foerster;
  short photo;
  short ps_photo;
  short faraday;
  short ps_faraday;
  short position;           /* Beamwedler */
  short position_sem;
};

/*
 * Run-Information
 */

struct MpsRunInfo {
  short runtime;	  /* general gate on (100 us)               */
  short realtime;         /* micro not busy (100 us)                */
  short ps_runtime;       /* prescaled runtime (1 s)                */
  short ps_realtime;      /* prescaled realtime (1 s)               */
  short ps_interrupts;    /* prescaled interrupts (1 k)             */
};

struct MpsEventInfo {
  short a_b_time_1;
  short a_b_time_2;
  short a_b_time_3;
  short a_c_time_1;
  short a_c_time_2;
  short a_c_time_3;
  short b_c_time_1;
  short b_c_time_2;
  short b_c_time_3;
  short	a_b_c_coinc;
  short	a_b_coinc;
  short	a_c_coinc;
  short	b_c_coinc;
  long  dead_a;
  long  dead_b;
  long  dead_c;
  long  dead_ab;
  long  dead_ac;
  long  dead_bc;
  long  dead_abc;
};

struct MpsDetA {
  struct MpsVdcA         vdc;
  struct MpsTriggerAC    trigger;
  short	 sync_info;
};

struct MpsDetB {
  struct MpsVdcB         vdc;
  struct MpsTriggerB     trigger;
  short	 sync_info;
};

struct MpsDetC {
  struct MpsVdcC         vdc;
  struct MpsTriggerAC    trigger;
  short  sync_info;
};

#endif
