//
// $Id: simplex.cc 2216 2008-06-13 21:13:47Z distler $
// 

#include <stdlib.h>
#include <math.h>
#include <iostream>
#include "fitspline.h"

#define MAXSTEP 100000

double simplextry(double **s, double f[], double cms[], double trial[], int hi,
		  double (*function)(double []), int dim, double factor)
{ 
  double ftry;
  int i;
  for(i=0;i<dim;i++) trial[i]=(1-factor)*(cms[i]-s[hi][i])/dim+factor*s[hi][i];
  ftry = (*function)(trial);
  if (ftry < f[hi]) {
    f[hi] = ftry;
    for(i=0; i<dim; i++) { cms[i]-=s[hi][i]; cms[i] += (s[hi][i]=trial[i]); }
  }
  return ftry;
}

double simplex(double (* func)(double[]), double x[], int dim, 
	       double scale[], double toleranz)
{ 
  int    i, j, step, hi, low, nhi;
  double fhi, flow, fnhi, ftry;
  double *s[dim+1], f[dim+1], cms[dim], trial[dim];

  std::cout << "SIMPLEX> params: " ;
  for (i=0; i<dim; i++) std::cout << " p" << i << "=" << x[i];
  std::cout << std::endl;

  for(i=0; i<dim+1; i++) s[i] = (double *) calloc(sizeof(double ), dim);
  for(i=0; i<dim+1; i++) {
    for(j=0; j<dim; j++) s[i][j] = x[j];  
    if (i) s[i][i-1] = s[i][i-1] + scale[i-1];
    f[i] = (*func)(s[i]);
  }
  for(j=0; j<dim; j++) cms[j] = 0;
  for(i=0; i<dim+1; i++) for(j=0; j<dim; j++) cms[j] += s[i][j];  
  
  for(step=0;step<MAXSTEP;step++) {
    std::cout << "SIMPLEX counter: " << step << "  ";
    fhi  = f[hi =  0]; for(i=0;i<dim+1;i++) if (f[i]>fhi )       fhi =f[hi =i];
    flow = f[low=  0]; for(i=0;i<dim+1;i++) if (f[i]<flow)       flow=f[low=i];
    fnhi = f[nhi=low]; for(i=0;i<dim+1;i++) if(i!=hi&&f[i]>fnhi) fnhi=f[nhi=i];
    
    if (2*fabs(fhi-flow)/(fabs(fhi)+fabs(flow)) < toleranz) break;
    ftry = simplextry(s, f, cms, trial, hi, func, dim, -1.0);
    //std::cout<< "SIMPLTRY> " << " ftry_" << step << " " << ftry <<std::endl;
    if (ftry <= flow) 
      ftry = simplextry(s, f, cms, trial, hi, func, dim, 2.0);
    else 
      if (ftry>=fnhi) {
	ftry = simplextry(s, f, cms, trial, hi, func, dim, 0.5);
	if (ftry>=fhi)
	  for(i=0;i<dim+1;i++)
	    if (i!= low) { 
	      for(j=0;j<dim;j++) s[i][j] = (s[i][j]+s[low][j])/2;
	      f[i] = (*func)(s[i]);
	    }
      }
  }
  if (step==MAXSTEP) {
    std::cerr << "SIMPLEX: aborted by STEP==MAXSTEP!" << std::endl;
    flow = -1.0;
  }
  for(i=0; i<dim; i++) x[i]=s[low][i];

  std::cout << "SIMPLEX> snodes: " ;
  for (i=0; i<dim; i++) std::cout << " x" << i << "=" << x[i];
  std::cout << "\nSIMPLEX> scales: " ;
  for (i=0; i<dim; i++) std::cout << " sc" << i << "=" << scale[i] ;
  std::cout << "\n dim=" << dim << " acure=" << toleranz << std::endl;
  return flow;
}
