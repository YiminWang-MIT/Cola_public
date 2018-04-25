//								      -*-c++-*-
// Created 1993 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Chamber/vdcPlane.h,v 2.11 1999-11-03 15:17:48 friedric Exp $
//
// vdcPlane definitions
//

#ifndef __vdcPlane__
#define __vdcPlane__

#include <float.h>

const short  CHANNELS_CARD  = 16;
const short  TDC_FULL_RANGE = 512;
const double TDC_RESOLUTION = 0.75;	// ns / bit
const double DRIFT_VELOCITY = 0.052;	// mm / ns
const double TIME_TO_LENGTH = TDC_RESOLUTION * DRIFT_VELOCITY;

const double S_DIST	    = 5.0;	// distance between signalwires / mm
const double C_DIST	    = 12.0;	// distance wires - cathode / mm

inline int
sign(double a)
{
    if (a > DBL_MIN) return 1;
    if (-a > DBL_MIN) return -1;

    return 0;
}

inline int
oppositSign(double a, double b)
{
    if (sign(a)*sign(b) < 0) return 1;

    return 0;
}

#endif










