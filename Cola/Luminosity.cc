//
// Calculation of Luminosity, runtime, count rates etc...
// $Id: Luminosity.cc 2640 2015-04-01 14:03:00Z aqua $
//

#define _DEFAULT_SOURCE
#include "Luminosity.h"
#include "abcn.h"
#include <cmath>
#include <netinet/in.h>
#include "Aqua/AquaMessage.h"
#include "Aqua/AquaUnzip.h"
#include "Aqua/AquaReader.h"
#include <cstdlib>
#include <fstream>

#undef BAD_PS_SCALER

#ifndef HUGE
#define HUGE 1.797693134862315708e308
#endif
// define physical constants
const double CHARGE     = 1.60217733E-19;
const double N_AVOGADRO = 6.0221367E23;

// define trippOff mode
const int NOTRIPP  = 0;
const int TRIPPOFF = 1;

// define ExpTyp
const int UNKNOWN    = 0;
const int SINGLE_A   = 2;
const int SINGLE_B   = 4;
const int SINGLE_C   = 8;
const int DOUBLE_AB  = 6;
const int DOUBLE_AC  = 10;
const int DOUBLE_BC  = 12;
const int TRIPLE_ABC = 14;

// define prescaler factors for foerster
const double PRESCALER_A = 5000.0;
const double PRESCALER_B = 5000.0;
const double PRESCALER_C = 4096.0;

// define prescaler factors for times
const double PSTIME_A  = 1000.0;
const double PSTIME_B  = 1000.0;
const double PSTIME_C  = 1024.0;

// define prescaler factor for scint hits (rate)
const double PS_RATE   = 5000.0;

// define maximal number of Events without Chamber
//const int MAXLOSSVDC = 50;
const int MAXLOSSVDC = 10000;

int
luminosity::clear()
{
  // init starttime, stoptime
  start_time = 0;
  stop_time  = 0;

  // init TrippOff Control-variables
  VDCtripped = calcmode = NOTRIPP;
  trippoffControl = 0;
  noVDCA1 = noVDCA2 = noVDCB1 = noVDCB2 = noVDCC1 = noVDCC2 = 0;

  // init int class-variables
  firstAps = firstBps = firstCps =
    a_ps_foerster_overflow   = a_ps_foerster_overflow_trip   =
    b_ps_foerster_overflow   = b_ps_foerster_overflow_trip   =
    c_ps_foerster_overflow   = c_ps_foerster_overflow_trip   = 
    a_ps_scint_hits_overflow = a_ps_scint_hits_overflow_trip =
    b_ps_scint_hits_overflow = b_ps_scint_hits_overflow_trip = 
    c_ps_scint_hits_overflow = c_ps_scint_hits_overflow_trip = 0;

  // init long class-variables
  events           = firstevent    = lastevent      = 
    lastevent_trip = tripevent     =
    Aevents        = Bevents       = Cevents        =
    Aevents_end    = Bevents_end   = Cevents_end    =
    Aevents_trip   = Bevents_trip  = Cevents_trip   =
    ABevents       = ACevents      = BCevents       =
    ABevents_end   = ACevents_end  = BCevents_end   = 
    ABevents_trip  = ACevents_trip = BCevents_trip  =
    ABCevents      = ABCevents_end = ABCevents_trip = 0;
  
  // init double class-variables
  a_foerster_sum         = b_foerster_sum       = c_foerster_sum       =
    a_foerster_sum_end   = b_foerster_sum_end   = c_foerster_sum_end   =
    a_foerster_sum_trip  = b_foerster_sum_trip  = c_foerster_sum_trip  =
    a_realtime_offset    = a_runtime_offset     = a_foerster_offset    = 
    a_realtime_end       = a_runtime_end        = a_foerster_end       =
    a_realtime_trip      = a_runtime_trip       = a_foerster_trip      =
    a_realtime           = a_runtime            = a_foerster           =
    b_realtime_offset    = b_runtime_offset     = b_foerster_offset    = 
    b_realtime_end       = b_runtime_end        = b_foerster_end       =
    b_realtime_trip      = b_runtime_trip       = b_foerster_trip      =
    b_realtime           = b_runtime            = b_foerster           =
    c_realtime_offset    = c_runtime_offset     = c_foerster_offset    =
    c_realtime_end       = c_runtime_end        = c_foerster_end       =
    c_realtime_trip      = c_runtime_trip       = c_foerster_trip      =
    c_realtime           = c_runtime            = c_foerster           =
    a_deadtime           = b_deadtime           = c_deadtime           =
    a_deadtime_trip      = b_deadtime_trip      = c_deadtime_trip      =
    ab_deadtime          = ac_deadtime          = bc_deadtime          =
    ab_deadtime_trip     = ac_deadtime_trip     = bc_deadtime_trip     =
    abc_deadtime         = abc_deadtime_trip    =
    a_ps_realtime_offset = a_ps_runtime_offset  = a_ps_foerster_offset =
    a_ps_realtime_last   = a_ps_runtime_last    = a_ps_foerster_last   =
    a_ps_realtime_end    = a_ps_runtime_end     = a_ps_foerster_end    =
    a_ps_realtime_trip   = a_ps_runtime_trip    = a_ps_foerster_trip   =
    a_ps_realtime        = a_ps_runtime         = a_ps_foerster        =
    a_ps_scint_hits      = a_ps_scint_hits_last =
    a_ps_scint_hits_end  = a_ps_scint_hits_trip =
    b_ps_realtime_offset = b_ps_runtime_offset  = b_ps_foerster_offset =
    b_ps_realtime_last   = b_ps_runtime_last    = b_ps_foerster_last   =
    b_ps_realtime_end    = b_ps_runtime_end     = b_ps_foerster_end    =
    b_ps_realtime_trip   = b_ps_runtime_trip    = b_ps_foerster_trip   =
    b_ps_realtime        = b_ps_runtime         = b_ps_foerster        =
    b_ps_scint_hits      = b_ps_scint_hits_last =
    b_ps_scint_hits_end  = b_ps_scint_hits_trip =
    c_ps_realtime_offset = c_ps_runtime_offset  = c_ps_foerster_offset =
    c_ps_realtime_last   = c_ps_runtime_last    = c_ps_foerster_last   =
    c_ps_realtime_end    = c_ps_runtime_end     = c_ps_foerster_end    =
    c_ps_realtime_trip   = c_ps_runtime_trip    = c_ps_foerster_trip   =
    c_ps_realtime        = c_ps_runtime         = c_ps_foerster        = 
    c_ps_scint_hits      = c_ps_scint_hits_last =
    c_ps_scint_hits_end  = c_ps_scint_hits_trip = 0.0;

  // init Silicon variables
  si_cnt =  si_clock  = 0;
  si_cnt_hi = si_clock_hi = 0;

  return 0;
}

luminosity::luminosity(AquaTree *atree, target *Target,
		       reaction *Reaction, double Turns) : statPageBase(atree)
{
  // init # of turns, turns=100 needed for old position of förster probe!
  turns = ( Turns>90 && Turns!=100 ? 90.0 : Turns ); 

  // init MatFactor
  MatFactor = 1.0 / CHARGE       // particles/C
    * Target->getTargetMat()->getDensity()    // g/cm^3
    / Target->getTargetMat()->getMassnumber() // mol/g
    * N_AVOGADRO                 // 1/mol
    * Target->getLength() / 10.0 // cm
    / 1E30;                      // cm^2/mubarn
  
  // init experiment type
  ExpTyp = UNKNOWN; keinA = ABisKA = AisK = BCwithoutA = CisSI = BisSI = 0;
  if ((Reaction->getA() == NULL) && (Reaction->getB() != NULL) &&
      (Reaction->getC() != NULL)) BCwithoutA = 1;  

  if (Reaction->getB() != NULL) ExpTyp += SINGLE_B;
  if (Reaction->getC() != NULL) ExpTyp += SINGLE_C;

  // experiments with Kaos
  if ((Reaction->getKAOS() != NULL) && (Reaction->getA() != NULL) 
      && (Reaction->getC() != NULL) ) {
    ExpTyp += SINGLE_B; ExpTyp += SINGLE_A; ABisKA = 1;

    if (Reaction->getC() != NULL) BAD_RUNTIME_C = 1;
    else BAD_RUNTIME_C = 0;
  // ------------------------------------------------------------------
  }
  else if (Reaction->getKAOS() != NULL) { 
    ExpTyp += SINGLE_A; AisK = 1;

    if (Reaction->getC() != NULL) BAD_RUNTIME_C = 1;
    else BAD_RUNTIME_C = 0;
  // ------------------------------------------------------------------
  }
  else if (Reaction->getA() != NULL) ExpTyp += SINGLE_A; 
  else keinA = 1;

  if (Reaction->getNDET() != NULL) { ExpTyp += SINGLE_C; };
  if (Reaction->getSI() != NULL){
      if (Reaction->getB() != NULL) { ExpTyp += SINGLE_C; CisSI = 1; }; //Silicon runs as C
      if (Reaction->getC() != NULL) { ExpTyp += SINGLE_B; BisSI = 1; }; //Silicon runs as B
  }
  
  clear();
}

