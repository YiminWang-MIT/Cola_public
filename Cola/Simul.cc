//
// $Id: Simul.cc 2688 2017-05-09 14:28:23Z schlimme $
//
// Main part of Cola Simulation program "Simul++"
//

#define YWdebug 0
#define _POSIX_C_SOURCE 2
#define _DEFAULT_SOURCE    1

#include "config.h"                  // Created by autoconf

#include "Simulation/Generate.h"          // Event Generator
#include "Simulation/ElasticRadiative/radiative.h"          // Event Generator
#include "Simulation/TriplePol.h"         // TriplePol model
#ifdef __ColaMIT__
#include "ColaMIT/Simul.h"
#else
#include "Cola/Cola.h"
#include "Cola/Simul.h"
#endif
#include "Model/Model.h"             // Model library
#include "Radiation.h"
#include "Simulation/Colli.h"        // Definition of Collimators
#include "Simulation/simDetector.h"        // Class for other detectors
#include "Simulation/simSpectrometer.h"    // Class for each spectrometer
#include "Simulation/simSpecwithTrace.h"   // Class for spectrometer with trace
#include "Simulation/simSiliconDetector.h" // Class for silicon detector
#include "Simulation/simKAOS.h"            // Class for Kaos spectrometer
#include "fpu.h"
#if defined(__GNU_LIBRARY__)
#  include <getopt.h>
#endif
#include <unistd.h>
#include <signal.h>                  // Signal handling for SIGHUP
#include <setjmp.h>
#include <iostream>
#include <typeinfo>
//#include "Cola/RandomFPP.cc"
#include "Model/ChPT_VCS.h"
//int badevents=0;
extern double T1nrsimul; 
extern double T1nrmaxsimul; 

// Ntuple stuff
static int ntuple = 0;
struct pawc {
  int hmemor[HBOOKMEM];
} pawc_;
char *ntvarn[MAX_NT_VAR];   // Scalar name related to ntvar
char *ntdes[MAX_NT_VAR];    // NTvariable name
char *ntbck[MAX_NT_VAR];    // Block name for each ntvar
char *nttitle;              // Ntuple Title
int  ntiden, ntnvar;
int iselastic=false;

// Needed for snow correction (yuck!)
extern Element *E_H1, *E_O16;
 
static modeltype ModelType = Isotropic;
static double  simStart, simEnd, simNormalize, simCharge;
static char *setting, *simulation_his, *simulation_rz;

int scale         = 0;
int inplane       = getenv("INPLANE") != NULL;
int nowidth       = getenv("NOWIDTH") != NULL;
int specres       = getenv("SPECRESOLUTION")?atoi(getenv("SPECRESOLUTION")): 0;
int decay         = getenv("DECAY") ? atoi(getenv("DECAY")) : 0;
double generateW  = getenv("GENERATEW") ?atof(getenv("GENERATEW")) : 0;
double generatep  = getenv("GENERATEP") ?atof(getenv("GENERATEP")) : 0;
double fixedweight= getenv("WEIGHT")    ?atof(getenv("WEIGHT"))    : 0;

HMVFrame *frame;
HMVPage  *cntpage;
HMVPage  *statpage;

SobolSequence sobol;

double 
Simul::events = 0;


