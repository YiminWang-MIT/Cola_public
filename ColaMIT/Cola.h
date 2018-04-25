//								      -*-c++-*-
// Function Prototypes


#include "FourVector/FourVector.h"
#include "Cola/Reactions.h"    // Definition of Reactions, Particles
#include "Targetlength.h"      // Targetcells

#include "Aqua_oops1.h"        // Input data structure for Cindy++
#include "Aqua_online.h"       // 1st level reconstruced data 
#include "CindyReader.h"
#include "BatesReader.h"

#include "HDCraw.h"
#include "VDCraw.h"
#include "Focus.h"
#include "StatPage.h"
#include "Cola/ColaNtuple.h"
#include <setjmp.h>

#include "HMBook/hmbook.h"     // HMBook X11 Histogram library
#ifdef HAVE_WXWIN
#include "HMBook/hmview.h"         // HMBook X11 Histogram Browser
int        work (wxApp *app);
#else
int        work ();
#endif

class MezzoReader;
FourVector Lorentz(FourVector a, FourVector reference);
void       HMLaunchViewWork(int arc, char *arv[], void (*work) (wxApp *wap));
extern void (*HMMenuCallback) (int id);

void       parse_file(AquaTree *, AquaTree *, MezzoReader *,
		      reaction *, char *name,
		      const char *Label="Counts", const char *Unit="");
int        evaluate_expressions(const FourVector& A, const FourVector& B,
				const FourVector& C, const FourVector& D,
				const FourVector& O, const FourVector& H3,
				const FourVector& TOF, const FourVector& NPOL,
				const FourVector& NDET, const FourVector& SI,
				const FourVector& beam,
				double weight = 1.0);
void       printCounter(ostream *);

double     target_mass(char *iso);
int        particle_id(double mass);
void       targetFillHisto(struct TargetCoord *t, struct TargetCo *tgt);
void       MenuCallback(int id);
void       signalhandler(int);

#define env_def(name,var,default) char *name = (char *)getenv(var);\
                                  if (!name) name= default;
#define env_set(name,var,default) { name = (char *) getenv(var);\
				    if (!name) name= default; }

const int REWIND       = 4711;
const int SHOWCNTPAGE  = 4712;
const int SHOWSTATPAGE = 4713;

int   verbose = (getenv("VERBOSE") != NULL);
int   display = ((char *) getenv("DISPLAY") != NULL);
int   ergloss = (getenv("ENERGYLOSS") ? atoi(getenv("ENERGYLOSS")) : 0);
int   dorewind  = 0;
int   firstInit = 0;
long  events    = 0;
jmp_buf env;

class pdcD;
pdcD *PdcD = NULL;

int steps_IN, steps_O, steps_A, steps_B, steps_C, steps_D;
target *target = new cryo_cyl;

char *online_his, *online_rz;
Cola_Ntuple *nt;
Focus *focus = NULL;

struct oops1 oops1; 
struct online online;
struct ohips_data & Ohips = oops1.det.ohips;
struct oops_data  & OopsA = oops1.det.oopsA;
struct oops_data  & OopsB = oops1.det.oopsB;
struct oops_data  & OopsC = oops1.det.oopsC;
struct oops_data  & OopsD = oops1.det.oopsD;

AquaReader *reader = NULL;
Aqua_oops1  atree(&oops1);
Aqua_online out(&online);
statPage StatisticPage(&atree);

time_t lastUpdateTime = 0;
int lastUpdateEvent = 0;

const int maxWire = OU1SIZE;
u_int16_t wiretime[2*maxWire];

AquaShortSubTree *subou1 = atree.getShortSubTree(".det.ohips.chamber.u1");
AquaShortSubTree *subov1 = atree.getShortSubTree(".det.ohips.chamber.v1");
AquaShortSubTree *subou2 = atree.getShortSubTree(".det.ohips.chamber.u2");
AquaShortSubTree *subov2 = atree.getShortSubTree(".det.ohips.chamber.v2");
VdcPlane ou1(".det.ohips.chamber.u1", OU1SIZE);
VdcPlane ov1(".det.ohips.chamber.v1", OV1SIZE);
VdcPlane ou2(".det.ohips.chamber.u2", OU2SIZE);
VdcPlane ov2(".det.ohips.chamber.v2", OV2SIZE);
VdcOHIPSpacket vdcO(&ou1, &ov1, &ou2, &ov2);