int 
luminosity::handle(void *data)
{
  if (!data || !atree) return -1;
  struct abcn *abcn = (struct abcn *) data;
  struct MpsDataA& spekA = abcn->a;
  struct MpsDataB& spekB = abcn->b;
  struct MpsDataC& spekC = abcn->c;
  struct MpsDataKAOS& spekK = abcn->kaos;
#if HASNDET == 1
  struct MpsDataN& ndet = abcn->ndet;
#endif

  long info   = 0;
  long a_info = 0;
  long b_info = 0;
  long c_info = 0;

  if (events == 0) { // first event
    // get start-time of data-taking
    start_time = atree->getTimestamp();
    
    // get runname
    if (atree->getRunName()) {
      if ((strlen(atree->getRunName()) > 12) &&
	  (!strncmp(atree->getRunName(), "run", 3))) {
	strncpy(runname, atree->getRunName(), 23);
	runname[23] = '\0';
      }
      else sprintf(runname, "run_%12s", atree->getRunName());
    }
    //    else if (atree->getInputFilename())
    //      sprintf(runname, "%s", atree->getInputFilename());
  }

  // increment events
  events++;

  // check runinfo
  if (atree->itemOK(&spekA.det.sync_info) && (spekA.det.sync_info & 0x2000))
    a_info = spekA.det.sync_info & 0xe000;

  if (atree->itemOK(&spekB.det.sync_info) && (spekB.det.sync_info & 0x4000))
    b_info = spekB.det.sync_info & 0xe000;

  if (atree->itemOK(&spekC.det.sync_info) && (spekC.det.sync_info & 0x8000))
    c_info = spekC.det.sync_info & 0xe000;

  if (BCwithoutA) { b_info = 0xc000; c_info = 0xc000; }

  if (CisSI){
    if (atree->itemOK(&abcn->si.label) && (abcn->si.label & 0x8000)) 
      c_info = abcn->si.label & 0xe000; 
  }

  // experiments with Kaos
  if (AisK){
    if (atree->itemOK(&abcn->kaos.sync_info) &&
	(abcn->kaos.sync_info & 0x2000)) a_info = abcn->kaos.sync_info&0xe000; 
  }
  if (ABisKA){
    if (atree->itemOK(&abcn->kaos.sync_info) &&
	(abcn->kaos.sync_info & 0x2000)) a_info = abcn->kaos.sync_info&0xe000; 
    if (atree->itemOK(&spekA.det.sync_info) &&
	(spekA.det.sync_info & 0x4000)) b_info = spekA.det.sync_info&0xe000;
  }
  
#if HASNDET == 1
  if (atree->itemOK(&ndet.det.sync_info) && (ndet.det.sync_info & 0x8000))
    c_info = ndet.det.sync_info & 0xe000;
#endif
  
  info = a_info | b_info | c_info;

  // read not scaled realtime, runtime and foerster SpekA
  if (!ABisKA && !AisK) { // A is SpekA and not Kaos
    if (atree->itemOK(&spekA.run.realtime) &&
	atree->itemOK(&spekA.run.runtime)  &&
	atree->itemOK(&spekA.beam.foerster)  ) {
      a_realtime += (spekA.run.realtime  & 0x3fff);
      a_runtime  += (spekA.run.runtime   & 0x3fff);
      a_foerster += (spekA.beam.foerster & 0x3fff);
      a_foerster_sum += (spekA.beam.foerster & 0x3fff);
      
      // !!!!!!!!!!!!!! Trick um leeren Scaler auf B zu umgehen !!!!!!!!!!!!!
      //b_foerster += (spekA.beam.foerster & 0x3fff);
      //b_foerster_sum += (spekA.beam.foerster & 0x3fff);
    }
  } // !K

  // read not scaled realtime, runtime and foerster SpekB
  if (atree->itemOK(&spekB.run.realtime) &&
      atree->itemOK(&spekB.run.runtime)  &&
      atree->itemOK(&spekB.beam.foerster)  ) {
    b_realtime += (spekB.run.realtime  & 0x3fff);
    b_runtime  += (spekB.run.runtime   & 0x3fff);
    // !!!!!!!!!!!!!! Trick um leeren Scaler auf B zu umgehen !!!!!!!!!!!!!
    b_foerster += (spekB.beam.foerster & 0x3fff);
    b_foerster_sum += (spekB.beam.foerster & 0x3fff);
  }


  // read not scaled realtime, runtime and foerster SpekC
  if (atree->itemOK(&spekC.run.realtime) &&
      atree->itemOK(&spekC.run.runtime)  &&
      atree->itemOK(&spekC.beam.foerster)  ) { 
    c_realtime += (spekC.run.realtime  & 0x3fff);
    c_runtime  += (spekC.run.runtime   & 0x3fff);
    c_foerster += (spekC.beam.foerster & 0x3fff);
    c_foerster_sum += (spekC.beam.foerster & 0x3fff);
  }

  // read scalers in Silicon
  if(CisSI){
    if (atree->itemOK(&abcn->si.realtime) &&
	atree->itemOK(&abcn->si.runtime)) { 
      c_realtime += (abcn->si.realtime  & 0x3fff);
      c_runtime  += (abcn->si.runtime   & 0x3fff);
    }
    if (atree->itemOK(&abcn->si.scaler[6])) { 
      uint last_cnt = si_cnt & 0x0fffffff;
      si_cnt = abcn->si.scaler[6];
      if (si_cnt < last_cnt) si_cnt_hi++;
      si_cnt += si_cnt_hi << 28;
    }
    if (atree->itemOK(&abcn->si.microticks)) { 
      uint last_clock = si_clock-si_clock_hi*3600*1e6;
      si_clock = abcn->si.microticks;
      if (si_clock < last_clock) si_clock_hi++;
      si_clock += si_clock_hi*3600*1e6;
    }
  }

  // read scalers in Kaos
  if(AisK){
    if (atree->itemOK(&abcn->kaos.run.realtime) &&
	atree->itemOK(&abcn->kaos.run.runtime)) { 
      a_realtime += (abcn->kaos.run.realtime  & 0x3fff);
      a_runtime  += (abcn->kaos.run.runtime   & 0x3fff);
    }
  }
  if(ABisKA){
    if (atree->itemOK(&abcn->kaos.run.realtime) &&
	atree->itemOK(&abcn->kaos.run.runtime)) { 
      a_realtime += (abcn->kaos.run.realtime  & 0x3fff);
      a_runtime  += (abcn->kaos.run.runtime   & 0x3fff);
    }
    if (ExpTyp & SINGLE_B) {
    a_foerster += (spekB.beam.foerster & 0x3fff);
    a_foerster_sum += (spekB.beam.foerster & 0x3fff);}

    if (ExpTyp & SINGLE_C) {
    a_foerster += (spekC.beam.foerster & 0x3fff);
    a_foerster_sum += (spekC.beam.foerster & 0x3fff);}

    if (atree->itemOK(&spekA.run.realtime) &&
	atree->itemOK(&spekA.run.runtime)  &&
	atree->itemOK(&spekA.beam.foerster)  ) {
      b_realtime += (spekA.run.realtime  & 0x3fff);
      b_runtime  += (spekA.run.runtime   & 0x3fff);
      b_foerster += (spekA.beam.foerster & 0x3fff);
      b_foerster_sum += (spekA.beam.foerster & 0x3fff);
    }
  }


#if HASNDET == 1
  if (atree->itemOK(&ndet.run.realtime) &&
      atree->itemOK(&ndet.run.runtime)) {
    c_realtime += (ndet.run.realtime  & 0x3fff);
    c_runtime  += (ndet.run.runtime   & 0x3fff);
  }
#endif

  // check for first-event if ExpTyp = SINGLE_A
  if ((ExpTyp == SINGLE_A) && !firstevent) {
    firstevent = events;

      a_realtime = 0;
      a_runtime = 0;
      a_foerster = 0;
      a_foerster_sum = 0;
  }

  // check for first-event if ExpTyp = SINGLE_B
  if ((ExpTyp == SINGLE_B) && !firstevent) {
    firstevent = events;

    b_realtime = 0;
    b_runtime = 0;
    b_foerster = 0;
    b_foerster_sum = 0;
  }

  // check for first-event if ExpTyp = SINGLE_C
  if ((ExpTyp == SINGLE_C) && !firstevent) {
    firstevent = events;

    c_realtime = 0;
    c_runtime = 0;
    c_foerster = 0;
    c_foerster_sum = 0;
  }

  // check for first-event if ExpTyp includes Kaos
  if ((AisK || ABisKA) && !firstevent) {
    firstevent = events;

    a_realtime = 0;
    a_runtime = 0;
    a_foerster = 0;
    a_foerster_sum = 0;
    b_realtime = 0;
    b_runtime = 0;
    b_foerster = 0;
    b_foerster_sum = 0;
    c_realtime = 0;
    c_runtime = 0;
    c_foerster = 0;
    c_foerster_sum = 0;
  }

  // check for first-event if A and Kaos are not present
  if (keinA && !firstevent) {
    firstevent = events;
    
    // clear unscaled realtime, runtime and foerster in B and C
    b_realtime = b_runtime = b_foerster = b_foerster_sum = 0.0;
    if(ExpTyp & SINGLE_C)
      c_realtime = c_runtime = c_foerster = c_foerster_sum = 0.0;
  }

  //check for first-event if ExpTyp = DOUBLE_BC and BCwithoutA=1
  if (BCwithoutA == 1) {
    if ((ExpTyp == DOUBLE_BC) && !firstevent) {
      firstevent = events;
      
      b_realtime = 0;
      b_runtime = 0;
      b_foerster = 0;
      b_foerster_sum = 0;
      c_realtime = 0;
      c_runtime = 0;
      c_foerster = 0;
      c_foerster_sum = 0;
    }
  }
  
  ////////////////// SPEKA ////////////////////////

  // check for prescaler values and read them from SpekA
  if (atree->itemOK(&spekA.run.ps_realtime) &&
      atree->itemOK(&spekA.run.ps_runtime)  &&
      atree->itemOK(&spekA.beam.ps_foerster)) { 
    // this happens all 512 events in SpekA
    
    // read realtime, runtime and foerster
    a_ps_realtime_last   = a_ps_realtime;
    a_ps_runtime_last    = a_ps_runtime;
    a_ps_foerster_last   = a_ps_foerster;
    a_ps_scint_hits_last = a_ps_scint_hits;
    a_ps_realtime        = (spekA.run.ps_realtime  & 0x3fff);
    a_ps_runtime         = (spekA.run.ps_runtime   & 0x3fff);
    a_ps_foerster        = (spekA.beam.ps_foerster & 0x3fff);
    if (atree->itemOK(&spekA.det.trigger.scint.ps_hits))
      a_ps_scint_hits      = (spekA.det.trigger.scint.ps_hits & 0x3fff);


    if (!ABisKA && !AisK){ // no Kaos 

    // read deadtimes
    if (atree->itemOK(&spekA.coinc.dead_a)) 
      a_deadtime   += (spekA.coinc.dead_a   / 1E5);

    if (atree->itemOK(&spekA.coinc.dead_b))
      b_deadtime   += (spekA.coinc.dead_b   / 1E5);

    if (atree->itemOK(&spekA.coinc.dead_c))
      c_deadtime   += (spekA.coinc.dead_c   / 1E5);

    if (atree->itemOK(&spekA.coinc.dead_ab))
      ab_deadtime  += (spekA.coinc.dead_ab  / 1E5);

    if (atree->itemOK(&spekA.coinc.dead_ac))
      ac_deadtime  += (spekA.coinc.dead_ac  / 1E5);

    if (atree->itemOK(&spekA.coinc.dead_bc))
      bc_deadtime  += (spekA.coinc.dead_bc  / 1E5);

    if (atree->itemOK(&spekA.coinc.dead_abc))
      abc_deadtime += (spekA.coinc.dead_abc / 1E5);
    }

    // check for first Prescaler event in A
    if (!firstAps) {
      firstAps = 1;

      if (ExpTyp == SINGLE_A) {
	// set offsets for unscaled values (between 'first event'
	// and next prescaler-event in SpekA)
	a_realtime_offset = a_realtime;
	a_runtime_offset  = a_runtime;
	a_foerster_offset = a_foerster;
      } else
	// first event if first prescaler-event in SpekA
	firstevent = events;
      
      // set prescaler offsets
      a_ps_realtime_offset   = a_ps_realtime;
      a_ps_runtime_offset    = a_ps_runtime;
      a_ps_foerster_offset   = a_ps_foerster;
      a_ps_scint_hits_offset = a_ps_scint_hits;

      // clear deadtimes
      a_deadtime    = b_deadtime  = c_deadtime  =
	ab_deadtime = ac_deadtime = bc_deadtime =
	abc_deadtime = 0.0;
      
#ifndef BAD_PS_SCALER
      if (ExpTyp != SINGLE_A)
	// clear unscaled realtime, runtime and foerster in B and C
	a_foerster_sum = b_foerster_sum = c_foerster_sum =
	  b_realtime   = b_runtime      = b_foerster     = 
	  c_realtime   = c_runtime      = c_foerster     = 0.0;
#endif
    }
    
    // check prescaler overflow
    if (a_ps_foerster   < a_ps_foerster_last)   a_ps_foerster_overflow++;
    if (a_ps_scint_hits < a_ps_scint_hits_last) a_ps_scint_hits_overflow++;

    // clear unscaled realtime, runtime and foerster
    a_realtime = a_runtime = a_foerster = 0.0;

    // last event if prescaler-event in SpekA
    lastevent = events;
    
    if (firstevent) {
      // save unscaled B and C values
      b_realtime_end = b_realtime;
      b_runtime_end  = b_runtime;
      b_foerster_end = b_foerster;
      c_realtime_end = c_realtime;
      c_runtime_end  = c_runtime;
      c_foerster_end = c_foerster;

      // save unscaled foerster sum
      a_foerster_sum_end = a_foerster_sum;
      b_foerster_sum_end = b_foerster_sum;
      c_foerster_sum_end = c_foerster_sum;

      // save eventcounters
      Aevents_end    = Aevents;
      Bevents_end    = Bevents;
      Cevents_end    = Cevents;
      ABevents_end   = ABevents;
      ACevents_end   = ACevents;
      BCevents_end   = BCevents;
      ABCevents_end  = ABCevents;

      // save last ps values for A, B and C
      a_ps_realtime_end   = a_ps_realtime;
      a_ps_runtime_end    = a_ps_runtime;
      a_ps_foerster_end   = a_ps_foerster;
      a_ps_scint_hits_end = a_ps_scint_hits;
      b_ps_realtime_end   = b_ps_realtime;
      b_ps_runtime_end    = b_ps_runtime;
      b_ps_foerster_end   = b_ps_foerster;
      b_ps_scint_hits_end = b_ps_scint_hits;
      c_ps_realtime_end   = c_ps_realtime;
      c_ps_runtime_end    = c_ps_runtime;
      c_ps_foerster_end   = c_ps_foerster;
      c_ps_scint_hits_end = c_ps_scint_hits;
    }
  } // if (atree->itemOK(&spekA.run.ps_realtime) &&

  if(AisK || ABisKA){ // read deadtimes from Kaos
    if (atree->itemOK(&spekK.dead_k)) 
      a_deadtime   += (spekK.dead_k   / 1E5);
    
    if (atree->itemOK(&spekK.dead_b))
      b_deadtime   += (spekK.dead_b   / 1E5);
    
    if (atree->itemOK(&spekK.dead_c))
      c_deadtime   += (spekK.dead_c   / 1E5);

    if (atree->itemOK(&spekK.dead_kb))
      ab_deadtime  += (spekK.dead_kb  / 1E5);
    
    if (atree->itemOK(&spekK.dead_kc))
      ac_deadtime  += (spekK.dead_kc  / 1E5);
    
    if (atree->itemOK(&spekK.dead_bc))
      bc_deadtime  += (spekK.dead_bc  / 1E5);
    
    if (atree->itemOK(&spekK.dead_kbc))
      abc_deadtime += (spekK.dead_kbc / 1E5);

    a_runtime_offset  = a_runtime;

    ABevents_end   = ABevents;
    ACevents_end   = ACevents;

  } // AisK

  ////////////////// SPEKB ////////////////////////

  // check for prescaler values and read them from SpekB
  if (atree->itemOK(&spekB.run.ps_realtime) &&
      atree->itemOK(&spekB.run.ps_runtime)  &&
      atree->itemOK(&spekB.beam.ps_foerster)) { 
    // this happens all 512 events in SpekB

    // Using B as leading detector when A is not present
    if (keinA && firstevent) {
      // save unscaled B values
      b_realtime_end = b_realtime;
      b_runtime_end  = b_runtime;
      b_foerster_end = b_foerster;
      
      // save unscaled foerster sum
      b_foerster_sum_end = b_foerster_sum;
      
      if(ExpTyp & SINGLE_C) {
	// save unscaled C values
	c_realtime_end = c_realtime;
	c_runtime_end  = c_runtime;
	c_foerster_end = c_foerster;
	
	// save unscaled foerster sum
	c_foerster_sum_end = c_foerster_sum;
      }

      // save eventcounters
      Aevents_end    = Aevents;
      Bevents_end    = Bevents;
      Cevents_end    = Cevents;
      ABevents_end   = ABevents;
      ACevents_end   = ACevents;
      BCevents_end   = BCevents;
      ABCevents_end  = ABCevents;
      
      // save last ps values of B
      b_ps_realtime_end   = b_ps_realtime;
      b_ps_runtime_end    = b_ps_runtime;
      b_ps_foerster_end   = b_ps_foerster;
      b_ps_scint_hits_end = b_ps_scint_hits;

      if(ExpTyp & SINGLE_C) {
	// save last ps values of C
	c_ps_realtime_end   = c_ps_realtime;
	c_ps_runtime_end    = c_ps_runtime;
	c_ps_foerster_end   = c_ps_foerster;
	c_ps_scint_hits_end = c_ps_scint_hits;
      }

      lastevent = events;
    }

    // read realtime, runtime and foerster
    b_ps_realtime_last   = b_ps_realtime;
    b_ps_runtime_last    = b_ps_runtime;
    b_ps_foerster_last   = b_ps_foerster;
    b_ps_scint_hits_last = b_ps_scint_hits;
    b_ps_realtime        = (spekB.run.ps_realtime  & 0x3fff);
    b_ps_runtime         = (spekB.run.ps_runtime   & 0x3fff);

    if (a_ps_runtime >100 && fabs(b_ps_runtime/a_ps_runtime-2)<0.1) 
      b_ps_runtime = a_ps_runtime; //dirty trick for double pulses in b

    b_ps_foerster        = (spekB.beam.ps_foerster & 0x3fff);
    if (atree->itemOK(&spekB.det.trigger.scint.ps_hits))
      b_ps_scint_hits      = (spekB.det.trigger.scint.ps_hits & 0x3fff);

    // check for first prescaler-event after 'first event'
    if (firstevent && !firstBps) {
      firstBps = 1;

      // set prescaler offsets
      b_ps_realtime_offset   = b_ps_realtime;
      b_ps_runtime_offset    = b_ps_runtime;
      b_ps_foerster_offset   = b_ps_foerster;
      b_ps_scint_hits_offset = b_ps_scint_hits;

      // set offsets for unscaled values (between 'first event'
      // and next prescaler-event in SpekB)
      b_realtime_offset = b_realtime;
      b_runtime_offset  = b_runtime;
      b_foerster_offset = b_foerster;
    }

    // check prescaler overflow
    if (b_ps_foerster   < b_ps_foerster_last)   b_ps_foerster_overflow++;
    if (b_ps_scint_hits < b_ps_scint_hits_last) b_ps_scint_hits_overflow++;

#ifndef BAD_PS_SCALER
    // clear unscaled realtime, runtime and foerster
    b_realtime = b_runtime = b_foerster = 0.0;
#endif
  }
  

  ////////////////// SPEKC ////////////////////////

  // check for prescaler values and read them from SpekC
  if (atree->itemOK(&spekC.run.ps_realtime) &&
      atree->itemOK(&spekC.run.ps_runtime)  &&
      atree->itemOK(&spekC.beam.ps_foerster) ) { 
   // this happens all 512 events in SpekC

    // Using C as leading detector when A and B are not present
    if ( (AisK || ABisKA) && firstevent) {
      
      if((ExpTyp & SINGLE_C)) {
	// save unscaled C values
	c_realtime_end = c_realtime;
	c_runtime_end  = c_runtime;
	c_foerster_end = c_foerster;
	
	// save unscaled foerster sum
	c_foerster_sum_end = c_foerster_sum;
      }

      // save eventcounters
      Aevents_end    = Aevents;
      Bevents_end    = Bevents;
      Cevents_end    = Cevents;
      ABevents_end   = ABevents;
      ACevents_end   = ACevents;
      BCevents_end   = BCevents;
      ABCevents_end  = ABCevents;
      
      if((ExpTyp & SINGLE_C)) {
	// save last ps values of C
	c_ps_realtime_end   = c_ps_realtime;
	c_ps_runtime_end    = c_ps_runtime;
	c_ps_foerster_end   = c_ps_foerster;
	c_ps_scint_hits_end = c_ps_scint_hits;
      }

      lastevent = events;
    }

    // read realtime, runtime and foerster
    c_ps_realtime_last   = c_ps_realtime;
    c_ps_runtime_last    = c_ps_runtime;
    c_ps_foerster_last   = c_ps_foerster;
    c_ps_scint_hits_last = c_ps_scint_hits;
    c_ps_realtime        = (spekC.run.ps_realtime  & 0x3fff);
    c_ps_runtime         = (spekC.run.ps_runtime   & 0x3fff);
    c_ps_foerster        = (spekC.beam.ps_foerster & 0x3fff);
    if (atree->itemOK(&spekC.det.trigger.scint.ps_hits))
	c_ps_scint_hits      = (spekC.det.trigger.scint.ps_hits & 0x3fff);

    // check for first prescaler-event after 'first event'
    if (firstevent && !firstCps) {
      firstCps = 1;

      // set prescaler offsets
      c_ps_realtime_offset   = c_ps_realtime;
      c_ps_runtime_offset    = c_ps_runtime;
      c_ps_foerster_offset   = c_ps_foerster;
      c_ps_scint_hits_offset = c_ps_scint_hits;

      // set offsets for unscaled values (between 'first event'
      // and next prescaler-event in SpekC)
      c_realtime_offset = c_realtime;
      c_runtime_offset  = c_runtime;
      c_foerster_offset = c_foerster;
    }

    // check prescaler overflow
    if (c_ps_foerster   < c_ps_foerster_last)   c_ps_foerster_overflow++;
    if (c_ps_scint_hits < c_ps_scint_hits_last) c_ps_scint_hits_overflow++;

    // clear unscaled realtime, runtime and foerster
    c_realtime = c_runtime = c_foerster = 0.0;

  }

  ////////////////// NDET ////////////////////////

#if HASNDET == 1
  if (atree->itemOK(&ndet.run.ps_realtime) &&
      atree->itemOK(&ndet.run.ps_runtime)) { 
    // this happens all 512 events in ndet

    // read realtime, runtime and foerster
    c_ps_realtime_last = c_ps_realtime;
    c_ps_runtime_last  = c_ps_runtime;
    c_ps_foerster_last = c_ps_foerster;
    c_ps_realtime      = (ndet.run.ps_realtime  & 0x3fff);
    c_ps_runtime       = (ndet.run.ps_runtime   & 0x3fff);

    // check for first prescaler-event after 'first event'
    if (firstevent && !firstCps) {
      firstCps = 1;

      // set prescaler offsets
      c_ps_realtime_offset = c_ps_realtime;
      c_ps_runtime_offset  = c_ps_runtime;
      c_ps_foerster_offset = c_ps_foerster;

      // set offsets for unscaled values (between 'first event'
      // and next prescaler-event in SpekB)
      c_realtime_offset = c_realtime;
      c_runtime_offset  = c_runtime;
      c_foerster_offset = c_foerster;
    }

    // check prescaler overflow
    if (c_ps_foerster < c_ps_foerster_last) c_ps_foerster_overflow++;

    // clear unscaled realtime, runtime and foerster
    c_realtime = c_runtime = c_foerster = 0.0;
  }
#endif

  // increment eventcounter for different event types
  if (firstevent) {
    switch (info) {
    case 0x2000 : // single A event
      Aevents++;
      break;
    case 0x4000 : // single B event
      Bevents++;
      break;
    case 0x8000 : // single C event
      Cevents++;
      break;
    case 0x6000 : // double AB event
      ABevents++;
      if (a_info != b_info) return 1;
      break;
    case 0xa000 : // double AC event
      ACevents++;
      if (a_info != c_info) return 1;
      break;
    case 0xc000 : // double BC event
      BCevents++;
      if (b_info != c_info) return 1;
      break;
    case 0xe000 : // triple ABC event
      ABCevents++;
      if ((a_info != b_info) || (b_info != c_info)) return 1;
      break;
    }
  }

  // save last values if ExpTyp = SINGLE_A
  if (ExpTyp == SINGLE_A) {
    lastevent = events;

    a_foerster_sum_end  = a_foerster_sum;
    a_realtime_end      = a_realtime;
    a_runtime_end       = a_runtime;
    a_foerster_end      = a_foerster;
    a_ps_realtime_end   = a_ps_realtime;
    a_ps_runtime_end    = a_ps_runtime;
    a_ps_foerster_end   = a_ps_foerster;
    a_ps_scint_hits_end = a_ps_scint_hits;
    
    Aevents_end = Aevents;
  }

  // save last values if ExpTyp = SINGLE_B
  if (ExpTyp == SINGLE_B) {
    lastevent = events;

    b_foerster_sum_end  = b_foerster_sum;
    b_realtime_end      = b_realtime;
    b_runtime_end       = b_runtime;
    b_foerster_end      = b_foerster;
    b_ps_realtime_end   = b_ps_realtime;
    b_ps_runtime_end    = b_ps_runtime;
    b_ps_foerster_end   = b_ps_foerster;
    b_ps_scint_hits_end = b_ps_scint_hits;

    Bevents_end = Bevents;
  }

  // save last values if ExpTyp = SINGLE_C
  if (ExpTyp == SINGLE_C) {
    lastevent = events;

    c_foerster_sum_end  = c_foerster_sum;
    c_realtime_end      = c_realtime;
    c_runtime_end       = c_runtime;
    c_foerster_end      = c_foerster;
    c_ps_realtime_end   = c_ps_realtime;
    c_ps_runtime_end    = c_ps_runtime;
    c_ps_foerster_end   = c_ps_foerster;
    c_ps_scint_hits_end = c_ps_scint_hits;

    Cevents_end = Cevents;
  }

  // save last values if ExpTyp = DOUBLE_BC and BCwithoutA == 1
  if (BCwithoutA == 1) {
    if (ExpTyp == DOUBLE_BC) {
      lastevent = events;
      
      b_foerster_sum_end  = b_foerster_sum;
      b_realtime_end      = b_realtime;
      b_runtime_end       = b_runtime;
      b_foerster_end      = b_foerster;
      b_ps_realtime_end   = b_ps_realtime;
      b_ps_runtime_end    = b_ps_runtime;
      b_ps_foerster_end   = b_ps_foerster;
      b_ps_scint_hits_end = b_ps_scint_hits;
      
      Bevents_end = Bevents;

      c_foerster_sum_end  = c_foerster_sum;
      c_realtime_end      = c_realtime;
      c_runtime_end       = c_runtime;
      c_foerster_end      = c_foerster;
      c_ps_realtime_end   = c_ps_realtime;
      c_ps_runtime_end    = c_ps_runtime;
      c_ps_foerster_end   = c_ps_foerster;
      c_ps_scint_hits_end = c_ps_scint_hits;
      
      Cevents_end = Cevents;
      BCevents_end = BCevents;
    }
  }

  // save last values if ExpTyp includes Kaos
  if (AisK || ABisKA) {
    lastevent = events;

    if (ExpTyp & SINGLE_B) {
      
      b_foerster_sum_end  = b_foerster_sum;
      b_realtime_end      = b_realtime;
      b_runtime_end       = b_runtime;
      b_foerster_end      = b_foerster;
      b_ps_realtime_end   = b_ps_realtime;
      b_ps_runtime_end    = b_ps_runtime;
      b_ps_foerster_end   = b_ps_foerster;
      b_ps_scint_hits_end = b_ps_scint_hits;
      
      Bevents_end = Bevents;
      ABevents_end = ABevents;
    }
    if (ExpTyp & SINGLE_C) {
      c_foerster_sum_end  = c_foerster_sum;
      c_realtime_end      = c_realtime;
      c_runtime_end       = c_runtime;
      c_foerster_end      = c_foerster;
      c_ps_realtime_end   = c_ps_realtime;
      c_ps_runtime_end    = c_ps_runtime;
      c_ps_foerster_end   = c_ps_foerster;
      c_ps_scint_hits_end = c_ps_scint_hits;
      
      Cevents_end = Cevents;
      ACevents_end = ACevents;
    }
  }


  ////////////////// TripOff //////////////////////

  // check for VDC-TrippOff
  if (trippoffControl && (VDCtripped == NOTRIPP)) {
    int wire1found = 0;
    int wire2found = 0;
    int i = 0;

    // check VDCA if A has trigger
    if (info & 0x2000) {
      noVDCA1++; noVDCA2++;
      i = 0;
      while (!wire1found && (i<AX1SIZE))
	wire1found = atree->itemOK(&spekA.det.vdc.x1[i++]);
      i = 0;
      while (!wire1found && (i<AS1SIZE))
	wire1found = atree->itemOK(&spekA.det.vdc.s1[i++]);
      i = 0;
      while (!wire2found && (i<AX2SIZE))
	wire2found = atree->itemOK(&spekA.det.vdc.x2[i++]);
      i = 0;
      while (!wire2found && (i<AS2SIZE))
	wire2found = atree->itemOK(&spekA.det.vdc.s2[i++]);
      if (wire1found) noVDCA1 = 0;
      if (wire2found) noVDCA2 = 0;
      if (noVDCA1 >= MAXLOSSVDC) {
	std::cerr << "TrippOff indicated in VDCA Chamber 1 !" 
		  << " EventNr. " << events
		  << "                        " << std::endl;
	VDCtrippOff();
      }
      if (noVDCA2 >= MAXLOSSVDC) {
	std::cerr << "TrippOff indicated in VDCA Chamber 2 !"
		  << " EventNr. " << events
		  << "                        " << std::endl;
	VDCtrippOff();
      }
    }
    
    wire1found = wire2found = 0;
    // check VDCB if B has trigger
    if (info & 0x4000) {
      noVDCB1++; noVDCB2++;
      i = 0;
      while (!wire1found && (i<BX1SIZE))
	wire1found = atree->itemOK(&spekB.det.vdc.x1[i++]);
      i = 0;
      while (!wire1found && (i<BS1SIZE))
	wire1found = atree->itemOK(&spekB.det.vdc.s1[i++]);
      i = 0;
      while (!wire2found && (i<BX2SIZE))
	wire2found = atree->itemOK(&spekB.det.vdc.x2[i++]);
      i = 0;
      while (!wire2found && (i<BS2SIZE))
	wire2found = atree->itemOK(&spekB.det.vdc.s2[i++]);
      if (wire1found) noVDCB1 = 0;
      if (wire2found) noVDCB2 = 0;
      if (noVDCB1 >= MAXLOSSVDC) {
	std::cerr << "TrippOff indicated in VDCB Chamber 1 !"
		  << " EventNr. " << events
		  << "                        " << std::endl;
	VDCtrippOff();
      }
      if (noVDCB2 >= MAXLOSSVDC) {
	std::cerr << "TrippOff indicated in VDCB Chamber 2 !"
		  << " EventNr. " << events
		  << "                        " << std::endl;
	VDCtrippOff();
      }
    }
    
    wire1found = wire2found = 0;
    // check VDCC if C has trigger
    if (info & 0x8000) {
      noVDCC1++; noVDCC2++;
      i = 0;
      while (!wire1found && (i<CX1SIZE))
	wire1found = atree->itemOK(&spekC.det.vdc.x1[i++]);
      i = 0;
      while (!wire1found && (i<CS1SIZE))
	wire1found = atree->itemOK(&spekC.det.vdc.s1[i++]);
      i = 0;
      while (!wire2found && (i<CX2SIZE))
	wire2found = atree->itemOK(&spekC.det.vdc.x2[i++]);
      i = 0;
      while (!wire2found && (i<CS2SIZE))
	wire2found = atree->itemOK(&spekC.det.vdc.s2[i++]);
      if (wire1found) noVDCC1 = 0;
      if (wire2found) noVDCC2 = 0;
      if (noVDCC1 >= MAXLOSSVDC) {
	std::cerr << "TrippOff indicated in VDCC Chamber 1 !"
		  << " EventNr. " << events
		  << "                        " << std::endl;
	VDCtrippOff();
      }
      if (noVDCC2 >= MAXLOSSVDC) {
	std::cerr << "TrippOff indicated in VDCC Chamber 2 !"
		  << " EventNr. " << events
		  << "                        " << std::endl;
	VDCtrippOff();
      }
    }
  }

  // get time of event (stop-time for last event)
  stop_time = atree->getTimestamp();

  return 0;
}

