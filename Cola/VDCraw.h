//								      -*-c++-*-
// Created 1995 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Cola/VDCraw.h,v 2.1 2002-03-05 03:49:05 distler Exp $
//
// initialize VDCs and make raw histograms
//

#ifndef __VDC_RAW_H__
#define __VDC_RAW_H__

#include "AquaTree.h"
#include "HMBook/hmbook.h"
#include "Chamber/chlib++.h"

struct Spectrometer;
struct SPECTROMETER;

class vdcHIST {
protected:
  AquaTree *out;
  VdcPlane *plane;
  struct vdc *onl;
  HIST *paddle[16];
  HIST *drift;
  HIST *wire;
  HIST *tagged;
  HIST *eff;
  HIST *cell;
  HIST *pos;
  HIST *bad;
  HIST *good;
  HIST *tvw;
  HIST *minDriftTime;
  HIST *mDT_Nwire;
  HIST *Nwire;
public:
  vdcHIST(AquaTree *atree, VdcPlane *vdcplane, struct vdc *onlptr);
  virtual ~vdcHIST();
  int handle(int npaddle);
};

const int maxWire = 512;

class vdcBase {
protected:
    static int vdc_op;
    AquaShortSubTree *subX1, *subS1, *subX2, *subS2;
    AquaTree *out;
    struct Spectrometer *onl;
    struct SPECTROMETER *rdb;
    VdcPacket *packet;
    vdcHIST *histX1, *histS1, *histX2, *histS2;
    char *prefix;
    u_int16_t *wiretime;
    int vdcSpecials();
public:
    vdcBase(AquaTree *atree, AquaTree *outTree, struct Spectrometer *onlPtr,
	    struct SPECTROMETER *rdbPtr, VdcPacket *vdc);
    virtual ~vdcBase();
    int disable(double *x1disabled, double *s1disabled,
		double *x2disabled, double *s2disabled, int max);
    VdcPlane *x1Plane() { return packet->x1Plane(); }
    VdcPlane *s1Plane() { return packet->s1Plane(); }
    VdcPlane *x2Plane() { return packet->x2Plane(); }
    VdcPlane *s2Plane() { return packet->s2Plane(); }
    VdcPacket *vdcPacket() { return packet; }
    void Reset() { packet->Reset(); }
    int handle();
    // return 1 if coordinates are useful
    int handle(double &x, double &th, double &y, double &ph);
};

class vdcSpecA : public vdcBase {
public:
    vdcSpecA(AquaTree *atree, AquaTree *outTree, const char *name,
	     struct Spectrometer *onlPtr, struct SPECTROMETER *rdbPtr,
	     int ax1size, int as1size, int ax2size, int as2size);
    virtual ~vdcSpecA() { ; }
};

class vdcSpecB : public vdcBase {
public:
    vdcSpecB(AquaTree *atree, AquaTree *outTree, const char *name,
	     struct Spectrometer *onlPtr, struct SPECTROMETER *rdbPtr,
	     int bx1size, int bs1size, int bx2size, int bs2size);
    virtual ~vdcSpecB() { ; }
};

class vdcSpecC : public vdcBase {
public:
    vdcSpecC(AquaTree *atree, AquaTree *outTree, const char *name,
	     struct Spectrometer *onlPtr, struct SPECTROMETER *rdbPtr,
	     int cx1size, int cs1size, int cx2size, int cs2size);
    virtual ~vdcSpecC() { ; }
};

#endif /* __VDC_RAW_H__ */