Simul::Simul() 
{
  out   = new Aqua_online(&online);

#ifdef __ColaMIT__
  atree = new Aqua_oops1(&oops1); 
  rundb.Ebeam = 0;
  rundb.oopsA.collimator = rundb.oopsB.collimator = rundb.oopsC.collimator
    = rundb.oopsD.collimator = rundb.ohips.collimator = "";
  rundb.oopsA.momentum = rundb.oopsB.momentum = 
  rundb.oopsC.momentum = rundb.oopsD.momentum = rundb.ohips.momentum = 100;
  rundb.reaction = "H(e,O:e'A:p)";
  // defaults for BPM offsets and slopes 
  for (int i=0; i<2; i++) {
    rundb.bpm[i].x0 = 0;
    rundb.bpm[i].dx = 0;
    rundb.bpm[i].y0 = 0;
    rundb.bpm[i].dy = 0;
  }
#else
  atree = new Aqua_abcn(&abcn); 
  rundb.Ebeam = 855;
  rundb.Etarget = 0;
  rundb.sim.wobx = rundb.sim.woby = 3;   // Default: +/- 3 mm if not given 
  rundb.sim.BHmin = rundb.sim.BHmax = 0;
  rundb.A.collimator = rundb.B.collimator = "";
  rundb.C.collimator = rundb.D.collimator = "";
  rundb.kaos.collimator = "";
  rundb.A.momentum = rundb.B.momentum = 
  rundb.C.momentum = rundb.D.momentum = 100;
  rundb.reaction = "H(e,B:e'A:p)";
#endif
  rundb.sim.DecayMass[0]  =    0;  rundb.sim.DecayMass[1]  = HUGE_VAL;
  rundb.sim.DecayTheta[0] =    0;  rundb.sim.DecayTheta[1] = 180;
  rundb.sim.DecayPhi[0]   = -180;  rundb.sim.DecayPhi[1]   = 180;
  rundb.sim.CMSTheta[0]   =    0;  rundb.sim.CMSTheta[1]   = 180;
  rundb.sim.CMSPhi[0]     = -180;  rundb.sim.CMSPhi[1]     = 180;
  rundb.sim.excitation    = 0;
  rundb.sim.RadiationCutOff = 0;
  rundb.sim.maxWeight = 0;
  rundb.sim.TargetPol[0] = 0;
  rundb.sim.TargetPol[1] = 0;
  rundb.sim.TargetPol[2] = 0;
  rundb.target = "";
  
  // Defaults from Thesis Manfred Korn Page 4
  // momResolution p/pref , angularResolution in mrad, vertexresolution in mm

#ifdef __ColaMIT__
  // Defaults; momResolution, angularResolution in mrad, vertexresolution in mm

  rundb.ohips.momResolution = 1E-4; rundb.ohips.angularResolution = 3;
  rundb.oopsA.momResolution = 1E-4; rundb.oopsA.angularResolution = 3;
  rundb.oopsB.momResolution = 1E-4; rundb.oopsB.angularResolution = 3;
  rundb.oopsC.momResolution = 1E-4; rundb.oopsC.angularResolution = 3;
  rundb.oopsD.momResolution = 1E-4; rundb.oopsD.angularResolution = 3;
  rundb.ohips.momResolution2 = 1E-4; rundb.ohips.angularResolution2 = 3;
  rundb.oopsA.momResolution2 = 1E-4; rundb.oopsA.angularResolution2 = 3;
  rundb.oopsB.momResolution2 = 1E-4; rundb.oopsB.angularResolution2 = 3;
  rundb.oopsC.momResolution2 = 1E-4; rundb.oopsC.angularResolution2 = 3;
  rundb.oopsD.momResolution2 = 1E-4; rundb.oopsD.angularResolution2 = 3;
  rundb.ohips.verticalAngleRatio = 1;
  rundb.oopsA.verticalAngleRatio = 1;
  rundb.oopsB.verticalAngleRatio = 1;
  rundb.oopsC.verticalAngleRatio = 1;
  rundb.oopsD.verticalAngleRatio = 1;

  for (int i=0; i<20; i++) {
    rundb.ohips.angularRatio[i] = -1; rundb.ohips.momRatio[i] = -1;
    rundb.oopsA.angularRatio[i] = -1; rundb.oopsA.momRatio[i] = -1;
    rundb.oopsB.angularRatio[i] = -1; rundb.oopsB.momRatio[i] = -1;
    rundb.oopsC.angularRatio[i] = -1; rundb.oopsC.momRatio[i] = -1;
    rundb.oopsD.angularRatio[i] = -1; rundb.oopsD.momRatio[i] = -1;
    rundb.ohips.dpcuts[i] = -100;
    rundb.oopsA.dpcuts[i] = -100;
    rundb.oopsB.dpcuts[i] = -100;
    rundb.oopsC.dpcuts[i] = -100;
    rundb.oopsD.dpcuts[i] = -100;
  }
#else
  // Defaults from Thesis Manfred Korn Page 4
  // momResolution p/pref , angularResolution in mrad, vertexresolution in mm

  rundb.A.momResolution = 1E-4; rundb.A.angularResolution = 3;
  rundb.B.momResolution = 1E-4; rundb.B.angularResolution = 3;
  rundb.C.momResolution = 1E-4; rundb.C.angularResolution = 3;
  rundb.D.momResolution = 1E-4; rundb.D.angularResolution = 3;
  rundb.A.momResolution2 = 1E-4; rundb.A.angularResolution2 = 3;
  rundb.B.momResolution2 = 1E-4; rundb.B.angularResolution2 = 3;
  rundb.C.momResolution2 = 1E-4; rundb.C.angularResolution2 = 3;
  rundb.D.momResolution2 = 1E-4; rundb.D.angularResolution2 = 3;
  rundb.A.verticalAngleRatio = 1;
  rundb.B.verticalAngleRatio = 1;
  rundb.C.verticalAngleRatio = 1;
  rundb.D.verticalAngleRatio = 1;

  for (int i=0; i<20; i++) {
    rundb.A.angularRatio[i] = -1; rundb.A.momRatio[i] = -1;
    rundb.B.angularRatio[i] = -1; rundb.B.momRatio[i] = -1;
    rundb.C.angularRatio[i] = -1; rundb.C.momRatio[i] = -1;
    rundb.D.angularRatio[i] = -1; rundb.D.momRatio[i] = -1;
    rundb.A.dpcuts[i] = -100;
    rundb.B.dpcuts[i] = -100;
    rundb.C.dpcuts[i] = -100;
    rundb.D.dpcuts[i] = -100;
  }

  rundb.A.left = 1;
  rundb.B.left = 0;
  rundb.C.left= 0;

  rundb.A.simChamber=NULL;
  rundb.B.simChamber=NULL;
  rundb.C.simChamber=NULL;

  // Kaos spectrometer default values
  rundb.kaos.momResolution      = 0.1; 
  rundb.kaos.angularResolution  = 2;
  rundb.kaos.angularResolution2 = 2;
  rundb.kaos.acceptance=NULL;

  // Silicon default values
  rundb.si.angle = 90.0;
  rundb.si.distance = 8.0;
  rundb.si.simCentMom = 0.240;
  rundb.si.simAcceptScale = 1.0;
  for (int ii=0;ii<8;ii++) rundb.si.simResolution[ii] = 0.0;
#endif
  
  rundb.Target.externalRadiation = 15; // is interpreted as bitmask integer
  //rundb.Target.internalRadiation =  1;
  rundb.Target.internalRadiation =  15;
  rundb.sim.GeneratorFlag =  0;

  rundb.Target.vertexresolution = 1.0;
  rundb.Target.vertexresolution2 = 1.0;  
  rundb.Target.vertexratio = 0.0;  
  rundb.use_vertex = "automatic";
  rundb.Target.vertexdistribution = 0.0;

  rundb.B.collimatoracceptance = 0.0;

  rundb.Target.length = rundb.Target.angle = rundb.Target.density = 
    rundb.Target.wallthickness = rundb.Target.snowthickness = 0;
  rundb.Target.snowdensity = 0.4;
  rundb.Target.offset.x = 0;
  rundb.Target.offset.y = 0;
  rundb.Target.offset_sim.x = 0;
  rundb.Target.offset_sim.y = 0;
  rundb.TargetpositionIsKnown = 0;
  rundb.Target.Polarisation[0] = 0;
  rundb.Target.Polarisation[1] = 0; 
  rundb.Target.Polarisation[2] = 0;
  rundb.Target.wallparam[0] = 0;
  rundb.Target.wallparam[1] = 0;
  rundb.Target.wallparam[2] = 0;

  env_set(run_db, "RUN_DB", "run.db");
  readRunDatabase(run_db, NULL, setting, 0);
  
  if (rundb.Target.snowdensity != 0.4) {
    printf("RunDB: Snow Density %f (not 0.4), correcting...\n", 
	   rundb.Target.snowdensity);
    delete Snow;
    Snow = new Compound( rundb.Target.snowdensity, *E_H1, 2, *E_O16, 1);
  }
  
  if (!rundb.TargetpositionIsKnown) 
    rundb.Target.offset_sim = rundb.Target.offset;

  BeamEnergy   = rundb.Ebeam / 1e3;
  BeamMomentum = sqrt(BeamEnergy*BeamEnergy - m_electron*m_electron);
  setq2        = rundb.sim.q2;
  steps_beam   = 1;
  helicity     = 1; 
  if (rundb.beam.P_e) helicity = 1;       // I couldn't see any reason why it
  else helicity = 0;                      // should by 1 all the time:
                                          // changed by P.A. on 31.10.2011
                                          // It is not. It is swapped from event to event.. anyhow

#ifdef __ColaMIT__
  int Ondp=0, Andp=0, Bndp=0, Cndp=0, Dndp=0;
  for (int i=0; i<20; i++) {
    if (rundb.ohips.angularRatio[i] != -1 &&
	rundb.ohips.momRatio[i]     != -1 &&
	rundb.ohips.dpcuts[i]       != -100) Ondp++;
    if (rundb.oopsA.angularRatio[i] != -1 &&
	rundb.oopsA.momRatio[i]     != -1 &&
	rundb.oopsA.dpcuts[i]       != -100) Andp++;
    if (rundb.oopsB.angularRatio[i] != -1 &&
	rundb.oopsB.momRatio[i]     != -1 &&
	rundb.oopsB.dpcuts[i]       != -100) Bndp++;
    if (rundb.oopsC.angularRatio[i] != -1 &&
	rundb.oopsC.momRatio[i]     != -1 &&
	rundb.oopsC.dpcuts[i]       != -100) Cndp++;
    if (rundb.oopsD.angularRatio[i] != -1 &&
	rundb.oopsD.momRatio[i]     != -1 &&
	rundb.oopsD.dpcuts[i]       != -100) Dndp++;
  }
#else
  int Andp=0, Bndp=0, Cndp=0, Dndp=0;
  for (int i=0; i<20; i++) {
    if (rundb.A.angularRatio[i] != -1 &&
	rundb.A.momRatio[i]     != -1 &&
	rundb.A.dpcuts[i]       != -100) Andp++;
    if (rundb.B.angularRatio[i] != -1 &&
	rundb.B.momRatio[i]     != -1 &&
	rundb.B.dpcuts[i]       != -100) Bndp++;
    if (rundb.C.angularRatio[i] != -1 &&
	rundb.C.momRatio[i]     != -1 &&
	rundb.C.dpcuts[i]       != -100) Cndp++;
    if (rundb.D.angularRatio[i] != -1 &&
	rundb.D.momRatio[i]     != -1 &&
	rundb.D.dpcuts[i]       != -100) Dndp++;
  }
#endif

  // select reaction and target
  
  Reaction = new reaction(rundb.reaction);  
  Reaction->electronIn.setMomentum(0, 0, BeamMomentum);
  target   = SetTargetFromRunDB(Reaction);
  Reaction->setTargetMomentum(rundb.Etarget*1E-3);
  // used only to calculate material factor --> taken out 2016-02-17 (bss)
  //Luminosity   = new luminosity(NULL, target, Reaction, 1);
  
  // init model

  if (ModelType == Elastic) {
    env_def(crosssectionfile, "CROSSSECTION", "CrossSection.his");
    crossSection = HMScanFile(crosssectionfile, "Cross Section");
  }

  // init detectors
  sim = new simDetectorBase*[12];

#ifdef __ColaMIT__
  sim[0] = new simSpectrometer(this->out, - rundb.oopsA.angle * rad, 
			       rundb.oopsA.angularResolution  / 1000,
			       rundb.oopsA.angularResolution2 / 1000,
			       rundb.oopsA.verticalAngleRatio,
			       rundb.oopsA.momentum / 1e3, 
			       rundb.oopsA.momResolution,
			       rundb.oopsA.momResolution2, 
			       Andp, rundb.oopsA.dpcuts,
			       rundb.oopsA.angularRatio, rundb.oopsA.momRatio, 
			       refmomA, 30/2.0, rundb.oopsA.collimator,
			       ColliOopsAList,
			       (int) rundb.oopsA.vacuum, Reaction->getA(),
			       &online.oopsA.Eloss_corr,
			       &online.oopsA.Eloss_sim,
			       0.012*1.1, 0.025*1.1, 0.15*1.1,
			       - rundb.oopsA.oop_angle * rad, decay);

  sim[1] = new simSpectrometer(this->out, - rundb.oopsB.angle * rad, 
			       rundb.oopsB.angularResolution  / 1000,
			       rundb.oopsB.angularResolution2 / 1000,
			       rundb.oopsB.verticalAngleRatio,
			       rundb.oopsB.momentum / 1e3, 
			       rundb.oopsB.momResolution,
			       rundb.oopsB.momResolution2,
			       Bndp, rundb.oopsB.dpcuts,
			       rundb.oopsB.angularRatio, rundb.oopsB.momRatio, 
			       refmomB, 30/2.0, rundb.oopsB.collimator,
			       ColliOopsBList,
			       (int)rundb.oopsB.vacuum, Reaction->getB(),
			       &online.oopsB.Eloss_corr,
			       &online.oopsB.Eloss_sim,
			       0.012*1.1, 0.025*1.1, 0.15*1.1,
			       - rundb.oopsB.oop_angle * rad, decay);

  sim[2] = new simSpectrometer(this->out, - rundb.oopsC.angle * rad, 
			       rundb.oopsC.angularResolution  / 1000,
			       rundb.oopsC.angularResolution2 / 1000,
			       rundb.oopsC.verticalAngleRatio,
			       rundb.oopsC.momentum / 1e3, 
			       rundb.oopsC.momResolution,
			       rundb.oopsC.momResolution2,
			       Cndp, rundb.oopsC.dpcuts,
			       rundb.oopsC.angularRatio, rundb.oopsC.momRatio, 
			       refmomC, 30/2.0, rundb.oopsC.collimator,
			       ColliOopsCList,
			       (int)rundb.oopsC.vacuum, Reaction->getC(),
			       &online.oopsC.Eloss_corr,
			       &online.oopsC.Eloss_sim,
			       0.012*1.1, 0.025*1.1, 0.15*1.1,
			       - rundb.oopsC.oop_angle * rad, decay);

  sim[3] = new simSpectrometer(this->out, - rundb.oopsD.angle * rad, 
			       rundb.oopsD.angularResolution  / 1000,
			       rundb.oopsD.angularResolution2 / 1000,
			       rundb.oopsD.verticalAngleRatio,
			       rundb.oopsD.momentum / 1e3, 
			       rundb.oopsD.momResolution,
			       rundb.oopsD.momResolution2,
			       Dndp, rundb.oopsD.dpcuts,
			       rundb.oopsD.angularRatio, rundb.oopsD.momRatio, 
			       refmomD, 30/2.0, rundb.oopsD.collimator,
			       ColliOopsDList,
			       (int)rundb.oopsD.vacuum, Reaction->getD(),
			       &online.oopsD.Eloss_corr,
			       &online.oopsD.Eloss_sim,
			       0.012*1.1, 0.025*1.1, 0.15*1.1,
			       - rundb.oopsD.oop_angle * rad, decay);

  sim[4] = new simSpectrometer(this->out, - rundb.ohips.angle * rad, 
			       rundb.ohips.angularResolution  / 1000,
			       rundb.ohips.angularResolution2 / 1000,
			       rundb.ohips.verticalAngleRatio,
			       rundb.ohips.momentum / 1e3, 
			       rundb.ohips.momResolution,
			       rundb.ohips.momResolution2, 
			       Ondp, rundb.ohips.dpcuts,
			       rundb.ohips.angularRatio, rundb.ohips.momRatio, 
			       refmomO, 14/2.0, rundb.ohips.collimator,
			       ColliOhipsList,
			       (int) rundb.ohips.vacuum, Reaction->getO(),
			       &online.ohips.Eloss_corr,
			       &online.ohips.Eloss_sim,
			       0.020*1.1, 0.070*1.1, 0.07*1.1, 0.0, decay);
  sim[5] = new simDummy(this->out, Reaction->getH3(), 0, 0, 0, 0, 0, 0);
  sim[6] = new simDummy(this->out, Reaction->getTOF(), 0, 0, 0, 0, 0, 0);
  sim[7] = new simDummy(this->out, Reaction->getNPOL(), 0, 0, 0, 0, 0, 0);
  sim[8] = new simDummy(this->out, Reaction->getNDET(), 0, 0, 0, 0, 0, 0);
  sim[9] = new simDummy(this->out, Reaction->getSI(), 0, 0, 0, 0, 0, 0);
  sim[10] = new simDummy(this->out, Reaction->getKAOS(), 0, 0, 0, 0, 0, 0);

#else
  // Spectrometer A
  /*  For Wire-dependendant cuts. 
  sim[0] = new simSpecwithTrace(this->out, - rundb.A.angle * rad, 
			       rundb.A.angularResolution  / 1000,
			       rundb.A.angularResolution2 / 1000,
			       rundb.A.verticalAngleRatio,
			       rundb.A.momentum / 1e3, 
			       rundb.A.momResolution, rundb.A.momResolution2, 
			       Andp, rundb.A.dpcuts,
			       rundb.A.angularRatio, rundb.A.momRatio, 
			       refmomA, 20/2.0, rundb.A.collimator, ColliAList,
			       (int) rundb.A.vacuum, Reaction->getA(),
			       &online.A.Eloss_corr, &online.A.Eloss_sim,
			       0.110, 0.08, 0.10*1.2, rundb.A.oopangle * rad);
  */

  sim[0] = new simSpectrometer(this->out, - rundb.A.angle * rad, 
			       rundb.A.angularResolution  / 1000,
			       rundb.A.angularResolution2 / 1000,
			       rundb.A.verticalAngleRatio,
			       rundb.A.momentum / 1e3, 
			       rundb.A.momResolution, rundb.A.momResolution2, 
			       Andp, rundb.A.dpcuts,
			       rundb.A.angularRatio, rundb.A.momRatio, 
			       refmomA, 20/2.0, rundb.A.collimator, ColliAList,
			       (int) rundb.A.vacuum, Reaction->getA(),
			       &online.A.Eloss_corr, &online.A.Eloss_sim,
			       0.110, 0.08, 0.10*1.2, rundb.A.oopangle * rad,
			       decay,rundb.A.simChamber);

  // Spectrometer B
  if (rundb.B.left) rundb.B.angle  = -rundb.B.angle;

  double Bdphi   = 0.025;
  double Bdtheta = 0.080;
  double Bpacc   = 7.5;
  double Bdp     = 0.075 * 1.2;


  if (!strncmp(rundb.B.collimator, "(", 1)) {

    double horiL = atof(rundb.B.collimator+1);
    double horiR = atof(rundb.B.collimator+5);
    double vertT = atof(rundb.B.collimator+11);
    double vertB = atof(rundb.B.collimator+15);
    Bdphi   = (horiL>horiR?horiL:horiR)/1000.0 + 0.005; 
    Bdtheta = (vertT>vertB?vertT:vertB)/1000.0 + 0.005;
  }

  if (!strncmp(rundb.B.collimator, "HES", 1)) {
    Bdphi   = 200/1000.0 + 0.005; // +- 11.4 deg
    Bdtheta = 30/1000.0 + 0.005;  // +-  1.7 deg
    Bpacc   = 8; // 0.75 GeV ... 0.91 GeV
    Bdp     = 0.08 * 1.2;
    rundb.B.simChamber = NULL;
  }

  //--- This few lines come handy, when we want to detect only hadron in A
  // without following the electron in B. Then, the 4pi collimator needs
  // to be engaged for B, but it is not enough just to make the collimator,
  // acceptance larger. The events also need to be thrown into larger angle, 
  // ie, to 4pi.
  if (!strncmp(rundb.B.collimator, "4pi", 1)) {
    Bdphi   = 3.141592654; // +- 180.0 deg
    Bdtheta = 3.141592654/2.0;  // +-  90.0 deg
    Bpacc   = 1000; // 1000% acceptance
    Bdp     = 10.0 * 1.2;
  }

  if (rundb.B.collimatoracceptance != 0)
    { 
      Bpacc = rundb.B.collimatoracceptance;
      Bdp = rundb.B.collimatoracceptance / 100 * 1.2;
    }
  
  sim[1] = new simSpectrometer(this->out, rundb.B.angle * rad, 
			       rundb.B.angularResolution  / 1000,
			       rundb.B.angularResolution2 / 1000,
			       rundb.B.verticalAngleRatio,
			       rundb.B.momentum / 1e3, 
			       rundb.B.momResolution, rundb.B.momResolution2,
			       Bndp, rundb.B.dpcuts,
			       rundb.B.angularRatio, rundb.B.momRatio, 
			       refmomB, Bpacc, rundb.B.collimator, ColliBList,
			       (int)rundb.B.vacuum, Reaction->getB(),
			       &online.B.Eloss_corr, &online.B.Eloss_sim,
			       Bdphi, Bdtheta, Bdp,
			       - rundb.B.oopangle * rad,
			       decay,rundb.B.simChamber);
  // Spectrometer C
  sim[2] = new simSpectrometer(this->out, rundb.C.angle * rad, 
			       rundb.C.angularResolution  / 1000,
			       rundb.C.angularResolution2 / 1000,
			       rundb.C.verticalAngleRatio,
			       rundb.C.momentum / 1e3, 
			       rundb.C.momResolution, rundb.C.momResolution2,
			       Cndp, rundb.C.dpcuts,
			       rundb.C.angularRatio, rundb.C.momRatio, 
			       refmomC, 25/2.0, rundb.C.collimator, ColliCList,
			       (int)rundb.C.vacuum, Reaction->getC(),
			       &online.C.Eloss_corr, &online.C.Eloss_sim,
			       0.110, 0.08, 0.125*1.2, rundb.C.oopangle * rad,
                               decay,rundb.C.simChamber);
  // Spectrometer D
  sim[3] = new simSpectrometer(this->out, rundb.D.angle * rad, 
			       rundb.D.angularResolution  / 1000,
			       rundb.D.angularResolution2 / 1000,
			       rundb.D.verticalAngleRatio,
			       rundb.D.momentum / 1e3, 
			       rundb.D.momResolution, rundb.D.momResolution2,
			       Dndp, rundb.D.dpcuts,
			       rundb.D.angularRatio, rundb.D.momRatio, 
			       refmomD, 20/2.0, rundb.D.collimator, ColliDList,
			       (int)rundb.D.vacuum, Reaction->getD(),
			       &online.D.Eloss_corr, &online.D.Eloss_sim,
			       0.13, 0.06, 0.13, rundb.D.oopangle * rad,
                               decay);
  // Spectrometer O
  sim[4] = new simDummy(this->out, Reaction->getO(), 0, 0, 0, 0, 0, 0);
  // Hadron3
  sim[5] = new simDummy(this->out, Reaction->getH3(), 0, 0, 0, 0, 0, 0);
  // ToF
  sim[6] = new simDummy(this->out, Reaction->getTOF(), 0, 0, 0, 0, 0, 0);
  // NPol
  sim[7] = new simNpol(this->out, Reaction->getNPOL(), 0, 0, 0, 0, 0, 0);

  sim[8] = new simNdet(this->out, Reaction->getNDET(),
		       rundb.N.angle, rundb.N.oop_angle, rundb.N.distance,
		       rundb.N.height, rundb.N.width, rundb.N.filename);

  // Silicon
  sim[9] = new simSiliconDetector(this->out,Reaction->getSI(),
				  rundb.si.angle*rad,0,
				  rundb.si.distance,rundb.si.simCentMom,
				  rundb.si.simResolution,
				  rundb.si.simAcceptScale);

  // Kaos spectrometer
  sim[10] = new simKAOS(this->out, rundb.kaos.angle * rad, 
			rundb.kaos.angularResolution, // theta resolution (mrad)
			rundb.kaos.angularResolution2, // phi resolution (mrad)
			rundb.kaos.momentum / 1000, // momentum 
		        rundb.kaos.momResolution, // rel. momentum resolution
			rundb.kaos.collimator, ColliKAOSList,
			(int)rundb.kaos.vacuum, Reaction->getKAOS(),// particle
			&online.kaos.Eloss_corr, &online.kaos.Eloss_sim,
			M_PI/180, // theta angular acceptance
			2*M_PI/180, // phi angular acceptance
			0.5 // relative momentum acceptance (p/ref)
			);

#endif
  sim[11] = NULL;

  for(int i=0; sim[i]; i++) {
    if (Reaction->get(i) == &Reaction->electronOut) sime  = sim[i];
    if (Reaction->get(i) == &Reaction->Out1)        sim1  = sim[i];
    if (Reaction->get(i) == &Reaction->Out2)        sim2  = sim[i];
    if (Reaction->get(i) == &Reaction->Decay1)      simD1 = sim[i];
    if (Reaction->get(i) == &Reaction->Decay2)      simD2 = sim[i];
  }

  //    cout <<Reaction->getM4() << " " << Reaction->getM3() << " " << Reaction->getM2() << endl;

  if (ModelType == ElasticProton) {
    iselastic=true;
    rundb.Target.internalRadiation = 0; // done by generator
    generator = new generateBremsstrahlung(Reaction, sime, sim1, &rundb.sim);
  } else if (ModelType == ElasticRadiative) {//Yimin Wang 2018/07/09 
    iselastic=true;
    rundb.Target.internalRadiation = 0; // done by generator
    generator = new generateElasticRadiative(Reaction, sime, sim1, &rundb.sim);
  } else if (ModelType == ElasticDipole) {
    iselastic=true;
    generator = new generateElasticProton(Reaction, sime, sim1, &rundb.sim);
    //plotBHBorn(0.855, 0.600, 0.1115);
  } else if (ModelType == ElasticPolElectronPolProton) {
    iselastic=true;
    generator = new generateElasticPolElectronPolProton(Reaction, sime, sim1, &rundb.sim);
    //plotBHBorn(0.855, 0.600, 0.1115);
    /////////////////////////////////////////////////////////////
    /////////ElasticNucleus und QuasiElasticNucleus von Jan//////
    ////////////////////////////////////////////////////////////
  } else if (ModelType == ElasticNucleus)      {
    rundb.Target.internalRadiation=0; //done by generator
    generator = new generateElasticNucleus(Reaction, sime, sim1, &rundb.sim);

  } else if (ModelType == QuasiElasticNucleus)      {
    rundb.Target.internalRadiation=0; //done by generator
    generator = new generateQuasiElasticNucleus(Reaction, sime, sim1, &rundb.sim);


    /////////////////////////////////////////////////////////////
    ///ElasticNucleus und QuasiElasticNucleus von Jan Ende///////
    ////////////////////////////////////////////////////////////
   
  } else if (ModelType == ElasticNitrogen) {
    rundb.Target.internalRadiation=0; //done by generator
    iselastic=true;
    generator = new generateElasticNitrogen(Reaction, sime, sim1, &rundb.sim); 
  } else if (ModelType == ElasticNuclearPA) {
    rundb.Target.internalRadiation=0; //done by generator
    iselastic=true;
    generator = new generateElasticPeakingApprox(Reaction, sime, sim1, &rundb.sim); 
  } else if (ModelType == ElasticHavar) {
    rundb.Target.internalRadiation=0; //done by generator
    iselastic=true;
    generator = new generateElasticHavar(Reaction, sime, sim1, &rundb.sim); 
  } else if (ModelType == ElasticCryogens) {
    rundb.Target.internalRadiation=0; //done by generator
    iselastic=true;
    generator = new generateElasticCryogens(Reaction, sime, sim1, &rundb.sim);        
  } else if (ModelType == ElasticDeuteron) {
    iselastic=true;
    if (rundb.sim.GeneratorFlag!=1)
      rundb.Target.internalRadiation=0; //usually done by generator; whereas for generatorflag==1, the standard internal radiation procedure is used instead
    generator = new generateElasticDeuteron(Reaction, sime, sim1, &rundb.sim);
    std::cerr << "rundb.sim.GeneratorFlag=" << rundb.sim.GeneratorFlag << std::endl;
    std::cerr << "rundb.Target.internalRadiation=" << rundb.Target.internalRadiation << std::endl;
  } else if (ModelType == Mott){
    iselastic=true;
    generator = new generateMott(Reaction, sime, sim1, &rundb.sim);
  } else if (ModelType == Inclusive) {
    generator = new generateInclusive(Reaction, sime, &rundb.sim, ModelType);
  } else if (ModelType == TripleLab || ModelType == TripleLabPP || ModelType == TripleLabHe3)
    generator = new generateTriple(Reaction, sime, simD1, simD2,
				   &rundb.sim, ModelType);
  else if (ModelType == He3eepn) 
    generator = new generateHe3eepn(Reaction, sime, simD1, simD2,
				    &rundb.sim, ModelType);
  else if (ModelType == He3nr)
    generator = new generateHe3nr(Reaction, sime, simD1, simD2,
				   &rundb.sim, ModelType);
  else if (ModelType == He3fast)
    generator = new generateHe3fast(Reaction, sime, simD1, simD2,
				    &rundb.sim, ModelType);
  else if (ModelType == DMBoson)
    generator = new generateDMBoson(Reaction, &rundb.sim);
  else if (ModelType == DMQEDBackground)
    generator = new generateDMQEDBackground(Reaction, &rundb.sim);
  else if (Reaction->getM4())
    generator = new generateResonance(Reaction, sime, sim1, simD1, simD2, 
				      &rundb.sim, ModelType);
  else if (Reaction->getM3())
    generator = new generateTripleCMS(Reaction, sime, sim1, simD1, simD2, 
				      &rundb.sim);
  else if (Reaction->getM2())
    switch (ModelType) {
    case Iso6D:   generator=new generateIsotropic6D(Reaction,sime,sim1, 
						    &rundb.sim,
						    ModelType); break;
    case DBreakup:   generator=new generateDeuteronBreakup(Reaction,sime,sim1, 
							   &rundb.sim,
							   ModelType); break;
    case TwoBodyPWIA:
    case TwoBodyHe3:   generator=new generateTwoBodyPWIA(Reaction,sime,sim1, 
							 &rundb.sim,
							 ModelType); break;
    case TriplePol:    generator=new generateTriplePol(Reaction,sime,sim1, 
						       &rundb.sim,
						       ModelType); break;
    case DMProton:     generator=new generateDMProton(Reaction,sime,sim1,sim2, 
						      &rundb.sim); break;
    default:           generator=new generateTwoBody(Reaction,sime,sim1,sim2, 
					       &rundb.sim, ModelType, scale);
    }
  else 
    switch (ModelType) {
    case ThreeBodyPWIA:
    case ThreeBodyHe3: generator=new generateThreeBodyPWIA(Reaction,sime,sim1, 
							   &rundb.sim, 
							   ModelType); break;
    default:
      iselastic = true;
      generator = (rundb.Etarget > 0.0) ?
	(eventGenerator *) new generateElasticCollider(Reaction, sime, sim1,
						       &rundb.sim) :
	(eventGenerator *) new generateElastic(Reaction, sime, sim1,
					       &rundb.sim);
    }

  //Read information about TriplePolFile (max cross sections)
  // if (ModelType == TriplePol && TriplePolNS::mode==4) {
  //   float tmp[16];
  //   TriplePolNS::infile.read((char *) tmp, sizeof (float)*16);
  //   if (TriplePolNS::infile.bad())
  //     cerr << "Could not read input file!" << endl;
  //   TriplePolNS::plusmax  = tmp[1];
  //   TriplePolNS::minusmax = tmp[2];
  //   TriplePolNS::unpolmax = tmp[3];
  // }

  if (simCharge != 0) { 
    generator->Label = ""; 
    generator->Unit = new char [255];
    sprintf(generator->Unit, "Counts/%.1fmC", simCharge);
  }
  
  // parse histogram definitions

  env_def(simulation_col, "SIMUL_COL", "Simulation.col");
  parse_file(atree, out, NULL, Reaction, simulation_col,
	     generator->Label, generator->Unit);

  for (HIST *id=HMFirst; id; id=id->next) {
    HMBookVariance(id);
    HMSetScale(id, 1/simNormalize);
  }

  // Cleans ntuples
  nt->delete_all();

  // Create ntuple
  if (ntnvar) nt->InitNtuple(ntiden, nttitle, ntnvar, ntvarn,
			     ntdes, ntbck, simulation_rz);

  // Checks existence of ntuple
  if ( nt->head ) { 
    nt = nt->head;
    ntuple = nt->nelt;
  } else ntuple = 0;

  cosGen = new CosGenerator(new PseudoRandom());  
  events = accepted = 0;
}

