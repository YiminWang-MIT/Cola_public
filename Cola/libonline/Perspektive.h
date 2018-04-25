//
// Created 1994 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Cola/libonline/Perspektive.h,v 2.1 2002-03-05 03:49:49 distler Exp $
//

#ifndef __Perspektive_h__
#define __Perspektive_h__

class Perspektive {
  double aInv;			// 1 / Abstand Target <-> Kolli
  double tan_th, tan_ph;	// Tangens der Kollioeffnungswinkel
public:
  Perspektive(double a, double th, double ph)
    { aInv = 1.0/a; tan_th = tan(th); tan_ph = tan(ph); }
  double thMin(double x) { return -atan(tan_th + aInv * x); }
  double thMax(double x) { return  atan(tan_th - aInv * x); }
  double phMin(double x) { return -atan(tan_ph + aInv * x); }
  double phMax(double x) { return  atan(tan_ph - aInv * x); }
};

#endif
