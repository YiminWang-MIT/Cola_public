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

#include <config.h>
#include "FourVector/FourVector.h"
#include "Cindy/Cindy++.h"
#include "TMA/FastTM.h"
#include "wx.h"
#include "wx_main.h"
#include "wx_timer.h"
#include "HMBook/hmbook.h"     // HMBook X11 Histogram library
#include "HMBook/hmview.h"     // HMBook X11 Histogram Browser
#include <iomanip.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <fstream.h>
#include "Cola/Masses.h"       // Particle Masses
#include "Cola/Reactions.h"    // Definition of Reactions, Particles
#include "rundatabase.h"       // "run.db" Parser
#include "VDCraw.h"
#include "hrs.h"               // Input data structure for Cindy++
#include "online.h"            // 1st level reconstruced data 
#include "StatPage.h"
#include "Cola.h"              // Headers and static variables

const double M_ELECTRON		= 5.1099906E-4;	// electron mass
const double V_LIGHT		= 29.9792458;   // cm/ns

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
  memset(&rundb, 0, sizeof(rundatabase));
  rundb.reaction="H(e,e'p)pi0";

  long int datatime =
    ct->getTimeOfDataTaking() ? ct->getTimeOfDataTaking() : time(NULL);
  readRunDatabase(run_db, datatime);
  Reaction = new reaction(rundb.reaction);
  if (Reaction) rundb.Target = Reaction->getTarget()->getMass();
  vdcOHIPSinit(vdcE, VDC_MAXWORD, VDC_MAXWORD, VDC_MAXWORD, VDC_MAXWORD);
  vdcOHIPSinit(vdcH, VDC_MAXWORD, VDC_MAXWORD, VDC_MAXWORD, VDC_MAXWORD);

  env_def(online_col, "ONLINE_COL", "Online.col");
  parse_file(ct, out, Reaction, online_col);
}

