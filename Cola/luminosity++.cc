//USE LIKE:
//schlimme@a1isos1 ~/d2 $ luminosity++ -f datalinks/run_2014-03-18-14-15-50.xz   |less   



//
// Based on ~cola/isr2013/src/luminosity++.cc
// written by Harald Merkel in 2013
// needs to be linked to AquaReader and MezzoReader
// and runs without run.db
//
// this version written by Patrick Achenbach in 2013 
//
// DISCLAIMER: checked only with Kaos beamtimes
// does not read target from run.db

// use to generate an updating output line like in Lumi++
//#define LUMI_DEBUG
// use to generate an updating output line like in Lumi++
//#define EVENT_DEBUG
// use to output scaler readout
//#define SCALER_DEBUG

/*
 //////////////////////// DATA STRUCTURE ///////////////////////////

 some information on the raw data structure used to calculate runtime,
 deadtime, charge, and luminosity is based on Simon Sirca's
 "abc_2.list" written in 1997

 The runtime is measured from the "run start" command until the "run
 stop" command (only when the general gate is open, which is for 
 instance not the case when the FNP module blocks the DAQ). 
 The realtime is counted only when the flip-flop in the
 µBusy-module is not set, i.e. when the electronics is ready to take
 and convey data.

 The (not-prescaled) "abcn.a.run.runtime" and "abcn.a.run.realtime"
 scalers (analogously defined for SpecB and SpecC and Kaos) are read
 out for each event and are zeroed directly afterwards. The runtime
 should be approximately equal on all three spectrometers; the
 realtime depends on the dead time on each of the spectrometers. For
 example, the difference between runtime and realtime from SpecA is
 the dead time of SpecA.

 The prescaled runtime and prescaled realtime are same as above, but
 the scalers are read out only every 512 events. This does not mean
 the 512th event in the data stream, but the 512th event on an
 individual spectrometer. 

 The prescaled "abcn.a.run.ps_runtime" and "abcn.a.run.ps_realtime"
 scaler values are not zeroed after being read out and make an
 overflow after a certain time.

 To make the elapsed and effective time determination from the
 ps_runtime and ps_realtime variables more accurate, time offsets
 during data acquisition start and stop for the individual
 spectrometer data streams have to be taken into account, especially
 for shorter runs with smaller interrupt rates.

 The "abcn.a.beam.foerster" counter gets its clock from the Förster
 beam current probe. The actual value of the counter is read out at
 each event and zeroed directly afterwards. The counter values should
 be approximately the same in all three spectrometers.

 The value coded into the "abcn.a.beam.ps_foerster" counter is read
 out only every 512 events (as numbered within the individual
 spectrometer event stream), scaled by a factor of 5000 on SpecA (5000
 on SpecB, 4096 on SpecC) and not zeroed after the readout.

 To make the current determination from the ps_foerster variable more
 accurate, time offsets during data acquisition start and stop for the
 individual spectrometer data streams have to be taken into account.

 Luminosity is calculated separately for single and coincidence
 events. These are identified by the synchronisation counter
 "sync_info" which must be the same on all spectrometers in a
 coincidence event.  Bit 0x2000 is for SpecA, bit 0x4000 is for SpecB
 and bit 0x8000 is for SpecC.  The remaining bits give the event
 number. The number of bits set (or unset) within this remaining bit
 pattern specify the event type: singleA, singleB, singleC, doubleAB,
 doubleBC, doubleAC, tripleABC. Kaos is mapped to one of the other
 spectrometers not in use, usually SpecA, i.e. bit 0x2000.

 The "abcn.a.coinc.dead_a" and "kaos.dead_b" (or analogous) counters
 are only valid on individual master spectrometers (SpecA or
 Kaos). Within the Sync module, the corresponding counters with a 100
 MHz clock are incremented until the µBusy-module allows further data
 acquisition. Which of the counters are involved depends on the type
 of the events expected in a given experiment
 (singles/doubles/triples, which spectrometers are used, etc.).

*/

//EXCEPTION HANDLING FLAGS: (option 
//1: compare and correct single-runtimes within double handling (ill-defined).
//2: compare and correct single-charges  within double handling (ill-defined). 
//4:




#include "Aqua/Aqua_abcn.h"
#include "Aqua/AquaUnzip.h"
#include "Aqua/AquaReader.h"
#include "Aqua/MainzReader.h"
#include "Aqua/MezzoReader.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cstring>
#include <math.h>

// Constants
const double         NA            = 6.0221367e23;   // Avogadro constant [1/mol]
const double         CHARGE        = 1.60217733e-19; // Cb
const double         pbarnpersqrcm = 1e36;

// deadtime for each PLU strobe (90 ns)
double DEADTIME_PLU = 90e-9;

// event pattern from sync-info module
const unsigned short SYNC_A        = 0x2000;
const unsigned short SYNC_K        = 0x2000; // KAOS runs as SpekA
const unsigned short SYNC_B        = 0x4000;
const unsigned short SYNC_C        = 0x8000;

// comment: in some experiment types other bits are
// used for sync-info: e.g. in the pseudo-triple experiments
// hyper-2011, hyper-2012 and hyper-2014 SpekA runs as SpekB

// prescaler factor for scintillator hits
const double PS_RATE    = 5000.0;

// prescaler factors for run-/realtimes
const double PS_TIME_AB  = 1000.0; 
const double PS_TIME_C   = 1024.0;

// conversion factors for run-/realtimes
const double TIME       = 1e-3;
const double FOERSTER   = 1e-9;
const double DEAD       = 1e-5;

// prescaler factors for beam current
const double PS_FOERSTER_AB = 5000.0;
const double PS_FOERSTER_C = 4096.0;

