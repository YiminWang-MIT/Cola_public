#include "TOFK.h"
#include <Aqua_online.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <iostream>

#undef USE_LARS_FOCLAPLANE
//#define DEBUG_KAOS_FE                    // Ausgabe der Fokalebenenkoord.

tofK::tofK(AquaTree *outTree)
{
  online = outTree;

  xpositionG = (AquaNodeDouble *) online->find("online.kaos.gdet.positionXZ");
  ypositionG = (AquaNodeDouble *) online->find("online.kaos.gdet.positionY");
  theta_tof = (AquaNodeDouble *) online->find("online.kaos.tof.theta");
  phi_tof   = (AquaNodeDouble *) online->find("online.kaos.tof.phi");
}

tofK::~tofK()
{
  ;
}

int 
tofK::init(double * fe_pos, double * det_pos)
{
  // polynomial fit to the focal plane position relative to the MWPC
  // the origin of the coordinate system is the mounting of the chamber holder
  // z(x) = fe0 + fe1 * x + fe2 * x^2
  fe0 = fe_pos[0];
  fe1 = fe_pos[1];
  fe2 = fe_pos[2];

  // xR is the x coordinate of the intersection between reference track and the focal plane
  xR  = fe_pos[3];

  // detector positions
  xLG = det_pos[4];
  zLG = det_pos[5];

  alphaG = det_pos[7];

  yG  = det_pos[11];
  lG  = det_pos[14];

  xL   = det_pos[15];
  offsetL= det_pos[30];

#ifdef USE_LARS_FOCLAPLANE
  // Ix = length along the focal plane to the crossing of the reference track
  double x1= fe1 + 2*fe2*xR;
  Ix= x1*sqrt(1+x1*x1)+asinh(x1);
#endif

  return 0;
} 

int tofK::handle(double &x, double &th, double &y, double &ph)
{
  // This function calculates the position of the tzrack hit on the focal plane
  // by extrapolating the track back to the coordinate system in which the MWPC L
  // channel 0 is the centre and then finding the intersection between the track 
  // and the focal plane.
  // For further documentation see the thesis of Lars Nungesser (p. 48 ff).
  // The coordinate system used here has its origin on chamber L channel 0,
  // except for the parametrisation of the focal plane

  double slopex= tan(theta_tof->scalar()/degree);
  //  double slopey= tan(phi_tof->scalar()/degree);
 
  // Xprime is the x position of the hit point in ToF wall G
  double Xprime = xpositionG->scalar() / cos(theta_tof->scalar()/degree) * 
    cos((theta_tof->scalar()+alphaG)/degree);
  
  // X is the reconstructed hit position in the plane of MWPC L
  double X = (Xprime + xLG - tan(theta_tof->scalar()/degree)*zLG)
    + xL - 1260.2; // new offset from ACAD (20.09.2011)
  
  //  double Zprime = zLG + tan(alphaG/degree)*(X - xLG);
  //  double Z = Zprime * cos(alphaG/degree)/cos((alphaG+theta_tof->scalar())/degree);
  //  double Y = ypositionG->scalar() + yG - tan(phi_tof->scalar()/degree)*Z;
 
  // the particle track is parametrisized by z(x) = mx * x + b
  double mx= 1./slopex;
  double b= -mx * X + offsetL; 
  // 225.63 is the offset between the chamber L pregap plane 
  // and the chamber holder midplane which is used as the baseline 
  // for the coordiante system in which the focal plane is parametrsised
  
  // w is the discriminant for solving the quadratic equation which arises from 
  // searching the intersection of the particle track and the focal plane
  double w= pow(fe1 - mx,2) - 4*fe2*(fe0 - b);
  double s1,z; 
  
#ifdef USE_LARS_FOCLAPLANE
  double d2,s2,Ix2;
#endif

  if (w>0)
    {
      s1= (-(fe1-mx) - sqrt(w))/2/fe2;
      // s1 is the x value of the intersection of track and focal plane
      z=  fe0 + fe1*s1 + fe2*s1*s1;
      // z is the according z value
#ifdef USE_LARS_FOCLAPLANE
      d2= pow(s1 - X,2) + (z*z);

      s2= fe1 + 2*fe2*s1;
      Ix2=s2*sqrt(1+s2*s2) + asinh(s2);
      // Ix2 is the length along the focal plane to the track intersection point

      // x, th, y and ph are the focalplane coordinates
      x=  (Ix2-Ix)/4/fe2; // in mm
      th= fe1 + 2*fe2*s1 - mx + 0.632;
      y=  Y + slopey * sqrt(d2); // in mm
      ph= slopey;
#else
      
#endif

      #ifdef DEBUG_KAOS_FE
      std::cout << "FE: " 
		<< "x: " << x << " th: " << th 
		<< " y: " << y << " ph: " << ph << std::endl;
      #endif

      return 1;
    }
  else 
    {
      #ifdef DEBUG_KAOS_FE
      std::cout << "ERROR in KAOS FOCAL PLANE COORDINATES" << std::endl;
      #endif

      return 0;
    }

}
