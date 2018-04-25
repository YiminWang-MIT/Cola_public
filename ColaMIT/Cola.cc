//                                                                    -*-c++-*-
// Created 1995/96 by	Michael O. Distler 
//                      Harald Merkel
//
//			A1 Collaboration 
///                     Institut fuer Kernphysik
//			Johannes Gutenberg-Universitaet Mainz 
// 
// Main Part of Cindy Online Analysis 
//

#include "Cola.h"              // Headers and static variables
#include "rundatabase.h"       // "run.db" Parser
#include "TMA/FastTM.h"
#include "AquaCompress.h"
#include "AquaUnzip.h"
#include <signal.h>
#if defined(__GNU_LIBRARY__)
#  include <getopt.h>
#endif

#include "TCCC.h"              // Path-length correction of tcoinc
#include "utime.h"

extern "C" int MAIN__() { return 0;}; // bug in libf2c

const double M_ELECTRON		= 5.1099906E-4;	// electron mass
const double V_LIGHT		= 299.792458;   // mm/ns

// Ntuple stuff
static int ntuple = 0;
struct {
  int hmemor[HBOOKMEM];
} pawc_;
char *ntvarn[MAX_NT_VAR];   // Scalar name related to ntvar
char *ntdes[MAX_NT_VAR];    // NTvariable name
char *ntbck[MAX_NT_VAR];    // Block name for each ntvar
char *nttitle;              // Ntuple Title
int  ntiden, ntnvar;

HMVFrame *frame;
HMVPage *cntpage;
HMVPage *statpage;

long beamPuls;
long totalPuls;
reaction *Reaction = NULL;

void
readInputFiles()
{
  env_def(run_db, "RUN_DB", "run.db");
  env_def(online_col, "ONLINE_COL", "Online.col");
  memset(&rundb, 0, sizeof(rundatabase));
  rundb.reaction = "H(e,O:e'A:p)pi0";
  rundb.use_vertex = "";

  // read the run database from file 
  char runname[32];
  if (atree.getRunName())	
    sprintf(runname, "run_%12s", atree.getRunName());
  //aqua  else if (atree.getInputFilename()) 
  //aqua    strncpy(runname, atree.getInputFilename(),17);

  long int datatime =
    atree.getTimestamp() ? atree.getTimestamp() : time(NULL);
  readRunDatabase(run_db, runname, NULL, datatime);

  Reaction = new reaction(rundb.reaction);
  target = SetTargetFromRunDB(Reaction);

  //if (Reaction) rundb.Target = Reaction->getTarget()->getMass();
  hdcinit(hdcA, rundb.oopsA);
  hdcinit(hdcB, rundb.oopsB);
  hdcinit(hdcC, rundb.oopsC);
  hdcinit(hdcD, rundb.oopsD);
  vdcOHIPSinit(vdcO, OU1SIZE, OV1SIZE, OU2SIZE, OV2SIZE);
  if (datatime < 946702800) vdcO.Init98(); // before Y2K
  if ((rundb.ohips.VDCparam[0] > 0.0) || (rundb.ohips.VDCparam[1] > 0.0))
    vdcO.InitOffset((int)rundb.ohips.VDCparam[0],
		    (int)rundb.ohips.VDCparam[1],
		    (int)rundb.ohips.VDCparam[2],
		    (int)rundb.ohips.VDCparam[3]);
  // ohips drift time correction parameters are ignored
  // the values in the chamber library are based on a recent calculation
  // (Feb. 2001) and are tested for the old (pre 2000) and new ohips chambers
  // if you think they are not 100% perfect, talk to me ;-) -mod-
  //  if (rundb.ohips.dtcorr1[0]) vdcO.InitDTCorr1(rundb.ohips.dtcorr1);
  //  if (rundb.ohips.dtcorr2[0]) vdcO.InitDTCorr2(rundb.ohips.dtcorr2);
  if (rundb.ohips.drift_velocity > 0.0) { // overrides chinitO.h default
    vdcO.InitTimeToLength(rundb.ohips.drift_velocity);
  }

  steps_IN = steps_O = steps_A = steps_B = steps_C = steps_D = 1;
  if (ergloss > 1) {      
    steps_O = steps_A = steps_B = steps_C = steps_D = 5;
    if (Reaction->getO() != NULL) {
      if (*Reaction->getO() == P_electron) steps_O =   1;
      if (*Reaction->getO() == P_proton)   steps_O =  10;
      if (*Reaction->getO() == P_deuteron) steps_O = 100;
    }
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
      if (*Reaction->getD() == P_electron) steps_D =   1;
      if (*Reaction->getD() == P_proton)   steps_D =  10;
      if (*Reaction->getD() == P_deuteron) steps_D = 100;
    }
  }

  // Cleans ntuples
  nt->delete_all();

  parse_file(&atree, &out, NULL, Reaction, online_col);

  // Create ntuple
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

  // xfer BIC scales to StatPage
  StatisticPage.BIC(rundb.BPM_BIC_G_scale, rundb.FC_BIC_G_scale,
		    rundb.FC_BIC_UG_scale, rundb.BPM_BIC_G_ped,
		    rundb.FC_BIC_G_ped,	rundb.FC_BIC_UG_ped);
}