double
luminosity::getIntegrated()
{
  // return luminosity for current experiment in particles/mubarn
  return (firstevent ? MatFactor * lumi(ExpTyp) : 0);
}

double
luminosity::getDeadtime()
{
  // return deadtime for current experiment in %
  if (ABisKA || AisK) return (firstevent ? dead(SINGLE_A) : 100.0);
  else return (firstevent ? dead(ExpTyp) : 100.0);
}

double 
luminosity::getRunTime()
{
  double runtimeA = ((a_ps_runtime_end - a_ps_runtime_offset) * PSTIME_A
		       + a_runtime_offset + a_runtime_end) / 1000.0;
  double runtimeB = ((b_ps_runtime_end - b_ps_runtime_offset) * PSTIME_B
		     + b_runtime_offset + b_runtime_end) / 1000.0 ;
  double runtimeC = ((c_ps_runtime_end - c_ps_runtime_offset) * PSTIME_C
		       + c_runtime_offset + c_runtime_end) / 1000.0 ;

  double max = ((runtimeA>runtimeB) ? runtimeA : runtimeB);
  return ((runtimeC > max) ? runtimeC : max);
}

int
luminosity::print(std::ostream *out)
{
  if (!out) return -1;

  output(out);

  return 0;
}

int
luminosity::print(const char *filename, char *tripfile)
{
  // output of all statistics

  if (strcmp(filename,"nooutput") == 0)
    return 0;

  if (!firstevent) {
    std::cerr << "Can't calculate Luminosity for this run ("<<runname<<")!\n";
    return -1;
    }

  if (strcmp(filename,"-") == 0)
    output(&std::cout);
  else {
    char *fn = new char[strlen(filename) + strlen(runname)+1];
    sprintf(fn, filename, runname);
    std::ofstream file(fn);
    output(&file);
    file.close();
    delete [] fn;

    if ((VDCtripped == TRIPPOFF) && (tripfile != NULL)) {
      // produce trippOff Statistics
      calcmode = TRIPPOFF;
      char *fn = new char[strlen(tripfile) + strlen(runname)+1];
      sprintf(fn, tripfile, runname);
      std::ofstream file2(fn);
      output(&file2);
      file2.close();
      calcmode = NOTRIPP;
      delete [] fn;
    }
  }

  return 0;
}