int
Simul::chck(Particle &vf, double x[]) 
{
 
#ifdef __ColaMIT__
  if (Reaction->getA()==&vf) {
    if (!sim[0]->check(vf, x, target, &online.oopsA.target, NULL)) return 0;
    online.oopsA.trigger=1; out->packEventData(&online.oopsA.trigger,1);
  }
  if (Reaction->getB()==&vf) {
    if (!sim[1]->check(vf, x, target, &online.oopsB.target, NULL)) return 0;
    online.oopsB.trigger=1; out->packEventData(&online.oopsB.trigger,1);
  }
  if (Reaction->getC()==&vf) {
    if (!sim[2]->check(vf, x, target, &online.oopsC.target, NULL)) return 0;
    online.oopsC.trigger=1; out->packEventData(&online.oopsC.trigger,1);
  }
  if (Reaction->getD()==&vf) {
    if (!sim[3]->check(vf, x, target, &online.oopsD.target, NULL)) return 0;
    online.oopsD.trigger=1; out->packEventData(&online.oopsD.trigger,1);
  }
  if (Reaction->getO()==&vf) {
    if (!sim[4]->check(vf, x, target, &online.ohips.target, NULL)) return 0;
    online.ohips.trigger=1; out->packEventData(&online.ohips.trigger,1);
  }
#else
  //    cout << "Incoming : " << vf << endl;
  // if (Reaction->getA()) cout << "Check A : " << *Reaction->getA() << "=" << vf << endl;
  if (Reaction->getA()==&vf) {
    
    if (0==sim[0]->check(vf, x, target, &online.A.target, &online.A.simfp, ModelType)) 
       return 0;
    online.A.trigger=1; out->packEventData(&online.A.trigger,1); return 1;
  }
  //  if (Reaction->getB()) // cout << "Check B : " << *Reaction->getB() << "=" << vf << endl;
  if (Reaction->getB()==&vf) {
    if (!sim[1]->check(vf, x, target, &online.B.target, &online.B.simfp, ModelType)) 
      return 0;
    online.B.trigger=1; out->packEventData(&online.B.trigger,1); return 1;
  }
  //  if (Reaction->getC()) cout << "Check C : " << *Reaction->getC() << "=" << vf << endl;
  if (Reaction->getC()==&vf) {
    if (!sim[2]->check(vf, x, target, &online.C.target, &online.C.simfp, ModelType)) 
      return 0;
    online.C.trigger=1; out->packEventData(&online.C.trigger,1); return 1;
  }
  //  if (Reaction->getD()) cout << "Checke D : " << *Reaction->getD() << "=" << vf << endl;
  if (Reaction->getD()==&vf) {
    if (!sim[3]->check(vf, x, target, &online.D.target, NULL, ModelType)) return 0;
    online.D.trigger=1; out->packEventData(&online.D.trigger,1); return 1;
  }
  //  if (Reaction->getNPOL()) cout << "Chekce NPOL : " << *Reaction->getNPOL() << "=" << vf << endl;
  if (Reaction->getNPOL()==&vf) 
    return sim[7]->check(vf, x, target, NULL, NULL, ModelType);
  //  if (Reaction->getNDET()) cout << "Checke NDET : " << *Reaction->getNDET() << "=" << vf << endl;
  if (Reaction->getNDET()==&vf) 
    return sim[8]->check(vf, x, target, NULL, &online.N.simfp, ModelType); 
  //  if (Reaction->getSI()) cout << "Checke SI : " << *Reaction->getSI() << "=" << vf << endl;
  if (Reaction->getSI()==&vf){ 
      online.si.EnergyParticle=vf.energy(); out->packEventData(&online.si.EnergyParticle,1);
      online.si.MomentumParticle=vf.momentum(); out->packEventData(&online.si.MomentumParticle,1);
      online.si.ThetaParticle=vf.theta(); out->packEventData(&online.si.ThetaParticle,1);
      online.si.PhiParticle=vf.phi(); out->packEventData(&online.si.PhiParticle,1);
      if(!sim[9]->check(vf, x, target, NULL, NULL, ModelType)) return 0;
      online.si.OK=1; out->packEventData(&online.si.OK,1); return 1;
  }
  
  //      cout << "Check of Kaos acceptance" << endl;
  if (Reaction->getKAOS()==&vf)
    {
      return sim[10]->check(vf, x, target, &online.kaos.target, NULL, ModelType);
    }
#endif

  return 1;
}



