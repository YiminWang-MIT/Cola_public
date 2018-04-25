#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAXSTEP 100000

double simplextry(double **s, double f[], double cms[], double try[], int hi,
		  double (*function)(double []), int dim, double factor)
{ 
  double ftry;
  int i;
  for(i=0;i<dim;i++) try[i] = (1-factor)*(cms[i]-s[hi][i])/dim+factor*s[hi][i];
  ftry = (*function)(try);
  if (ftry < f[hi]) {
    f[hi] = ftry;
    for(i=0; i<dim; i++) { cms[i]-=s[hi][i]; cms[i] += (s[hi][i]=try[i]); }
  }
  return ftry;
}

double simplex(double (* func)(double[]), double x[], int dim, 
	       double scale[], double toleranz)
{ 
  int    i, j, step, hi, low, nhi;
  double fhi, flow, fnhi, ftry;
  double *s[dim+1], f[dim+1], cms[dim], try[dim];
  
  for(i=0; i<dim+1; i++) s[i] = (double *) calloc(sizeof(double ), dim);
  for(i=0; i<dim+1; i++) {
    for(j=0; j<dim; j++) s[i][j] = x[j];  
    if (i) s[i][i-1] = s[i][i-1] + scale[i-1];
    f[i] = (*func)(s[i]);
  }
  for(j=0; j<dim; j++) cms[j] = 0;
  for(i=0; i<dim+1; i++) for(j=0; j<dim; j++) cms[j] += s[i][j];  
  
  for(step=0;step<MAXSTEP;step++) {
    fhi  = f[hi =  0]; for(i=0;i<dim+1;i++) if (f[i]>fhi )       fhi =f[hi =i];
    flow = f[low=  0]; for(i=0;i<dim+1;i++) if (f[i]<flow)       flow=f[low=i];
    fnhi = f[nhi=low]; for(i=0;i<dim+1;i++) if(i!=hi&&f[i]>fnhi) fnhi=f[nhi=i];
    
    if (2*fabs(fhi-flow)/(fabs(fhi)+fabs(flow)) < toleranz) break;
    ftry = simplextry(s, f, cms, try, hi, func, dim, -1.0);
    if (ftry <= flow) 
      ftry = simplextry(s, f, cms, try, hi, func, dim, 2.0);
    else 
      if (ftry>=fnhi) {
	ftry = simplextry(s, f, cms, try, hi, func, dim, 0.5);
	if (ftry>=fhi)
	  for(i=0;i<dim+1;i++)
	    if (i!= low) { 
	      for(j=0;j<dim;j++) s[i][j] = (s[i][j]+s[low][j])/2;
	      f[i] = (*func)(s[i]);
	    }
      }
  }
  if (step==MAXSTEP) {
    fprintf(stderr,"SIMPLEX: aborted by STEP==MAXSTEP!");
    flow = -1.0;
  }
  for(i=0;i<dim;  i++) x[i]=s[low][i];
  
  return flow;
}






