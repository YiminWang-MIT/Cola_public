#ifndef __ElasticRadiative_h__
#define __ElasticRadiative_h__

#include "../Generate.h"

class generateElasticRadiative : public eventGenerator{
  // for a test
  PseudoRandom prndm;
protected:
  double BHmin, BHmax, RadCutOff;
public:
  virtual ~generateElasticRadiative() { ; }
  generateElasticRadiative(reaction *r, simDetectorBase *Sime, 
			 simDetectorBase *Sim1, SIMUL *rundb) { 
    Label      = "d[W]'";
    Unit       = "[m]b";
    Reaction   = r; sime=Sime; sim1=Sim1;
    targetmass = Reaction->getTarget()->getMass();
    dcte       = sin(sime->getDtheta());
    dphie      = calc_dphie();
    Ex         = rundb->excitation;
    Gamma      = 1;
    BHmin      = rundb->BHmin;
    BHmax      = rundb->BHmax;
    RadCutOff  = rundb->RadiationCutOff;
  };
  
  double generateEvent(double helicity);
  double integrationVolume() { return 4 * sime->getDphi() * dcte; }
};


#endif
