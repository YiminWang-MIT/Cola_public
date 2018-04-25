//
// Filter.cc: Rahmen f"ur ein Cindy++ Filter Programm
//

#include "ana++/Cindy++.h"

#include "ab940223.h"
struct ab940223 ab940223;
struct DataA& spekA = ab940223.a;
struct DataB& spekB = ab940223.b;

CindyTree *ct = cindyTreeAlloc();

int
main(int argc, char *argv[])
{
  int AtriggerOK, BtriggerOK;

  if (ct->analyseGetOpt(argc,argv)) return -1;

  while (!ct->analyseNextEvent()) {

    AtriggerOK = 0;
    // "Uberpr"ufung ob Trigger A gefeuert hat
    // und der Laser aus war
    if (ct->itemOK(&spekA.det.trigger.info) &&
        ct->itemOK(&spekA.det.sync_info)) {
      AtriggerOK = (spekA.det.sync_info & 0x2000)
                   && !spekA.det.trigger.info;
    }

    BtriggerOK = 0;
    // "Uberpr"ufung ob Trigger B gefeuert hat
    // und der Laser aus war
    if (ct->itemOK(&spekB.det.trigger.info) &&
	ct->itemOK(&spekB.det.sync_info)) {
      BtriggerOK = (spekB.det.sync_info & 0x4000)
                   && !spekB.det.trigger.info;
    }

    if (AtriggerOK && BtriggerOK) {
      ct->putEventData();    // ... then write event to output
    }
  }

  return 0;
}
