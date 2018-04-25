/*								        -*-c-*-
 * AQUA: data AcQUisition for A1 experiments
 *
 * Copyright (c) 1999-2015
 *
 * Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-5802
 * 55099 Mainz, Germany				fax  +49 6131 39-2964
 *
 * $Id: spekk.h 2657 2015-09-22 10:35:46Z distler $
 */

// WARNING: Do not omit the keyword 'struct' below, e.g. in
//    struct MpsKAOS_Paddle fdet[30];
// although it is allowed (or even favored) by C++, as in the equivalent
//    MpsKAOS_Paddle fdet[30];
// The Cindy-Parser seems to need that 'struct' keyword!

#ifndef __SPEKK_H__
#define __SPEKK_H__

#include "spectrometer.h"

struct MpsKAOS_Channel {
  unsigned short time;
  unsigned short charge;
};

struct MpsKAOS_Paddle {
  struct MpsKAOS_Channel top; 
  struct MpsKAOS_Channel bottom; 
};

struct MpsKAOS_TOF {
  struct MpsKAOS_Paddle fdet[30];
  struct MpsKAOS_Paddle gdet[30];
  struct MpsKAOS_Channel xtra[8];
};

struct MpsKAOS_Aerogel {
  struct MpsKAOS_Paddle segment[16];
};

struct MpsKAOS_MWPCHAMBER {
  unsigned short sync_info;
  unsigned int   clockticks;
  //  unsigned short y[70]; //real number of wires
  unsigned short y[80]; //number of Transputer channels
  unsigned short x[240];
};

struct MpsKAOS_MWPC {
  unsigned short sync_info;
  unsigned int   clockticks;
  struct MpsKAOS_MWPCHAMBER l;
  struct MpsKAOS_MWPCHAMBER m;
};

struct MpsKAOS_FIBRE_TDC {
  unsigned short channel;
  unsigned short time;
};

struct MpsKAOS_FIBRE_F1Status {
  unsigned short post;
  unsigned short geo_id;
  unsigned short error_code_special;
  unsigned short error_code;
};

struct MpsKAOS_FIBRE_CatchStatus {
  unsigned short error;
  unsigned short eventtype;
  unsigned short sourceID;
  unsigned short event_size;
  unsigned short status;
  unsigned short spill_number;
  unsigned int event_number;
  unsigned short format_version;
  unsigned int error_words;
  unsigned int TCS_error;
  unsigned int status2;
  unsigned int eventsizeerror;
  struct MpsKAOS_FIBRE_F1Status F1status[4];
};

#define KAOS_FIBRE_MAXHITS 300
#define KAOS_FIBRE_MAXCATCH 24

struct MpsKAOS_FIBRE_PLANE {
  unsigned short sync_info;
  unsigned int   clockticks;
  struct MpsKAOS_FIBRE_CatchStatus CatchStatus[KAOS_FIBRE_MAXCATCH];
  struct MpsKAOS_FIBRE_TDC hit[KAOS_FIBRE_MAXHITS];
  
};

struct MpsKAOS_FIBRE {
  unsigned short sync_info;
  unsigned int   clockticks;
  struct MpsKAOS_FIBRE_PLANE x;
  struct MpsKAOS_FIBRE_PLANE y;
  struct MpsKAOS_FIBRE_PLANE th;
};


struct MpsDataKAOS {
  unsigned short        label;
  struct MpsRunInfo	run;
  struct MpsKAOS_TOF    tof;
  struct MpsKAOS_MWPC   mwpc;
  struct MpsKAOS_FIBRE  fibre;
  unsigned short        sync_info;
  unsigned int          clockticks;

  unsigned int          dead_k;
  unsigned int          dead_b;
  unsigned int          dead_c;
  unsigned int          dead_kb;
  unsigned int          dead_kc;
  unsigned int          dead_bc;
  unsigned int          dead_kbc;

  struct MpsKAOS_Aerogel  aerogel;
  struct MpsBeamInfo      beam;   // new stuff always at bottom!
};

#endif /* __SPEKK_H__ */
