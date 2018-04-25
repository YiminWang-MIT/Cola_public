//								      -*-c++-*-
// Function Prototypes


FourVector Lorentz(FourVector a, FourVector reference);
void       HMLaunchViewWork(int arc, char *arv[], void (*work) (wxApp *wap));
extern void (*HMMenuCallback) (int id);

#ifdef HAVE_WXWIN
int        work (wxApp *app);
#else
int        work ();
#endif

void       parse_file(CindyTree *, CindyTree *, reaction *, char *name,
		      const char *Label="Counts", const char *Unit="");
int        evaluate_expressions(const FourVector& A, const FourVector& B,
				const FourVector& C, const FourVector& D,
				const FourVector& O, const FourVector& H3,
                                const FourVector& TOF, const FourVector& NPOL,
                                const FourVector& beam,
				const double weight = 1.0);
void       printCounter(ostream *);

double     target_mass(char *iso);
int        particle_id(double mass);
void       targetFillHisto(struct TargetCoord *t, struct TargetCo *tgt);
void       MenuCallback(int id);
void       signalhandler(int);

#define env_def(name,var,default) char *name = (char *)getenv(var);\
                                  if (!name) name= default;

const int REWIND       = 4711;
const int SHOWCNTPAGE  = 4712;
const int SHOWSTATPAGE = 4713;

static int     display = ((char *) getenv("DISPLAY") != NULL);
int    dorewind  = 0;
int    firstInit = 0;
long   events    = 0;
jmp_buf env;

struct hrs hrs; 
struct online online;

CindyTree  *ct  = cindyTreeAlloc();
CindyTree  *out = cindyOutTreeAlloc();
VdcOHIPSpacket vdcE(ct, ".e.vdc",
		    VDC_MAXWORD, VDC_MAXWORD, VDC_MAXWORD, VDC_MAXWORD);
VdcOHIPSpacket vdcH(ct, ".h.vdc",
		    VDC_MAXWORD, VDC_MAXWORD, VDC_MAXWORD, VDC_MAXWORD);
statPage StatisticPage(ct);
