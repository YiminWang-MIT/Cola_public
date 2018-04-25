//								      -*-c++-*-
// Created 2014 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header$
//
// VdcLikelihood definitions
//

#ifndef __vdcLikelihood__
#define __vdcLikelihood__

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

class TrackVector {
protected:
  double v[2];
public:
  TrackVector(double x, double y) {
    v[0] = x; v[1] = y;
  }
  virtual ~TrackVector() { ; }
  inline double operator[] (int i) const { return v[i]; }
  inline TrackVector operator+(const TrackVector &b) {
    return TrackVector(v[0]+b[0], v[1]+b[1]);
  }
  inline TrackVector operator-(const TrackVector &b) {
    return TrackVector(v[0]-b[0], v[1]-b[1]);
  }
  inline TrackVector operator*(double f) {
    return TrackVector(v[0]*f, v[1]*f);
  }
};

class Hessian {
protected:
  int bad;
  double xx, xy, yy, det;
  void initbad() {
    // fake values for 99% correlation, dx=10, dy=0.1
    bad = 1; det=1.0/sqrt(1.0-0.99*0.99);
    xx = 0.01*det; yy = 100.0*det; xy = -sqrt(det*(det-1));
  }
public:
  Hessian() { initbad(); }
  Hessian(double xx_, double xy_, double yy_) {
    xx = xx_; xy = xy_; yy = yy_; det = xx*yy-xy*xy; bad = 0;
    if (xx<=0 || yy<=0 || det<=0) initbad();
  }
  virtual ~Hessian() { ; }
  int isbad() { return bad; }
  double errorX() { return sqrt(yy/det); }
  double errorY() { return sqrt(xx/det); }
  double correlation() { return -xy/sqrt(xx*yy); }
};

class VdcLikelihood {
  int    N;
  char *ident;
  double *coord, *path, *yoffs, *times;
  double x_, dx_, sl_, dsl_, chi2;
protected:
  Hessian NHessian(double x, double sl, double hx, double hsl);
  double likelihood(double x, double sl, int debug=0);
  int simplex(double &x, double &sl);
  int vsort(TrackVector &vl, TrackVector &vm, TrackVector &vh,
	    double &pl, double &pm, double &ph);
  double area(TrackVector vl, TrackVector vm, TrackVector vh);
  double reflect(double f, double &pl,
		 TrackVector &vl, TrackVector &vm, TrackVector &vh);
  int contract(TrackVector &vl, TrackVector &vm, TrackVector &vh,
	       double &pl, double &pm, double &ph);
  inline double derivativeX(double x, double sl, double h) {
    return 0.5/h*(likelihood(x+h,sl)-likelihood(x-h,sl));
  }
  inline double minimumX(double x, double sl, double h) {
    double fm = likelihood(x-h,sl);
    double f0 = likelihood(x,sl);
    double fp = likelihood(x+h,sl);
    return (0.5*h*(fm-fp))/(fm-2*f0+fp)+x;
  }
  inline double derivativeSL(double x, double sl, double h) {
    return 0.5/h*(likelihood(x,sl+h)-likelihood(x,sl-h));
  }
  inline double minimumSL(double x, double sl, double h) {
    double fm = likelihood(x,sl-h);
    double f0 = likelihood(x,sl);
    double fp = likelihood(x,sl+h);
    return (0.5*h*(fm-fp))/(fm-2*f0+fp)+sl;
  }
  inline double curvatureX(double x, double sl, double h) {
    return (-likelihood(x-2*h,sl)+16*likelihood(x-h,sl)-30*likelihood(x,sl)
	    +16*likelihood(x+h,sl)-likelihood(x+2*h,sl))/(12*h*h);
  }
  inline double curvatureSL(double x, double sl, double h) {
    return (-likelihood(x,sl-2*h)+16*likelihood(x,sl-h)-30*likelihood(x,sl)
	    +16*likelihood(x,sl+h)-likelihood(x,sl+2*h))/(12*h*h);
  }
  inline double derivativeXSL(double x, double sl, double h) {
    return (2*likelihood(x,sl)+likelihood(x+h,sl+h)+likelihood(x-h,sl-h)-likelihood(x+h,sl)-likelihood(x,sl+h)-likelihood(x-h,sl)-likelihood(x,sl-h))/(2*h*h);
  }
  double driftTimeCorrFun(double a);
  double mean(double sl);
public:
  VdcLikelihood(const char *id=0);
  VdcLikelihood(VdcLikelihood *a, VdcLikelihood *b,
		double d, const char *id=0);
  ~VdcLikelihood(void);
  void init(int n, float *x, float *y, float *t);
  void reset(void);
  void calc(double);
  void print(char *);
  double x0(void);
  double dx0(void);
  double sl0(void);
  double dsl0(void);
  double chi(void);
  double chisqr(void);
};

#endif



















