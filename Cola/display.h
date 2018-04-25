//								      -*-c++-*-
// Copyright (c) 2002
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: display.h 2216 2008-06-13 21:13:47Z distler $
//

#ifndef EVENT_DISPLAY
#define EVENT_DISPLAY

#include "Aqua/AquaTree.h"
#include "Detector/pdcD.h"
#include <iostream>
#include <fstream>

class EventDisplay {
private:
  std::ofstream *out;
  AquaNodeInt *nr;
  AquaNodeDouble *A_dp, *A_y0, *A_phi0, *A_theta0;
  pdcDplane *planeX;
  pdcDplane *planeY;
  int pipeto[2], pipefrom[2];
  FILE *pipein, *pipeout;
  int procid;
  int isfirst;
  
public:
  EventDisplay(AquaTree *atree, AquaTree *onl);
  ~EventDisplay() {fclose(pipein); fclose(pipeout);}
  void doDisplay();
};

#endif /* EVENT_DISPLAY */
