/**************************************************************************
 * Standard Event                                                         *
 **************************************************************************/

#include "spectrometer.h"
#include "statistics.h"

struct DataC {
  struct RunInfo	run;
  struct BeamInfo	beam;
  struct DetC		det;
  struct EventInfo	coinc;
};
