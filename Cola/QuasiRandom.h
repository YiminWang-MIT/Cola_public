//                                                                    -*-C++-*-
// $RCSfile: QuasiRandom.h,v $
// $Revision: 2216 $ 
// $Date: 2008-06-13 17:13:47 -0400 (Fri, 13 Jun 2008) $ 
// $Author: distler $

#ifndef __QuasiRandom_h__
#define __QuasiRandom_h__

/** @name Random Generators.
 *
 *  These classes provides random generators for different distributions.
 * There are only quasi random numbers and no pseudo random numbers, since
 * we need this for simulation! See Numerical Recipies for the difference.
 *
 * For Sobol' Sequence see e.g. \\
 *    "Numerical Recipes in C"\\
 *    by W.H.Press,S.A.Teukolsky,W.T.Vetterling,B.P.Flannery\\
 *
 * usage:\\
 *    SobolSequence sobol(6);     *initializes a new sobol sequence in 6 dim\\
 *                                *maximum dimension = 6 at the moment \\
 *    sobol(double x[n])          *Next n-dim element of Sobol Sequence\\
 *       or\\
 *    sobol.nextValues();\\
 *    x1=sobol();\\
 *    x2=sobol();\\
 *      ...\\
 *\\
 *    HaltonSequence halton = 3   *New Sequence to the prime number base 3\\
 *    double halton()             *Next Element of Halton Sequence\\
 *
 *    normal ng = normal(new HaltonSequence(2),new HaltonSequence(3))\\
 *    double ng()                 * generates a normal distributed generator\\
 *                                  (two other generators are needed)
 */
//@{
#include <math.h>

const int MAXDIM = 18;
const int MAXBIT = 53;

static const int    lengthUL  = sizeof(unsigned long) * 8;
static const double highvalue = pow(2, (int) (sizeof(unsigned long) * 8));

/// internal class to handle bit operations on 8 byte integers
class lint { 
private:
  unsigned long high, low;
public:
  lint() {};
  lint(unsigned long l, unsigned long h=0) { low=l; high=h; };
  lint(double d) { 
    low  = (unsigned long) fmod(d, highvalue); 
    high = (unsigned long) floor(d/highvalue); 
  };
  lint operator^(const lint &b) const {return lint(low^b.low, high^b.high); };
  lint operator&(const lint &b) const {return lint(low&b.low, high&b.high); };
  lint operator|(const lint &b) const {return lint(low|b.low, high|b.high); };
  lint operator ^= (const lint &b) { low^=b.low; high^=b.high; return *this;};
  lint operator &= (const lint &b) { low&=b.low; high&=b.high; return *this;};
  lint operator |= (const lint &b) { low|=b.low; high|=b.high; return *this;};
  lint operator <<=(const int s)   { return *this = *this << s; };
  lint operator >>= (const int s)  { return *this = *this >> s; };
  lint operator >> (const int s) const {
    if(s==0) return *this;
    return (s < lengthUL ?
	    lint((low  >> s) | (high << (lengthUL-s)), high >> s) : 
	    lint(high  >> (s-lengthUL) , 0));
  };
  lint operator << (const int s) const {
    if(s==0) return *this;
    return (s<lengthUL ?
	    lint(low << s, (high<<s) | (low >> (lengthUL-s))) :
	    lint(0,low <<(s-lengthUL)));
  };
  operator double() {
    const double highvalue = pow(2,lengthUL);
    return highvalue * high + low;
  }
  //  friend ostream& operator<<(ostream& out, lint l) {
  //  for (int i=lengthUL-1; i>=0; i--) out << ((l.high >> i) & 1);
  //  for (int i=lengthUL-1; i>=0; i--) out << ((l.low  >> i) & 1);
  //  return out;
  // }
};

/// virtual class of Random Generators
class RandomGenerator {
public:
  virtual ~RandomGenerator() { ; }
  /// returns the next number of the random sequence 
  virtual double operator()() = 0;
};

/// virtual class for uniformly distributed random generators
class Uniform : public RandomGenerator {
public:
  virtual ~Uniform() { ; }
  virtual double operator()() = 0;
};

/** Pseudo Random Generator.
 *  Random numbers between 0, 1. See Numerical recipes.
 */
class PseudoRandom : public Uniform {
public:
  PseudoRandom();
  virtual ~PseudoRandom() { ; }
  double operator()();
};
/** Sobol-Antonov-Saleev sequences.
 * This class provides Sobol-Antonov-Saleev sequences upto
 * 18 dimensions and with a period of \TEX{$2^{53}$}.
 */
