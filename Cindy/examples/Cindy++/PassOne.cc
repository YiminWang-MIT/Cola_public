//
// PassOne.cc: Rahmen fuer einen Cindy++ Filter
//

#include <cstdio>
#include <cmath>
#include "ana++/Cindy++.h"
#include "chamber/chlib++.h"
#include "tma/TraceMachine.h"

#include "ab940223.h"
struct ab940223 ab940223;
struct DataA& spekA = ab940223.a;
struct DataB& spekB = ab940223.b;

#include "pass1.h"
struct pass1 pass1;

CindyTree *ct = cindyTreeAlloc();
CindyTree *cx = cindyOutTreeAlloc();

int
main(int argc, char *argv[])
{
  // struct focalplane_ko eventA, eventB;
  struct target_ko *resultA, *resultB;

  // Initialisierung von Chamber- und TMA-Library
  // ...

  if (ct->analyseGetOpt(argc,argv,cx)) return -1;

  while (!ct->analyseNextEvent()) {

    resultA = resultB = NULL;

    // Berechnung von Fokalebenenkoordinaten: eventA und eventB
    // Berechnung von Targetkoordinaten: resultA resultB
    // ...

    if (resultA && resultB) {
      pass1.a.target.dp  = resultA->dp_tg;
      pass1.a.target.th  = resultA->th_tg;
      pass1.a.target.y0  = resultA->y_tg;
      pass1.a.target.ph  = resultA->ph_tg;
      pass1.a.target.len = resultA->len;
      cx->packEventData(&pass1.a.target,
                        sizeof(struct Target)/sizeof(float));
      pass1.b.target.dp  = resultB->dp_tg;
      pass1.b.target.th  = resultB->th_tg;
      pass1.b.target.y0  = resultB->y_tg;
      pass1.b.target.ph  = resultB->ph_tg;
      pass1.b.target.len = resultB->len;
      cx->packEventData(&pass1.b.target,
                        sizeof(struct Target)/sizeof(float));
    }
  }

  return 0;
}
