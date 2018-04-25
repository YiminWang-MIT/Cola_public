//								      -*-c++-*-
// Created 1993 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Chamber/vdcRegression.h,v 2.13 1999-11-19 12:09:22 friedric Exp $
//
// VdcRegression definitions
//

#ifndef __vdcRegression__
#define __vdcRegression__

#include <math.h>
#include <float.h>

#ifndef M_PI
#define M_PI        3.14159265358979323846	/* pi */
#endif
#ifndef M_PI_2
#define M_PI_2      1.57079632679489661923	/* pi/2 */
#endif
#ifndef M_SQRT1_2
#define M_SQRT1_2   0.70710678118654752440	/* 1/sqrt(2) */
#endif

class VdcRegression {
  int    N;
  double sumN, sumX, sumY, sumXX, sumXY, sumYY;
  double x_, dx_, y_, dy_, sl_, dsl_, chi2;
public:
  VdcRegression(void);
  ~VdcRegression(void);
  void add(double x);
  void addw(double x, double w);
  void add(double x, double y);
  void addw(double x, double y, double w);
  void remove(double x);
  void removew(double x, double w);
  void remove(double x, double y);
  void removew(double x, double y, double w);
  void reset(void);
  void calc(void);
  void print(char *);
  double x0(void);
  double dx0(void);
  double y0(void);
  double dy0(void);
  double sl0(void);
  double dsl0(void);
  double chi(void);
  double chisqr(void);
  VdcRegression operator+(const VdcRegression &);
  VdcRegression operator+(const double);
};

#endif



