// number of passes in RTM3 (MAMI-B) that 
// make up the current in the foerster probe
double MAMI_TURNS = 0; 

using namespace std;

char str[255];
#define formatted(fmt, args...) (sprintf(str, fmt, ## args) > 0 ? str : fmt)




//=======================================================
//  BASE CLASS FOR singles, doubles (, triples): luminosity
//=======================================================

class luminosity {
private: 
  virtual void *Exzellenzcluster() { return NULL; };

protected:
  char name[10];
public:
  double events, totalhits;
  double deadtimefraction, deadtimefraction2; //[%]: real vs run  / dead vs run
  double runtime, runtime_only_us, runtime_only_ps;  //general gate on: [sum unscaled, prescaled, combined]
  double realtime, realtime_only_us, realtime_only_ps;; 
  double charge;
  double *scaling; //trigger prescaler, taken from Mezzo tree
  virtual void handle() = 0;
  virtual char *substitute(const char *fmt) = 0;

  static char *substitute(const char *p, const char *r, const char *s) {
    const char *m = strstr(p, r);
    if (!m) return (char *) p;
    char *res = new char[strlen(p) - strlen(r) + strlen(s) + 1];
    strncpy(res, p, m-p);
    strcpy(&res[m-p], s);
    return strcat(res, m+strlen(r));
  }
  
  char *substitute(const char *p, const char *r, const char *fmt, double d) {
    char str[255],pat[255];
    strcpy(pat, name);
    strcat(pat, "_");
    strcat(pat, r);
    sprintf(str, fmt, d);
    return substitute(p, pat, str);
  }
  void print_summary_header(std::ostream *o, int event_counter) {
    *o << "Experiment  |  Runtime |  Runtime |  Runtime |  Realtime|  Realtime|  Realtime|  Dead | Dead2 |   Charge |  Current | int. Lumi. | Cor. Lumi.|    Events |   Scaling \n"
       << "            |      [s] |   us [s] |   ps [s] |      [s] |   us [s] |   ps [s] |   [%] |   [%] |     [mC] |    [muA] |  [1/pbarn] |  [1/pbarn]|           |           \n"
       << "------------+----------+----------+----------+----------+----------+----------+-------+-------+----------+----------+------------+-----------+-----------+-----------\n"
       << "GLOBAL              -          -          -          -          -          -        -       -          -          -            -            -     " << event_counter << "\n";
  }
  const char *summary(const char *exp, double lumifactor) {
    double dtf = deadtimefraction2 > 0 ? deadtimefraction2 : deadtimefraction;
    return formatted("%-11s%11.2f%11.2f%11.2f%11.2f%11.2f%11.2f%8.2f%8.2f%11.5f%11.5f%13.3f%13.3f%11.0f%11.0f\n", 
		     exp, 
		     runtime, runtime_only_us, runtime_only_ps, 
		     realtime,realtime_only_us,realtime_only_ps, 
		     deadtimefraction, deadtimefraction2, 
		     charge*1e3, charge * 1e6/runtime, 
		     charge*lumifactor, charge * lumifactor * (1 - dtf/100) / *scaling, 
		     events, *scaling);
  }
};


// ========================================================
// luminosity handling for singles experiment
// ========================================================

class singles : public luminosity {

public:
  // readout_us = unscaled counters; readout_ps = prescaled counters
  AquaNodeShort *readout_us[6], *readout_ps[6], *sync_info;

  unsigned short pattern;
  double ps_factor[6], 
    deadtime, deadtime_counter, deadtime_difference;
  
  // counters
  double ps_offset[6], ps_lastvalue[6], ps_value[6], ps_overflow[6], ps_sum[6], 
    us_offset[6], us_sum[6], us_measure[6], ps_measure[6], measure[6];
  int firstevent[6], first_ps_event[6]; //used to see first occurence of counters in data -> setting offsets

  // errors
  int ps_error;

  // loop
  int min, max;
  int exceptionflag; 

  singles(const char *name, AquaTree *atree,
	  unsigned short pattern, const char *strsync, 
	  const char *strrun, 	  const char *strreal, 
	  const char *strpsrun,   const char *strpsreal, 
	  const char *strdead,    const char *strdead2,
	  const char *strhits,    double *Mezzo_scaling,
	  const char *strbeam,    const char *strpsbeam,
	  int exceptionflag_) {
    strncpy(singles::name, name, 10);
    exceptionflag=exceptionflag_;

    // readout scaler
    sync_info      = (AquaNodeShort *) atree->find(strsync);   // sync_info
      
    readout_us[0]  = (AquaNodeShort *) atree->find(strrun);    // run_runtime
    readout_ps[0]  = (AquaNodeShort *) atree->find(strpsrun);  // run_ps_runtime
    readout_us[1]  = (AquaNodeShort *) atree->find(strreal);   // run_realtime
    readout_ps[1]  = (AquaNodeShort *) atree->find(strpsreal); // run_ps_realtime
    readout_us[2]  = (AquaNodeShort *) atree->find(strbeam);   // beam_foerster
    readout_ps[2]  = (AquaNodeShort *) atree->find(strpsbeam); // beam_ps_foerster
    readout_ps[3]  = (AquaNodeShort *) atree->find(strhits);   // scint_ps_hits
    readout_us[4]  = (AquaNodeShort *) atree->find(strdead);   // first_dead
    readout_us[5]  = (AquaNodeShort *) atree->find(strdead2);  // second_dead

    //what about ps_interrupts? -> only spekB ?!

    singles::pattern = pattern;
    scaling = Mezzo_scaling;  //trigger prescaling, taken from Mezzo tree, things will go very wrong if this is changed during run

    // set prescaler factors
    if (SYNC_C == pattern) 
      {
	ps_factor[0] = ps_factor[1] = PS_TIME_C;
	ps_factor[2] = PS_FOERSTER_C;
      }
    else 
      {
	ps_factor[0] = ps_factor[1] = PS_TIME_AB;
	ps_factor[2] = PS_FOERSTER_AB;
    }
    ps_factor[3] = PS_RATE;
    ps_factor[4] = ps_factor[5] = 0;


    // initialize counters
    for (int i = 0; i < 6; i++) 
      {
	ps_offset[i] = ps_lastvalue[i] = ps_value[i] = ps_overflow[i] 
	  = ps_sum[i] = us_offset[i] = us_sum[i] =
	  us_measure[i] = ps_measure[i] = measure[i] = ps_error = 0;
	first_ps_event[i]=firstevent[i]=-1; //bss: it is not sufficient to init these with 0, since at first contact, they will be set to "events", which - at least for ps_... - can be zero!!
      }

    // initialize measurement values
    events = totalhits = deadtimefraction = deadtime_counter 
      = deadtime_difference = runtime = realtime = charge = 0;

  };

