//
// $Id: spline.cc 2216 2008-06-13 21:13:47Z distler $
// 

#include "spline.h"
#include <stdlib.h>
#include <string.h>

void spline::write()
{
  std::cout << " n=" << n << " dim=" << dim << 
    " min=" << min << " max=" << max << std::endl;
  for (int i=0; i<n+1; i++) std::cout << " y[" << i << "]=" << y[i];
  std::cout << std::endl;

  for (int i=0; i<n+1; i++) std::cout << " r[" << i << "]=" << r[i];
  std::cout << std::endl;
  std::cout << "*****************************************" << std::endl;
  std::cout << std::endl;
}

void spline::read(std::istream &in)
{
  in.read((char *) &n,   sizeof(int));
  in.read((char *) &dim, sizeof(int));
  in.read((char *) &min, sizeof(double));
  in.read((char *) &max, sizeof(double));
  y = new double[n+1]; in.read((char *) y, sizeof(double) * (n+1));
  r = new double[n+1]; in.read((char *) r, sizeof(double) * (n+1));
}

spline::spline(const int nr_of_points, double points[], 
	       double min, double max, boundaryCond boundary) 
  : interpolation(nr_of_points-1, 1, min, max)
{
  y = new double[n+1];
  memcpy(y, points, sizeof(double) * (n+1));
  r = new double[n+1];
  double *m = new double[(n+1)*(n+1)], *b = new double[n+1];
  for (int i=0; i<(n+1)*(n+1); i++) m[i]=0;
  for(int j=1;j<n;j++) {
    m[(j-1)*(n+1)+j] = m[(j+1)*(n+1)+j] = 1;
    m[j+j*(n+1)]     = 4;
    b[j]             = 6*(y[j+1] - 2*y[j] + y[j-1]);
  }

  // Choice of boundary conditions
  // NATURAL:     Zero second derivative f"(0)==f"(1)==0
  // ZERO:        Zero first derivative  f'(0)==f'(1)==0
  // PERIODIC:    Cyclic function        f(0)==f(1), f'(0)==f'(1), f"(0)==f"(1)
  switch (boundary) {
  case NATURAL:  m[0] = m[n+n*(n+1)] = 1; 
                 b[0] = b[n] = 0;
		 break;
  case ZERO:     m[0]             =-2; m[(n+1)]    =-1; b[0]=6*(y[0]   - y[1]);
                 m[n+(n-1)*(n+1)] = 1; m[n+n*(n+1)]= 2; b[n]=6*(y[n-1] - y[n]);
		 break;
  case PERIODIC: m[0]             = 6; m[n+n*(n+1)] = -6;
                 m[n+(n-1)*(n+1)] = 1; m[n+n*(n+1)] =  2; 
		 m[n]             = 2; m[n+(n+1)  ] =  1;
		 b[0]             = 0; b[n] = 6 * (y[n-1]-y[n]-y[0]+y[1]);
		 break;
  };
  
  // Gauss-Elimination with backsubstitution
  // Tri-diagonal algorithm should be slightly faster
    for (int s = 0; s < n; s++)
    for (int z = s + 1; z < n + 1; z++) {
      double faktor = m[z + (n + 1) * s] / m[s + (n + 1) * s];
      for(int i=0; i<(n+1); i++) m[z+(n+1)*i] -= m[s+(n+1)*i] * faktor;
      b[z] -= b[s] * faktor;
    }
  for (int z = n; z >= 0; z--) {
    double dummy = 0;
    for(int s = n; s > z; s--) dummy += m[z + (n+1) * s] * r[s];
    r[z] = (b[z] - dummy) / m[z + (n+1) * z];
  }
  for (int z=0;z<n+1; z++ ) r[z]/=6;
  delete[] m; delete[] b;
}
