/*
 *    12.5.95:  New structure for the full 3 spectrometer setup
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

#define CX1SIZE		448
#define CS1SIZE		352
#define CX2SIZE		448
#define CS2SIZE		352

struct MpsVdcA {
  short	x1[AX1SIZE];
  short	s1[AS1SIZE];
  short	x2[AX2SIZE];
  short	s2[AS2SIZE];
  short status;
};

struct MpsVdcB {
  short	x1[BX1SIZE];
  short	s1[BS1SIZE];
  short	x2[BX2SIZE];
  short	s2[BS2SIZE];
  short status;
};

struct MpsVdcC {
  short x1[CX1SIZE];
  short s1[CS1SIZE];
  short x2[CX2SIZE];
  short s2[CS2SIZE];
  short status;
};


/*
 *   Trigger (Cerenkov & Scintillators)
 */

#define ACPADDLES  15
#define BPADDLES   14
#define ACMIRRORS  12
#define BMIRRORS    5

struct MpsPm {
    short energy;
    long  hits;
    short time;
};


/*
 * Cerenkov
 */

struct MpsCerenkovAC {
    struct MpsPm  mirror[ACMIRRORS];
    short         ps_hits;
};

struct MpsCerenkovB {
    struct MpsPm  mirror[BMIRRORS];
    short         ps_hits;
    short         pattern;
};


/*
 * Scintillators
 */

struct MpsPaddle {
    struct MpsPm  right, left;
    long          hits;
};

struct MpsPlaneAC {
    struct MpsPaddle  pad[ACPADDLES];
};

struct MpsScintAC {
    struct MpsPlaneAC tof, de;
    short             de_tof_time;
    short             ps_hits;        /* prescaled scintillator hits       */
};

struct MpsPlaneB {
    struct MpsPaddle  pad[BPADDLES];
    short             pattern;
};

struct MpsPlaneBDe {
    struct MpsPm      pad[BPADDLES];
    short             pattern;
};

struct MpsScintB {
    struct MpsPlaneB    tof;
    struct MpsPlaneBDe  de;
    short               de_tof_time;
    short               ps_hits;        /* prescaled scintillator hits       */
};


struct MpsTopScintABC {
    short energy;		 /*  ADC: sum of three PMs             */
    short ps_hits;
};


struct MpsTriggerAC {
    struct MpsTopScintABC  top;
    struct MpsCerenkovAC   cerenkov;
    struct MpsScintAC      scint;
    short                  scint_top_time;
    short                  scint_ch_time;
    short                  info;
    unsigned short         adc[96];
}; 

struct MpsTriggerB {
    struct MpsTopScintABC  top;
    struct MpsCerenkovB    cerenkov;
    struct MpsScintB       scint;
    short                  scint_top_time;
    short                  scint_ch_time;
    short                  info;
}; 

struct MpsHelGated {
	short	foerster;
	short	freqgen;
};

struct MpsHdcPlane {
	short	time;
	short	oddeven;
};

#define HDCSIZE		103

struct MpsHdc {
	struct MpsHdcPlane	u1[HDCSIZE];
	struct MpsHdcPlane	v1[HDCSIZE];
	struct MpsHdcPlane	u2[HDCSIZE];
	struct MpsHdcPlane	v2[HDCSIZE];
	short  status;
	struct { short u1, v1, u2, v2; } hits;  
};

#endif
