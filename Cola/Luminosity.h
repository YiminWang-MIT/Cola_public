//								      -*-c++-*-
// $Id: Luminosity.h 2559 2014-05-21 14:02:13Z distler $
//

#ifndef __LUMINOSITY_H__
#define __LUMINOSITY_H__

#include "Targetlength.h"
#include "StatBase.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

/**
  This class calculates the luminosity and deadtime.
  The Constructor needs the CindyTree-Pointer, the used target, the Reaction
  variable and the number of Turns in MAMI-B for correct initialization.
  With the Reaction variable the type of experiment is determined.

  Every event in the event-loop of the analysis-program the method 'handle'
  has to be executed.

  With 'getIntegrated', 'getRunTime'  and 'getDeadtime' you get the actual
  values of the integrated luminosity, the integrated run time and the
  corresponding deadtime for the experiment type, set with the Reaction
  variable.

  Finaly you can produce an output of the complete statistics with the
  method 'print'. This output is send to the file with the given filename or
  to an opened output stream. If the filename is '-' the output is redirected
  to 'stdout'. If the filename is 'nooutput' there will be no output.

  Note, that the integrated luminosity is NOT corrected for deadtime.
*/

class luminosity : public statPageBase {
private:
  double foerster(double x);
  double lumi(int exptyp);
  double dead(int exptyp);
  void   output(std::ostream * out);

  time_t start_time, stop_time;

  char runname[255];
  struct {int pre; int singles; int doubles; int strobe; double strobedead;} 
    scalerA, scalerB, scalerC, scalerK;

  double turns;
  double MatFactor;

  int ExpTyp;
  int keinA;
  int AisK;
  int ABisKA;
  int BCwithoutA;
  int CisSI;
  int BisSI;

  int BAD_RUNTIME_C;

  int VDCtripped;
  int calcmode;

  int trippoffControl;
  int noVDCA1, noVDCA2, noVDCB1, noVDCB2, noVDCC1, noVDCC2;

  int firstAps, firstBps, firstCps;

  long events, firstevent, lastevent, lastevent_trip, tripevent;

  long Aevents,   Aevents_end,   Aevents_trip;
  long Bevents,   Bevents_end,   Bevents_trip;
  long Cevents,   Cevents_end,   Cevents_trip;
  long ABevents,  ABevents_end,  ABevents_trip;
  long ACevents,  ACevents_end,  ACevents_trip;
  long BCevents,  BCevents_end,  BCevents_trip;
  long ABCevents, ABCevents_end, ABCevents_trip;

  double a_deadtime,   a_deadtime_trip;
  double b_deadtime,   b_deadtime_trip;
  double c_deadtime,   c_deadtime_trip;
  double ab_deadtime,  ab_deadtime_trip;
  double ac_deadtime,  ac_deadtime_trip;
  double bc_deadtime,  bc_deadtime_trip;
  double abc_deadtime, abc_deadtime_trip;

  double a_foerster_sum, a_foerster_sum_end, a_foerster_sum_trip;
  double b_foerster_sum, b_foerster_sum_end, b_foerster_sum_trip;
  double c_foerster_sum, c_foerster_sum_end, c_foerster_sum_trip;

  double a_realtime, a_runtime, a_foerster;
  double b_realtime, b_runtime, b_foerster;
  double c_realtime, c_runtime, c_foerster;

  double a_realtime_offset, a_runtime_offset, a_foerster_offset;
  double b_realtime_offset, b_runtime_offset, b_foerster_offset;
  double c_realtime_offset, c_runtime_offset, c_foerster_offset;

  double a_realtime_end, a_runtime_end, a_foerster_end;
  double b_realtime_end, b_runtime_end, b_foerster_end;
  double c_realtime_end, c_runtime_end, c_foerster_end;

  double a_realtime_trip, a_runtime_trip, a_foerster_trip;
  double b_realtime_trip, b_runtime_trip, b_foerster_trip;
  double c_realtime_trip, c_runtime_trip, c_foerster_trip;

  int a_ps_foerster_overflow, a_ps_scint_hits_overflow;
  int b_ps_foerster_overflow, b_ps_scint_hits_overflow;
  int c_ps_foerster_overflow, c_ps_scint_hits_overflow;

  int a_ps_foerster_overflow_trip, a_ps_scint_hits_overflow_trip;
  int b_ps_foerster_overflow_trip, b_ps_scint_hits_overflow_trip;
  int c_ps_foerster_overflow_trip, c_ps_scint_hits_overflow_trip;

