// Harald Merkel, 18.12.1995
//
// $RCSfile: QuasiRandom.cc,v $
// $Revision: 2640 $ $Author: aqua $ $Date: 2015-04-01 10:03:00 -0400 (Wed, 01 Apr 2015) $

#define _DEFAULT_SOURCE 1
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "QuasiRandom.h"
#include "Landau.tab" // Table of Landau distribution

PseudoRandom::PseudoRandom() { }

double PseudoRandom::operator()()
{
  const int    M1  = 259200;
  const int    IA1 = 7141;
  const int    IC1 = 54773;
  const double RM1 = 1.0/M1;
  const int    M2  = 134456;
  const int    IA2 = 8121;
  const int    IC2 = 28411;
  const double RM2 = 1./M2;
  const int    M3  = 243000;
  const int    IA3 = 4561;
  const int    IC3 = 51349;

  static int idum = 0;
  static long ix1,ix2,ix3;
  static float r[98];

  float temp;
  static int iff=0;
  int j;

  if (idum < 0 || iff == 0) {
    iff=1;
    ix1=(IC1-(idum)) % M1;
    ix1=(IA1*ix1+IC1) % M1;
    ix2=ix1 % M2;
    ix1=(IA1*ix1+IC1) % M1;
    ix3=ix1 % M3;
    for (j=1;j<=97;j++) {
      ix1=(IA1*ix1+IC1) % M1;
      ix2=(IA2*ix2+IC2) % M2;
      r[j]=(ix1+ix2*RM2)*RM1;
    }
    idum=1;
  }
  ix1=(IA1*ix1+IC1) % M1;
  ix2=(IA2*ix2+IC2) % M2;
  ix3=(IA3*ix3+IC3) % M3;
  j=1 + ((97*ix3)/M3);
  if (j > 97 || j < 1) std::cerr << "rndm: This cannot happen." << std::endl;
  temp=r[j];
  r[j]=(ix1+ix2*RM2)*RM1;
  return temp;
}

SobolSequence::SobolSequence(const int d)
{
  const short initmdeg[] = {0, 
			1, 2, 3, 3, 4, 4, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6};
  const int initip[] = {0, 
			 0, 1, 1, 2, 1, 4, 2, 4, 7,11,13,14, 1,13,16,19,22,25};
  const int initiv[] = {0, 
			 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
			 3, 1, 3, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
			 5, 7, 7, 3, 3, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
			 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
			 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
			 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1};
  
  dim = (d > MAXDIM ? MAXDIM : d);
  xSet = new double[dim];
  in = 0;

  for (int k=0; k<=MAXDIM; k++) mdeg[k] = initmdeg[k]; // just copy
  for (int k=0; k<=MAXDIM; k++) ip[k]   = lint(initip[k],0);
  for (int k=1; k<=MAXDIM; k++) ix[k]   = lint(0,0);
  for (int k=0; k<=MAXDIM*mdeg[MAXDIM]; k++) 
    iv[k] = lint(initiv[k],0);

  for (int j=1,k=0; j <= MAXBIT; j++,k += MAXDIM) iu[j]=&iv[k]; //for 2d access

  for (int k=1; k<=MAXDIM; k++) {
    for (int j=1; j<=mdeg[k]; j++)   // normalize first elements
      iu[j][k] <<= (MAXBIT-j);
    for (int j=mdeg[k]+1; j<=MAXBIT; j++) {   // recurrency relation for vi
      lint ipp = ip[k];
      lint i   = iu[j-mdeg[k]][k];
      i ^= i >> mdeg[k];
      for(int l = mdeg[k]-1; l>=1; l--) {
	if (ipp & lint(1,0)) i ^= iu[j-l][k];
	ipp >>= 1;
      }
      iu[j][k]=i;
    }
  }
}  

double 
SobolSequence::operator()()
{
  if (access >= dim) {
    std::cerr << "ERROR: Sequence contains " << dim
	      << " values!" << std::endl << std::flush;
    exit(-1);
  }
  return xSet[access++];
}

void 
SobolSequence::operator()(double x[])
{
  const double fac = 1.0 / (lint(1,0) << MAXBIT);
  lint im = in++;
  int j = 0 ; 
  for(j = 1; j <= MAXBIT; j++) if (im & lint(1,0)) im >>= 1; else break;
  if (j > MAXBIT) {
    std::cerr << "\aERROR: Max length of Sobol' Sequence reached: "
	      << in << std::endl;
    exit(-1);
  }
  for (int k=1; k<=dim; k++) {
    ix[k]  ^= iv[(j-1) * MAXDIM + k];
    x[k-1] =  ix[k] * fac;
  }
}

void
SobolSequence::init(double start)
{
  in = start;
  for (int k=1; k<=dim; k++) {
    lint graycode = lint(in) ^ (lint(in) >> 1);
    ix[k] = lint(0,0);
    for (int i = 0; i<MAXBIT; i++, graycode >>= 1) 
      if (graycode & lint(1,0)) ix[k] ^= iv[i * MAXDIM + k];
  }
}
///////////////////////////////////////////////////////////////////////////////