class SobolSequence : public Uniform {
private:
  short dim, mdeg[MAXDIM+1];
  double in;
  lint ix[MAXDIM+1],*iu[MAXBIT+1], ip[19], iv[MAXDIM*MAXBIT+1];
  short  access;
  double *xSet;
public:
  virtual ~SobolSequence() { ; }
  /// Constructor
  SobolSequence(const int d = MAXDIM);
  /// fill the next elements in an array. Use this XOR nextValues() + Op() 
  void   operator()(double x[]);
  /// Calulate next elements and store them internal
  void   nextValues() { access = 0;  (*this)(xSet); }
  /// Return the next internal stored element
  double operator()();
  /// Skip the first (start) values of the sequence
  void   init(double start);
};
  
/** A uniformly distributed quasi random sequence.

    This kine of quasi random sequences is out of date 
    and should'nt be used! The period is small (less than \TEX{$2^{31}$}).
*/
class HaltonSequence : public Uniform {
private:
  unsigned long start, base, maxdigit, *digits;
public:
  virtual ~HaltonSequence() { ; }
  /// Constructor, b has to be a prime number
  HaltonSequence(const int b);
  /// returns the next number of the random sequence
  double operator()();
};

/** A gaussian distributed random generator.
  The distribution:\\
  \TEX{\[
   f(x) = \frac{1}{2\pi}e^{-x^2/2}
   \]}\\
  is generated by the Box-Muller algorithm
*/ 
class normal : public RandomGenerator {
private: 
  int iset;
  double gset;
  Uniform *r1, *r2; 
public:
  virtual ~normal() { ; }
  /// Constructor needs a uniformly [0,1] distributed generator 
  normal(Uniform *rg1, Uniform *rg2) {iset=0;r1=rg1;r2=rg2;};
  /// returns the next number of the random sequence
  double operator()();
};


/** A landau distributed random generator.
    Landau probability density function:\\  
    \TEX{\[
    \phi(\lambda)=\frac{1}\pi\int_0^\infty {e^{-\lambda r-r\ln r}sin(\pi r)}dr
    \]}\\
    Transformation to calculate the inifite integral:\\
    \TEX{\[  \int_0^\infty  f(x) dx = \int_0^1 \frac{f(-\ln t)}{t} dt \]}
*/
class landau : public RandomGenerator {
private:
  Uniform *rndm;
public:
  virtual ~landau() { ; }
  /// Constructor needs a uniformly [0,1] distributed generator 
  landau(Uniform *r) {rndm = r;};
  /// returns the next number of the random sequence
  double operator()();
};

extern HaltonSequence halton[];

/// A Breit-Wigner distributed random generator
class BreitWigner : public RandomGenerator {
private:
  Uniform *rndm;
  double Mass, Width;
public:
  virtual ~BreitWigner() { ; }
  /// Constructor needs a uniformly distributed generator 
  BreitWigner(Uniform *r, double mass, double width) {
    rndm = r; 
    Mass=mass; 
    Width=width;
  };
  /// returns the next number of the random sequence
  double operator()(){ return tan(M_PI * (2*(*rndm)()-1)) * Width/2 + Mass; };
};
//@}

/** This is a  1+A*cos(phi) distributed random generator
 *  slightly slower than the rejection method
 *  classical transformation method used:
 *  inverse(y = integral(1 + A*cos(phi)))
 *  The inverse function can't be calculated analytical, so I use
 *  the taylor expansion of the inverse up to order 3 and
 *  improve the result by four Newton steps
 */
class CosGenerator :  public RandomGenerator {
private:
  Uniform *rndm;
public:
  virtual ~CosGenerator() { ; }
  /// Constructor needs a uniformly distributed generator 
  CosGenerator(Uniform *rndm) {CosGenerator::rndm = rndm;}
  /// returns the next number of the random sequence
  double operator()(){ return this->operator()(1.0);}
  double operator()(double A){
    double x = 2 * M_PI * (*rndm)() - M_PI;
    double phi = x*(1/(1+A) + A*x*x*(1/(6.0*(1+A)*(1+A)*(1+A)*(1+A))));
    for (int i=0;i<4;i++) phi -= (phi+A*sin(phi)-x)/(1+A*cos(phi));
    return phi;
  }
};

#endif /* __QuasiRandom_h__ */
















