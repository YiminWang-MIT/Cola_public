// ppol.cc, V 1.0: T.Pospischil, 1996
//
//          V 1.1: C. Bauer, T.Pospischil,  8.10.97:
//                 Correction of phi offset (beta) with rotation matrix
//                 beta, rotation of HDC around HDC x axis
//                 NEW: gamma (psi-Offset): rotation of HDC around HDC z axis
 
#define _DEFAULT_SOURCE 1
#include <math.h>
#include <iostream>
#include "FourVector/FourVector.h"
#include "ppol.h"
#include "rundatabase.h"
#include "ConeTest.h"
#include "QuasiRandom.h"

using namespace std;

// ==========================================================================
/** Functions for calculation of a proton's energy loss between VDC and the 
    middle of the carbon, including calculation of the analyzing power there:
*/

///---------------------------- E_mid_Carbon ---------------------------------

/** The reason to write this function was that the Bethe-Bloch-Formula 
    is not very accurate for step sizes around 10. If one uses something 
    like 10000 it becomes to slow for the event by event analysis.
  
    Therefore I ran the Bethe-Bloch energy loss calculation (E_mid_BeBl)
    for energy loss of protons in carbon (1.76 g/cm^3) blocks 
    with various (0.5-10cm full) carbon thickness and for kinetic proton 
    energies betw. 75 and 275 MeV. Then we made a 2D fit to the results.
  
    This function gives also now the kin. energy (in MeV) of protons 
    in the middle of a carbon (1.76 g/cm^3) block for given 
                T    (MeV,    kin. energy at the entrance)
                df   (cm,     full path length in the carbon)

    The function is only valid for 75 MeV <= T_kin <= 275 MeV and df < 10cm !!
*/

double E_mid_Carbon(double T,          /// kin. energy at carbon entrance (MeV)
		    double df)         /// full path length in the carbon (cm)
{
  if (T <   75) T  =  75;
  if (T >  275) T  = 275;
  if (df < 0.5) df =  0.5;
  if (df >  10) df = 10.0;

  double T_akt;
  double d  = df/2.0;
  double d2 = d*d;
  double d3 = d2*d;
  double d4 = d3*d;
  double T2 = T*T;
  double T3 = T2*T;
  double T4 = T3*T;
  
  T_akt = -18.0824       +     
    1.68021       * T    +
    -43.8742      * d    +
    -0.00962642   * T2   +
    0.708271      * T*d  +
    -4.97526      * d2   +
    6.3999e-05    * T3   +
    -0.00544655   * T2*d +
    0.06826       * T*d2 +
    -0.0388278    * d3   +
    -2.00971e-07  * T4   + 
    1.9044e-05    * T3*d +
    -0.000313217  * T2*d2+
    0.000418985   * T*d3 +
    -0.00627964   * d4   +
    2.40173e-10   * T4*T +
    -2.46583e-08  * T4*d +
    4.75607e-07   * T3*d2+
    -1.08313e-06  * T2*d3+
    2.28446e-05   * T*d4 +
    0.000108603   * d*d4;
  
  return T_akt;
}


///---------------------------- E_loss_in_Air ---------------------------------

/** I ran the Bethe-Bloch energy loss calculation (E_full_BeBl)
    for energy loss of protons in Nitrogen (1.3 mg/cm^3)
    with various (100-250cm) thickness and for kinetic proton 
    energies betw. 75 and 275 MeV. Then i made a 2D fit to the results.

    This function gives the kin. energy (in MeV) of protons 
    after the Nitrogen for given 
               T    (MeV,    kin. energy at the entrance)
               d    (cm,     path length through the Nitrogen)
 
    The function is only valid for 75 MeV <= T_kin <= 275 MeV and 
    for 100 cm < d < 250 cm !!
*/


