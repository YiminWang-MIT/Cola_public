#include "Radiation.h"               // radiation corrections
#include <iostream>
#include "HMBook/hmbook.h"

#include "Landau_g.cc"

static const double alpha   = 1/137.0359895;

double Spence(double x)
{
  const double Bernouilly[19] = { 1,    -1.0/2,  1.0/6,   
				  0,    -1.0/30,     0,     1.0/42,  
				  0,    -1.0/30,     0,     5.0/66,  
				  0,  -691.0/2730,   0,     7.0/6,   
				  0, -3617.0/510,    0, 43867.0/798 };
  if (x>-1 && x<=0.5) { 
    double z = -log(1-x);
    double res = 0;
    double zn = 1;
    double nfac = 1;
    for(int i=0; i<14; i++) {
      zn *= z;
      nfac *= ((double)i + 1);
      res += Bernouilly[i] * zn / nfac;
    }
    return res;
  } 
  if (x ==  1)      return   M_PI*M_PI /  6;
  if (x == -1)      return - M_PI*M_PI / 12;
  if (x>0.5 && x<1) return   M_PI*M_PI /  6 - log(x)*log(1-x) - Spence(1-x);
  return -pow(log(fabs(x)),2)/2 + M_PI*M_PI/ (x>0 ? 3 : -6) - Spence(1/x);
}

//
// pow() is not well installed on Pentium (try, for example, pow(0.1, -800)).
//
double save_pow(double x, double y) {
  if (x==0) return 0;
  double e=y*log(fabs(x));
  if (e<-500) return 0;
  else        return exp(e);
}

//
// Inverse of integrated distribution for external Bremsstrahlung
// energy loss (see Pieroth et al, Nucl. Instr. Meth. B36(1989)263)
//
double InverseBremsInteg(double I, double bt) {
  double n  = 1/(1/bt - 1/(bt+1) + 0.75/(bt+2));
  double u=1e-16;
  if ( I < n * (1/bt - u/(bt+1) + 0.75*u*u/(bt+2)) * save_pow(u, bt) )
    return 0;
  double ua = 0, ub = 1;
  int i=0;
  while ( ub-ua > 1e-15 && i<100) {
    u = (2*ua+ub)/3;   // In average faster convergence than (ua+ub)/2
    double It = n * (1/bt - u/(bt+1) + 0.75*u*u/(bt+2)) * save_pow(u, bt);
    if (I>It) ua=u;
    else      ub=u;
    i++;
  }
  return (ua+ub)/2;
}

void externalBremsstrahlung(Particle& p, const Material *m,
			    double x, Uniform& rndm)
{
  // Previous distribution function bt*u^(bt-1)
  //double exponent = log(rndm()) * 1 / ( x/m->ThicknessBremsstrahlung );
  //double dE = (exponent > -500) ? p.energy() * exp(exponent)
  //  : 0;
  // p -= dE;

  p -= p.energy() * InverseBremsInteg(rndm(), x/m->ThicknessBremsstrahlung);
  // rndm()
}

//
// Extra treatment of internal Bremsstrahlung (electron contributions 
//  only for the moment)
// The equivalent radiator method gives only a rough approximation to the
// internal bremsstrahlung spectrum, neglecting differences in the shape
// of internal and external radiation.
//
void internalBremsstrahlung(Particle& p, const Material *m,
			    Uniform& rndm, double q2)
{
  if (fabs(q2) >0.0001) {
    double inv_t = 1/(alpha/M_PI*(log(fabs(q2)/m_e_sqr)-1));
    p -= p.energy() * save_pow(rndm(), inv_t);
  }
}

double equivalentRadiator(const Material* m, double q2)
{
  return alpha/M_PI*(log(fabs(q2)/m_e_sqr)-1) 
    * m->ThicknessBremsstrahlung;  // t -> cm
}

double vertexCorrection(double q2, double E, double Ep, double theta) 
{
  double res = -28.0/9 + 13.0 / 6 * log(fabs(q2)/m_e_sqr)
    - 0.5 * pow(log(E/Ep),2) - M_PI*M_PI/6 
    + Spence(pow(cos(theta/2),2));
  return exp(alpha / M_PI * res);
}

//char hname_ll[10][30];

void LandauLoss (Particle& p, const Material* m, double x, 
		 Uniform& rndm1, Uniform& rndm2) 
{

  double zeta = m->LandauPar * x / p.beta().square();
  //static landau rndm(&rndm1);  // rndm2 not used
  //double dE = m->dEdx(p, x) - rndm() * zeta;
  double dE = Landau_g(m->dEdx(p, x), zeta, rndm1);

  p += dE;

  //static int check = 2;
  //static int cnt = 1;
  //if (cnt && check) {
  //  check--;
  //  printf("zeta = %g\n", zeta);
  //  sprintf(hname_ll[check], "Landau Z=%f", m->getZ());
  //  HIST *id = HMBook1(hname_ll[check],"dE","#","MeV","", 
  //		       6001, -0.01, 60);
  //  cnt=0;
  //  for(int i=0; i<10000000; i++) {
  //    Particle p2 = p; LandauLoss(p2, m, x, rndm1, rndm2);
  //    HMFill(id, (p.energy() - p2.energy())*1000,0.0,1.0);
  //  }
  //  cnt=1;
  //}
  
}
