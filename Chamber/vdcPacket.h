//								      -*-c++-*-
// Created 1993 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Chamber/vdcPacket.h,v 2.10 1999-11-19 12:09:22 friedric Exp $
//
// vdcPacket definitions
//

#ifndef __vdcPacket__
#define __vdcPacket__

const double L		= 12.0;		// distance between wires and kathode
const double DIST_X_1_2	= 272.0;	// distance between two x-planes
const double L_2	= 2 * L;	// distance betw. X-S-plane

inline double square(double x) { return x*x; }
inline double  cubic(double x) { return x*x*x; }

#endif














