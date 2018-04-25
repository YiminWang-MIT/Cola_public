/* Changes:

   11.1.94: A. Richter:
            4434 Scaler entries changed to long format
            unfortunately I had to split the pm_hits because left,right are
	    defined as shorts.

   6.12.93: A. Richter
            Cerenkov B and Top B added
	    struct CerenkovB cerenkov;
            short speca_coinc;       deleted
            short ps_speca_coinc;    deleted

*/


#ifndef _DETECTOR_H_
#define _DETECTOR_H_

/*
 * VDCs
 */

#define AX1SIZE		400
#define AS1SIZE		320
#define AX2SIZE		416
#define AS2SIZE		336

#define BX1SIZE		368
#define BS1SIZE		336
#define BX2SIZE		400
#define BS2SIZE		368

struct VdcA {
  short	x1[AX1SIZE];
  short	s1[AS1SIZE];
  short	x2[AX2SIZE];
  short	s2[AS2SIZE];
};

struct VdcB {
  short	x1[BX1SIZE];
  short	s1[BS1SIZE];
  short	x2[BX2SIZE];
  short	s2[BS2SIZE];
};

/*
 * Cerenkov
 */

struct CerenkovA {
#define CMIRRORSA 12
  short energy[CMIRRORSA];
  long  pmhits[CMIRRORSA];		/* indiv. PMs dark rate         */
  short ch_ToF_time;
  short ps_hits;
};

struct CerenkovB {
#define CMIRRORSB 5 
  short energy[CMIRRORSB];
  long  pmhits[CMIRRORSB];		/* indiv. PMs dark rate         */
  short ch_ToF_time;
  short pattern;
  short ps_hits;
};

/*
 * Scintilators
 */

struct paddle {
  short	left;
  short	right;
};

struct full_paddle {
  struct paddle	energy;
  long hits;				/* right-left coinc */
};


#define APADDELS 15
#define BPADDELS 14

struct ScintA {
  struct full_paddle pad[APADDELS]; 
  long pmhits_r[APADDELS];            /* unfortunately we have to split that */
  long pmhits_l[APADDELS];
  short lr_time;			/* left/right time difference  */
};

struct TopScint {
  short energy;			/* ADC: sum of three PMs */
  short ToF_top_time;		/* TDC: ToF / 3rd paddle time difference */
  short hits;			/* # of pulses in the 3rd paddle */
  short ps_hits;
};

/*
 *  Trigger detectors
 */

struct TriggerA {
  short info;			/* laser events and more  (Trigger-Info)  */
  struct CerenkovA cerenkov;
  struct ScintA dE, ToF;
  struct TopScint top;
  short dEr_ToF_time;		/* dE right / ToF right time difference   */
  short dEl_ToF_time;		/* dE left / ToF right time difference    */
  short dE_ToF_coinc;		/* prescaled layer coinc. (1 k)           */
}; 

struct TriggerB {
  short info;			/* laser events and more  (Trigger-Info)  */
  struct CerenkovB cerenkov;
  struct TopScint top;
  struct full_paddle ToF[BPADDELS]; 
  short dE_energy[BPADDELS];
  long  ToF_pmhits_r[BPADDELS]; /* unfortunately we have to split that */
  long  ToF_pmhits_l[BPADDELS];
  long  dE_pmhits[BPADDELS];
  short dE_ToF_coinc;		/* prescaled layer coinc. (1 k)           */
  short dE_pattern;
  short ToF_pattern;
}; 

#endif











