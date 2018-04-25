//								      -*-c++-*-
// Copyright (c) 2013
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id$
//

#include <iostream>
#include <cmath>
#include "BicubicInterpolation.h"

// test of BicubicSpline class
// same result as with mathematica program 2013/02/12 M.Distler
#define SPLINE_TEST 1

int main(int argc, char *argv[])
{
  const int N = 5;
  int i, j;

#if defined(SPLINE_TEST)
  double pts[N];

  for (i=0; i<N; i++) pts[i] = sin(1.0*i);

  BicubicSpline spline(N);
  spline.printA();

  std::cout << std::endl;
  spline.init(pts);
  for (i=0; i<(N-1); i++)
    std::cout << spline[4*i] << '\t' << spline[4*i+1] << '\t'
	      << spline[4*i+2] << '\t' << spline[4*i+3] << std::endl;

  std::cout << std::endl;
  for (i=0; i<11; i++) {
    double x = 0.5*i;
    std::cout << x << '\t' << spline(x) << '\t' << spline.slope(x) << std::endl;
  }
#endif /* SPLINE_TEST */

  // Interpolation[Flatten[MapIndexed[{#2-{1,1},#1}&,
  //     Partition[Table[Prime[n],{n,30}],5,5],{2,2}],1]]

  BicubicInterpolation interpol(5,4);

  interpol.init(0, 0, 2.0);
  interpol.init(0, 1, 3.0);
  interpol.init(0, 2, 5.0);
  interpol.init(0, 3, 7.0);
  interpol.init(0, 4, 11.0);
  interpol.init(1, 0, 13.0);
  interpol.init(1, 1, 17.0);
  interpol.init(1, 2, 19.0);
  interpol.init(1, 3, 23.0);
  interpol.init(1, 4, 29.0);
  interpol.init(2, 0, 31.0);
  interpol.init(2, 1, 37.0);
  interpol.init(2, 2, 41.0);
  interpol.init(2, 3, 43.0);
  interpol.init(2, 4, 47.0);
  interpol.init(3, 0, 53.0);
  interpol.init(3, 1, 59.0);
  interpol.init(3, 2, 61.0);
  interpol.init(3, 3, 67.0);
  interpol.init(3, 4, 71.0);
  interpol.init(4, 0, 73.0);
  interpol.init(4, 1, 79.0);
  interpol.init(4, 2, 83.0);
  interpol.init(4, 3, 89.0);
  interpol.init(4, 4, 97.0);
  interpol.init(5, 0, 101.0);
  interpol.init(5, 1, 103.0);
  interpol.init(5, 2, 107.0);
  interpol.init(5, 3, 109.0);
  interpol.init(5, 4, 113.0);

  interpol.initSlope();

  std::cout << interpol(2, 0) << std::endl;
  std::cout << interpol(0, 2) << std::endl;
  std::cout << interpol(1.5, 2.3) << std::endl;
  std::cout << interpol(2.5, 3.3) << std::endl;

#ifdef NEVER
  BicubicPoint **point;
  BicubicPoint *pnt;
  point = new BicubicPoint*[10];
  std::cout << ((long)point) << std::endl << std::endl;
  for (i=0; i<10; i++) {
    pnt = point[i];
    point[i] = new BicubicPoint(i, 10+i, 20+i, 30+i);
    std::cout << i << ' ' << ((long)pnt) << std::endl;
  }
  pnt = point[2];
  std::cout << std::endl << ((long)point[2]) << std::endl;
  std::cout << (*pnt)[2] << std::endl;
  std::cout << (*point[2])[2] << std::endl;
#endif

  return 0;
}
