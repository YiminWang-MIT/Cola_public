//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 2003
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: BaselNdet.cc 2216 2008-06-13 21:13:47Z distler $
//

#include "BaselNdet.h"
#include <cstdio>

BaselBar::BaselBar(BaselBase *father, AquaTree *aquatree,
		   int baselflags, const char *dataPrefix)
  : BaselBase(father, aquatree, NULL, baselflags)
{
  ;
}

BaselBar::~BaselBar()
{
  ;
}

int BaselBar::handle()
{
  return 0;
}

int BaselBar::init(int typ, const double *data, int size)
{
  return 0;
}

int BaselBar::print()
{
  return 0;
}

///////////////////////////////////////////////////////////////////////////////

BaselDetector::BaselDetector(BaselBase *father, AquaTree *aquatree,
			     AquaTree *outtree, int baselflags,
			     const char *dataPrefix, const char *outPrefix)
  : BaselBase(father, aquatree, outtree, baselflags)
{
  char aPrefix[32];
  //  char oPrefix[32];

  for (int i=0; i<=4; i++)
    for (int j=0; i<=5; i++) {
      if (!i || !j) { bar[i][j] = NULL; continue; }
      sprintf(aPrefix, "%s.E[%d]", dataPrefix, 5*(i-1)+(j-1));
      bar[i][j] = new BaselBar(this, atree, flags, aPrefix);
    }
  dE[0] = NULL;
  for (int i=1; i<=9; i++) {
    sprintf(aPrefix, "%s.dE[%d]", dataPrefix, i);
    dE[i] = new BaselBar(this, atree, flags, aPrefix);
  }
}

BaselDetector::~BaselDetector()
{
  for (int i=1; i<=4; i++)
    for (int j=1; i<=5; i++) delete bar[i][j];
  for (int i=1; i<=9; i++) delete dE[i];
}

int BaselDetector::handle()
{
  for (int i=1; i<=4; i++)
    for (int j=1; i<=5; i++) bar[i][j]->handle();
  for (int i=1; i<=9; i++) dE[i]->handle();

  return 0;
}

int BaselDetector::init(int typ, const double *data, int size)
{
  return 0;
}

int BaselDetector::print()
{
  for (int i=1; i<=4; i++)
    for (int j=1; i<=5; i++) bar[i][j]->print();
  for (int i=1; i<=9; i++) dE[i]->print();

  return 0;
}

///////////////////////////////////////////////////////////////////////////////

BaselNdet::BaselNdet(AquaTree *aquatree, AquaTree *outtree, int baselflags)
  : BaselBase(NULL, aquatree, outtree, baselflags)
{
  char aPrefix[32];
  char oPrefix[32];

  sprintf(aPrefix, "%s.det", dataPrefix); sprintf(oPrefix, "%s.N", outPrefix);
  det[0] = new BaselDetector(this, atree, online, flags, aPrefix, oPrefix);
  sprintf(aPrefix, "%s.det2",dataPrefix); sprintf(oPrefix, "%s.N2",outPrefix);
  det[1] = new BaselDetector(this, atree, online, flags, aPrefix, oPrefix);
}

BaselNdet::~BaselNdet()
{
  delete det[0];
  delete det[1];
}

int BaselNdet::handle()
{
  det[0]->handle();
  det[1]->handle();

  return 0;
}

int BaselNdet::init(int typ, const double *data, int size)
{
  return 0;
}

int BaselNdet::print()
{
  det[0]->print();
  det[1]->print();

  return 0;
}

///////////////////////////////////////////////////////////////////////////////