double E_loss_in_Air(double T,       /// Initial kin. energy (MeV)
		     double d)       /// path length in the nitrogen (cm)
{
  if (T <  75) T =  75;
  if (T > 275) T = 275;
  if (d < 100) d = 100;
  if (d > 250) d = 250;

  double T_akt;
  double d2 = d*d;
  double d3 = d2*d;
  double d4 = d3*d;
  double T2 = T*T;
  double T3 = T2*T;
  double T4 = T3*T;

  T_akt = -0.678814      +     
    1.0236       * T     +
    -0.0234935   * d     +
    -0.000309939 * T2    +
    0.00028087   * T*d   +
    -2.18614e-07 * d2    +
    1.93732e-06  * T3    +
    -1.77498e-06 * T2*d  +
    2.234e-08    * T*d2  +
    -8.99602e-09 * d3    +
    -5.79446e-09 * T4    + 
    5.41678e-09  * T3*d  +
    -1.24622e-10 * T2*d2 +
    1.35552e-11  * T*d3  +
    2.47416e-11  * d4    +
    6.66763e-12  * T4*T  +
    -6.38132e-12 * T4*d  +
    2.20046e-13  * T3*d2 +
    -1.97975e-14 * T2*d3 +
    -1.03616e-14 * T*d4  +
    -2.82056e-14 * d*d4;
  
  return T_akt;
}


///---------------------------- E_loss_in_NE102 -------------------------------

/** I ran the Bethe-Bloch energy loss calculation (E_full_BeBl) 
    for energy loss of protons in "Carbon" of a density of 1.165 mg/cm^3,
    which is a good approximation for the scintillator material NE 102
    with various (1.3-3.3cm) thickness and for kinetic proton 
    energys betw. 75 and 275 MeV. Then I made a 2D fit to the results.

    This function gives the kin. energy (in MeV) of protons 
    after the Scintillator for given 
                T    (MeV,    kin. energy at the entrance)
                d    (cm,     path length in the scintillator)
  
    The function is only valid for 75 MeV <= T_kin <= 275 MeV and 
    for 1.3 cm < d < 3.3 cm !!
*/

double E_loss_in_NE102(double T,       /// Initial kin. energy (MeV)
		       double d)       /// path length in the scintillator (cm)
{
  if (T <  75) T =  75;
  if (T > 275) T = 275;
  if (d < 1.3) d = 1.3;
  if (d > 3.3) d = 3.3;

  double T_akt;
  double d2 = d*d;
  double d3 = d2*d;
  double d4 = d3*d;
  double T2 = T*T;
  double T3 = T2*T;
  double T4 = T3*T;

  T_akt =   -12.4057       +     
    1.47853        * T     +
    -27.1508       * d     +
    -0.00709161    * T2    +
    0.446048       * T*d   +
    -3.09274       * d2    +
    4.98646e-05    * T3    +
    -0.00362562    * T2*d  +
    0.0424844      * T*d2  +
    0.227429       * d3    +
    -1.67151e-07   * T4    + 
    1.40909e-05    * T3*d  +
    -0.000294943   * T2*d2 +
    0.00491351     * T*d3  +
    -0.175779      * d4    +
    2.12392e-10    * T4*T  +
    -1.97554e-08   * T4*d  +
    4.39253e-07    * T3*d2 +
    3.63562e-06    * T2*d3 +
    -0.000677075   * T*d4  +
    0.0276222      * d*d4;
  
  return T_akt;
}

// (pre declaration because call by next func):
double AnapowMcNauL(double T,    // Kin. Energy (MeV, center of the carbon)
		    double th);  // Scattering angle in the carbon (deg)

/// ---------------------------- AnapowMcNauH --------------------------------
/** 
    This routine calculates the effective proton carbon analyzing power A_c
    following the parametrization of McNaughton et al. (NIM A241 (1985) 
    435-440) for the "high energy" fit (T = 450-750 MeV).
*/


