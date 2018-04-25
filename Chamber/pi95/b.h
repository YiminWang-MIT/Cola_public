/**************************************************************************
 * Standard Event                                                         *
 **************************************************************************/

#include "spectrometer.h"
#include "statistics.h"

struct DataB {
  struct BeamInfo	beam;
  struct RunInfo	run;
  struct DetB		det;
};
