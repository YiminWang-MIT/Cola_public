#include "KaosK.h"
#include <Aqua_online.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <iostream>

#undef DEBUG_KAOS_FE                    // Ausgabe der Fokalebenenkoord.

kaosK::kaosK(AquaTree *outTree)
{
  online = outTree;

  hitG      = (AquaNodeDouble *) online->find("online.kaos.gdet.positionXZ");
  yhitG     = (AquaNodeDouble *) online->find("online.kaos.gdet.positionY");
  hitH      = (AquaNodeDouble *) online->find("online.kaos.hdet.positionXZ");
  yhitH     = (AquaNodeDouble *) online->find("online.kaos.hdet.positionY");

  hitLx     = (AquaNodeDouble *) online->find("online.kaos.mwpc.l.x.hit");
  theta     = (AquaNodeDouble *) online->find("online.kaos.mwpc.theta");
  hitLy     = (AquaNodeDouble *) online->find("online.kaos.mwpc.l.y.hit");
  phi       = (AquaNodeDouble *) online->find("online.kaos.mwpc.phi");

  hitx_i    = (AquaNodeDouble *) online->find("online.kaos.coord.x_i");
  hitz_i    = (AquaNodeDouble *) online->find("online.kaos.coord.z_i");
}

kaosK::~kaosK()
{
  ;
}

int 
kaosK::init(double * fe_pos, double * det_pos, double rundb_use_track)
{

  use_track = rundb_use_track;

  // polynomial fit to the focal plane position in the 2012 coordinate system
  // the origin of the coordinate system is a mark on the platform
  // exactly 2500 mm (x) and 1000 mm (z) from pole entry
  // z(x) = C + B * x + A * x^2
  C    = fe_pos[2];
  B    = fe_pos[1];
  A    = fe_pos[0];

  // position and angle of reference trajectory
  x_R    = fe_pos[3]; // mm
  theta_R= fe_pos[4]/degree; // converted to rad

  // detector positions from run.db in the 2012 coordinate system
  xG     = det_pos[4];  // mm
  zG     = det_pos[5];  // mm
  alphaG = det_pos[7]/degree;  // converted to rad
  yG     = det_pos[11]; // mm
  lG     = det_pos[14]; // mm

  xH     = det_pos[2];  // mm
  zH     = det_pos[3];  // mm
  alphaH = det_pos[6]/degree;  // converted to rad
  yH     = det_pos[10]; // mm
  lH     = det_pos[13]; // mm

  xL     = det_pos[15]; // mm
  zL     = det_pos[30]; // mm
  yL     = det_pos[8];  // mm
  alphaL = det_pos[31]/degree; // converted to rad

  return 0;
} 