double AnapowMcNauH(double T,    /// Kin. Energy (MeV, center of the carbon)
		    double th)   /// Scattering angle in the carbon (deg)
{
  if (T < 450) return AnapowMcNauL(T,th);
  if (T > 750) T = 750;

  double p, a, b, c, d, r, r2, Ac, p1, p2, p3;
  const double M = 938.27;

  p    = 0.001 * sqrt(T*T+2*M*T);       // p in GeV here !
  p1   = p - 1.2;
  p2   = p1 * p1;
  p3   = p2 * p1;
  r    = p * sin(th * 0.017453293);
  r2   = r * r;

  a =   1.6575  +  1.3855 *p1  +   9.7700 *p2  -   149.27 *p3;
  b =  -16.346  +  152.53 *p1  +   139.16 *p2  -   3231.1 *p3;
  c =   1052.2  -  3210.8 *p1  -   2293.3 *p2  +  60327.0 *p3;
  d =  0.13887  - 0.19266 *p1  -  0.45643 *p2  +   8.1528 *p3;

  Ac = (a * r) / ( 1 + b*r2 + c*r2*r2 ) + d * p * sin(5 *  0.017453293 * th);

  return Ac;
}

/// ---------------------------- AnapowMcNauL --------------------------------
/** 
    This routine calculates the effective proton carbon analyzing power A_c
    following the parametrization of McNaughton et al. (NIM A241 (1985) 
    435-440) for the "low energy" fit (T = 100-450 MeV).
*/


double AnapowMcNauL(double T,    /// Kin. Energy (MeV, center of the carbon)
		    double th)   /// Scattering angle in the carbon (deg)
{
  if (T < 100) T = 100;
  if (T > 450) return AnapowMcNauH(T,th);

  double p, a, b, c, r, r2, Ac, p1, p2, p3, p4;
  const double M = 938.27;

  p    = 0.001 * sqrt(T*T+2*M*T);       // p in GeV here !
  p1   = p - 0.7;
  p2   = p1 * p1;
  p3   = p2 * p1;
  p4   = p3 * p1;
  r    = p * sin(th * 0.017453293);
  r2   = r * r;

  a =  5.3346  - 5.5361 *p1  +  2.8353 *p2  +  61.915 *p3  -   145.54 *p4;
  b = -12.774  - 68.339 *p1  +  1333.5 *p2  -  3713.5 *p3  +   3738.3 *p4;
  c =  1095.3  + 949.50 *p1  - 28012.0 *p2  + 96833.0 *p3  - 118830.0 *p4;

  Ac = (a * r) / ( 1 + b*r2 + c*r2*r2 );

  return Ac;
}

/// ---------------------------- ExtAnapowMcNauL ------------------------------
/** 
    For the theta (scattering angle) range between 0 (5) and 18.5 deg,
    this routine calculates the effective proton carbon analyzing power A_c
    following the parametrization of McNaughton et al. (NIM A241 (1985) 
    435-440) for the "low energy" fit (T = 100-450 MeV).

    For the theta-region above 18.5 deg and T below 210 MeV the 
    analysing power is calculated by a 2D-fit (4th order in th, 2nd order in 
    T) to the ST2-ST4ff data, where the analysing power was determined by 
    measuring the asymmetries relative to the 7-20 deg region.
    The fit is only valid for 140 MeV < T  < 210 MeV  !!!
                          and  18 deg < th <  40 deg  !!!
 
                                                      T.Pospischil, 29.7.98 
*/


double ExtAnapowMcNauL(double T,    /// Kin. Energy (MeV, center of the carbon)
		       double th)   /// Scattering angle in the carbon (deg)
{
  if ((th < 18.5) || (T>210)) return AnapowMcNauL(T,th);
  if (th > 43) return 0;
  if (T < 100) T = 100;

  double Apc;
  double th2 = th*th;
  double th3 = th2*th;
  double th4 = th3*th;
  double T2  = T*T;

  Apc = -18.5902            +
    1.47447      *      th  +  
    -0.0184451   *      th2 +
    -0.00084808  *      th3 +
    1.66656e-05  *      th4 +
    0.162901     * T        +
    -0.00897434  * T  * th  +
    -0.000186479 * T  * th2 +
    1.81209e-05  * T  * th3 +
    -2.56371e-07 * T  * th4 +
    -0.00034948  * T2       +
    1.18913e-05  * T2 * th  +
    1.10527e-06  * T2 * th2 +
    -6.23088e-08 * T2 * th3 +
    8.07095e-10  * T2 * th4;

  if (Apc>0) return Apc;
  return 0;
}
// ==========================================================================


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

