//								      -*-c++-*-
//
// $Id: Cola.cc 2691 2017-09-19 21:19:15Z schlimme $
// Main Part of Cindy Online Analysis 
//

#define YWdebug 0

#define _POSIX_C_SOURCE 2

#include "config.h"                // created by configure

#include "Simulation/EventSim/EventSimReader.h"

#include "Cola.h"              // Headers and static variables
#include "evaluate.h"
#include "Aqua/AquaUnzip.h"
#include "Aqua/AquaCompress.h"

#include <signal.h>
#include <unistd.h>

#include "fpu.h"

#include <iostream>
// Dirty hack for focus correction circumvention for spline fit
#undef DIRTYHACK_FORFIT
#ifdef DIRTYHACK_FORFIT
double dpcorr=0; 
double thcorr=0;
#endif

extern "C" int MAIN__() { return 0;}; // bug in libf2c

static int maxevents = 0;
static int skipevents = 0;
time_t starttime = 0;

const double V_LIGHT = 29.9792458;   // cm/ns
// const double deg2rad = M_PI/180;     // rad/degree

// Ntuple stuff
static int ntuple = 0;
struct hbookmem {
  int hmemor[HBOOKMEM];
} pawc_;
char *ntvarn[MAX_NT_VAR];  // Scalar name related to ntvar
char *ntdes[MAX_NT_VAR];    // NTvariable name
char *ntbck[MAX_NT_VAR];    // Block name for each ntvar
char *nttitle;     // Ntuple Title 
int  ntiden, ntnvar;

#define dv (-4711)             // That's the default value for "not valid"
#define assignitem(var, item) if (atree->itemOK(&item)) var=item;
inline double securesqrt(double c) { return (c > 0) ? 1/sqrt(c) : 0; }

#ifndef USE_QT
HMVFrame   *frame    = NULL;
HMVPage    *cntpage  = NULL;
HMVPage    *statpage = NULL;
#endif

SobolSequence sobol;
///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////
Cola::Cola(AquaReader *myreader, MezzoReader *mreader, int cflag)
{
  reader = myreader;
  mezzoReader = mreader;
  switch (cindyflag = cflag) {
  case 2:
    atree = new Aqua_abc_2(&abcn);
    break;
  case 3:
    atree = new Aqua_abc(&abcn);
    break;
  case 4:
    atree = new Aqua_ab940223(&abcn);
    break;
  default:
    cindyflag = 1;
    atree = new Aqua_abcn(&abcn);
    break;
  }
  out = new Aqua_online(&online);
  reader->initTree(atree, out);

  Luminosity = NULL;
  Reaction   = NULL; 
  focus      = NULL;
  tmaA       = tmaB       = tmaC     = 
  tmaD       = tmaK       = tmaSciFi = NULL;

  lasttriggerA = lasttriggerB = lasttriggerC = 0;

  vdcA = vdcB = vdcC = NULL;
  PdcD = NULL;
  ndet = NULL;
  siDet = NULL;
  hdcA = new HdcPacket(atree, ".a.det.hdc");
  if (rundb.fadc.use_fadc) fADC = NULL;

  // Kaos spectrometer
  MWPCK   = NULL;
  SciFi   = NULL;
  KaosTOF = NULL;
  KaosMWPC= NULL;
  matrixK = NULL;

  // multi track analysis
  passPerEvent = 0;

  // Ndet2016 Neutron detector
  ndet2016 = NULL;
  
  env_set(run_db,     "RUN_DB",         "run.db");
  env_set(online_col, "ONLINE_COL",     "Online.col");
  env_set(online_cnt, "ONLINE_COUNTER", "Online.cnt");
  env_set(online_rz,  "ONLINE_RZ",      "Online.rz");
  env_set(lumi_out,   "LUMI_OUT",       "Luminosity.out");
  env_set(lumi_trip,  "LUMI_TRIP",      "Luminosity.trip.out");
  env_set(tof_out,    "TOF_OUT",        "ToF.out");
}
///////////////////////////////////////////////////////////////////////////////
class checkMezzoRundb {
private:
  double *mezzovalue, rundbvalue, delta;
  char *rundbname, *mezzoname;
  static class checkMezzoRundb *first;
  class checkMezzoRundb *next;

  void check() {
    if (fabs(*mezzovalue - rundbvalue) > delta)
      std::cerr<<"WARNING MEZZO:  "<<mezzoname<<" = "<<*mezzovalue<<std::endl
	       <<"        RUN_DB: "<<rundbname<<" = "<<rundbvalue <<std::endl;
  }
public:
  checkMezzoRundb(MezzoReader *mreader, char *mezzoname, 
		  char *rundbname, double rundbvalue=0, double delta=0) {
    checkMezzoRundb::rundbvalue = rundbvalue;
    checkMezzoRundb::rundbname  = rundbname;
    checkMezzoRundb::mezzoname  = mezzoname;
    checkMezzoRundb::delta      = delta;  
    mezzovalue=mreader?mreader->init(mezzoname,rundbvalue):&(this->rundbvalue);
                               
    next=first;
    first=this;
  }
  static void checkAll() {
    for (checkMezzoRundb *cmr = first; cmr; cmr = cmr->next) cmr->check();
  }
};

#ifdef HAVE_ROOTTOOLS
void Cola::Initialize()
{
  if (RootFileName) {
    rttool = new RootTools(RootFileName);
    std::cout << "<Cola.cc> Root tree output into "
	      << RootFileName << std::endl;
    //    rttool->SetOutput(atree,out, 1);
    rttool->SetOutput(atree,out, 2);
    rttool->SetRunID(starttime);
  }
}
#endif

class checkMezzoRundb *checkMezzoRundb::first = NULL;

///////////////////////////////////////////////////////////////////////////////
void
Cola::printStatus()
{
  if (cntpage) 
    if(cntpage->IsShown()) 
      if (!cntpage->IsFrozen()) {
	cntpage->textWindow->Clear();
	ostream *stream = new ostream(cntpage->textWindow);
        printCounter(stream);
	delete stream;
      }
  char status[255];
  status[0]=0;
  if (events>1000) sprintf(status, "Events: %7ld     Luminosity: %.2f/pbarn"
	  "     Dead Time: %4.1f%%", events,
	  Luminosity->getIntegrated()/1e6,
	  Luminosity->getDeadtime());
  if (frame) frame->SetStatusText(status);
  else       std::cerr << "\r" << status << " \r" << std::flush;
}
///////////////////////////////////////////////////////////////////////////////
void 
Cola::finish()
{ 
  checkMezzoRundb::checkAll();
  char *fn = new char[strlen(online_his) + 34];
  char *lumin = new char[strlen(online_his) + 34];
  char *tripn = new char[strlen(online_his) + 34];
  strcpy(fn, online_his);
  strcpy(lumin, lumi_out);
  strcpy(tripn, lumi_trip);
  if (atree->getRunName())
    if (strlen(atree->getRunName())>22) {
      char runname[32];
      strncpy(runname, atree->getRunName(), 23);    
      runname[23] = '\0';
      sprintf(fn,    online_his, runname);
      sprintf(lumin, lumi_out,   runname);
      sprintf(tripn, lumi_trip,  runname);
    }

  // check if mezzo items existed
  if (mezzoReader) mezzoReader->item_scan();

  //  Write and close NTUPLE
  if (ntuple) nt->delete_ntlist();

  ostream *onlcnt = new ofstream(online_cnt);  // write counter
  printCounter(onlcnt);
  delete onlcnt;
  
  Luminosity->print(lumin, tripn);       // write luminosity info
  sleep(1);         // 1 second for the makefile                            
  HMWriteAll(fn);                        // write histograms


#ifdef TOF_DEBUGFILE
  if (tof) tof->print(tof_out);           // ToF debugging output
#endif

  // atree->closeIO();
  // out->closeIO();                               // close IO

#ifdef HAVE_ROOTTOOLS
  if (RootFileName) rttool->CloseFile();
#endif

  printStatus();
}
///////////////////////////////////////////////////////////////////////////////
void LoadVDCConfiguration(char *filename, char *prefix, VdcPacket *vdc) {
  FILE *file = filename ? fopen(filename, "r") : NULL;
  if (file) {
    char tdc_format[128];
    strcpy(tdc_format, prefix);
    strcat(tdc_format, ".VDC.TDC.%[xs12][%d] = %d\n");
    while (!feof(file)) {
      char layer[128];
      char text[128];
      int wire = -1;
      int offset = 0;
      if (fgets(text, 128, file));
      if (sscanf(text, tdc_format, layer, &wire, &offset) == 3) {
        if (offset > 0) {
          if (!strcmp(layer, "x1"))
            vdc->x1Plane()->InitOffset(offset, wire, 1);
          else if (!strcmp(layer, "s1"))
            vdc->s1Plane()->InitOffset(offset, wire, 1);
          else if (!strcmp(layer, "x2"))
            vdc->x2Plane()->InitOffset(offset, wire, 1);
          else if (!strcmp(layer, "s2"))
            vdc->s2Plane()->InitOffset(offset, wire, 1);
        }
      }
    }
    fclose(file);
  }
}

