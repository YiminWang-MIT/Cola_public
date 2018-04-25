/**************************************************************************
 * Standard Event                                                         *
 **************************************************************************/

#include "spectrometer.h"
#include "statistics.h"

struct MpsDataC {
  struct MpsRunInfo	run;
  struct MpsBeamInfo	beam;
  struct MpsDetC	det;
  struct MpsEventInfo	coinc;
};
