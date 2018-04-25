//								      -*-c++-*-
// Copyright (c) 2013
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id$
//

#include "BicubicInterpolation.h"
#include <cmath>
#include <cstring>
#include <iostream>
#ifdef HAVE_CONFIG_H
#include "config.h"
#ifdef HAVE_GSL
#include <gsl/gsl_linalg.h>
#endif /* HAVE_GSL */
#endif /* HAVE_CONFIG_H */

using namespace std;

int BicubicPoint::fill(double *ptr)
{
  int i, valid = v[0]*v[1]*v[2]*v[3];
  if (valid)
    for (i=0;i<4;i++) ptr[i] = p[i];

  return valid;
}

BicubicSquare::BicubicSquare(BicubicPoint *f00, BicubicPoint *f10,
			     BicubicPoint *f01, BicubicPoint *f11)
{
  double f[16] = {0};

  // { f(0,0), fx(0,0), fy(0,0), fxy(0,0),
  //   f(1,0), fx(1,0), fy(1,0), fxy(1,0),
  //   f(0,1), fx(0,1), fy(0,1), fxy(0,1),
  //   f(1,1), fx(1,1), fy(1,1), fxy(1,1) }
  f00->fill(f);
  f10->fill(f+4);
  f01->fill(f+8);
  f11->fill(f+12);

  // http://en.wikipedia.org/wiki/Bicubic_interpolation
  // using a different order for vectors alpha and x
  a[0][0] = f[0];
  a[0][1] = f[2];
  a[0][2] = -3*f[0]-2*f[2]+3*f[8]-f[10];
  a[0][3] = 2*f[0]+f[2]-2*f[8]+f[10];
  a[1][0] = f[1];
  a[1][1] = f[3];
  a[1][2] = -3*f[1]-2*f[3]+3*f[9]-f[11];
  a[1][3] = 2*f[1]+f[3]-2*f[9]+f[11];
  a[2][0] = -3*f[0]-2*f[1]+3*f[4]-f[5];
  a[2][1] = -3*f[2]-2*f[3]+3*f[6]-f[7];
  a[2][2] = 9*f[0]+6*f[1]+6*f[2]+4*f[3]-9*f[4]+3*f[5]-6*f[6]+2*f[7]
    -9*f[8]-6*f[9]+3*f[10]+2*f[11]+9*f[12]-3*f[13]-3*f[14]+f[15];
  a[2][3] = -6*f[0]-4*f[1]-3*f[2]-2*f[3]+6*f[4]-2*f[5]+3*f[6]-f[7]
    +6*f[8]+4*f[9]-3*f[10]-2*f[11]-6*f[12]+2*f[13]+3*f[14]-f[15];
  a[3][0] = 2*f[0]+f[1]-2*f[4]+f[5];
  a[3][1] = 2*f[2]+f[3]-2*f[6]+f[7];
  a[3][2] = -6*f[0]-3*f[1]-4*f[2]-2*f[3]+6*f[4]-3*f[5]+4*f[6]-2*f[7]
    +6*f[8]+3*f[9]-2*f[10]-f[11]-6*f[12]+3*f[13]+2*f[14]-f[15];
  a[3][3] = 4*f[0]+2*f[1]+2*f[2]+f[3]-4*f[4]+2*f[5]-2*f[6]+f[7]
    -4*f[8]-2*f[9]+2*f[10]+f[11]+4*f[12]-2*f[13]-2*f[14]+f[15];
}


double BicubicSquare::operator()(double x, double y)
{
  int i, j;
  double result = 0.0;

  for (i=0; i<4; i++)
    for (j=0; j<4; j++)
      result += a[i][j] * pow(x,i) * pow(y,j);

  return result;
}

BicubicSpline::BicubicSpline(int n_points)
{
  n = n_points;
  A_data = new double[16*(n-1)*(n-1)];
  x_data = new double[4*(n-1)];

  memset(A_data, 0, 16*(n-1)*(n-1)*sizeof(double));
  memset(x_data, 0, 4*(n-1)*sizeof(double));

  int i = 0, j = 4*(n-1), k = 0;
  while (i < 3*(n-2)) {
    A_data[i*j+k]   =  1; /* continuity */
    A_data[i*j+k+1] =  1;
    A_data[i*j+k+2] =  1;
    A_data[i*j+k+3] =  1;
    A_data[i*j+k+4] = -1;
    i++;
    A_data[i*j+k+1] =  1; /* continuity of 1st derivative */
    A_data[i*j+k+2] =  2;
    A_data[i*j+k+3] =  3;
    A_data[i*j+k+5] = -1;
    i++;
    A_data[i*j+k+2] =  2; /* continuity of 2nd derivative */
    A_data[i*j+k+3] =  6;
    A_data[i*j+k+6] = -2;
    i++;
    k+=4;
  }
  A_data[2+j*i++] = 2;
  A_data[++i*j-2] = 2;
  A_data[i*j-1] = 6;
  k = 0;
  while (i < 4*n-5) {
    A_data[k+j*i++] = 1;
    k += 4;
  }
  i++;
  A_data[i*j-1] = A_data[i*j-2] = A_data[i*j-3] = A_data[i*j-4] = 1;
}

