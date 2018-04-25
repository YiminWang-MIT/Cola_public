#ifndef __TOF_K_H__
#define __TOF_K_H__

#include "AquaTree.h"

#define degree (180./M_PI)

class tofK {
protected:
  AquaTree *online;

  AquaNodeDouble *theta_tof;
  AquaNodeDouble *phi_tof;
  AquaNodeDouble *xpositionG;
  AquaNodeDouble *ypositionG;
 
  double slopex,slopey,X,Y;

  double fe0, fe1, fe2, xR, thetaR;
  double Ix;

  double xLG, zLG;
  double yG;
  double alphaG;
  double lG;

  double xL, offsetL;
  
public:
    tofK(AquaTree *outTree);
    virtual ~tofK();

    // return 1 if coordinates are useful
    int handle(double &x, double &th, double &y, double &ph);
    int init(double * fe_pos, double * det_pos);
};

#endif /* __TOF_K_H__ */
