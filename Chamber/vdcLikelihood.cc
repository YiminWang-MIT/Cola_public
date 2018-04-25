//
// Created 2014 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Chamber/vdcRegression.cc,v 2.14 2002-11-08 20:27:26 distler Exp $
//
// Implementation of class VdcLikelihood
//

static char rcsid[] =
 "@(#)$Id: vdcRegression.cc 2216 2008-06-13 21:13:47Z distler $";

#include "vdcLikelihood.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>

using namespace std;

inline double square(double x) { return x*x; }

VdcLikelihood::VdcLikelihood(const char *id)
{
  N = 0;
  ident = NULL;
  coord = path = yoffs = times = NULL;
  if (id) {
    ident = new char[strlen(id)+1];
    strcpy(ident, id);
  }
}


VdcLikelihood::VdcLikelihood(VdcLikelihood *a, VdcLikelihood *b,
			     double d, const char *id)
{
  int i;

  N = 0;
  ident = NULL;
  coord = path = yoffs = times = NULL;
  if (id) {
    ident = new char[strlen(id)+1];
    strcpy(ident, id);
  }

  if (a==NULL || b==NULL) return;

  if ((N = a->N + b->N) > 0) {
    coord = new double[N];
    path  = new double[N];
    yoffs = new double[N];
    times = new double[N];

    memcpy(coord, a->coord, a->N*sizeof(double));
    memcpy(path,  a->path,  a->N*sizeof(double));
    memcpy(yoffs, a->yoffs, a->N*sizeof(double));
    memcpy(times, a->times, a->N*sizeof(double));

    memcpy(coord+a->N, b->coord, b->N*sizeof(double));
    memcpy(path+a->N,  b->path,  b->N*sizeof(double));
    for (i=0; i<b->N; i++) yoffs[a->N+i] = b->yoffs[i]+d;
    memcpy(times+a->N, b->times, b->N*sizeof(double));
  }
}

VdcLikelihood::~VdcLikelihood()
{
  reset();
  delete[] ident;
  if (rcsid[0] == '@') return;
}


void
VdcLikelihood::init(int n, float *x, float *y, float *t)
{
  int i;
  if ((N = n) < 1) return;
  coord = new double[n];
  path  = new double[n];
  yoffs = new double[n];
  times = new double[n];
  for (i=0; i<n; i++) {
    coord[i] = x[i];
    path[i]  = y[i];
    yoffs[i] = 0.0;
    times[i] = t[i];
  }
    
  return;
}

void
VdcLikelihood::reset(void)
{
  N = 0;
  delete[] coord;
  delete[] path;
  delete[] yoffs;
  delete[] times;
  coord = path = yoffs = times = NULL;
}


double
VdcLikelihood::x0(void)
{
  return x_;
}

double
VdcLikelihood::dx0(void)
{
  return dx_;
}

double
VdcLikelihood::sl0(void)
{
  return sl_;
}

double
VdcLikelihood::dsl0(void)
{
  return dsl_;
}

double
VdcLikelihood::chi(void)
{
  if (chi2 == FLT_MAX)
    return FLT_MAX;

  if (chi2 < 0)
    return 0;

  return sqrt(chi2);
}

double
VdcLikelihood::chisqr(void)
{
  return chi2;
}

void
VdcLikelihood::print(char *com)
{
  cout << com << "\tN: " << N << endl;
  cout << "\tx0: " << x0() << " +/- " << dx0() << endl
       << "\tsl: " << sl0() << " +/- " << dsl0()
       << endl << endl;
}

int
VdcLikelihood::simplex(double &x, double &sl)
{
  TrackVector vlow(x+0.5, sl-0.01);
  TrackVector vmed(x-0.5, sl-0.01);
  TrackVector vhigh(x, sl+0.01);

  double plow = likelihood(vlow[0], vlow[1]);
  double pmed = likelihood(vmed[0], vmed[1]);
  double phigh = likelihood(vhigh[0], vhigh[1]);

  vsort(vlow, vmed, vhigh, plow, pmed, phigh);

  //  cout << "simplex start" << '\t' << x << '\t' << sl << endl;
  //  cout << vlow[0] << '\t' << vlow[1] << '\t' << plow << endl;
  //  cout << vmed[0] << '\t' << vmed[1] << '\t' << pmed << endl;
  //  cout << vhigh[0] << '\t' << vhigh[1] << '\t' << phigh << endl;

  int times = 0;
  while (times<60 && 1E-12 < area(vlow, vmed, vhigh)) {
    times++;
    double ptry = reflect(-1.0, phigh, vlow, vmed, vhigh);
    // cout << vlow[0] << '\t' << vlow[1] << '\t' << plow << endl;
    // cout << vmed[0] << '\t' << vmed[1] << '\t' << pmed << endl;
    // cout << vhigh[0] << '\t' << vhigh[1] << '\t' << phigh << endl;

    if (ptry < plow) {
      // result is better than the best point - try a factor of 2
      // cout << "reflect 2.0\t" << ptry << endl;
      ptry = reflect(2.0, phigh, vlow, vmed, vhigh);
    } else if (ptry > pmed) {
      // reflected point is worse than second best - try 1dim contraction
      double psave = phigh;
      // cout << "reflect 0.5\t" << ptry << endl;
      ptry = reflect(0.5, phigh, vlow, vmed, vhigh);
      if (ptry >= psave) {
	// cout << "contract\t" << ptry << endl;
	contract(vlow, vmed, vhigh, plow, pmed, phigh);
      }
    }
    vsort(vlow, vmed, vhigh, plow, pmed, phigh);
  }

  x = vlow[0]; sl = vlow[1];

  //  cout << "simplex end" << '\t' << x << '\t' << sl << endl;
  //  cout << "times: " << times
  //       << "\tarea: " << area(vlow, vmed, vhigh) << endl;

  return 0;
}

