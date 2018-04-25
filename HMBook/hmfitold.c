#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "hmbook.h"

static float *x,*y,*erry;
static double (*function)(double x, double p[]);
static int nd;

int minimizef(double (*function)(double x[]),double p[], int dim);

double chisquare(double par[])
{ double sum = 0.0, fw;
  
  int i;
  for(i=0; i<nd; i++)
    { fw = y[i] - function(x[i],par);
      if (erry != NULL) fw /= erry[i];
      sum += fw*fw;
    }
  return sum;
}

float  HMFitData(int ndata, float xd[], float yd[], float erryd[],
	  double (*func)(double x, double p[]), float par[], int dim)
{ int i;
  float retvalue;
  double *p= (double *) calloc(sizeof(double), dim);

  for(i=0;i<dim;i++) p[i] = par[i];

  x = xd;
  y = yd;
  erry = erryd;
  function = func;
  nd = ndata;

  if (erry != NULL)
    for(i=0;i<nd;i++) 
      if (erry[i]==0.0) 
	{ fprintf(stderr,"HMFitData: y-Error[%d]== 0.0 is not allowed\n",i);
	  free(p); return -1.0;
	}

 
  /* printf("Startchisquare: %f\n",chisquare(p)); */
  if (!minimizef(chisquare, p, dim))
    retvalue =  chisquare(p);
  else 
    retvalue =  -1.0;

  for(i=0;i<dim;i++) par[i] = p[i];
  return retvalue;
}

/*

double testfunction(double xf, double a[])
{ double x = xf-a[0]; 
  return a[1]
       + a[2] * x * x  
       + a[3] * x * x * x
       + a[4] * x * x * x * x;
}

int main()
{ float x[100], y[100], erry[100];
  int i,j;
  double a[5]={1,2,3,2,1};
  float b[5]={.5,1,3,2,0};
  float err;

  for(j=0;j<20;j++)
    { for(i=0;i<100;i++)
	{ norran(&err);
	  x[i] = i/100.0;
	  y[i] = err/100.0 + testfunction(x[i],a);
	  erry[i]=1.0;
	}
      for(i=0;i<5;i++) b[i]=0.0;
      printf("%f:  ",HMFitData(100, x, y, erry, testfunction, b, 5));
      printf("%f %f %f %f %f\n", b[0], b[1], b[2], b[3], b[4]);
    }
  return 0;
}

*/