///////////////////////////////////////////////////////////////////////////////
// read input: this has to be done after the first event was read!
///////////////////////////////////////////////////////////////////////////////
void 
Cola::readInputFiles(int dorewind)
{
  // Default initialization. don't rely on them!

  struct MpsDataA& spekA = abcn.a;
  struct MpsDataB& spekB = abcn.b;
  struct MpsDataC& spekC = abcn.c;

  rundb.Ebeam = 855;
  rundb.tdc_resolution = 0.050;
  rundb.ab_tdc_resolution = rundb.bc_tdc_resolution = 0;
  rundb.ac_tdc_resolution = 0;
  rundb.a_tdc_resolution = 0;  rundb.b_tdc_resolution = 0;  
  rundb.c_tdc_resolution = 0;
  rundb.C.scint.use_dE = rundb.turns = 0;
  rundb.Target.length = rundb.Target.angle = rundb.Target.density = 
    rundb.Target.wallthickness = rundb.Target.snowthickness =
    rundb.Target.snowdensity = 0; 

  rundb.Target.size_length = rundb.Target.size_width = rundb.Target.size_height = 0;
  rundb.Target.snowparam = 0;
  rundb.Target.offset.x = 0;
  rundb.Target.offset.y = 0;
  rundb.Target.offset.z = 0;
  rundb.Target.offset_sim.x = 0;
  rundb.Target.offset_sim.y = 0;
  rundb.Target.offset_sim.z = 0;
  rundb.TargetpositionIsKnown = 0;
  rundb.Target.Polarisation[0] = 0;
  rundb.Target.Polarisation[1] = 0; 
  rundb.Target.Polarisation[2] = 0;
  rundb.Target.wallparam[0] = 0;
  rundb.Target.wallparam[1] = 0;
  rundb.Target.wallparam[2] = 0;
  rundb.A.vacuum = rundb.B.vacuum = rundb.C.vacuum = rundb.kaos.vacuum = 0;     
  rundb.beam.offset.x = rundb.beam.offset.y = 0;
  rundb.beam.P_e = 0;  
  rundb.coincidencetime_AB = rundb.coincidencetime_CB = 0;
  rundb.coincidencetime_CA = 0;
  rundb.singletime_A = rundb.singletime_B = 0;
  rundb.singletime_C = rundb.singletime = 0;
  rundb.A.left = 1;
  rundb.B.left = 0;
  rundb.C.left = 0;
  rundb.A.vdc.x.mode = 0; rundb.A.vdc.s.mode = 0;
  rundb.B.vdc.x.mode = 0; rundb.B.vdc.s.mode = 0;
  rundb.C.vdc.x.mode = 0; rundb.C.vdc.s.mode = 0;
  rundb.A.vdc.corr_driftvel = 1.00;
  rundb.B.vdc.corr_driftvel = 1.00;
  rundb.C.vdc.corr_driftvel = 1.00;
  rundb.A.vdc.trackps = NULL;
  rundb.B.vdc.trackps = NULL;
  rundb.C.vdc.trackps = NULL;
  rundb.A.Prescaler    = rundb.B.Prescaler    = rundb.C.Prescaler    = 1;
  rundb.A.ScalerSingle = rundb.B.ScalerSingle = rundb.C.ScalerSingle = 1; 
  rundb.A.ScalerDouble = rundb.B.ScalerDouble = rundb.C.ScalerDouble = 1;
  rundb.A.PreStrobe    = rundb.B.PreStrobe    = rundb.C.PreStrobe    = 1;
  rundb.A.StrobeDead   = rundb.B.StrobeDead   = rundb.C.StrobeDead   = 90; //ns
    
  rundb.target     = "";
  rundb.reaction   = "H(e,B:e'A:p)";
  rundb.use_vertex = "automatic";
  rundb.use_wobbler_adc = "automatic";
  rundb.focus_corr = "no";
  rundb.wobbler_corr = "yes";
  rundb.additional_title="";

  for (int i=0; i<12; i++) {
    rundb.A.cerenkov.pedestal[i] = rundb.A.cerenkov.offset[i] = 0;
    rundb.B.cerenkov.pedestal[i] = rundb.B.cerenkov.offset[i] = 0;
    rundb.C.cerenkov.pedestal[i] = rundb.C.cerenkov.offset[i] = 0;
    rundb.A.cerenkov.scale[i] = 1;
    rundb.B.cerenkov.scale[i] = 1;
    rundb.C.cerenkov.scale[i] = 1;
  }
  for (int i=0;i<16;i++) {
    rundb.A.vdc.x1.disabled[i] = rundb.A.vdc.s1.disabled[i] =
    rundb.A.vdc.x2.disabled[i] = rundb.A.vdc.s2.disabled[i] =
    rundb.B.vdc.x1.disabled[i] = rundb.B.vdc.s1.disabled[i] =
    rundb.B.vdc.x2.disabled[i] = rundb.B.vdc.s2.disabled[i] =
    rundb.C.vdc.x1.disabled[i] = rundb.C.vdc.s1.disabled[i] =
    rundb.C.vdc.x2.disabled[i] = rundb.C.vdc.s2.disabled[i] = -1;
  }
  rundb.A.cerenkov.FloatingPed = 0; rundb.B.cerenkov.FloatingPed = 0; 
  rundb.C.cerenkov.FloatingPed = 0;
  rundb.A.cerenkov.FloatingEvents = 0; rundb.B.cerenkov.FloatingEvents=0; 
  rundb.C.cerenkov.FloatingEvents = 0;
  env_set(rundb.A.cerenkov.PedFile, "PEDFILE_A",  ""); 
  env_set(rundb.B.cerenkov.PedFile, "PEDFILE_B",  ""); 
  env_set(rundb.C.cerenkov.PedFile, "PEDFILE_C",  ""); 

  rundb.A.scint.MinAdcOverThresholdValue=5; //bss, software ped sub
  rundb.B.scint.MinAdcOverThresholdValue=5; //bss
  rundb.C.scint.MinAdcOverThresholdValue=5; //bss
  rundb.A.scint.UseNewHandle=0;
  rundb.B.scint.UseNewHandle=0;
  rundb.C.scint.UseNewHandle=0;

  // Scintillator correction
  for (int i=0; i<15; i ++) {
   rundb.A.scint.dE_corr_left_offset[i] =rundb.A.scint.dE_corr_right_offset[i]=
   rundb.A.scint.ToF_corr_left_offset[i]=rundb.A.scint.ToF_corr_right_offset[i]=0;
   rundb.B.scint.dE_corr_left_offset[i] =rundb.B.scint.dE_corr_right_offset[i]=
   rundb.B.scint.ToF_corr_left_offset[i]=rundb.B.scint.ToF_corr_right_offset[i]=0;
   rundb.C.scint.dE_corr_left_offset[i] =rundb.C.scint.dE_corr_right_offset[i]=
   rundb.C.scint.ToF_corr_left_offset[i]=rundb.C.scint.ToF_corr_right_offset[i]=0;
   rundb.A.scint.dE_corr_right_scale[i] =rundb.A.scint.dE_corr_left_scale[i]=
   rundb.A.scint.ToF_corr_right_scale[i]=rundb.A.scint.ToF_corr_left_scale[i]=1;
   rundb.B.scint.dE_corr_right_scale[i] =rundb.B.scint.dE_corr_left_scale[i]=
   rundb.B.scint.ToF_corr_right_scale[i]=rundb.B.scint.ToF_corr_left_scale[i]=1;
   rundb.C.scint.dE_corr_right_scale[i] =rundb.C.scint.dE_corr_left_scale[i]=
   rundb.C.scint.ToF_corr_right_scale[i]=rundb.C.scint.ToF_corr_left_scale[i]=1;
;

  }


  // Initialization for NPOL run.db values
  memset(rundb.npol.tdcScale, 0, 5*sizeof(double));
  memset(rundb.npol.minADC, 0, 5*sizeof(double));
  rundb.npol.alpha = 0;
  rundb.npol.maxjitter = 0;
  memset(rundb.npol.W1_origin, 0, 3*sizeof(double));
  memset(rundb.npol.Top_origin, 0, 3*sizeof(double));
  memset(rundb.npol.Bottom_origin, 0, 3*sizeof(double));
  memset(rundb.npol.trig1_pos, 0, 3*sizeof(double));
  memset(rundb.npol.trig2_pos, 0, 3*sizeof(double));
  memset(rundb.npol.trig3_pos, 0, 3*sizeof(double));
  rundb.npol.file_position = "";
  rundb.npol.file_pedestal = "";
  rundb.npol.file_tdcDiff = "";
  rundb.npol.file_tdcSum = "";
  rundb.npol.file_tdcWalk = "";
  rundb.npol.file_tdcCut = "";
  rundb.npol.file_adcCut = "";
  rundb.npol.file_timeCut = "";
  rundb.npol.file_pulshCut = "";
  rundb.npol.file_adcRatio = "";
  rundb.npol.W1_single = 0;
  rundb.npol.W1_double[0]=rundb.npol.W1_double[1]=0;
  rundb.npol.W2_single = 0;
  memset(rundb.npol.W2_double, 0, 6*sizeof(double));
  memset(rundb.npol.W2_triple, 0, 3*sizeof(double));
  rundb.npol.veto_use = 1;
  rundb.npol.n_veto_use = 1;
  rundb.npol.magnet_I = 0;
  memset(rundb.npol.magnet_pos, 0, 3*sizeof(double));
  rundb.npol.magnet_path = ""; 
  rundb.npol.usemulti = 0;
  
  // Initialize Silicon run.db parameters
  rundb.si.configfile = NULL;
  rundb.si.angle = 90.0;
  rundb.si.distance = 8.0; //[cm]
  rundb.si.simCentMom = 0.240; //[GeV/c]
  rundb.si.simAcceptScale = 1.0;
  for (int ii=0;ii<8;ii++) rundb.si.simResolution[ii] = 0.0; // FWHM [GeV]
  // End Silicon initialization

  // Initialize Flash ADC run.db parameters
  rundb.fadc.use_fadc = 0;
  rundb.fadc.M = 940;
  rundb.fadc.trap_RT = 14;
  rundb.fadc.trap_FT_min = 100;
  rundb.fadc.trap_FT_max = 100;
  rundb.fadc.signal_start = 50;
  rundb.fadc.pileup_control = 0;
  rundb.fadc.pileup_threshold = 0;
  // End Flash ADC initialization

  // Initialize NDet run.db parameters
  for (int i=0; i<36; i++) {
    rundb.N.offset[2*i] = rundb.N.offset[2*i+1] = 800.0;
    rundb.N.pedestal[2*i] = rundb.N.pedestal[2*i+1] = 50.0;
    rundb.N.position[i] = rundb.N.timeAtTarget[i] = 0.0;
  }
  rundb.N.angle = 24.79;
  rundb.N.oop_angle = 0.0;
  rundb.N.distance = 1800.0;
  rundb.N.height = 500.0;
  rundb.N.width = 500.0;
  rundb.N.filename = NULL;
  // End NDet initialization

  // Initialize Kaos Detectors run.db parameters
  rundb.kaos.tma.filename  = strdup("");
  rundb.kaos.use_tof       = 0;
  rundb.kaos.use_mwpc      = 0;
  rundb.kaos.use_track     = 1;
  rundb.kaos.use_tagger    = 0;
  rundb.kaos.use_trigger   = 0;
  rundb.kaos.min_quality   = 0;
  rundb.kaos.MWPC_trackDir = NULL;
  rundb.kaos.trigger       = NULL;

  for (int i=0; i<2; i++) {rundb.kaos.ADC_scale[i] = 0.01;}

  for (int i=0; i<5; i++) {
    rundb.kaos.threshold[i] = 1;
    rundb.kaos.offset[i] = 0;
  }

  for (int i=0; i<120; i++) {
    rundb.kaos.ADC_gain[i]       = 1;
    rundb.kaos.TDC_offset[i]     = 0;
    rundb.kaos.TDC_time_corr[i]  = 0;
    rundb.kaos.ADC_absorption[i] = 200;
    rundb.kaos.ADC_absorption[i+120] = 200;
  }

  for (int i=120; i<128; i++) {
    rundb.kaos.TDC_correction[i] = 0;
    rundb.kaos.ADC_correction[i] = 0;
    rundb.kaos.ADC_pedestal[i] = 200;
  }
  for (int i=0; i<4; i++) {
    rundb.kaos.fe_pos[i] = 1;}
  for (int i=0; i<31; i++) {
    rundb.kaos.track_mwpc[i] = 0;}
  for (int i=0; i<19; i++) {
    rundb.kaos.track_tof[i] = 0;}
  for (int i=0; i<50; i++) {
    rundb.kaos.det_pos[i] = 0;}
  rundb.kaos.det_pos[30] = 225.63; // new offset introduced in 2011

  for (int i=0; i<620; i++) {rundb.kaos.MWPC_gain[i] = 1;}

  for (int i=0; i<24; i++) {
    rundb.kaos.Aerogel_gain[i] = 1;
    rundb.kaos.Aerogel_pedestal[i] = 0;
    rundb.kaos.Aerogel_correction[i] = 0;
    rundb.kaos.Aerogel_offset[i] = 0;
  }

  // Initialize Fibre Detector run.db parameters
  rundb.SciFi.use_fibre = 0;
  
  rundb.SciFi.analysisParameters[0] = 4;
  rundb.SciFi.analysisParameters[1] = 5;
  rundb.SciFi.analysisParameters[2] = 15;
  rundb.SciFi.analysisParameters[3] = -90;
  rundb.SciFi.analysisParameters[4] = 90;
  
  rundb.SciFi.eventDisplay[0] = 0;
  rundb.SciFi.eventDisplay[1] = 0;
  rundb.SciFi.eventDisplay[2] = 0;

  for (int i=0; i<FIBRECHNS; i++) {
    rundb.SciFi.TDC_offset[i] = 0;
  }
  for (int i=0; i< 2; i++) {
    rundb.SciFi.trigger[i]    = 0;};

  // read the run database from file 
  char runname[32];
  if (atree->getRunName())	
    sprintf(runname, "%s", atree->getRunName());
  //  else if (atree->getInputFilename()) 
  //  strncpy(runname, atree->getInputFilename(),17);
 
  readRunDatabase(run_db, runname, NULL, 
		  starttime ? starttime : atree->getTimer()->getStarttime());

  if (Reaction) delete Reaction;
  Reaction = new reaction(rundb.reaction);

  // construct histogram evaluation tree
  parse_file(atree, out, mezzoReader, Reaction, online_col);

  target = SetTargetFromRunDB(Reaction);

  delete vdcA;
  vdcA = new vdcSpecA(atree, out, ".a.det.vdc", &online.A, &rundb.A,
		      AX1SIZE, AS1SIZE, AX2SIZE, AS2SIZE);
  delete vdcB;
  vdcB = new vdcSpecB(atree, out, ".b.det.vdc", &online.B, &rundb.B,
		      BX1SIZE, BS1SIZE, BX2SIZE, BS2SIZE);
  delete vdcC;
  vdcC = new vdcSpecC(atree, out, ".c.det.vdc", &online.C, &rundb.C,
		      CX1SIZE, CS1SIZE, CX2SIZE, CS2SIZE);
  
  delete PdcD;
  PdcD = new pdcD(".d.pdc", atree, "online.D" ,out, 1);

  delete siDet;
  siDet = new SiliconDetector(atree, out, rundb.fadc.use_fadc); 
  siDet->init(rundb.si.configfile, rundb.si.angle, rundb.si.distance);
 
  if (rundb.fadc.use_fadc == 2) {
    delete fADC;
    fADC = new FlashADC(atree, out);
    fADC->init(rundb.fadc.M,rundb.fadc.trap_RT,			\
	       rundb.fadc.trap_FT_min, rundb.fadc.trap_FT_max, \
	       rundb.fadc.signal_start, rundb.fadc.pileup_control, \
	       rundb.fadc.pileup_threshold);
  }


  delete ndet;
  ndet = new NDetector(atree, out);
  ndet->init(&rundb.N);

  if (rundb.kaos.use_mwpc) {
    delete KaosMWPC;
    KaosMWPC = new MWPCDetectors(atree, out);
    KaosMWPC->init(rundb.kaos.det_pos, rundb.kaos.track_mwpc,
		   rundb.kaos.min_quality,
		   rundb.kaos.MWPC_gain, rundb.kaos.TDC_correction,
		   rundb.kaos.TDC_time_corr, rundb.kaos.TDC_offset,
		   rundb.kaos.use_trigger, rundb.kaos.use_tof, 
		   rundb.kaos.use_tagger, rundb.kaos.use_track, 
		   rundb.kaos.trigger);
  }
  if (rundb.kaos.use_tof) {
    delete KaosTOF;
    KaosTOF = new KaosDetectors(atree, out);
    KaosTOF->init( rundb.kaos.det_pos, rundb.kaos.track_tof,
		   rundb.kaos.threshold,
		   rundb.kaos.ADC_gain, rundb.kaos.ADC_pedestal,
		   rundb.kaos.ADC_correction, 
		   rundb.kaos.TDC_offset, rundb.kaos.TDC_correction,
		   rundb.kaos.TDC_time_corr, rundb.kaos.timewalk_corr,
                   rundb.kaos.ADC_absorption, rundb.kaos.ADC_scale,
		   rundb.kaos.Aerogel_gain, rundb.kaos.Aerogel_pedestal,
		   rundb.kaos.Aerogel_correction, rundb.kaos.Aerogel_offset,
		   rundb.kaos.use_trigger, rundb.kaos.use_tof,
                   rundb.kaos.use_timewalk_corr,
		   rundb.kaos.use_tagger, rundb.kaos.use_mwpc, 
		   rundb.kaos.use_track, 
		   rundb.kaos.use_aerogel, rundb.kaos.trigger,
                   rundb.kaos.use_multi_track);
   }
  if (rundb.kaos.use_track == 1) 
    {
      delete MWPCK;
      MWPCK = new mwpcK(out);
      MWPCK->init(rundb.kaos.fe_pos, rundb.kaos.det_pos);
    }
  if (rundb.kaos.use_track == 2 || rundb.kaos.use_multi_track == 1)
    {
      delete TOFK;
      TOFK = new tofK(out);
      TOFK->init(rundb.kaos.fe_pos, rundb.kaos.det_pos);
    }

  if (rundb.kaos.use_track > 2)
    {
      delete KAOSK;
      KAOSK = new kaosK(out);
      KAOSK->init(rundb.kaos.fe_pos, rundb.kaos.det_pos, rundb.kaos.use_track);
    }
  
  if (rundb.SciFi.use_fibre) {
    delete SciFi;
    SciFi = new FibreDetector(atree, out);
    SciFi->init( rundb.SciFi.TDC_offset, 
//		 rundb.SciFi.TDC2pmt, 
		 rundb.SciFi.trigger,
		 rundb.SciFi.analysisParameters,
		 rundb.SciFi.eventDisplay);
  }

  // Ndet2016
  if (rundb.ndet.use_ndet) {
    delete ndet2016;
    trb = (AquaTRBsubevent **) reader->provideObjectPtr(0x07A4);
    ndet2016 = new Ndet2016(atree, out, trb);
    ndet2016->init(rundb.ndet.mappingFile, rundb.ndet.calibrationFile);
  }


  
  ScintD = new scintD(".d", atree, "online.D", out);

  if (!rundb.TargetpositionIsKnown)
    rundb.Target.offset_sim = rundb.Target.offset;

  // run.db dependent initializations
  BeamEnergy     = rundb.Ebeam / 1000.0;
  qsddaMomentum  = rundb.A.momentum / 1047.6203;  //bss 660/630 ~ 1.0476
  qsddaAngle     = rundb.A.angle * 0.017453293;
  bclamMomentum  = rundb.B.momentum / 1000.0;
  if (rundb.B.left) rundb.B.angle  = -rundb.B.angle;
  bclamAngle     = rundb.B.angle * 0.017453293;
  qsddcMomentum  = rundb.C.momentum / 1067.4711; // bss: 490/459 ~ 1.067
  qsddcAngle     = rundb.C.angle * 0.017453293;
  ddMomentum     = rundb.D.momentum / 1000.0;
  //if (rundb.D.left) rundb.D.angle = -rundb.D.angle;
  ddAngle        = rundb.D.angle * 0.017453293;

  kAngle         = rundb.kaos.angle * 0.017453293;
  kMomentum      = rundb.kaos.momentum/1000;

  if (!rundb.turns) rundb.turns = ((int)(rundb.Ebeam + 0.5) - 180.0)/7.5;
  if (!rundb.ab_tdc_resolution) rundb.ab_tdc_resolution = rundb.tdc_resolution;
  if (!rundb.bc_tdc_resolution) rundb.bc_tdc_resolution = rundb.tdc_resolution;
  if (!rundb.ac_tdc_resolution) rundb.ac_tdc_resolution = rundb.tdc_resolution;
  //for dETof-TDC's
  if (!rundb.A.de_tof_tdc_resolution) 
    rundb.A.de_tof_tdc_resolution = rundb.tdc_resolution;
  if (!rundb.B.de_tof_tdc_resolution) 
    rundb.B.de_tof_tdc_resolution = rundb.tdc_resolution;
  if (!rundb.C.de_tof_tdc_resolution) 
    rundb.C.de_tof_tdc_resolution = rundb.tdc_resolution;
  //for pulsed Beam
  if (!rundb.a_tdc_resolution) rundb.a_tdc_resolution = rundb.tdc_resolution;
  if (!rundb.b_tdc_resolution) rundb.b_tdc_resolution = rundb.tdc_resolution;
  if (!rundb.c_tdc_resolution) rundb.c_tdc_resolution = rundb.tdc_resolution;

  vdcA->disable(rundb.A.vdc.x1.disabled, rundb.A.vdc.s1.disabled,
		rundb.A.vdc.x2.disabled, rundb.A.vdc.s2.disabled, 16);
  vdcB->disable(rundb.B.vdc.x1.disabled, rundb.B.vdc.s1.disabled,
		rundb.B.vdc.x2.disabled, rundb.B.vdc.s2.disabled, 16);
  vdcC->disable(rundb.C.vdc.x1.disabled, rundb.C.vdc.s1.disabled,
		rundb.C.vdc.x2.disabled, rundb.C.vdc.s2.disabled, 16);

  delete Luminosity;
  Luminosity = new luminosity(atree, target, Reaction, rundb.turns);
  Luminosity->
    setReadoutScaler((int) rundb.A.Prescaler, (int) rundb.A.ScalerSingle,
		     (int) rundb.A.ScalerDouble, (int) rundb.A.PreStrobe,
		     (double) rundb.A.StrobeDead,
		     (int) rundb.B.Prescaler, (int) rundb.B.ScalerSingle,
		     (int) rundb.B.ScalerDouble, (int) rundb.B.PreStrobe,
		     (double) rundb.B.StrobeDead,
		     (int) rundb.C.Prescaler, (int) rundb.C.ScalerSingle,
		     (int) rundb.C.ScalerDouble, (int) rundb.C.PreStrobe,
		     (double) rundb.C.StrobeDead, (int) rundb.kaos.ScalerSingle, 
		     (int) rundb.kaos.ScalerDouble);
  
  npol = new Cnpol(&(abcn.npol),&(online.npol),&verbose);
 
  cerA = new cerenkovAC(&online.A.Cerenkov, &spekA.det.trigger.cerenkov,
			&rundb.A.cerenkov); 
  cerB = new cerenkovB (&online.B.Cerenkov, &spekB.det.trigger.cerenkov,
			&rundb.B.cerenkov); 
  cerC = new cerenkovAC(&online.C.Cerenkov, &spekC.det.trigger.cerenkov,
			&rundb.C.cerenkov);

  if (focus) delete focus;
  focus = new Focus(rundb.A.wobbler.x0, rundb.A.wobbler.dx / rundb.Ebeam,
	            rundb.A.wobbler.y0, rundb.A.wobbler.dy / rundb.Ebeam, 
                    rundb.B.wobbler.x0, rundb.B.wobbler.dx / rundb.Ebeam,
	            rundb.B.wobbler.y0, rundb.B.wobbler.dy / rundb.Ebeam, 
                    rundb.C.wobbler.x0, rundb.C.wobbler.dx / rundb.Ebeam,
	            rundb.C.wobbler.y0, rundb.C.wobbler.dy / rundb.Ebeam, 
		    rundb.A.angle * 0.017453293, 
		    rundb.B.angle * 0.017453293, 
		    rundb.C.angle * 0.017453293,
		    rundb.B.oopangle * 0.017453293);

  // Initialize vdcA:
  for (int p = 0; p < 16; p++) {
    vdcA->x1Plane()->InitPaddleOffset(short(rundb.A.vdc.x1.paddle[p]), p);
    vdcA->s1Plane()->InitPaddleOffset(short(rundb.A.vdc.s1.paddle[p]), p);
    vdcA->x2Plane()->InitPaddleOffset(short(rundb.A.vdc.x2.paddle[p]), p);
    vdcA->s2Plane()->InitPaddleOffset(short(rundb.A.vdc.s2.paddle[p]), p);
  }
  vdcA->x1Plane()->InitOffset(short(rundb.A.vdc.offset[0]));
  vdcA->s1Plane()->InitOffset(short(rundb.A.vdc.offset[1]));
  vdcA->x2Plane()->InitOffset(short(rundb.A.vdc.offset[2]));
  vdcA->s2Plane()->InitOffset(short(rundb.A.vdc.offset[3]), 0, 320);
  // The last preamplifier card of the S2 plane has a longer cable
  vdcA->s2Plane()->InitOffset(short(rundb.A.vdc.offset[3]) - 120, 320, 16);
  // Get additional offsets from the configuration file
  LoadVDCConfiguration(rundb.A.vdc.initfile, "A", vdcA->vdcPacket());
  vdcA->vdcPacket()->InitTimeToLength(rundb.A.vdc.driftvelocity,
				      rundb.A.vdc.driftvelocity*rundb.A.vdc.corr_driftvel,
				      rundb.A.vdc.driftvelocity,
				      rundb.A.vdc.driftvelocity*rundb.A.vdc.corr_driftvel);
  vdcA->vdcPacket()->InitMinimumDrift(rundb.A.vdc.mindrift);
  vdcA->vdcPacket()->InitIgnoreCriteria((int)rundb.A.vdc.x.mode, rundb.A.vdc.x.cut[0],
                           rundb.A.vdc.x.cut[1],    rundb.A.vdc.x.cut[2],
                           (int)rundb.A.vdc.s.mode, rundb.A.vdc.s.cut[0],
                           rundb.A.vdc.s.cut[1],    rundb.A.vdc.s.cut[2]);
  // Initialize hdcA
  if (rundb.hdcA.configfile) hdcA->InitHDC(rundb.hdcA); 
  hdcA->Reset();

  // Initialize vdcB:
  for (int p = 0; p < 16; p++) {
    vdcB->x1Plane()->InitPaddleOffset(short(rundb.B.vdc.x1.paddle[p]), p);
    vdcB->s1Plane()->InitPaddleOffset(short(rundb.B.vdc.s1.paddle[p]), p);
    vdcB->x2Plane()->InitPaddleOffset(short(rundb.B.vdc.x2.paddle[p]), p);
    vdcB->s2Plane()->InitPaddleOffset(short(rundb.B.vdc.s2.paddle[p]), p);
  }
  vdcB->x1Plane()->InitOffset(short(rundb.B.vdc.offset[0]));
  vdcB->s1Plane()->InitOffset(short(rundb.B.vdc.offset[1]));
  vdcB->x2Plane()->InitOffset(short(rundb.B.vdc.offset[2]), 32, BX2SIZE - 32);
  // The first two cards of the x2 layer have smaller offsets
  vdcB->x2Plane()->InitOffset(short(rundb.B.vdc.offset[2]) - 12, 0, 32);
  vdcB->s2Plane()->InitOffset(short(rundb.B.vdc.offset[3]));
  // Get additional offsets from the configuration file
  LoadVDCConfiguration(rundb.B.vdc.initfile, "B", vdcB->vdcPacket());
  vdcB->vdcPacket()->InitTimeToLength(rundb.B.vdc.driftvelocity,
				      rundb.B.vdc.driftvelocity*rundb.B.vdc.corr_driftvel,
				      rundb.B.vdc.driftvelocity,
				      rundb.B.vdc.driftvelocity*rundb.B.vdc.corr_driftvel);
  vdcB->vdcPacket()->InitMinimumDrift(rundb.B.vdc.mindrift);
  vdcB->vdcPacket()->InitIgnoreCriteria((int)rundb.B.vdc.x.mode, rundb.B.vdc.x.cut[0],
                           rundb.B.vdc.x.cut[1],    rundb.B.vdc.x.cut[2],
                           (int)rundb.B.vdc.s.mode, rundb.B.vdc.s.cut[0],
                           rundb.B.vdc.s.cut[1],    rundb.B.vdc.s.cut[2]);

  // Initialize vdcC:
  for (int p = 0; p < 16; p++) {
    vdcC->x1Plane()->InitPaddleOffset(short(rundb.C.vdc.x1.paddle[p]), p);
    vdcC->s1Plane()->InitPaddleOffset(short(rundb.C.vdc.s1.paddle[p]), p);
    vdcC->x2Plane()->InitPaddleOffset(short(rundb.C.vdc.x2.paddle[p]), p);
    vdcC->s2Plane()->InitPaddleOffset(short(rundb.C.vdc.s2.paddle[p]), p);
  }
  vdcC->x1Plane()->InitOffset(short(rundb.C.vdc.offset[0]));
  vdcC->s1Plane()->InitOffset(short(rundb.C.vdc.offset[1]));
  vdcC->x2Plane()->InitOffset(short(rundb.C.vdc.offset[2]));
  vdcC->s2Plane()->InitOffset(short(rundb.C.vdc.offset[3]));
  // Get additional offsets from the configuration file
  LoadVDCConfiguration(rundb.C.vdc.initfile, "C", vdcC->vdcPacket());
  vdcC->vdcPacket()->InitTimeToLength(rundb.C.vdc.driftvelocity,
				      rundb.C.vdc.driftvelocity*rundb.C.vdc.corr_driftvel,
				      rundb.C.vdc.driftvelocity,
				      rundb.C.vdc.driftvelocity*rundb.C.vdc.corr_driftvel);
  vdcC->vdcPacket()->InitMinimumDrift(rundb.C.vdc.mindrift);
  vdcC->vdcPacket()->InitIgnoreCriteria((int)rundb.C.vdc.x.mode, rundb.C.vdc.x.cut[0],
					rundb.C.vdc.x.cut[1],    rundb.C.vdc.x.cut[2],
					(int)rundb.C.vdc.s.mode, rundb.C.vdc.s.cut[0],
					rundb.C.vdc.s.cut[1],    rundb.C.vdc.s.cut[2]);
  
  // delete old trace machines before creating new one's  
  delete tmaA; delete tmaB; delete tmaC; delete tmaD; delete tmaK; delete tmaSciFi;
  
  if (rundb.A.tma.type==1) tmaA=new SPLINE(rundb.A.tma.filename,rundb.A.tma.firstorder); else
      tmaA = new TMAC(rundb.A.tma.filename);
  if (rundb.B.tma.type==1) tmaB=new SPLINE(rundb.B.tma.filename,rundb.B.tma.firstorder); else
      tmaB = new TMAC(rundb.B.tma.filename);
  if (rundb.C.tma.type==1) tmaC=new SPLINE(rundb.C.tma.filename,rundb.C.tma.firstorder); else
      tmaC = new TMAC(rundb.C.tma.filename);
  if (rundb.D.tma.filename==NULL) 
    std::cerr << "No TMA file for spektrometer D given in run.db."
	      <<std::endl
	      << "Use for example" 
	      <<std::endl
	      << "   D.tma.file = \"/usr/local/a1/share/tma/pspek_66.mxl\""
	      <<std::endl;
  if (rundb.D.tma.type==1) tmaD=new SPLINE(rundb.D.tma.filename,rundb.D.tma.firstorder); else
      tmaD = new TMAC(rundb.D.tma.filename);

  if (rundb.kaos.tma.type) // this type is for using KaosMatrixManager
    {
      if (rundb.kaos.tma.filename!=NULL)
	{
	  int filesokay = 1;

	  char geantino_filename[80] = {};
	  strcpy(geantino_filename, rundb.kaos.tma.filename);
	  strncat(geantino_filename, "_geantino", 15);
	  FILE *file =fopen(geantino_filename, "r");
	  if (file) fclose(file); else filesokay = 0;

	  char pca1_filename[80] = {};
	  strcpy(pca1_filename, rundb.kaos.tma.filename);
	  strncat(pca1_filename, "_pca1.param", 11);
	  file =fopen(pca1_filename, "r");
	  if (file) fclose(file); else filesokay = 0;
	  
	  char pca2_filename[80] = {};
	  strcpy(pca2_filename, rundb.kaos.tma.filename);
	  strncat(pca2_filename, "_pca2.param", 11);
	  file =fopen(pca2_filename, "r");
	  if (file) fclose(file); else filesokay = 0;

	  if (!filesokay) {
	    cout << "Error: KaosMatrixManager needs " << endl; 
	    cout << geantino_filename << endl;
	    cout << pca1_filename << endl;
	    cout << pca2_filename << endl;
	  }
	  else
	    {
	      cout << "Info: KaosMatrixManager uses " << endl; 
	      cout << geantino_filename << endl;
	      cout << pca1_filename << endl;
	      cout << pca2_filename << endl;

	      // geantino-based matrix
	      matrixK = new KaosMatrixManager(geantino_filename,
					      pca1_filename,
					      pca2_filename);

	      // if rundb.kaos.tma.type > 1 
	      // using particle-specific corrections to the geantino-based matrix
	      if (rundb.kaos.tma.type == 2) 
		{
		  char pion_filename[80] = {""};
		  strcpy(pion_filename, rundb.kaos.tma.filename);
		  strncat(pion_filename, "_pion.param.1", 13);
		  file =fopen(pion_filename, "r");
		  if (file) fclose(file); else filesokay = 0;

		  if (!filesokay) {
		    cout << "Error: KaosMatrixManager needs " << endl; 
		    cout <<  pion_filename << endl;
		  }
		  else matrixK->SetMatrixFile(pion_filename);
		}
	      else if (rundb.kaos.tma.type == 3)
		{
		  char kaon_filename[80] = {""};
		  strcpy(kaon_filename, rundb.kaos.tma.filename);
		  strncat(kaon_filename, "_kaon.param.1", 13);
		  file =fopen(kaon_filename, "r");
		  if (file) fclose(file); else filesokay = 0;

		  if (!filesokay) {
		    cout << "Error: KaosMatrixManager needs " << endl; 
		    cout <<  kaon_filename << endl;
		  }
		  else matrixK->SetMatrixFile(kaon_filename);
		}
	      else if (rundb.kaos.tma.type == 4)
		{
		  char proton_filename[80] = {""};
		  strcpy(proton_filename, rundb.kaos.tma.filename);
		  strncat(proton_filename, "_proton.param.1", 15);
		  file =fopen(proton_filename, "r");
		  if (file) fclose(file); else filesokay = 0;

		  if (!filesokay) {
		    cout << "Error: KaosMatrixManager needs " << endl; 
		    cout <<  proton_filename << endl;
		  }
		  else matrixK->SetMatrixFile(proton_filename);
		}
	    }
	}
    }
  else
    if (rundb.kaos.tma.filename!=NULL)
      tmaK = new TMAC(rundb.kaos.tma.filename); // Standard TMA

  if (rundb.SciFi.tma.filename!=NULL)
    {
      tmaSciFi = new TMAC(rundb.SciFi.tma.filename); // Standard TMA
    }

  // initialize tma by overwriting the offsets defined in matrix file
  // disable this behavior by seting the theta offset to zero
  // this should be a sufficient trigger because the theta offset
  // is around pi/4 = 785.4mrad on average.
  if (rundb.A.tma.theta != 0.0)
    tmaA->setOffsets(0.1*rundb.A.tma.x, rundb.A.tma.theta,
		     0.1*rundb.A.tma.y, rundb.A.tma.phi);
  if (rundb.B.tma.theta != 0.0)
    tmaB->setOffsets(0.1*rundb.B.tma.x, rundb.B.tma.theta,
		     0.1*rundb.B.tma.y, rundb.B.tma.phi);
  if (rundb.C.tma.theta != 0.0)
    tmaC->setOffsets(0.1*rundb.C.tma.x, rundb.C.tma.theta,
		     0.1*rundb.C.tma.y, rundb.C.tma.phi);
  if (rundb.D.tma.theta != 0.0)
    tmaD->setOffsets(0.1*rundb.D.tma.x, rundb.D.tma.theta,
		     0.1*rundb.D.tma.y, rundb.D.tma.phi);
  if (tmaK) tmaK->setOffsets(0, 0, 0, 0);
  if (tmaSciFi) tmaSciFi->setOffsets(0, 0, 0, 0);

  steps_IN = steps_A = steps_B = steps_C = steps_D = steps_K = 1;
  if (ergloss > 1) {      
    steps_A = steps_B = steps_C = steps_D = steps_K = 5;
    if (Reaction->getA() != NULL) {
      if (*Reaction->getA() == P_electron) steps_A =   1;
      if (*Reaction->getA() == P_proton)   steps_A =  10;
      if (*Reaction->getA() == P_deuteron) steps_A = 100;
    }
    if (Reaction->getB() != NULL) {
      if (*Reaction->getB() == P_electron) steps_B =   1;
      if (*Reaction->getB() == P_proton)   steps_B =  10;
      if (*Reaction->getB() == P_deuteron) steps_B = 100;
    }
    if (Reaction->getC() != NULL) {
      if (*Reaction->getC() == P_electron) steps_C =   1;
      if (*Reaction->getC() == P_proton)   steps_C =  10;
      if (*Reaction->getC() == P_deuteron) steps_C = 100;
    }
    if (Reaction->getD() != NULL) {
      if (*Reaction->getD() == P_electron) steps_C =   1;
      if (*Reaction->getD() == P_proton)   steps_C =  10;
      if (*Reaction->getD() == P_deuteron) steps_C = 100;
    }
    if (Reaction->getKAOS() != NULL) {
      if (*Reaction->getKAOS() == P_electron) steps_K =   1;
      if (*Reaction->getKAOS() == P_proton)   steps_K =  10;
      if (*Reaction->getKAOS() == P_deuteron) steps_K = 100;
    }
  }
  
  switch ((int) rundb.use_ab_tdc) {
  case 1 :  abtime = &abtime_1; break;
  case 2 :  abtime = &abtime_2; break;
  case 3 :  abtime = &abtime_3; break;
  default : abtime = &abtime_1;
  }
  switch ((int) rundb.use_ac_tdc) {
  case 1 :  catime = &actime_1; break;
  case 2 :  catime = &actime_2; break;
  case 3 :  catime = &actime_3; break;
  case 4 :  catime = &catime_1; break;
  case 5 :  catime = &catime_2; break;
  case 6 :  catime = &catime_3; break;
  default : catime = &actime_1;
  }
  switch ((int) rundb.use_bc_tdc) {
  case 1 :  bctime = &cbtime_1; break;
  case 2 :  bctime = &cbtime_2; break;
  case 3 :  bctime = &cbtime_3; break;
  default : bctime = &cbtime_1;
  }      
  
  // for pulsed Singles
  switch ((int) rundb.use_a_tdc) {
  case 1 :  a_time = &abtime_2; break;
  default : a_time = &abtime_2;
  }
  switch ((int) rundb.use_b_tdc) {
  case 1 :  b_time = &btime_1; break;
  default : b_time = &btime_1;
  }
  switch ((int) rundb.use_c_tdc) {
  case 1 :  c_time = &catime_2; break;
  default : c_time = &catime_2;
  }

  // Cleans ntuples
  nt->delete_all();

  // place to create ntuple
  if (ntnvar) {
    nt->InitNtuple(ntiden, nttitle, ntnvar, ntvarn, ntdes, ntbck, online_rz);
  }

  // Checks existence of ntuple
  if ( nt->head ) { 
    nt = nt->head;
    ntuple = nt->nelt;
  } else {
    ntuple = 0;
  }

  // rewind inputfile
  // if (dorewind) atree->rewind(); 
  
  // compare run.db entries with readout mezzo values...

  new checkMezzoRundb(mezzoReader,"beam.energy","Ebeam",rundb.Ebeam,20);
  new checkMezzoRundb(mezzoReader,"beam.turns","Turns",rundb.turns);
  if (Reaction->getA() != NULL)
    new checkMezzoRundb(mezzoReader,"a.angle","A.Angle",rundb.A.angle);
  new checkMezzoRundb(mezzoReader,"b.angle","B.Angle",rundb.B.angle);
  if (Reaction->getA() != NULL)
    new checkMezzoRundb(mezzoReader,"a.trigger.logic.strobe",
			"A.scint.use_dE",rundb.A.scint.use_dE);
  new checkMezzoRundb(mezzoReader,"b.trigger.logic.strobe",
		      "B.scint.use_dE",rundb.B.scint.use_dE);
  new checkMezzoRundb(mezzoReader,"trigger.scaling.single.a",
		      "A.Scaler.Single",rundb.A.ScalerSingle);
  new checkMezzoRundb(mezzoReader,"trigger.scaling.single.b",
		      "B.Scaler.Single",rundb.B.ScalerSingle);
  new checkMezzoRundb(mezzoReader,"trigger.scaling.double.ab",
		      "A.Scaler.Double",rundb.A.ScalerDouble);

  // update window title/status line etc.

#ifdef USE_QT
  
#else
  if (frame) { 
    if (frame->HMVhistogram) {
      if (!(frame->HMVhistogram = HMFind(frame->histos->GetLabel())))
	frame->HMVhistogram = HMFirst;
      frame->histos->SetLabel((char *) frame->HMVhistogram->title);
    }

    
    static char *title = new char[255];
    long int datatime = atree->getTimestamp();

    sprintf(title,"Cindy++ Online Analysis %s",rundb.additional_title);
    if (atree->getRunName())
      sprintf(title, "Cindy++ Online Analysis of \"%s\" %s",
	      atree->getRunName(),
	      rundb.additional_title
	  );
    //    else 
    //    if (atree->getInputFilename())
    //      sprintf(title, "Cindy++ Online Analysis of \"%s\" (%s,%s)",
    //	atree->getInputFilename(), rundb.setup,
    //	ctime((const time_t *) &datatime));
  
    frame->SetTitle(title);       
    frame->SetStatusText("Events: 0");
    //    sprintf(frame->hmoptions = new char[255],
    //	    "-D RUN=\"%s\" -D SETUP=\"%s\" -D DATATIME=\"%s\"",
    //	    (atree->getRunName() ? atree->getRunName() : atree->getInputFilename()), 
    //	    rundb.setup, ctime((const time_t *) &datatime));
    sprintf(frame->hmoptions = new char[255],
	    "-S RUN=\"%s\" -S SETUP=\"%s\" -S DATATIME=\"%s\"",
	    atree->getRunName(),rundb.setup,ctime((const time_t *) &datatime));
  } 
#endif // USE_QT
}

