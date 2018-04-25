/**************************************************************************
 * Standard Event                                                         *
 **************************************************************************/

#include "spectrometer.h"
#include "statistics.h"

struct DataA {
  struct RunInfo	run;
  struct BeamInfo	beam;
  struct DetA		det;
  struct EventInfo	coinc;
};