  void handle() {

    if (sync_info->ok() && (*sync_info & 0xe000) == pattern) events++; //will be increased only for single event, not for double or triple!
    
    // ========================================================
    // loop over counters: [0] = runtime, [1] = realtime,
    //  [2] = current, [3] = hits, [4] and [5] = deadtime
    // ========================================================

    for (int i = 0; i < 6; i++) 
      {
	//=================================================
	// check unscaled counter (8200 is arbitrary limit)
	//=================================================
	if ( readout_us[i] && readout_us[i]->ok()
	     && (*readout_us[i] & 0x3fff) >= 0 )  // && (*readout_us[i] & 0x3fff) < 8200) bss: not good idea for dead time counter 100kHz!!	     
	  {
	    if (firstevent[i]<0) 
	      { // set unscaled counter offset
		us_offset[i] = *readout_us[i] & 0x3fff;
		firstevent[i] = events;
	      }
	    ps_sum[i] += *readout_us[i] & 0x3fff; // sum unscaled counts between prescaler events -> combine ps values with intermediate us values
	    us_sum[i] += *readout_us[i] & 0x3fff; // sum all unscaled counts
	  }
	// final measurement from unscaled counts:
	us_measure[i] = us_sum[i] - us_offset[i];

	//=================================
	// check prescaled counter
	//=================================
	if ( readout_ps[i] && readout_ps[i]->ok() && events) 
	  {
	    ps_lastvalue[i] = ps_value[i];
	    ps_value[i]     = *readout_ps[i] & 0x3fff;

	    // clear unscaled counts between prescaler events; 
	    //bss include: IF p_value[i]!=ps_lastvalue !!!!! 
	    //Otherwise it was set to zero whenever ps was readout->arbitrary
	    if (ps_value[i] != ps_lastvalue[i])
	      ps_sum[i] = 0;

	    // check for first prescaler event
	    if (first_ps_event[i]<0) 
	      {	// set prescaled counter offset
		ps_offset[i] = ps_value[i]; 
		first_ps_event[i] = events;
	      }

	    //bss: the following is completely unreliable... :
	    // check for double pulses (happens in SpekB)
	    // max increase of 100 is arbitrary limit
	    // and check for positive values
	    if ( ps_value[i] - ps_lastvalue[i] > 200
		 && (ps_value[i] - ps_offset[i] >= 0) )
	      {
		cerr << "ERROR in event " << events 
		     << " prescaled value = " << ps_value[i] 
		     << " following prescaled value = " << ps_lastvalue[i] 
		     << endl; 
		ps_value[i] = ps_lastvalue[i] = ps_offset[i] = 0;
		ps_overflow[i] = 0; //bss
		ps_error = 1; 
	      }

	    // check prescaler overflow
	    if (ps_value[i] < ps_lastvalue[i]) 
	      ps_overflow[i]++;

	  }

	//	if (sync_info->ok() && (*sync_info & 0xe000) == SYNC_B and i==0)
	//	  cout << name << " " << us_sum[i]-us_offset[i] << " " << (ps_value[i]-ps_offset[i]) *ps_factor[i] +ps_sum[i] << " "  << " " << (ps_value[i]-ps_offset[i])*ps_factor[i]<< " "  << ps_sum[i]<< std::endl;
	
	
	// measurement from prescaled counts
	if (!ps_error) 
	  ps_measure[i] = ((ps_value[i] - ps_offset[i] + ps_overflow[i] * 0x4000)* ps_factor[i] + ps_sum[i]);
	
	// chosing larger value from the two measurements
	measure[i] = us_measure[i] > ps_measure[i] ? 
	  us_measure[i] : ps_measure[i];

      } // end of loop

    // ========================================================
    // assignment of measurements
    // ========================================================
    //bss    runtime  = measure[0] * TIME; // in seconds 
    runtime  = ps_measure[0] * TIME; // in seconds 
    runtime_only_us =  us_measure[0] * TIME; 
    runtime_only_ps = (ps_value[0] - ps_offset[0] + ps_overflow[0] * 0x4000)* ps_factor[0] * TIME;


    realtime = measure[1] * TIME; // in seconds
    realtime_only_us =  us_measure[1] * TIME; 
    realtime_only_ps = (ps_value[1] - ps_offset[1] + ps_overflow[1] * 0x4000)* ps_factor[1] * TIME;
    


    // patch for wrong cables: use the larger value for runtime
    if (runtime < realtime) 
      {
	runtime  = measure[1]*TIME; 
	realtime = measure[0]*TIME;
	//std::cerr << "warning: runtime <-> realtime swapped for " << name << std::endl;
      }

    int i_run=0;
    if (runtime < realtime)
      i_run=1;
    int i_real=1-i_run;

    charge   = measure[2] * FOERSTER * (100/MAMI_TURNS); // in nC
    totalhits= measure[3];

    if (runtime)
      {
	// deadtime given by deadtime counters
	deadtime = measure[4] > measure[5] ? // in seconds  || [4] is standard spectrometer, [5] might be remapped Kaos (bss)
	  measure[4] * DEAD : measure[5] * DEAD;
	if (deadtime > 0)
	  deadtime_counter = (deadtime + totalhits * DEADTIME_PLU)/runtime*100;
	if (deadtime_counter >= 100 || deadtime_counter < 0) 
	  deadtime_counter = 0; 

	// deadtime given by run-/realtime difference
	deadtime_difference = (runtime - realtime)/runtime*100;  //bss we miss some PLU deadtime here, no?
	if (deadtime_difference >= 100 || deadtime_difference < 0) 
	  deadtime_difference = 0;

	// deadtimefraction in percent
	//bssout deadtimefraction = deadtime_counter > deadtime_difference ?
	//bssout deadtime_counter : deadtime_difference;  
	deadtimefraction =  deadtime_difference; //real vs run time
	deadtimefraction2 = deadtime_counter; //dead vs run time
      }
    
#ifdef LUMI_DEBUG
    cerr << 
      formatted(" SINGLE 0x%x: run %.3f, %.3f [s], real %.3f, %.3f [s], "
      "dead %.3f, %.3f [s], %.2f [%%], hits %.0f, charge %.3f, %.3f [mC]\n", 
      pattern, 
      us_measure[0]*TIME, ps_measure[0]*TIME, 
      us_measure[1]*TIME, ps_measure[1]*TIME, 
      measure[4]*DEAD,    measure[5]*DEAD, 
      deadtimefraction, totalhits, 
      us_measure[2]*1000*FOERSTER*(100/MAMI_TURNS), 
      ps_measure[2]*1000*FOERSTER*(100/MAMI_TURNS));
#endif

    #ifdef EVENT_DEBUG
    cerr << 
      formatted(" SINGLE 0x%x:  %.0f", 
      pattern,events);
    #endif

  }; // handle
  
