/**************************************************************************
 * Standard Event                                                         *
 **************************************************************************/

#include "spectrometer.h"
#include "statistics.h"

struct MpsDataA {
  struct MpsRunInfo	run;
  struct MpsBeamInfo	beam;
  struct MpsDetA       	det;
  struct MpsEventInfo	coinc;
};