void
Simul::printStatus() 
{
  if (cntpage && !cntpage->IsFrozen()) {
    cntpage->textWindow->Clear();
    ostream * stream = new ostream(cntpage->textWindow);
    printCounter(stream);
    delete stream;
  }
  char status[255];
  sprintf(status, "Events: %.0f/%.0f", events,accepted);

#ifdef HAVE_WXWIN
    if (display) frame->SetStatusText(status);
    else
#endif
    std::cout << "\r" << status << " \r" << std::flush;
}

extern double VertexQ2;
extern double ThetaGammaHardCMS;
extern double PhiGammaHardCMS;

///////////////////////////////////////////////////////////////////////////////
int
Simul::eventloop()
{
#if YWDEBUG
  std::cout<< events << " Start"<< std::endl;
#endif
  // update status line
  static unsigned int last = 0;
  if (++events > last + 4999) { 
    printStatus();
    last += 5000;
  }
  out->clear();
  // exit condition

  if (events >= simEnd) {
    double errorNorm = 1/pow(simNormalize, 1.0/2.0);
    for (HIST *id=HMFirst; id; id=id->next) {
      int bins = (id->ny ? id->nx*id->ny: id->nx);
      if (!id->error) id->error = new FLOAT[bins];
      for(int i=0; i < bins; i++) {
        id->data[i]    /= simNormalize;
      }
      if (id->datasqr)
        for(int i=0; i < bins; i++) {
          id->datasqr[i] /= simNormalize;
          id->error[i]   = sqrt(id->datasqr[i]-pow(id->data[i],2))*errorNorm;
        }
      free(id->datasqr); id->datasqr = NULL;
      free(id->nbin);    id->nbin    = NULL;
      HMSetScale(id,1.0);
    }
    HMWriteAll(simulation_his);

    // Write and close ntuple
    if (ntuple) nt->delete_ntlist();

    env_def(online_cnt, "SIMULATION_COUNTER",  "Simulation.cnt");
    ostream * onlcnt = new ofstream(online_cnt);
    printCounter(onlcnt);
    delete onlcnt;
    return 1;
  }
  
  // Signal Hangup -> write histogram files

  sigset_t set;
  sigemptyset(&set);
  sigpending(&set);
  if (sigismember(&set, SIGHUP)) {
    env_def(online_hup, "SIMULATION_HUP", "Simulation.his.hup");
    signal(SIGHUP, SIG_IGN);
    std::cerr << std::endl << "Signal HUP: Writing \""
	      << online_hup << "\"...";
    HMWriteAll(online_hup); 
    if (ntuple) nt->delete_ntlist();
    std::cerr << "done." << std::endl;
  }
  
  // generate vertex position
  
  static PseudoRandom psrandom;
  static normal norm(&psrandom, &psrandom);
  
  sobol.nextValues();

  //Read line from cross section file
  if (TriplePolNS::mode == 4 || TriplePolNS::mode == 6) {
    TriplePolNS::infile.read((char *) TriplePolNS::buf, sizeof (float)*46);
    if (TriplePolNS::infile.bad())
      cerr << "Could not read input file!" << endl;
    //cout << TriplePolNS::buf[1] << endl;
  }


  if (He3HACK::mode==3){
    He3HACK::csfile.read((char *) He3HACK::buf,sizeof (float)*14);
    if (He3HACK::csfile.bad() || He3HACK::csfile.eof())
      std::cerr<<"\n\n Warning! Could not read from CS-File. Check #events!\n\n";
  }

  // vertex coordinates in target system
  double targetpos_tar[3] = {0,0,0};
  double sobol2[3] = {sobol(), sobol(), sobol()};
  double norm_vertex[5] = {norm(), norm(), norm(), norm(), norm()};

  //std::cout << targetpos_tar[0] << " "  << targetpos_tar[1] << " "  << targetpos_tar[2] << "\n" ;
  if (ModelType == ElasticRadiative){
      if (!target->Generate_Vertex(norm_vertex, targetpos_tar, rundb.sim.wobx, 
                 rundb.sim.woby, ModelType)) return 0;
  } else {
    if (!target->Generate_Vertex(sobol2, targetpos_tar, rundb.sim.wobx, 
               rundb.sim.woby, ModelType)) return 0;
  }

#if YWdebug == 1

  cout << targetpos_tar[0] << endl;
  cout << targetpos_tar[1] << endl;
  cout << targetpos_tar[2] << endl;
#endif

  //save target position and helicity
  if (TriplePolNS::mode==3) {
    TriplePolNS::buf[0] = targetpos_tar[0];
    TriplePolNS::buf[1] = targetpos_tar[1];
    TriplePolNS::buf[2] = targetpos_tar[2];
    TriplePolNS::buf[6] = helicity;
  }
  
  //get target position and helicity
  if (TriplePolNS::mode==4 || TriplePolNS::mode==6) {
    targetpos_tar[0] = TriplePolNS::buf[0];
    targetpos_tar[1] = TriplePolNS::buf[1];
    targetpos_tar[2] = TriplePolNS::buf[2];
    helicity         = TriplePolNS::buf[6];
  }
 

  if (He3HACK::mode==2)
    {	
      He3HACK::buf[0]=targetpos_tar[0];
      He3HACK::buf[1]=targetpos_tar[1];
      He3HACK::buf[2]=targetpos_tar[2];
    }
  
  if (He3HACK::mode==3)
    {
      targetpos_tar[0]=He3HACK::buf[0];
      targetpos_tar[1]=He3HACK::buf[1];
      targetpos_tar[2]=He3HACK::buf[2];
    }
  
  // vertex coordinates in hall system which is equivalent to BeO system

  double targetpos_hall[3] = {targetpos_tar[0] + rundb.Target.offset_sim.x,
			      targetpos_tar[1] + rundb.Target.offset_sim.y,
			      targetpos_tar[2] + rundb.Target.offset_sim.z};

#if YWdebug == 1

  cout << targetpos_hall[0] << endl;
  cout << targetpos_hall[1] << endl;
  cout << targetpos_hall[2] << endl;
  cout << endl;
#endif
  // beam and target offsets are relative to hall system
  // ---- ????? Why again. Because of that all particles have wrong elos corrections!


  online.Vertex.x = targetpos_tar[0];
  online.Vertex.y = targetpos_tar[1];
  online.Vertex.z = targetpos_tar[2];
  out->packEventData(&online.Vertex.x, 3);

#ifdef __ColaMIT__
  online.ohips.vertex.x = targetpos_hall[0];
  online.ohips.vertex.y = targetpos_hall[1];
  online.ohips.vertex.z = targetpos_hall[2];
  out->packEventData(&online.ohips.vertex.x, 3);

  online.oopsA.vertex.x = targetpos_hall[0];
  online.oopsA.vertex.y = targetpos_hall[1];
  online.oopsA.vertex.z = targetpos_hall[2];
  out->packEventData(&online.oopsA.vertex.x, 3);

  online.oopsB.vertex.x = targetpos_hall[0];
  online.oopsB.vertex.y = targetpos_hall[1];
  online.oopsB.vertex.z = targetpos_hall[2];
  out->packEventData(&online.oopsB.vertex.x, 3);

  online.oopsC.vertex.x = targetpos_hall[0];
  online.oopsC.vertex.y = targetpos_hall[1];
  online.oopsC.vertex.z = targetpos_hall[2];
  out->packEventData(&online.oopsC.vertex.x, 3);

  online.oopsD.vertex.x = targetpos_hall[0];
  online.oopsD.vertex.y = targetpos_hall[1];
  online.oopsD.vertex.z = targetpos_hall[2];
  out->packEventData(&online.oopsD.vertex.x, 3);
#else
  online.A.vertex.x = targetpos_hall[0];
  online.A.vertex.y = targetpos_hall[1];
  online.A.vertex.z = targetpos_hall[2];
  out->packEventData(&online.A.vertex.x, 3);

  online.B.vertex.x = targetpos_hall[0];
  online.B.vertex.y = targetpos_hall[1];
  online.B.vertex.z = targetpos_hall[2];
  out->packEventData(&online.B.vertex.x, 3);

  online.C.vertex.x = targetpos_hall[0];
  online.C.vertex.y = targetpos_hall[1];
  online.C.vertex.z = targetpos_hall[2];
  out->packEventData(&online.C.vertex.x, 3);
#endif

  // energyloss of incoming electron
  
  Reaction->electronIn.setMomentum(0, 0, BeamMomentum);
  if (ergloss) {
    target->EnergyLossSimBeam(Reaction->electronIn, targetpos_tar[0], targetpos_tar[1], targetpos_tar[2], steps_beam, ModelType);//ehemals FIXME

#ifdef __ColaMIT__
    online.beam.Eloss_sim = (BeamEnergy - Reaction->electronIn.energy());
    out->packEventData(&online.beam.Eloss_sim, 1);
#else
    online.Beam.Eloss_sim = (BeamEnergy - Reaction->electronIn.energy());
    out->packEventData(&online.Beam.Eloss_sim, 1);
#endif
  }
  
  /////////////////////////////////////////////////////////////////////////////
  volatile double weight = 1; 
  online.sim.Volume = 1;
  online.sim.scale = 1;

  //std::cout << "Model type =" << ModelType << endl; 

  if (ModelType != TwoBodyPWIA   && ModelType != TwoBodyHe3 &&
      ModelType != ThreeBodyPWIA && ModelType != ThreeBodyHe3 &&
      ModelType != TripleLab && ModelType != TripleLabPP && 
      ModelType != TripleLabHe3 && ModelType != TriplePol &&
      ModelType != He3eepn && ModelType !=He3nr && ModelType != DMBoson && 
      ModelType != DMQEDBackground&&
      Reaction->getM1() && !iselastic && ModelType !=He3fast){
    online.sim.Volume = generator->eoutIntegrationVolume(generateW, 
							 rundb.sim.q2_range,
							 generatep);    
    weight = generator->generateElectron(setq2, generateW, 
					 rundb.sim.q2_range,generatep);
    //Will fix the kinFactor in the radiative generator
    if (ModelType == ElasticRadiative || ModelType == ElasticProton)
      weight=1;
  }
  if (!weight) return 0;
  online.sim.Volume *=generator->integrationVolume();
  // std::cout << "weight (integration Volume): " 
  //	    << online.sim.Volume << std::endl;

  VertexQ2 = 0;
  ThetaGammaHardCMS=0;
  PhiGammaHardCMS=0;  
  //std::cout<<"Event\t" << events << std::endl;
  weight *= generator->generateEvent(helicity) * online.sim.Volume;

  online.sim.Gamma = generator->getGamma();
  online.sim.VertexQ2 = VertexQ2;
  online.sim.ThetaGammaHardCMS = ThetaGammaHardCMS;  
  online.sim.PhiGammaHardCMS = PhiGammaHardCMS;    
  online.sim.weight = weight;
    if (scale>0) online.sim.scale = generator->getScale();

  out->packEventData(&online.sim.VertexQ2, 1);
  out->packEventData(&online.sim.ThetaGammaHardCMS, 1);
  out->packEventData(&online.sim.PhiGammaHardCMS, 1);  
  out->packEventData(&online.sim.Volume, 1);
  out->packEventData(&online.sim.weight, 1);
  out->packEventData(&online.sim.Gamma,  1);
  out->packEventData(&online.sim.scale,  1);

  //std::cout << "Packed weight: " << weight << endl; 

  if (!weight)                                      return 0;  
  if (!chck(Reaction->electronOut, targetpos_hall)) return 0;
  //std::cout<<"Electron accepted!\n";
  if (!chck(Reaction->Out1,        targetpos_hall)) return 0;    
  if (!chck(Reaction->Out2,        targetpos_hall)) return 0;    
  if (!chck(Reaction->Decay1,      targetpos_hall)) return 0;    
  if (!chck(Reaction->Decay2,      targetpos_hall)) return 0;
  if (He3HACK::mode==2){
    He3HACK::listfile.write((char *) He3HACK::buf,sizeof (float)*16);
    if (He3HACK::listfile.bad())
      std::cerr<<"\n\n Warning! Could not write to List-File.\n\n";
  }

  //write variables to file
  if (TriplePolNS::mode == 3) {
    TriplePolNS::outfile.write((char *) TriplePolNS::buf, sizeof (float)*10);
    if (TriplePolNS::outfile.bad())
      cerr << "Could not write output file!" << endl;
  }

  if (ModelType == VCS) {
    Momentum Spin(0,0,0);
    
    double Msqr, k=-1, kp=0;

    weight /= online.sim.Gamma; // we'll calculate an 5-fold cross section
    weight *= ElasticCrossSection(Reaction->electronIn,Reaction->electronOut,
				  Reaction->Out2, helicity, 0, 1, &Spin, 
				  &Reaction->CMSPolarization, &Msqr,&k,kp);
    if (k<rundb.sim.BHmin ||k>rundb.sim.BHmax ) return 0;
    
    static double max = rundb.sim.maxWeight;
    if (weight>max) {
      cerr << "WARNING: run.db: Simul.maxWeight should be greater than "
	   << weight << " (was up to now "<<max<<")." << endl; 
      max = weight;
    }
    Reaction->Out1 = Reaction->Out1.setSpin(Spin);
    //    Spin = Reaction->getA()->getSpin();
    //    cout << Spin<< endl<<endl;

    if (sobol() * max > weight) return 0; 
    weight = max; // All events will have the same weight now.
  }

  accepted++;
  online.nr = (int) accepted;
  out->packEventData(&online.nr, 1);  

  //  cout <<endl << " specres: " << specres << endl;

  if (specres) {
        
#ifdef __ColaMIT__
    sim[0]->resolution(&online.oopsA.target);
    sim[1]->resolution(&online.oopsB.target);
    sim[2]->resolution(&online.oopsC.target);
    sim[3]->resolution(&online.oopsD.target);
    sim[4]->resolution(&online.ohips.target);
#else

    sim[0]->resolution(&online.A.target);
    sim[1]->resolution(&online.B.target);
    sim[2]->resolution(&online.C.target);
    sim[3]->resolution(&online.D.target);
    // &online.B.target is needed just for the happiness of the compiler
    // sim[5]->resolution(&online.B.target);
    sim[10]->resolution(&online.kaos.target);
      
#endif
    
    double dz = (psrandom()<rundb.Target.vertexratio ? rundb.Target.vertexresolution2 * norm() : rundb.Target.vertexresolution * norm());

    targetpos_hall[2] += dz;
    // changed by P. Achenbach  15.04.2015
    // to take vertex distribution into account
    if  (rundb.Target.vertexdistribution == 0)
      targetpos_tar[2] += dz;
    else
      targetpos_tar[2] = 0;
    online.Vertex.z += dz;

#ifdef __ColaMIT__
    online.ohips.vertex.z += dz;
    online.oopsA.vertex.z += dz;
    online.oopsB.vertex.z += dz;
    online.oopsC.vertex.z += dz;
    online.oopsD.vertex.z += dz;
    out->packEventData(&online.Vertex.z, 1);
    out->packEventData(&online.ohips.vertex.z, 1);
    out->packEventData(&online.oopsA.vertex.z, 1);
    out->packEventData(&online.oopsB.vertex.z, 1);
    out->packEventData(&online.oopsC.vertex.z, 1);
    out->packEventData(&online.oopsD.vertex.z, 1);
#else
    online.A.vertex.z += dz;
    online.B.vertex.z += dz;
    online.C.vertex.z += dz;
    online.sim.norm = norm();
    online.sim.normunity = psrandom();
    out->packEventData(&online.Vertex.z, 1);
    out->packEventData(&online.A.vertex.z, 1);
    out->packEventData(&online.B.vertex.z, 1);
    out->packEventData(&online.C.vertex.z, 1);
    out->packEventData(&online.sim.norm, 1);
    out->packEventData(&online.sim.normunity, 1);
#endif
  }    
  
  // changed by P. Achenbach 15.04.2015
  // I do not understand why electron momentum was set again to BeamMomentum
  // after target->EnergyLossSimBeam() has already been called.
  // I believe the energy loss simulation of the incoming beam gets lost.
  // In my simulation using a vertex distribution I do not reset it.
  if (rundb.Target.vertexdistribution == 0) 
    Reaction->electronIn.setMomentum(0, 0, BeamMomentum);
  double E0 = Reaction->electronIn.energy();
  double q2      = (Reaction->electronIn - Reaction->electronOut).square();
  double theta_e = Reaction->electronOut.theta(); 
  double Eprime  = Reaction->electronOut.momentum(); 
  
  //std::cout << rundb.sim.GeneratorFlag <<std::endl;
  if (ergloss) { 
    if (rundb.sim.GeneratorFlag!=2 && 
    rundb.sim.GeneratorFlag!=3 && ModelType!=ElasticRadiative){ //generateElasticDeuteron uses its own radiation correction for these flags
      //Radiative Generator take care of this part by itself
      double vertex = vertexCorrection(q2, E0, Eprime, theta_e); //bss PARAMETERS ARE THE NOT CORRECTED EBEAM, .... ehemals FIXME
      weight *= vertex; //bss: vertex was static before... is 'vertex' a constant for all events?!?! -> removed static, now evaluated for each event individually ehemals FIXME
    }
    double EnergyBefore = Reaction->electronIn.energy();

    target->EnergyLossCorrBeam(Reaction->electronIn,
			       targetpos_tar[0], targetpos_tar[1], 
			       targetpos_tar[2],
			       steps_beam);//ehemals FIXME
    
#ifdef __ColaMIT__
    online.beam.Eloss_corr = (EnergyBefore - Reaction->electronIn.energy());
    out->packEventData(&online.beam.Eloss_corr, 1);   

    sim[4]->energyLossCorrection(target, targetpos_tar);
    out->packEventData(&online.ohips.Eloss_sim, 1);   
    out->packEventData(&online.ohips.Eloss_corr, 1);   

    sim[0]->energyLossCorrection(target, targetpos_tar);
    out->packEventData(&online.oopsA.Eloss_sim, 1);   
    out->packEventData(&online.oopsA.Eloss_corr, 1);   

    sim[1]->energyLossCorrection(target, targetpos_tar);
    out->packEventData(&online.oopsB.Eloss_sim, 1);   
    out->packEventData(&online.oopsB.Eloss_corr, 1);   

    sim[2]->energyLossCorrection(target, targetpos_tar);
    out->packEventData(&online.oopsC.Eloss_sim, 1);   
    out->packEventData(&online.oopsC.Eloss_corr, 1);   

    sim[3]->energyLossCorrection(target, targetpos_tar);
    out->packEventData(&online.oopsD.Eloss_sim, 1);   
    out->packEventData(&online.oopsD.Eloss_corr, 1);   
#else
    online.Beam.Eloss_corr = (EnergyBefore - Reaction->electronIn.energy());
    out->packEventData(&online.Beam.Eloss_corr, 1);   

    sim[0]->energyLossCorrection(target, targetpos_tar);//ehemals FIXME
    out->packEventData(&online.A.Eloss_sim, 1);   
    out->packEventData(&online.A.Eloss_corr, 1);   

    sim[1]->energyLossCorrection(target, targetpos_tar);//ehemals FIXME
    out->packEventData(&online.B.Eloss_sim, 1);   
    out->packEventData(&online.B.Eloss_corr, 1);   
    
    sim[2]->energyLossCorrection(target, targetpos_tar);//ehemals FIXME
    out->packEventData(&online.C.Eloss_sim, 1);   
    out->packEventData(&online.C.Eloss_corr, 1);   
    
    sim[3]->energyLossCorrection(target, targetpos_tar);
    out->packEventData(&online.D.Eloss_sim, 1);   
    out->packEventData(&online.D.Eloss_corr, 1);   
#endif
  }
  
  if (fixedweight!=0) weight = fixedweight;
  if (simCharge>0) {
    //bss old(using Luminosity++): 
    //weight *= simCharge*Luminosity->getMaterialFactor()/1e3;
    //=>  skip the Luminosity.cc detour to finally throw out old Luminosity.cc
    double MatFactor = 1.0 / 1.60217733E-19       // particles/C   //FROM Luminosity.cc
      * target->getTargetMat()->getDensity()    // g/cm^3
      / target->getTargetMat()->getMassnumber() // mol/g
      * 6.0221367E23 // N_AVOGADRO [1/mol]
      * target->getLength() / 10.0 // cm
      / 1E30;                      // cm^2/mubarn
    weight *= simCharge*MatFactor/1e3; //[simcharge]=mC -> * 1/1000 -> C
  }
  if (ModelType==Elastic) weight *= HMInterpolate(crossSection, theta_e/rad);


#ifdef __ColaMIT__
  online.beam.helicity = helicity;
  out->packEventData(&online.beam.helicity);
#else
  online.Beam.helicity = helicity;
  out->packEventData(&online.Beam.helicity);
#endif
 
#ifndef __ColaMIT__

  if (Reaction->getA()) {

    
    //double s[3]={-0.5, 0, 0.5};
    //Momentum Spin = s;
    //Spin *= helicity;
     
    Momentum Spin = Reaction->getA()->getSpin();
    //Spin *= helicity;

    if (Spin[0]!=0 ||  Spin[1]!=0 || Spin[2]!=0 ) {

      double thetaA = rundb.A.angle * rad;
      
      Momentum spinA (Spin[1],
		      -(cos(thetaA) * Spin[0] + sin(thetaA)*Spin[2]),
		       -sin(thetaA) * Spin[0] + cos(thetaA)*Spin[2]);

      Momentum FPpol = 
	SpinMatrix::matrix.SpinTrans(spinA, 
				     online.A.target.dp, online.A.target.th,
				     10*online.A.target.y0, online.A.target.ph,
				     rundb.A.momentum/refmomA);
      
      online.sim.fppx = FPpol[0]; out->packEventData(&online.sim.fppx);
      online.sim.fppy = FPpol[1]; out->packEventData(&online.sim.fppy);
      online.sim.fppz = FPpol[2]; out->packEventData(&online.sim.fppz);
      online.sim.labpx = Spin[0]; out->packEventData(&online.sim.labpx);
      online.sim.labpy = Spin[1]; out->packEventData(&online.sim.labpy);
      online.sim.labpz = Spin[2]; out->packEventData(&online.sim.labpz);
      online.sim.cmpx = Reaction->CMSPolarization[0];
      online.sim.cmpy = Reaction->CMSPolarization[1];
      online.sim.cmpz = Reaction->CMSPolarization[2];
      out->packEventData(&online.sim.cmpx);
      out->packEventData(&online.sim.cmpy);
      out->packEventData(&online.sim.cmpz);
      
      online.ppolA.scat.th = GenerateFPPTheta(Reaction->getA()->momentum());
      online.ppolA.anapow = Apow(Reaction->getA()->energy(),online.ppolA.scat.th);

      //static CosGenerator cosGen(new PseudoRandom());
      //online.ppolA.scat.ph = cosGen(online.ppolA.anapow
      //			    *sqrt(pow(FPpol[0],2)+pow(FPpol[1],2)))
      //	-atan2(FPpol[1],FPpol[0]) - M_PI/2;
      //while (online.ppolA.scat.ph < -M_PI) online.ppolA.scat.ph += 2 * M_PI;
      //while (online.ppolA.scat.ph >  M_PI) online.ppolA.scat.ph -= 2 * M_PI;      
      //online.ppolA.anapow  = Apow(Reaction->getA()->energy(),online.ppolA.scat.th);//<---??
      
      double angle = atan2(FPpol[1], FPpol[0]);
      double nn    = sqrt(FPpol[0] * FPpol[0] + FPpol[1] * FPpol[1]);
      do {
	online.ppolA.scat.ph = rndm() * 2 * M_PI - M_PI;
      } while ((1-sin(online.ppolA.scat.ph - angle) * online.ppolA.anapow*nn)
	       <rndm()*2);
            
      out->packEventData(&online.ppolA.anapow);
      online.ppolA.scat.ph*=1000; //silly mrad in online struct.
      online.ppolA.scat.th*=1000;
      out->packEventData(&online.ppolA.scat.ph);
      out->packEventData(&online.ppolA.scat.th);
    }
  }
#endif

  if (ModelType == DMQEDBackground) weight *= generator->crosssectionOfThisEvent();
  if (!setjmp(env)){
    evaluate_expressions(sim[0]->getParticle(), sim[1]->getParticle(),
			 sim[2]->getParticle(), sim[3]->getParticle(),
			 sim[4]->getParticle(), sim[5]->getParticle(),
			 sim[6]->getParticle(), sim[7]->getParticle(),
			 sim[8]->getParticle(), sim[9]->getParticle(),
			 sim[10]->getParticle(),
			 Reaction->electronIn, weight);
  }

  // write ntuple event
  if (ntuple) nt->fill_ntuple();
  helicity = -helicity;
  //std::cout<< "Final weight: " << weight << std::endl;
  return 0;
}