int
VdcLikelihood::vsort(TrackVector &vl, TrackVector &vm, TrackVector &vh,
		     double &pl, double &pm, double &ph)
{
  if (pl>pm) {
    double p = pm;
    TrackVector v = vm;
    pm = pl; vm = vl; pl = p; vl = v;
  }
  if (pm>ph) {
    double p = pm;
    TrackVector v = vm;
    pm = ph; vm = vh; ph = p; vh = v;
  }
  if (pl>pm) {
    double p = pm;
    TrackVector v = vm;
    pm = pl; vm = vl; pl = p; vl = v;
  }

  return 0;
}

double
VdcLikelihood::area(TrackVector vl, TrackVector vm, TrackVector vh)
{
  TrackVector a = vl - vm;
  TrackVector b = vh - vm;

  return 0.5*fabs(a[0]*b[1]-a[1]*b[0]);
}

int
VdcLikelihood::contract(TrackVector &vl, TrackVector &vm, TrackVector &vh,
			double &pl, double &pm, double &ph)
{
  vh = (vh+vl)*0.5;
  ph = likelihood(vh[0], vh[1]);
  vm = (vm+vl)*0.5;
  pm = likelihood(vm[0], vm[1]);

  return 0;
}

double
VdcLikelihood::reflect(double f, double &ph, TrackVector &vl,
		       TrackVector &vm, TrackVector &vh)
{
  TrackVector m = (vm+vl)*0.5;
  TrackVector n = m+(vh-m)*f;
  double result = likelihood(n[0], n[1]);

  if (result < ph) {
    ph = result;
    vh = n;
  }

  return result;
}

// CorrectionPar[0] is parameter A
// CorrectionPar[1-4] is used for polynomial B
// see diploma thesis of M.Distler page 27f
// http://wwwa1.kph.uni-mainz.de/A1/publications/diploma/distler.ps.gz

const double corrPar[5] = {
  1.1, -0.12458, 0.36502, 0.22858, -0.024383
};

double VdcLikelihood::driftTimeCorrFun(double a)
{
  if (a<0.0) a = -a;
  if ((10.0<a)||(a<0.1)) return 0.0;
  return ((corrPar[4] * a + corrPar[3]) * a + corrPar[2]) * a + corrPar[1];
}

double VdcLikelihood::likelihood(double x, double sl, int debug)
{
  //static double sigma = 0.1;
  static double sigma = 0.2;
  static double nlfact = log(sqrt(8.0*M_PI)*sigma);
  static double efact = -0.5/(sigma*sigma);

  int i;
  double result = 0.0;
  double korrektur = driftTimeCorrFun(sl);

  for (i=0; i<N; i++) {
    double lnp, yc, pathc, pathe; 
    yc = (coord[i]-x)*sl-yoffs[i];
    if (path[i]<corrPar[0]) {
      pathe = pathc = path[i]*(1.0+korrektur/corrPar[0]);
      if (pathe<0.2) pathe = 0.2;
    } else pathc = pathe = path[i]+korrektur;
#ifdef NEVER
    double etmp;
    //if (pathe < 0.7) {
    if (pathe < 1.0) {
      etmp = exp(efact*square(yc-pathe))+exp(efact*square(yc+pathe));
      if (etmp>0.0) {
	lnp = nlfact-log(etmp);
      } else {
	etmp = exp(1E-3*efact*square(yc-pathe))
	  +exp(1E-3*efact*square(yc+pathe));
	lnp = nlfact-1E3*log(etmp);
      }
    } else lnp = nlfact-efact*square(fabs(yc)-pathe);
#else
    lnp = (pathe<0.6
	   ? -1.40927+pathe*(12.5082
			     -pathe*(39.8248-pathe*(54.7657-27.6868*pathe)))
	   : 0.0)+nlfact;
    if ((pathe < 0.8) && (fabs(yc)<(0.8-pathe)/3.)) {
      lnp -= efact*square(4*(0.2-pathe)/3);
    } else lnp -= efact*square(fabs(yc)-pathe);
#endif
    //if (lnp>15000) lnp = 15000;
    if (lnp>1200) lnp = 1200;
    result += lnp;
    if (debug) {
      cout.setf(ios::fixed, ios::floatfield);
      cout << left << setw(7) << (ident ? ident : "likeli")
	   << right << setprecision(2) << setw(7) << coord[i]
	   << right << setw(5) << (int)yoffs[i]
	   << right << setw(10) << yc
	   << right << setw(7) << pathc
	   << right << setw(7) << path[i]
	   << right << setw(9) << lnp
	   << right << setw(9) << x
	   << right << setprecision(4) << setw(9) << sl
	   << endl;
      cout.unsetf(ios::floatfield);
      cout.precision(6);
    }
  }

  return result;
}

