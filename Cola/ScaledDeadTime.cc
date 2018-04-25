// Created by Harald Merkel
//            Institut fuer Kernphysik
//            Universitaet Mainz
//            27/03/96
//
// Calulation of dead time correction for prescaled events
//
#include <math.h>
#include "hmplot.h"

static double 
Correction(double lambda, int scaling)
{ int    n = 0;
  double sum = 0, Corr = 0, P = exp(-lambda);
  do 
  { P *= (n?lambda/n:1);     // Recursion for Poisson Distribution P(n,lambda)
    sum  += P;               // Total probability needed for exit condition
    Corr += P * (n/scaling); // That's the Correction, (n/scaling) is an int!
    n++;
  } while ( lambda / scaling * (1-sum) > 1e-10);

  return scaling * (1.0 + Corr);
}

double 
TrueRate(double Rate, double Deadtimefraction, int Scaling)
{
  double True = Correction(Deadtimefraction*Scaling, Scaling);
  double Last;
  do 
  { Last = True;
    True = Correction(Deadtimefraction*Last, Scaling);
//    printf("%f %f\n", True,1-Scaling/True);
  } while (fabs(Last-True) > 1e-7);
  return True * Rate;
}

int
main()
{
  HMPlotFile("deadtime.eps");
  double x[100],y[100];
  int i,k;
  for (k=1;k<50;k++)  
  {i=0;
   double alpha;
   for(alpha=0;alpha<0.8; alpha+= 0.8/99)
   { x[i]=alpha*100;
     y[i] = TrueRate(1,alpha,k);
   }
   if (k>1) HMSame();
   HMSetOption(ON_SCALE_X,1.0)
   HMPlotFunction(i,x,y,"Dead Time Correction","[a]","%","Prescaling","");
   HMClose();
}


