/*
	+---------------------------------------------------------------+
	!	Numerisches Minimieren 					!
	!	======================					!
	!								!
	!	Verfahren nach Broyden,Fletcher,Goldfarb und Shanno	!
	!	(BFGS-Verfahren)					!
	!								!
	!	Aufruf:							!
	!								!
	!	1. Gradient bekannt => 					!
	!		minimize(gradient, x, dimension)		!
	!	2. Gradient unbekannt =>				!
	!		minimizef(funktion, x, dimension)		!
	!								!
	!	- double funktion(double x[dimension])			!
	!	  ist hierbei die zu minimierende Funktion		!
	!	- int gradient(double x[dimension,g[dimension])		!
	!	  ist hierbei eine Routine, die den Gradienten		!
	!	  der Funktion berechnet und in g ablegt.		!
	!	  Rueckgabe != 0 bei Fehler moeglich			!
	!	- int dimension						!
	!	  ist die Dimension des Problems			!
	!	- double x[dimension]					!
	!	  ist Startwert der Minimierung und 			!
	!	  gleichzeitig Ergebnisverktor				!
	! 								!
	!	Rueckgabewert != 0 bei Fehler				!
	+---------------------------------------------------------------+
*/
#include <stdio.h>
#include <math.h>
#define	epsilon	0.000001
#define maxdim	50
#define maxdi2  2500
#define	fo(i)	for(i=0;i<dim;i++)

static	short	dim;
static	int	(* mgradient)(double *, double *);
static	double	(* mfunction)(double x[]);
static	double	sk[maxdi2], xk[maxdi2];
static	int	fault;

#define GOLDENSECTION 0.38196601 /* = (3-sqrt(5))/2 */

double linmin(double (*f)(double x),double *mini)
{
  double a, b, c, n, fa, fb, fc=0, fn;
  
  fa = f(a = 0.0);
  fb = f(b = 0.001); 
  c = 0;
  while(fa==fb) 
  { fb = f(b = b+(b-a)/GOLDENSECTION);
    if (b>10000.0) exit(-1);
  }

  while(fa < fb)
    { c = b; fc= fb;
      fb = f(b -= GOLDENSECTION*b);
if(0)      printf("fa<fb -> %g %g %g %g %g %g\n",a,b,c,fa,fb,fc);
    }

  if (fa==fb) 
  { puts("Boese Falle 2");
    fa = f(a = -0.001);
    while (fa<=fb)
      { fa = f(a *= 1.0/GOLDENSECTION);
	if (a<-10000.0) {puts("Keine Rettung");return(1.0);}
      }
  }
  if (c==0)
    { fc = f(c = b+(b-a)/GOLDENSECTION);
      while(fc==fb)
	{ b = c; fb = fc;
	  fc = f(c = b+(b-a)/GOLDENSECTION);
	}
      while(fc<fb)
	{ a = b; fa = fb;
	  b = c; fb = fc;
	  fc = f(c = b+(b-a)/GOLDENSECTION);
	}
      if(fa<=fb || fb>=fc || a>=b || b>=c) puts("linmin 0");
    }

  if(fa<fb || fb>fc || a>b || b>c) puts("error 1");
  if(fa==fb) puts("Falle 1");
  if(fb==fc) puts("Falle 2");
  if(a==b) puts("Falle 3");
  if(b==c) puts("Falle 4");

  while ((c-a) > 1e-2 * b)
  { if (b-a > c-b)
      { fn = f(n = b - (b-a) * GOLDENSECTION);
	if (fn<fb) {c=b; fc=fb; b=n; fb=fn;}
	else       {a=n;fa=fn;}
      }
    else
      { fn = f(n = b + (c-b) * GOLDENSECTION);
	if (fn<fb) {a=b; fa=fb; b=n; fb=fn;}
	else       {c=n; fc=fn;}
      }
  }
  if(fa<=fb || fb>=fc || a>=b || b>=c) puts("linmin 2");
  *mini=fb;
  return b;
}

double linfunction(double lambda)
{ double xx[maxdi2];
  int i;

  fo(i) xx[i] = xk[i] - lambda * sk[i];
  fo(i) if (fabs(xx[i])>10E10) {fault = -1; return (0.0); }
  return mfunction(xx);
}

#define MAX_NUMBER_OF_ALLOWED_STEPS 200

int	minimize(int 	(* gradient)(double *,double *),
		 double  (* function)(double *),
		 double	x[maxdim],
		 int	dimension)