///////////////////////////////////////////////////////////////////////////////
// This is the main event loop
///////////////////////////////////////////////////////////////////////////////
int 
Cola::eventloop()
{  
  //std::cout << "Event " << events << std::endl;
  struct MpsDataA& spekA = abcn.a;
  struct MpsDataB& spekB = abcn.b;
  struct MpsDataC& spekC = abcn.c;

  static int oldevents=-1;

  out->clear();

  if (passPerEvent == 0)
  { // in case of multi track analysis only read next event when
    // no track remains to be analysed
    switch (reader->nextEvent(atree)) {
    case  1: 
      if (events!=oldevents) { 
        printStatus();
        oldevents=events;
      };
      usleep(100); //force context switch to reduce CPU load
      return 0;                 break; // timeOut
    case  0:
      if (! maxevents || events < maxevents)  break;
    case -1:
      finish(); return 1;                   // end the Callback loop
    }

    if (dorewind || events == 0) {  // Reinitialization, i.e. start or rewind
      readInputFiles(dorewind);
      dorewind = 0; 
    }
  }
  
  sigset_t set;             // Signal Hangup -> write histogram files
  sigemptyset(&set);
  sigpending(&set);
  if (sigismember(&set, SIGHUP)) {
    env_def(online_hup, "ONLINE_HUP", "Online.his.hup");
    signal(SIGHUP, SIG_IGN);
    std::cerr << std::endl << "Signal HUP: Writing \""
	      << online_hup << "\"...";
    HMWriteAll(online_hup); 
    if (ntuple) nt->delete_ntlist();
    std::cerr << "done." << std::endl;
  }
  
  // -mod- moved up: out->clear();

  Luminosity->handle(&abcn);
  if (events < skipevents) {
    if (!(++events % 500)) printStatus();  // update status line
    vdcA->Reset();
    hdcA->Reset();
    vdcB->Reset();
    vdcC->Reset();
    PdcD->Reset();
    return 0;
  }       
  
  online.nr = events;
  out->packEventData(&online.nr, 1);

  online.IntegratedLuminosity = Luminosity->getIntegrated() / 1e6;
  online.RunTime = Luminosity->getRunTime();
  online.datataking = ((double)atree->getTimestamp())/3600.0;
  static double starttm = online.datataking * 3600; 
  online.datataking2 = ((double)atree->getTimestamp()) - starttm;
  out->packEventData(&online.datataking, 1);
  out->packEventData(&online.datataking2, 1);
  out->packEventData(&online.IntegratedLuminosity, 1);
  out->packEventData(&online.RunTime, 1);

  //std::cout << "Time" << std::endl;
  //std::cout << online.datataking << std::endl;
  //std::cout << online.datataking2 << std::endl;

  Particle A_Out = P_proton;
  Particle B_Out = P_electron;
  Particle C_Out = P_electron;
  Particle D_Out = P_electron;
  Particle O_Out = P_proton;
  Particle H3_Out = P_proton;
  Particle TOF_Out = P_neutron;
  Particle NPOL_Out = P_neutron;
  Particle NDET_Out = P_neutron;
  Particle SI_Out = P_proton; 
  Particle KAOS_Out = P_kplus;

  if (Reaction->getA() != NULL)    A_Out   = *Reaction->getA();
  if (Reaction->getB() != NULL)    B_Out   = *Reaction->getB();
  if (Reaction->getC() != NULL)    C_Out   = *Reaction->getC();
  if (Reaction->getD() != NULL)    D_Out   = *Reaction->getD();
  if (Reaction->getO() != NULL)    O_Out   = *Reaction->getO();
  if (Reaction->getH3() != NULL)   H3_Out  = *Reaction->getH3();
  if (Reaction->getTOF() != NULL)  TOF_Out = *Reaction->getTOF();
  if (Reaction->getNPOL() != NULL) NPOL_Out = *Reaction->getNPOL();
  if (Reaction->getNDET() != NULL) NDET_Out = *Reaction->getNDET();
  if (Reaction->getSI() != NULL)   SI_Out = *Reaction->getSI();
  if (Reaction->getKAOS() != NULL) KAOS_Out = *Reaction->getKAOS();

  ////////////////////////////////////////////////////////////////////////////
  // Check of Sync Info data
  ////////////////////////////////////////////////////////////////////////////

  int info = 0;
  int a_info = 0;
  int b_info = 0;
  int c_info = 0;

  if (atree->itemOK(&spekA.det.sync_info))
    a_info = spekA.det.sync_info & 0xffff;
  if (atree->itemOK(&spekB.det.sync_info))
    b_info = spekB.det.sync_info & 0xffff; 
  if (atree->itemOK(&spekC.det.sync_info))
    c_info = spekC.det.sync_info & 0xffff;

  if (atree->itemOK(&abcn.d.sync_info))
    b_info = abcn.d.sync_info & 0xffff;		// overwrite B

  if (atree->itemOK(&abcn.kaos.sync_info))
    a_info = abcn.kaos.sync_info & 0xffff;	// KAOS runs as SpekA

  if (atree->itemOK(&abcn.n.sync_info))
    c_info = abcn.n.sync_info & 0xffff;	        // Ndet2016 runs as SpekC
    
  if (atree->itemOK(&spekA.det.sync_info) && Reaction->getKAOS() != NULL)
    b_info = spekA.det.sync_info & 0xffff;	// overwrite B for KAOS && A 
  
  if (atree->itemOK(&abcn.hadron3.label))
    a_info = abcn.hadron3.label & 0xffff;	// overwrite A

  if (atree->itemOK(&abcn.tof.label))
    c_info = abcn.tof.label & 0xffff;		// overwrite C

  if (atree->itemOK(&abcn.si.label))
    //b_info = abcn.si.label & 0xffff;		// Silicon runs as SpekB
    c_info = abcn.si.label & 0xffff;		// Silicon runs as SpekC
 

  int NDET_TO_BE_HANDLED = 0;  
#if HASNDET == 1
  online.N.trigger = 0;
  if (atree->itemOK(&abcn.ndet.det.sync_info)) {
    c_info = abcn.ndet.det.sync_info & 0xEFFF;
    online.N.trigger = (spekA.det.sync_info & 0x8000);
    NDET_TO_BE_HANDLED = 1;
  }
  out->packEventData(&online.N.trigger, 1);
#endif

   if (atree->itemOK(&abcn.npol.sync_info)) {    
    c_info = abcn.npol.sync_info & 0xffff;
   }

   if (atree->itemOK(&abcn.ndet.det.sync_info)) {    
    c_info = abcn.ndet.det.sync_info & 0xffff;
   }

   info = (a_info | b_info | c_info) & 0xE000;
  
   // fibre detector runs as SpekB or SpekC
   if (atree->itemOK(&abcn.kaos.fibre.sync_info) && 
      rundb.SciFi.use_fibre == 2.) b_info = a_info; 
   if (atree->itemOK(&abcn.kaos.fibre.sync_info) && 
      rundb.SciFi.use_fibre == 3.) c_info = a_info; 
   if (atree->itemOK(&abcn.kaos.fibre.sync_info) && 
      rundb.SciFi.use_fibre == 4.) a_info = a_info; 
 
   // In several prototype tests the kaos.fibre sturcture has been used for storing data.
   // This env variable can be used to relay the sync_info bit for coincidences.
   static int KaosFibreSyncBit = (getenv("KAOSFIBRESYNCBIT")?atoi(getenv("KAOSFIBRESYNCBIT")):0);
   if (KaosFibreSyncBit > 0){
     if (events == 1) {
       cerr <<"WARNING: Using abcn.kaos.fibre.sync_info as ";
       if(KaosFibreSyncBit==1) cerr << "A";
       if(KaosFibreSyncBit==2) cerr << "B";
       if(KaosFibreSyncBit==3) cerr << "C";
       cerr << "\n";
     }     
     if(KaosFibreSyncBit==1) a_info = abcn.kaos.fibre.sync_info & 0xffff;
     if(KaosFibreSyncBit==2) b_info = abcn.kaos.fibre.sync_info & 0xffff;
     if(KaosFibreSyncBit==3) c_info = abcn.kaos.fibre.sync_info & 0xffff;
   }

   if (info == 0x6000) {
     if (a_info != b_info) {
       std::cerr << "Inconsistent data: sync_info a != b at"
		 << " ab_double event " << events << std::endl
		 << "Sync A: " << std::hex << std::setw(6) << a_info
		 << "   Sync B: " << std::setw(6) << b_info
		 << std::dec << std::endl;
       if (Luminosity) Luminosity->VDCtrippOff();
       return 0;
     }
   }
  
  if (info == 0xA000) {
    if (a_info != c_info) {
      std::cerr << "Inconsistent data: sync_info a != c at"
		<< " ac_double event " << events << std::endl
		<< "Sync A: " << std::hex << std::setw(6) << a_info
		<< "   Sync C: " << std::setw(6) << c_info
		<< std::dec << std::endl;
      return 0;
    }
  }
  
  if (info == 0xC000) {
    if (b_info != c_info) {
      std::cerr << "Inconsistent data: sync_info b != c at"
		<< " bc_double event " << events << std::endl
		<< "Sync B: " << std::hex << std::setw(6) << b_info
		<< "   Sync C: " << std::setw(6) << c_info
		<< std::dec << std::endl;
      if (!events) events++;
      return 0;
    }
  }
  
  if (info == 0xe000) {
    if ((a_info != b_info) || (b_info != c_info)) {
      std::cerr << "Inconsistent data: sync_info at"
		<< " triple event " << events << std::endl
		<< "Sync A: " << std::hex << std::setw(6) << a_info
		<< "   Sync B: " << std::setw(6) << b_info
		<< "   Sync C: " << std::setw(6) << c_info
		<< std::dec << std::endl;
      return 0;
    }
  }

  static NCubeDet NC(atree, rundb.ncube);
  NC.handle(out, online.ncube);

  ////////////////////////////////////////////////////////////////////////////
  //	Trigger
  ////////////////////////////////////////////////////////////////////////////

  abtime_1 = abtime_2 = abtime_3 = dv;
  cbtime_1 = cbtime_2 = cbtime_3 = dv;
  btime_1 = dv;
  catime_1 = catime_2 = catime_3 = dv;
  online.A.vdcOK = online.B.vdcOK = online.C.vdcOK = 0; 
  online.A.trigger = online.B.trigger = online.C.trigger = online.D.trigger 
    = online.kaos.trigger = 0;
  online.A.p4_recon = online.B.p4_recon = online.C.p4_recon = 0;
  online.A.target_coor_ok = online.B.target_coor_ok = online.C.target_coor_ok = 0;
  online.A.TimeAtTarget = online.B.TimeAtTarget = online.C.TimeAtTarget 
    = online.kaos.TimeAtTarget = 0;

  int AlaserOK=0;
  int BlaserOK=0;
  int ClaserOK=0;
  
  if (atree->itemOK(&spekA.det.sync_info)) {
    if (Reaction->getKAOS() == NULL)
      online.A.trigger = (spekA.det.sync_info & 0x2000);
    else // Kaos runs as A, SpekA runs as SpekB:
      online.A.trigger = (spekA.det.sync_info & 0x4000);

    if (atree->itemOK(&spekA.det.trigger.info)) AlaserOK=spekA.det.trigger.info&1;
    assignitem(abtime_1, spekA.coinc.a_b_time_1);
    assignitem(abtime_2, spekA.coinc.a_b_time_2);
    assignitem(abtime_3, spekA.coinc.a_b_time_3);
    assignitem(actime_1, spekA.coinc.a_c_time_1);
    assignitem(actime_2, spekA.coinc.a_c_time_2);
    assignitem(actime_3, spekA.coinc.a_c_time_3);
    /*
    std::cout << "Spec A trigger" << std::endl;
    std::cout << spekA.coinc.a_b_time_1 << std::endl;
    std::cout << spekA.coinc.a_b_time_2 << std::endl;
    std::cout << spekA.coinc.a_b_time_3 << std::endl;
    std::cout << spekA.coinc.a_c_time_1 << std::endl;
    std::cout << spekA.coinc.a_c_time_2 << std::endl;
    std::cout << spekA.coinc.a_c_time_3 << std::endl;
    */
  }

  if (atree->itemOK(&spekB.det.sync_info)) {
    online.B.trigger = (spekB.det.sync_info & 0x4000);
    if (atree->itemOK(&spekB.det.trigger.info)) BlaserOK=spekB.det.trigger.info&1;
    // for pulsedBeam-SpecBSingles:
    assignitem(btime_1, spekB.det.trigger.scint_top_time);
    /*
    std::cout << "Spec B trigger" << std::endl;
    std::cout << spekB.det.trigger.scint_top_time << std::endl;
    */
  }

  if (atree->itemOK(&spekC.det.sync_info)) {
    online.C.trigger = (spekC.det.sync_info & 0x8000);     
    if (atree->itemOK(&spekC.det.trigger.info)) ClaserOK=spekC.det.trigger.info&1;
    assignitem(catime_1, spekC.coinc.a_c_time_1)
    assignitem(catime_2, spekC.coinc.a_c_time_2);
    assignitem(catime_3, spekC.coinc.a_c_time_3);
    assignitem(cbtime_1, spekC.coinc.b_c_time_1);
    assignitem(cbtime_2, spekC.coinc.b_c_time_2);
    assignitem(cbtime_3, spekC.coinc.b_c_time_3);
  }

  /////////////////////////////////////////////////////////////////////////////
  // KAOS trigger
  /////////////////////////////////////////////////////////////////////////////
  online.kaos.trigger = (abcn.kaos.sync_info & 0x6000); // 0x2000

  /////////////////////////////////////////////////////////////////////////////
  // FIBER trigger
  /////////////////////////////////////////////////////////////////////////////
  online.kaos.scifi.trigger = (abcn.kaos.fibre.sync_info & 0x8000); // 0x8000

  /////////////////////////////////////////////////////////////////////////////
  // Ndet2016 trigger
  /////////////////////////////////////////////////////////////////////////////
  online.ndet.trigger = (abcn.n.sync_info & 0x8000);
  
  //std::cout << events << std::endl;
  //std::cout << online.A.trigger << '\t' << online.B.trigger << '\t' << online.C.trigger << std::endl;
  //std::cout << online.A.p4_recon << '\t' << online.B.p4_recon << '\t' << online.C.p4_recon << std::endl;

  out->packEventData(&online.A.trigger, 1);
  out->packEventData(&online.B.trigger, 1);
  out->packEventData(&online.C.trigger, 1);
  out->packEventData(&online.A.p4_recon, 1);
  out->packEventData(&online.B.p4_recon, 1);
  out->packEventData(&online.C.p4_recon, 1);
  out->packEventData(&online.A.target_coor_ok, 1);
  out->packEventData(&online.B.target_coor_ok, 1);
  out->packEventData(&online.C.target_coor_ok, 1);
  /////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
  // Detector components
  /////////////////////////////////////////////////////////////////////////////
  scintillator(atree,out,online.A, rundb.A, 15, 15, 
	       spekA.det.trigger.scint.de.pad, 
	       spekA.det.trigger.scint.tof.pad, NULL,
	       &spekA.det.trigger.scint.de_tof_time, 'A');
  scintillator(atree,out,online.B, rundb.B, 14, 14, 
	       NULL, spekB.det.trigger.scint.tof.pad,
	       spekB.det.trigger.scint.de.pad,
	       &spekB.det.trigger.scint.de_tof_time, 'B');
  scintillator(atree,out,online.C, rundb.C, 15, 15, 
	       spekC.det.trigger.scint.de.pad, 
	       spekC.det.trigger.scint.tof.pad, NULL,
	       &spekC.det.trigger.scint.de_tof_time, 'C');
  ScintD->handle();

  if (online.kaos.scifi.trigger)
    {
      ///////////////////////////////////////////////////////////////////////// 
      // Kaos Fibre Detector
      ///////////////////////////////////////////////////////////////////////// 
      
      if (rundb.SciFi.use_fibre) SciFi->handle();
    }

  if (online.kaos.trigger)
    {
      ///////////////////////////////////////////////////////////////////////// 
      // Kaos MWPC Detectors
      ///////////////////////////////////////////////////////////////////////// 

      if (rundb.kaos.use_mwpc) {
	KaosMWPC->handle();
	if (rundb.kaos.MWPC_trackDir) {
	  char trackDir[80]; 
	  sprintf(trackDir, "%s", rundb.kaos.MWPC_trackDir);
	  KaosMWPC->draw(trackDir, events);
	}
      }
       
      ///////////////////////////////////////////////////////////////////////// 
      // Kaos TOF Detectors
      ///////////////////////////////////////////////////////////////////////// 
      
      if (rundb.kaos.use_tof)
	passPerEvent =  KaosTOF->handle();
  
      ///////////////////////////////////////////////////////////////////////// 
      // Kaos Fibre Detector
      ///////////////////////////////////////////////////////////////////////// 
      
      if (rundb.SciFi.use_fibre) SciFi->handle();
    }

  if (online.ndet.trigger)
    {
      ///////////////////////////////////////////////////////////////////////// 
      // Ndet2016
      ///////////////////////////////////////////////////////////////////////// 
      
      if (rundb.ndet.use_ndet)
        ndet2016->handle();
    }
  

  /////////////////////////////////////////////////////////////////////////////
  //	Drift Chambers -> Focal Plane coord's -> target coords
  /////////////////////////////////////////////////////////////////////////////

  TargetCo *resultA = NULL, *resultB = NULL, *resultC = NULL,
    *resultD = NULL, *resultK = NULL, *resultSciFi = NULL;

  double xA = 0, yA = 0, tA = 0, pA = 0;
  double xB = 0, yB = 0, tB = 0, pB = 0;
  double xC = 0, yC = 0, tC = 0, pC = 0;
  double xD = 0, yD = 0, tD = 0, pD = 0;
  double xK = 0, yK = 0, tK = 0, pK = 0;
  double xSciFi = 0, ySciFi = 0, tSciFi = 0, pSciFi = 0;

  if (online.A.trigger && !AlaserOK) {
    vdcA->handle();

    if (online.A.vdcOK && vdcA->handle(xA, tA, yA, pA)) 
      if (fabs(xA) < 3000 && fabs(yA) < 1000)  {
	// only run tma if coordinates are reasonable
  online.A.p4_recon=1;
	resultA = tmaA->run(xA, tA, yA, pA);      
	targetFillHisto(&online.A.target, resultA, out);
	online.A.len = resultA->len;
	out->packEventData(&online.A.len);
	if (rundb.A.vdc.trackps) {
	  char trackfn[80]; 
	  sprintf(trackfn, "%s/event_%ld.ps", rundb.A.vdc.trackps, events);
	  vdcA->vdcPacket()->drawTrack(trackfn, "");
	}
      }
  }
  
  if (online.B.trigger && !BlaserOK) {
    vdcB->handle();
    if (online.B.vdcOK && vdcB->handle(xB, tB, yB, pB)) 
      if (fabs(xB) < 3000 && fabs(yB) < 1000) {
      // only run tma if coordinates are reasonable
      online.B.p4_recon=1;
      resultB = tmaB->run(xB, tB, yB, pB);
      targetFillHisto(&online.B.target, resultB, out);
      online.B.len = resultB->len;
      out->packEventData(&online.B.len);
      if (rundb.B.vdc.trackps) {
	char trackfn[80]; 
	sprintf(trackfn, "%s/event_%ld.ps", rundb.B.vdc.trackps, events);
	vdcB->vdcPacket()->drawTrack(trackfn, "");
      }
    }
  }

  if (online.C.trigger && !ClaserOK) {
    vdcC->handle();
    if (online.C.vdcOK && vdcC->handle(xC, tC, yC, pC)) 
      if (fabs(xC) < 3000 && fabs(yC) < 1000) {
      // only run tma if coordinates are reasonable
      resultC = tmaC->run(xC, tC, yC, pC);
      targetFillHisto(&online.C.target, resultC, out);
      online.C.len = resultC->len;
      out->packEventData(&online.C.len);
      if (rundb.C.vdc.trackps) {
	char trackfn[80]; 
	sprintf(trackfn, "%s/event_%ld.ps", rundb.C.vdc.trackps, events);
	vdcC->vdcPacket()->drawTrack(trackfn, "");
      }
    }
  }  

  if (online.D.trigger) {
    PdcD->handle();
    if (online.D.pdcOK) {
      xD = online.D.x;
      yD = online.D.y;
      tD = online.D.theta;
      pD = online.D.phi;
      if (fabs(xD) < 1000 && fabs(yD) < 300)  {
	// only run tma if coordinates are reasonable
	resultD = tmaD->run(xD, tD, yD, pD);      
	targetFillHisto(&online.D.target, resultD, out);
	online.D.len = resultD->len;
	out->packEventData(&online.D.len);
      }
    }
  }
 
  int tmaOK = 0;
  if (rundb.kaos.use_mwpc && rundb.kaos.use_track == 1)
    {if (out->find("online.kaos.mwpc.quality")->scalar() 
	 > rundb.kaos.min_quality)
	{tmaOK = MWPCK->handle(xK, tK, yK, pK);}
    }
  if (rundb.kaos.use_tof && rundb.kaos.use_track == 2) 
    {
      if ((out->find("online.kaos.tof.quality")->scalar() > rundb.kaos.min_quality) || 
	  (rundb.kaos.use_multi_track == 1))
      {
	tmaOK = TOFK->handle(xK, tK, yK, pK);
      }
    }
  if (rundb.kaos.use_tof && rundb.kaos.use_track == 3) 
    {
      if ((out->find("online.kaos.tof.quality")->scalar() > rundb.kaos.min_quality) || 
	  (rundb.kaos.use_multi_track == 1))
      {
	tmaOK = KAOSK->handle(xK, tK, yK, pK);
      }
    }
  if (rundb.kaos.use_mwpc && rundb.kaos.use_track == 4) 
    {if (out->find("online.kaos.mwpc.quality")->scalar() 
	 > rundb.kaos.min_quality)
	{tmaOK = KAOSK->handle(xK, tK, yK, pK);}
    }

  if (online.kaos.trigger && tmaOK && 
      ( (!rundb.kaos.tma.type && tmaK) || 
	rundb.kaos.tma.type) )
    {
      // focal plane coordinates
      online.kaos.coord.x  = xK; // mm
      online.kaos.coord.th = tK;
      online.kaos.coord.y  = yK; // mm
      online.kaos.coord.ph = pK;

      out->packEventData(&online.kaos.coord.x);
      out->packEventData(&online.kaos.coord.th);
      out->packEventData(&online.kaos.coord.y);
      out->packEventData(&online.kaos.coord.ph);
      
      /** dp: Momentum [difference to reference]        */
      double dp = 0;
      /** th: theta_0 (positive to A) [mrad]            */
      double th = 0;
      /** y0: y0 [cm]                                   */
      double y0 = 0; y0=y0; //bss: to get rid of the  warning: variable 'y0' set but not used
      /** ph: phi0 [mrad]                               */
      double ph = 0;
      /** len: pathlength in spectrometer [cm]          */
      double len = 0;
	
      if (fabs(xK) < 1000 && fabs(yK) < 1000) {
      // only run tma if fp coordinates are reasonable

	if (rundb.kaos.tma.type) 
	  {
	    double inputK[4] = {xK, tK, yK, pK};
	    double outputK[4]= {0,  0,  0,  0};

	    matrixK->Calc(inputK,outputK); 
	    
	    // target coordinates
	    dp = online.kaos.target.dp = (outputK[0])/100;  // norm
	    len= online.kaos.target.len= (outputK[1]);      // cm
	    th = online.kaos.target.th = (outputK[2])/1000; // rad
	    ph = online.kaos.target.ph = (outputK[3])/1000; // rad
	    y0 = online.kaos.target.y0 = 0;                 // cm
	  }
	else
	  {
	    resultK = tmaK->run(xK, tK, yK, pK);      

	    if (rundb.kaos.use_track > 2) 
	      {
		// target coordinates
		dp = online.kaos.target.dp = (resultK->dp)/100; // norm
		th = online.kaos.target.th = (resultK->th)/1000;// rad
		y0 = online.kaos.target.y0 = resultK->y0;       // cm
		ph = online.kaos.target.ph = (resultK->ph)/1000;// rad
	      }
	    else
	      {
		// target coordinates
		dp = online.kaos.target.dp = resultK->dp;
		th = online.kaos.target.th = (resultK->th);     // rad
		y0 = online.kaos.target.y0 = resultK->y0;       // cm
		ph = online.kaos.target.ph = (resultK->ph)/10;  // rad
	      }
	    
	    len = online.kaos.target.len= resultK->len;      // cm	    
	  }
	
	out->packEventData(&online.kaos.target.dp);
	out->packEventData(&online.kaos.target.th);
	out->packEventData(&online.kaos.target.y0);
	out->packEventData(&online.kaos.target.ph);
	out->packEventData(&online.kaos.target.len);
	    
	KAOS_Out = particle(KAOS_Out.getMass(), 
			    kMomentum*(1+dp),
			    -kAngle, -th, -ph);
	
	double veloK             = V_LIGHT * KAOS_Out.beta().abs();

	online.kaos.TimeAtTarget = len/veloK;

	online.kaos.CoincTimeAtTarget = online.kaos.xtra.time[0]*0.025 
	  - online.kaos.gdet.time - len/veloK;
	
	out->packEventData(&online.kaos.TimeAtTarget);
	out->packEventData(&online.kaos.CoincTimeAtTarget);

	if (ergloss) { 
	  double EnergyK_Before = KAOS_Out.energy();
	  if (!rundb.kaos.vacuum) target->EnergyLossCorrKaos(KAOS_Out);
	  target->EnergyLossCorr(KAOS_Out, 
				 online.Vertex.x - rundb.Target.offset.x,
				 online.Vertex.y - rundb.Target.offset.y,
				 online.Vertex.z - rundb.Target.offset.z,
				 steps_K);
	  online.kaos.Eloss_corr = (KAOS_Out.energy() - EnergyK_Before);
	  out->packEventData(&online.kaos.Eloss_corr, 1);
	}
      }
    }

  if (online.kaos.scifi.trigger && tmaSciFi)
    {
      // focal plane coordinates
      double Dscifi = 0.83;
      xSciFi= (online.kaos.scifi.x.hitLeadingChannel - 1152) * Dscifi 
	* cos(1.247); // mm;
      tSciFi= (atan((online.kaos.scifi.theta.hitLeadingChannel * Dscifi
		     - online.kaos.scifi.x.hitLeadingChannel
		     * Dscifi + 602.7) / 259.1) - 1.247) * 1000;
      online.kaos.scifi.coord.x  = xSciFi; // mm
      online.kaos.scifi.coord.th = tSciFi;
      online.kaos.scifi.coord.y  = 0; // mm
      online.kaos.scifi.coord.ph = 0;

      out->packEventData(&online.kaos.scifi.coord.x);
      out->packEventData(&online.kaos.scifi.coord.th);
      out->packEventData(&online.kaos.scifi.coord.y);
      out->packEventData(&online.kaos.scifi.coord.ph);

      if (fabs(xSciFi) < 1000) {
      // only run tma if fp coordinates are reasonable

	resultSciFi = tmaSciFi->run(xSciFi, tSciFi, ySciFi, pSciFi);      
	/** dp: Momentum [difference to reference]        */
	/** th: theta_0 (positive to A) [mrad]            */
	/** y0: y0 [cm]                                   */
	/** ph: phi0 [drad]                               */
	/** len: pathlength in spectrometer [cm]          */
	
	// target coordinates
	online.kaos.scifi.target.dp = resultSciFi->dp;
	online.kaos.scifi.target.th = (resultSciFi->th);     // rad
	online.kaos.scifi.target.y0 = resultSciFi->y0;       // cm
	online.kaos.scifi.target.ph = (resultSciFi->ph);     // rad
	online.kaos.scifi.target.len= resultSciFi->len;      // cm

	out->packEventData(&online.kaos.scifi.target.dp);
	out->packEventData(&online.kaos.scifi.target.th);
	out->packEventData(&online.kaos.scifi.target.y0);
	out->packEventData(&online.kaos.scifi.target.ph);
	out->packEventData(&online.kaos.scifi.target.len);
     }
    }
  
  if (resultA && fabs(resultA->dp)>100) {resultA=NULL;online.A.trigger=0;}
  if (resultB && fabs(resultB->dp)>100) {resultB=NULL;online.B.trigger=0;}
  if (resultC && fabs(resultC->dp)>100) {resultC=NULL;online.C.trigger=0;}
  if (resultD && fabs(resultD->dp)>100) {resultD=NULL;online.D.trigger=0;}

  if ( !rundb.kaos.tma.type && resultK && fabs(resultK->dp)>100) 
    {resultK=NULL;online.kaos.trigger=0;}

  if (resultSciFi && fabs(resultSciFi->dp)>100) 
    {resultSciFi=NULL;online.kaos.scifi.trigger=0;}

  /////////////////////////////////////////////////////////////////////////////
  //	Cerenkov
  /////////////////////////////////////////////////////////////////////////////

  cerA->rawdata(atree, vdcA, online.nr);
  cerB->rawdata(atree, vdcB, online.nr);
  cerC->rawdata(atree, vdcC, online.nr);
  
  out->packEventData(&online.A.Cerenkov.hit);
  out->packEventData(&online.A.Cerenkov.sum);
  out->packEventData(&online.A.Cerenkov.pattern);
  out->packEventData(&online.B.Cerenkov.hit);
  out->packEventData(&online.B.Cerenkov.sum);
  out->packEventData(&online.B.Cerenkov.pattern);
  out->packEventData(&online.C.Cerenkov.hit);
  out->packEventData(&online.C.Cerenkov.sum);
  out->packEventData(&online.C.Cerenkov.pattern);
  
  out->packEventData(&online.A.Cerenkov.FloatedPed[0], 12);
  out->packEventData(&online.B.Cerenkov.FloatedPed[0], 12);
  out->packEventData(&online.C.Cerenkov.FloatedPed[0], 12);

  /////////////////////////////////////////////////////////////////////////////
  // beam helicity:
  //
  // is taken from the 2 MSBs of the first found spekX.det.trigger.info 
  // and has 5 possible values:
  // online.Beam.helicity = 1: h+, -1: h-, 0/2:both bits unset/set, -2:no info
  /////////////////////////////////////////////////////////////////////////////
  short heliVal = -2;
  if (atree->itemOK(&spekA.det.trigger.info))
    heliVal = (spekA.det.trigger.info >> 14) & 0x0003;
  else if (atree->itemOK(&spekB.det.trigger.info))
    heliVal = (spekB.det.trigger.info >> 14) & 0x0003;
  else if (atree->itemOK(&spekC.det.trigger.info))
    heliVal = (spekC.det.trigger.info >> 14) & 0x0003;
  if (heliVal == -2) online.Beam.helicity = -2;
  else {
    if (heliVal & 2) online.Beam.helicity = heliVal-1; 
    else online.Beam.helicity = -1*heliVal; 
  }
  out->packEventData(&online.Beam.helicity,1);

  /////////////////////////////////////////////////////////////////////////////
  // HDC raw data and polarization analysis:
  /////////////////////////////////////////////////////////////////////////////
  if (online.A.trigger && !AlaserOK) {
    hdcA->handle();
    hdcA->FillOnline(out, &online);
    PPolFillOnline(out, &online);  
  }  
  /////////////////////////////////////////////////////////////////////////////
  // reset all chambers:
  /////////////////////////////////////////////////////////////////////////////
  vdcA->Reset();
  hdcA->Reset();
  vdcB->Reset();
  vdcC->Reset();
  PdcD->Reset();  


  /////////////////////////////////////////////////////////////////////////////bss
  // -Depending on rundb.use_wobbler_adc, the wobbler info is considered to
  //  calculate the vertex position in (x,y) direction from the wobbler ADC readout
  //  via focus->horA/B/C and focus->vertA/B/C
  // -Default wobbler info: Each spectrometer uses its own wobbler adc readout if available
  // -The spectrometer tracking info is used to calculate vertex-z position via
  //  focus->z_by_A/B/C
  //  the previously determined (x,y)-position can be considered 
  //  when rundb.wobbler_corr is set to 'yes'
  // -Default online.Vertex.x/y/z: The spectrometer-unspecific online-variables for
  //  beam position are determined by B if available, otherwise by A; 
  //  the behaviour is fixed by rundb.use_vertex = "A", "B", "C", or anything else for default
  /////////////////////////////////////////////////////////////////////////////

  online.A.vertex.x=online.B.vertex.x=online.C.vertex.x = rundb.beam.offset.x;
  online.A.vertex.y=online.B.vertex.y=online.C.vertex.y = rundb.beam.offset.y;
  online.Vertex.x = online.Vertex.y = online.Vertex.z = 0; 


#if YWdebug == 1
  std::cout << "Initial" << std::endl;
  std::cout << online.A.vertex.x << '\t' << online.A.vertex.y << '\t' << online.A.vertex.z << std::endl;
  std::cout << online.B.vertex.x << '\t' << online.B.vertex.y << '\t' << online.B.vertex.z << std::endl;
  std::cout << online.Vertex.x << '\t' << online.Vertex.y << '\t' << online.Vertex.z << std::endl;
#endif


  if (resultA) {
    if (!strcmp(rundb.use_wobbler_adc, "B") 
     && atree->itemOK(&spekB.beam.position) ) {
      online.A.vertex.x += focus->horB( spekB.beam.position);
      online.A.vertex.y += focus->vertB(spekB.beam.position);
    } else if (!strcmp(rundb.use_wobbler_adc, "C") 
     && atree->itemOK(&spekC.beam.position) ) {
      online.A.vertex.x += focus->horC( spekC.beam.position);
      online.A.vertex.y += focus->vertC(spekC.beam.position);
    } else if (!strcmp(rundb.use_wobbler_adc, "none")) {
	; //added bss: do not use wobbler at all
    } else if (!strcmp(rundb.use_wobbler_adc, "enforceWobbler")) {
      ; //added bss: used for random pulser data, where usually no resultA is valid; handle separately below, search for "enforceWobbler"
    } else if (atree->itemOK(&spekA.beam.position) ) {
      online.A.vertex.x += focus->horA( spekA.beam.position);
      online.A.vertex.y += focus->vertA(spekA.beam.position);
    }
    if (!strcmp(rundb.wobbler_corr, "yes"))
      online.A.vertex.z = focus->z_by_A(online.A.vertex.x, resultA);
    else
      online.A.vertex.z = focus->z_by_A(0, resultA);

    if (strcmp(rundb.use_wobbler_adc, "enforceWobbler")) //only do it if not enforceWobbler is selected (this should be usually the case)
      out->packEventData(&online.A.vertex.x, 3);
    // Official determination of vertex by A if explicitly chosen
    if (!strcmp(rundb.use_vertex, "A") ) {
      online.Vertex.x = online.A.vertex.x;
      online.Vertex.y = online.A.vertex.y; 
      online.Vertex.z = online.A.vertex.z; 
      out->packEventData(&online.Vertex.x, 3);
    }
  }

  if (resultB) {
    if (!strcmp(rundb.use_wobbler_adc, "A") 
     && atree->itemOK(&spekB.beam.position) ) {
      online.B.vertex.x += focus->horA( spekA.beam.position);
      online.B.vertex.y += focus->vertA(spekA.beam.position);
    } else if (!strcmp(rundb.use_wobbler_adc, "C") 
     && atree->itemOK(&spekC.beam.position) ) {
      online.B.vertex.x += focus->horC( spekC.beam.position);
      online.B.vertex.y += focus->vertC(spekC.beam.position);
    } else if (!strcmp(rundb.use_wobbler_adc, "none")) {
	; //added bss: do not use wobbler at all
    } else if (!strcmp(rundb.use_wobbler_adc, "enforceWobbler")) {
      ; //added bss: used for random pulser data, where usually no resultB is valid; handle separately below, search for "enforceWobbler"
    } else if (atree->itemOK(&spekB.beam.position) ) {
      online.B.vertex.x += focus->horB( spekB.beam.position);
      online.B.vertex.y += focus->vertB(spekB.beam.position);
    }
    if (!strcmp(rundb.wobbler_corr, "yes"))
      online.B.vertex.z = focus->z_by_B(online.B.vertex.x, 
					online.B.vertex.y, resultB);
    else
      online.B.vertex.z = focus->z_by_B(0, 0, resultB);

    if (strcmp(rundb.use_wobbler_adc, "enforceWobbler")) //only do it if not enforceWobbler is selected (this should be usually the case)
      out->packEventData(&online.B.vertex.x, 3);
    // Official determination of vertex by B if explicitly chosen
    if (!strcmp(rundb.use_vertex, "B")) {
      online.Vertex.x = online.B.vertex.x;
      online.Vertex.y = online.B.vertex.y; 
      online.Vertex.z = online.B.vertex.z; 
      out->packEventData(&online.Vertex.x, 3);
    }
  }
      
  if (resultC) {
    if (!strcmp(rundb.use_wobbler_adc, "A") 
     && atree->itemOK(&spekA.beam.position) ) {
      online.C.vertex.x += focus->horA( spekA.beam.position);
      online.C.vertex.y += focus->vertA(spekA.beam.position);
    } else if (!strcmp(rundb.use_wobbler_adc, "onlyA") 
	       && atree->itemOK(&spekA.beam.position) ) {
      online.C.vertex.x += focus->horA( spekA.beam.position);
      online.C.vertex.y += focus->vertA(spekA.beam.position);
      //now make sure that wobbler offsets for C and A are the same
    } else if (!strcmp(rundb.use_wobbler_adc, "B") 
     && atree->itemOK(&spekB.beam.position) ) {
      online.C.vertex.x += focus->horB( spekB.beam.position);
      online.C.vertex.y += focus->vertB(spekB.beam.position);
    } else if (!strcmp(rundb.use_wobbler_adc, "none")) {
	; //added bss: do not use wobbler at all
    } else if (!strcmp(rundb.use_wobbler_adc, "enforceWobbler")) {
      ; //added bss: used for random pulser data, where usually no resultA is valid; handle separately below, search for "enforceWobbler"
    } else if (atree->itemOK(&spekC.beam.position) ) {
      online.C.vertex.x += focus->horC( spekC.beam.position);
      online.C.vertex.y += focus->vertC(spekC.beam.position);
    }
    if (!strcmp(rundb.wobbler_corr, "yes"))
      online.C.vertex.z = focus->z_by_C(online.C.vertex.x, resultC);
    else
      online.C.vertex.z = focus->z_by_C(0, resultC);

    if (strcmp(rundb.use_wobbler_adc, "enforceWobbler")) //only do it if not enforceWobbler is selected (this should be usually the case)
      out->packEventData(&online.C.vertex.x, 3);
    // Official determination of vertex by C only if explicitly chosen
    if (!strcmp(rundb.use_vertex, "C")) {
      online.Vertex.x = online.C.vertex.x;
      online.Vertex.y = online.C.vertex.y; 
      online.Vertex.z = online.C.vertex.z; 
      out->packEventData(&online.Vertex.x, 3);
    }
  }

#if YWdebug == 1
  std::cout << "After focus" << std::endl;
  std::cout << online.A.vertex.x << '\t' << online.A.vertex.y << '\t' << online.A.vertex.z << std::endl;
  std::cout << online.B.vertex.x << '\t' << online.B.vertex.y << '\t' << online.B.vertex.z << std::endl;
  std::cout << online.Vertex.x << '\t' << online.Vertex.y << '\t' << online.Vertex.z << std::endl;
#endif


  if (!strcmp(rundb.use_wobbler_adc, "enforceWobbler")) {
    //added bss: used for random pulser data, where usually no resultX is valid
    if (atree->itemOK(&spekA.beam.position) ) {
      online.A.vertex.x += focus->horA( spekA.beam.position);
      online.A.vertex.y += focus->vertA(spekA.beam.position);
      out->packEventData(&online.A.vertex.x, 2); //only pack x and y
    }
    if (atree->itemOK(&spekB.beam.position) ) {
      online.B.vertex.x += focus->horB( spekB.beam.position);
      online.B.vertex.y += focus->vertB(spekB.beam.position);
      out->packEventData(&online.B.vertex.x, 2); //only pack x and y
    }
    else if (atree->itemOK(&spekC.beam.position) ) {
      online.C.vertex.x += focus->horC( spekC.beam.position);
      online.C.vertex.y += focus->vertC(spekC.beam.position);
      out->packEventData(&online.C.vertex.x, 2); //only pack x and y
    }
  }
  
#if YWdebug == 1
  std::cout << "Wobbler" << std::endl;
  std::cout << online.A.vertex.x << '\t' << online.A.vertex.y << '\t' << online.A.vertex.z << std::endl;
  std::cout << online.B.vertex.x << '\t' << online.B.vertex.y << '\t' << online.B.vertex.z << std::endl;
  std::cout << online.Vertex.x << '\t' << online.Vertex.y << '\t' << online.Vertex.z << std::endl;
#endif

  // default: automatic determination, B if available, otherwise A or finally C
  if (strcmp(rundb.use_vertex, "A") && strcmp(rundb.use_vertex, "B") && 
      strcmp(rundb.use_vertex, "C") ) {
    // If only C has fired, the vertex is determined by C
    if (resultC && !resultB && !resultA && 
	(atree->itemOK(&spekC.beam.position) || 
	 atree->itemOK(&spekA.beam.position) ) // this line added for single SpekC event with wobbler readout in SpekA (P.Achenbach 10.10.2013) 
	) {
      online.Vertex.x = online.C.vertex.x;
      online.Vertex.y = online.C.vertex.y;
      online.Vertex.z = online.C.vertex.z;
      out->packEventData(&online.Vertex.x, 3);
    }
    // when A has fired and NOT B, i.e. the case AC and A, then vertex is
    // determined by A
    if (resultA && !resultB && atree->itemOK(&spekA.beam.position) ) {
      online.Vertex.x = online.A.vertex.x;
      online.Vertex.y = online.A.vertex.y;
      online.Vertex.z = online.A.vertex.z;
      out->packEventData(&online.Vertex.x, 3);
    }
    // Default: when B has fired, B is used for the vertex determination,
    // i.e. the case AB, BC and B   
    if (resultB && atree->itemOK(&spekB.beam.position) ) {
      online.Vertex.x = online.B.vertex.x;
      online.Vertex.y = online.B.vertex.y;
      online.Vertex.z = online.B.vertex.z;
      out->packEventData(&online.Vertex.x, 3);
    }
  }
#if YWdebug == 1
  std::cout << "Final" << std::endl;
  std::cout << online.A.vertex.x << '\t' << online.A.vertex.y << '\t' << online.A.vertex.z << std::endl;
  std::cout << online.B.vertex.x << '\t' << online.B.vertex.y << '\t' << online.B.vertex.z << std::endl;
  std::cout << online.Vertex.x << '\t' << online.Vertex.y << '\t' << online.Vertex.z << std::endl;
  std::cout << std::endl;
#endif

  //    if (!resultA && !strcmp(rundb.use_vertex, "A") && 
  //  			  resultB && atree->itemOK(&spekB.beam.position)) {
  //      online.Vertex.x = online.B.vertex.x;
  //      online.Vertex.y = online.B.vertex.y;
  //      online.Vertex.z = online.B.vertex.z;
  //      out->packEventData(&online.Vertex.x, 3);
  //    }

  /////////////////////////////////////////////////////////////////////////////
  // Apply the Focus Correction of delta and theta
  /////////////////////////////////////////////////////////////////////////////

  if (!strcmp(rundb.focus_corr, "yes")) {
    if (resultA) 
      focus->focuscorrA(online.Vertex.x, online.Vertex.y, resultA);
    if (resultB) 
      focus->focuscorrB(online.Vertex.x, online.Vertex.y, resultB);
    if (resultC) 
      focus->focuscorrC(online.Vertex.x, online.Vertex.y, resultC);
    //    if (resultD) 
    //      focus->focuscorrD(online.Vertex.x, online.Vertex.y, resultD);
    //    if (resultK) 
    //      focus->focuscorrK(online.Vertex.x, online.Vertex.y, resultK);
  }

 // Dirty hack for focus correction circumvention for spline fit
#ifdef DIRTYHACK_FORFIT
    online.C.vertex.x=dpcorr;
    online.C.vertex.y=thcorr;
    out->packEventData(&online.C.vertex.x, 3);
#endif
  

  /////////////////////////////////////////////////////////////////////////////
  // Coincidence Time Correction
  /////////////////////////////////////////////////////////////////////////////

  if (online.A.trigger) {
    lasttriggerA++;
    if (lasttriggerA>50) {
      Luminosity->VDCtrippOff();
      if (vdcwarning)
	std::cerr << "WARNING: Event " << events
		  << ": No valid four vector during last "
		  << lasttriggerA << " events in A" << std::endl;
    }
  } 

  if (resultA && fabs(resultA->dp)<100) {
    online.A.target_coor_ok = 1;
    lasttriggerA = 0;
    A_Out = particle(A_Out.getMass(), qsddaMomentum * (1 + resultA->dp/100),
		     -qsddaAngle, -resultA->ph/1000, resultA->th/1000);

    online.A.x_scint       = xA + tan(tA * 0.001) * 390.0;
    //online.A.y_scint       = yA + tan(pA * 0.001) * 390.0 / cos(tA * 0.001);
    online.A.y_scint       = yA + tan(pA * 0.001) * 390.0;
    if (rundb.A.scint.use_dE==0)
      online.A.Index_pattern = qsddaScintIndex(online.A.ToF.pattern);
    else
      online.A.Index_pattern = qsddaScintIndex(online.A.dE.pattern);
    online.A.Index_x       = qsddaScintIndex(online.A.x_scint);

    double veloA           = (V_LIGHT * A_Out.beta().abs());
    double lenCorrA        = resultA->len / veloA;
    double extendedA       = -focus->l_R_A(online.Vertex.x, online.Vertex.y, 
                                          resultA) * 0.1 / veloA;
    double scintDistA      = 0.1 * sqrt(390*390 + pow(online.A.x_scint-xA,2) + 
				        pow(online.A.y_scint-yA,2)) / veloA;
    online.A.TimeAtTarget  = qsddaTimeShift(online.A.y_scint, &online)
                           + extendedA + scintDistA + lenCorrA;
    online.A.raw_Time_Left = rundb.tdc_resolution * actime_3 
                           - qsddaLeftShift(0, &online);
    online.A.Time_Left     = rundb.tdc_resolution * actime_3 //bss: wtf: why is it hard coded dE?!?! done better for specC..
                           - rundb.A.scint.walk[2]                           
                           * securesqrt(online.A.dE.Energy.left) 
                           - qsddaLeftShift(online.A.y_scint, &online); 
    
    if (rundb.A.scint.use_dE)
      online.A.TimeAtTarget -= rundb.A.scint.walk[2]
	                     * securesqrt(online.A.dE.Energy.right );
    else
      online.A.TimeAtTarget -= rundb.A.scint.walk[1]
	                     * securesqrt(online.A.ToF.Energy.right);
    

    if (ergloss) { 
      double EnergyA_Before = A_Out.energy();
      if (!rundb.A.vacuum) target->EnergyLossCorrChamber(A_Out);
      target->EnergyLossCorr(A_Out, online.Vertex.x - rundb.Target.offset.x,
			     online.Vertex.y - rundb.Target.offset.y,
			     online.Vertex.z - rundb.Target.offset.z,
			     steps_A);
      online.A.Eloss_corr = (A_Out.energy() - EnergyA_Before);
      out->packEventData(&online.A.Eloss_corr, 1);
    }

    online.A.ScintCorr    = online.A.TimeAtTarget - lenCorrA - extendedA
                            - scintDistA;
    online.A.Extended     = extendedA;
    online.A.ScintDist    = scintDistA;
    out->packEventData(&online.A.x_scint,1);
    out->packEventData(&online.A.y_scint,1);
    out->packEventData(&online.A.Index_pattern,1);
    out->packEventData(&online.A.Index_x,1);
    out->packEventData(&online.A.TimeAtTarget,1);
    out->packEventData(&online.A.ScintCorr,1);
    out->packEventData(&online.A.Extended,1);
    out->packEventData(&online.A.ScintDist,1);
    out->packEventData(&online.A.raw_Time_Left,1);
    out->packEventData(&online.A.Time_Left,1);
  }

  if (online.B.trigger) {
    lasttriggerB++;
    if (lasttriggerB>50) {
      Luminosity->VDCtrippOff();
      if (vdcwarning)
	std::cerr << "WARNING: Event " << events
		  << ": No valid four vector during last "
		  << lasttriggerB << " events in B" << std::endl;
    }
  }
  
  if (resultB && fabs(resultB->dp)<100) {
    online.B.target_coor_ok = 1;
    lasttriggerB = 0;

    B_Out = particle(B_Out.getMass(), 
		     bclamMomentum * (1 + resultB->dp/100),
		     bclamAngle, -rundb.B.oopangle/180*M_PI,
		     -resultB->ph/1000, resultB->th/1000);

    online.B.x_scint       = xB + tan(tB * 0.001) * 405.0;
    //online.B.y_scint       = yB + tan(pB * 0.001) * 405.0 / cos(tB * 0.001);
    online.B.y_scint       = yB + tan(pB * 0.001) * 405.0;
    online.B.Index_pattern = bclamScintIndex(online.B.ToF.pattern);
    online.B.Index_x       = bclamScintIndex(online.B.x_scint);
    
    double veloB           = (V_LIGHT * B_Out.beta().abs());
    double lenCorrB        = resultB->len / veloB;
    double extendedB       = -focus->l_R_B(online.Vertex.x, online.Vertex.y,
                                          resultB) * 0.1 / veloB;
    double scintDistB      = 0.1 * sqrt(405*405 + pow(online.B.x_scint-xB,2) + 
				  pow(online.B.y_scint-yB,2)) / veloB;
    online.B.TimeAtTarget  = bclamTimeShift(online.B.y_scint, &online) 
                             + extendedB + scintDistB + lenCorrB;
    
    if (rundb.B.scint.use_dE == 0)
      online.B.TimeAtTarget -= rundb.B.scint.walk[1]
	                     * securesqrt(online.B.ToF.Energy.right);
    else
      online.B.TimeAtTarget -= rundb.B.scint.walk[2]
	                     * securesqrt(online.B.dE.Energy.right );
    
    if (ergloss) {
      double EnergyB_Before = B_Out.energy();
      if (!rundb.B.vacuum) target->EnergyLossCorrChamber(B_Out);
      target->EnergyLossCorr(B_Out, online.Vertex.x - rundb.Target.offset.x,
			     online.Vertex.y - rundb.Target.offset.y,
			     online.Vertex.z - rundb.Target.offset.z,
			     steps_B);
      online.B.Eloss_corr = (B_Out.energy() - EnergyB_Before);
      out->packEventData(&online.B.Eloss_corr,1);
    }
      
    online.B.ScintCorr    = online.B.TimeAtTarget - extendedB - lenCorrB
                            - scintDistB;
    online.B.Extended     = extendedB;
    online.B.ScintDist    = scintDistB;
    out->packEventData(&online.B.x_scint,1);
    out->packEventData(&online.B.y_scint,1);
    out->packEventData(&online.B.Index_pattern,1);
    out->packEventData(&online.B.Index_x,1);
    out->packEventData(&online.B.TimeAtTarget,1);
    out->packEventData(&online.B.ScintCorr,1);
    out->packEventData(&online.B.Extended,1);
    out->packEventData(&online.B.ScintDist,1);

    //cout << online.B.TimeAtTarget << endl;
  }
  
  if (online.C.trigger) {
    lasttriggerC++;
    if (lasttriggerC>50) {
      Luminosity->VDCtrippOff();
      if (vdcwarning)
	std::cerr << "WARNING: Event " << events
		  << ": No valid four vector during last "
		  << lasttriggerC << " events in C" << std::endl;
    }
  }
  
  if (resultC&& fabs(resultC->dp)<100) {
    lasttriggerC = 0;
    C_Out = particle(C_Out.getMass(), qsddcMomentum * (1 + resultC->dp/100),
		     qsddcAngle, -resultC->ph/1000, resultC->th/1000);

    online.C.x_scint      = xC + tan(tC/1000) * 400.0;
    //online.C.y_scint      = yC + tan(pC/1000) * 400.0 / cos(tC * 0.001);
    online.C.y_scint      = yC + tan(pC/1000) * 400.0;
    if (rundb.C.scint.use_dE==0)
      online.C.Index_pattern = qsddcScintIndex(online.C.ToF.pattern);
    else
      online.C.Index_pattern = qsddcScintIndex(online.C.dE.pattern);
    online.C.Index_x       = qsddcScintIndex(online.C.x_scint);
    
    double veloC           = (V_LIGHT * C_Out.beta().abs());
    double lenCorrC        = resultC->len / veloC;
    double extendedC       = -focus->l_R_C(online.Vertex.x, online.Vertex.y,
                                          resultC) * 0.1 / veloC; 
    double scintDistC      = 0.1 * sqrt(400*400 + pow(online.C.x_scint-xC,2) + 
				  pow(online.C.y_scint-yC,2)) / veloC;
    online.C.TimeAtTarget  = qsddcTimeShift(online.C.y_scint, &online)
                            + extendedC + scintDistC + lenCorrC;    

    online.C.raw_Time_Left = rundb.tdc_resolution * cbtime_1
                           - qsddcLeftShift(0, &online);
    online.C.Time_Left     = rundb.tdc_resolution * cbtime_1
                           - qsddcLeftShift(online.C.y_scint, &online);

    if (rundb.C.scint.use_dE) {
      online.C.TimeAtTarget -= rundb.C.scint.walk[2]
	                     * securesqrt(online.C.dE.Energy.right);
      online.C.Time_Left    -= rundb.C.scint.walk[2]
	                     * securesqrt(online.C.dE.Energy.left);
    } else {
      online.C.TimeAtTarget -= rundb.C.scint.walk[1]
	                    * securesqrt(online.C.ToF.Energy.right);
      online.C.Time_Left    -=  rundb.C.scint.walk[1]
                            * securesqrt(online.C.ToF.Energy.left);
    }

    if (ergloss) { 
      double EnergyC_Before = C_Out.energy();
      if (!rundb.C.vacuum) target->EnergyLossCorrChamber(C_Out); 
      target->EnergyLossCorr(C_Out, online.Vertex.x - rundb.Target.offset.x,
			     online.Vertex.y - rundb.Target.offset.y,
			     online.Vertex.z - rundb.Target.offset.z,
			     steps_C);
      online.C.Eloss_corr = (C_Out.energy() - EnergyC_Before);

      out->packEventData(&online.C.Eloss_corr,1);
    }

    online.C.ScintCorr    = online.C.TimeAtTarget - lenCorrC - extendedC
                            - scintDistC;
    online.C.Extended     = extendedC;
    online.C.ScintDist    = scintDistC;
    out->packEventData(&online.C.x_scint,1);
    out->packEventData(&online.C.y_scint,1);
    out->packEventData(&online.C.Index_pattern,1);
    out->packEventData(&online.C.Index_x,1);
    out->packEventData(&online.C.TimeAtTarget,1);
    out->packEventData(&online.C.ScintCorr,1);
    out->packEventData(&online.C.Extended,1);
    out->packEventData(&online.C.ScintDist,1);
    out->packEventData(&online.C.raw_Time_Left,1);
    out->packEventData(&online.C.Time_Left,1);
  }

  if (resultD && fabs(resultD->dp)<100) {
    D_Out = particle(D_Out.getMass(), ddMomentum * (1 + resultD->dp/100),
		     ddAngle, -resultD->ph/1000, resultD->th/1000);
    double veloD           = (V_LIGHT * D_Out.beta().abs());
    double lenCorrD        = resultD->len / veloD;
    online.D.TimeAtTarget = lenCorrD; /* weitere Terme fehlen noch */
    out->packEventData(&online.D.TimeAtTarget,1);
  }

  if (resultA && resultB) {
    online.ab_tdc = *abtime; 
    out->packEventData(&online.ab_tdc,1);
    online.abcorr = rundb.ab_tdc_resolution * *abtime 
      + online.A.TimeAtTarget - online.B.TimeAtTarget
      - rundb.coincidencetime - rundb.coincidencetime_AB;
    out->packEventData(&online.abcorr,1);
  }

  if (resultB && resultC) {
    online.bc_tdc = *bctime; 
    out->packEventData(&online.bc_tdc,1);
    online.bccorr = rundb.bc_tdc_resolution * *bctime 
      + online.C.TimeAtTarget - online.B.TimeAtTarget
      - rundb.coincidencetime - rundb.coincidencetime_CB;
    out->packEventData(&online.bccorr,1);
  }

  if (resultA && resultC) {
    online.ca_tdc = *catime; 
    out->packEventData(&online.ca_tdc,1);
    online.cacorr = rundb.ac_tdc_resolution * *catime 
      - online.C.TimeAtTarget + online.A.TimeAtTarget
      - rundb.coincidencetime - rundb.coincidencetime_CA;
    out->packEventData(&online.cacorr,1);
  }

  if (resultA && resultD) {
    online.ab_tdc = *abtime; 
    out->packEventData(&online.ab_tdc,1);
    online.abcorr = rundb.ab_tdc_resolution * *abtime 
      + online.A.TimeAtTarget - online.D.TimeAtTarget
      - rundb.coincidencetime - rundb.coincidencetime_AB;
    out->packEventData(&online.abcorr,1);
  }

  //pulsed Beam:
  if (resultA) {
    online.A.single_tdc = *a_time;
    out->packEventData(&online.A.single_tdc,1);
    online.A.singlecorr = rundb.a_tdc_resolution * *a_time
      + online.A.TimeAtTarget - rundb.singletime_A - rundb.singletime;
    out->packEventData(&online.A.singlecorr,1);
  }
  if (resultB) {
    online.B.single_tdc = *b_time;
    out->packEventData(&online.B.single_tdc,1);
    online.B.singlecorr = rundb.b_tdc_resolution * *b_time
      + online.B.TimeAtTarget - rundb.singletime_B - rundb.singletime;
    out->packEventData(&online.B.singlecorr,1);
  }
  if (resultC) {
    online.C.single_tdc = *c_time;
    out->packEventData(&online.C.single_tdc,1);
    online.C.singlecorr = rundb.c_tdc_resolution * *c_time
      + online.C.TimeAtTarget - rundb.singletime_C - rundb.singletime;
    out->packEventData(&online.C.singlecorr,1);
  }

  /*
  std::cout << "TDC" << std::endl;
  std::cout << online.A.single_tdc << std::endl;
  std::cout << online.B.single_tdc << std::endl;
  std::cout << online.C.single_tdc << std::endl;
  */
  //////////////////////////////////////////////////////////////////////////// 
  // Flash ADC
  //////////////////////////////////////////////////////////////////////////// 
 
  if (Reaction->getSI()!=NULL && atree->itemOK(&abcn.si.label) && fADC){
    fADC->handle();
   }
  

  //////////////////////////////////////////////////////////////////////////// 
  // Silicon Detector
  //////////////////////////////////////////////////////////////////////////// 
 
  online.si.OK = 0;
  if (Reaction->getSI()!=NULL && atree->itemOK(&abcn.si.label)){
      siDet->handle();
      
      //Outgoing particle
      //double Energy = online.si.EnergyParticle;
      //double Momentum  = online.si.MomentumParticle;
      //double theta = online.si.si_theta;
      //double phi = online.si.si_phi;
      //double Theta = online.si.ThetaParticle + rundb.si.angle/180*M_PI;
      //double Phi = online.si.PhiParticle;
      
      int steps_Si = 20;
      double EnergySi_Before;      

      if (online.si.OK == 1){
	  
	  SI_Out = particle(SI_Out.getMass(), online.si.MomentumParticle, rundb.si.angle/180*M_PI, online.si.si_theta, online.si.si_phi);
	  //SI_Out.initPolar(Energy,Momentum,th0+rundb.si.angle/180*M_PI,ph0);
	  
	  // Target energy loss corrections.     
	  if (ergloss){
	      EnergySi_Before = SI_Out.energy();
	      if (SI_Out.energy()>SI_Out.getMass()){
		  target->EnergyLossCorr(SI_Out,
				 online.Vertex.x - rundb.Target.offset.x,
				 online.Vertex.y - rundb.Target.offset.y,
				 online.Vertex.z - rundb.Target.offset.z,
				 steps_Si);
		  
		  online.si.EnergyParticle = SI_Out.energy();
		  online.si.MomentumParticle = SI_Out.momentum();
		  online.si.ThetaParticle = SI_Out.theta();
		  online.si.PhiParticle = SI_Out.phi();
		  
		  online.si.Eloss_corr = SI_Out.energy() - EnergySi_Before;
	      }
	  }

	  if (rundb.fadc.use_fadc != 0){
	      //double EnergyP_fadc = online.si.EnergyP_fadc;
	      //double MomentumP_fadc = online.si.MomentumP_fadc;    
	      Particle P_fadc = P_proton; 
	     
	      P_fadc = particle(P_fadc.getMass(), online.si.MomentumP_fadc, rundb.si.angle/180*M_PI, online.si.si_theta, online.si.si_phi);
	      //P_fadc.initPolar(EnergyP_fadc,MomentumP_fadc,Theta,Phi);
	     	      
	      // Target energy loss corrections.     
	      if (ergloss){
		  EnergySi_Before = P_fadc.energy();
		  if (P_fadc.energy()>P_fadc.getMass()){
		      target->EnergyLossCorr(P_fadc,
					     online.Vertex.x - rundb.Target.offset.x,
					     online.Vertex.y - rundb.Target.offset.y,
					     online.Vertex.z - rundb.Target.offset.z,
					     steps_Si);
		      
		      online.si.EnergyP_fadc = P_fadc.energy();
		      online.si.MomentumP_fadc = P_fadc.momentum();
		      online.si.ThetaParticle = P_fadc.theta();
		      online.si.PhiParticle = P_fadc.phi();

		      online.si.Eloss_corr = P_fadc.energy() - EnergySi_Before;
		  }
		  
	      }
	  }
      }		
      else online.si.OK=0; 
      out->packEventData(&online.si.EnergyParticle,1);
      out->packEventData(&online.si.MomentumParticle,1);
      out->packEventData(&online.si.EnergyP_fadc,1);
      out->packEventData(&online.si.MomentumP_fadc,1);
      out->packEventData(&online.si.ThetaParticle,1);
      out->packEventData(&online.si.PhiParticle,1);
      out->packEventData(&online.si.Eloss_corr,1);
  }
    
  /////////////////////////////////////////////////////////////////////////////
  // Neutron polarimeter
  /////////////////////////////////////////////////////////////////////////////

  if (npol && (atree->itemOK(&abcn.npol.sync_info))) {
    double sTime = 0;
    if (online.A.trigger) 
      sTime = online.A.TimeAtTarget - rundb.coincidencetime; //??????????
    npol->reconstruct(atree, out, NPOL_Out, sTime);
  };


  /////////////////////////////////////////////////////////////////////////////
  // ndet 2008
  /////////////////////////////////////////////////////////////////////////////

  if (NDET_TO_BE_HANDLED == 1) {
    NDET_TO_BE_HANDLED = 0;
    ndet->handle(); //bss: moved ndet->handle() below spektrometer A to have coincidence time information (A.TimeAtTarget) available
  }

  /////////////////////////////////////////////////////////////////////////////
  // Fill Histograms
 
  Particle electronIn = P_electron;    
  electronIn.setMomentum(0, 0,
			 sqrt(BeamEnergy*BeamEnergy-m_electron*m_electron));
  if (online.A.trigger || online.B.trigger || online.C.trigger || online.kaos.trigger) {  
    if (ergloss) {
      target->EnergyLossCorrBeam(electronIn, 
				 online.Vertex.x - rundb.Target.offset.x,
				 online.Vertex.y - rundb.Target.offset.y,
				 online.Vertex.z - rundb.Target.offset.z,
				 steps_IN);
      online.Beam.Eloss_corr = (BeamEnergy - electronIn.energy());
      out->packEventData(&online.Beam.Eloss_corr, 1);    
    }
  }
  static FourVector dummy(0,0,0,0);
  if (!resultA) A_Out = dummy;
  if (!resultB) B_Out = dummy;
  if (!resultC) C_Out = dummy;
  if (!resultD) D_Out = dummy;

  if (!rundb.kaos.tma.type && !resultK) KAOS_Out = dummy;

  if (online.si.OK != 1) SI_Out = dummy;

  if (!sigsetjmp(env, 1))
    if (evaluate_expressions(A_Out, B_Out, C_Out, D_Out, O_Out, 
  			     H3_Out, TOF_Out, NPOL_Out, NDET_Out, SI_Out, 
			     KAOS_Out, electronIn))
      reader->writeEvent();

  /////////////////////////////////////////////////////////////////////////////
  // write ntuple event
     if (ntuple) nt->fill_ntuple();
  //
  ///////////////////////////////////////////////////////////////////////


#ifdef HAVE_ROOTTOOLS
  if (RootFileName && (rttool->use_root()==1 || rttool->use_root()==2) ) {
    rttool->Fill("evID",events);
    rttool->Fill();
  }
#endif

  // event counter should not be increased if passPerEvent > 0
  if (passPerEvent == 0)
    if (!(++events % 500)) printStatus(); // update status line
  
  return 0; // This means for an X11 working procedure: run again
}