void
luminosity::VDCtrippOff(void)
{
  // VDC's tripped already -> nothing more to do
  if (VDCtripped == TRIPPOFF) return;

  // set TrippOff-Flag
  VDCtripped = TRIPPOFF;

  // save event nr of tripp
  tripevent = events;

  // save actual 'end'-Values
  lastevent_trip = lastevent;

  Aevents_trip   = Aevents_end;
  Bevents_trip   = Bevents_end;
  Cevents_trip   = Cevents_end;
  ABevents_trip  = ABevents_end;
  ACevents_trip  = ACevents_end;
  BCevents_trip  = BCevents_end;
  ABCevents_trip = ABCevents_end;

  a_deadtime_trip   = a_deadtime;
  b_deadtime_trip   = b_deadtime;
  c_deadtime_trip   = c_deadtime;
  ab_deadtime_trip  = ab_deadtime;
  ac_deadtime_trip  = ac_deadtime;
  bc_deadtime_trip  = bc_deadtime;
  abc_deadtime_trip = abc_deadtime;

  a_foerster_sum_trip = a_foerster_sum_end;
  b_foerster_sum_trip = b_foerster_sum_end;
  c_foerster_sum_trip = c_foerster_sum_end;

  a_realtime_trip = a_realtime_end;
  a_runtime_trip  = a_runtime_end;
  a_foerster_trip = a_foerster_end;
  b_realtime_trip = b_realtime_end;
  b_runtime_trip  = b_runtime_end;
  b_foerster_trip = b_foerster_end;
  c_realtime_trip = c_realtime_end;
  c_runtime_trip  = c_runtime_end;
  c_foerster_trip = c_foerster_end;

  a_ps_foerster_overflow_trip = a_ps_foerster_overflow;
  b_ps_foerster_overflow_trip = b_ps_foerster_overflow;
  c_ps_foerster_overflow_trip = c_ps_foerster_overflow;

  a_ps_scint_hits_overflow_trip = a_ps_scint_hits_overflow;
  b_ps_scint_hits_overflow_trip = b_ps_scint_hits_overflow;
  c_ps_scint_hits_overflow_trip = c_ps_scint_hits_overflow;

  a_ps_realtime_trip   = a_ps_realtime_end;
  a_ps_runtime_trip    = a_ps_runtime_end;
  a_ps_foerster_trip   = a_ps_foerster_end;
  a_ps_scint_hits_trip = a_ps_scint_hits_end;

  b_ps_realtime_trip   = b_ps_realtime_end;
  b_ps_runtime_trip    = b_ps_runtime_end;
  b_ps_foerster_trip   = b_ps_foerster_end;
  b_ps_scint_hits_trip = b_ps_scint_hits_end;

  c_ps_realtime_trip   = c_ps_realtime_end;
  c_ps_runtime_trip    = c_ps_runtime_end;
  c_ps_foerster_trip   = c_ps_foerster_end;
  c_ps_scint_hits_trip = c_ps_scint_hits_end;

  return;
}

double 
luminosity::foerster(double x)
{
  // if turns=0 (EBeam=180 MeV) return 0
  return ( turns ? (x * 100.0 / turns) : 0);
}

