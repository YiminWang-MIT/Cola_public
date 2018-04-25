/*                                                                      -*-c-*-
 * the guys to blame:
 *
 * Michael O. Distler                        mailto:distler@kph.uni-mainz.de
 * Insitut fuer Kernphysik, Univ. Mainz      +49 6131 39 22956
 * Mainz, FRG                                +49 6131 39 22964
 *
 * Itaru Nakagawa                            mailto:itaru@mitlns.mit.edu
 * Simon Sirca                               mailto:sirca@mitlns.mit.edu
 * MIT, Lab for Nuclear Science, 26-402      tel.   (617) 253-3051 (Itaru)
 * Cambridge, MA 02139                       tel.   (617) 258-5438 (Simon)
 *                                           fax.   (617) 258-5440
 *
 * $Header: /tmp/cvsroot/Cola/ColaMIT/include/oops1/oops1.h,v 2.1 2002-03-05 03:50:22 distler Exp $
 *
 * oops data structure
 *
 */

#ifndef __OOPS_1_H__
#define __OOPS_1_H__

#include "datatypes.h"

struct event3 {
  Uword16 scaler;
};

struct oops_scint_scaler {
  Uword32 right;
  Uword32 left;
  Uword32 left_AND_right;
  struct {
    Uword32 right;
    Uword32 left;
  } low;
};

struct oops_hdc_scaler {
  struct {
    Uword32 top;
    Uword32 bottom;
  } x;
  struct {
    Uword32 right;
    Uword32 left;
  } y;
}
;
struct helicity_scaler {
  Uword32 oopsA_prescal;
  Uword32 oopsB_prescal;
  Uword32 oopsC_prescal;
  Uword32 oopsD_prescal;
  Uword32 ohips_prescal;
  Uword32 oops_prescal_OR;
  Uword32 coincidence;
  Uword32 oops_live;
  Uword32 ohips_live;
  Uword32 oops_trigger;
  Uword32 ohips_trigger;
  Uword32 master_trigger;
};

struct oops_scaler {
  struct oops_scint_scaler scint[4];
  struct oops_hdc_scaler hdc[4];
  Uword32 prescaler;
  Uword32 and_123;
  Uword32 and_12;
  Uword32 sincb;
  Uword32 fired;
  Uword32 prescalercb;
  Uword32 and_123cb;
  Uword32 and_12cb;
};

struct ohips_scaler {
  Uword32 scaler53;
  Uword32 live54;
  Uword32 trigger;
  Uword32 scaler106;
  Uword32 prescal;
  Uword32 coincidence;
  Uword32 live109;
  Uword32 AND_oopsA;
  Uword32 AND_oopsB;
  Uword32 AND_oopsC;
  Uword32 scaler113;
  Uword32 scaler114;
  Uword32 scaler115;
  Uword32 scaler116;
  Uword32 scaler117;
  Uword32 sca[43];
  Uword32 event5;
  Uword32 bb;
  Uword32 bbcb;
};  

struct ohips_scaler_2000 {
  Uword32 AND_oopsA;
  Uword32 AND_oopsB;
  Uword32 AND_oopsC;
  Uword32 AND_oopsD;
  Uword32 AND_oopsAcb;
  Uword32 AND_oopsBcb;
  Uword32 AND_oopsCcb;
  Uword32 AND_oopsDcb;
  Uword32 pit74;
  Uword32 coincidence;
  Uword32 pit80cb;
  Uword32 coincb;
  Uword32 live;
  Uword32 trigger;
  Uword32 pit90cb;
  Uword32 prescal;
  Uword32 spare1; /* can be used for scaler93 */
  Uword32 spare2; /* can be used for scaler93 */
  Uword32 livecb;
  Uword32 triggercb;
  Uword32 pit102;
  Uword32 prescalcb;
  Uword32 event6;
  Uword32 sca[65];
};  

struct oopslive_scaler {
  Uword32 prescalOR;
  Uword32 retimingOR;
  Uword32 live;
  Uword32 trigger;
};  

struct oopslive_scaler_2000 {
  Uword32 retimingORcb;
  Uword32 singleORcb;
  Uword32 livecb;
  Uword32 triggercb;
  Uword32 retimingOR;
  Uword32 singleOR;
  Uword32 live;
  Uword32 trigger;
};  

struct dutyfactor_scaler {
  Uword32 scint1;
  Uword32 scint2;
  Uword32 coinc12;
  Uword32 timers;
  Uword32 coincAB;
};

struct event5 {
  struct oops_scaler oopsA;
  struct oops_scaler oopsB;
  struct oops_scaler oopsC;
  struct oops_scaler oopsD;
  struct oopslive_scaler oops;
  struct ohips_scaler ohips;
  struct helicity_scaler heli_negative;
  struct helicity_scaler heli_positive;
  Uword32 bt3bic;
  Uword32 computer_busy;
  Uword32 event_5;
  Uword32 scaler31;
  Uword32 busclc;
  Uword32 coincidence;
  Uword32 final_trigger;
  Uword32 scaler57;
  Uword32 clock;
  Uword32 sincb;
  Uword32 retcb;
  Uword32 live;
  Uword32 cb;
  Uword32 trig;
  struct dutyfactor_scaler df;
};

