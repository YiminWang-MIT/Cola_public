//								      -*-c++-*-
// Created 1994 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Cola/libonline/setup.h,v 2.1 2002-03-05 03:49:49 distler Exp $
//
// Setup Parameter (Strahlenergie, Spektrometer Winkel ...)
//

#ifndef __Setup_h__
#define __Setup_h__

#include <sys/types.h>
#include "Physics.h"

const double XA_OFFSET		= 37.4;
const double YA_OFFSET		= 1.2;
const double THETAA_OFFSET	= 785.0;	// = 787.3;
const double PHIA_OFFSET	= 4.2;

const double XB_OFFSET		= 21.0;
const double YB_OFFSET		= 1.0;
const double THETAB_OFFSET	= 839.2;	// = 840.9;
const double PHIB_OFFSET	= -1.2;

const double XC_OFFSET		= 11.57;
const double YC_OFFSET		= 0.011;
const double THETAC_OFFSET	= 773.56;
const double PHIC_OFFSET	= 13.6;

const double ANGLE_A_OFFSET	= -0.00242;
const double ANGLE_B_OFFSET	=  0.0;

const double TARGET_Z_OFFSET	=  -2.5;

inline double
codeAtoGrad(int code)
{ return (13382 - code) / 78.65; }

inline double
codeAtoRad(int code)
{ return codeAtoGrad(code) / CONST_180_PI; }

inline double
codeBtoGrad(int code)
{ return (code - 13451) / 78.62; }

inline double
codeBtoRad(int code)
{ return codeBtoGrad(code) / CONST_180_PI; }

inline double
codeCtoGrad(int code)
{ return (code - 13349) / 78.66; }

inline double
codeCtoRad(int code)
{ return codeCtoGrad(code) / CONST_180_PI; }

#endif