const char *currentHistogram();
void signalhandler(int sig)
{
  if (sig==SIGFPE) {
    std::cerr << "WARNING: Floating point exeception for event "
	      << Simul::events << std::endl;
    std::cerr << "During filling of "<<currentHistogram()<<std::endl;
    enableFPE();
    signal(SIGFPE, signalhandler);
    longjmp(env,1);
  }
  std::cerr << "Unknown Signal "<< sig << " caught." << std::endl;
  exit(-1);
}

#ifdef HAVE_WXWIN
int work (wxApp *app)
{
  static int firstInit = 0;
  if (!firstInit) { 
    frame    = (HMVFrame*) app->GetTopWindow();
    cntpage  = frame->counterPage;
    statpage = frame->statPage;
    frame->file_menu->AppendSeparator();
    frame->file_menu->Append(SHOWCNTPAGE,  "Show &Counter Window");
    char title[255]; 
    sprintf(title,"Cindy++ Simulation for \'%s\'", setting);
    frame->SetTitle(title);
    firstInit++;
  }

  static Simul *kernel = new Simul();
  return kernel->eventloop();
}

void MenuCallback(int id)
{
  switch (id) {
  case SHOWCNTPAGE:
    if (cntpage) cntpage->Show(TRUE);
    break;
  case SHOWSTATPAGE:
    if (statpage) statpage->Show(TRUE);
    break;
  }
}
#endif

