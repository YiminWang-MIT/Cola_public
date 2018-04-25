#ifndef _simChamber_h_
#define _simChamber_h_

#include "Cola/Masses.h" 

class Chamber
{
 private:
  bool nofile;
  int bins[4];
  int bininc[4];
  double start[4];
  double increment[4];
  double *values;

 public:
  Chamber(char * filename);
  bool ok(double angle, double dp,double z, double th, double ph);
  

};

#endif