///////////////////////////////////////////////////////////////////////////////
// This is the Event loop
///////////////////////////////////////////////////////////////////////////////
int work (wxApp *app)
{  
  static const struct online onlineinit = { -1, 0, 0, 0 };
  char runname[32];
  if (atree.getRunName())	
    sprintf(runname, "run_%12s", atree.getRunName());
  //aqua  else if (atree.getInputFilename()) 
  //aqua    strncpy(runname, atree.getInputFilename(), 18);
  
  if (!firstInit) { // First initialization
    frame = (app ? (HMVFrame*)app->GetTopWindow() : (HMVFrame*)NULL);
    cntpage = (frame ? frame->counterPage : (HMVPage *)NULL);
    statpage = (frame ? frame->statPage : (HMVPage *)NULL);
    firstInit++;
  }

  switch (reader->nextEvent(&atree)) {
  case 1: // timeOut
    return 0;
    break;
  case -1: { // finished: clean up and write files
    HMWriteAll(online_his);  

    // Close ntuple
    if (ntuple) nt->delete_ntlist();

    if (cntpage && !cntpage->IsFrozen()) {
      int hpos = cntpage->textWindow->GetScrollPos(wxHORIZONTAL);
      int vpos = cntpage->textWindow->GetScrollPos(wxVERTICAL);
      cntpage->textWindow->Clear();
      std::ostream * stream = new std::ostream(cntpage->textWindow);
      printCounter(stream);
      delete stream;
      cntpage->textWindow->SetScrollPos(wxHORIZONTAL, hpos);
      cntpage->textWindow->SetScrollPos(wxVERTICAL, vpos);
    }

    env_def(online_cnt, "ONLINE_COUNTER",  "Online.cnt");
    std::ostream * onlcnt = new std::ofstream(online_cnt);
    printCounter(onlcnt);
    delete onlcnt;

    if (statpage && !statpage->IsFrozen()) {
      statpage->textWindow->Clear();
      std::ostream * stream = new std::ostream(statpage->textWindow);
      StatisticPage.print(stream, runname);
      StatisticPage.print(stream);
      delete stream;
    }

    env_def(online_stat, "ONLINE_STATPAGE", "Online.stat");
    std::ostream * onlstat = new std::ofstream(online_stat);
    StatisticPage.print(onlstat, runname);
    StatisticPage.print(onlstat);
    delete onlstat;

    online.datataking = atree.getTimestamp();
    online.RunTime = StatisticPage.getRunTime();

    env_def(lumi_out,   "LUMINOSITY_OUT", "Luminosity.out");
    FILE *lumi = fopen(lumi_out, "w");
    fprintf(lumi,"Integrated Luminsity: %.2f fbarn^-1\n",
	    online.IntegratedLuminosity/1e9);
    fprintf(lumi,"Events %ld\n",events);
    fprintf(lumi,"RunTime %.0g\n",online.RunTime);
    fclose(lumi); 

    //aqua      atree.closeIO();
    //aqua      out->closeIO();
    char status[255];
    sprintf(status, "events: %ld    run gate = %d:%02d:%02d", events,
	    (int)(online.RunTime/3600), (int)fmod(online.RunTime/60,60),
	    (int)fmod(online.RunTime,60));
    if (display) frame->SetStatusText(status);
    return 1;                      // This ends the X11 Callback loop!
  } break;
  default: // next event
    break;
  }

  if (dorewind || events == 0) { // Reinitialization, i.e. start or rewind
    if (display) frame->SetStatusText("Events: 0");
    readInputFiles();
    beamPuls = totalPuls = 0;
  }

  if(!dorewind && events==0 && display) {
    static char *title = new char[255];
    //    long int datatime =
    //      atree.getTimestamp() ? atree.getTimestamp() : time(NULL);
    sprintf(title,"Cindy++ Online Analysis");
    if (atree.getRunName())
      sprintf(title, "Cindy++ Online Analysis of \"run_%s\"",
	      atree.getRunName());
    //aqua    else if (atree.getInputFilename() && rundb.setup)
    //aqua      sprintf(title, "Cindy++ Online Analysis of \"%s\" (%s,%s)",
    //aqua	      atree.getInputFilename(), rundb.setup,
    //aqua              ctime((const time_t *) &datatime));

    frame->SetTitle(title);       
    frame->file_menu->AppendSeparator();
    frame->file_menu->Append(SHOWCNTPAGE, "Show &Counter Window");
    frame->file_menu->Append(SHOWSTATPAGE, "Show Statistic &Page");
    frame->file_menu->Append(REWIND, "&Rewind Input Data File");
  }
  
  if (dorewind) {
    //aqua    atree.rewind(); 
    dorewind = 0;
    StatisticPage.clear();
    if (display && frame->HMVhistogram) {
      if (!(frame->HMVhistogram = HMFind(frame->histos->GetLabel())))
	frame->HMVhistogram = HMFirst;
      frame->histos->SetLabel((char *) frame->HMVhistogram->title);
    }
  }
  
  online = onlineinit; // complete initialization of online structure

  static const double BeamEnergy = rundb.Ebeam / 1000.0;

  static const double oopsAmomentum = rundb.oopsA.momentum / 1000.0;
  static const double oopsAangle    = rundb.oopsA.angle * atan(1.0) / 45.0;
  static const double oopsAoopAngle = rundb.oopsA.oop_angle * atan(1.0) / 45.0;

  static const double oopsBmomentum = rundb.oopsB.momentum / 1000.0;
  static const double oopsBangle    = rundb.oopsB.angle * atan(1.0) / 45.0;
  static const double oopsBoopAngle = rundb.oopsB.oop_angle * atan(1.0) / 45.0;

  static const double oopsCmomentum = rundb.oopsC.momentum / 1000.0;
  static const double oopsCangle    = rundb.oopsC.angle * atan(1.0) / 45.0;
  static const double oopsCoopAngle = rundb.oopsC.oop_angle * atan(1.0) / 45.0;

  static const double oopsDmomentum = rundb.oopsD.momentum / 1000.0;
  static const double oopsDangle    = rundb.oopsD.angle * atan(1.0) / 45.0;
  static const double oopsDoopAngle = rundb.oopsD.oop_angle * atan(1.0) / 45.0;

  static const double ohipsMomentum = rundb.ohips.momentum / 1000.0;
  static const double ohipsAngle    = rundb.ohips.angle * atan(1.0) / 45.0;

  if (dorewind || events == 0) {  
    ;
  }

  online.datataking = atree.getTimestamp();
  out.packEventData(&online.datataking);

  online.RunTime = StatisticPage.getRunTime();
  out.packEventData(&online.RunTime);

  time_t now = time(NULL);
  if ((lastUpdateTime+10 <= now) || ((lastUpdateEvent+10)*100 <= events)) {
    lastUpdateTime = now;
    lastUpdateEvent = events/100;
    char status[255];
    sprintf(status, "events: %ld    run gate = %d:%02d:%02d", events,
	    (int)(online.RunTime/3600), (int)fmod(online.RunTime/60,60),
	    (int)fmod(online.RunTime,60));
    if (display) {
      frame->SetStatusText(status);
    } else std::cerr <<"\r"<<status<<" "<<std::flush;

    if (cntpage && !cntpage->IsFrozen()) {
      int hpos = cntpage->textWindow->GetScrollPos(wxHORIZONTAL);
      int vpos = cntpage->textWindow->GetScrollPos(wxVERTICAL);
      cntpage->textWindow->Clear();
      std::ostream * stream = new std::ostream(cntpage->textWindow);
      printCounter(stream);
      delete stream;
      cntpage->textWindow->SetScrollPos(wxHORIZONTAL, hpos);
      cntpage->textWindow->SetScrollPos(wxVERTICAL, vpos);
    }
    if (statpage && !statpage->IsFrozen()) {
      statpage->textWindow->Clear();
      std::ostream * stream = new std::ostream(statpage->textWindow);
      StatisticPage.print(stream);
      delete stream;
    }
  }
  events++;

  ////////////////////////////////////////////////////////////////////////////
  //	analyse event10 (beam information)
  ////////////////////////////////////////////////////////////////////////////

  int isEvent10 = 0;
  static struct BeamInfo beamInfo;

  if (atree.itemOK(&oops1.beam.helicity) || atree.itemOK(&oops1.beam.timeslot)) {
    switch (oops1.beam.helicity & 0x3) {
    case 1:
      beamInfo.helicity = (rundb.helicity_flipped != 0.0) ? 1 : -1;
      break;
    case 2:
      beamInfo.helicity = (rundb.helicity_flipped != 0.0) ? -1 : 1;
      break;
    default:
      beamInfo.helicity = 0;
      break;
    }
    // beamInfo.helicity = (short)(((oops1.beam.helicity & 0x3) % 3)*2) - 3;
    beamInfo.gunFlag  = (oops1.beam.helicity & 0x8) >> 3;
    if (beamInfo.gunFlag) beamPuls++;
    totalPuls++;
    memcpy(&beamInfo.hel_pattern, &oops1.beam.helicity, 10*sizeof(short));
    ++isEvent10;
  }

  // copy the last beam information to the online structure
  memcpy(&online.beam, &beamInfo, sizeof(struct BeamInfo));
  out.packEventData(&online.beam.helicity);
  out.packEventData(&online.beam.gunFlag, 11);

  ////////////////////////////////////////////////////////////////////////////
  //	Statistic Page
  ////////////////////////////////////////////////////////////////////////////

  StatisticPage.handle(&oops1); // will return quickly if not event5 or event6

  ////////////////////////////////////////////////////////////////////////////
  //	Drift Chambers -> Focal Plane coord's -> target coords
  ////////////////////////////////////////////////////////////////////////////

  TargetCo *resultA, *resultB, *resultC, *resultD, *resultO;
  resultA = resultB = resultC = resultD = resultO = NULL;

  static TCCC *tccA = new TCCC(1, rundb.oopsA.tcc.initfile);
  static TCCC *tccB = new TCCC(1, rundb.oopsB.tcc.initfile);
  static TCCC *tccC = new TCCC(1, rundb.oopsC.tcc.initfile);
  static TCCC *tccD = new TCCC(1, rundb.oopsD.tcc.initfile);
  static TCCC *tccO = new TCCC(0, rundb.ohips.tcc.initfile);

  // don't do detector analysis for beam events
  if (atree.itemOK(&oops1.det.flag) ||
      atree.itemOK(&oops1.det.latch1) ||
      atree.itemOK(&oops1.det.ohips.chamber.dcos)) {

    int AhdcOK, BhdcOK, ChdcOK, DhdcOK, OvdcOK;
    AhdcOK = BhdcOK = ChdcOK = DhdcOK = OvdcOK = 0;

    static TMAC *tmaA = new TMAC(rundb.oopsA.tma.initfile);
    static TMAC *tmaB = new TMAC(rundb.oopsB.tma.initfile);
    static TMAC *tmaC = new TMAC(rundb.oopsC.tma.initfile);
    static TMAC *tmaD = new TMAC(rundb.oopsD.tma.initfile);
    static TMAC *tmaO = new TMAC(rundb.ohips.tma.initfile);

    double xA = 0, yA = 0, tA = 0, pA = 0;
    double xB = 0, yB = 0, tB = 0, pB = 0;
    double xC = 0, yC = 0, tC = 0, pC = 0;
    double xD = 0, yD = 0, tD = 0, pD = 0;
    double xO = 0, yO = 0, tO = 0, pO = 0;

    int items;
    unsigned short data[6];

    if (atree.itemOK(&OopsA.scint[2].tdc.left)) {
      hdcA.Reset();
      if ((items = subAx1->pack(data, 3)) > 0) hdcAx1.FillData(data, 2*items);
      if ((items = subAy1->pack(data, 3)) > 0) hdcAy1.FillData(data, 2*items);
      if ((items = subAx2->pack(data, 3)) > 0) hdcAx2.FillData(data, 2*items);
      if ((items = subAy2->pack(data, 3)) > 0) hdcAy2.FillData(data, 2*items);
      if ((items = subAx3->pack(data, 3)) > 0) hdcAx3.FillData(data, 2*items);
      if ((items = subAy3->pack(data, 3)) > 0) hdcAy3.FillData(data, 2*items);
      hdcraw(&out, hdcA, &online.oopsA);
      if (hdcA.ok() == 63) {
	xA = hdcA.x(); tA = hdcA.theta();
	yA = hdcA.y(); pA = hdcA.phi(); AhdcOK = 1;
	if (tmaA) resultA = tmaA->run(xA, tA, yA, pA);
	hdccoord(&out, &online.oopsA.chamber, xA, tA, yA, pA);
	double dxA = hdcA.dx();
	double dtA = hdcA.dtheta();
	double dyA = hdcA.dy();
	double dpA = hdcA.dphi();
	hdccoord(&out, &online.oopsA.chamberError, dxA, dtA, dyA, dpA);
	FPcoord(&out, &online.oopsA.chamber,
		&online.oopsA.focal, rundb.oopsA.FPparam);
	hdccoord(&out, &online.oopsA.focalError, dxA, dtA, dyA, dpA);
	targetFillHisto(&online.oopsA.target, resultA);
      }
    }

    if (atree.itemOK(&OopsB.scint[2].tdc.left)) {
      hdcB.Reset();
      if ((items = subBx1->pack(data, 3)) > 0) hdcBx1.FillData(data, 2*items);
      if ((items = subBy1->pack(data, 3)) > 0) hdcBy1.FillData(data, 2*items);
      if ((items = subBx2->pack(data, 3)) > 0) hdcBx2.FillData(data, 2*items);
      if ((items = subBy2->pack(data, 3)) > 0) hdcBy2.FillData(data, 2*items);
      if ((items = subBx3->pack(data, 3)) > 0) hdcBx3.FillData(data, 2*items);
      if ((items = subBy3->pack(data, 3)) > 0) hdcBy3.FillData(data, 2*items);
      hdcraw(&out, hdcB, &online.oopsB);
      if (hdcB.ok() == 63) {
	xB = hdcB.x(); tB = hdcB.theta();
	yB = hdcB.y(); pB = hdcB.phi(); BhdcOK = 1;
	if (tmaB) resultB = tmaB->run(xB, tB, yB, pB);
	hdccoord(&out, &online.oopsB.chamber, xB, tB, yB, pB);
	double dxB = hdcB.dx();
	double dtB = hdcB.dtheta();
	double dyB = hdcB.dy();
	double dpB = hdcB.dphi();
	hdccoord(&out, &online.oopsB.chamberError, dxB, dtB, dyB, dpB);
	FPcoord(&out, &online.oopsB.chamber,
		&online.oopsB.focal, rundb.oopsB.FPparam);
	hdccoord(&out, &online.oopsB.focalError, dxB, dtB, dyB, dpB);
	targetFillHisto(&online.oopsB.target, resultB);
      }
    }

    if (atree.itemOK(&OopsC.scint[2].tdc.left)) {
      hdcC.Reset();
      if ((items = subCx1->pack(data, 3)) > 0) hdcCx1.FillData(data, 2*items);
      if ((items = subCy1->pack(data, 3)) > 0) hdcCy1.FillData(data, 2*items);
      if ((items = subCx2->pack(data, 3)) > 0) hdcCx2.FillData(data, 2*items);
      if ((items = subCy2->pack(data, 3)) > 0) hdcCy2.FillData(data, 2*items);
      if ((items = subCx3->pack(data, 3)) > 0) hdcCx3.FillData(data, 2*items);
      if ((items = subCy3->pack(data, 3)) > 0) hdcCy3.FillData(data, 2*items);
      hdcraw(&out, hdcC, &online.oopsC);
      if (hdcC.ok() == 63) {
	xC = hdcC.x(); tC = hdcC.theta();
	yC = hdcC.y(); pC = hdcC.phi(); ChdcOK = 1;
	if (tmaC) resultC = tmaC->run(xC, tC, yC, pC);
	hdccoord(&out, &online.oopsC.chamber, xC, tC, yC, pC);
	double dxC = hdcC.dx();
	double dtC = hdcC.dtheta();
	double dyC = hdcC.dy();
	double dpC = hdcC.dphi();
	hdccoord(&out, &online.oopsC.chamberError, dxC, dtC, dyC, dpC);
	FPcoord(&out, &online.oopsC.chamber,
		&online.oopsC.focal, rundb.oopsC.FPparam);
	hdccoord(&out, &online.oopsC.focalError, dxC, dtC, dyC, dpC);
	targetFillHisto(&online.oopsC.target, resultC);
      }
    }

    if (atree.itemOK(&OopsD.scint[2].tdc.left)) {
      hdcD.Reset();
      if ((items = subDx1->pack(data, 3)) > 0) hdcDx1.FillData(data, 2*items);
      if ((items = subDy1->pack(data, 3)) > 0) hdcDy1.FillData(data, 2*items);
      if ((items = subDx2->pack(data, 3)) > 0) hdcDx2.FillData(data, 2*items);
      if ((items = subDy2->pack(data, 3)) > 0) hdcDy2.FillData(data, 2*items);
      if ((items = subDx3->pack(data, 3)) > 0) hdcDx3.FillData(data, 2*items);
      if ((items = subDy3->pack(data, 3)) > 0) hdcDy3.FillData(data, 2*items);
      hdcraw(&out, hdcD, &online.oopsD);
      if (hdcD.ok() == 63) {
	xD = hdcD.x(); tD = hdcD.theta();
	yD = hdcD.y(); pD = hdcD.phi(); DhdcOK = 1;
	if (tmaD) resultD = tmaD->run(xD, tD, yD, pD);
	hdccoord(&out, &online.oopsD.chamber, xD, tD, yD, pD);
	double dxD = hdcD.dx();
	double dtD = hdcD.dtheta();
	double dyD = hdcD.dy();
	double dpD = hdcD.dphi();
	hdccoord(&out, &online.oopsD.chamberError, dxD, dtD, dyD, dpD);
	FPcoord(&out, &online.oopsD.chamber,
		&online.oopsD.focal, rundb.oopsD.FPparam);
	hdccoord(&out, &online.oopsD.focalError, dxD, dtD, dyD, dpD);
	targetFillHisto(&online.oopsD.target, resultD);
      }
    }

    if (atree.itemOK(&Ohips.scint[2].meantime) ||
	(atree.itemOK(&Ohips.scint[2].tdc.left) &&
	 atree.itemOK(&Ohips.scint[2].tdc.right)) ||
	(atree.itemOK(&Ohips.chamber.dcos) && (Ohips.chamber.dcos > 0))) {
      int wires;
      vdcO.Reset();
      if ((wires = subou1->pack(wiretime, maxWire)) > 0)
	ou1.FillTime((chRawData *)wiretime, 2*wires);
      if ((wires = subov1->pack(wiretime, maxWire)) > 0)
	ov1.FillTime((chRawData *)wiretime, 2*wires);
      if ((wires = subou2->pack(wiretime, maxWire)) > 0)
	ou2.FillTime((chRawData *)wiretime, 2*wires);
      if ((wires = subov2->pack(wiretime, maxWire)) > 0)
	ov2.FillTime((chRawData *)wiretime, 2*wires);
      vdcOHIPSraw(&out, vdcO, &online.ohips);
      if (vdcO.ok1() == PACKET_STATUS_BOTH) {
	xO = vdcO.x(); tO = vdcO.theta();
	yO = vdcO.y(); pO = vdcO.phi(); OvdcOK = 1;
	if (tmaO) resultO = tmaO->run(xO, tO, yO, pO);
	vdccoord(&out, &online.ohips.chamber, xO, tO, yO, pO);
	vdccoord(&out, &online.ohips.chamberError,
		 vdcO.dx(), vdcO.dtheta(), vdcO.dy(), vdcO.dphi());
	FPcoord(&out, &online.ohips.chamber,
		&online.ohips.focal, rundb.ohips.FPparam);
	vdccoord(&out, &online.ohips.focalError,
		 vdcO.dx(), vdcO.dtheta(), vdcO.dy(), vdcO.dphi());
	targetFillHisto(&online.ohips.target, resultO);
      }
    }
  } // end of if (!event10)

  /////////////////////////////////////////////////////////////////////////////
  // Cerenkov 
  /////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////
  // LeadGlass 
  /////////////////////////////////////////////////////////////////////////////

  online.ohips.leadglas.sum = 0.0;
  online.ohips.leadglas.top = 0.0;
  online.ohips.leadglas.btm = 0.0;

  for (int i=0; i<18; i++) {
    if (atree.itemOK(&oops1.det.ohips.leadglas[i+1].adc)) {
      if ( i < 9) {
	online.ohips.leadglas.sum += rundb.ohips.pbg_gmfactor[i]*
	  rundb.ohips.pbg_tbfactor*
	  (oops1.det.ohips.leadglas[i+1].adc - rundb.ohips.pbg_offset[i]);
	online.ohips.leadglas.top += rundb.ohips.pbg_gmfactor[i]*
	  rundb.ohips.pbg_tbfactor*
	  (oops1.det.ohips.leadglas[i+1].adc - rundb.ohips.pbg_offset[i]);
      } else {
	online.ohips.leadglas.btm += rundb.ohips.pbg_gmfactor[i]*
	  (oops1.det.ohips.leadglas[i+1].adc - rundb.ohips.pbg_offset[i]);
	online.ohips.leadglas.sum += rundb.ohips.pbg_gmfactor[i]*
	  (oops1.det.ohips.leadglas[i+1].adc - rundb.ohips.pbg_offset[i]);
      }
    }
  }

  out.packEventData(&online.ohips.leadglas.top);
  out.packEventData(&online.ohips.leadglas.btm);
  out.packEventData(&online.ohips.leadglas.sum);

  /////////////////////////////////////////////////////////////////////////////
  // Coincidence Time Correction
  /////////////////////////////////////////////////////////////////////////////

  double tdcO, tdcA, tdcB, tdcC, tdcD;
  tdcO = tdcA = tdcB = tdcC = tdcD = 0;

  // First check if items are OK
  if (atree.itemOK(&oops1.det.coincidence.tdc[8]))
    tdcO = oops1.det.coincidence.tdc[8];
  if (atree.itemOK(&oops1.det.coincidence.tdc[2]))
    tdcA = oops1.det.coincidence.tdc[2];
  if (atree.itemOK(&oops1.det.coincidence.tdc[3]))
    tdcB = oops1.det.coincidence.tdc[3];
  if (atree.itemOK(&oops1.det.coincidence.tdc[4]))
    tdcC = oops1.det.coincidence.tdc[4];
  if (atree.itemOK(&oops1.det.coincidence.tdc[5]))
    tdcD = oops1.det.coincidence.tdc[5];

  /* PAUL

  double s1a_adc_left, s1a_adc_right, s1b_adc_left, s1b_adc_right,
         s1c_adc_left, s1c_adc_right, s1d_adc_left, s1d_adc_right;
  s1a_adc_left = s1a_adc_right = s1b_adc_left = s1b_adc_right =
  s1c_adc_left = s1c_adc_right = s1d_adc_left = s1d_adc_right = 0;

  if (atree.itemOK(&Ohips.scint1[1].adc.left))
     s1a_adc_left = Ohips.scint1[1].adc.left;
  if (atree.itemOK(&Ohips.scint1[1].adc.right))
    s1a_adc_right = Ohips.scint1[1].adc.right;
  if (atree.itemOK(&Ohips.scint1[2].adc.left))
     s1b_adc_left = Ohips.scint1[2].adc.left;
  if (atree.itemOK(&Ohips.scint1[2].adc.right))
    s1b_adc_right = Ohips.scint1[2].adc.right;
  if (atree.itemOK(&Ohips.scint1[3].adc.left))
     s1c_adc_left = Ohips.scint1[3].adc.left;
  if (atree.itemOK(&Ohips.scint1[3].adc.right))
    s1c_adc_right = Ohips.scint1[3].adc.right;
  if (atree.itemOK(&Ohips.scint1[4].adc.left))
     s1d_adc_left = Ohips.scint1[4].adc.left;
  if (atree.itemOK(&Ohips.scint1[4].adc.right))
    s1d_adc_right = Ohips.scint1[4].adc.right;

  // Calculate the average ADC signal in each paddle of S1
  double s1a_adc_avg = sqrt(s1a_adc_left*s1a_adc_right);
  double s1b_adc_avg = sqrt(s1b_adc_left*s1b_adc_right);
  double s1c_adc_avg = sqrt(s1c_adc_left*s1c_adc_right);
  double s1d_adc_avg = sqrt(s1d_adc_left*s1d_adc_right);

  // Fix OHIPS S1 timing offsets.  First check which paddle fired
  // and apply offset to the respective paddle
  double tofO_a_o, tofO_b_o, tofO_c_o, tofO_d_o, tofO_o;
  tofO_a_o = tofO_b_o = tofO_c_o = tofO_d_o = tofO_o = 0;

  if (s1a_adc_left && s1a_adc_right) {
    tofO_a_o = (tdcO + rundb.Coinc.O_tdc[0])/rundb.Coinc.O_tdc[1]
      + rundb.ohips.S1_tdc_offset[0];
  }
  if (s1b_adc_left && s1b_adc_right) {
    tofO_b_o = (tdcO + rundb.Coinc.O_tdc[0])/rundb.Coinc.O_tdc[1]
      + rundb.ohips.S1_tdc_offset[1];
  }
  if (s1c_adc_left && s1c_adc_right) {
    tofO_c_o = (tdcO + rundb.Coinc.O_tdc[0])/rundb.Coinc.O_tdc[1]
      + rundb.ohips.S1_tdc_offset[2];
  }
  if (s1d_adc_left && s1d_adc_right) {
    tofO_d_o = (tdcO + rundb.Coinc.O_tdc[0])/rundb.Coinc.O_tdc[1]
      + rundb.ohips.S1_tdc_offset[3];
  }

  // Use the paddle with the largest average ADC to determine OHIPS timing
  tofO_o = tofO_a_o;
  if (s1b_adc_avg > s1a_adc_avg) tofO_o = tofO_b_o;
  if (s1c_adc_avg > s1b_adc_avg) tofO_o = tofO_c_o;
  if (s1d_adc_avg > s1c_adc_avg) tofO_o = tofO_d_o;

  // Get coinc times
  if (tdcO && tdcA) {
    online.oa_tc = tofO_o - (tdcA + rundb.Coinc.A_tdc[0])/rundb.Coinc.A_tdc[1];
    out.packEventData(&online.oa_tc);
  }
  if (tdcO && tdcB) {
    online.ob_tc = tofO_o - (tdcB + rundb.Coinc.B_tdc[0])/rundb.Coinc.B_tdc[1];
    out.packEventData(&online.ob_tc);
  }
  if (tdcO && tdcC) {
    online.oc_tc = tofO_o - (tdcC + rundb.Coinc.C_tdc[0])/rundb.Coinc.C_tdc[1];
    out.packEventData(&online.oc_tc);
  }
  if (tdcO && tdcD) {
    online.od_tc = tofO_o - (tdcD + rundb.Coinc.D_tdc[0])/rundb.Coinc.D_tdc[1];
    out.packEventData(&online.od_tc);
  }

  PAUL */

  if (tdcO && tdcA) {
    online.oa_tc = (tdcO + rundb.Coinc.O_tdc[0])/rundb.Coinc.O_tdc[1]
                 - (tdcA + rundb.Coinc.A_tdc[0])/rundb.Coinc.A_tdc[1];
    out.packEventData(&online.oa_tc);
  }

  if (tdcO && tdcB) {
    online.ob_tc = (tdcO + rundb.Coinc.O_tdc[0])/rundb.Coinc.O_tdc[1]
                 - (tdcB + rundb.Coinc.B_tdc[0])/rundb.Coinc.B_tdc[1];
    out.packEventData(&online.ob_tc);
  }

  if (tdcO && tdcC) {
    online.oc_tc = (tdcO + rundb.Coinc.O_tdc[0])/rundb.Coinc.O_tdc[1]
                 - (tdcC + rundb.Coinc.C_tdc[0])/rundb.Coinc.C_tdc[1];
    out.packEventData(&online.oc_tc);
  }

  if (tdcO && tdcD) {
    online.od_tc = (tdcO + rundb.Coinc.O_tdc[0])/rundb.Coinc.O_tdc[1]
                 - (tdcD + rundb.Coinc.D_tdc[0])/rundb.Coinc.D_tdc[1];
    out.packEventData(&online.od_tc);
  }

  /////////////////////////////////////////////////////////////////////////////
  // Vertex and Four-vectors
  /////////////////////////////////////////////////////////////////////////////

  Particle A_Out = P_proton;
  Particle B_Out = P_proton;
  Particle C_Out = P_proton;
  Particle D_Out = P_proton;
  Particle O_Out = P_electron;
  Particle H3_Out = P_proton;
  Particle TOF_Out = P_neutron;
  Particle NPOL_Out = P_neutron;
  Particle NDET_Out = P_neutron;

  if (Reaction && Reaction->getA() != NULL) A_Out = *Reaction->getA();
  if (Reaction && Reaction->getB() != NULL) B_Out = *Reaction->getB();
  if (Reaction && Reaction->getC() != NULL) C_Out = *Reaction->getC();
  if (Reaction && Reaction->getD() != NULL) D_Out = *Reaction->getD();
  if (Reaction && Reaction->getO() != NULL) O_Out = *Reaction->getO();
  if (Reaction && Reaction->getH3() != NULL) H3_Out = *Reaction->getH3();
  if (Reaction && Reaction->getTOF() != NULL) TOF_Out = *Reaction->getTOF();
  if (Reaction && Reaction->getNPOL() != NULL) NPOL_Out = *Reaction->getNPOL();
  if (Reaction && Reaction->getNDET() != NULL) NDET_Out = *Reaction->getNDET();

  // Vertex

  if (!focus) focus = new Focus(rundb.bpm,
				rundb.ohips.angle*M_PI/180.0,
				rundb.oopsA.angle*M_PI/180.0,
				rundb.oopsB.angle*M_PI/180.0,
				rundb.oopsC.angle*M_PI/180.0,
				rundb.oopsD.angle*M_PI/180.0);

  double box =
    online.oopsA.vertex.x = online.oopsB.vertex.x =
    online.oopsC.vertex.x = online.oopsD.vertex.x =
    online.ohips.vertex.x = rundb.beam.offset.x;
  double boy =
    online.oopsA.vertex.y = online.oopsB.vertex.y =
    online.oopsC.vertex.y = online.oopsD.vertex.y =
    online.ohips.vertex.y = rundb.beam.offset.y;

  // Vertex is always defined by BPMs and/or OHIPS

  if (atree.itemOK(&oops1.det.beam.BPM[1]) && // if only BPM0 works
      atree.itemOK(&oops1.det.beam.BPM[2]) &&
      !atree.itemOK(&oops1.det.beam.BPM[3]) &&
      !atree.itemOK(&oops1.det.beam.BPM[4])) {
    online.ohips.vertex.x += focus->hor(oops1.det.beam.BPM[1]);
    online.ohips.vertex.y += focus->vert(oops1.det.beam.BPM[2]);
  } else if (!atree.itemOK(&oops1.det.beam.BPM[1]) && // if only BPM1 works
	     !atree.itemOK(&oops1.det.beam.BPM[2]) &&
	     atree.itemOK(&oops1.det.beam.BPM[3]) &&
	     atree.itemOK(&oops1.det.beam.BPM[4])) {
    online.ohips.vertex.x += focus->hor(oops1.det.beam.BPM[3]);
    online.ohips.vertex.y += focus->vert(oops1.det.beam.BPM[4]);
  } else if (atree.itemOK(&oops1.det.beam.BPM[1]) && // if both BPMs work
	     atree.itemOK(&oops1.det.beam.BPM[2]) &&
	     atree.itemOK(&oops1.det.beam.BPM[3]) &&
	     atree.itemOK(&oops1.det.beam.BPM[4])) {
    online.ohips.vertex.x += focus->hor(oops1.det.beam.BPM[1],
					oops1.det.beam.BPM[3]);
    online.ohips.vertex.y += focus->vert(oops1.det.beam.BPM[2],
					 oops1.det.beam.BPM[4]);
  } else { // if BPMs don't work
    online.ohips.vertex.x += 0;
    online.ohips.vertex.y += 0;
  }

  // Consider three cases:

  // 1 -- BPMs are valid, OHIPS has a track, use all this information
  //      to get vertex x, y, z (use_vertex=="BPM" in the run.db)
  // 2 -- BPMs are valid, OHIPS has a track, but ignore BPMs, just
  //      set vertex x, y to run.db offsets, and use OHIPS to get z
  //      (use_vertex=="OHIPS")
  // 3 -- ignore all BPM and tracking information, set x and y to
  //      run.db offsets and z to 0

  if (resultO) {
    if (!strcmp(rundb.use_vertex, "BPM")) {
      online.ohips.vertex.z  = focus->z_by_O(online.ohips.vertex.x, resultO);
    } else if (!strcmp(rundb.use_vertex, "OHIPS")) {
      online.oopsA.vertex.x = online.oopsB.vertex.x = online.oopsC.vertex.x =
	online.oopsD.vertex.x = online.ohips.vertex.x = box;
      online.oopsA.vertex.y = online.oopsB.vertex.y = online.oopsC.vertex.y =
	online.oopsD.vertex.y = online.ohips.vertex.y = boy;
      online.ohips.vertex.z  = focus->z_by_O(online.ohips.vertex.x, resultO);
    } else {
      online.ohips.vertex.z = 0;
    }
  } else {
    online.ohips.vertex.z = 0;
  }

  out.packEventData(&online.ohips.vertex.x, 3);
  online.Vertex.x = online.ohips.vertex.x;
  online.Vertex.y = online.ohips.vertex.y; 
  online.Vertex.z = online.ohips.vertex.z; 
  out.packEventData(&online.Vertex.x, 3);

  // Four-vectors 

  if (resultA) {
    A_Out = particle(A_Out.getMass(), oopsAmomentum * (1.0+resultA->dp/100.0),
		     oopsAangle, oopsAoopAngle,
		     resultA->ph/1000.0, -resultA->th/1000.0);
    online.oopsA.particle.id = A_Out.getID();
    online.oopsA.particle.mass = A_Out.getMass();
    online.oopsA.particle.Ekin = A_Out.Ekin();
    online.oopsA.particle.momentum = A_Out.momentum();
    online.oopsA.particle.fourvec[0] = A_Out[0];
    online.oopsA.particle.fourvec[1] = A_Out[1];
    online.oopsA.particle.fourvec[2] = A_Out[2];
    online.oopsA.particle.fourvec[3] = A_Out[3];
    out.packEventData(&online.oopsA.particle.id);
    out.packEventData(&online.oopsA.particle.mass, 3);
    out.packEventData(&online.oopsA.particle.fourvec[0], 4);

    if (ergloss) { 
      double EnergyA_Before = A_Out.energy();
      target->EnergyLossCorrChamber(A_Out);
      target->EnergyLossCorr(A_Out, 
			     online.Vertex.x - rundb.Target.offset.x,
			     online.Vertex.y - rundb.Target.offset.y,
			     online.Vertex.z - rundb.Target.offset.z, steps_A);
      online.oopsA.Eloss_corr = (A_Out.energy() - EnergyA_Before);
      out.packEventData(&online.oopsA.Eloss_corr);
    }
  }

  if (resultB) {
    B_Out = particle(B_Out.getMass(), oopsBmomentum * (1.0+resultB->dp/100.0),
		     oopsBangle, oopsBoopAngle,
		     resultB->ph/1000.0, -resultB->th/1000.0);
    online.oopsB.particle.id = B_Out.getID();
    online.oopsB.particle.mass = B_Out.getMass();
    online.oopsB.particle.Ekin = B_Out.Ekin();
    online.oopsB.particle.momentum = B_Out.momentum();
    online.oopsB.particle.fourvec[0] = B_Out[0];
    online.oopsB.particle.fourvec[1] = B_Out[1];
    online.oopsB.particle.fourvec[2] = B_Out[2];
    online.oopsB.particle.fourvec[3] = B_Out[3];
    out.packEventData(&online.oopsB.particle.id);
    out.packEventData(&online.oopsB.particle.mass, 3);
    out.packEventData(&online.oopsB.particle.fourvec[0], 4);

    if (ergloss) { 
      double EnergyB_Before = B_Out.energy();
      target->EnergyLossCorrChamber(B_Out);
      target->EnergyLossCorr(B_Out, 
			     online.Vertex.x - rundb.Target.offset.x,
			     online.Vertex.y - rundb.Target.offset.y,
			     online.Vertex.z - rundb.Target.offset.z, steps_B);
      online.oopsB.Eloss_corr = (B_Out.energy() - EnergyB_Before);
      out.packEventData(&online.oopsB.Eloss_corr);
    }
  }

  if (resultC) {
    C_Out = particle(C_Out.getMass(), oopsCmomentum * (1.0+resultC->dp/100.0),
		     oopsCangle, oopsCoopAngle,
		     resultC->ph/1000.0, -resultC->th/1000.0);
    online.oopsC.particle.id = C_Out.getID();
    online.oopsC.particle.mass = C_Out.getMass();
    online.oopsC.particle.Ekin = C_Out.Ekin();
    online.oopsC.particle.momentum = C_Out.momentum();
    online.oopsC.particle.fourvec[0] = C_Out[0];
    online.oopsC.particle.fourvec[1] = C_Out[1];
    online.oopsC.particle.fourvec[2] = C_Out[2];
    online.oopsC.particle.fourvec[3] = C_Out[3];
    out.packEventData(&online.oopsC.particle.id);
    out.packEventData(&online.oopsC.particle.mass, 3);
    out.packEventData(&online.oopsC.particle.fourvec[0], 4);

    if (ergloss) { 
      double EnergyC_Before = C_Out.energy();
      target->EnergyLossCorrChamber(C_Out);
      target->EnergyLossCorr(C_Out,  
			     online.Vertex.x - rundb.Target.offset.x,
			     online.Vertex.y - rundb.Target.offset.y,
			     online.Vertex.z - rundb.Target.offset.z, steps_C);
      online.oopsC.Eloss_corr = (C_Out.energy() - EnergyC_Before);
      out.packEventData(&online.oopsC.Eloss_corr);
    }
  }

  if (resultD) {
    D_Out = particle(D_Out.getMass(), oopsDmomentum * (1.0+resultD->dp/100.0),
		     oopsDangle, oopsDoopAngle,
		     resultD->ph/1000.0, -resultD->th/1000.0);
    online.oopsD.particle.id = D_Out.getID();
    online.oopsD.particle.mass = D_Out.getMass();
    online.oopsD.particle.Ekin = D_Out.Ekin();
    online.oopsD.particle.momentum = D_Out.momentum();
    online.oopsD.particle.fourvec[0] = D_Out[0];
    online.oopsD.particle.fourvec[1] = D_Out[1];
    online.oopsD.particle.fourvec[2] = D_Out[2];
    online.oopsD.particle.fourvec[3] = D_Out[3];
    out.packEventData(&online.oopsD.particle.id);
    out.packEventData(&online.oopsD.particle.mass, 3);
    out.packEventData(&online.oopsD.particle.fourvec[0], 4);

    if (ergloss) { 
      double EnergyD_Before = D_Out.energy();
      target->EnergyLossCorrChamber(D_Out);
      target->EnergyLossCorr(D_Out,  
			     online.Vertex.x - rundb.Target.offset.x,
			     online.Vertex.y - rundb.Target.offset.y,
			     online.Vertex.z - rundb.Target.offset.z, steps_D);
      online.oopsD.Eloss_corr = (D_Out.energy() - EnergyD_Before);
      out.packEventData(&online.oopsD.Eloss_corr);
    }
  }

  if (resultO) {
    O_Out = particle(O_Out.getMass(), ohipsMomentum * (1.0+resultO->dp/100.0),
		     ohipsAngle, resultO->ph/1000.0, -resultO->th/1000.0);
    online.ohips.particle.id = O_Out.getID();
    online.ohips.particle.mass = O_Out.getMass();
    online.ohips.particle.Ekin = O_Out.Ekin();
    online.ohips.particle.momentum = O_Out.momentum();
    online.ohips.particle.fourvec[0] = O_Out[0];
    online.ohips.particle.fourvec[1] = O_Out[1];
    online.ohips.particle.fourvec[2] = O_Out[2];
    online.ohips.particle.fourvec[3] = O_Out[3];
    out.packEventData(&online.ohips.particle.id);
    out.packEventData(&online.ohips.particle.mass, 3);
    out.packEventData(&online.ohips.particle.fourvec[0], 4);

    if (ergloss) { 
      double EnergyO_Before = O_Out.energy();
      target->EnergyLossCorrChamber(O_Out);
      // have to correct for additional air between OHIPS and target
      target->EnergyLossCorr(O_Out, 
			     online.Vertex.x - rundb.Target.offset.x,
			     online.Vertex.y - rundb.Target.offset.y,
			     online.Vertex.z - rundb.Target.offset.z, steps_O);
      online.ohips.Eloss_corr = (O_Out.energy() - EnergyO_Before);
      out.packEventData(&online.ohips.Eloss_corr);
    }
  }

  // Path-length correction of tcoinc -----------------------------------------

  double timeA, timeB, timeC, timeD, timeO, time,
    pathA, pathB, pathC, pathD, pathO,
    toffA, toffB, toffC, toffD;

  toffA = toffB = toffC = toffD = time = 0;
  int type = 0;

  // initialize paths to lengths central rays

  pathA = tccA->central();
  pathB = tccB->central();
  pathC = tccC->central();
  pathD = tccD->central();
  pathO = tccO->central();

  // OOPSA-OHIPS coincs

  if (tdcO && tdcA) {
    if (resultA) pathA += tccA->run(resultA);
    if (resultO) pathO += tccO->run(resultO);
    timeA = pathA/A_Out.beta().abs()/V_LIGHT;
    timeO = pathO/O_Out.beta().abs()/V_LIGHT;
    if (resultO && resultA) {
      type = 3;
      time = online.oa_tc - (timeO-tccO->r00()) + (timeA-tccA->r00());
    } else if (resultO && !resultA) {
      type = 2;
      time = online.oa_tc - (timeO-tccO->r00());
    } else if (!resultO && resultA) {
      type = 1;
      time = online.oa_tc + (timeA-tccA->r00());
    } else {
      type = 0;
      time = online.oa_tc;
    }
    online.oa_tccorr_type = type;
    online.oa_tccorr = time;
    out.packEventData(&online.oa_tccorr_type);
    out.packEventData(&online.oa_tccorr);
  }
  
  // OOPSB-OHIPS coincs

  if (tdcO && tdcB) {
    if (resultB) pathB += tccB->run(resultB);
    if (resultO) pathO += tccO->run(resultO);
    timeB = pathB/B_Out.beta().abs()/V_LIGHT;
    timeO = pathO/O_Out.beta().abs()/V_LIGHT;
    if (resultO && resultB) {
      type = 3;
      time = online.ob_tc - (timeO-tccO->r00()) + (timeB-tccB->r00());
    } else if (resultO && !resultB) {
      type = 2;
      time = online.ob_tc - (timeO-tccO->r00());
    } else if (!resultO && resultB) {
      type = 1;
      time = online.ob_tc + (timeB-tccB->r00());
    } else {
      type = 0;
      time = online.ob_tc;
    }
    online.ob_tccorr_type = type;
    online.ob_tccorr = time;
    out.packEventData(&online.ob_tccorr_type);
    out.packEventData(&online.ob_tccorr);
  }
  
  // OOPSC-OHIPS coincs

  if (tdcO && tdcC) {
    if (resultC) pathC += tccC->run(resultC);
    if (resultO) pathO += tccO->run(resultO);
    timeC = pathC/C_Out.beta().abs()/V_LIGHT;
    timeO = pathO/O_Out.beta().abs()/V_LIGHT;
    if (resultO && resultC) {
      type = 3;
      time = online.oc_tc - (timeO-tccO->r00()) + (timeC-tccC->r00());
    } else if (resultO && !resultC) {
      type = 2;
      time = online.oc_tc - (timeO-tccO->r00());
    } else if (!resultO && resultC) {
      type = 1;
      time = online.oc_tc + (timeC-tccC->r00());
    } else {
      type = 0;
      time = online.oc_tc;
    }
    online.oc_tccorr_type = type;
    online.oc_tccorr = time;
    out.packEventData(&online.oc_tccorr_type);
    out.packEventData(&online.oc_tccorr);
  }
  
  // OOPSD-OHIPS coincs

  if (tdcO && tdcD) {
    if (resultD) pathD += tccD->run(resultD);
    if (resultO) pathO += tccO->run(resultO);
    timeD = pathD/D_Out.beta().abs()/V_LIGHT;
    timeO = pathO/O_Out.beta().abs()/V_LIGHT;
    if (resultO && resultD) {
      type = 3;
      time = online.od_tc - (timeO-tccO->r00()) + (timeD-tccD->r00());
    } else if (resultO && !resultD) {
      type = 2;
      time = online.od_tc - (timeO-tccO->r00());
    } else if (!resultO && resultD) {
      type = 1;
      time = online.od_tc + (timeD-tccD->r00());
    } else {
      type = 0;
      time = online.od_tc;
    }
    online.od_tccorr_type = type;
    online.od_tccorr = time;
    out.packEventData(&online.od_tccorr_type);
    out.packEventData(&online.od_tccorr);
  }
  
  // --------------------------------------------------------------------------

  // Fill Histograms
  
  online.nr = events;
  out.packEventData(&online.nr);

  Particle electronIn = P_electron;
  electronIn.setMomentum(0, 0, momentum(BeamEnergy, M_ELECTRON));
  if (ergloss) {
    target->EnergyLossCorrBeam(electronIn,
			       online.Vertex.x - rundb.Target.offset.x,
			       online.Vertex.y - rundb.Target.offset.y,
			       online.Vertex.z - rundb.Target.offset.z,
			       steps_IN);
    online.beam.Eloss_corr = (BeamEnergy - electronIn.energy());
    out.packEventData(&online.beam.Eloss_corr);
  }

  static FourVector dummy(0, 0, 0, 0);
  if (!resultA) A_Out=dummy;
  if (!resultB) B_Out=dummy;
  if (!resultC) C_Out=dummy;
  if (!resultD) D_Out=dummy;
  if (!resultO) O_Out=dummy;
  H3_Out  = dummy;
  TOF_Out = dummy;
  NPOL_Out = dummy;
  NDET_Out = dummy;

  signal(SIGFPE,signalhandler);
  if (!setjmp(env))
    if (evaluate_expressions(A_Out, B_Out, C_Out, D_Out, O_Out, 
			     H3_Out, TOF_Out, NPOL_Out, NDET_Out, dummy,
			     electronIn))
      reader->writeEvent();

  /////////////////////////////////////////////////////////////////////////////
  // write ntuple event
  if (ntuple) nt->fill_ntuple();
  //
  ///////////////////////////////////////////////////////////////////////

  out.clear();

  return 0; // This means for an X11 working procedure: run again
}

