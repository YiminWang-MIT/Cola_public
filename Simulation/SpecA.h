//
// $Id: SpecA.h 2216 2008-06-13 21:13:47Z distler $
//

#ifndef __SPECA_H__
#define __SPECA_H__

// +++++++++++++++++++ Description of spectrometer A: +++++++++++++++++++++++++
//
//   V1.0, T.Pospischil, 22.01.98
//

// =============================== Quadrupole ================================
double Q_FringeFall[6] = {0.10120, 6.5287, -1.5947, 2.2773, 0.496, 0.0};
double Q_Fields[6] = {0.0, -0.51615, 0.0, 0.04405, 0.0, 0.00339};
//double Q_Fields[6] = {0.0, -0.51615, 0.0, 0.0, 0.0, 0.0};
//double Q_Fields[6] = {0.0, -0.529043, 0.0, 0.04515, 0.0, 0.00409};//setRefMom
Multipol Quad(Q_Fields,        // Multipole field strength (Tesla)
	      200.0,           // Radius of (quad) aperture (mm)
	      1016.0,          // Effective length of the multipole (mm)
	      Vector3D(0,0,792), // Origin of the MCS in the ACS (mm)
	      0.0,             // Angle of MCS against ACS in zx-plane (deg)
	      -400.0,          // Effective x-range min of the multipole
	      400.0,           // Effective x-range max of the multipole
	      Q_FringeFall,    // Coeff of entrance fringing field fall off
	      -500.0,          // Min. of z-Range of Entrance Boundary
	      500.0,           // Max. of z-Range of Entrance Boundary
	      Q_FringeFall,    // Coefficients of exit fringing field fall off
	      -500.0,          // Min. of z-Range of Exit Boundary
	      500.0,           // Max. of z-Range of Exit Boundary
	      1.0,             // ff fall off hexapole radius rel. to R
	      1.0,             // ff fall off octapole radius rel. to R
	      1.0,             // ff fall off decapole radius rel. to R
	      1.0,             // ff fall off dodecapole rad. rel. to R
	      0.0,             // hexapole EFB inward displacem. (DSH = dz/2R) 
	      0.254,           // rel. octapole EFB inward displacement 
	      0.0,             // rel. decapole EFB inward displacement 
	      0.5);            // rel. dodecapole EFB inward displacement

// =============================== Sextupole =================================
double S_FringeFall[6] = {-0.03358,9.2438,-1.9596,-5.2344,-12.2224,60.3104};
//double S_Fields[6] = {0.0, 0.0, 0.33179, 0.0, 0.0, 0.0};
double S_Fields[6] = {0.0, 0.0, 0.33179, 0.0, 0.02651, 0.0};
Multipol Sext(S_Fields,        // Multipole field strength (Tesla)
	      400.0,           // Radius of (quad) aperture (mm)
	      470.0,           // Effective length of the multipole (mm)
	      Vector3D(0,0,2115), // Origin of the MCS in the ACS (mm)
	      0.0,             // Angle of MCS against ACS in zx-plane (deg)
	      -600,            // Effective x-range min of the multipole
	      600,             // Effective x-range max of the multipole
	      S_FringeFall,    // Coeff of entrance fringing field fall off
	      -600.0,          // Min. of z-Range of Entrance Boundary
	      500.0,           // Max. of z-Range of Entrance Boundary
	      S_FringeFall,    // Coefficients of exit fringing field fall off
	      -600.0,          // Min. of z-Range of Exit Boundary
	      500.0,           // Max. of z-Range of Exit Boundary
	      0.8,             // ff fall off hexapole radius rel. to R
	      0.8,             // ff fall off octapole radius rel. to R
	      0.8,             // ff fall off decapole radius rel. to R
	      0.8,             // ff fall off dodecapole rad. rel. to R
	      0.0,             // hexapole EFB inward displacem. (DSH = dz/2R) 
	      0.0,             // rel. octapole EFB inward displacement 
	      0.0,             // rel. decapole EFB inward displacement 
	      0.0);            // rel. dodecapole EFB inward displacement

// =============================== Dipole 1 ===================================
double D_FringeFall[6] = {0.148, 1.8674, -0.3337, 0.6745, -0.1445, 0.0426};
double D1EnBound[9] = {0, 0, 0.416, -0.502, 0.03, 0.225, 0, 0, 0};
double D1ExBound[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
Dipol D1(1.50101,  // Homogenious Field By (Tesla)
	 1400,     // Radius of curvature (mm)
	 200,      // Gap width (mm)
	 Vector3D(0,0,3050),        // Origin of the EnCS in the ACS (mm)
	 169,      // Angle of EnCS against ACS in zx-plane ( 180 + 11 deg)
	 D1EnBound,// entrance boundary curvature polynom coefficients
	 D_FringeFall, // Coefficients of entrance fringing field fall off
	 -750,     // Min. of x-Range of Entrance Boundary
	 1000,     // Max. of x-Range of Entrance Boundary
	 -550,     // Min. of z-Range of Entrance Boundary
	  500,     // Max. of z-Range of Entrance Boundary
	 Vector3D(-597,0,4196.8),   // Origin of the ExCS in the ACS (mm)
	 33,       // Angle of ExCS against ACS in zx-plane (      -33 deg)
	 D1ExBound,// exit boundary curvature polynom coefficients
	 D_FringeFall, // Coefficients of exit fringing field fall off
	 -800,     // Min. of x-Range of Exit Boundary
	 900,      // Max. of x-Range of Exit Boundary
	 -550,     // Min. of z-Range of Exit Boundary
	 750);     // Max. of z-Range of Exit Boundary

// =============================== Dipole 2 ===================================
double D2EnBound[9] = {0, 0, -0.062, 0.063, 0.095, 0.01, 0, 0, 0};
double D2ExBound[9] = {0, 0, -0.086, 0.152, -0.18, 0.15, -0.085, 0.015, 0};
Dipol D2(1.50101,  // Homogenious Field By (Tesla)
	 1400,     // Radius of curvature (mm)
	 200,      // Gap width (mm)
	 Vector3D(-2071.4,0,5229.2), // Origin of the EnCS in the ACS (mm)
	 -83.4,     // Angle of EnCS against ACS in zx-plane ( 90 - 6.6 deg)
	 D2EnBound, // entrance boundary curvature polynom coefficients
	 D_FringeFall, // Coefficients of entrance fringing field fall off
	 -1600,     // Min. of x-Range of Entrance Boundary
	 1000,      // Max. of x-Range of Entrance Boundary
	 -550,      // Min. of z-Range of Entrance Boundary
	 750,       // Max. of z-Range of Entrance Boundary
	 Vector3D(-3120,0,5460.7),   // Origin of the ExCS in the ACS (mm)
	 114,       // Angle of ExCS against ACS in zx-plane (-90 - 24 deg)
	 D2ExBound, // exit boundary curvature polynom coefficients
	 D_FringeFall, // Coefficients of exit fringing field fall off
	 -800,      // Min. of x-Range of Exit Boundary
	 1800,      // Max. of x-Range of Exit Boundary
	 -550,      // Min. of z-Range of Exit Boundary
	 750);      // Max. of z-Range of Exit Boundary
 

#endif