{	double	qHq, pq, lambda, grbetrag, mini;
	double	gr[maxdim], grneu[maxdim], xneu[maxdim],
		ps[maxdim], qs[maxdim], qH[maxdim],
		H[maxdi2], pqH[maxdi2], Hqp[maxdi2], qp[maxdi2], pp[maxdi2];
	int	i, j, k;

	int steps_still_allowed = MAX_NUMBER_OF_ALLOWED_STEPS;

	dim = dimension;
	mgradient = gradient;
        mfunction = function;

	for(i=0;i<dim*dim;i++) H[i] = 0.0;
	fo(i)	H[i+dim*i] = 1.0;
	fo(i)	xk[i]	   = x[i];

	fo(i)	if (fabs(xk[i])>10E10) return(-1);
	if(mgradient(xk,gr)) return(-1);

	while (--steps_still_allowed)
	{ fo(i)
	  { sk[i] = 0.0;
	    fo(j) sk[i] += H[i+dim*j] * gr[j];
	  }
	  lambda = linmin(linfunction, &mini);
/*	  printf("lambda:%g %g\n",lambda,mini); */
	  if (fault||fabs(lambda)>10E20) return(-1);
	  
	  fo(i)	xneu[i] = xk[i] - lambda * sk[i];
	  fo(i)	if (fabs(xneu[i])>10E10) return(-1);
	  if (mgradient(xneu,grneu)) return -1;
	  
	  grbetrag = 0.0;
	  fo(i)
	  { qs[i] = grneu[i] - gr[i];
	    ps[i] = xneu[i] - xk[i];
	    xk[i] = xneu[i];
	    gr[i] = grneu[i];
	    grbetrag += gr[i] * gr[i];
	  }
	  
	  grbetrag = sqrt(grbetrag);
/*	  printf("Gradient: %g\n",grbetrag);
	  fo(i) printf("%15g %15g %15g\n",sk[i],ps[i],qs[i]);
*/	  if (grbetrag<0.00001) break;
	  
	  fo(i)
	    { qH[i]=0.0;
	      fo(j) qH[i] = qH[i] + qs[j] * H[j + dim*i];
	    }
	  
	  qHq = 0.0;
	  fo(i) fo(j) 
	    { pqH[i+dim*j] = ps[i]*qH[j];
	      qp [i+dim*j] = qs[i]*ps[j];
	      pp [i+dim*j] = ps[i]*ps[j];
	      qHq = qHq + qs[i] * H[i+dim*j] * qs[j];
	    }
	  
	  pq = 0.0;
	  fo(i)	pq += ps[i] * qs[i];
	  
	  if (pq == 0.0) {puts("pq=0");break;}
	  
	  fo(i) fo(j) 
	    { Hqp[i+dim*j] = 0.0;
	      fo(k) Hqp[i+dim*j] += H[i+k*dim]*qp[k+j*dim];
	    }
	  
	  fo(i) fo(j) H[i+dim*j] += (1 + qHq/pq)*pp[i+dim*j]/pq
	    - (pqH[i+dim*j]+Hqp[i+dim*j])/pq;
	  
	}
	
	if (!steps_still_allowed)
	  {
	    fprintf(stderr,"\nBFGS:Iteration exceeded max number of steps");
	    return(-1);
          }
	
	fo(i)	x[i] = xk[i];
	/* printf("Gradient: %g\n",grbetrag); */
	return(0);
}

/*	+---------------------------------------------------------------+
	!	Minimieren durch numerisches Differenzieren		!
	+---------------------------------------------------------------+
*/
static double    (* numfunc)(double *);
static short	numdim;

void	numdiff(function,x,grad,dimension)	/* num. Differenzieren */
double	(* function)(double *);
double	*x,*grad;
short	dimension;
{	double myx[maxdi2], f1, f2;
	int i, j; 

	for(i=0;i<dimension;i++)
	{ for(j=0;j<dimension;j++)
	    myx[j] = x[j];
	  myx[i] -= epsilon;
	  f1 = function(myx);
	  myx[i] += epsilon + epsilon;
	  f2 = function(myx);
	  grad[i] = (f2-f1)/epsilon/2.0;
	}
}

int	numgrad(xx, gg)				/* numer. Gradient	*/
double	*xx, *gg;
{	numdiff(numfunc, xx, gg, numdim);
	return(0);
}

int	minimizef(funktion,xx,dimension)
double	(* funktion)(double *);
double 	*xx;
int	dimension;
{	numdim = dimension;
	numfunc = funktion;
	return(minimize(numgrad,funktion,xx,dimension));
}






















