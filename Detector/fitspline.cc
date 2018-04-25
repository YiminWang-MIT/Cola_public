//
// $Id: fitspline.cc 2216 2008-06-13 21:13:47Z distler $
//

#include <stdlib.h>
#include <math.h>
#include "HMBook/hmplot.h"
#include "HMBook/hmbook.h"
#include <iostream>
#include "fitspline.h"

#define epsilon 0.0001

double simplex(double (* func)(double[]), double x[], int dim, 
	    double scale[], double toleranz);
double simplextry(double **s, double f[], double cms[], double trial[], int hi,
		double (*function)(double []), int dim, double factor);

double *datavalue;
double *dataerror;
double *datapoint;

static double (*function)(double x, double p[]);

int    nr_of_datapoints;

double chisqrpdc(double par[])
{
  double result=0, ch;
  /*
  std::cout << "\n" << "CHISQUARE>  par: " << std::endl;
  for (int i=0; i<10; i++) std::cout << par[i] << std::endl;
  std::cout << " dim_par=" << (SAMPLES+2) << 
    " ndata=" << nr_of_datapoints << std::endl;
  */
  for (int i=0;i<nr_of_datapoints;i++) {
    /*
      std::cout << "i=" << i << " data=" << datapoint[i] <<
	" min=" << par[SAMPLES] << " max=" << par[SAMPLES+1] <<
	" func(x)=" << function(datapoint[i], par) << std::endl;
    */
    ch = (function(datapoint[i], par)-datavalue[i]);
    if (dataerror != NULL) ch /= dataerror[i];
    
    result += ch*ch;
    //std::cout << "  y=" << datavalue[i]
    //      << " ch=" << ch << " sum=" << result << " *** " << std::endl;
  }
  std::cout << " FIT: chisquare= " << result << " / " << 
    NDAT-SAMPLES-1 << "= chired= " << result/(NDAT-SAMPLES-1) << std::endl;
  return result;
}

double fitSplineData(int nr_d_points,
	   double d_point[], double d_value[], double d_error[],
	   int nr_param, double (* func)(double x, double parameter[]),
	   double param[], double scale[], double acure){ 
  double *scl = new double[nr_param];
  int i;
  datavalue = d_value;
  dataerror = d_error;
  datapoint = d_point;
  function = func;
  const int dim_parameter = nr_param;
  nr_of_datapoints = nr_d_points;

  // for (int i=0; i<nr_d_points; i++) std::cout << datapoint[i] << "/" << 
  //   datavalue[i] << "+-" << dataerror[i] << "  " ;
  std::cout << " x/y/erry initialized. "  << std::endl;
  std::cout << "Startchisquare: "<< chisqrpdc(param) << std::endl;

  for(i=0;i<dim_parameter;i++) {
    if (scale) scl[i] = scale[i];
    // if (0) ;
    else {
      scl[i]    = - 2.0 * chisqrpdc(param);
      param[i] += epsilon;         /* set scale sqrt(f(x)/f''(x)) */
      scl[i]   += chisqrpdc(param);
      param[i] -=  2.0 * epsilon; 
      scl[i]   += chisqrpdc(param);
      param[i] += epsilon;
      scl[i] = (scl[i]==0) ? 1 : epsilon*sqrt(fabs(chisqrpdc(param) / scl[i]));
    }
      std::cout << " scale_" << i << " = " << scl[i] << std::endl;
  }
  simplex(chisqrpdc, param, dim_parameter, scl, acure);
  delete [] scl;
  return chisqrpdc(param);
}
