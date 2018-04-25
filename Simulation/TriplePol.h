#ifndef __TriplePol_h__
#define __TriplePol_h__
#include "Generate.h"
#include <cstdlib>

namespace TriplePolNS
{
  extern ifstream infile;
  extern ofstream outfile;
  extern int mode;
  extern float buf[46];
  extern float plusmax, minusmax, unpolmax; 
}

class generateTriplePol : public eventGenerator {
 private:
  double
    dcthe, dphie, dpe, dcth1, dphi1, Eprime, thetae, phie,
    TPx, TPy, TPz, spinpol, XSmax;
  FourVector Target;
  void Photon_rotation(FourVector Photon, FourVector* Proton);
  double S(double E, double p);
 public:
  virtual ~generateTriplePol() { ; }
  generateTriplePol(reaction *r, simDetectorBase *Sime, 
		    simDetectorBase *Sim1, SIMUL *rundb, modeltype ModelType);
  double generateEvent(double helicity);
  double integrationVolume();
  double pwia(FourVector Ein, FourVector Eout, FourVector Proton, FourVector Missing);
  void Output(FourVector Ein, FourVector Eout, FourVector Proton);
};

#endif
