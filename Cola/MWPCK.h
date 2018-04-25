#ifndef __MWPC_K_H__
#define __MWPC_K_H__

#include "AquaTree.h"

class mwpcK {
protected:
  AquaTree *online;

  AquaNodeDouble *theta;
  AquaNodeDouble *phi;
  AquaNodeDouble *hitLx;
  AquaNodeDouble *hitLy;

  double slopex,slopey,X,Y;

  double fe0,fe1,fe2,x0;
  double Ix;
  double xL, offsetL;

public:
  mwpcK(AquaTree *outTree);
  virtual ~mwpcK();

  // return 1 if coordinates are useful
  int handle(double &x, double &th, double &y, double &ph);
  int init(double * fe_pos, double * det_pos);
};

#endif /* __MWPC_K_H__ */