  double a_ps_realtime, a_ps_runtime, a_ps_foerster, a_ps_scint_hits;
  double b_ps_realtime, b_ps_runtime, b_ps_foerster, b_ps_scint_hits;
  double c_ps_realtime, c_ps_runtime, c_ps_foerster, c_ps_scint_hits;

  double a_ps_realtime_last,   b_ps_realtime_last,   c_ps_realtime_last;
  double a_ps_runtime_last,    b_ps_runtime_last,    c_ps_runtime_last;
  double a_ps_foerster_last,   b_ps_foerster_last,   c_ps_foerster_last;
  double a_ps_scint_hits_last, b_ps_scint_hits_last, c_ps_scint_hits_last;

  double a_ps_realtime_offset,  b_ps_realtime_offset,  c_ps_realtime_offset;
  double a_ps_runtime_offset,   b_ps_runtime_offset,   c_ps_runtime_offset;
  double a_ps_foerster_offset,  b_ps_foerster_offset,  c_ps_foerster_offset;
  double a_ps_scint_hits_offset,b_ps_scint_hits_offset,c_ps_scint_hits_offset;

  double a_ps_realtime_end,   b_ps_realtime_end,   c_ps_realtime_end;
  double a_ps_runtime_end,    b_ps_runtime_end,    c_ps_runtime_end;
  double a_ps_foerster_end,   b_ps_foerster_end,   c_ps_foerster_end;
  double a_ps_scint_hits_end, b_ps_scint_hits_end, c_ps_scint_hits_end;

  double a_ps_realtime_trip,   b_ps_realtime_trip,   c_ps_realtime_trip;
  double a_ps_runtime_trip,    b_ps_runtime_trip,    c_ps_runtime_trip;
  double a_ps_foerster_trip,   b_ps_foerster_trip,   c_ps_foerster_trip;
  double a_ps_scint_hits_trip, b_ps_scint_hits_trip, c_ps_scint_hits_trip;

  uint si_cnt, si_clock;
  short si_cnt_hi, si_clock_hi;

public:
  /// Constructor
  luminosity(AquaTree *atree, target *Target,
	     reaction *Reaction, double Turns);

  /// Main method of class. Has to be executed every event.
  virtual int handle(void * data);

  /// Returns integrated luminosity for current experiment in particles/mubarn
  virtual double getIntegrated(void);

  /// Returns integrated run time for current experiment in seconds
  virtual double getRunTime(void);

  /// Returns Deadtime for current experiment in \TEX{\%}
  virtual double getDeadtime(void);

  /// Output of complete statistics to a stream
  virtual int print(std::ostream * out);

  void output(char * &StartTime, char * &StopTime, int &Events,
	   double A[],  double B[],  double C[],
	   double AB[], double BC[], double AC[],
	   double ABC[]);
  /// Output of complete statistics to given 'filename'
  virtual int print(const char *filename, char *tripfile = NULL);

  /// clear statistic page
  virtual int clear(void);

  /// save statistics and produce special output, used by Cola++
  void VDCtrippOff(void);

  /// use internal VDC trippoff control mechanism, used by Lumi++
  inline void useTrippoffControl(void) { trippoffControl = 1; return; };

  /// Get factor to convert charge to luminosity. Depends on target, reaction.
  inline double getMaterialFactor(void) { return MatFactor; }; 

  /// Set readout prescaler
  inline void setReadoutScaler(int preA, int singleA, int doubleA, int strobeA,
			       double strobedeadA,
			       int preB, int singleB, int doubleB, int strobeB,
			       double strobedeadB,
			       int preC, int singleC, int doubleC, int strobeC,
			       double strobedeadC, int singleK, int doubleK) { 
    scalerA.pre = preA; scalerA.singles = singleA; scalerA.doubles = doubleA;
    scalerA.strobe = strobeA; scalerA.strobedead = strobedeadA * 1E-9;
    scalerB.pre = preB; scalerB.singles = singleB; scalerB.doubles = doubleB;
    scalerB.strobe = strobeB; scalerB.strobedead = strobedeadB * 1E-9;
    scalerC.pre = preC; scalerC.singles = singleC; scalerC.doubles = doubleC;
    scalerC.strobe = strobeC; scalerC.strobedead = strobedeadC * 1E-9;
    scalerK.singles = singleK; scalerK.doubles = doubleK;
  }; 
};

void updateSQLdatabase(const char *run, luminosity *lumi, const char *db);

#endif
