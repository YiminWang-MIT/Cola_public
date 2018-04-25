#include "Aqua_online.h"

/** VdcToHdc: This function determines the particle's crossing point 
              through the HDC plane. Therefore the coordinates measured 
              in the VDC are handed over in the struct FocalCoord& Vdc.
	      The origin of the HDC frame has the coordinates (xt,yt,zt) 
	      in the VDC frame and is rotated around the y axis by the 
	      angle alpha, as well as in the resulting vth frame by the 
	      angle beta around the x axis and by the angle gamma around
	      the z axis. 
	      The crossing point (in the HDC frame) is handed over in the 
	      struct FocalCoord& Hdc.
	      In addition, the length r, representing the length of the
	      particle's path from VDC to HDC, is calculated and handed 
	      over. 
	      The angles in the FocalCoord structs are defined in mrad
	      the positions in mm.
*/

int VdcToHdc(FocalCoord& Vdc, /// INPUT : VDC coordinates
	     double xt,       /// x shift of the origin in the VDC system
	     double yt,       /// y shift of the origin in the VDC system
	     double zt,       /// z shift of the origin in the VDC system
	     double alpha,    /// rotation of the HDC-System around the y axis
	     double beta,     /// rotation of the HDC-System around the x axis
	     double gamma,    /// rotation of the HDC-System around the z axis
	     FocalCoord& Hdc, /// OUTPUT: HDC coordinates
	     double& r);      /// particle's path from VDC to HDC


/** PPolFillOnline:
    This function works on the online structure and calculates:
    -  the coordinates online.ppolA.vth out of the coordinates online.A.x,
       y, theta and phi by the routine VdcToHdc
    -  the coordinates dvh by substraction of online.ppolA.hdc from 
       online.ppolA.vth
    -  the coordinates online.ppol.scat, representing the coordinates of the
       point of scattering (in the carbon or anywhere else), the minimum 
       distance dmin of the VDC trace to the HDC trace as well as  the 
       azimuthal and polar angle of the scattering. */

int PPolFillOnline(AquaTree *out, struct online *online); 
double GenerateFPPTheta(double protonMomentum);
double Apow(double protonenergy, double theta);




















