AquaShortSubTree *subAx1 = atree.getShortSubTree(".det.oopsA.chamber[1].x");
AquaShortSubTree *subAy1 = atree.getShortSubTree(".det.oopsA.chamber[1].y");
AquaShortSubTree *subAx2 = atree.getShortSubTree(".det.oopsA.chamber[2].x");
AquaShortSubTree *subAy2 = atree.getShortSubTree(".det.oopsA.chamber[2].y");
AquaShortSubTree *subAx3 = atree.getShortSubTree(".det.oopsA.chamber[3].x");
AquaShortSubTree *subAy3 = atree.getShortSubTree(".det.oopsA.chamber[3].y");
HdcPlane hdcAx1(".det.oopsA.chamber[1].x");
HdcPlane hdcAy1(".det.oopsA.chamber[1].y");
HdcPlane hdcAx2(".det.oopsA.chamber[2].x");
HdcPlane hdcAy2(".det.oopsA.chamber[2].y");
HdcPlane hdcAx3(".det.oopsA.chamber[3].x");
HdcPlane hdcAy3(".det.oopsA.chamber[3].y");
HdcChamber hdcA1(&hdcAx1, &hdcAy1);
HdcChamber hdcA2(&hdcAx2, &hdcAy2);
HdcChamber hdcA3(&hdcAx3, &hdcAy3);
HdcPacket hdcA(&hdcA1, &hdcA2, &hdcA3);

AquaShortSubTree *subBx1 = atree.getShortSubTree(".det.oopsB.chamber[1].x");
AquaShortSubTree *subBy1 = atree.getShortSubTree(".det.oopsB.chamber[1].y");
AquaShortSubTree *subBx2 = atree.getShortSubTree(".det.oopsB.chamber[2].x");
AquaShortSubTree *subBy2 = atree.getShortSubTree(".det.oopsB.chamber[2].y");
AquaShortSubTree *subBx3 = atree.getShortSubTree(".det.oopsB.chamber[3].x");
AquaShortSubTree *subBy3 = atree.getShortSubTree(".det.oopsB.chamber[3].y");
HdcPlane hdcBx1(".det.oopsB.chamber[1].x");
HdcPlane hdcBy1(".det.oopsB.chamber[1].y");
HdcPlane hdcBx2(".det.oopsB.chamber[2].x");
HdcPlane hdcBy2(".det.oopsB.chamber[2].y");
HdcPlane hdcBx3(".det.oopsB.chamber[3].x");
HdcPlane hdcBy3(".det.oopsB.chamber[3].y");
HdcChamber hdcB1(&hdcBx1, &hdcBy1);
HdcChamber hdcB2(&hdcBx2, &hdcBy2);
HdcChamber hdcB3(&hdcBx3, &hdcBy3);
HdcPacket hdcB(&hdcB1, &hdcB2, &hdcB3);

AquaShortSubTree *subCx1 = atree.getShortSubTree(".det.oopsC.chamber[1].x");
AquaShortSubTree *subCy1 = atree.getShortSubTree(".det.oopsC.chamber[1].y");
AquaShortSubTree *subCx2 = atree.getShortSubTree(".det.oopsC.chamber[2].x");
AquaShortSubTree *subCy2 = atree.getShortSubTree(".det.oopsC.chamber[2].y");
AquaShortSubTree *subCx3 = atree.getShortSubTree(".det.oopsC.chamber[3].x");
AquaShortSubTree *subCy3 = atree.getShortSubTree(".det.oopsC.chamber[3].y");
HdcPlane hdcCx1(".det.oopsC.chamber[1].x");
HdcPlane hdcCy1(".det.oopsC.chamber[1].y");
HdcPlane hdcCx2(".det.oopsC.chamber[2].x");
HdcPlane hdcCy2(".det.oopsC.chamber[2].y");
HdcPlane hdcCx3(".det.oopsC.chamber[3].x");
HdcPlane hdcCy3(".det.oopsC.chamber[3].y");
HdcChamber hdcC1(&hdcCx1, &hdcCy1);
HdcChamber hdcC2(&hdcCx2, &hdcCy2);
HdcChamber hdcC3(&hdcCx3, &hdcCy3);
HdcPacket hdcC(&hdcC1, &hdcC2, &hdcC3);

AquaShortSubTree *subDx1 = atree.getShortSubTree(".det.oopsD.chamber[1].x");
AquaShortSubTree *subDy1 = atree.getShortSubTree(".det.oopsD.chamber[1].y");
AquaShortSubTree *subDx2 = atree.getShortSubTree(".det.oopsD.chamber[2].x");
AquaShortSubTree *subDy2 = atree.getShortSubTree(".det.oopsD.chamber[2].y");
AquaShortSubTree *subDx3 = atree.getShortSubTree(".det.oopsD.chamber[3].x");
AquaShortSubTree *subDy3 = atree.getShortSubTree(".det.oopsD.chamber[3].y");
HdcPlane hdcDx1(".det.oopsD.chamber[1].x");
HdcPlane hdcDy1(".det.oopsD.chamber[1].y");
HdcPlane hdcDx2(".det.oopsD.chamber[2].x");
HdcPlane hdcDy2(".det.oopsD.chamber[2].y");
HdcPlane hdcDx3(".det.oopsD.chamber[3].x");
HdcPlane hdcDy3(".det.oopsD.chamber[3].y");
HdcChamber hdcD1(&hdcDx1, &hdcDy1);
HdcChamber hdcD2(&hdcDx2, &hdcDy2);
HdcChamber hdcD3(&hdcDx3, &hdcDy3);
HdcPacket hdcD(&hdcD1, &hdcD2, &hdcD3);
