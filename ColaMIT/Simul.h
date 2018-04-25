//                                                                    -*-c++-*-
// $Id: Simul.h 2216 2008-06-13 21:13:47Z distler $
//

#ifndef __SIMUL_H__
#define __SIMUL_H__

#include "Simulation/spinTransfer.h" // spin transfer target -> focal plane
#include "Targetlength.h"      // Targetcells
#include "ColaMIT/Aqua_oops1.h"
#include "ColaMIT/Aqua_online.h"
#include "Cola/Reactions.h"
#include "Cola/ColaNtuple.h"
#include "ColaMIT/rundatabase.h"
#include "ColaMIT/Luminosity.h"
#include <setjmp.h>

#include "HMBook/hmbook.h"     // HMBook X11 Histogram library
#ifdef HAVE_WXWIN
#include "HMBook/hmview.h"         // HMBook X11 Histogram Browser
int        work (wxApp *app);
#else
int        work ();
#endif

#define env_def(name,var,default) char *name = (char *)getenv(var);\
                                  if (!name) name= default;
#define env_set(name,var,default) { name = (char *) getenv(var);\
				    if (!name) name= default; }

const int SHOWCNTPAGE  = 4712;
const int SHOWSTATPAGE = 4713;

class MezzoReader;
void       parse_file(AquaTree *, AquaTree *, MezzoReader *,
		      reaction *, char *name,
		      const char *Label="Counts", const char *Unit="");
int        evaluate_expressions(const FourVector& A, const FourVector& B,
				const FourVector& C, const FourVector& D,
				const FourVector& O, const FourVector& H3,
				const FourVector& TOF, const FourVector& NPOL,
				const FourVector& NDET, const FourVector& SI,
				const FourVector& beam,
				const double weight = 1.0);
void       printCounter(ostream *);

target *target = new cryo_cyl;
int   verbose = (getenv("VERBOSE") != NULL);
int   display = ((char *) getenv("DISPLAY") != NULL);
int   ergloss = (getenv("ENERGYLOSS") ? atoi(getenv("ENERGYLOSS")) : 2);
jmp_buf env;

class pdcD;
pdcD *PdcD = NULL;

const double refmomO = 1;
const double refmomA = 1;
const double refmomB = 1; 
const double refmomC = 1;
const double refmomD = 1;

class Simul {
private:
  reaction              *Reaction;
  luminosity            *Luminosity;
  struct oops1          oops1;
  struct online         online;
  
  static double         events;
  double                accepted;
  class simDetectorBase **sim;

  class simDetectorBase *sime;
  class simDetectorBase *sim1;
  class simDetectorBase *sim2;
  class simDetectorBase *simD1;
  class simDetectorBase *simD2;

  SpinMatrix            spinMatrix;
  double                BeamEnergy;
  double                BeamMomentum;
  double                setq2;
  int                   steps_beam;
  int                   helicity;
  HIST                  *crossSection;
  class eventGenerator  *generator;
  class CosGenerator    *cosGen;
    
  int                   chck(Particle &vf, double x[]);
protected:
  char			*run_db; 
public:
  AquaTree              *out, *atree;

  Cola_Ntuple *nt;

  Simul();

  friend void           signalhandler(int signal);
  int                   eventloop();
  void                  printStatus();
};

#endif /* __SIMUL_H__ */
