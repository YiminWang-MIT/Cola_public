// ConeTest.cc, V 1.0: T.Pospischil, 6.4.98
//
 
#define _DEFAULT_SOURCE 1
#include <math.h>
#include "ConeTest.h"
#include "hdcPacket.h"   // For the distances of the wire planes to each other

/// --------------------------------------------------------- ConeTest:
/** 
   This rountine gets its input data from the data structure online, refering 
   to vth, hdc and scat coordinates. Therefore, the routine PPolFillOnline() 
   (module ppol.cc) has to be called before starting this routine.
   
   ConeTest calculates the proton's coordinates in the 4 HDC planes for an
   azimutal angle of scattering of online.ppolA.scat.ph + 180 deg.

   If these coordinates are located inside of so called restricted 
   areas (outside the acceptance of the HDC, in driftcells of dead 
   wires, ...), defined in the source code, the return value is 1. 
   Are the actual located inside of the restricted areas, the return 
   value is 2. Otherwise it is 0.
*/

int ConeTest(struct online *onl)
{ 
  if ( !(onl->A.vdcOK && onl->ppolA.hdcOK) ) return 3;

  const double sin_gamma = sin(0.78539816);  // 45 deg = 785.39816 mrad
  const double cos_gamma = cos(0.78539816);

  // declaration of variables:
  double a, b, c, d, e, f, g;
  double dx, dy, dz, D;
  double wi, xih, yih, zh;// Crossing point of 180-deg-proton through HDC-Ebene
  double xjh, yjh;        // Crossing point of actual proton through HDC-Ebene
  double ui, vi, uj, vj;

  // reservation of variables for calculation of Cone vector (e, f, g):
  a = tan(onl->ppolA.hdc.th/1000.0);
  b = tan(onl->ppolA.vth.th/1000.0);
  c = tan(onl->ppolA.hdc.ph/1000.0);
  d = tan(onl->ppolA.vth.ph/1000.0);
  
  D = sqrt(1+ b*b +d*d);
  dx = b/D;
  dy = d/D;
  dz = 1.0/D;

  // Calculation of Cone vector (cf. Polarimeter-Logbook page 284):
  e = 2.0*( (dx*dx-0.5)*a +       dx*dy*c +     dx*dz);
  f = 2.0*(       dy*dx*a + (dy*dy-0.5)*c +     dy*dz);
  g = 2.0*(       dz*dx*a +       dz*dy*c + dz*dz-0.5); 

  // Cone vector too flat or pointing downwards => not accepted
  if (g < 0.0001) return 1;

  // -------------------------------------- HDC U1 plane:
  zh = 0.0;
  // 180 deg. proton:
  wi  = (zh - onl->ppolA.scat.z) / g;
  xih = onl->ppolA.scat.x + wi * e;
  yih = onl->ppolA.scat.y + wi * f;
  ui  = xih * cos_gamma - yih * sin_gamma; 
  // actual proton:
  xjh = onl->ppolA.hdc.x + zh * a;
  yjh = onl->ppolA.hdc.y + zh * c;
  uj  = xjh * cos_gamma - yjh * sin_gamma; 
  // +++++++++++++++++  cut on the restricted areas of the U1 plane:
  // ++ for the 180 deg. proton:
  if ((xih <  367.70) || (xih > 2545.70)) return 1;
  if ((yih < -374.75) || (yih >  374.75)) return 1;
  // ++ for the actual proton:
  if ((xjh <  367.70) || (xjh > 2545.70)) return 2;
  if ((yjh < -374.75) || (yjh >  374.75)) return 2;

  // -------------------------------------- HDC V1 plane:
  zh = WP_DIST;
  // 180 deg. proton:
  wi  = (zh - onl->ppolA.scat.z) / g;
  xih = onl->ppolA.scat.x + wi * e;
  yih = onl->ppolA.scat.y + wi * f;
  vi  = xih * sin_gamma + yih * cos_gamma; 
  // actual proton:
  xjh = onl->ppolA.hdc.x + zh * a;
  yjh = onl->ppolA.hdc.y + zh * c;
  vj  = xjh * sin_gamma + yjh * cos_gamma; 
  // +++++++++++++++++  cut on the restricted areas of the V1 plane:
  // ++ for the 180 deg. proton:
  // ++ for the actual proton:

  // -------------------------------------- HDC U2 plane:
  zh = DC_DIST;
  // 180 deg. proton:
  wi  = (zh - onl->ppolA.scat.z) / g;
  xih = onl->ppolA.scat.x + wi * e;
  yih = onl->ppolA.scat.y + wi * f;
  ui  = xih * cos_gamma - yih * sin_gamma; 
  // actual proton:
  xjh = onl->ppolA.hdc.x + zh * a;
  yjh = onl->ppolA.hdc.y + zh * c;
  uj  = xjh * cos_gamma - yjh * sin_gamma; 
  // +++++++++++++++++  cut on the restricted areas of the U2 plane:
  // ++ for the 180 deg. proton:
  // cout << ui << endl;
  if ((ui >  360) && (ui <  380)) return 1;  // >>>>>>>>>>>>>>  wire U2 - 18
  if ((ui > 1660) && (ui < 1680)) return 1;  // >>>>>>>>>>>>>>  wire U2 - 83
  // ++ for  the actual proton:
  if ((uj >  360) && (uj <  380)) return 2;  // >>>>>>>>>>>>>>  wire U2 - 18
  if ((uj > 1660) && (uj < 1680)) return 2;  // >>>>>>>>>>>>>>  wire U2 - 83

  // -------------------------------------- HDC V2 plane:
  zh = DC_DIST + WP_DIST;
  // 180 deg. proton:
  wi  = (zh - onl->ppolA.scat.z) / g;
  xih = onl->ppolA.scat.x + wi * e;
  yih = onl->ppolA.scat.y + wi * f;
  vi  = xih * sin_gamma + yih * cos_gamma; 
  // actual proton:
  xjh = onl->ppolA.hdc.x + zh * a;
  yjh = onl->ppolA.hdc.y + zh * c;
  vj  = xjh * sin_gamma + yjh * cos_gamma; 
  // +++++++++++++++++  cut on the restricted areas of the V2 plane:
  // ++ for the 180 deg. proton:
  if ((vi > 1840) && (vi < 1860)) return 1;  // >>>>>>>>>>>>>> wire V2 - 92
  if ((xih <  367.70) || (xih > 2545.70)) return 1;
  if ((yih < -374.75) || (yih >  374.75)) return 1;
  // ++ for the actual proton:
  if ((vj > 1840) && (vj < 1860)) return 2;  // >>>>>>>>>>>>>> wire V2 - 92
  if ((xjh <  367.70) || (xjh > 2545.70)) return 2;
  if ((yjh < -374.75) || (yjh >  374.75)) return 2;

  return 0;
}