double 
luminosity::lumi(int exptyp)
{
  // luminosity A,B,C in Coulomb
  double resultA1, resultA2, resultA;
  double resultB1, resultB2, resultB;
  double resultC1, resultC2, resultC;

  if (calcmode == TRIPPOFF) {
    // TrippOff indicated
    resultA1 = foerster((a_ps_foerster_trip - a_ps_foerster_offset
			 + (double) a_ps_foerster_overflow_trip * 0x4000)
			* PRESCALER_A + a_foerster_offset
			+ a_foerster_trip) * 1E-9;
    resultA2 = foerster(a_foerster_sum_trip) * 1E-9;
    resultB1 = foerster((b_ps_foerster_trip - b_ps_foerster_offset
			 + (double) b_ps_foerster_overflow_trip * 0x4000)
			* PRESCALER_B + b_foerster_offset
			+ b_foerster_trip) * 1E-9;
    resultB2 = foerster(b_foerster_sum_trip) * 1E-9;
    resultC1 = foerster((c_ps_foerster_trip - c_ps_foerster_offset
			 + (double) c_ps_foerster_overflow_trip * 0x4000)
			* PRESCALER_C + c_foerster_offset 
			+ c_foerster_trip) * 1E-9;
    resultC2 = foerster(c_foerster_sum_trip) * 1E-9;
  } else {
    // No TrippOff indicated
    resultA1 = foerster((a_ps_foerster_end - a_ps_foerster_offset
			 + (double) a_ps_foerster_overflow * 0x4000)
			* PRESCALER_A + a_foerster_offset
			+ a_foerster_end) * 1E-9;
    resultA2 = foerster(a_foerster_sum_end) * 1E-9;
    resultB1 = foerster((b_ps_foerster_end - b_ps_foerster_offset
			 + (double) b_ps_foerster_overflow * 0x4000)
			* PRESCALER_B + b_foerster_offset
			+ b_foerster_end) * 1E-9;
    resultB2 = foerster(b_foerster_sum_end) * 1E-9;
    resultC1 = foerster((c_ps_foerster_end - c_ps_foerster_offset
			 + (double) c_ps_foerster_overflow * 0x4000)
			* PRESCALER_C + c_foerster_offset 
			+ c_foerster_end) * 1E-9;
    resultC2 = foerster(c_foerster_sum_end) * 1E-9;
  }
  resultA = ( resultA1 > resultA2 ? resultA1 : resultA2 );
  resultB = ( resultB1 > resultB2 ? resultB1 : resultB2 );
  resultC = ( resultC1 > resultC2 ? resultC1 : resultC2 );

  // Silicon lacks charge information in scalers. It takes from A.
  if (CisSI && (ExpTyp & SINGLE_C)) resultC = resultA;

  // Kaos lacks charge information in scalers. It takes from B or C.
  if (AisK && (ExpTyp & SINGLE_A) && (ExpTyp & SINGLE_B)) resultA = resultB;
  if (AisK && (ExpTyp & SINGLE_A) && (ExpTyp & SINGLE_C)) resultA = resultC;

  if (ABisKA && (ExpTyp & SINGLE_A) && (ExpTyp & SINGLE_B)) resultA = resultB;
  if (ABisKA && (ExpTyp & SINGLE_A) && (ExpTyp & SINGLE_C)) resultA = resultC;

  if (BCwithoutA) resultB = resultC;

  switch (exptyp) {
  case SINGLE_A  : return resultA;
  case SINGLE_B  : return resultB;
  case SINGLE_C  : return resultC;
  case DOUBLE_AB : return ((resultA + resultB) / 2.0);
  case DOUBLE_AC : return ((resultA + resultC) / 2.0);
  case DOUBLE_BC : return ((resultB + resultC) / 2.0);
  case TRIPLE_ABC: return ((resultA + resultB + resultC) / 3.0);
  case UNKNOWN: // Error, no experiment type set !
  default : 
    std::cerr << "unknown experiment type\n";
    exit(-1);
  }
}

double 
luminosity::dead(int exptyp)
{
  double runtime  = 0.0;
  double deadtime = 0.0;
  double plu_deadtime = 0.0;

  double realtimeA, realtimeB, realtimeC;
  double runtimeA, runtimeB, runtimeC, runtimeC2;

  double totalA=0,totalB=0,totalC=0;

  if (calcmode == TRIPPOFF) {
    // TrippOff indicated
    if ((ExpTyp & SINGLE_A) && a_ps_runtime_trip != a_ps_runtime_offset)
      totalA = (a_ps_scint_hits_trip - a_ps_scint_hits_offset + 
	       (0x4000 * a_ps_scint_hits_overflow_trip)) * PS_RATE;
    if ((ExpTyp & SINGLE_B) && b_ps_runtime_trip != b_ps_runtime_offset)
      totalB = (b_ps_scint_hits_trip - b_ps_scint_hits_offset + 
	       (0x4000 * b_ps_scint_hits_overflow_trip)) * PS_RATE;
    if ((ExpTyp & SINGLE_C) && c_ps_runtime_trip != c_ps_runtime_offset)
      totalC = (c_ps_scint_hits_trip - c_ps_scint_hits_offset + 
	       (0x4000 * c_ps_scint_hits_overflow_trip)) * PS_RATE;
  } else {
    // No TrippOff indicated
    if ((ExpTyp & SINGLE_A) && a_ps_runtime_end != a_ps_runtime_offset)
      totalA = (a_ps_scint_hits_end - a_ps_scint_hits_offset + 
	       ( 0x4000 * a_ps_scint_hits_overflow ) ) * PS_RATE;
    if ((ExpTyp & SINGLE_B) && b_ps_runtime_end != b_ps_runtime_offset)
      totalB = (b_ps_scint_hits_end - b_ps_scint_hits_offset + 
	       ( 0x4000 * b_ps_scint_hits_overflow ) ) * PS_RATE;
    if ((ExpTyp & SINGLE_C) && c_ps_runtime_end != c_ps_runtime_offset)
      totalC = (c_ps_scint_hits_end - c_ps_scint_hits_offset + 
	       ( 0x4000 * c_ps_scint_hits_overflow ) ) * PS_RATE;
  }

  // calculate correct real- and runtimes for A, B and C
  if (calcmode == TRIPPOFF) {
    // ---------------------- Trip Off indicated ------------------

    realtimeA = ((a_ps_realtime_trip - a_ps_realtime_offset) * PSTIME_A
		 + a_realtime_offset + a_realtime_trip) / 1000.0;
    realtimeB = ((b_ps_realtime_trip - b_ps_realtime_offset) * PSTIME_B
		 + b_realtime_offset + b_realtime_trip) / 1000.0;

    realtimeC = (c_ps_realtime_trip - c_ps_realtime_offset) * PSTIME_C 
      / 1000.0;
    // !!!!!!!!!!!!! unscaled realtimeC is not correct !!!!!!!!!!!!!!!!
    //    realtimeC = ((c_ps_realtime_trip-c_ps_realtime_offset) * PSTIME_C
    //		 + c_realtime_offset + c_realtime_trip) / 1000.0;
      
    runtimeA = ((a_ps_runtime_trip - a_ps_runtime_offset) * PSTIME_A
		+ a_runtime_offset + a_runtime_trip) / 1000.0;
    runtimeB = ((b_ps_runtime_trip - b_ps_runtime_offset) * PSTIME_B
		+ b_runtime_offset + b_runtime_trip) / 1000.0;
    runtimeC = ((c_ps_runtime_trip - c_ps_runtime_offset) * PSTIME_C
    		+ c_runtime_offset + c_runtime_trip) / 1000.0;

    // !!!!!!!!!!!!! unscaled realtimeC is not correct !!!!!!!!!!!!!!!!
    runtimeC2 = (c_ps_runtime_trip - c_ps_runtime_offset) * PSTIME_C
      / 1000.0;
  } else {
    // ---------------------- No Trip Off indicated ------------------
    realtimeA = ((a_ps_realtime_end - a_ps_realtime_offset) * PSTIME_A
		 + a_realtime_offset + a_realtime_end) / 1000.0;
    realtimeB = ((b_ps_realtime_end - b_ps_realtime_offset) * PSTIME_B
		 + b_realtime_offset + b_realtime_end) / 1000.0;

    realtimeC = (c_ps_realtime_end - c_ps_realtime_offset) * PSTIME_C
      / 1000.0;
    // !!!!!!!!!!!!! unscaled realtimeC is not correct !!!!!!!!!!!!!!!!
    //    realtimeC = ((c_ps_realtime_end - c_ps_realtime_offset) * PSTIME_C
    //		 + c_realtime_offset + c_realtime_end) / 1000.0;

    runtimeA = ((a_ps_runtime_end - a_ps_runtime_offset) * PSTIME_A
		+ a_runtime_offset + a_runtime_end) / 1000.0;
    runtimeB = ((b_ps_runtime_end - b_ps_runtime_offset) * PSTIME_B
		+ b_runtime_offset + b_runtime_end) / 1000.0;

    runtimeC = ((c_ps_runtime_end - c_ps_runtime_offset) * PSTIME_C
    		+ c_runtime_offset + c_runtime_end) / 1000.0;
    // !!!!!!!!!!!!! unscaled realtimeC is not correct !!!!!!!!!!!!!!!!

    runtimeC2 = (c_ps_runtime_end - c_ps_runtime_offset) * PSTIME_C
          / 1000.0;
  }

  if (runtimeA == 0) runtimeA = HUGE;  // avoid NaN's!!!
  if (runtimeB == 0) runtimeB = HUGE;
  if (runtimeC == 0) runtimeC = HUGE;
  if (runtimeC2 == 0) runtimeC2 = HUGE;

  // ------------------------------------------------------------------
  // hack for problems in analysing runtimeC
  if (BAD_RUNTIME_C) runtimeC = runtimeC2 = c_runtime = runtimeA;
  // ------------------------------------------------------------------

  // calculate deadtimes for different exptyp
  if (calcmode == TRIPPOFF)
    // ---------------------- Trip Off indicated ------------------
    switch (exptyp) {
    case SINGLE_A:
      deadtime = (a_deadtime_trip ? a_deadtime_trip : (runtimeA - realtimeA));
      plu_deadtime = totalA * scalerA.strobedead / scalerA.strobe;
      deadtime += plu_deadtime;
      runtime = runtimeA;
      break;
    case SINGLE_B:
      deadtime = (b_deadtime_trip ? b_deadtime_trip : (runtimeB - realtimeB));
      plu_deadtime = totalB * scalerB.strobedead / scalerB.strobe;
      deadtime += plu_deadtime;
      runtime = runtimeB;
      if (BCwithoutA) {
	deadtime = runtimeC2 - realtimeC;
	runtime = runtimeC2;
      }
      break;
    case SINGLE_C:
      if (c_deadtime_trip) {
	deadtime = c_deadtime_trip;
	plu_deadtime = totalC * scalerC.strobedead / scalerC.strobe;
	deadtime += plu_deadtime;
	runtime = runtimeC;
      } else {
	// !!!!!!!!!!!!! unscaled realtimeC is not correct !!!!!!!!!!!!!!!!
	deadtime = runtimeC2 - realtimeC;
	runtime = runtimeC2;
      }
      break;
    // ---------------------- Trip Off indicated ------------------
    case DOUBLE_AB:
      runtime = ((runtimeA + runtimeB) / 2.0);
      deadtime = a_deadtime_trip + b_deadtime_trip - ab_deadtime_trip;
      plu_deadtime = totalA * scalerA.strobedead / scalerA.strobe 
	+  totalB * scalerB.strobedead / scalerB.strobe 
	- ABevents_trip * scalerB.strobedead / scalerB.strobe;
      deadtime += plu_deadtime;
      break;
    case DOUBLE_AC:
      runtime = ((runtimeA + runtimeC) / 2.0);
      deadtime = a_deadtime_trip + c_deadtime_trip - ac_deadtime_trip;
      plu_deadtime = totalA * scalerA.strobedead / scalerA.strobe 
	+  totalC * scalerC.strobedead / scalerC.strobe 
	- ACevents_trip * scalerC.strobedead / scalerC.strobe;
      deadtime += plu_deadtime;
      break;
    case DOUBLE_BC:
      runtime = ((runtimeB + runtimeC) / 2.0);
      deadtime = b_deadtime_trip + c_deadtime_trip - bc_deadtime_trip;
      plu_deadtime = totalB * scalerB.strobedead / scalerB.strobe 
	+ totalC * scalerC.strobedead / scalerC.strobe 
	- BCevents_trip * scalerC.strobedead / scalerC.strobe;
      deadtime += plu_deadtime;
      if (BCwithoutA) {
	deadtime = runtimeC2 - realtimeC;
	runtime = runtimeC2;
      }
      break;
    // ---------------------- Trip Off indicated ------------------
    case TRIPLE_ABC:
      runtime = ((runtimeA + runtimeB + runtimeC) / 3.0);
      deadtime = a_deadtime_trip + b_deadtime_trip + c_deadtime_trip 
	- ab_deadtime_trip - ac_deadtime_trip - bc_deadtime_trip 
	+ abc_deadtime_trip;
      plu_deadtime = totalA * scalerA.strobedead / scalerA.strobe 
	+ totalB * scalerB.strobedead / scalerB.strobe 
	+ totalC * scalerC.strobedead / scalerC.strobe 
	- ABevents_trip * scalerB.strobedead / scalerB.strobe
	- ACevents_trip * scalerC.strobedead / scalerC.strobe
	- BCevents_trip * scalerC.strobedead / scalerC.strobe
	+ ABCevents_trip * scalerB.strobedead / scalerB.strobe;
      deadtime += plu_deadtime;
      break;
    case UNKNOWN: // Error, no experiment type set !
    default : 
      std::cerr << "unknown experiment type\n";
      exit(-1);
    }
  else
    // ---------------------- No Trip Off indicated ------------------
    switch (exptyp) {
    case SINGLE_A:
      deadtime = (a_deadtime ? a_deadtime : (runtimeA - realtimeA));
      plu_deadtime = totalA * scalerA.strobedead / scalerA.strobe;
      deadtime += plu_deadtime;
      runtime = runtimeA;
      break;
    case SINGLE_B:
      deadtime = (b_deadtime ? b_deadtime : (runtimeB - realtimeB));
      plu_deadtime = totalB * scalerB.strobedead / scalerB.strobe;
      deadtime += plu_deadtime;
      runtime = runtimeB;
      if (BCwithoutA) {
	deadtime = runtimeC2 - realtimeC;
	runtime = runtimeC2;
      }
      break;
    case SINGLE_C:
      if (c_deadtime) {
	deadtime = c_deadtime;
	plu_deadtime = totalC * scalerC.strobedead / scalerC.strobe;
	deadtime += plu_deadtime;
	runtime = runtimeC;
      } else {
	// !!!!!!!!!!!!! unscaled realtimeC is not correct !!!!!!!!!!!!!!!!
	deadtime = runtimeC2 - realtimeC;
	runtime = runtimeC2;
      }
      break;
    // ---------------------- No Trip Off indicated ------------------
    case DOUBLE_AB:
      runtime = ((runtimeA + runtimeB) / 2.0);
      deadtime = a_deadtime + b_deadtime - ab_deadtime;
      plu_deadtime = totalA * scalerA.strobedead / scalerA.strobe 
	+  totalB * scalerB.strobedead / scalerB.strobe 
	- ABevents_end * scalerB.strobedead / scalerB.strobe;
      deadtime += plu_deadtime;
      break;
    case DOUBLE_AC:
      runtime = ((runtimeA + runtimeC) / 2.0);
      deadtime = a_deadtime + c_deadtime - ac_deadtime;
      plu_deadtime = totalA * scalerA.strobedead / scalerA.strobe 
	+  totalC * scalerC.strobedead / scalerC.strobe 
	- ACevents_end * scalerC.strobedead / scalerC.strobe;
      deadtime += plu_deadtime;
      break;
    case DOUBLE_BC:
      runtime = ((runtimeB + runtimeC) / 2.0);
      deadtime = b_deadtime + c_deadtime - bc_deadtime;
      plu_deadtime = totalB * scalerB.strobedead / scalerB.strobe 
	+ totalC * scalerC.strobedead / scalerC.strobe 
	- BCevents_end * scalerC.strobedead / scalerC.strobe;
      deadtime += plu_deadtime;
      if (BCwithoutA) {
	deadtime = runtimeC2 - realtimeC;
	runtime = runtimeC2;
      }
      break;
    // ---------------------- No Trip Off indicated ------------------
    case TRIPLE_ABC:
      runtime = ((runtimeA + runtimeB + runtimeC) / 3.0);
      deadtime = a_deadtime + b_deadtime + c_deadtime 
	- ab_deadtime - ac_deadtime - bc_deadtime 
	+ abc_deadtime;
      plu_deadtime = totalA * scalerA.strobedead / scalerA.strobe 
	+ totalB * scalerB.strobedead / scalerB.strobe 
	+ totalC * scalerC.strobedead / scalerC.strobe 
	- ABevents_end * scalerB.strobedead / scalerB.strobe
	- ACevents_end * scalerC.strobedead / scalerC.strobe
	- BCevents_end * scalerC.strobedead / scalerC.strobe
	+ ABCevents_end * scalerB.strobedead / scalerB.strobe;
      deadtime += plu_deadtime;
      break;
    case UNKNOWN: // Error, no experiment type set !
    default : 
      std::cerr << "unknown experiment type\n";
      exit(-1);
    }

  return (100.0 * deadtime / runtime);
}