void help(char *argv[]) 
{
  std::cout << "\nUsage: " << argv[0] << " [options] setting\n\n"
    "       Monte-Carlo Simulation of Cola++\n\n" 
    "Options:\n\n" 
    "-d,   --nodisplay      don't start Histogramviewer\n"
    "-v,   --verbose        verbose output of the run.db contents\n"
    "-s N, --skip=N         skip N events before starting\n"
    "-e N, --end=N          end after N events\n"
    "-n N, --normalize=N    normalize with N events\n"
    "-Q C, --charge=C       use charge C [mC] to calculate number of events\n"
    "-m M, --model=MODEL    use model for event generation.\n"
    "                       "
    "MODEL is one of:\n"
    "                       Isotropic,     PWIA,         BoundDelta,\n"
    "                       Pi0Threshold,  TripleLab,    TripleLabPP,\n"
    "                       TripleLabHe3,  TriplePol,    ElasticProton,\n"
    "                       ElasticNucleus,QuasiElasticNucleus\n"
    "                       ElasticDipole, TwoBodyPWIA,  TwoBodyHe3,\n"
    "                       ThreeBodyPWIA, ThreeBodyHe3, He3eepn,\n"
    "                       Maid, ChPT, Iso6D, DBreakup, VCS,\n"
    "                       DMBoson, DMQEDBackground, DMProton, ElasticCryogens\n"
    "                       Elastic, EtaMaid, He3nr, He3fast, Inclusive, ElasticHavar\n"
    "                       kMaid, ElasticDeuteron, ElasticNitrogen, ElasticNuclearPA, Mott\n"
    "                       ElasticPolElectronPolProton, ElasticRadiative\n"
    "                       default: Isotropic\n"
    "-C,   --directory=DIR  change to directory before doing anything\n"
    "-i,   --ignore         ignore floating point exceptions\n"
    "-x,   --exception      exit on floating point exceptions\n"
    "-o FILE, --output=FILE output file name for histogram file\n\n"
    "-r FILE, --rz=FILE     output file name for ntuple file\n\n"
    "Environment        Default         Description\n\n"
    "SIMUL_COL          Simulation.col  Cola object description filename \n"
    "SIMULATION_HIS     Simulation.his  Output file name for histograms \n"
    "SIMULATION_RZ      Simulation.rz   Output file name for ntuple \n"
    "SIMULATION_COUNTER Simulation.cnt  File name for counter information\n" 
    "COLAPRINT          stdout          File name for Print statements\n" 
    "DISPLAY                            "
    "Display variable for histogram viewer\n" 
    "VERBOSE            false           be more verbose\n" 
    "RUN_DB             run.db          Input file name for run information\n" 
    "ENERGYLOSS         2               Level of energyloss calculation.\n"
    "                                   "
    "0 = no correction, 1=one step correction\n"
    "                                   2 = multistep correction\n"
    "                                       and radiation correction \n"
    "INPLANE            false           Generate only events in plane \n"
    "NOWIDTH            false           don't use width of a resonance\n" 
    "SPECRESOLUTION     not used        resolution of spectrometer\n" 
    "GENERATEW          false           generate in W \n"
    "                                    (threshold up to this value in GeV)\n"
    "He3eepnGRID        false           path to He3 grid file\n"
    "WEIGHT             not used        fixed weight for each  event\n"
    "TOF_CS             0               weight*=1 instead of weight*=Gamma\n"
    "1: weight*=1, anything else: weight*=Gamma\n\n"
    "SCALE              0               1= scale Q2,W; 2= scale W,costheta; 3= scale Q2,costheta\n"
    "                                   "
    "compiled "__DATE__" "__TIME__"\n"
    "for detailed help see:\n"
    "http:/""/wwwa1.kph.uni-mainz.de/A1/trigger/doc/Simul++.html\n\n";
  exit(0);
}