void 
Cola::targetFillHisto(struct TargetCoord *t, struct TargetCo *tgt, 
		      AquaTree *o)
{
  if (!tgt) return;
  t->dp = tgt->dp;    
  t->th = tgt->th;
  t->y0 = tgt->y0;     
  t->ph = tgt->ph;
  t->len = tgt->len;
  o->packEventData((double *)t, 5);
}

void MenuCallback(int id)
{
  switch (id) {
  case SHOWCNTPAGE:    if (cntpage) cntpage->Show(TRUE);    break;
  case SHOWSTATPAGE:   if (statpage) statpage->Show(TRUE);  break;
  case REWIND:         dorewind = 1;  events = 0;           break;
  }
}

char *currentHistogram();
void signalhandler(int)
{
  static int errorcnt=0;
  char str[32];
  sprintf(str, "%5.1f", (++errorcnt) * 100.0 / (events + 1));
  //  std::cerr << "WARNING: Floating point exception for Event " << events
  //	    << ", " << str << "% of Events lost." << std::endl;
  std::cerr << "WARNING: Floating point exception for Event " << events
	    << ", FPE's disabled." << std::endl;
  if (expression::current) 
      std::cerr << "During evaluation of \""
		<< expression::current->name << "\"\n";
  if (verbosity) {
    std::cerr << "During filling of histogram "
	      << currentHistogram() << std::endl;
  }
  signal(SIGFPE, signalhandler);
  enableFPE();
  siglongjmp(env,1);
}