double
VdcLikelihood::mean(double sl)
{
  int i, n = 0;
  double sum = 0.0;

  for (i=0; i<N; i++) {
    if (yoffs[i]==0.0) {
      sum += coord[i];
      n++;
    }
  }

  if (n>0) return sum/n;

  for (i=0; i<N; i++) {
    sum += coord[i];
    n++;
  }

  return sum/n-yoffs[0]/sl;
}
  

void
VdcLikelihood::calc(double sl)
{
  int error = 0;

  double meanX = mean(sl);
  double meanSL = sl;
  simplex(meanX, meanSL);

  if (fabs(meanX)  > 5000) { meanX  = 5000*meanX/fabs(meanX); error |= 1; }
  if (fabs(meanSL) <  0.1) { meanSL =  0.1*meanSL/fabs(meanSL); error |= 4; }
  if (fabs(meanSL) > 10.0) { meanSL = 10.0*meanSL/fabs(meanSL); error |= 16; }
  x_ = meanX;
  sl_ = meanSL;
  if (error) {
    chi2 = FLT_MAX;
    return;
  }

  // -mod- debug version
  // double meanLikeli = likelihood(meanX, meanSL, 0);
  double meanLikeli = likelihood(meanX, meanSL, ident!=0);
  chi2 = meanLikeli;

  //  cout << "mean:\t" << meanX << '\t' << meanSL
  //  	    << '\t' << meanLikeli << endl;

  Hessian hessian = NHessian(meanX, meanSL, 0.005, 0.001);

  dx_  = hessian.errorX();
  dsl_ = hessian.errorY();

#ifdef NEVER
  cout << "delta:\t"
       << likelihood(meanX-0.05, meanSL) << '\t'
       << likelihood(meanX, meanSL) << '\t'
       << likelihood(meanX+0.05, meanSL) << endl << "\t"
       << likelihood(meanX, meanSL-0.01) << '\t'
       << likelihood(meanX, meanSL) << '\t'
       << likelihood(meanX, meanSL+0.01) << endl;

  double derivX;
  double derivSL;

  cout << "derivative:\t"
       << derivativeX(meanX, meanSL, 0.05) << '\t'
       << (derivX=minimumX(meanX, meanSL, 0.05)) << endl
       << "\t"
       << derivativeSL(meanX, meanSL, 0.01) << '\t'
       << (derivSL=minimumSL(meanX, meanSL, 0.01)) << endl;

  simplex(derivX, derivSL);
  double derivLikeli = likelihood(derivX, derivSL, 1);

  cout << "deriv:\t" << derivX << '\t' << derivSL
       << '\t' << derivLikeli << endl;

  x_ = derivX;
  sl_ = derivSL;
#endif
}

Hessian VdcLikelihood::NHessian(double x, double sl, double hx, double hsl)
{
  int i, j;
  // remap array indices: -1,0,1
  double ll[9], *l[3] = {ll+1, ll+4, ll+7}, **f = l+1;

  for (i=-1; i<=1; i++)
    for (j=-1; j<=1; j++)
      f[i][j] = likelihood(x+i*hx,sl+j*hsl);
  // 9 evaluations
  // numerical hessian matrix needs 9+7+9=25 points

  // Abramowitz: chapter: Numerical Analysis page: 378
  double xx = (f[1][1]-2*f[0][1]+f[-1][1] + f[1][0]-2*f[0][0]+f[-1][0] +
    f[1][-1]-2*f[0][-1]+f[-1][-1])/(3*hx*hx);
  double xs = -0.5*(f[1][0]+f[-1][0]+f[0][1]+f[0][-1]
    -2*f[0][0]-f[1][1]-f[-1][-1])/(hx*hsl);
  double ss = (f[1][1]+f[0][1]+f[-1][1] -2*(f[1][0]+f[0][0]+f[-1][0]) +
    f[1][-1]+f[0][-1]+f[-1][-1])/(3*hsl*hsl);

  // cout << "hessian\t"
  //      << xx << '\t' << xs << endl << "\t"
  //      << xs << '\t' << ss << endl << endl;

  return Hessian(xx, xs, ss);
}