void 
luminosity::output(std::ostream *out)
{
  double luminos = 0.0;
  double runtime = 0.0;
  double rawA = 0;
  double rawB = 0;
  double rawC = 0;
  double rateA = 0.0;
  double rateB = 0.0;
  double rateC = 0.0;
  double eventsA = 0;
  double eventsB = 0;
  double eventsC = 0;
  double ratioA = 1;
  double ratioB = 1;
  double ratioC = 1;
  double N_A_corr = 0;
  double N_B_corr = 0;
  double N_C_corr = 0;
  double deltaA = 0;
  double deltaB = 0;
  double deltaC = 0;

  double runtimeA = 0, runtimeB = 0, runtimeC = 0;
  // calculate correct runtimes for A, B and C
  if (calcmode == TRIPPOFF) {
    // TrippOff indicated
    eventsA = Aevents_trip + ABevents_trip + ACevents_trip + ABCevents_trip;
    runtimeA = ((a_ps_runtime_trip - a_ps_runtime_offset) * PSTIME_A
		+ a_runtime_offset + a_runtime_trip) / 1000.0;
    eventsB = Bevents_trip + ABevents_trip + BCevents_trip + ABCevents_trip;
    runtimeB = ((b_ps_runtime_trip - b_ps_runtime_offset) * PSTIME_B
		+ b_runtime_offset + b_runtime_trip) / 1000.0 ;
    eventsC = Cevents_trip + ACevents_trip + BCevents_trip + ABCevents_trip;
    runtimeC = ((c_ps_runtime_trip - c_ps_runtime_offset) * PSTIME_C
		+ c_runtime_offset + c_runtime_trip) / 1000.0 ;
  } else {
    // No TrippOff indicated
    eventsA = Aevents_end + ABevents_end + ACevents_end + ABCevents_end;
    runtimeA = ((a_ps_runtime_end - a_ps_runtime_offset) * PSTIME_A
		+ a_runtime_offset + a_runtime_end) / 1000.0;
    eventsB = Bevents_end + ABevents_end + BCevents_end + ABCevents_end;
    runtimeB = ((b_ps_runtime_end - b_ps_runtime_offset) * PSTIME_B
		+ b_runtime_offset + b_runtime_end) / 1000.0 ;
    eventsC = Cevents_end + ACevents_end + BCevents_end + ABCevents_end;
    runtimeC = ((c_ps_runtime_end - c_ps_runtime_offset) * PSTIME_C
		+ c_runtime_offset + c_runtime_end) / 1000.0 ;
  }


  // ------------------------------------------------------------------
  // hack for problems in analysing runtimeC
  double NEW_RUNTIME_C = 0, NEW_DEADTIME_C = 0;
  if (BAD_RUNTIME_C) {
    NEW_RUNTIME_C = runtimeA; 
    NEW_DEADTIME_C = dead(SINGLE_A);
  };
  // ------------------------------------------------------------------

  // calculate rates for A, B, C
  if (calcmode == TRIPPOFF) {
    // TrippOff indicated
    if ((ExpTyp & SINGLE_A) && a_ps_runtime_trip != a_ps_runtime_offset) {
      rawA = (a_ps_scint_hits_trip - a_ps_scint_hits_offset + 
	      (0x4000 * a_ps_scint_hits_overflow_trip)) * PS_RATE;
      rateA = rawA / ((a_ps_runtime_trip - a_ps_runtime_offset) * PSTIME_A);
    }
    if ((ExpTyp & SINGLE_B) && b_ps_runtime_trip != b_ps_runtime_offset) {
      rawB = (b_ps_scint_hits_trip - b_ps_scint_hits_offset + 
	      (0x4000 * b_ps_scint_hits_overflow_trip)) * PS_RATE;
      rateB = rawB / ((b_ps_runtime_trip - b_ps_runtime_offset) * PSTIME_B);
    }
    if ((ExpTyp & SINGLE_C) && c_ps_runtime_trip != c_ps_runtime_offset) {
      rawC = (c_ps_scint_hits_trip - c_ps_scint_hits_offset + 
	      (0x4000 * c_ps_scint_hits_overflow_trip)) * PS_RATE; 
      rateC = rawC / ((c_ps_runtime_trip - c_ps_runtime_offset) * PSTIME_C);
    }
    if (AisK && (ExpTyp & SINGLE_B))
      {
	rateA=(Aevents_trip*scalerA.singles/(a_runtime-a_deadtime))
	  +(ABevents_trip/(((a_runtime + b_runtime) / 2.0)
			   -a_deadtime-b_deadtime+ab_deadtime));
	rawA=rateA*a_runtime;
      }
    if ( (AisK) && (ExpTyp & SINGLE_C))
      {
	rateA=(Aevents_trip*scalerA.singles/(a_runtime-a_deadtime))
	  +(ACevents_trip/(((a_runtime + c_runtime) / 2.0)
			   -a_deadtime-c_deadtime+ac_deadtime));
	rawA=rateA*a_runtime;
      }
  } else {
    // No TrippOff indicated
    if ((ExpTyp & SINGLE_A) && a_ps_runtime_end != a_ps_runtime_offset) {
      rawA = (a_ps_scint_hits_end - a_ps_scint_hits_offset + 
	      ( 0x4000 * a_ps_scint_hits_overflow ) ) * PS_RATE;
      rateA = rawA / ((a_ps_runtime_end - a_ps_runtime_offset) * PSTIME_A);
    }
    if ((ExpTyp & SINGLE_B) && b_ps_runtime_end != b_ps_runtime_offset) {
      rawB = (b_ps_scint_hits_end - b_ps_scint_hits_offset + 
	      ( 0x4000 * b_ps_scint_hits_overflow ) ) * PS_RATE;
      rateB = rawB / ((b_ps_runtime_end - b_ps_runtime_offset) * PSTIME_B);
    }
    if ((ExpTyp & SINGLE_C) && c_ps_runtime_end != c_ps_runtime_offset) {
      rawC = (c_ps_scint_hits_end - c_ps_scint_hits_offset + 
	      ( 0x4000 * c_ps_scint_hits_overflow ) ) * PS_RATE; 
      rateC = rawC / ((c_ps_runtime_end - c_ps_runtime_offset) * PSTIME_C);
    }
    if (AisK && (ExpTyp & SINGLE_B))
      {
	rateA=(Aevents_end*scalerA.singles/(a_runtime-a_deadtime))
	  +(ABevents_end/(((a_runtime + b_runtime) / 2.0)
			  -a_deadtime-b_deadtime+ab_deadtime));
	rawA=rateA*a_runtime;
      }
    if (AisK && (ExpTyp & SINGLE_C))
      {
	rateA=(Aevents_end*scalerA.singles/(a_runtime-a_deadtime))
	  +(ACevents_end/(((a_runtime + c_runtime) / 2.0)
			  -a_deadtime-c_deadtime+ac_deadtime));
	rawA=rateA*a_runtime;
      }
  }
  
  ratioA = (eventsA>0 ? rawA/eventsA : 1.0);
  ratioB = (eventsB>0 ? rawB/eventsB : 1.0);
  ratioC = (eventsC>0 ? rawC/eventsC : 1.0);

  if (BCwithoutA) runtimeB = runtimeC;

  if (CisSI){
    rawC = (double)si_cnt;
    rateC = si_cnt/(si_clock*1e-3); //kHz
    ratioC = (eventsC>0 ? rawC/eventsC : 1.0);
  }

  // output of header
  *out << "Luminosity calculation for run : " << runname << std::endl;
  *out << "---------------------------------";
  for(unsigned int i=0; i<strlen(runname); i++) *out << '-';
  if (calcmode == TRIPPOFF)
    *out << std::endl
	 << "(VDC's tripped at end of run ! (around event # "
	 << tripevent << "))";
  *out << std::endl << std::endl;
  
  // output of times
  double minuten  = (stop_time - start_time) / 60.0;
  *out << "start time   : " << ctime(&start_time);
  *out << "stop time    : " << ctime(&stop_time);
  char str[128];
  sprintf(str, "total length : %.3f minutes\n", minuten);
  *out << str;

  // output of events
  sprintf(str, "total events : %7ld\n", events);
  *out << str;
  sprintf(str,
	  "start event  : %7ld (first event for luminosity calculation)\n",
	  firstevent);
  *out << str;
  if (calcmode == TRIPPOFF) {
    // TrippOff indicated
    sprintf(str,
	    "stop event   : %7ld (last  event for luminosity calculation)\n",
	    lastevent_trip);
    *out << str;
  } else {
    // No TrippOff indicated
    sprintf(str,
	    "stop event   : %7ld (last  event for luminosity calculation)\n",
	    lastevent);
    *out << str;
  }
  switch (ExpTyp) {
  case SINGLE_A:   if (AisK)  *out << "experiment   : Single Kaos\n\n";
                   else *out << "experiment   : Single A\n\n"; break;
  case SINGLE_B:   *out << "experiment   : Single B\n\n"; break;
  case SINGLE_C:   *out << "experiment   : Single C\n\n"; break;
  case DOUBLE_AB:  if (AisK)  *out << "experiment   : Coinc KB\n\n"; 
                   else *out << "experiment   : Coinc AB\n\n"; break;
  case DOUBLE_BC:  *out << "experiment   : Coinc BC\n\n"; break;
  case DOUBLE_AC:  if (AisK)  *out << "experiment   : Coinc KC\n\n"; 
                   else *out <<"experiment   : Coinc AC\n\n"; break;
  case TRIPLE_ABC: 
    if (ABisKA) *out <<"experiment   : Coinc KA|KC\n\n";
    else if (!CisSI) *out <<"experiment   : Coinc ABC\n\n";
    else *out <<"experiment   : Coinc ABC (C==Si)\n\n"; break;
  default:         *out << "experiment   : Unknown\n\n";
  }

  // output of Foerster info and sync info
  if ( (ExpTyp & SINGLE_B) && (ExpTyp & SINGLE_A) && ABisKA) 
    {
    *out << "Kaos  sync info is mapped to SpekA sync info\n";
    *out << "SpekA sync info is mapped to SpekB sync info\n";
    *out << "deadtimes taken from kaos.dead_* info\n";

    if (ExpTyp & SINGLE_C)
      *out << "Kaos Foerster info is taken from SpekC Foerster info\n";
    
    if (BAD_RUNTIME_C) 
      *out << "SpekC runtime is taken from Kaos runtime\n";

    *out << "\n";
    }
  else if ( (ExpTyp & SINGLE_A) && AisK) 
    {
      *out << "Kaos sync info is mapped to SpekA sync info\n"; 
      
      if (ExpTyp & SINGLE_B)
	*out << "Kaos Foerster info is taken from SpekB Foerster info\n"; 
      
      if (ExpTyp & SINGLE_C)
	*out <<  "Kaos Foerster info is taken from SpekC Foerster info";
      
      *out << "deadtimes taken from kaos.dead_* info\n";
      
      if (BAD_RUNTIME_C) 
	*out << "SpekC runtime is taken from Kaos runtime\n";

      *out << "\n";
    }

  // output of countrates
  if (!AisK && !ABisKA)
    sprintf(str,
	    "Rate A       : %6.2f kHz;  Raw A : %9.0f cts;  Ratio A :%9.3f\n", 
	    rateA, rawA, ratioA);
  if (!AisK && !ABisKA) {
  *out << str;
  sprintf(str,
	  "Rate B       : %6.2f kHz;  Raw B : %9.0f cts;  Ratio B :%9.3f\n", 
	  rateB, rawB, ratioB);
  *out << str;
  sprintf(str,
	  "Rate C       : %6.2f kHz;  Raw C : %9.0f cts;  Ratio C :%9.3f",
	  rateC, rawC, ratioC);
  *out << str << std::endl << std::endl;
  }

  // detector labels
  const char *label[6];
  for (int i=0; i<6; i++) label[i] = new char[6]; 

  if ( AisK ) {
    label[0] = "  Kaos";
    label[1] = "  B   ";
    label[2] = "  KB  ";
    label[3] = "  KC  ";
    label[4] = "  BC  ";
    label[5] = " KBC  ";
  } else if ( ABisKA ) {
    label[0] = "  Kaos";
    label[1] = "  A   ";
    label[2] = "  KA  ";
    label[3] = "  KC  ";
    label[4] = "  AC  ";
    label[5] = " KAC  ";
  } else {
    label[0] = "  A   ";
    label[1] = "  C   ";
    label[2] = "  AB  ";
    label[3] = "  AC  ";
    label[4] = "  BC  ";
    label[5] = " ABC  ";
  }
  // output of prescalers (from run.db)

  if (ABisKA) {scalerB.pre = scalerA.pre; scalerB.singles = scalerA.singles; 
    scalerB.doubles = scalerA.doubles;}
  
  if (AisK || ABisKA) {
    scalerA.pre = 1; scalerA.singles = scalerK.singles; 
    scalerA.doubles = scalerK.doubles;}

  if (AisK && !(ExpTyp & SINGLE_B)) {
      *out << "Readout scaler             " << label[0] 
	   << "       C        " << std::endl;}
  else if (ABisKA) *out << "Readout scaler             " << label[0] 
		   << "       A        "
		   << label[3] << std::endl;
  else 
    *out << "Readout scaler             " << label[0] 
	 << "       B        " 
	 << label[1] << std::endl;

  if ( AisK && !(ExpTyp & SINGLE_B)) {
  sprintf(str, "Prescaler               %6d     %6d\n", 
	  scalerA.pre,     scalerC.pre); *out << str;
  sprintf(str, "Scaler Single           %6d     %6d\n",
	  scalerA.singles, scalerC.singles); *out << str;
  sprintf(str, "Scaler Double           %6d     %6d\n\n",
	  scalerA.doubles, scalerC.doubles); *out << str;}
  else {
  sprintf(str, "Prescaler               %6d     %6d     %6d\n", 
	  scalerA.pre,     scalerB.pre,     scalerC.pre); *out << str;
  sprintf(str, "Scaler Single           %6d     %6d     %6d\n",
	  scalerA.singles, scalerB.singles, scalerC.singles); *out << str;
  sprintf(str, "Scaler Double           %6d     %6d     %6d\n\n",
	  scalerA.doubles, scalerB.doubles, scalerC.doubles); *out << str;
  }
  if (!AisK && !ABisKA)
    {sprintf(str, "Scaler Strobe           %6d     %6d     %6d\n\n",
	  scalerA.strobe,  scalerB.strobe,  scalerC.strobe); *out << str;
    }
  // Comparison of deadtime corrected number and number of raw triggers

  if ((ExpTyp & SINGLE_A) && (runtimeA>0)) {
    runtime = runtimeA;
    if (calcmode == TRIPPOFF) {  // TrippOff indicated
      N_A_corr += scalerA.singles / (1 - dead(SINGLE_A) / 100) 
	* Aevents_trip;
    }
    else {                       // No TrippOff indicated
      N_A_corr += scalerA.singles / (1 - dead(SINGLE_A) / 100)
	* Aevents_end;
    }
  }

  if ((ExpTyp & SINGLE_B) && (runtimeB>0)) {
    runtime = runtimeB;
    if (calcmode == TRIPPOFF) {  // TrippOff indicated
      N_B_corr += scalerB.singles / (1 - dead(SINGLE_B) / 100)
	* Bevents_trip;
    }
    else {                       // No TrippOff indicated
      N_B_corr += scalerB.singles / (1 - dead(SINGLE_B) / 100)
	* Bevents_end;
    }
  }

  if ((ExpTyp & SINGLE_C) && (runtimeC>0)) {
    runtime = runtimeC;
    if (calcmode == TRIPPOFF) {  // TrippOff indicated
      N_C_corr += scalerC.singles / (1 - dead(SINGLE_C) / 100)
	     * Cevents_trip;
    }
    else {                       // No TrippOff indicated
      N_C_corr += scalerC.singles / (1 - dead(SINGLE_C) / 100)
	* Cevents_end; 
    }
  }

  if ((ExpTyp & DOUBLE_AB) == DOUBLE_AB && (runtimeA + runtimeB>0)) {
    runtime = (runtimeA + runtimeB) / 2.0;
    if (calcmode == TRIPPOFF) {  // TrippOff indicated
      N_A_corr += (1 - dead(DOUBLE_AB) / 100) * ABevents_trip;
      N_B_corr += (1 - dead(DOUBLE_AB) / 100) * ABevents_trip;
    }
    else {                       // No TrippOff indicated
      N_A_corr += (1 - dead(DOUBLE_AB) / 100) * ABevents_end;
      N_B_corr += (1 - dead(DOUBLE_AB) / 100) * ABevents_end;
    }
  }

  if ((ExpTyp & DOUBLE_AC) == DOUBLE_AC && (runtimeA + runtimeC>0)) {
    runtime = (runtimeA + runtimeC) / 2.0;
    if (calcmode == TRIPPOFF) {  // TrippOff indicated
      N_A_corr += (1 - dead(DOUBLE_AC) / 100) * ACevents_trip;
      N_C_corr += (1 - dead(DOUBLE_AC) / 100) * ACevents_trip;
    }
    else {                       // No TrippOff indicated
      N_A_corr += (1 - dead(DOUBLE_AC) / 100) * ACevents_end;
      N_C_corr += (1 - dead(DOUBLE_AC) / 100) * ACevents_end;
    }
  }

  if ((ExpTyp & DOUBLE_BC) == DOUBLE_BC && (runtimeB + runtimeC>0)) {
    runtime = (runtimeB + runtimeC) / 2.0;
    if (calcmode == TRIPPOFF) {  // TrippOff indicated
      N_B_corr += (1 - dead(DOUBLE_BC) / 100) * BCevents_trip;
      N_C_corr += (1 - dead(DOUBLE_BC) / 100) * BCevents_trip;
    }
    else {                       // No TrippOff indicated
      N_B_corr += (1 - dead(DOUBLE_BC) / 100) * BCevents_end;
      N_C_corr += (1 - dead(DOUBLE_BC) / 100) * BCevents_end;
    }
  }

  if ((ExpTyp & TRIPLE_ABC) == TRIPLE_ABC && (runtimeA+runtimeB+runtimeC>0)) {
    runtime = (runtimeA + runtimeB + runtimeC) / 3.0;
    if (calcmode == TRIPPOFF) {  // TrippOff indicated
      N_A_corr += (1 - dead(TRIPLE_ABC) / 100) * 
	ABCevents_trip;
      N_B_corr += (1 - dead(TRIPLE_ABC) / 100) * 
	ABCevents_trip;
      N_C_corr += (1 - dead(TRIPLE_ABC) / 100) * 
	ABCevents_trip;
    }
    else {                       // No TrippOff indicated
      N_A_corr += (1 - dead(TRIPLE_ABC) / 100) * 
	ABCevents_end;
      N_B_corr += (1 - dead(TRIPLE_ABC) / 100) * 
	ABCevents_end;
      N_C_corr += (1 - dead(TRIPLE_ABC) / 100) * 
	ABCevents_end;
    }
  }

  deltaA = (rawA>0 ? 100*(N_A_corr-rawA)/rawA : 0);
  deltaB = (rawB>0 ? 100*(N_B_corr-rawB)/rawB : 0);
  deltaC = (rawC>0 ? 100*(N_C_corr-rawC)/rawC : 0);

  if (!ABisKA && !AisK)
    {sprintf(str, "(Ncorr-Nraw)/Nraw    %7.2f %%  %7.2f %%  %7.2f %%\n\n", 
	  deltaA, deltaB, deltaC);
      *out << str;
    }

  // output of eventcounters
  if (!ABisKA && !AisK) {
  *out << "          | " << label[0] << " |   B    | " << label[1] << " | "
       << label[2] << " | " << label[3] << " | " << label[4] << " | "
       << label[5] << "\n"
    "----------+--------+--------+--------+--------+--------+--------+--------"
       << std::endl;
  }
  else {
    *out << "          | " << label[0] << " | " << label[1] << " |   C    | "
       << label[2] << " | " << label[3] << " | " << label[4] << " | "
       << label[5] << "\n"
    "----------+--------+--------+--------+--------+--------+--------+--------"
       << std::endl;
  }
  if (calcmode == TRIPPOFF)    // TrippOff indicated
    sprintf(str, "# events   %6ld   %6ld   %6ld   %6ld   %6ld   %6ld   %6ld",
	    Aevents_trip, Bevents_trip, Cevents_trip, ABevents_trip,
	    ACevents_trip, BCevents_trip, ABCevents_trip);
  else                         // No TrippOff indicated
    sprintf(str, "# events   %6ld   %6ld   %6ld   %6ld   %6ld   %6ld   %6ld",
	    Aevents_end, Bevents_end, Cevents_end, ABevents_end,
	    ACevents_end,  BCevents_end, ABCevents_end);
  *out << str << std::endl << std::endl;

  // output of luminosity, charge, current, runtime, deadtime etc.
  *out << "Experiment  |  Runtime |  Dead |   Charge |"
    "  Current | int. Lumi. | Cor. Lumi.\n"
       << "            |      [s] |   [%] |     [mC] |"
    "    [muA] |  [1/pbarn] |  [1/pbarn]\n"
       << "------------+----------+-------+----------+"
    "----------+------------+-----------\n";

  const char *format = "%-12s %9.2f %7.2f %10.3f %10.3f %12.6g %12.6g\n";

  if ((ExpTyp & SINGLE_A) && (runtimeA>0)) {
    luminos = lumi(SINGLE_A);
    runtime = runtimeA;
    const char *type = new char[9];
    if (AisK || ABisKA) type = "Single K";
    else type = "Single A";
    sprintf(str, format, type,
	    runtime, dead(SINGLE_A), luminos*1E3, luminos/runtime*1E6,
	    luminos*MatFactor/1E6,
	    luminos*MatFactor/1E6*(1-dead(SINGLE_A)/100)/scalerA.singles);
    *out << str;
  }

  if ((ExpTyp & SINGLE_B) && (runtimeB>0)) {
    char *xxxlumi = getenv("XXXLUMI");
    double xxx = (xxxlumi ? strtod(xxxlumi, NULL) : 0.0);
    if (0.0 == xxx) {
      luminos = lumi(SINGLE_B);
      runtime = runtimeB;
    } else {
      runtime=minuten*60;
      luminos=runtime*xxx*1E-6;
      std::cerr << xxxlumi << ' ' << xxx << ' '
		<< runtime << ' ' << luminos << std::endl;
    }
    const char *type = new char[9];
    if (ABisKA) type = "Single A|C";
    else type = "Single B";
    sprintf(str, format, type,
	    runtime, dead(SINGLE_B), luminos*1E3, luminos/runtime*1E6,
	    luminos*MatFactor/1E6,
	    luminos*MatFactor/1E6*(1-dead(SINGLE_B)/100)/scalerB.singles);
    *out << str;
  }

  if ((ExpTyp & SINGLE_C) && (runtimeC>0) && !ABisKA) {
    luminos = lumi(SINGLE_C);
    runtime = runtimeC;
    const char *type = new char[10];
    type = "Single C  ";

    if (BAD_RUNTIME_C) 
      {sprintf(str, format, type,
	       NEW_RUNTIME_C, NEW_DEADTIME_C, luminos*1E3, 
	       luminos/NEW_RUNTIME_C*1E6,
	       luminos*MatFactor/1E6,
	       luminos*MatFactor/1E6*(1-NEW_DEADTIME_C/100)/scalerC.singles);
	*out << str;}
    else 
      {    sprintf(str, format, type,
	    runtime, dead(SINGLE_C), luminos*1E3, luminos/runtime*1E6,
	    luminos*MatFactor/1E6,
	    luminos*MatFactor/1E6*(1-dead(SINGLE_C)/100)/scalerC.singles);
	*out << str;}
  }

  if ((ExpTyp & DOUBLE_AB) == DOUBLE_AB && (runtimeA + runtimeB>0)) {
    luminos = lumi(DOUBLE_AB);
    runtime = ((runtimeA + runtimeB) / 2.0);
    const char *type = new char[9];
    if (AisK) type = "Coinc KB"; 
    else if (ABisKA) type = "Coinc KA|KC";
    else type = "Coinc AB";
    sprintf(str, format, type,
	    runtime, dead(DOUBLE_AB), luminos*1E3, luminos/runtime*1E6,
	    luminos*MatFactor/1E6,
	    luminos*MatFactor/1E6*(1-dead(DOUBLE_AB)/100)/scalerA.doubles);
    *out << str;
  }

  if ((ExpTyp & DOUBLE_AC) == DOUBLE_AC && (runtimeA + runtimeC>0) && !ABisKA) {
    luminos = lumi(DOUBLE_AC);

    runtime = ((runtimeA + runtimeC) / 2.0);
    const char *type = new char[11];
    if (AisK || ABisKA) type = "Coinc KC";
    else type = "Coinc AC   ";

    if (BAD_RUNTIME_C) 
      {sprintf(str, format, type,
	       NEW_RUNTIME_C, NEW_DEADTIME_C, luminos*1E3, 
	       luminos/NEW_RUNTIME_C*1E6,
	       luminos*MatFactor/1E6,
	       luminos*MatFactor/1E6*(1-NEW_DEADTIME_C/100)/scalerK.doubles);
	*out << str;}
    else {
      sprintf(str, format, type,
	      runtime, dead(DOUBLE_AC), luminos*1E3, luminos/runtime*1E6,
	      luminos*MatFactor/1E6,
	      luminos*MatFactor/1E6*(1-dead(DOUBLE_AC)/100)/scalerB.doubles);
      *out << str;}
  }

  if ((ExpTyp & DOUBLE_BC) == DOUBLE_BC && (runtimeB + runtimeC>0) && !ABisKA) {
    luminos = lumi(DOUBLE_BC);
    runtime = ((runtimeB + runtimeC) / 2.0);
    const char *type = new char[10];
    type = "Coinc BC ";
    sprintf(str, format, type,
	    runtime, dead(DOUBLE_BC), luminos*1E3, luminos/runtime*1E6,
	    luminos*MatFactor/1E6,
	    luminos*MatFactor/1E6*(1-dead(DOUBLE_BC)/100)/scalerC.doubles);
    *out << str;
  }

  if ((ExpTyp & TRIPLE_ABC) == TRIPLE_ABC && (runtimeA+runtimeB+runtimeC>0) && !ABisKA) {
    luminos = lumi(TRIPLE_ABC);
    runtime = ((runtimeA + runtimeB + runtimeC) / 3.0);
    const char *type = new char[12];
    type = "Coinc ABC   ";
    if (ABisKA) type = "Coinc KA|KC";
    sprintf(str, format, type,
	    runtime, dead(TRIPLE_ABC), luminos*1E3, luminos/runtime*1E6,
	    luminos*MatFactor/1E6,
	    luminos*MatFactor/1E6*(1-dead(TRIPLE_ABC)/100));
    *out << str;
  }

  return;
}