int VdcToHdc(FocalCoord& Vdc,  /// INPUT : VDC coordinates
	     double xt,        /// x shift of the origin in the VDC system
	     double yt,        /// y shift of the origin in the VDC system
	     double zt,        /// z shift of the origin in the VDC system
	     double alpha,     /// rotation of the HDC system around the y axis
	     double beta,      /// rotation of the HDC system around the x axis
	     double gamma,     /// rotation of the HDC system around the z axis
	     FocalCoord& Hdc,  /// OUTPUT: HDC coordinates
	     double& r)        /// particle's path from VDC to HDC
{          
  double cos_al = cos(alpha/1000);
  double sin_al = sin(alpha/1000);
  double cos_be = cos(beta/1000);
  double sin_be = sin(beta/1000);
  double cos_ga = cos(gamma/1000);
  double sin_ga = sin(gamma/1000);

  double cos_th = cos(Vdc.th/1000);
  double sin_th = sin(Vdc.th/1000);
  double cos_ph = cos(Vdc.ph/1000);
  double sin_ph = sin(Vdc.ph/1000);
  double tan_th = sin_th / cos_th;
  double tan_ph = sin_ph / cos_ph;

  double c = cos_al * cos_be - sin_be * tan_ph + cos_be * sin_al * tan_th;
  double ic = 1.0 / c;

  double dx = Vdc.x - xt;
  double dy = Vdc.y - yt;
  double lambda = (zt*cos_al*cos_be - dx*cos_be*sin_al + dy*sin_be) * ic;
  double dz = lambda - zt;
  r = lambda * sqrt(1 + tan_th * tan_th + tan_ph * tan_ph);

  double xth = dx + lambda * tan_th;
  double yph = dy + lambda * tan_ph;

  double mx1 = -cos_ga * sin_al + cos_al * sin_be * sin_ga;
  double mx2 =  cos_be * sin_ga;
  double mx3 =  cos_al * cos_ga + sin_al * sin_be * sin_ga;

  double my1 =  cos_al * cos_ga * sin_be + sin_al * sin_ga;
  double my2 =  cos_be * cos_ga;
  double my3 =  cos_ga * sin_al * sin_be - cos_al * sin_ga;

  Hdc.x = mx1 * dz + mx2 * yph + mx3 * xth;
  Hdc.y = my1 * dz + my2 * yph + my3 * xth;

  Hdc.th = 1000 * atan((mx1 + mx2 * tan_ph + mx3 * tan_th) * ic);
  Hdc.ph = 1000 * atan((my1 + my2 * tan_ph + my3 * tan_th) * ic);

  return 0;
}


/// --------------------------------------------------------- PPolFillOnline:
/** PPolFillOnline:
    This function works on the online structure and calculates:
    -  the coordinates online.ppolA.vth (displacement and rotation of the HDC 
       system against the VDC system is determined by the database run.db)
       out of the coordinates online.A.x, y, theta and phi by the routine 
       VdcToHdc
    -  the coordinates dvh by substraction of online.ppolA.hdc from 
       online.ppolA.vth
    -  the coordinates online.ppol.scat, representing the coordinates of the
       point of scattering (in the carbon or anywhere else), the minimum 
       distance dmin of the VDC trace to the HDC trace as well as  the 
       azimutalhal and polar angle of the scattering. */


