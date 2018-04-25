//								      -*-c++-*-
// AQUA: data AcQUisition for A1 experiments
//
// Copyright (c) 2001
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: pdcD.h 2216 2008-06-13 21:13:47Z distler $
//

#ifndef __PDCD_H__
#define __PDCD_H__

#include <cmath>
#include "Aqua/AquaTree.h"
#include "HMBook/hmbook.h"
#include "HMBook/hmplot.h"
#include "Cola/rundatabase.h"
#include "fitspline.h"

class VdcRegression;

const int Xcells = 8;
const int Ycells = 2;
const int cellWires = 8;   /*   nr of wires per cell   */
const int minX = (getenv("minX") ? atoi(getenv("minX")) : 1000);
const int maxX = (getenv("maxX") ? atoi(getenv("maxX")) : 2850);
const int minY = (getenv("minY") ? atoi(getenv("minY")) : 1000);
const int maxY = (getenv("maxY") ? atoi(getenv("maxY")) : 2800);

const int    TDC_CHANNELS   = 4096;     // for drift-path tables
const double CELL_LENGTH    = 18.4;     // mm (max. drift path)
const double STAGGERING     = 0.1;	// mm
const double Z_DISTANCE     = 5.08;	// mm
const int    CALIBTABSIZE   = NDAT;      // entries in Tab. for spline opti.

#define PDCD_HISTO_FLAG 0x0001
#define PDCD_DEBUG_FLAG 0x0000 // 0x0001

const int DDR_CORRECTION_X = (getenv("DDR_CORRECTION_X") 
				 ? atoi(getenv("DDR_CORRECTION_X")) : 0);
const int DDR_CORRECTION_Y = (getenv("DDR_CORRECTION_Y") 
				 ? atoi(getenv("DDR_CORRECTION_Y")) : 0);

struct wiretime {
  u_int16_t wire;
  u_int16_t time;
};

struct timepathlength {
  u_int16_t time;
  float length;
};

typedef int (* TimeToLenFun)(u_int16_t);

class pdcDbase {
protected:
  pdcDbase *parent;
  AquaTree *tree;
  AquaTree *onl;
  int flags;
public:
  pdcDbase(pdcDbase *father, AquaTree *atree, AquaTree *otree, int pdcflags) {
    parent = father; tree = atree; onl = otree; flags = pdcflags;
  }
  virtual ~pdcDbase() { ; }
  virtual int handle() = 0;
  virtual int Reset() = 0;
};

class pdcDcell : public pdcDbase {
protected:
  double dpath[cellWires];
public:
  pdcDcell(pdcDbase *father, int pdcflags)
    : pdcDbase(father, NULL, NULL, pdcflags) { ; }
  virtual ~pdcDcell() { ; }
  virtual int handle();
  virtual int Reset();
  int fill(u_int16_t w, u_int16_t t, double l);
};

const int pdcDonlineLen = 16;
class pdcDplane : public pdcDbase {
protected:
  AquaShortSubTree *subTree;
  struct wiretime *data;
  double *length;
  pdcDcell **cell;
  int maxcells;
  HIST *timeH, *wireH, *tagH, *effH, *lenH, *resH, *staggH;
  AquaNodeInt *raw_time[pdcDonlineLen], *raw_wire[pdcDonlineLen];
  AquaNodeInt *multi, *error, *pattern;
  AquaNodeDouble *x, *sl, *chi2, *raw_len[pdcDonlineLen];
  //double splineNode[SAMPLES]; // sample points for spline ("Stuetzstellen")
  char *sno_in;
public:
  double a, b, chisqr, l12, l22;
  int n, time;
  int tmin, tmax, ndata, nr_of_iterations;

  double *stuetze;
  double *varParam;
  char *sno;
  struct timepathlength *calibTab;

  pdcDplane(pdcDbase *father, int maxCells,
	    const char *aname, AquaTree *atree,
	    const char *oname, AquaTree *otree, int pdcflags);
  virtual ~pdcDplane();
  virtual int handle();
  virtual int Reset();
  struct wiretime *getWireTime(int &mult) {mult = *multi;return data;};
  double timeToLength(u_int16_t t);
  double *getLength(int &mult) {mult = *multi; return length;};
  double *getLength() {return length;};
  double CorrectDDR(int wire, double path);
  void InitSplines();
  void ReadSplines(char *filename);
  void FillCalibTab(u_int16_t t, double l, int n);
  void WriteCalibTab(int n);
  void WriteSplineNodes(double *sn, int iter, int xory);
};

class pdcD : public pdcDbase {
protected:
  pdcDplane *planeX, *planeY;
  AquaNodeInt *pdcOK;
  AquaNodeDouble *x, *y, *th, *ph, *dx, *dy, *dth, *dph;
public:
  pdcD(const char *aname, AquaTree *atree, 
       const char *oname, AquaTree *otree, int pdcflags);
  virtual ~pdcD();
  virtual int handle();
  virtual int Reset();
  pdcDplane *getPlaneX() {return planeX;};
  pdcDplane *getPlaneY() {return planeY;};
  void InitDDR();
  void PrintDDR(char *, int);
  void WriteDDR(char *);
  void WriteDDR(char *, int);
  void WriteDDR(int);
  void ReadDDR(char *);
};

#endif /* __PDCD_H__ */
