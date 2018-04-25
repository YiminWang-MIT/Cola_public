//								      -*-c++-*-
// $Id: Cola.h 2683 2016-12-09 14:00:15Z aesser $
//

#include "FourVector/FourVector.h" // Four Vector Arithmetics
#include "Reactions.h"         // Definition of Reactions, Particles
#include "Targetlength.h"      // Targetcells

#include "Aqua_abcn.h"         // Input data structure for AquaReader
#include "Aqua_abc_2.h"
#include "Aqua_abc.h"
#include "Aqua_ab940223.h"
#include "Aqua_online.h"
#include "Aqua/CindyReader.h"
#include "Aqua/MainzReader.h"
#include "Aqua/AquaTRB.h"

#include "VDCraw.h"            // Wire chambers
#include "MWPCK.h"             // KAOS wire chambers track reconstruction
#include "TOFK.h"              // KAOS TOF track reconstruction
#include "KaosK.h"             // KAOS TOF track reconstruction (2012)
#include "KaosMatrixManager.h" // KAOS transfer matrix handling (2013)
#include "npol.h"              // Neutron polarimeter
#include "hdcPacket.h"         // Proton-Polarimeter stuff
#include "ppol.h"
#include "ndet.h"
#include "Detector/pdcD.h"
#include "Detector/ScintD.h"
#include "Detector/SiliconDetector.h"
#include "Detector/FlashADC.h"
#include "Detector/FibreDetector.h"
#include "Detector/KaosDetectors.h"
#include "Detector/MWPCDetectors.h"
#include "Detector/KaosMultiTrack.h"
#include "Detector/NCube.h"
#include "Detector/Ndet.h"
#include "Simulation/spinTransfer.h"


#include "ColaNtuple.h"
#include "Focus.h"
#include "Luminosity.h"
#include "Cerenkov.h"          // Cerenkov Counter
#include "TMA/baseTM.h"            // Transport matrices
#include "TMA/FastTM.h"            // Transport matrices
#include "TMA/SplineTM.h"          

#include <setjmp.h>

#ifdef HAVE_ROOTTOOLS
#include "RootTools.h"
#endif

#include "HMBook/hmbook.h"         // HMBook Histogram library
#ifdef HAVE_WXWIN
#include "HMBook/hmview.h"         // HMBook X11 Histogram Browser
int        work (wxApp *app);
#else
int        work ();
#endif
void       MenuCallback(int id);
void       signalhandler(int);

//ParserMain.cc:

void       parse_file(AquaTree *, AquaTree *, MezzoReader *,
		      reaction *Reaction, char *name,
		      const char *Label="Counts", const char *Unit="");
int        evaluate_expressions(const FourVector& A, const FourVector& B,
				const FourVector& C, const FourVector& D,
				const FourVector& O, const FourVector& H3,
                                const FourVector& TOF, const FourVector& NPOL,
				const FourVector& NDET, const FourVector& SI,
				const FourVector& KAOS,
				const FourVector& beam,
				const double weight = 1.0);
void       printCounter(ostream *);

//
void       energyLossBeam(FourVector& electronIn, double, double z);
void       energyLossProton(FourVector& protonOut, double hor, double z);
void       energyLossElectron(FourVector& electronOut, double hor, double z);

// SCINTraw.cc:
// NEW VERSION 2014, old can be accessed if appropriate run.db variables (rundb.A.scint.UseNewHandle, ...) NOT set (==0) -> default
int        scintillator(AquaTree *atree, AquaTree *out,
			struct Spectrometer &onl, struct SPECTROMETER &rund, 
			int num_dEpad, int num_ToFpad, 
                        struct MpsPaddle *dEpad, struct MpsPaddle *Tofpad,
                        struct MpsPm *dE_B,short *de_tof_time, char SpecName);

double     qsddaTimeShift(double y, struct online *onl);
double     qsddaLeftShift(double y, struct online *onl);
double     bclamTimeShift(double y, struct online *onl);
double     bclamLeftShift(double y, struct online *onl);
double     qsddcTimeShift(double y, struct online *onl);
double     qsddcLeftShift(double y, struct online *onl);
int        qsddaScintIndex(int pattern);
int        bclamScintIndex(int pattern);
int        qsddcScintIndex(int pattern);
int        qsddaScintIndex(double x);
int        bclamScintIndex(double x);
int        qsddcScintIndex(double x);