int kaosK::handle(double &x_f, double &th_f, double &y_f, double &ph_f)
{
  // This function calculates the track position and angle 
  // in the 2012 coordinate system defined by the mark on the platform
  // Then the intersection of the track with the focal plane is determined

  double theta_T = 0, phi_T = 0, x_T = 0, z_T = 0, y_T = 0;// track coordinates
  double hitxzG;
  double hitxzH;
  if (use_track == 3) // using the wall G and H hits
    {
      
      // distances are measured from the low-momentum edge of the wall
      // positions in mm
      hitxzG = hitG->scalar();
      hitxzH = hitH->scalar();

      double hityG  = yhitG->scalar() + yG;
      double hityH  = yhitH->scalar() + yH;
  
      double hitxG  = hitxzG * cos(alphaG) + xG;
      double hitzG  = hitxzG * sin(alphaG) + zG;

      double hitxH  = hitxzH * cos(alphaH) + xH;
      double hitzH  = hitxzH * sin(alphaH) + zH;

      // track inclination w.r.t. z-axis in xz-plane (rad)
      theta_T = atan( (hitxH - hitxG)/(hitzH - hitzG) );

      // track inclination w.r.t. y-axis in yT-plane (rad)
      phi_T = atan( (hityH - hityG)/sqrt(pow(hitxH - hitxG,2)
						+ pow(hitzH - hitzG,2) ) );
      
      #ifdef DEBUG_KAOS_FE
      std::cout << "G: " 
		<< "hitxG: " << hitxG << " hitzG:" << hitzG << std::endl;
      std::cout << "H: " 
		<< "hitxH: " << hitxH << " hitzH:" << hitzH << std::endl;
      #endif

      if (tan (theta_T) == 0)
	{
         #ifdef DEBUG_KAOS_FE
	  std::cout << "ERROR in focal plane coordinates" << std::endl;
         #endif
	  return 0;
	};
    
      // x_T is the reconstructed hit position along x-axis
      x_T = hitxG - hitzG * tan(theta_T);
    
      // z_T is the reconstructed hit position along z-axis
      z_T = hitzG - hitxG / tan(theta_T);
    
      // y_T is the reconstructed hit position along y-axis
      y_T = hityG - tan(phi_T) * sqrt( pow(hitxG,2) + pow(hitzG,2) ); 

    }
  else if (use_track == 4) // using the MWPC L and M hits
    {
      double hitxL  = hitLx->scalar()*cos(alphaL) + xL;
      double hitzL  = hitLx->scalar()*sin(alphaL) + zL;
      double hityL  = hitLy->scalar() + yL;

      // track inclination w.r.t. z-axis in xz-plane (rad)
      theta_T = theta->scalar()/degree;

      // track inclination w.r.t. y-axis in yT-plane (rad)
      phi_T = phi->scalar()/degree;

      // x_T is the reconstructed hit position along x-axis
      x_T = hitxL - hitzL * tan(theta_T);
    
      // z_T is the reconstructed hit position along z-axis
      z_T = hitzL - hitxL / tan(theta_T);
    
      // y_T is the reconstructed hit position along y-axis
      y_T = hityL - tan(phi_T) * sqrt( pow(hitxL,2) + pow(hitzL,2) ); 

    }

  #ifdef DEBUG_KAOS_FE
  std::cout << "TRACK: " 
	    << "x_T: " << x_T << " y_T: " << y_T 
	    << " th_T: " << theta_T << " ph_T: " << phi_T << std::endl;
  #endif

  // discriminant in solution of the quadratic equation
  double discriminant = pow(B - 1 / tan(theta_T),2) 
    - 4 * A * (C + x_T / tan(theta_T) );

  double x_i = 0, y_i = 0, z_i = 0;
			    
  if (discriminant > 0)
    {
      // coordinates of intersection between track and focal plane 
      x_i = (-1 * (B - 1 / tan (theta_T)) - sqrt(discriminant)) / (2 * A);
      z_i = (x_i - x_T) / tan(theta_T);
      y_i = y_T + z_i / cos(theta_T) * tan(phi_T);

      hitx_i->handle(x_i);
      hitz_i->handle(z_i);

      // focal plane coordinates
      x_f = (x_i - x_R - z_i * tan(theta_R)) * cos(theta_R); // mm 
      y_f = y_i;                                             // mm
      
      th_f = (theta_T - theta_R) * 1000; // mrad
      ph_f = phi_T * 1000;               // mrad

      #ifdef DEBUG_KAOS_FE
      std::cout << "INTERSECTION: " << "D:" << discriminant
		<< "x_i:" << x_i << " y_i:" << y_i 
		<< " z_i:" << z_i << std::endl;

      std::cout << "FE: " 
		<< "x_f:" << x_f << " th_f:" << th_f 
		<< " y_f:" << y_f << " ph_f:" << ph_f << std::endl;
      #endif

      return 1;
    }
  else 
    {
      #ifdef DEBUG_KAOS_FE
      std::cout << "ERROR in focal plane coordinate determination" << std::endl;
      #endif

      return 0;
    }
}
