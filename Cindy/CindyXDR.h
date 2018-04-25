//								      -*-c++-*-
// Copyright (c) 1993-95 by	Michael O. Distler,
//				Institut fuer Kernphysik,
//				Johannes Gutenberg-Universitaet Mainz
//
// All rights reserved.
//
// $Header: /tmp/cvsroot/Cola/Cindy/CindyXDR.h,v 4.6 1997-07-09 20:39:45 distler Exp $
//

#ifndef __CindyXDR__
#define __CindyXDR__

#include "arch.h"

#ifdef WORDS_BIGENDIAN
inline void
XDRcpy(int *i, char *c)
{
  char *I = (char *)i;

  I[0] = c[0];
  I[1] = c[1];
  I[2] = c[2];
  I[3] = c[3];
}

inline void
XDRcpy(long *l, char *c)
{
  char *L = (char *)l;

  L[0] = c[0];
  L[1] = c[1];
  L[2] = c[2];
  L[3] = c[3];
}

inline void
XDRcpy(int *i, unsigned short *s)
{
  unsigned short *I = (unsigned short *)i;

  I[0] = s[0];
  I[1] = s[1];
}

inline void
XDRcpy(unsigned int *ui, unsigned short *s)
{
  unsigned short *UI = (unsigned short *)ui;

  UI[0] = s[0];
  UI[1] = s[1];
}

inline void
XDRcpy(long *l, unsigned short *s)
{
  unsigned short *L = (unsigned short *)l;

  L[0] = s[0];
  L[1] = s[1];
}

inline void
XDRcpy(unsigned long *ul, unsigned short *s)
{
  unsigned short *UL = (unsigned short *)ul;

  UL[0] = s[0];
  UL[1] = s[1];
}

inline void
XDRcpy(float *f, unsigned short *s)
{
  unsigned short *F = (unsigned short *)f;

  F[0] = s[0];
  F[1] = s[1];
}

inline void
XDRcpy(double *d, unsigned short *s)
{
  unsigned short *D = (unsigned short *)d;

  D[0] = s[0];
  D[1] = s[1];
  D[2] = s[2];
  D[3] = s[3];
}

inline void
XDRcpy(short *s, int *i)
{
  short *I = (short *)i;

  s[0] = I[0];
  s[1] = I[1];
}

inline void
XDRcpy(short *s, unsigned int *ui)
{
  short *UI = (short *)ui;

  s[0] = UI[0];
  s[1] = UI[1];
}

inline void
XDRcpy(short *s, long *l)
{
  short *L = (short *)l;

  s[0] = L[0];
  s[1] = L[1];
}

inline void
XDRcpy(short *s, unsigned long *ul)
{
  short *UL = (short *)ul;

  s[0] = UL[0];
  s[1] = UL[1];
}

inline void
XDRcpy(short *s, float *f)
{
  short *F = (short *)f;

  s[0] = F[0];
  s[1] = F[1];
}

inline void
XDRcpy(short *s, double *d)
{
  short *D = (short *)d;

  s[0] = D[0];
  s[1] = D[1];
  s[2] = D[2];
  s[3] = D[3];
}
#else
void XDRcpy(int *i, char *c);
void XDRcpy(long *i, char *c);
void XDRcpy(int *i, unsigned short *s);
void XDRcpy(unsigned int *ui, unsigned short *s);
void XDRcpy(long *l, unsigned short *s);
void XDRcpy(unsigned long *ul, unsigned short *s);
void XDRcpy(float *f, unsigned short *s);
void XDRcpy(double *d, unsigned short *s);
void XDRcpy(short *s, int *i);
void XDRcpy(short *s, unsigned int *ui);
void XDRcpy(short *s, long *l);
void XDRcpy(short *s, unsigned long *ul);
void XDRcpy(short *s, float *f);
void XDRcpy(short *s, double *d);
#endif

#endif