void 
luminosity::output(char * &StartTime, char * &StopTime, int &Events,
		   double A[],  double B[],  double C[],
		   double AB[], double BC[], double AC[],
		   double ABC[])
{
  double luminos = 0.0;
  double runtime = 0.0;
  double runtimeA, runtimeB, runtimeC;

  // calculate correct runtimes for A, B and C
  if (calcmode == TRIPPOFF) {
    // TrippOff indicated
    runtimeA = ((a_ps_runtime_trip - a_ps_runtime_offset) * PSTIME_A
		+ a_runtime_offset + a_runtime_trip) / 1000.0;
    runtimeB = ((b_ps_runtime_trip - b_ps_runtime_offset) * PSTIME_B
		+ b_runtime_offset + b_runtime_trip) / 1000.0 ;
    runtimeC = ((c_ps_runtime_trip - c_ps_runtime_offset) * PSTIME_C
		+ c_runtime_offset + c_runtime_trip) / 1000.0 ;
  } else {
    // No TrippOff indicated
    runtimeA = ((a_ps_runtime_end - a_ps_runtime_offset) * PSTIME_A
		+ a_runtime_offset + a_runtime_end) / 1000.0;
    runtimeB = ((b_ps_runtime_end - b_ps_runtime_offset) * PSTIME_B
		+ b_runtime_offset + b_runtime_end) / 1000.0 ;
    runtimeC = ((c_ps_runtime_end - c_ps_runtime_offset) * PSTIME_C
		+ c_runtime_offset + c_runtime_end) / 1000.0 ;
  }

  for(int i=0;i<8;i++) A[i]=B[i]=C[i]=AB[i]=AC[i]=BC[i]=ABC[i]=0;
  StartTime= strcpy(new char[strlen(ctime(&start_time))+1],ctime(&start_time));
  StopTime = strcpy(new char[strlen(ctime(&stop_time ))+1],ctime(&stop_time ));
  Events   = events;
  A[1]=B[1]=C[1]=AB[1]=AC[1]=BC[1]=ABC[1]= firstevent;
  A[2]=B[2]=C[2]=AB[2]=AC[2]=BC[2]=ABC[2]= 
      (calcmode == TRIPPOFF ? lastevent_trip: lastevent);

  if (calcmode == TRIPPOFF)  {  // TrippOff indicated
    A[0]=Aevents_trip;  B[0]=Bevents_trip;  C[0]=Cevents_trip;
    AB[0]=ABevents_trip;AC[0]=ACevents_trip;BC[0]=BCevents_trip;
    ABC[0]=ABCevents_trip;
  } else {
    A[0]=Aevents_end;  B[0]=Bevents_end;  C[0]=Cevents_end;
    AB[0]=ABevents_end;AC[0]=ACevents_end;BC[0]=BCevents_end;
    ABC[0]=ABCevents_end;
  }
  
  if ((ExpTyp & SINGLE_A) && (runtimeA>0)) {
    luminos = lumi(SINGLE_A);
    A[3] = runtime = runtimeA;
    A[4] = dead(SINGLE_A);
    A[5] = luminos*1E3;
    A[6] = luminos/runtime*1E6;
    A[7] = luminos*MatFactor/1E6*(1-dead(SINGLE_A)/100)/scalerA.singles;
  }

  if ((ExpTyp & SINGLE_B) && (runtimeB>0)) {
    luminos = lumi(SINGLE_B);
    B[3] = runtime = runtimeB;
    B[4] = dead(SINGLE_B);
    B[5] = luminos*1E3;
    B[6] = luminos/runtime*1E6;
    B[7] = luminos*MatFactor/1E6*(1-dead(SINGLE_B)/100)/scalerB.singles;
  }
  if ((ExpTyp & SINGLE_C) && (runtimeC>0)) {
    luminos = lumi(SINGLE_C);
    C[3] = runtime = runtimeC;
    C[4] = dead(SINGLE_C);
    C[5] = luminos*1E3;
    C[6] = luminos/runtime*1E6;
    C[7] = luminos*MatFactor/1E6*(1-dead(SINGLE_C)/100)/scalerC.singles;
  }
  
  if ((ExpTyp & DOUBLE_AB) == DOUBLE_AB && (runtimeA + runtimeB>0)) {
    luminos = lumi(DOUBLE_AB);
    AB[3] = runtime = (runtimeA + runtimeB) / 2.0;
    AB[4] = dead(DOUBLE_AB); AB[5] =  luminos*1E3;
    AB[6] = luminos/runtime*1E6;
    AB[7] = luminos*MatFactor/1E6*(1-dead(DOUBLE_AB)/100);
  }
  
  if ((ExpTyp & DOUBLE_AC) == DOUBLE_AC && (runtimeA + runtimeC>0)) {
    luminos = lumi(DOUBLE_AC);
    AC[3] = runtime = (runtimeA + runtimeC) / 2.0;
    AC[4] = dead(DOUBLE_AC);
    AC[5] = luminos*1E3;
    AC[6] = luminos/runtime*1E6;
    AC[7] = luminos*MatFactor/1E6*(1-dead(DOUBLE_AC)/100);
  }
  
  if ((ExpTyp & DOUBLE_BC) == DOUBLE_BC && (runtimeB + runtimeC>0)) {
    luminos = lumi(DOUBLE_BC);
    BC[3] = runtime = (runtimeB + runtimeC) / 2.0;
    BC[4] = dead(DOUBLE_BC);
    BC[5] = luminos*1E3;
    BC[6] = luminos/runtime*1E6;
    BC[7] = luminos*MatFactor/1E6*(1-dead(DOUBLE_BC)/100);
  }
  
  if ((ExpTyp & TRIPLE_ABC) == TRIPLE_ABC && (runtimeA+runtimeB+runtimeC>0)) {
    luminos = lumi(TRIPLE_ABC);
    ABC[3] = runtime = (runtimeA + runtimeB + runtimeC) / 3.0;
    ABC[4] = dead(TRIPLE_ABC);
    ABC[5] = luminos*1E3;
    ABC[6] = luminos/runtime*1E6;
    ABC[7] = luminos*MatFactor/1E6*(1-dead(TRIPLE_ABC)/100);
  }

  return;
}
