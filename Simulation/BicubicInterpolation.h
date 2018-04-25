//								      -*-c++-*-
// Copyright (c) 2013
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id$
//

#ifndef __BICUBIC_INTERPOLATION__
#define __BICUBIC_INTERPOLATION__

class BicubicPoint {
  double p[4];
  int v[4];
public:
  BicubicPoint(double f) {
    p[0] = f; v[0] = 1; v[1] = v[2] = v[3] = 0;
  }
  BicubicPoint(double f, double fx, double fy, double fxy) {
    p[0] = f; p[1] = fx; p[2] = fy; p[3] = fxy; v[0] = v[1] = v[2] = v[3] = 1;
  }
  double& operator[](int index) {
    v[index] = 1; return p[index];
  }
  double const& operator[](int index) const {
    return p[index];
  }
  int fill(double *ptr);
};

class BicubicSquare {
  double a[4][4];
public:
  BicubicSquare(BicubicPoint *f00,BicubicPoint *f10,BicubicPoint *f01,BicubicPoint *f11);
  double operator()(double x, double y);
};

class BicubicSpline {
  int n;          // number of data points, x coordinates are 0..n-1
  double *A_data; // matrix of the system of linear equations
  double *x_data; // spline coefficients a0, b0, c0, d0, .. ,d(n-1)
public:
  BicubicSpline(int n_points);
  ~BicubicSpline();
  int init(double *b_data);
  int printA(); // debug
  double operator[](int i) { return x_data[i]; }
  double operator()(double x);
  double slope(double x);
};

class BicubicInterpolation {
  int m, n;
  BicubicSquare **square;
  BicubicPoint **point;
public:
  BicubicInterpolation(int m_, int n_);
  ~BicubicInterpolation();
  double operator()(double x, double y);
  int init(int i, int j, double val) {
    BicubicPoint *pnt = point[(n+1)*i+j];
    if (pnt) {
      (*pnt)[0] = val;
    } else point[(n+1)*i+j] = new BicubicPoint(val);
    return 0;
  }
  int init(int i, int j, double val, double valx, double valy, double valxy) {
    BicubicPoint *pnt = point[(n+1)*i+j];
    if (pnt) {
      (*pnt)[0] = val;
      (*pnt)[1] = valx;
      (*pnt)[2] = valy;
      (*pnt)[3] = valxy;
    } else point[(n+1)*i+j] = new BicubicPoint(val, valx, valy, valxy);
    return 0;
  }
  int initSlope();
};

#endif /* __BICUBIC_INTERPOLATION__ */