#define env_def(name,var,default) char *name = (char *)getenv(var);\
                                  if (!name) name= default;
#define env_set(name,var,default) { name = (char *) getenv(var);\
				    if (!name) name= default; }

const int REWIND       = 4711;
const int SHOWCNTPAGE  = 4712;
const int SHOWSTATPAGE = 4713;

int  display   = (getenv("DISPLAY")?(strchr(getenv("DISPLAY"),':')!=NULL) : 0);
int  ergloss   = (getenv("ENERGYLOSS") ? atoi(getenv("ENERGYLOSS")) : 2);
int  verbosity = (getenv("VERBOSE") != NULL);
int  vdcwarning = (getenv("NOVDCWARNING")?atoi(getenv("NOVDCWARNING")):1);
int  verbose   = 0;
int  dorewind  = 0;
long events    = 0;
extern int quiet;
sigjmp_buf env;
pdcD             *PdcD;

target *target = new cryo_ewald;
extern SobolSequence sobol;
class Cola {
private:

  struct abcn      abcn;
  struct online    online;

  int              cindyflag;
  AquaTree         *atree; 
  AquaTree         *out;
  AquaReader       *reader;
  MezzoReader      *mezzoReader;

  Cola_Ntuple *nt;

  char             *run_db; 
  char             *online_col, *online_his, *online_cnt, *online_rz;
  char             *lumi_out, *lumi_trip, *tof_out;
  Focus            *focus;
  reaction         *Reaction;
  luminosity       *Luminosity;
  vdcBase          *vdcA, *vdcB, *vdcC;
  mwpcK            *MWPCK;
  tofK             *TOFK;
  kaosK            *KAOSK;
  scintD           *ScintD;
  HdcPacket        *hdcA;
  cerenkovDetector *cerA;
  FibreDetector    *SciFi;
  KaosDetectors    *KaosTOF;
  MWPCDetectors    *KaosMWPC;
  SiliconDetector  *siDet;
  FlashADC         *fADC;
  Cnpol            *npol;
  NDetector        *ndet;
  
  Ndet2016         *ndet2016;
  AquaTRBsubevent **trb;
  
  cerenkovDetector *cerB;
  cerenkovDetector *cerC;
  int              display;
  double           BeamEnergy;
  double           qsddaMomentum;
  double           qsddaAngle;
  double           bclamMomentum;
  double           bclamAngle;
  double           qsddcMomentum;
  double           qsddcAngle;
  double           ddMomentum;
  double           ddAngle;
  double           kMomentum;
  double           kAngle;
  long             lasttriggerA;
  long             lasttriggerB;
  long             lasttriggerC;
  int              passPerEvent;

  baseTM             *tmaA;
  baseTM             *tmaB;
  baseTM             *tmaC;
  baseTM             *tmaD;
  baseTM             *tmaK;
  baseTM             *tmaSciFi;
  
  KaosMatrixManager  *matrixK;

  short abtime_1, abtime_2, abtime_3;
  short actime_1, actime_2, actime_3;
  short cbtime_1, cbtime_2, cbtime_3;
  short catime_1, catime_2, catime_3;
  short btime_1;

  short *abtime, *bctime, *catime, *a_time, *b_time, *c_time;
  int steps_IN;
  int steps_A, steps_B, steps_C, steps_D, steps_K, steps_H3, steps_TOF;
 
  void  targetFillHisto(struct TargetCoord *t, 
			struct TargetCo *tgt, AquaTree *out);

#ifdef HAVE_ROOTTOOLS
  // for ROOT tree
  RootTools *rttool;
  char *RootFileName;
#endif

public:
  Cola(AquaReader *, MezzoReader *, int cflag);
  int  eventloop();
  void finish();
  void printStatus();
  void readInputFiles(int dorewind);
  void setOutput(char *o) {online_his = o;};
#ifdef HAVE_ROOTTOOLS
  void Initialize();
  void setOutputROOT(char *o) {if (o) {RootFileName = o;} else {RootFileName = NULL;} };
#endif
  pdcD *getPdcD() {return PdcD;};
};
