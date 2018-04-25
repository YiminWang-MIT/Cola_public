//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 2003
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: detectorBase.h 2216 2008-06-13 21:13:47Z distler $
//

#ifndef __DETECTOR_BASE_H__
#define __DETECTOR_BASE_H__

#include "Aqua/AquaTree.h"

class detectorBase {
protected:
  AquaTree *atree;
  AquaTree *online;
public:
  detectorBase(AquaTree *aquatree, AquaTree *outtree) {
    atree  = aquatree;
    online = outtree;
  }
  virtual ~detectorBase() { ; }
  virtual int handle() = 0;
};

#endif /* __DETECTOR_BASE_H__ */