BicubicSpline::~BicubicSpline()
{
  delete[] A_data;
  delete[] x_data;
}

int BicubicSpline::init(double *b_data)
{
  int i;
  for (i=0;i<n;i++) x_data[3*n-4+i] = b_data[i];

#ifdef HAVE_GSL
  gsl_matrix_view A = gsl_matrix_view_array(A_data, 4*(n-1), 4*(n-1));
  gsl_vector_view b = gsl_vector_view_array(x_data, 4*(n-1));
  gsl_vector *x = gsl_vector_alloc(4*(n-1));

  int s;
  gsl_permutation * p = gsl_permutation_alloc(4*(n-1));
  gsl_linalg_LU_decomp(&A.matrix, p, &s);
  gsl_linalg_LU_solve(&A.matrix, p, &b.vector, x);
  for (i=0;i<4*(n-1);i++) x_data[i] = x->data[i];
  
  gsl_permutation_free(p);
  gsl_vector_free(x);
#else /* HAVE_GSL */
#warning "GNU scientific library not found."
#warning "Some features using BicubicInterpolation will not work"
  cerr << "BicubicInterpolation.cc: GNU scientific library not found." << endl;
#endif /* HAVE_GSL */
  return 0;
}

int BicubicSpline::printA()
{
  int i, j;
  for (i=0; i<4*(n-1); i++) {
    for (j=0; j<4*(n-1); j++) {
      cout << A_data[4*(n-1)*i+j] << ' ';
    }
    cout << endl;
  }
  return 0;
}

double BicubicSpline::operator()(double x)
{
  int i = x;
  if (i<0) i = 0;
  if (i>=(n-2)) i = n-2;
  double *par = x_data+4*i;
  x -= i;
  return par[0]+x*(par[1]+x*(par[2]+x*par[3]));
}

double BicubicSpline::slope(double x)
{
  int i = x;
  if (i<0) i = 0;
  if (i>=(n-2)) i = n-2;
  double *par = x_data+4*i;
  x -= i;
  return par[1]+x*(2.0*par[2]+3.0*x*par[3]);
}

BicubicInterpolation::BicubicInterpolation(int m_, int n_)
{
  m = m_; n = n_;
  square = new BicubicSquare *[n*m];
  point = new BicubicPoint *[(n+1)*(m+1)];

  memset(square, 0, n*m*sizeof(BicubicSquare *));
  memset(point, 0, (n+1)*(m+1)*sizeof(BicubicPoint *));
}

BicubicInterpolation::~BicubicInterpolation()
{
  int i, j;
  for (i=0; i<m; i++)
    for (j=0; j<n; j++)
      delete square[i*n+j];
  for (i=0; i<(m+1); i++)
    for (j=0; j<(n+1); j++)
      delete point[i*(n+1)+j];
  delete[] square;
  delete[] point;
}

double BicubicInterpolation::operator()(double x, double y)
{
  int i, j;

  if (x<0) {
    i = 0;
  } else if ((m-1)<x) {
    i = m-1;
  } else i = floor(x);

  if (y<0) {
    j = 0;
  } else if ((n-1)<y) {
    j = n-1;
  } else j = floor(y);

  //  cout << i << ' ' << x << endl;
  //  cout << j << ' ' << y << endl;

  return (*square[i*n+j])(x-i,y-j);
}

int BicubicInterpolation::initSlope()
{
  int i, j;

  // init f_x
  double *data = new double[m+1];
  for (j=0; j<=n; j++) {
    BicubicSpline spline(m+1);
    for (i=0; i<=m; i++) data[i] = (*point[(n+1)*i+j])[0];
    spline.init(data);
    for (i=0; i<=m; i++) {
      // cout << 1.0*i << '\t' << spline(1.0*i) << '\t' << spline.slope(1.0*i) << endl;
      (*point[(n+1)*i+j])[1] = spline.slope(1.0*i);
    }
  }
  delete[] data;

  // init f_y
  data = new double[n+1];
  for (i=0; i<=m; i++) {
    BicubicSpline spline(n+1);
    for (j=0; j<=n; j++) data[j] = (*point[(n+1)*i+j])[0];
    spline.init(data);
    for (j=0; j<=n; j++) (*point[(n+1)*i+j])[2] = spline.slope(1.0*j);
  }

  // init f_xy
  for (i=0; i<=m; i++) {
    BicubicSpline spline(n+1);
    for (j=0; j<=n; j++) data[j] = (*point[(n+1)*i+j])[1];
    spline.init(data);
    for (j=0; j<=n; j++) (*point[(n+1)*i+j])[3] = spline.slope(1.0*j);
  }
  delete[] data;

  for (i=0; i<m; i++)
    for (j=0; j<n; j++)
      square[i*n+j] = new BicubicSquare(point[i*(n+1)+j], point[(i+1)*(n+1)+j],
					point[i*(n+1)+j+1], point[(i+1)*(n+1)+j+1]);

  return 0;
}
