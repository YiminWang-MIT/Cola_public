//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 2001
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: simDetector.cc 2588 2014-10-30 10:31:33Z kohly $
//
//
// M.Distler 21.5.14 Revision 2559   removed Hadron3 and ToF code

#include "Cola/rundatabase.h"    // Read the file "run.db"
#include "Cola/Targetlength.h"   // Targetcells and Energyloss
#include "Cola/QuasiRandom.h"    // Random Generator
#include "simDetector.h"
#include "Aqua/AquaTree.h"

using namespace std;

simNdet::simNdet(class AquaTree *outtree, Particle *p,
		 double ang, double oopang, double dist,
		 double hght, double wdth, const char *filename)
  : simDetectorBase(outtree, p, ang, oopang, 0, 0, 0, 0)
{
  distance = dist;
  height = hght;
  width = wdth;

#ifdef NEVER  
  cerr << "angle    = " << angle << endl;
  cerr << "oopangle = " << oop << endl;
  cerr << "distance = " << distance << endl;
  cerr << "height   = " << height << endl;
  cerr << "width    = " << width << endl;
  cerr << "filename = '"
	    << (filename ? filename : "NULL") << "'" << endl;
#endif
  colli = new colliPlane(distance, 0.5*width, 0.5*height);
  colli->init(angle*M_PI/180.0, oop);
}

simNdet::~simNdet()
{
  delete colli;
}

int simNdet::check(Particle vf, double x[3], class target *,
		   struct TargetCoord *, struct simFocalCoord *fc, modeltype ModelType)
{
  particle = vf;
  if (colli->checkHorizontal(x, vf) && colli->checkVertical(x, vf)) {
    fc->x = colli->posHorizontal(x, vf);
    fc->y = colli->posVertical(x, vf);
    atree->packEventData(&(fc->x), 1);
    atree->packEventData(&(fc->y), 1);
    return 1;
  }
  return 0;
}
