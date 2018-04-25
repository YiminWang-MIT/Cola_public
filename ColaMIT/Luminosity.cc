//
// Calculation of Luminosity, runtime, count rates etc...
// $Id: Luminosity.cc 2640 2015-04-01 14:03:00Z aqua $
//

#define _DEFAULT_SOURCE
#include "Luminosity.h"
#include "oops1.h"
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <ctime>

#ifndef HUGE
#define HUGE 1.797693134862315708e308
#endif
// define physical constants
const double CHARGE     = 1.60217733E-19;
const double N_AVOGADRO = 6.0221367E23;

// define ExpTyp
const int UNKNOWN    = 0;
const int SINGLE_A   = 2;
const int SINGLE_B   = 4;
const int SINGLE_C   = 6;
const int SINGLE_D   = 8;
const int DOUBLE_OA  = 10;
const int DOUBLE_OB  = 12;
const int DOUBLE_OC  = 14;
const int DOUBLE_OD  = 16;

int
luminosity::clear()
{
  // init starttime, stoptime
  start_time = 0;
  stop_time  = 0;

  // init int class-variables
  firstOps = firstAps = firstBps = firstCps = firstDps = 0;

  // init long class-variables
  events           = firstevent    = lastevent      = 
    lastevent_trip = tripevent     =
    Oevents        = 
    Aevents        = Bevents       = Cevents        = Devents      =
    Oevents_end    =
    Aevents_end    = Bevents_end   = Cevents_end    = Devents_end  =
    OAevents       = OBevents      = OCevents       = ODevents     =
    OAevents_end   = OBevents_end  = OCevents_end   = ODevents_end = 0;
  
  // init double class-variables
    o_realtime_offset    = o_runtime_offset     =
    o_realtime_end       = o_runtime_end        =
    o_realtime           = o_runtime            =
    a_realtime_offset    = a_runtime_offset     =
    a_realtime_end       = a_runtime_end        =
    a_realtime           = a_runtime            =
    b_realtime_offset    = b_runtime_offset     =
    b_realtime_end       = b_runtime_end        =
    b_realtime           = b_runtime            =
    c_realtime_offset    = c_runtime_offset     =
    c_realtime_end       = c_runtime_end        =
    c_realtime           = c_runtime            =
    d_realtime_offset    = d_runtime_offset     =
    d_realtime_end       = d_runtime_end        =
    d_realtime           = d_runtime            =
    o_deadtime  = a_deadtime  = b_deadtime  = c_deadtime = d_deadtime =
    oa_deadtime = ob_deadtime = oc_deadtime = od_deadtime =
    o_ps_realtime_offset = o_ps_runtime_offset  =
    o_ps_realtime_last   = o_ps_runtime_last    =
    o_ps_realtime_end    = o_ps_runtime_end     =
    o_ps_realtime        = o_ps_runtime         =
    o_ps_scint_hits      = o_ps_scint_hits_last = o_ps_scint_hits_end  =
    a_ps_realtime_offset = a_ps_runtime_offset  =
    a_ps_realtime_last   = a_ps_runtime_last    =
    a_ps_realtime_end    = a_ps_runtime_end     =
    a_ps_realtime        = a_ps_runtime         =
    a_ps_scint_hits      = a_ps_scint_hits_last = a_ps_scint_hits_end  =
    b_ps_realtime_offset = b_ps_runtime_offset  =
    b_ps_realtime_last   = b_ps_runtime_last    =
    b_ps_realtime_end    = b_ps_runtime_end     =
    b_ps_realtime        = b_ps_runtime         =
    b_ps_scint_hits      = b_ps_scint_hits_last = b_ps_scint_hits_end  =
    c_ps_realtime_offset = c_ps_runtime_offset  =
    c_ps_realtime_last   = c_ps_runtime_last    =
    c_ps_realtime_end    = c_ps_runtime_end     =
    c_ps_realtime        = c_ps_runtime         =
    c_ps_scint_hits      = c_ps_scint_hits_last = c_ps_scint_hits_end  =
    d_ps_realtime_offset = d_ps_runtime_offset  =
    d_ps_realtime_last   = d_ps_runtime_last    =
    d_ps_realtime_end    = d_ps_runtime_end     =
    d_ps_realtime        = d_ps_runtime         =
    d_ps_scint_hits      = d_ps_scint_hits_last = d_ps_scint_hits_end  = 0.0;
  return 0;
}

luminosity::luminosity(AquaTree *atree, target *Target,
		       reaction *Reaction, double Ebeam) : statPageBase(atree)
{
  // init MatFactor
  MatFactor = 1.0 / CHARGE       // particles/C
    * Target->getTargetMat()->getDensity()    // g/cm^3
    / Target->getTargetMat()->getMassnumber() // mol/g
    * N_AVOGADRO                 // 1/mol
    * Target->getLength() / 10.0 // cm
    / 1E30;                      // cm^2/mubarn

  // init experiment type
  ExpTyp = UNKNOWN;
 
  clear();
}

//SS%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

int 
luminosity::handle(void *data)
{
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
  return (firstevent ? dead(ExpTyp) : 100.0);
}

double 
luminosity::getRunTime()
{
  return 0.0;
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
  return 0;
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
  return 0.0;
}

double 
luminosity::dead(int exptyp)
{
  return 0.0;
}

void 
luminosity::output(std::ostream *out)
{
  return;
}

void 
luminosity::output(char * &StartTime, char * &StopTime, int &Events,
	   double O[], double A[], double B[], double C[], double D[],
	   double OA[], double OB[], double OC[], double OD[])
{
  return;
}
