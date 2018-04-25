//								      -*-c++-*-
// Created 1994-95 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Cindy/CindyCoinc.h,v 4.7 2002-12-12 12:24:41 distler Exp $
//

#ifndef __CindyCoinc_h__
#define __CindyCoinc_h__

#include <ctime>

struct increment {
    time_t time;
    float runtimeA;
    float foersterA;
    float photoA;
    float faradayA;
    float runtimeB;
    unsigned int flags;
};

const unsigned int INCFL0 = 0x0000;
const unsigned int INCFL1 = 0x0001;	// ignore first deadtime A
const unsigned int INCFL2 = 0x0002;	// ignore first deadtime B
const unsigned int INCFL3 = 0x0004;	// ignore first deadtime AB

const unsigned int INCFL123 = INCFL1 | INCFL2 | INCFL3;

const struct increment Increment[] =
{
    {         0, 1.913E-4, 1.313E-4, 1.013E-4, 0.713E-4,
		 1.625E-4, INCFL123 },
    { 787005311, 1.771E-4, 1.471E-4, 1.171E-4, 0.871E-4,
		 1.648E-4, INCFL0 },
    { 794080731, 1.771E-4, 1.471E-4, 1.171E-4, 0.871E-4,
		 1.648E-4, INCFL123 },
};

const int MAXINC = sizeof(Increment)/sizeof(struct increment);

inline const struct increment *
getIncrement(time_t start)
{
  for (int i=0;i<MAXINC;i++)
    if (start<Increment[i].time) return(&Increment[i-1]);

  return(&Increment[MAXINC-1]);
}

#endif /* __CindyCoinc_h__ */