void targetFillHisto(struct TargetCoord *t, struct TargetCo *tgt)
{
  if (!tgt || !t) return;

  t->dp = tgt->dp;    
  t->th = tgt->th;
  t->y0 = tgt->y0;     
  t->ph = tgt->ph;
  t->len = tgt->len;
  out.packEventData(&t->dp, 5);
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
    break;
  case REWIND: 
    dorewind = 1;
    events = 0;
    break;
  }
}

void signalhandler(int)
{
  std::cerr << "Floating point exception caught! Event " << 
    events << " skipped." << std::endl << std::flush;
  longjmp(env,1);
}

int printHelp(const char *progname)
{
  std::cerr << progname << " - new Cola version using Aqua libraries" << std::endl;
  std::cerr << "Usage:" << std::endl;
  std::cerr << "\t-c,         --cindy          use old cindy format" << std::endl;
  std::cerr << "\t-f <name>,  --file=<name>    read data from file" << std::endl;
  std::cerr << "\t-O <name>,  --histo=<name>   histogram file" << std::endl;
  std::cerr << "\t-R <name>,  --rz=<name>      ntuple file" << std::endl;
  std::cerr << "\t-o <name>,  --output=<name>  write data to file" << std::endl;
  std::cerr << "\t-z,         --gzip           compress output using gzip" << std::endl;
  std::cerr << "\t-h,         --help           this help" << std::endl;
  std::cerr << "\t-v          --version        print version info" << std::endl;
  return 0;
}

