/*								        -*-c-*-
 * AQUA: data AcQUisition for A1 experiments
 *
 * Copyright (c) 2016
 *
 * Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-5802
 * 55099 Mainz, Germany				fax  +49 6131 39-2964
 *
 * $Id$
 */

// WARNING: Do not omit the keyword 'struct' below, e.g. in
//    struct MpsKAOS_Paddle fdet[30];
// although it is allowed (or even favored) by C++, as in the equivalent
//    MpsKAOS_Paddle fdet[30];
// The Cindy-Parser seems to need that 'struct' keyword!

#ifndef __TRB_H__
#define __TRB_H__

#define NDET_TRB_NUMBER_TRBS 4
#define NDET_TRB_NUMBER_TDCS 4
#define NDET_TRB_NUMBER_CHANNELS 64

struct NdetTdcChannel {
  unsigned long        epoch;     // 28 bit
  unsigned short       coarse;    // 11 bit
  unsigned short       fine;      // 9 bit
};

struct NdetTdc {
  unsigned long        error; // bufferOverflow: bit 1
  struct NdetTdcChannel chan[NDET_TRB_NUMBER_CHANNELS+1];
};


struct NdetTrb {
  struct NdetTdc    tdc[NDET_TRB_NUMBER_TDCS];
};


struct DataNdetTrb {
  unsigned short       sync_info; // 
  unsigned short       label;     // 0x00A1
  unsigned long        clockticks;      // time when the UTF package arrived
  unsigned long        triggerNumber;  // 32 bit?

  struct NdetTrb    trb[NDET_TRB_NUMBER_TRBS];
};

#endif /* __TRB_H__ */