  char *substitute(const char *fmt) {
    char *str;
    str = luminosity::substitute(fmt, "EVENTS",  "%.0f", events);
    str = luminosity::substitute(str, "SCALING", "%.0f", *scaling);
    str = luminosity::substitute(str, "HITS",    "%.0f", totalhits);
    str = luminosity::substitute(str, "DEAD",    "%.2f", deadtimefraction);
    return str;
  };
};
  

// ========================================================
// luminosity handling for doubles experiment
// ========================================================

class doubles : public luminosity {
  singles        *spec_one, *spec_two;
  AquaNodeShort  *readout[2];
  unsigned short pattern;
  // counters
  double sum[2], offset[2], strobe, deadtime;
  int firstevent[2];
  int exceptionflag;
public:
  doubles(const char *name, AquaTree *atree,
	  singles *spec_one, singles *spec_two, unsigned short pattern,
	  const char *strdead, const char *strdead2,
	  double *Mezzo_scaling, int exceptionflag_) {

    strncpy(doubles::name,name,10);
    exceptionflag=exceptionflag_;

    readout[0] = (AquaNodeShort *) atree->find(strdead); // deadtime scaler
    readout[1] = (AquaNodeShort *) atree->find(strdead2);// alternative scaler

    doubles::spec_one       = spec_one;
    doubles::spec_two       = spec_two;
    doubles::pattern        = pattern;

    events = deadtimefraction = 0;

    scaling = Mezzo_scaling;  //trigger prescaling, if this is changed during a run, things might go totally wrong
  }

