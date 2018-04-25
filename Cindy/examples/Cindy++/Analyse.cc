//
// Analyse.cc: Rahmen f"ur ein Cindy++ Analyse Programm
//

#include <iostream>
#include <iomanip>
#include "ana++/Cindy++.h"
#include "chamber/chlib++.h"

CindyTree *ct = cindyTreeAlloc();

#include "ab940223.h"
struct ab940223 ab940223;

VdcApacket vdcA(ct,".a.det.vdc",AX1SIZE,AS1SIZE,AX2SIZE,AS2SIZE);

int
main(int argc, char *argv[])
{
  if (ct->analyseGetOpt(argc,argv)) return -1;

  while (!ct->analyseNextEvent()) {
    if (vdcA.ok() == PACKET_STATUS_BOTH) {
      cout.setf(ios::fixed,ios::floatfield);
      cout << ct->eventCount()
        <<"\tx: " << setw(8) << setprecision(3) << vdcA.x()
        << " y: " << setw(8) << setprecision(3) << vdcA.y() 
        << " t: " << setw(8) << setprecision(3) << vdcA.theta()
        << " p: " << setw(8) << setprecision(3) << vdcA.phi() << endl;
    }
    vdcA.Reset();
  }

  return 0;
}
