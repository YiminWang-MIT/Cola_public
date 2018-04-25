#include <math.h>
#include <stdio.h>

/* Old program-code by Harald Merkel */

/*

double Gamma(double x)
{ if (x == 1.0) return 1.0;
  if (x == 0.5) return sqrt(3.1415926535);
  return(Gamma(x-1.0)*(x-1.0));
}


double pchisq(double chisq, int  free)
{	double sum=1, term=1, chi2=chisq/2, LI = (free-2)/2.0;
	int   i;
	
	if(!(free%2))
	{	for(i = 1;i<=LI;i++)
		{	term *= chi2/i;
			sum += term;
		}
		return exp(-chi2)*sum;
	}
	term = exp(log(chi2)*(LI+1));
	sum = term/(LI+1.0);
	for(i=1; i<100; i++)
	{	term *= -chi2/i;
		sum += term/(LI+i+1);
	}
	return 1.0 - 1.0/Gamma(LI+1.0)*sum;
}

*/

/* New code by Ingo Ewald  June 1998
   Algorithms: Numerical Recipes in C  
               2nd Edition
               Cambridge University Press */

#define EPS   1E-12
#define ITMAX 10000
#define FPMIN 1E-32

double gammln(double xx)
{
  int j;
  double x,y,tmp,ser;
  static double cof[6] = { 76.18009172947146     , -86.5052032941677,
			   24.01409824083091     ,  -1.231739572450155,
			    0.1208650973866179E-2,  -0.5395239384953E-5};
  
  y = x = xx;
  tmp = x + 5.5;
  tmp -= (x + 0.5) * log(tmp);
  ser = 1.000000000190015;
  for (j=0; j<=5; j++) ser += cof[j] / ++y;
  return -tmp + log(2.5066282746310005 * ser / x);
}


void gcf(double *gammcf, double a, double x, double *gln)
{
  int i;
  double an, b, c, d, del, h;

  *gln = gammln(a);
  b = x + 1 - a;
  c = 1 / FPMIN;
  d = 1 / b;
  h = d;
  for (i=1; i<=ITMAX; i++) {
    an = -i * (i - a);
    b += 2.0;
    d = an * d + b;
    if (fabs(d) < FPMIN) d = FPMIN;
    c = b + an / c;
    if (fabs(c) < FPMIN) c = FPMIN;
    d = 1 / d;
    del = d * c;
    h *= del;
    if (fabs(del - 1) < EPS) break;
  }
  if (i > ITMAX) printf("a too large, ITMAX too small in routine gcf");
  *gammcf =  exp(-x + a * log(x) - (*gln)) * h;
}

void gser(double *gamser, double a, double x, double *gln) 
{
  int n;
  double sum, del, ap;
  
  *gln=gammln(a);
  if (x <= 0) {
    if (x < 0) printf("x less than 0 in rotuine gser \n");
    *gamser = 0;
    return;
  }
  else {
    ap = a;
    del = sum = 1 / a;
    for (n=1; n<=ITMAX; n++) {
      ++ap;
      del *= x / ap;
      sum += del;
      if (fabs(del) < fabs(sum)*EPS) {
	*gamser = sum * exp(-x + a * log(x) - (*gln));
	return;
      }
    }
    printf("a too large, ITMAX too small in routine gser \n");
    return;
      }
}


double gammq(double chisquare, int dof)
{
  double a, x, gamser, gammcf, gln;

  a = dof/2; x = chisquare/2;

  if (x < 0 || a <= 0) printf("Invalid arguments in routine gammp !\n");
  if (x < (a + 1)) {
    gser(&gamser, a, x, &gln);
    return 1 - gamser;
  }
  else {
    gcf(&gammcf, a, x, &gln);
    return gammcf;
  }
}

double pchisq(double chisquare, int dof)
{
  return gammq(chisquare, dof);
}