struct event6 {
  struct oops_scaler oopsA;
  struct oops_scaler oopsB;
  struct oops_scaler oopsC;
  struct oops_scaler oopsD;
  struct oopslive_scaler_2000 oops;
  struct ohips_scaler_2000 ohips;
  Uword32 final_triggercb;
  Uword32 coincidencecb;
  Uword32 final_trigger;
  Uword32 coincidence;
  Uword32 sync;
  Uword32 synccb;
  Uword32 cbinh;
  Uword32 gun;
  Uword32 guncb;
  Uword32 crm[25];
  Uword32 event_6;
};

struct oops_data {
  short noLAM;
  struct {
    struct {
      Uword16 right;
      Uword16 left;
    } adc;
    struct {
      Uword16 right;
      Uword16 left;
    } tdc, low;
  } scint[4];
  struct {
    struct {
      Uword16 adc;
      Uword16 top;
      Uword16 bottom;
    } x;
    struct {
      Uword16 adc;
      Uword16 right;
      Uword16 left;
    } y;
  } chamber[4];
  struct {
    Uword16 adc;
    Uword16 tdc;
  } leadglas[4];
};

struct ohips_data {
  short noLAM;
  struct {
    struct {
      Uword16 right;
      Uword16 left;
    } adc;
    struct {
      Uword16 right;
      Uword16 left;
    } tdc;
    Uword16 meantime;
  } scint[4];
  struct {
    Uword16 adc;
    Uword16 tdc;
  } cerenkov[4];
  struct {
    Uword16 cerenkov;
    Uword16 leadglas;
  } sum;
  struct {
    Uword16 dcos;
    Uword16 u1[256];
    Uword16 v1[256];
    Uword16 u2[256];
    Uword16 v2[256];
  } chamber;
  struct {
    Uword16 adc;
    Uword16 tdc;
  } leadglas[19]; /* For runs later than May 2000, went from 15 to 19 here */
  struct {        /* and added scint1[5] */
    struct {
      Uword16 right;
      Uword16 left;
    } adc;
    struct {
      Uword16 right;
      Uword16 left;
    } tdc;
  } scint1[5];
};

struct event8 {
  Uword16 latch1;
  Uword16 flag;
  struct oops_data oopsA;
  struct oops_data oopsB;
  struct oops_data oopsC;
  struct oops_data oopsD;
  struct ohips_data ohips;
  struct {
    short noLAM;
    Uword16 tdc[9];
  } coincidence;
  struct {
    Uword16 helicity;
    Uword16 latch2;
    Uword16 notUsed1;
    Uword16 notUsed2;
  } misc;
  struct {
    struct {
      Uword16 adc;
      Uword16 tdc;
    } block[12];
    struct {
      Uword16 adc;
      Uword16 tdc;
    } veto[4];
    Uword16 latch;
  } leadglas;
  struct {
    Uword16 BPM[6];
    Uword32 BeamTAG;
    Uword16 BPM2[4];
  } beam;
  Uword16 latch2;
  struct {
    short noLAM;
    Uword16 oopsA;
    Uword16 oopsB;
    Uword16 oopsC;
    Uword16 oopsD;
    Uword16 ohips;
    Uword16 coin;
    Uword16 sync; 
    Uword16 CB;
  } LATCHtdc;
  Uword16 BeamTAGshort;
};

struct event10 {
  Uword16 flag;
  Uword16 helicity;
  Uword16 timeslot;
  struct {
    Uword16 bt1;
    Uword16 bt2;
  } toroid;
  struct {
    Uword16 molx;
    Uword16 moly;
    Uword16 tgtx;
    Uword16 tgty;
  } position;
  struct {
    Uword16 halo1;
    Uword16 halo2;
  } halo;
  struct {
    Uword32 oapre;
    Uword32 obpre;
    Uword32 ocpre;
    Uword32 hpre;
    Uword32 opreor;
    Uword32 coin10;
    Uword32 oliv;
    Uword32 hliv;
    Uword32 otrig;
    Uword32 htrig;
    Uword32 mtrig;
  } bit24;
  struct {
    Uword16 scaler12;
    Uword16 scaler13;
    Uword16 scaler14;
    Uword16 scaler15;
  } reserved;
};

struct event13 {
  struct {
    Uword16 month;
    Uword16 day;
    Uword16 hour;
    Uword16 minute;
    Uword16 second;
  } date;
  struct {
    float pressure;
    float cellTemp;
    float exchTemp;
  } top, bottom;
};

struct moller5 {
  Uword16 helicity;
  Uword16 timeslot;
  Uword32 qshunt;
  struct {
    Uword32 cerenkov1;
    Uword32 cerenkov2;
    Uword32 early;
    Uword32 ontime;
    Uword32 late;
    Uword32 spare;
  } scaler;
  Uword32 spare;
  Uword32 clock;
  Uword16 flag;
  Uword16 intCer1;
  Uword16 intCer2;
  Uword16 intBT1;
  Uword16 intBT2;
  struct {
    Uword16 molx;
    Uword16 moly;
    Uword16 tgtx;
    Uword16 tgty;
  } position;
  struct {
    Uword16 moller;
    Uword16 target;
  } halo;
  Uword16 tdc[9];
  Uword32 event5;
  Uword32 prescaled5;
};

struct moller16 {
  Uword32 qshunt;
  Uword32 intvdt;
  Uword32 current;
  Uword32 scaler[21];
};

struct oops1 {
  struct event3  clear;
  struct event5  old_scaler;
  struct event8  det;
  struct event10 beam;
  struct event13 target;
  Uword32 event[32];
  struct {
    struct moller5 coinc;
    struct moller16 quad;
  } moller;
  struct event6  scaler;
};

extern struct oops1 oops1;

#endif /* __OOPS_1_H__ */