static Cola *kernel;

int work (wxApp *app)
{
  static int firstInit = 0;
  if (!firstInit) { 
    frame    = (HMVFrame*) app->GetTopWindow();
    cntpage  = frame->counterPage;
    statpage = frame->statPage;
    frame->file_menu->AppendSeparator();
    frame->file_menu->Append(SHOWCNTPAGE,  "Show &Counter Window");
    // frame->file_menu->Append(SHOWSTATPAGE, "Show Statistic &Page");
    frame->file_menu->Append(REWIND,  "&Rewind Input Data File");
    firstInit++;
  }

  //std::cout << "Event" << std::endl;
  return kernel->eventloop();
}

time_t 
dateFromFilename(const char *name)
{ 
  struct tm t;
  for(unsigned int i=0; i<strlen(name); i++) {
    if ( (sscanf(&name[i],"%4d - %2d - %2d - %2d - %2d - %2d",
		 &t.tm_year, &t.tm_mon, &t.tm_mday,
		 &t.tm_hour, &t.tm_min, &t.tm_sec) == 6) )
      {
	//if (t.tm_year<70) t.tm_year += 100;
	t.tm_mon--;
	t.tm_isdst = -1;

	return mktime(&t);
      }
    else if ( (sscanf(&name[i],"%02d%02d%02d%02d%02d%02d",
		      &t.tm_year, &t.tm_mon, &t.tm_mday,
		      &t.tm_hour, &t.tm_min, &t.tm_sec) == 6) )
      {
	if (t.tm_year<70) t.tm_year += 100;
	t.tm_mon--;
	t.tm_isdst = -1;
	
	return mktime(&t);
      }
  }
  std::cerr << "ERROR: Can't extract start time from \""
	    << name << "\"." << std::endl;
  exit (-1);
  return 0;
} 

