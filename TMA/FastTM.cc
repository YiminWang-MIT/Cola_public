//                                                                    -*-c++-*-
// the new guy to blame:
// 
// Jan C. Bernauer             mailto:bernauer@kph.uni-mainz.de
// A1 Collaboration 
// Nov. of 2003 
//
// the guy formerly known to be the guy to blame:
//
// Michael O. Distler                        mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// former version   Jan Friedrich 5/97 
//                  A1 Collaboration
// copyright        Institut fuer Kernphysik Mainz
//
// $Header: /tmp/cvsroot/Cola/TMA/FastTM.cc,v 1.13 2004-07-21 10:35:39 bernauer Exp $
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "FastTM.h"
#include "ftmInit.h"


struct TargetCo *
TMAC::run(double x_fp, double th_fp, double y_fp, double ph_fp)
{
  // VDC-unit: mm, Spec-unit: cm
  double coo[4], ini[4] = { 0.1*x_fp, th_fp, 0.1*y_fp, ph_fp };

  // apply offsets
  int i, j;
  for (i=0; i<4; i++) {
    coo[i] = 0.0;
    for (j=power[i]-1; j>0; j--) {
      coo[i] -= offs[i][j];
      coo[i] *= coo[0]; // coo[0] = x; make sure: power[0]=1
    }
    coo[i] += ini[i];
    if (power[i]) coo[i] -= offs[i][0];
  }

  // initialize evaluation
  ftmEntry *itmp = elements;
  for (; itmp; itmp=itmp->getNext()) itmp->initialize(coo);

  // dirty hack !!
  // I assume : struct TargetCo { float dp, th, y0, ph, len; };
  float *target = (float *)&result;
  double value;
  for (i=0; i<5; i++) {
    ftmEntry **etmp = node[i];
    value = 0.0;
    while (*etmp) value += (*etmp++)->evaluate(i);
    target[i] = value;
  }

  return &result;
}

TMAC::TMAC(const char *filename):baseTM(filename)
{
  // run parser and get entries and offsets
  ftmInit * init = new ftmInit(filename);
  int entries = init->getEntries(&elements);
  power[0] = init->getXoffset(&offs[0]);
  power[1] = init->getToffset(&offs[1]);
  power[2] = init->getYoffset(&offs[2]);
  power[3] = init->getPoffset(&offs[3]);
  delete init;

  // initialize nodes using only non-zero elements
  int i, j;
  for (i=0; i<5; i++) { // dp, th0, y0, ph0, len
    node[i] = new ftmEntry *[entries+1];
    ftmEntry *tmp; // scan all matrix elements
    for (tmp=elements,j=0; tmp; tmp=tmp->getNext())
      if ((*tmp)[i] != 0.0) (node[i])[j++] = tmp;
    (node[i])[j] = NULL;
  }
}

TMAC::~TMAC()
{
  int i;
  for (i=0; i<4; i++) delete offs[i];
  for (i=0; i<5; i++) delete node[i];
  delete elements;
}

int
TMAC::setOffsets(double x_offs, double th_offs, double y_offs, double ph_offs)
{
  delete offs[0]; offs[0] = new double[1]; *offs[0] =  x_offs; power[0] = 1;
  delete offs[1]; offs[1] = new double[1]; *offs[1] = th_offs; power[1] = 1;
  delete offs[2]; offs[2] = new double[1]; *offs[2] =  y_offs; power[2] = 1;
  delete offs[3]; offs[3] = new double[1]; *offs[3] = ph_offs; power[3] = 1;
  return 0;
}