///////////////////////////////////////////////////////////////////////////////
// This is the Event loop
///////////////////////////////////////////////////////////////////////////////
int work (wxApp *app)
{  
  static const struct online onlineinit = { -1, 0, 0, 0 };
  
  if (!firstInit) { // First initialization
    frame = (app ? (HMVFrame*)app->GetTopWindow() : (HMVFrame*)NULL);
    cntpage = (frame ? frame->counterPage : (HMVPage *)NULL);
    statpage = (frame ? frame->statPage : (HMVPage *)NULL);
    firstInit++;
  }

  switch (ct->analyseNextEvent()) {
  case 1: // timeOut
    return 0;
    break;
  case -1: // finished: clean up and write files
    {
      env_def(online_his, "ONLINE_HIS",     "Online.his");
      HMWriteAll(online_his);  

      env_def(online_cnt, "ONLINE_COUNTER",  "Online.cnt");
      ostream * onlcnt = new ofstream(online_cnt);
      printCounter(onlcnt);
      delete onlcnt;

      env_def(online_stat, "ONLINE_STATPAGE",  "Online.stat");
      ostream * onlstat = new ofstream(online_stat);
      StatisticPage.print(onlstat);
      delete onlstat;

      online.datataking = ct->getTimeOfDataTaking();
      online.RunTime = StatisticPage.getRunTime();

      env_def(lumi_out,   "LUMINOSITY_OUT", "Luminosity.out");
      FILE *lumi = fopen(lumi_out, "w");
      fprintf(lumi,"Integrated Luminsity: %.2f fbarn^-1\n",
	      online.IntegratedLuminosity/1e9);
      fprintf(lumi,"Events %ld\n",events);
      fprintf(lumi,"RunTime %.0g\n",online.RunTime);
      fclose(lumi); 

      ct->closeIO();
      out->closeIO();
      char status[255];
      sprintf(status, "events: %ld    run gate = %d:%02d:%02d", events,
	      (int)(online.RunTime/3600), (int)fmod(online.RunTime/60,60),
	      (int)fmod(online.RunTime,60));
      if (display) frame->SetStatusText(status);
      return 1;                      // This ends the X11 Callback loop!
    }
    break;
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
    long int datatime =
      ct->getTimeOfDataTaking() ? ct->getTimeOfDataTaking() : time(NULL);
    sprintf(title,"Cindy++ Online Analysis");
    if (ct->getRunName())
      sprintf(title, "Cindy++ Online Analysis of \"run_%s\"",ct->getRunName());
    else if (ct->getInputFilename() && rundb.setup)
      sprintf(title, "Cindy++ Online Analysis of \"%s\" (%s,%s)",
	      ct->getInputFilename(), rundb.setup,
              ctime((const time_t *) &datatime));

    frame->SetTitle(title);       
    frame->file_menu->AppendSeparator();
    frame->file_menu->Append(SHOWCNTPAGE, "Show &Counter Window");
    frame->file_menu->Append(SHOWSTATPAGE, "Show Statistic &Page");
    frame->file_menu->Append(REWIND, "&Rewind Input Data File");
  }
  
  if (dorewind) {
    ct->rewind(); 
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

  online.datataking = ct->getTimeOfDataTaking();
  out->packEventData(&online.datataking);

  online.RunTime = StatisticPage.getRunTime();
  out->packEventData(&online.RunTime);

  if (!(events % 5000)) {
    if (cntpage && !cntpage->IsFrozen()) {
      int hpos = cntpage->textWindow->GetScrollPos(wxHORIZONTAL);
      int vpos = cntpage->textWindow->GetScrollPos(wxVERTICAL);
      cntpage->textWindow->Clear();
      ostream * stream = new ostream(cntpage->textWindow);
      printCounter(stream);
      delete stream;
      cntpage->textWindow->SetScrollPos(wxHORIZONTAL, hpos);
      cntpage->textWindow->SetScrollPos(wxVERTICAL, vpos);
    }
    if (statpage && !statpage->IsFrozen()) {
      statpage->textWindow->Clear();
      ostream * stream = new ostream(statpage->textWindow);
      StatisticPage.print(stream);
      delete stream;
    }
  }
  if (!(++events % (ct->getInputFilename() ? 5000 : 500))) {
    char status[255];
    sprintf(status, "events: %ld    run gate = %d:%02d:%02d", events,
	    (int)(online.RunTime/3600), (int)fmod(online.RunTime/60,60),
	    (int)fmod(online.RunTime,60));
    if (display) frame->SetStatusText(status);
    else cerr <<"\r"<<status<<" "<<flush;
  }

  ////////////////////////////////////////////////////////////////////////////
  //	Statistic Page
  ////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////
  //	Trigger
  ////////////////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////
  //	Drift Chambers -> Focal Plane coord's -> target coords
  ////////////////////////////////////////////////////////////////////////////

  TargetCo *resultE, *resultH;
  resultE = resultH = NULL;

  //  int EvdcOK, HvdcOK;
  //  EvdcOK = HvdcOK = 0;

  //  static TMAC *tmaE = new TMAC(rundb.oopsA.tma.initfile);
  //  static TMAC *tmaH = new TMAC(rundb.oopsB.tma.initfile);

  //  double xE = 0, yE = 0, tE = 0, pE = 0;
  //  double xH = 0, yH = 0, tH = 0, pH = 0;

  /*
    if (ct->itemOK(&Ohips.scint[2].meantime) ||
	(ct->itemOK(&Ohips.scint[2].tdc.left) &&
	 ct->itemOK(&Ohips.scint[2].tdc.right)) ||
	(ct->itemOK(&Ohips.chamber.dcos) && (Ohips.chamber.dcos > 0))) {
      vdcOHIPSraw(vdcO, &online.ohips);
      if (vdcO.ok1() == PACKET_STATUS_BOTH) {
	xO = vdcO.x(); tO = vdcO.theta();
	yO = vdcO.y(); pO = vdcO.phi(); OvdcOK = 1;
	if (tmaO) resultO = tmaO->run(xO, tO, yO, pO);
	vdccoord(&online.ohips.chamber, xO, tO, yO, pO);
	vdccoord(&online.ohips.chamberError,
		 vdcO.dx(), vdcO.dtheta(), vdcO.dy(), vdcO.dphi());
	FPcoord(&online.ohips.chamber,&online.ohips.focal,rundb.ohips.FPparam);
	vdccoord(&online.ohips.focalError,
		 vdcO.dx(), vdcO.dtheta(), vdcO.dy(), vdcO.dphi());
	targetFillHisto(&online.ohips.target, resultO);
      }
    }
  */

  //  vdcE.Reset();
  //  vdcH.Reset();

  /////////////////////////////////////////////////////////////////////////////
  // Cerenkov 
  /////////////////////////////////////////////////////////////////////////////


  /////////////////////////////////////////////////////////////////////////////
  // Coincidence Time Correction
  /////////////////////////////////////////////////////////////////////////////

  Particle A_Out = P_proton;
  Particle B_Out = P_proton;
  Particle C_Out = P_proton;
  Particle D_Out = P_proton;
  Particle O_Out = P_electron;
  Particle H3_Out = P_electron;
  Particle TOF_Out = P_electron;
  Particle NPOL_Out = P_electron;

  if (Reaction && Reaction->getA() != NULL) A_Out = *Reaction->getA();
  if (Reaction && Reaction->getB() != NULL) B_Out = *Reaction->getB();
  if (Reaction && Reaction->getC() != NULL) C_Out = *Reaction->getC();
  if (Reaction && Reaction->getD() != NULL) D_Out = *Reaction->getD();
  if (Reaction && Reaction->getO() != NULL) O_Out = *Reaction->getO();
  if (Reaction && Reaction->getH3() != NULL) H3_Out = *Reaction->getH3();
  if (Reaction && Reaction->getTOF() != NULL) TOF_Out = *Reaction->getTOF();
  if (Reaction && Reaction->getNPOL() != NULL) NPOL_Out = *Reaction->getNPOL();

  if (resultE) {
    A_Out = particle(A_Out.getMass(), oopsAmomentum * (1.0+resultE->dp/100.0),
		     oopsAangle, oopsAoopAngle,
		     resultE->ph/1000.0, -resultE->th/1000.0);
    online.oopsA.particle.id = A_Out.getID();
    online.oopsA.particle.mass = A_Out.getMass();
    online.oopsA.particle.Ekin = A_Out.Ekin();
    online.oopsA.particle.momentum = A_Out.momentum();
    online.oopsA.particle.fourvec[0] = A_Out[0];
    online.oopsA.particle.fourvec[1] = A_Out[1];
    online.oopsA.particle.fourvec[2] = A_Out[2];
    online.oopsA.particle.fourvec[3] = A_Out[3];
    out->packEventData(&online.oopsA.particle.id,4);
    out->packEventData(&online.oopsA.particle.fourvec[0],4);
  }

  if (resultH) {
    B_Out = particle(B_Out.getMass(), oopsBmomentum * (1.0+resultH->dp/100.0),
		     oopsBangle, oopsBoopAngle,
		     resultH->ph/1000.0, -resultH->th/1000.0);
    online.oopsB.particle.id = B_Out.getID();
    online.oopsB.particle.mass = B_Out.getMass();
    online.oopsB.particle.Ekin = B_Out.Ekin();
    online.oopsB.particle.momentum = B_Out.momentum();
    online.oopsB.particle.fourvec[0] = B_Out[0];
    online.oopsB.particle.fourvec[1] = B_Out[1];
    online.oopsB.particle.fourvec[2] = B_Out[2];
    online.oopsB.particle.fourvec[3] = B_Out[3];
    out->packEventData(&online.oopsB.particle.id,4);
    out->packEventData(&online.oopsB.particle.fourvec[0],4);
  }

   // Fill Histograms
		
  online.nr = events;
  out->packEventData(&online.nr);

  Particle electronIn = P_electron;
  electronIn.setMomentum(0, 0, momentum(BeamEnergy, M_ELECTRON));

  static FourVector dummy(0, 0, 0, 0);
  if (!resultE) A_Out=dummy;
  if (!resultH) B_Out=dummy;

  signal(SIGFPE,signalhandler);
  if (!setjmp(env))
    if (evaluate_expressions(A_Out, B_Out, C_Out, D_Out, O_Out, 
			     H3_Out, TOF_Out, NPOL_Out, electronIn))
      ct->putEventData();

  out->clearData();

  return 0; // This means for an X11 working procedure: run again
}

void
targetFillHisto(struct TargetCoord *t, struct TargetCo *tgt)
{
  if (!tgt || !t) return;

  t->dp = tgt->dp;    
  t->th = tgt->th;
  t->y0 = tgt->y0;     
  t->ph = tgt->ph;
  t->len = tgt->len;
  out->packEventData(t, 5);
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
  cerr << "Floating point exception caught! Event " << 
    events << " skipped." << endl << flush;
  longjmp(env,1);
}

int
main(int ac, char *av[])
{
    if (ct->analyseGetOpt(ac, av)) return 0;
    HMMenuCallback = MenuCallback; // Call back for our new menu entry
    if (display)  
	HMLaunchViewWork(ac, av, (void(*)(wxApp*))work);
    else
	while(work(NULL) != 1);  // Event loop 

    return 0;
}