HaltonSequence::HaltonSequence(const int b)
{ unsigned int i;
  base = b;
  start = 0;
  maxdigit = (int) (30.0/log(base)*log(2))-1;
  digits = new unsigned long int [maxdigit+2];
  digits[0]=1; 
  for(i=1;i<=maxdigit+1;i++) digits[i] = digits[i-1]*base;
}

double 
HaltonSequence::operator()()
{
  volatile unsigned long int result = 0;
  start++;
  for(unsigned int i = 0; i < maxdigit; i++) 
    result += ((unsigned long)(start / digits[i]) % base) * digits[maxdigit-i];
  return (double) result / (double) digits[maxdigit+1];
}

// To get independend Halton sequences, they must be inititalized by a 
// prime number. Use one of these:

HaltonSequence halton[] = {
  002,  3,  5,  7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67,
  071, 73, 79, 83, 89, 97,101,103,107,109,113,127,131,137,139,149,151,157,163,
  167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,
  271,277,281,283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,
  389,397,401,409,419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,
  503,509,521,523,541,547,557,563,569,571,577,587,593,599,601,607,613,617,619,
  631,641,643,647,653,659,661,673,677,683,691,701,709,719,727,733,739,743,751,
  757,761,769,773,787,797,809,811,821,823,827,829,839,853,857,859,863,877,881,
  883,887,907,911,919,929,937,941,947,953,967,971,977,983,991,997
};

// Gaussian distributed random generator. It consumes two even distributed
// generators

double 
normal::operator()()
{
  double fac, r, v1, v2;
  
  if  (iset == 0) {
    do {
      v1 = 2 * (*r1)() - 1;
      v2 = 2 * (*r2)() - 1;
      r = v1 * v1 + v2 * v2;
    } while (r >= 1);
    fac = sqrt(- 2 * log(r) / r);
    gset = v1 * fac;
    iset = 1;
    return v2 * fac;
  } else {
    iset = 0;
    return gset;
  }
}


double
landau::operator()()
{
  static double integral[lambda_n + 1];
  static int init = -1;

  if (init) {
    init = 0;
    integral[0] = 0;
    for (int i=1; i<=lambda_n; i++) integral[i] = integral[i-1] + landauC[i-1];
    for (int i=1; i<=lambda_n; i++) integral[i] /= integral[lambda_n];
  }
  
  double x=0, y = (*rndm)();
  double x1 =0, y1 = 0, x2 = 1, y2 = 1;

  for(int i=0;i<100;i++) {
    x = x1 + (y-y1) / (y2-y1) * (x2-x1);
    if (x2 - x1 <= 1.001 / lambda_n) break;
    int    ix = (int) (x * lambda_n);
    double dx = x - (double) ix / lambda_n; 
    double yneu = integral[ix] 
      + dx * (double) lambda_n *(integral[ix+1]-integral[ix]);
    if(yneu<y) { 
      x1 = x; 
      y1 = yneu; 
    } else {
      x2 = x; 
      y2 = yneu; 
    }
  }
  return lambda_min + x * (lambda_max - lambda_min);
}


/*
int
main (int, char *) 
{
     FILE *test = popen("gs -q - -c quit","w");
  //  FILE *test = fopen("test.ps","w");
  fprintf(test,
	  "10 10 translate 72 0.254 div dup scale \n"
          "0.001 setlinewidth\n"
	  "/dot {moveto 0.001 0.001 rlineto stroke} def\n"
	  );
  SobolSequence r;
 r.init(pow(2,53)-100);
  
  for (int i=0;i<1000000;i++) {
    r.nextValues();
    double y = r();
    if (y>1) printf("%f > 1 !!!\n",y);
    fprintf(test, "%f %f dot\n", r(), .10+y/10);
    fprintf(test, "%f %f dot\n", r(), .20+y/10);
    fprintf(test, "%f %f dot\n", r(), .30+y/10);
    fprintf(test, "%f %f dot\n", r(), .40+y/10);
    fprintf(test, "%f %f dot\n", r(), .50+y/10);
    fprintf(test, "%f %f dot\n", r(), .60+y/10);
    fprintf(test, "%f %f dot\n", r(), .70+y/10);
    fprintf(test, "%f %f dot\n", r(), .80+y/10);
    fprintf(test, "%f %f dot\n", r(), .90+y/10);
    fprintf(test, "%f %f dot\n", r(),1.00+y/10);
    fprintf(test, "%f %f dot\n", r(),1.10+y/10);
    fprintf(test, "%f %f dot\n", r(),1.20+y/10);
    fprintf(test, "%f %f dot\n", r(),1.30+y/10);
    fprintf(test, "%f %f dot\n", r(),1.40+y/10);
    fprintf(test, "%f %f dot\n", r(),1.50+y/10);
    fprintf(test, "%f %f dot\n", r(),1.60+y/10);
    fprintf(test, "%f %f dot\n", r(),1.70+y/10);
  }
  fprintf(test,"showpage");
  fclose(test);

  SobolSequence r1, r2;
  r2.init(100);
  for(int i=0;i<100;i++) r1.nextValues();
  for(int i=0;i<20;i++) {
    r1.nextValues();
    r2.nextValues();
    cout.form("%3d:  %5.3f %5.3f %5.3f   %5.3f %5.3f %5.3f\n",
	      i,r1(),r1(),r1(),r2(),r2(),r2());
  }

  return 0;
}
*/