  void handle() {

    // values taken from individual spectrometers
    
    //bss: that is evil because it changes also the charge of the singles, which might not be wanted!!
    //also, if several double routines are used, things get random, depending which double event was last...
    //I leave it in for those who need it
    if ((exceptionflag & 0x2) == 0x2) {
      if (2*fabs(spec_one->charge - spec_two->charge)/(spec_one->charge + spec_two->charge) > 0.1)
	{ if (spec_one->charge == 0) spec_one->charge = spec_two->charge;
	  else if (spec_two->charge == 0) spec_two->charge = spec_one->charge;
	  else if (spec_one->charge > spec_two->charge) spec_one->charge = spec_two->charge;
	  else if (spec_one->charge < spec_two->charge) spec_two->charge = spec_one->charge;
	}
    }
    charge = (spec_one->charge + spec_two->charge)/2;

    //bss: that is evil because it changes also the runtime of the singles, which might not be wanted!!
    //also, if several double routines are used, things get random, depending which double event was last...
    //I leave it in for those who need it
    if ((exceptionflag & 0x1) == 0x1) {
      if ( 2*fabs(spec_one->runtime - spec_two->runtime)
	   /(spec_one->runtime + spec_two->runtime) > 0.1)
	{ if (spec_one->runtime == 0) spec_one->runtime = spec_two->runtime;
	  else if (spec_two->runtime == 0) spec_two->runtime = spec_one->runtime;
	  else if (spec_one->runtime > spec_two->runtime) spec_one->runtime = spec_two->runtime;
	  else if (spec_one->runtime < spec_two->runtime) spec_two->runtime = spec_one->runtime;
	}
    }
    runtime = (spec_one->runtime + spec_two->runtime)/2;
    deadtimefraction = 
      spec_one->deadtimefraction + spec_two->deadtimefraction -
      spec_one->deadtimefraction * spec_two->deadtimefraction/100;

    if (spec_one->sync_info->ok() && spec_two->sync_info->ok() &&
	(*(spec_one->sync_info) & pattern) == pattern &&
	(*(spec_two->sync_info) & pattern) == pattern ) events++;
    else return;

    // ========================================================
    // loop over counters: [0] and [1] = deadtime
    // ========================================================

    for (int i = 0; i < 2; i++) {

      // check unscaled counter
      if (readout[i] && readout[i]->ok())
	{
	  // set unscaled counter offset
	  if (!firstevent[i]) 
	    {
	      offset[i] = *readout[i] & 0x3fff;;
	      firstevent[i] = events;
	    }
	  // sum all unscaled counts
	  sum[i] += *readout[i] & 0x3fff;
	  
          #ifdef SCALER_DEBUG
	  cerr << "i= " << i 
	    << " sum: "       << sum[i]
	    << " offset: "    << offset[i]
	    << endl;
          #endif
	}
    }

    // ========================================================
    // assignment of measurements
    // ========================================================

    // why always the larger value?
    // also counter 0 and counter 1 is the same for AB and AC doubles, but not for BC doubles...
    deadtime = (sum[1]-offset[1]) > (sum[0]-offset[0]) ?
      (sum[1]-offset[1]) * DEAD : (sum[0]-offset[0]) * DEAD;
    
    strobe = (spec_one->totalhits + spec_two->totalhits - events) 
      * DEADTIME_PLU;

    // deadtime given by deadtime counters
    if (deadtime > 0 && runtime > 0 && deadtime < runtime)
      deadtimefraction2 = spec_one->deadtimefraction2 
	+ spec_two->deadtimefraction2
	- (deadtime + strobe)/runtime*100;

    #ifdef LUMI_DEBUG
    cerr << 
      formatted(" COINCS 0x%x: run %.3f [s], dead 1: %.2f, 2: %.2f [%%] ,"
      " 1&2: %.3f, %.3f [s], %.2f [%%], %.2f [%%]\n",
		pattern, runtime, spec_one->deadtimefraction,
		spec_two->deadtimefraction,
		(sum[0]-offset[0])*DEAD/runtime*100,
		(sum[1]-offset[1])*DEAD/runtime*100,
		deadtimefraction,
                deadtimefraction2
                );
    #endif

    #ifdef EVENT_DEBUG
    cerr << 
      formatted(" COINCS 0x%x:  %.0f", 
      pattern,events);
    #endif

    //#ifdef LUMI_DEBUG
    //    cerr << "\n"; 
    //#endif
    //#ifdef EVENT_DEBUG
    //    cerr << "\n"; 
    //#endif



  } // handle

  char *substitute(const char *fmt) {
    char *str;
    str = luminosity::substitute(fmt, "EVENTS", "%.0f", events);
    str = luminosity::substitute(str, "DEAD",   "%.2f", deadtimefraction);
    str = luminosity::substitute(str, "SCALING", "%.0f", *scaling);
    return str;
  };
};


// ========================================================
// Class to provide Mezzo values (from run file)
// and to check for variation during run
// bss: one could do that class much more compact, but 
// explicite seems better than implicite here to me, less 
// error prone in that case
// ========================================================

class MezzoParameterControl {  
private:
  MezzoReader *mreader;
  std::ostream *OStream;
  int check(int event_no, char *name, double *oldval, double *newval, double rel_tolerance) {
    if (event_no>1 && fabs(float(*newval-*oldval)/ float(*oldval)) > rel_tolerance) {
      *OStream << "warning: " << name << " changed from " << *oldval << " to " << *newval << " (event " << event_no << ")" << std::endl;
      *oldval = *newval;   return 1;
    } else {
      *oldval = *newval;   return 0;
    }
  }
public:
  int warnings;
  double *scaling_a,  *old_scaling_a; 
  double *scaling_b,  *old_scaling_b;
  double *scaling_c,  *old_scaling_c;
  double *scaling_k,  *old_scaling_k;
  double *scaling_ab, *old_scaling_ab;
  double *scaling_ac, *old_scaling_ac; 
  double *scaling_bc, *old_scaling_bc;
  //  double *scaling_abc,*old_scaling_abc;
  double *scaling_kb, *old_scaling_kb;
  double *scaling_kc, *old_scaling_kc; 
  double *scaling_kbc,*old_scaling_kbc;
  double *autoturns,  *old_autoturns;
  double *pAmeter,    *old_pAmeter;
  MezzoParameterControl(AquaReader *reader, std::ostream *streamout=&std::cout) {
    warnings=0;
    OStream = streamout;
    //*OStream << "MezzoParameterControl engaged" << std::endl;
    mreader = ((MainzReader *)reader)->getMezzoReader();
    scaling_a  = mreader->init("trigger.scaling.single.a",  -1);   old_scaling_a  = new double;
    scaling_b  = mreader->init("trigger.scaling.single.b",  -1);   old_scaling_b  = new double;
    scaling_c  = mreader->init("trigger.scaling.single.c",  -1);   old_scaling_c  = new double;
    scaling_k  = mreader->init("trigger.scaling.single.k",  -1);   old_scaling_k  = new double;
    scaling_ab = mreader->init("trigger.scaling.double.ab", -1);   old_scaling_ab = new double;
    scaling_ac = mreader->init("trigger.scaling.double.ac", -1);   old_scaling_ac = new double;
    scaling_bc = mreader->init("trigger.scaling.double.bc", -1);   old_scaling_bc = new double;
    scaling_kb = mreader->init("trigger.scaling.double.kb", -1);   old_scaling_kb = new double;
    scaling_kc = mreader->init("trigger.scaling.double.kc", -1);   old_scaling_kc = new double;
    scaling_kbc= mreader->init("trigger.scaling.triple.kbc",-1);   old_scaling_kbc= new double;
    autoturns  = mreader->init("beam.turns.rtm3", -1);             old_autoturns  = new double;
    pAmeter    = mreader->init("beam.picoam.current",-1);          old_pAmeter    = new double;
  }
  ~MezzoParameterControl() {}
  int check_all_changes(int event_no) {
    int chksum=0;
    chksum+=check(event_no, "trigger.scaling.single.a", old_scaling_a, scaling_a, 0);
    chksum+=check(event_no, "trigger.scaling.single.b", old_scaling_b, scaling_b, 0);   
    chksum+=check(event_no, "trigger.scaling.single.c", old_scaling_c, scaling_c, 0);  
    chksum+=check(event_no, "trigger.scaling.single.k", old_scaling_k, scaling_k, 0);  
    chksum+=check(event_no, "trigger.scaling.double.ab", old_scaling_ab, scaling_ab, 0);
    chksum+=check(event_no, "trigger.scaling.double.ac", old_scaling_ac, scaling_ac, 0);
    chksum+=check(event_no, "trigger.scaling.double.bc", old_scaling_bc, scaling_bc, 0);
    chksum+=check(event_no, "trigger.scaling.double.kb", old_scaling_kb, scaling_kb, 0);
    chksum+=check(event_no, "trigger.scaling.double.kc", old_scaling_kc, scaling_kc, 0);
    chksum+=check(event_no, "trigger.scaling.double.kbc", old_scaling_kbc, scaling_kbc, 0);
    chksum+=check(event_no, "beam.turns.rtm3",           old_autoturns,  autoturns, 0);
    chksum+=check(event_no, "beam.picoam.current",       old_pAmeter,    pAmeter, 0);
    warnings+=chksum;
    return chksum;
  }
};