int PPolFillOnline(AquaTree *out, struct online *onl)
{ 
  // +++++++++ Transformation of VDC coordinates into HDC frame:
  double dis_bet_DC;
  FocalCoord VdcAtmp;

  if (!(onl->A.vdcOK)) return 1;

  VdcAtmp.x  = onl->A.x;
  VdcAtmp.y  = onl->A.y;
  VdcAtmp.th = onl->A.theta;
  VdcAtmp.ph = onl->A.phi;
  VdcToHdc(VdcAtmp,              // INPUT : Coordinates in the VDC
	   rundb.ppolA.hdc_x_offs,  // x coord. of HDC origin in the VDC-system
	   rundb.ppolA.hdc_y_offs,  // y coord. of HDC origin in the VDC-system
	   rundb.ppolA.hdc_z_offs,  // z coord. of HDC origin in the VDC-system
	   rundb.ppolA.hdc_theta_offs, // rot. of HDC-system ar. y-axis (mrad)
	   rundb.ppolA.hdc_phi_offs,   // rot. of HDC-system ar. x-axis (mrad)
	   rundb.ppolA.hdc_psi_offs,   // rot. of HDC-system ar. z-axis (mrad)
	   onl->ppolA.vth,     // OUTPUT: coordinates VDC -> HDC
	   dis_bet_DC);             // particle's path from VDC to HDC

  onl->ppolA.r = dis_bet_DC;

  // +++++++++++++++++++++++++++++implementation of the scattering process:

  if (!(onl->ppolA.hdcOK)) return 2;

  // Declaration of variables:
  double xv_xh, yv_yh;
  double s, t, a, b, c, d, a2, b2, c2, d2, ab, cd;
  double xd, yd, zd;
  double phdr, cos_phdr, Gs, Gc;
  double abs_v, abs_h, abs_d_2;

  // reservation of variablef fro calculation of s and t:
  xv_xh  = onl->ppolA.vth.x - onl->ppolA.hdc.x;
  yv_yh  = onl->ppolA.vth.y - onl->ppolA.hdc.y;
  a = tan(onl->ppolA.hdc.th/1000.0);
  b = tan(onl->ppolA.vth.th/1000.0);
  c = tan(onl->ppolA.hdc.ph/1000.0);
  d = tan(onl->ppolA.vth.ph/1000.0);
  a2 = a * a;
  b2 = b * b;
  c2 = c * c;
  d2 = d * d;
  ab = a * b;
  cd = c * d;
  
  // Calculation of t and s (cf. Polarimeter-Logbook page 264):
  t = ((c2*b - a*cd + b - a)*xv_xh + (a2*d - ab*c + d - c)*yv_yh) /
    ( 2*(ab + ab*cd + cd) - (a2*d2 + c2*b2) - (a2 + b2 + c2 + d2) );
  s = ((1 + b2 + d2)*t + xv_xh*b + yv_yh*d) / (ab + cd +1);
  
  // Calculation of point of scattering:
  onl->ppolA.scat.x = 0.5*(onl->ppolA.vth.x+onl->ppolA.hdc.x+s*a+t*b);
  onl->ppolA.scat.y = 0.5*(onl->ppolA.vth.y+onl->ppolA.hdc.y+s*c+t*d);
  onl->ppolA.scat.z = 0.5*(s+t);

  // Calculation of minimum distance:
  xd = xv_xh + t*b - s*a;
  yd = yv_yh + t*d - s*c;
  zd = s - t;
  onl->ppolA.scat.dmin = sqrt( xd*xd + yd*yd + zd*zd ); 

  // Calculation of dvhA coordinates:
  onl->ppolA.dvh.x  = xv_xh;
  onl->ppolA.dvh.y  = yv_yh;
  onl->ppolA.dvh.th = onl->ppolA.vth.th - onl->ppolA.hdc.th;
  onl->ppolA.dvh.ph = onl->ppolA.vth.ph - onl->ppolA.hdc.ph;
      
  // calculation of scat coordinates (cf Polarimeter-Logbook page 277):
  // theta_scat (like acos(scalarproduct/amount)):
  abs_v = sqrt(b2+d2+1);
  abs_h = sqrt(a2+c2+1);
  onl->ppolA.scat.th = 1000*acos( (ab+cd+1)/(abs_v*abs_h) );
  // phi_scat (like atan2(H'y/H'x),  H':final proton in system of initial one):
  phdr = acos(1/abs_v);
  if (phdr < 0.000001) onl->ppolA.scat.ph = 1000*atan2(c,a);
  else {
    cos_phdr = cos(phdr);
    abs_d_2 = (d2 + b2);
    Gs = sin(phdr)/sqrt(abs_d_2);
    Gc = (b*c - a*d)*(1-cos_phdr)/abs_d_2;
    onl->ppolA.scat.ph = 1000*atan2( ( b*Gc + c*cos_phdr - d*Gs),
				       (-d*Gc + a*cos_phdr - b*Gs)  );
  }

  // ----- Calculation of analyzing power:

  // momentum (MeV/c) and kin. energy (MeV) in the focal plane:
  double p_fp = (rundb.A.momentum / 1.0476203) * (1.0+onl->A.target.dp*0.01);
  double T_fp = sqrt( 938.27 * 938.27  +  p_fp * p_fp ) - 938.27;

    // Calculate eff path length in the scint (for energy loss there)
  double tan_vdc_th  = tan(onl->A.theta/1000.0);
  double tan_vdc_ph  = tan(onl->A.phi/1000.0);
  double d_scint_eff = 1.3*sqrt(tan_vdc_th*tan_vdc_th+tan_vdc_ph*tan_vdc_ph+1);

  // Calculate eff path length in the carbon (for energy loss there)
  double d_carbon_eff = rundb.ppolA.CarbonThickness * sqrt(b2+d2+1);

  // Kin. energy in the middle of the carbon analyzer
  onl->ppolA.T_cc =
    E_mid_Carbon(E_loss_in_Air(E_loss_in_NE102(T_fp, d_scint_eff),
			       (onl->ppolA.r/10.0) ),
		 d_carbon_eff );

  // calc Analyzing power:
  onl->ppolA.anapow = ExtAnapowMcNauL(onl->ppolA.T_cc, 
				       onl->ppolA.scat.th * 0.05729578);

  // Cone-Test:
  onl->ppolA.ConeTest = ConeTest(onl);

  // To make Cola generating the histograms: 
  out->packEventData(&onl->ppolA.vth.x,    4);
  out->packEventData(&onl->ppolA.dvh.x,    4);
  out->packEventData(&onl->ppolA.scat.x,   6);
  out->packEventData(&onl->ppolA.r,        3);
  out->packEventData(&onl->ppolA.ConeTest, 1);

  return 0;
}

