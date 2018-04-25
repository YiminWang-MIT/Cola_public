//								      -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/FourVector/Rand.h,v 3.5 2006-06-27 12:33:55 distler Exp $
//
// Header file for random number generators
//
// class definitions taken from GNU g++ library
//

#ifndef __Rand_h__
#define __Rand_h__

#include <stdlib.h>
#include <math.h>

class RNG {
public:
  RNG() { ; }
  virtual ~RNG() { ; }
  virtual long asLong() = 0;
  virtual void reset() = 0;
  float asFloat() { return asDouble(); }
  double asDouble() {
    return double(asLong() & RAND_MAX)/(double(RAND_MAX)+1.0);
  }
};

class ACG : public RNG {
  long initialSeed;
public:
  ACG(long seed = 0, int = 0) { srand(initialSeed = seed); }

  virtual ~ACG() { ; }
  virtual long asLong() { return rand(); }
  virtual void reset() { srand(initialSeed); }
};

class Random {
protected:
  RNG *pGenerator;
public:
  Random(RNG *generator) { pGenerator = generator; }
  virtual ~Random() { ; }
  virtual double operator()() = 0;

  RNG *generator() { return pGenerator; }
  void generator(RNG *p) { pGenerator = p; }
};

class Uniform : public Random {
  double pLow;
  double pHigh;
  double delta;
public:
  Uniform(double low, double high, RNG *gen) : Random(gen) {
    pLow = (low < high) ? low : high;
    pHigh = (low < high) ? high : low;
    delta = pHigh - pLow;
  }
  virtual ~Uniform() { ; }

  double low() { return pLow; }
  double low(double x) {
    double tmp = pLow; pLow = x; delta = pHigh - pLow; return tmp;
  }
  double high() { return pHigh; }
  double high(double x)  {
    double tmp = pHigh; pHigh = x; delta = pHigh - pLow; return tmp;
  }

  double operator()() { return pLow+delta*pGenerator->asDouble(); }
};

class Normal : public Random {
  char haveCachedNormal;
  double cachedNormal;
protected:
  double pMean;
  double pVariance;
  double pStdDev;
public:
  Normal(double xmean, double xvariance, RNG *gen) : Random(gen) {
    pMean = xmean;
    pVariance = xvariance;
    pStdDev = sqrt(pVariance);
    haveCachedNormal = 0;
    cachedNormal = 0;
  }
  virtual ~Normal() { ; }

  double mean() { return pMean; }
  double mean(double x) { double t=pMean; pMean = x; return t; }
  double variance() { return pVariance; }
  double variance(double x) {
    double t=pVariance; pVariance = x; pStdDev = sqrt(pVariance); return t;
  }

  double operator()() {
    if (haveCachedNormal == 1) {
      haveCachedNormal = 0;
      return(cachedNormal * pStdDev + pMean );
    } else {
      for(;;) {
	double u1 = pGenerator -> asDouble();
	double u2 = pGenerator -> asDouble();
	double v1 = 2 * u1 - 1;
	double v2 = 2 * u2 - 1;
	double w = (v1 * v1) + (v2 * v2);
	if (w <= 1) {
	  double y = sqrt( (-2 * log(w)) / w);
	  double x1 = v1 * y;
	  double x2 = v2 * y;
	  haveCachedNormal = 1;
	  cachedNormal = x2;
	  return(x1 * pStdDev + pMean);
	}
      }
    }
  }
};

class Erlang : public Random {
protected:
  double pMean;
  double pVariance;
  int k;
  double a;
  void setState() {
    k = int( (pMean * pMean ) / pVariance + 0.5 );
    k = (k > 0) ? k : 1;
    a = k / pMean;
  }
public:
  Erlang(double mean, double variance, RNG *gen) : Random(gen) {
    pMean = mean; pVariance = variance; setState();
  }
  virtual ~Erlang() { ; }

  double mean() { return pMean; }
  double mean(double x) {
    double tmp = pMean; pMean = x; setState(); return tmp;
  }
  double variance() { return pVariance; }
  double variance(double x) {
    double tmp = pVariance; pVariance = x; setState(); return tmp;
  }

  virtual double operator()() {
    double prod = 1.0;
    for (int i = 0; i < k; i++) prod *= pGenerator -> asDouble();
    return(-log(prod)/a);
  }
};

class LogNormal: public Normal {
protected:
  double logMean;
  double logVariance;
  void setState() {
    double m2 = logMean * logMean;
    pMean = log(m2 / sqrt(logVariance + m2) );
    pStdDev = sqrt(log((logVariance + m2)/m2 )); 
  }
public:
  LogNormal(double mean, double variance, RNG *gen)
    : Normal(mean, variance, gen) {
    logMean = mean; logVariance = variance; setState();
  }
  virtual ~LogNormal() { ; }

  double mean() { return logMean; }
  double mean(double x) {
    double t=logMean; logMean = x; setState(); return t;
  }
  double variance() { return logVariance; }
  double variance(double x) {
    double t=logVariance; logVariance = x; setState(); return t;
  }
  virtual double operator()() { return exp(this->Normal::operator()()); }
};

class NegativeExpntl: public Random {
protected:
  double pMean;
public:
  NegativeExpntl(double xmean, RNG *gen) : Random(gen) { pMean = xmean; }
  virtual ~NegativeExpntl() { ; }

  double mean() { return pMean; }
  double mean(double x) { double t = pMean; pMean = x; return t; }
  virtual double operator()() { return(-pMean*log(pGenerator->asDouble())); }
};

#endif
