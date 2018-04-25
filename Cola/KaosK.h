#ifndef __KAOS_K_H__
#define __KAOS_K_H__

#include "AquaTree.h"

#define degree (180./M_PI)

class kaosK {
protected:
  AquaTree *online;
  AquaNodeDouble *hitG, *hitH;
  AquaNodeDouble *yhitG, *yhitH;

  AquaNodeDouble *theta;
  AquaNodeDouble *phi;
  AquaNodeDouble *hitLx;
  AquaNodeDouble *hitLy;

  AquaNodeDouble *hitx_i, *hitz_i;

  double use_track;

  double A,B,C;

  double x_R, theta_R;

  double xG, zG, xH, zH, xL, zL;
  double yG, yH, yL;
  double alphaG, alphaH, alphaL;
  double lG, lH;
  
  int use_multi_track_analysis;

public:
  kaosK(AquaTree *outTree);
  virtual ~kaosK();

  int handle(double &x, double &th, double &y, double &ph);
  int init(double * fe_pos, double * det_pos, double rundb_use_track);
};

#endif /* __KAOS_K_H__ */