int main(int argc, char *argv[])
{
  int sighand  = 0;
  int cindyFormat = 0;
  int EventSim =0;
  archive_code useGZIP = ARCHIVE_PLAIN;
  char opt, *online_his, *online_rz;
  env_set(online_his, "ONLINE_HIS", "Online.his");
  env_set(online_rz,  "ONLINE_RZ",  "Online.rz");
  char *input  = NULL;
  char *output = NULL;
#ifdef HAVE_ROOTTOOLS
  char *rootfile = NULL;
#endif

  while ((opt = getopt(argc, argv, "qO:R:w:C:hcdvSs:n:o:f:ixjJzt:T")) != EOF) {
    switch (opt) {
    case 'f': input  = optarg;             break;
    case 'O': online_his = optarg;         break;
    case 'R': online_rz  = optarg;         break;
    case 'o': output = optarg;             break;
    case 'v': verbose = 1;                 break;
    case 'c': cindyFormat = 1;             break;
    case 'S': EventSim = 1;                break;
    case 'd': display = 0;                 break;
    case 'C': if (chdir(optarg));          break;
    case 'T': starttime = -1;              break;
    case 't': starttime = dateFromFilename(optarg); break;
    case 'i': sighand = 1;                 break;
    case 'x': sighand = 2;                 break;
    case 'q': quiet = 1;                   break;
    case 'n': maxevents = atoi(optarg);    break;
    case 's': skipevents = atoi(optarg);   break;
    case 'j': useGZIP = ARCHIVE_BZIP2;     break;
    case 'J': useGZIP = ARCHIVE_XZ;        break;
    case 'z': useGZIP = ARCHIVE_GZIP;      break;
    case 'V': std::cout << &"$Revision: 2691 $"[11] << std::endl; return 0;
#ifdef HAVE_ROOTTOOLS
    case 'w': rootfile = optarg;           break;
#endif
    case '?':                    // unknown option
    case ':':                    // missing argument
    case 'h':             
      std::cout << 
	"\nUSAGE:   " << argv[0] <<" [options]\n\n"
	"         Cindy Online Analysis.\n\n" 
	"Options:\n\n" 
	"-h         this help.\n"
	"-f <file>  Input data file name.\n"
	"-o <file>  Output data file name.\n"
        "-j         Compress (bzip2) output data.\n"
        "-J         Compress (xz) output data.\n"
        "-z         Compress (gzip) output data.\n"
	"-O <file>  Output histogram file name.\n"
	"-R <file>  Output ntuple file name.\n"
	"-C <dir>   Change to directory before doing anything.\n"
	"-w <file>  Output ROOT file name.\n"
        "-c <num>   Use Cindy reader (old format):\n"
	"     1       abcn,\n"
	"     2       abc_2,\n"
	"     3       abc,\n"
	"     4       ab940223,\n"
	"-S         Use Event-Simulator.\n"
        "-d         Don't start X11 Histogramviewer.\n"
        "-v         Verbose output of the 'run.db' contents.\n"
        "-V         Print version and exit.\n"
        "-q         Quiet output: suppress comment lines in run.db/*.col\n"
	"-n <n>     Terminate after <n> events.\n"
	"-s <n>     Ignore first <n> events.\n"
        "-i         Ignore Floating Point Exceptions"
	            "(default: use signalhandler).\n"
        "-x         Core dump on 'Floating point exception'.\n"
	"-t time    Use argument as start time, Format: 'yymmddhhmmss'\n"
	"-T         Try to extract start time from filename.\n\n"
	"compiled "__DATE__" "__TIME__"\n"
        "for detailed help see:\n"
        "http:/""/wwwa1.kph.uni-mainz.de/A1/trigger/doc/Cola++.html\n\n";  
      exit(0);
      break;
    default : std::cerr << "How strange, I can't analyze the options!"
			<< std::endl; 
    }
  }
  if (starttime==-1)  starttime = dateFromFilename(input);

  // parse input parameter (if any)
  AquaIO *aquaIN = NULL;
  if (!input || (*input=='-')) {
    aquaIN = new AquaUnzip(0);
  } else aquaIN = new AquaUnzip(input);
  MezzoReader *mreader = NULL;
  AquaReader *reader = NULL;
  if (cindyFormat) {
    reader = (AquaReader *)new CindyReader(aquaIN);
  } else {
    if (EventSim){
	reader =(AquaReader *) new EventSimReader(aquaIN);
	} else {
    MainzReader *tmp = new MainzReader(aquaIN);
    mreader = tmp->getMezzoReader();
    reader = (AquaReader *)tmp;
	}
 }

  if (output) {
    AquaIO *aout = NULL;
    switch (useGZIP) {
    case ARCHIVE_GZIP:
      aout = new AquaGzip(output, 0644); break;
    case ARCHIVE_BZIP2:
      aout = new AquaBzip2(output, 0644); break;
    case ARCHIVE_XZ:
      aout = new AquaXz(output, 0644); break;
    default:
      aout = new AquaIO(output, 0644); break;
    }
    reader->initOutput(aout);
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
  kernel = new Cola(reader, mreader, 0);
  kernel->setOutput(online_his);
#ifdef HAVE_ROOTTOOLS
  kernel->setOutputROOT(rootfile);
  kernel->Initialize();
#endif
  if (display) {
    char *argv[] = {"Cola"};
    HMMenuCallback = (void (*)(int)) MenuCallback; 
    HMLaunchViewWork(1, argv, (void(*)(wxApp*)) work);
  } else {
    while(kernel->eventloop() != 1);
      //std::cout << "Event" << std::endl;

  }

  return 0;
}
