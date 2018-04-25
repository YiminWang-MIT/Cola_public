/**************************************************************************
 * Standard Event                                                         *
 **************************************************************************/

#include "spectrometer.h"
#include "statistics.h"

struct MpsDataB {
  struct MpsRunInfo	run;
  struct MpsBeamInfo	beam;
  struct MpsDetB	det;
  struct MpsEventInfo   coinc;
};
