//                                                                    -*-c++-*-
// $Id: Simul.h 2669 2016-02-17 13:25:10Z schlimme $

#include "Simulation/spinTransfer.h" // spin transfer target -> focal plane
#include "HMBook/hmbook.h"         // HMBook Histogram library
#include "Aqua/AquaTree.h"
#include "Cola/ColaNtuple.h"
#include "Cola/Focus.h"

const double refmomA = 660 / 630.0;
const double refmomB = 1; 
const double refmomC = 490 / 459.0;
const double refmomD = 1;

extern SobolSequence sobol;

class Simul {
private:
  reaction              *Reaction;
  //bss out 2016-02-17  luminosity            *Luminosity;
  struct online         online;
  struct abcn           abcn;

  class Focus            *focus;
  
  static double         events;
  double                accepted;
  class simDetectorBase **sim;

  class simDetectorBase *sime;
  class simDetectorBase *sim1;
  class simDetectorBase *sim2;
  class simDetectorBase *simD1;
  class simDetectorBase *simD2;

  double                BeamEnergy;
  double                BeamMomentum;
  double                setq2;
  int                   steps_beam;
  int                   helicity;
  HIST                  *crossSection;
  class eventGenerator  *generator;
  class CosGenerator    *cosGen;
  int                   chck(Particle &vf, double x[], class Focus *focus);
protected:
  char			*run_db; 
public:
  AquaTree             *out, *atree;

  Cola_Ntuple *nt;

  Simul();

  friend void           signalhandler(int signal);
  int                   eventloop();
  void                  printStatus();
};
