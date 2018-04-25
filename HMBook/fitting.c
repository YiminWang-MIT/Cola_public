#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "hmplot.h"
#include "fitting.h"

#define sqr(a) ((a)*(a))
#define epsilon 0.0001


double simplex(double (* func)(double[]), double x[], int dim, 
	       double scale[], double toleranz);
double invert(double *in, double *out, int dim);

double *datavalue;
double *dataerror;
double *datapoint;

double (* fitfunction)(double parameter[], double point[]);

int    dim_parameter;
int    dim_function;
int    nr_of_datapoints;

double chisquare(double parameter[])
{
  int    i;
  double result=0, ch;
  
  for(i=0;i<nr_of_datapoints;i++)
      {
	  ch = (fitfunction(parameter,&datapoint[i*dim_function])-datavalue[i])
	      /dataerror[i];  
	  result += sqr(ch);
      }
  return result;
}

double fit(int nr_d_points, int dim_func,
	   void *d_point, double d_value[], double d_error[],
	   int nr_param, double (* func)(double parameter[], double point[]),
	   double param[], double scale[], double acure,
	   double *chisq, int *dof)
{ double *scl = (double *) calloc(nr_param, sizeof(double));
  int i;

  datavalue = d_value;
  dataerror = d_error;
  datapoint = d_point;
  fitfunction = func;

  dim_parameter = nr_param;
  dim_function = dim_func;
  nr_of_datapoints = nr_d_points;
  *dof = nr_of_datapoints - dim_parameter;

  /* printf("chisquare = %g\n",chisquare(param)); */
  for(i=0;i<nr_param;i++)
  { if (scale) scl[i] = scale[i];
    else
    { scl[i]    = - 2.0 * chisquare(param);
      param[i] += epsilon;         /* set scale sqrt(f(x)/f''(x)) */
      scl[i]   += chisquare(param);
      param[i] -=  2.0 * epsilon; 
      scl[i]   += chisquare(param);
      param[i] += epsilon;
      if (scl[i])
        scl[i]   = epsilon * sqrt(fabs(chisquare(param) / scl[i])); 
      else
	scl[i]=1;
    }
  /*    printf("scale[%d] = %g\n",i,scl[i]); */
  }
  simplex(chisquare,param,nr_param,scl,acure);
  free(scl);
  return *chisq = chisquare(param);
}

void errormatrix(int dim, double *kov, 
		 double (* f)(double[]), double xp[], double eps)
{ int i,j;
  double 
    *x     = (double *) calloc(dim,       sizeof(double)),
    *deriv = (double *) calloc(dim * dim, sizeof(double)),
    f1,f2,f3,f4;

  for(i=0;i<dim;i++) x[i]=xp[i];
  for(i=0;i<dim;i++)
    for(j=0;j<dim;j++)
    { x[i] +=eps; x[j] += eps; f1=f(x); x[i]=xp[i]; x[j]=xp[j];
      x[i] -=eps; x[j] += eps; f2=f(x); x[i]=xp[i]; x[j]=xp[j]; 
      x[i] +=eps; x[j] -= eps; f3=f(x); x[i]=xp[i]; x[j]=xp[j]; 
      x[i] -=eps; x[j] -= eps; f4=f(x); x[i]=xp[i]; x[j]=xp[j]; 
      deriv[i+dim*j] = (f1 - f2 - f3 + f4)/eps/eps/4;
    }

  invert(deriv, kov, dim);

  free(deriv); free(x);
}

double fiterror(double xx[], double point[], double *kov, double eps)
{ int i,j;
  double 
    *deriv = (double *) calloc(dim_parameter, sizeof(double)),
    *x     = (double *) calloc(dim_parameter, sizeof(double)),
    f1,f2,result=0;
  for(i=0;i<dim_parameter;i++) x[i] = xx[i];
  for(i=0;i<dim_parameter;i++)
    { x[i] = xx[i] + eps; f1=fitfunction(x, point); 
      x[i] = xx[i] - eps; f2=fitfunction(x, point);
      x[i] = xx[i];
      deriv[i] = (f1 - f2)/eps/2;
    }
  for(i=0; i<dim_parameter; i++)
    for(j=0; j<dim_parameter; j++)
      result += kov[i + dim_parameter * j] * deriv[i] * deriv[j];

  free(deriv);
  free(x);
  return sqrt(result);
}

#include "hmbook.h"
#include "hmplot.h"

void HMPlotFit(double *par, double *kov, double minx, double maxx, int points)
{
  double x;
  extern int HMFrameNr; 
  HIST *func  = HMBook1("", "", "", "", "", points, minx, maxx);
  HIST *func1 = HMBook1("", "", "", "", "", points, minx, maxx);
  HIST *func2 = HMBook1("", "", "", "", "", points, minx, maxx);
  
  for(x=minx+(maxx-minx)/points/2.0;x<maxx;x += (maxx-minx)/points) {
    double fitfunc = fitfunction(par,&x);
    HMFill(func,x,0,fitfunc);
    if (HMGetOption(FIT_ERROR)) {
      double err = fiterror(par,&x,(double *)kov,0.00001);
      HMFill(func1, x, 0, fitfunc + err);
      HMFill(func2, x, 0, fitfunc - err); 
    }
  } 
  HMFrameNr = 4; HMSame(); HMFunction(func);
  if (HMGetOption(FIT_ERROR)==1) {
    HMSetOption(LINE_COLOR,0xFF0000);HMFrameNr=5; HMSame(); HMFunction(func2);
    HMSetOption(LINE_COLOR,0xFF0000);HMFrameNr=5; HMSame(); HMFunction(func1);
    HMSetOption(LINE_COLOR,0x000000);
  } else   
  if (HMGetOption(FIT_ERROR)) {
    HMFrameNr=2; HMSame(); HMFunction(func2);
    HMFrameNr=2; HMSame(); HMFunction(func1);
  }    
  //  HMDelete(func);
  //HMDelete(func1);
  //HMDelete(func2);
}