//=======================================================
//  THE CLASS TO PERFORM THE ACTION
//=======================================================

class LuminosityCalculation {
private:
  char *dataFile, *target;
  double length, lumifactor;
  int maxevents, skipevents, scale;
  
  int timeout;
  struct abcn abcn;
  double density, A, anglefactor;

  AquaTree *atree;
  AquaReader *reader; 
  MezzoParameterControl *Mezzo;
  std::ostream *MezzoOutput;
  //FIXME  luminosity *la, *lb, *lc, *lk, *lab, *lbc, *lac, *lkb, *lkc, *lkbc;
  time_t start_time, stop_time;
  int event_counter;
  int exceptionflag;

public:
  char *fmt;
  luminosity *la, *lb, *lc, *lk, *lab, *lbc, *lac, *lkb, *lkc, *lkbc;
  LuminosityCalculation() {
    dataFile = NULL;
    fmt = NULL;
    target = "cryo";
    maxevents = 0;
    skipevents = 0;
    scale = 1;
    MezzoOutput = &std::cout;
    anglefactor = 1;
    start_time=stop_time=-1;
    event_counter = 0;
    exceptionflag = 0;
  };
  void get_opt(int argc, char *argv[]);
  void set_target_parameters();
  void GetTreeAndReaders(); 
  void SetLuminosityInstances();
  void ActionForFirstEvent();
  void ActionForLastEvent();
  void EventLoop(); 
  void PrintMetaData();
  void PrintFormattedOutput();
  void PrintOutput();
};

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//CLASS FUNCTIONS:
void LuminosityCalculation::get_opt(int argc, char *argv[]){
  char opt;
  while ((opt = getopt(argc, argv, "Ff:t:n:s:p:T:l:we:")) != EOF) {
    switch (opt) {
    case 'f': dataFile = optarg;             break;
    case 't': MAMI_TURNS = atof(optarg);     break;
    case 'n': maxevents = atoi(optarg);      break;
    case 's': skipevents = atoi(optarg);     break;
    case 'p': fmt = optarg;                  break;
    case 'F': scale = 100;                   break;
    case 'T': target = optarg;               break;
    case 'l': length = atoi(optarg);         break;
    case 'w': MezzoOutput = &std::cerr;      break;
    case 'e': exceptionflag = atoi(optarg);  break;
    case '?':                    // unknown option
    case ':':                    // missing argument
    case 'h': cout << "\nUSAGE:   " << argv[0] <<" [options]\n\n"
		   << "         Luminosity calculation.\n\n" 
		   <<  "Options:\n\n" 
		   << "-h           this help.\n"
		   << "-f <file>    Input data file name.\n"
		   << "-t <number>  Turns in RTM3 (Default: autoturns(Mezzo), if not available: 90).\n"
		   << "-n <n>       Terminate after <n> events.\n"
		   << "-s <n>       Ignore first <n> events.\n"
		   << "-w           print Mezzo-parameter warnings to std::cerr (default: std::cout).\n"
		   << "-e <n>       Exception handling flag (additive):\n"
		   << "             1: compare and correct single-runtimes within double handling (ill-defined).\n"
		   << "             2: compare and correct single-charges within double handling (ill-defined).\n" 
		   << "-p <pattern> Text pattern for output,\n"
		   << "             e.g. \"a=A_EVENTS dead=A_DEAD scaling=A_SCALING hits=A_HITS q=CHARGE\".\n"
		   << "-F           Use fine scale for Foerster probe.\n"
		   << "             (default: coarse scale)\n"
		   << "-T <target>  target type.\n"
		   << "             e.g. Be-9 (default: cryo)\n"
		   << "-l <length>  target length in micrometer.\n"
		   << "             (default: 119)\n"
	;
      exit(-1);
    }
  }
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void LuminosityCalculation::set_target_parameters(){
  if (strncmp(target,"Be-9",5) == 0)
    {	// Be-9
      density    = 1.85;           // Target density in g/cm^3 
      length = 119;                                                     //FIXME CHECKME BSS
      A          = 9.0122;         // Atomic mass in g/mol
      anglefactor= 1.701;          // Effective length for tilted targets
      // = 1/cos(54°/180*pi)
    }
  else if (strncmp(target,"cryo",5) == 0 || strcmp(target,"H_Cryo.Long"))
    {	// liquid hydrogen
      density    = 0.0688;       // Target density in g/cm^3 
      length     = 4.95*10000;   // Target length in cm 
      A          = 1.00794;      // Atomic mass in g/mol [NIST, 2016]
    }
  else if (strcmp(target,"D_Cryo.Cylinder") == 0)
    { // liquid deuterium	
      density    = 0.169186;     // Target density in g/cm^3 
      length     = 2.0*10000;    // Target length in cm 
      A          = 2.014101764;  // atomic mass; http://pdg.lbl.gov/2014/AtomicNuclearProperties/HTML/liquid_deuterium.html
      std::cerr << "bss, Cryo.Cylinder" << std::endl;
    }
  else {
    std::cerr << "Target " << target << " is undefined" << std::endl;
    exit(-1);
  }
  lumifactor = NA / CHARGE * density * length / 10000 / A / pbarnpersqrcm * anglefactor / scale;
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void LuminosityCalculation::GetTreeAndReaders(){
  AquaIO *aquaIO = (!dataFile || (*dataFile=='-')) ? 
    new AquaUnzip(0) : new AquaUnzip(dataFile);
  atree = new Aqua_abcn(&abcn);
  reader = (AquaReader *) new MainzReader(aquaIO);
  reader->initTree(atree);
  Mezzo = new MezzoParameterControl(reader, MezzoOutput);
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void LuminosityCalculation::SetLuminosityInstances(){
  // there is always one master spectrometer for reading coincidence deadtimes 
  // if SpekA/B/C is master then ".coinc.dead" and ".trigger.scaling" is used
  // if KAOS is master then ".kaos.dead" and ".kaos.scaling." is used
  // two alternatives are automatically analyzed
  
  // singles
  la  = new singles("A", atree, SYNC_A,   ".a.det.sync_info",
		    ".a.run.runtime",    ".a.run.realtime",
		    ".a.run.ps_runtime", ".a.run.ps_realtime",
		    ".a.coinc.dead_a",   ".a.coinc.dead_a",     //bss: these are not two different alternatives as introduced above ...
		    ".a.det.trigger.scint.ps_hits",
		    Mezzo->scaling_a,
		    ".a.beam.foerster", ".a.beam.ps_foerster", exceptionflag);
  
  // some counters are not available in KAOS so that dummies are used for
  // prescaled run-/realtime counters, for the hit counter, and for the 
  // beam current counters
  lk  = new singles("K", atree, SYNC_K,   ".kaos.sync_info",
		    ".kaos.run.runtime", ".kaos.run.realtime",
		    ".a.run.ps_runtime", ".a.run.ps_realtime", 
		    ".a.coinc.dead_a",   ".kaos.dead_k",      
		    ".a.det.trigger.scint.ps_hits",
		    Mezzo->scaling_k,
		    ".a.beam.foerster",  ".a.beam.ps_foerster", exceptionflag);
  
  // unscaled & prescaled runtime counters in B sometimes have problems
  lb  = new singles("B", atree, SYNC_B,   ".b.det.sync_info",
		    ".b.run.runtime",    ".b.run.realtime",
		    ".b.run.ps_runtime", ".b.run.ps_realtime",
		    ".a.coinc.dead_b",   ".kaos.dead_b",      
		    ".b.det.trigger.scint.ps_hits",
		    Mezzo->scaling_b,
		    ".b.beam.foerster", ".b.beam.ps_foerster", exceptionflag);
  
  lc  = new singles("C", atree, SYNC_C,   ".c.det.sync_info",
		    ".c.run.runtime",    ".c.run.realtime",
		    ".c.run.ps_runtime", ".c.run.ps_realtime", 
		    ".a.coinc.dead_b",   ".kaos.dead_b",   
		    ".c.det.trigger.scint.ps_hits",
		    Mezzo->scaling_c,	
	    ".c.beam.foerster",  ".c.beam.ps_foerster", exceptionflag);
  // doubles
  lab = new doubles("AB", atree, (singles *) la, (singles *) lb, 
		    SYNC_A | SYNC_B, ".a.coinc.dead_ab",
		    ".a.coinc.dead_ab", Mezzo->scaling_ab, exceptionflag);
  lbc = new doubles("BC", atree, (singles *) lb, (singles *) lc, 
		    SYNC_B | SYNC_C, ".a.coinc.dead_bc",
		    ".kaos.dead_bc", Mezzo->scaling_bc, exceptionflag);
  lac = new doubles("AC", atree, (singles *) la, (singles *) lc, 
		    SYNC_A | SYNC_C, ".a.coinc.dead_ac",
		    ".a.coinc.dead_ac", Mezzo->scaling_ac, exceptionflag);
  lkb = new doubles("KB", atree, (singles *) lk, (singles *) lb, 
		    SYNC_K | SYNC_B, ".a.coinc.dead_ab",
		    ".kaos.dead_kb", Mezzo->scaling_kb, exceptionflag);
  lkc = new doubles("KC", atree, (singles *) lk, (singles *) lc, 
		    SYNC_K | SYNC_C, ".a.coinc.dead_ac",
		    ".kaos.dead_kc", Mezzo->scaling_kc, exceptionflag);
  
  // pseudo-triples (2 spectrometers trigger as 1) are handled as doubles
  lkbc= new doubles("KBC", atree, (singles *) lk, (singles *) lc, 
		    SYNC_K | SYNC_A | SYNC_C, ".kaos.dead_kb", 
		    ".kaos.dead_kc", Mezzo->scaling_kbc, exceptionflag);
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void LuminosityCalculation::ActionForFirstEvent() {
  start_time = atree->getTimestamp();
  // ========================================================
  // MAMI_TURNS determination (if not set via option)
  // ========================================================
  if (MAMI_TURNS<1){
    MAMI_TURNS = *Mezzo->autoturns; //-- if flag was not set, use the one from mezzo!
  }
  if (MAMI_TURNS<1 ) {
    std::cerr << "warning: MAMI turns not found, using MAMI_TURNS = 90" << std::endl; //bss
      MAMI_TURNS = 90; // if value from mezzo is nonsense, then use default of 90.
  }
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void LuminosityCalculation::ActionForLastEvent() {
  stop_time = atree->getTimestamp();
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void LuminosityCalculation::EventLoop() {
  while (reader && atree
	 && (!maxevents || event_counter < maxevents)
	 && (timeout = reader->nextEvent(atree))>=0) 
    {
      if (timeout) { usleep(10000); continue; }
      else
	{
	  event_counter++; // increment events
	  if (event_counter==1) // FIRST EVENT
	    ActionForFirstEvent(); //like finding out MAMI_TURNS
	  Mezzo->check_all_changes(event_counter);
	  if (!skipevents || event_counter > skipevents)  
	    {
	      la->handle();
	      lb->handle();
	      lc->handle();
	      lk->handle();
	      lab->handle();
	      lac->handle();
	      lbc->handle();
	      lkb->handle();
	      lkc->handle();
	      lkbc->handle();	      
#ifdef LUMI_DEBUG
	      cerr << formatted("\rEvents: %8d",event_counter); 
#endif
#ifdef EVENT_DEBUG
	      cerr << formatted("\r"); 
#endif
	    }}} // end of event loop
  ActionForLastEvent();
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void LuminosityCalculation::PrintMetaData(){
  std::cout << "\nLuminosity calculation for run : " << atree->getRunName() << std::endl;
  std::cout << "----------------------------------------------------------\n";
  std::cout << "start time            : " << start_time << " = " << ctime(&start_time);
  std::cout << "stop time             : " << stop_time  << " = " << ctime(&stop_time) ;
  std::cout << "total length          : "  << (stop_time - start_time) << "s = " << (stop_time - start_time)/60. << "min (world time)" << std::endl;
  std::cout << "MAMI_TURNS            : " << MAMI_TURNS << " (used for Foerster normalization)" << std::endl;
  std::cout << "no. of Mezzo warnings : " << Mezzo->warnings << std::endl;
  std::cout << std::endl;
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
void LuminosityCalculation::PrintFormattedOutput(){
  fmt = la->substitute(fmt);
  fmt = lb->substitute(fmt);
  fmt = lab->substitute(fmt);
  cout << fmt << endl;
}
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
  void LuminosityCalculation::PrintOutput(){
    la->print_summary_header(&cout, event_counter);
    //    cout << "Experiment  |  Runtime |  Realtime|  Dead | Dead2 |   Charge |  Current | int. Lumi. | Cor. Lumi.|    Events |   Scaling \n"
      //	 << "            |      [s] |      [s] |   [%] |   [%] |     [mC] |    [muA] |  [1/pbarn] |  [1/pbarn]|           |           \n"
      //	 << "------------+----------+----------+-------+-------+----------+----------+------------+-----------+-----------+-----------\n"
      //	 << "GLOBAL              -          -        -       -          -          -            -            -     " << event_counter << "\n";
    //singles
    if (la->events > 0) cout << la->summary("Single A   ", lumifactor);
    if (lb->events > 0) cout << lb->summary("Single B   ", lumifactor);
    if (lc->events > 0) cout << lc->summary("Single C   ", lumifactor);
    if (lk->events > 0) cout << lk->summary("Single K   ", lumifactor);
    //doubles
    if (lab->events > 0) cout << lab->summary("Coinc AB   ", lumifactor);
    if (lac->events > 0) cout << lac->summary("Coinc AC   ", lumifactor);
    if (lbc->events > 0) cout << lbc->summary("Coinc BC   ", lumifactor);
    if (lkb->events > 0) cout << lkb->summary("Coinc KB   ", lumifactor);
    if (lkc->events > 0) cout << lkc->summary("Coinc KC   ", lumifactor);
    // pseudo-triples
    if (lkbc->events > 0) cout << lkbc->summary("Coinc KA|KC", lumifactor);
  }
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////




// ========================================================
int main(int argc, char *argv[])
// ========================================================
{ 

  LuminosityCalculation *LC = new LuminosityCalculation();
  LC->get_opt(argc, argv);
  LC->set_target_parameters();
  LC->GetTreeAndReaders();
  LC->SetLuminosityInstances();
  LC->EventLoop();
  if (LC->fmt)
    LC->PrintFormattedOutput();
  else {
    LC->PrintMetaData();
    LC->PrintOutput();
  }

  return 0;
}
 
