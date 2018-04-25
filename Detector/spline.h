//                                                                    -*-c++-*-
//
// $Id: spline.h 2348 2009-10-09 10:07:29Z merkel $
// 

#ifndef __SPLINE_H__
#define __SPLINE_H__

//
// Equidistant grid interpolation
//

#include<iostream>
#include<fstream>
#include<stdlib.h>

//-----------------------------------------------------------------------------
// Generic multidimensional interpolation
//-----------------------------------------------------------------------------
//
class interpolation {
protected:
  int n, dim;
  double min, max;
  inline double cubic(double x) {return x*x*x;}

public:
  interpolation(int n=0, int dim=0, double min=0.0, double max=1.0) { 
    interpolation::n = n; 
    interpolation::dim = dim; 
    interpolation::min = min; 
    interpolation::max = max; 
  };
  virtual ~interpolation() {};
};
//-----------------------------------------------------------------------------
// 1-dim cubic spline interpolation
//-----------------------------------------------------------------------------
//
class spline : public interpolation {
private:
  double *y, *r;
  
public:
  enum boundaryCond {NATURAL, ZERO, PERIODIC};
  //  spline(int n = 0) : interpolation (n, 1) { y = r = NULL;};   ???
  ~spline() { delete[] y; delete[] r; y = r = NULL;}
  spline(const int nr_of_points, double points[], double min, double max, 
	 boundaryCond b = NATURAL);
  double operator()(double xx) {
    double x = (xx - min)/(max - min);
    if (x < 0 || x > 1) {
      std::cerr << "ERROR: Spline interpolation out of range." << std::endl;
      exit(-1);
    }
    int j = (int) (x * n) + 1;
    if (j==n+1) j = n;
    return r[j-1] * cubic(j-x*n  ) + (y[j-1]- r[j-1]) * (j - x*n    ) +
           r[j  ] * cubic(x*n-j+1) + (y[j]  - r[j]  ) * (x*n - j + 1);
  };
  void write();
  void write(std::ostream &out);
  void write(char *fname) {std::ofstream out(fname); write(out); out.close(); }
  void read (std::istream &in );
};

#endif /* __SPLINE_H__ */