double GenerateFPPTheta(double protonMomentum)
{
  static normal rnd(new PseudoRandom(), new PseudoRandom());
  double m=0, s=0, res=0;

  if      (protonMomentum<=0.530) {m = 15.776; s = 8.6512;}
  else if (protonMomentum<=0.550) {m = 15.776; s = 8.6512;}
  else if (protonMomentum<=0.570) {m = 14.762; s = 8.2196;}
  else if (protonMomentum<=0.590) {m = 13.967; s = 9.0911;}
  else if (protonMomentum<=0.610) {m = 13.004; s = 10.460;}
  else if (protonMomentum<=0.630) {m = 14.046; s = 10.715;}
  else if (protonMomentum<=0.650) {m = 15.152; s = 12.456;}
  else                            {m = 17.615; s = 12.055;};
  while (res<7 || res>25) res = m + s* rnd();
  return res/180*M_PI; 
}

double Apow(double proton, double theta) //GeV
{
  double T = proton - 0.93827203;//proton.energy() - proton.mass();

  if (T <  0.450 && T>=0.100)  return AnapowMcNauL(T*1e3,theta/M_PI*180);
  else if (T >= 0.450)         return AnapowMcNauH(T*1e3,theta/M_PI*180);
  else ;//cerr << "Proton Energy too low or to high: T = " 
	//    << T << " Th = " << theta << endl;
  return 0;
}









