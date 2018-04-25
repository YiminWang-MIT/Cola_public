//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 2003
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: BaselNdet.h 2216 2008-06-13 21:13:47Z distler $
//

#ifndef __BASEL_NDET_H__
#define __BASEL_NDET_H__

#include "detectorBase.h"

class BaselBase : public detectorBase {
protected:
  BaselBase *parent;
  int flags;
public:
  BaselBase(BaselBase *father, AquaTree *aquatree, AquaTree *outtree,
	    int baselflags) : detectorBase(aquatree, outtree) {
    parent = father; flags = baselflags;
  }
  virtual ~BaselBase() { ; }
  virtual int handle() = 0;
  virtual int init(int typ, const double *data, int size) = 0;
  virtual int print() = 0;
};

class BaselBar : public BaselBase {
protected:
public:
  BaselBar(BaselBase *father, AquaTree *aquatree,
	   int baselflags, const char *dataPrefix);
  virtual ~BaselBar();
  virtual int handle();
  virtual int init(int typ, const double *data, int size);
  virtual int print();
};


class BaselDetector : public BaselBase {
protected:
  BaselBar *bar[5][6];
  BaselBar *dE[10];
public:
  BaselDetector(BaselBase *father, AquaTree *aquatree, AquaTree *outtree,
		int baselflags, const char *dataPrefix, const char *outPrefix);
  virtual ~BaselDetector();
  virtual int handle();
  virtual int init(int typ, const double *data, int size);
  virtual int print();
};


class BaselNdet : public BaselBase {
public:
  static char *dataPrefix;
  static char *outPrefix;
protected:
  BaselDetector *det[2]; // 0:old detector, 1:new detector
public:
  BaselNdet(AquaTree *aquatree, AquaTree *outtree, int baselflags);
  virtual ~BaselNdet();
  virtual int handle();
  virtual int init(int typ, const double *data, int size);
  virtual int print();
};

#endif /* __BASEL_NDET_H__ */