int printVersion(const char *progname)
{
  std::cerr << progname << " - $Revision: 2216 $" << std::endl;
  return 0;
}

int main(int argc, char *argv[])
{
  int opt, cindyFormat = 0;
  int useGZIP = 0;
  char *progname = argv[0];
  char *dataFile = NULL;
  char *output = NULL;

  env_set(online_his, "ONLINE_HIS", "Online.his");
  env_set(online_rz,  "ONLINE_RZ",  "Online.rz");

#if defined(__GNU_LIBRARY__)
  struct option lo[] = {
    {"cindy",   0, NULL, 'c'},
    {"file",    1, NULL, 'f'},
    {"histo",   1, NULL, 'O'},
    {"rz",      1, NULL, 'R'},
    {"output",  1, NULL, 'o'},
    {"help",    0, NULL, 'h'},
    {"gzip",    0, NULL, 'z'},
    {"version", 0, NULL, 'v'},
    {NULL, 0, NULL, 0}};
  while ((opt = getopt_long(argc, argv, "cf:O:R:o:vzh?", lo, NULL)) != EOF) {
#else
  while ((opt = getopt(argc, argv, "cf:O:R:o:vzh?")) != EOF) {
#endif
    switch (opt) {
    case 'c': cindyFormat++;          break;
    case 'f': dataFile = optarg;      break;
    case 'O': online_his = optarg;    break;
    case 'R': online_rz  = optarg;    break;
    case 'o': output = optarg;        break;
    case 'z': useGZIP = 1;            break;
    case 'v': printVersion(progname); return 0;
    default:  printHelp(progname);    return 0;
    }
  }

  // parse input parameter (if any)
  AquaIO *aquaIO = NULL;
  if (!dataFile || (*dataFile=='-')) {
    aquaIO = new AquaUnzip(0);
  } else aquaIO = new AquaUnzip(dataFile);

  // main loop
  reader = (cindyFormat ?
	    (AquaReader *)new CindyReader(aquaIO) :
	    (AquaReader *)new BatesReader(aquaIO));
  reader->initTree(&atree);

  if (output) reader->initOutput(useGZIP ? new AquaGzip(output)
				 : new AquaIO(output, 0644));
  
  //aqua if (atree.analyseGetOpt(ac, av)) return 0;
  HMMenuCallback = MenuCallback; // Call back for our new menu entry
  if (display) {
    HMLaunchViewWork(argc, argv, (void(*)(wxApp*))work);
  } else while(work(NULL) != 1);  // Event loop 

  return 0;
}
