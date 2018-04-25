//								      -*-c++-*-
// $Id: Luminosity.h 2216 2008-06-13 21:13:47Z distler $
//

#ifndef __LUMINOSITY_H__
#define __LUMINOSITY_H__

#include "Cola/Reactions.h"
#include "Targetlength.h"
#include "Cola/StatBase.h"
#include <ctime>
#include <iostream>

/**
  This class calculates the luminosity and deadtime.
  The Constructor needs the AquaTree-Pointer, the used target, the Reaction
  variable and the number of Turns in MAMI-B for correct initialization.
  With the Reaction variable the type of experiment is determined.

  Every event in the event-loop of the analysis-program the method 'handle'
  has to be executed.

  With 'getIntegrated', 'getRunTime'  and 'getDeadtime' you get the actual
  values of the integrated luminosity, the integrated run time and the
  corresponding deadtime for the experimenttype, set with the Reaction
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
  struct {
    int pre;
    int singles;
    int doubles;
  } scalerO, scalerA, scalerB, scalerC, scalerD;

  double turns;
  double MatFactor;

  int ExpTyp;

  int calcmode;

  int firstOps, firstAps, firstBps, firstCps, firstDps;

  long events, firstevent, lastevent, lastevent_trip, tripevent;

  long Oevents,   Oevents_end;
  long Aevents,   Aevents_end;
  long Bevents,   Bevents_end;
  long Cevents,   Cevents_end;
  long Devents,   Devents_end;
  long OAevents,  OAevents_end;
  long OBevents,  OBevents_end;
  long OCevents,  OCevents_end;
  long ODevents,  ODevents_end;

  double o_deadtime, a_deadtime, b_deadtime, c_deadtime, d_deadtime;
  double oa_deadtime, ob_deadtime, oc_deadtime, od_deadtime;

  double o_realtime, o_runtime;
  double a_realtime, a_runtime;
  double b_realtime, b_runtime;
  double c_realtime, c_runtime;
  double d_realtime, d_runtime;

  double o_realtime_offset, o_runtime_offset;
  double a_realtime_offset, a_runtime_offset;
  double b_realtime_offset, b_runtime_offset;
  double c_realtime_offset, c_runtime_offset;
  double d_realtime_offset, d_runtime_offset;

  double o_realtime_end, o_runtime_end;
  double a_realtime_end, a_runtime_end;
  double b_realtime_end, b_runtime_end;
  double c_realtime_end, c_runtime_end;
  double d_realtime_end, d_runtime_end;

  double o_ps_realtime, o_ps_runtime, o_ps_scint_hits;
  double a_ps_realtime, a_ps_runtime, a_ps_scint_hits;
  double b_ps_realtime, b_ps_runtime, b_ps_scint_hits;
  double c_ps_realtime, c_ps_runtime, c_ps_scint_hits;
  double d_ps_realtime, d_ps_runtime, d_ps_scint_hits;

  double o_ps_realtime_last, a_ps_realtime_last, b_ps_realtime_last,
         c_ps_realtime_last, d_ps_realtime_last;
  double o_ps_runtime_last, a_ps_runtime_last, b_ps_runtime_last,
         c_ps_runtime_last, d_ps_runtime_last;
  double o_ps_scint_hits_last, a_ps_scint_hits_last, b_ps_scint_hits_last,
         c_ps_scint_hits_last, d_ps_scint_hits_last;

  double o_ps_realtime_offset, a_ps_realtime_offset, b_ps_realtime_offset,
         c_ps_realtime_offset, d_ps_realtime_offset;
  double o_ps_runtime_offset, a_ps_runtime_offset, b_ps_runtime_offset,
         c_ps_runtime_offset, d_ps_runtime_offset;
  double o_ps_scint_hits_offset, a_ps_scint_hits_offset, b_ps_scint_hits_offset,
         c_ps_scint_hits_offset, d_ps_scint_hits_offset;

  double o_ps_realtime_end, a_ps_realtime_end, b_ps_realtime_end,
         c_ps_realtime_end, d_ps_realtime_end;
  double o_ps_runtime_end, a_ps_runtime_end, b_ps_runtime_end,
         c_ps_runtime_end, d_ps_runtime_end;
  double o_ps_scint_hits_end, a_ps_scint_hits_end, b_ps_scint_hits_end,
         c_ps_scint_hits_end, d_ps_scint_hits_end;

public:
  /// Constructor
  luminosity(AquaTree *atree, target *Target,
	     reaction *Reaction, double Ebeam);

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
	   double O[], double A[], double B[], double C[], double D[],
	   double OA[], double OB[], double OC[], double OD[]);

  /// Output of complete statistics to given 'filename'
  virtual int print(const char *filename, char *tripfile = NULL);

  /// clear statistic page
  virtual int clear(void);

  /// Get factor to convert charge to luminosity. Depends on target, reaction.
  inline double getMaterialFactor(void) { return MatFactor; }; 

  /// Set readout prescaler
  inline void setReadoutScaler(int preO, int singleO, int doubleO,
			       int preA, int singleA, int doubleA,
			       int preB, int singleB, int doubleB,
			       int preC, int singleC, int doubleC,
			       int preD, int singleD, int doubleD) {
    scalerO.pre = preO; scalerO.singles = singleO; scalerO.doubles = doubleO;
    scalerA.pre = preA; scalerA.singles = singleA; scalerA.doubles = doubleA;
    scalerB.pre = preB; scalerB.singles = singleB; scalerB.doubles = doubleB;
    scalerC.pre = preC; scalerC.singles = singleC; scalerC.doubles = doubleC;
    scalerD.pre = preD; scalerD.singles = singleD; scalerD.doubles = doubleD;
  }; 
};

#endif










