#include <stdlib.h>
#include <math.h>

#include "hmbook.h"
#include "hmplot.h"
#include "fitting.h"

double (* func1)(double parameter[], double x);
double (* func2)(double parameter[], double x, double y);

double *parameter = NULL;

double ffunction(double parameter[], double point[])
{ if (func1) 
    return func1(parameter, point[0]);
  else 
    return func2(parameter, point[0], point[1]);
}

float plotfunc(float x)
{ double point[1];
  point[0]=x;
  return ffunction(parameter, point);
}

void HMFit1(HIST *id, double (*function)(double [],double x), 
	    double *par, int params)
{ double *x, *y, *err, chisq, *kov;
  extern float HMminimumx, HMmaximumx;
  char mns[255];
  int dof, i, j=0;

  parameter = (double *) calloc(params, sizeof(double));
  for (i=0;i<params;i++) parameter[i]=par[i];

  if (id->ny) puts("Dieser Aufruf ist nur fuer 1-Dim Histogramme!");
  func1 = function;
  func2 = NULL;

  x   = (double *) calloc(id->nx, sizeof(double));
  y   = (double *) calloc(id->nx, sizeof(double));
  err = (double *) calloc(id->nx, sizeof(double));
  kov = (double *) calloc(params*params, sizeof(double));

  for(i=0; i<id->nx; i++) { 
    float thisx=id->minx+(id->maxx-id->minx)/id->nx*(i+0.5);
    if (id->data[i]>0 
	&& (HMminimumx==NOMINIMUM || thisx >= HMminimumx)
	&& (HMmaximumx==NOMINIMUM || thisx <= HMmaximumx))
    x[j]   = thisx;
    y[j]   = id->data[i];
    err[j] = sqrt(id->data[i]+2.0);
    j++;
  }
  fit(j, 1, (void*) x, y, err, params, ffunction, 
      parameter, NULL, 0.0000001, &chisq, &dof);
  fit(j, 1, (void*) x, y, err, params, ffunction, 
      parameter, NULL, 0.0000001, &chisq, &dof);

  if (HMGetOption(FIT_ERROR))
    errormatrix(params, kov, chisquare, parameter, 0.0000001);

  HMPlotFit(parameter, kov, (HMminimumx!=NOMINIMUM ? HMminimumx : id->minx), 
                            (HMmaximumx!=NOMINIMUM ? HMmaximumx : id->maxx), 100);
  for (i=0;i<params;i++)
    { if (HMGetOption(FIT_ERROR))
       sprintf(mns,"p_%d! = %6.2f [\\261] %5.2f", 
	    i, parameter[i], sqrt(kov[i+i*params]));
      else
       sprintf(mns,"p_%d! = %6.2f", i, parameter[i]);
    HMComment(mns,i+1);
  }
  sprintf(mns,"[c]^2!/DoF = %6.1f/%d", chisq, dof);
  HMComment(mns,0);
  for (i=0;i<params;i++) par[i]=parameter[i];
  free(x);
  free(y);
  free(err);
}











