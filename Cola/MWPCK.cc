#include "MWPCK.h"
#include <Aqua_online.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <iostream>

//#define DEBUG_KAOS_FE                    // Ausgabe der Fokalebenenkoord.

// For documentation see TOFK.cc


mwpcK::mwpcK(AquaTree *outTree)
{
  online = outTree;

  hitLx     = (AquaNodeDouble *) online->find("online.kaos.mwpc.l.x.hit");
  theta     = (AquaNodeDouble *) online->find("online.kaos.mwpc.theta");
  hitLy     = (AquaNodeDouble *) online->find("online.kaos.mwpc.l.y.hit");
  phi       = (AquaNodeDouble *) online->find("online.kaos.mwpc.phi");
}

mwpcK::~mwpcK()
{
  ;
}

int 
mwpcK::init(double * fe_pos, double * det_pos)
{
  // polynomial fit to the focal plane position relative to the MWPC
  fe0 = fe_pos[0];
  fe1 = fe_pos[1];
  fe2 = fe_pos[2];

  x0  = fe_pos[3];

  xL   = det_pos[15];
  offsetL= det_pos[30];

  double x1= fe1 + 2*fe2*x0;
  Ix= x1*sqrt(1+x1*x1)+asinh(x1);

  return 0;
} 

int mwpcK::handle(double &x, double &th, double &y, double &ph)
{
  double slopex= tan(theta->scalar()/180*M_PI);

  if (slopex == 0) return 0;

  double slopey= tan(phi->scalar()/180*M_PI);
  double X= hitLx->scalar() + xL - 1260.2; // new offset from ACAD (20.09.2011)
  double Y= hitLy->scalar();

  double mx= 1./slopex;
  double b= -mx * X + offsetL; // 225.63 is the offset between the chamber L
  // pregap plane and the chamber holder midplane which is used as the baseline 
  // for the coordiante system in which the focal plane is parametrsised
  double w= pow(fe1 - mx,2) - 4*fe2*(fe0 - b);
  double s1,z,d2,s2,Ix2;

  if (w>0)
    {
      s1= (-(fe1-mx) - sqrt(w))/2/fe2;
      z=  fe0 + fe1*s1 + fe2*s1*s1;
      d2= pow(s1 - X,2) + (z*z);
      s2= fe1 + 2*fe2*s1;
      Ix2=s2*sqrt(1+s2*s2) + asinh(s2);

      x=  (Ix2-Ix)/4/fe2; // in mm
      th= fe1 + 2*fe2*s1 - mx + 0.632;
      y=  Y + slopey * sqrt(d2); // in mm
      ph= slopey;

      #ifdef DEBUG_KAOS_FE
      std::cout << "FE: " 
		<< "x: " << x << " th: " << th 
		<< " y: " << y << " ph: " << ph << std::endl;
      #endif

      return 1;
    }
  else 
    {
      std::cout << "ERROR in KAOS FOCAL PLANE COORDINATES" << std::endl;
      
      return 0;
    }
}
