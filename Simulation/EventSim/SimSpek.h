
#ifndef __SIMSPEK_H_
#define __SIMSPEK_H_

#include "debug.h"



#define OUTX 0
#define OUTTHETA 1
#define OUTY 2
#define OUTPHI 3
#define OUTLEN 4





#define MAXWIRECOUNT 500

#include "Aqua/AquaReader.h"
#include "Generator.h"
#include "TMA/transandlook.h"
#include "Cola/QuasiRandom.h"
#include <string>



class SimSpek {
 protected:
   AquaNodeShort *syncinfo;
  AquaNodeShort *triggerinfo;
  AquaNodeShort *beamposition;
  AquaNodeShort *vdcx1[MAXWIRECOUNT];
  AquaNodeShort *vdcs1[MAXWIRECOUNT];
  AquaNodeShort *vdcx2[MAXWIRECOUNT];
  AquaNodeShort *vdcs2[MAXWIRECOUNT];
  std::string name;
  int speknum;
  int maxX1,maxX2,maxS1,maxS2;
  double sposX1,sposX2,sposS1,sposS2;

  double sin_x_s, sec_x_s, cos_x_s, csc_x_s,  tan_x_s, cot_x_s;
  double wobdx,wobdy,wobx0,woby0;

  TL transform;
  double in[DEF_DIMIN];
  double out[DEF_DIMOUT];

  double slopex,slopes,posx1,posx2,poss1,poss2;

  double EFeffX1[MAXWIRECOUNT], EFeffS1[MAXWIRECOUNT],EFeffX2[MAXWIRECOUNT],EFeffS2[MAXWIRECOUNT];

  double Efficency(double dist,double slope, double eff);
  double rnd();
  double norm();
  short  Time(double dist, double slope);
  int offx1,offx2,offs1,offs2;
  int iset;
  double gset;
  double dvel;

 public:
  SimSpek(AquaIO *io,int type);
  void rescanPointers(AquaTree *atree);
  void trigger(GenState *state);
  void fill(GenState *state);
};


#endif
