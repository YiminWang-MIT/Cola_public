#ifndef _FITTING_H
#define _FITTING_H

#if defined(__cplusplus)
extern "C" {
#endif
  
double simplex(double (* func)(double[]), double x[], int dim, 
	       double scale[], double toleranz);
double pchisq(double chisq, int  free);
double chisquare(double parameter[]);
double fit(int nr_d_points, int dim_func,
	   void *d_point, double d_value[], double d_error[],
	   int nr_param, double (* func)(double parameter[], double point[]),
	   double param[], double scale[], double acure,
	   double *chisq, int *dof);
void   errormatrix(int dim, double *kov, 
		   double (* f)(double[]), double xp[],double eps);
double fiterror(double xx[], double point[], double *kov, double  eps);
void HMPlotFit(double *par, double *kov, double minx, double maxx, int points);

#if defined(__cplusplus)
}
#endif

#endif




