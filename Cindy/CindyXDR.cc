//
// Copyright (c) 1993-95 by	Michael O. Distler,
//				Institut fuer Kernphysik,
//				Johannes Gutenberg-Universitaet Mainz
//
// All rights reserved.
//
// $Header: /tmp/cvsroot/Cola/Cindy/CindyXDR.cc,v 4.6 1997-07-09 20:39:45 distler Exp $
//

static const char rcsid[] =
 "@(#)$Id: CindyXDR.cc 2216 2008-06-13 21:13:47Z distler $";

#include "CindyXDR.h"

#ifndef WORDS_BIGENDIAN

void
XDRcpy(int *i, char *c)
{
  char *I = (char *)i;

  I[0] = c[3];
  I[1] = c[2];
  I[2] = c[1];
  I[3] = c[0];
}

void
XDRcpy(long *i, char *c)
{
  char *I = (char *)i;

  I[0] = c[3];
  I[1] = c[2];
  I[2] = c[1];
  I[3] = c[0];
}

void
XDRcpy(int *i, unsigned short *s)
{
  char *I = (char *)i;
  char *S = (char *)s;

  I[0] = S[2];
  I[1] = S[3];
  I[2] = S[0];
  I[3] = S[1];
}

void
XDRcpy(unsigned int *ui, unsigned short *s)
{
  char *I = (char *)ui;
  char *S = (char *)s;

  I[0] = S[2];
  I[1] = S[3];
  I[2] = S[0];
  I[3] = S[1];
}

void
XDRcpy(long *l, unsigned short *s)
{
  char *L = (char *)l;
  char *S = (char *)s;

  L[0] = S[2];
  L[1] = S[3];
  L[2] = S[0];
  L[3] = S[1];
}

void
XDRcpy(unsigned long *ul, unsigned short *s)
{
  char *L = (char *)ul;
  char *S = (char *)s;

  L[0] = S[2];
  L[1] = S[3];
  L[2] = S[0];
  L[3] = S[1];
}

void
XDRcpy(float *f, unsigned short *s)
{
  char *F = (char *)f;
  char *S = (char *)s;

  F[0] = S[2];
  F[1] = S[3];
  F[2] = S[0];
  F[3] = S[1];
}

void
XDRcpy(double *d, unsigned short *s)
{
  char *D = (char *)d;
  char *S = (char *)s;

  D[0] = S[6];
  D[1] = S[7];
  D[2] = S[4];
  D[3] = S[5];
  D[4] = S[2];
  D[5] = S[3];
  D[6] = S[0];
  D[7] = S[1];
}

void
XDRcpy(short *s, int *i)
{
  char *S = (char *)s;
  char *I = (char *)i;

  S[0] = I[2];
  S[1] = I[3];
  S[2] = I[0];
  S[3] = I[1];
}

void
XDRcpy(short *s, unsigned int *ui)
{
  char *S = (char *)s;
  char *I = (char *)ui;

  S[0] = I[2];
  S[1] = I[3];
  S[2] = I[0];
  S[3] = I[1];
}

void
XDRcpy(short *s, long *l)
{
  char *S = (char *)s;
  char *L = (char *)l;

  S[0] = L[2];
  S[1] = L[3];
  S[2] = L[0];
  S[3] = L[1];
}

void
XDRcpy(short *s, unsigned long *ul)
{
  char *S = (char *)s;
  char *L = (char *)ul;

  S[0] = L[2];
  S[1] = L[3];
  S[2] = L[0];
  S[3] = L[1];
}

void
XDRcpy(short *s, float *f)
{
  char *S = (char *)s;
  char *F = (char *)f;

  S[0] = F[2];
  S[1] = F[3];
  S[2] = F[0];
  S[3] = F[1];
}

void
XDRcpy(short *s, double *d)
{
  char *S = (char *)s;
  char *D = (char *)d;

  S[0] = D[6];
  S[1] = D[7];
  S[2] = D[4];
  S[3] = D[5];
  S[4] = D[2];
  S[5] = D[3];
  S[6] = D[0];
  S[7] = D[1];
}

#endif
