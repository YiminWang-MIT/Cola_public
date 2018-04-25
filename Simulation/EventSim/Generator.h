#ifndef __GENERATOR_H_
#define __GENERATOR_H_
#include "debug.h"

#include <math.h>
#include "Aqua/AquaReader.h"
#include "Cola/QuasiRandom.h"
#include "Cola/Targetlength.h"
#include "Cola/Reactions.h"
#include "Cola/rundatabase.h" 
#include "Cola/include/online/online.h"
#include "Simulation/Generate.h"
#include "Simulation/Colli.h"
#include "Simulation/simDetectorBase.h"
#include "Simulation/simSpecEventSim.h"
#include <string>

#define SPEKA 0
#define SPEKB 1
#define SPEKC 2


const double refmomA = 660 / 630.0;
const double refmomB = 1;

class GenState {
 public:
  bool trigger[3];
  struct TargetCoord tc[3]; //target coord in Spektrometer system
  double TargetCoords[3];// Of Target
  double Wedler[2];
  double Volume;
  double Weight;
  int EventNumber;
};

class Generator {
 protected:
  GenState myState;
  int evnum;

  target *tgt;
  reaction *rea;
  double BeamEnergy;
  double BeamMomentum;
  double simwobx,simwoby;
  double beamoffsetx,beamoffsety;
  double targetoffset_simx,targetoffset_simy;

  double simq2,simq2_range;
  std::string reactionstring;
  eventGenerator *gen;
  simDetectorBase *simA,*simB;
 public:
  Generator(AquaIO *io);
  ~Generator();
  GenState *Generate();
};


extern int verbose;
extern  SobolSequence sobol;
#endif