int main(int argc, char *argv[])
{
  simStart = simEnd = simNormalize = simCharge = 0;
  simulation_his = (char *) getenv("SIMULATION_HIS");
  simulation_rz  = (char *) getenv("SIMULATION_RZ");
  if (!simulation_his) simulation_his = "Simulation.his";  
  if (!simulation_rz)  simulation_rz  = "Simulation.rz";  
  int sighand = 0;
  char opt;



#if defined(__GNU_LIBRARY__)
  struct option lo[] = {
    {"nodisplay", 0, NULL, 'd'},  {"skip",      1, NULL, 's'},
    {"end",       1, NULL, 'e'},  {"normalize", 1, NULL, 'n'},
    {"output",    1, NULL, 'o'},  {"help",      0, NULL, 'h'},
    {"model",     1, NULL, 'm'},  {"verbose",   0, NULL, 'v'},
    {"ignore",    0, NULL, 'i'},  {"exception", 0, NULL, 'x'},
    {"directory", 1, NULL, 'C'},  {"charge",    1, NULL, 'Q'},
    {"rz",        1, NULL, 'r'},  {"scale",     1, NULL, 'K'},
    {NULL,        0, NULL,  0 }};
  while ((opt = getopt_long(argc, argv, "ds:e:n:o:r:hm:vK:ixC:Q:",lo,NULL))!=EOF) {
#else
  while ((opt = getopt(argc, argv, "ds:e:n:o:r:hm:vKixC:Q:")) != EOF) {
#endif

    switch (opt) {
    case '?':
    case ':':
    case 'h': help(argv);                    break;
    case 'C': if (chdir(optarg));            break;
    case 'i': sighand        = 1;            break;
    case 'x': sighand        = 2;            break;
    case 'v': verbose        = 1;            break;
    case 'K': scale          = atof(optarg); break;
    case 'd': display        = 0;            break;
    case 'o': simulation_his = optarg;       break;
    case 'r': simulation_rz  = optarg;       break;
    case 's': simStart       = atof(optarg); break;
    case 'e': simEnd         = atof(optarg); break;
    case 'n': simNormalize   = atof(optarg); break;
    case 'Q': simCharge      = atof(optarg); break;
    case 'm': 
      //      if(!strcmp(optarg, "Isotropic"))    {break;}
      if(!strcmp(optarg, "Elastic"))      {ModelType = Elastic;       break;}
      if(!strcmp(optarg, "ElasticProton")){ModelType = ElasticProton; break;}
      if(!strcmp(optarg, "ElasticRadiative")){ModelType = ElasticRadiative; break;}
      if(!strcmp(optarg, "ElasticDipole")){ModelType = ElasticDipole; break;}
      if(!strcmp(optarg, "ElasticPolElectronPolProton")){ModelType = ElasticPolElectronPolProton; break;}
      if(!strcmp(optarg, "ElasticNucleus")){ModelType = ElasticNucleus; break;}
      if(!strcmp(optarg, "QuasiElasticNucleus")){ModelType = QuasiElasticNucleus; break;}
      if(!strcmp(optarg, "VCS"))          {ModelType = VCS;           break;}
      if(!strcmp(optarg, "PWIA"))         {ModelType = PWIA;          break;}
      if(!strcmp(optarg, "BoundDelta"))   {ModelType = BoundDelta;    break;}
      if(!strcmp(optarg, "TripleLab"))    {ModelType = TripleLab;     break;}
      if(!strcmp(optarg, "TripleLabPP"))  {ModelType = TripleLabPP;   break;}
      if(!strcmp(optarg, "TripleLabHe3")) {ModelType = TripleLabHe3;  break;}
      if(!strcmp(optarg, "Pi0Threshold")) {ModelType = Pi0Threshold;  break;}
      if(!strcmp(optarg, "Maid"))         {ModelType = Maid;          break;}
      if(!strcmp(optarg, "EtaMaid"))      {ModelType = EtaMaid;       break;}
      if(!strcmp(optarg, "kMaid"))        {ModelType = kMaid;         break;}
      if(!strcmp(optarg, "ChPT"))         {ModelType = ChPTh;         break;}
      if(!strcmp(optarg, "TwoBodyPWIA"))  {ModelType = TwoBodyPWIA;   break;}
      if(!strcmp(optarg, "TwoBodyHe3"))   {ModelType = TwoBodyHe3;    break;}
      if(!strcmp(optarg, "ThreeBodyPWIA")){ModelType = ThreeBodyPWIA; break;}
      if(!strcmp(optarg, "ThreeBodyHe3")) {ModelType = ThreeBodyHe3;  break;}
      if(!strcmp(optarg, "TriplePol"))    {ModelType = TriplePol;     break;}
      if(!strcmp(optarg, "He3eepn"))      {ModelType = He3eepn;       break;}
      if(!strcmp(optarg, "He3nr"))        {ModelType = He3nr;         break;}
      if(!strcmp(optarg, "He3fast"))      {ModelType = He3fast;       break;}
      if(!strcmp(optarg, "DBreakup"))     {ModelType = DBreakup;      break;}
      if(!strcmp(optarg, "Iso6D"))        {ModelType = Iso6D;         break;}
      if(!strcmp(optarg, "Inclusive"))    {ModelType = Inclusive;     break;}
      if(!strcmp(optarg, "DMBoson"))      {ModelType = DMBoson;       break;}
      if(!strcmp(optarg, "DMQEDBackground")) {ModelType = DMQEDBackground; break;}
      if(!strcmp(optarg, "DMProton"))     {ModelType = DMProton;      break;}
      if(!strcmp(optarg, "ElasticNitrogen")){ModelType = ElasticNitrogen; break;}
      if(!strcmp(optarg, "ElasticNuclearPA")){ModelType = ElasticNuclearPA; break;}
      if(!strcmp(optarg, "ElasticHavar")){ModelType = ElasticHavar; break;}      
      if(!strcmp(optarg, "ElasticCryogens")){ModelType = ElasticCryogens; break;}            
      if(!strcmp(optarg, "ElasticDeuteron")){ModelType = ElasticDeuteron; break;}
      if(!strcmp(optarg, "Mott"))         {ModelType = Mott; break;}
      std::cerr << "ERROR: Model '" << optarg << "' not defined" << std::endl;
      help(argv);
    }
  }
  if (optind == argc) help(argv);
  setting = argv[optind];

  if (scale>0) std::cout << "-> scaling of K-MAID cross-section in Q2 and W" 
			 << std::endl;

  if (simStart>0) sobol.init(simStart);
  if (simEnd == 0) simEnd = (optind+1 < argc ? atof(argv[optind + 1]) : 1e6);
  if (simNormalize == 0) simNormalize = simEnd;
  if (simStart > pow(2.0, MAXBIT) - simEnd) {
    std::cerr << "ERROR: Max number of events == "
	      << pow(2.0,MAXBIT) << std::endl;
    help(argv);
  }

  switch (sighand) { // Signal handler for floating point exception
  case 0: enableFPE(); signal(SIGFPE, signalhandler); break; // catch it
  case 1:              signal(SIGFPE, SIG_IGN);       break; // just ignore it
  case 2: enableFPE();                                break; // throw core dump
  }
  sigset_t set;        // Block signal Hangup -> write histogram files
  sigemptyset(&set);
  sigaddset(&set, SIGHUP);
  sigprocmask(SIG_BLOCK, &set, 0);

#ifdef HAVE_WXWIN
  HMMenuCallback = MenuCallback;
  if (display) { 
    HMLaunchViewWork(0, argv, (void (*)(wxApp *))work);
    exit(0);
  }
#endif

  Simul *kernel = new Simul(); 
  while(kernel->eventloop() != 1);



  return 0;
}
