#include "Colli.h"
#include <cstdio>

A15::A15()
{
  name = "15msr";
  //  Plane_h[0] = Plane_v[0] = new colliPlane(565.5 +  0, 43.55, 28.3);
  Plane_h[0] = Plane_v[0] = new colliPlane(565.5 +  0, 29.63, 42.73);
  Plane_h[1] =              new colliPlane(565.5 + 16, 29.77,  0.00);
  Plane_h[2] =              new colliPlane(565.5 + 32, 30.25,  0.00);
  Plane_h[3] =              new colliPlane(565.5 + 48, 31.01,  0.00);
  Plane_h[4] =              new colliPlane(565.5 + 64, 32.20,  0.00);
  Plane_h[5] = Plane_v[3] = new colliPlane(565.5 + 80, 33.69, 48.74);
  Plane_v[1] =              new colliPlane(565.5 + 26,  0.00, 44.45);
  Plane_v[2] =              new colliPlane(565.5 + 54,  0.00, 46.56);
  init = -1;
}

int 
A15::accepted(double x0[3], FourVector p, double theta, double oop, double)
{
  if (init) {
    for(int i=init=0; i<6; i++) Plane_h[i]->init(theta, oop);
    for(int i=0;      i<4; i++) Plane_v[i]->init(theta, oop);
  }
  if (!Plane_v[0]->checkVertical  (x0, p)) return 0;
  if (!Plane_h[0]->checkHorizontal(x0, p)) return 0;
  for(int i=1; i<4; i++) if (!Plane_v[i]->checkVertical  (x0, p)) return 0;
  for(int i=1; i<6; i++) if (!Plane_h[i]->checkHorizontal(x0, p)) return 0;
  return 1;
}

A21::A21()
{
  name = "21msr";
  Plane_h[0] = Plane_v[0] = new colliPlane(565.5 +  0, 43.55, 39.78);
  Plane_h[1] =              new colliPlane(565.5 + 16, 44.08,  0.00);
  Plane_h[2] =              new colliPlane(565.5 + 32, 44.94,  0.00);
  Plane_h[3] =              new colliPlane(565.5 + 48, 46.16,  0.00);
  Plane_h[4] =              new colliPlane(565.5 + 64, 47.69,  0.00);
  Plane_h[5] = Plane_v[3] = new colliPlane(565.5 + 80, 49.56, 45.37);
  Plane_v[1] =              new colliPlane(565.5 + 26,  0.00, 41.36);
  Plane_v[2] =              new colliPlane(565.5 + 54,  0.00, 43.33);
  init = -1;
}

int 
A21::accepted(double x0[3], FourVector p, double theta, double oop, double)
{
  if (init) {
    for(int i=init=0; i<6; i++) Plane_h[i]->init(theta, oop);
    for(int i=0;      i<4; i++) Plane_v[i]->init(theta, oop);
  }
  if (!Plane_v[0]->checkVertical  (x0, p)) return 0;
  if (!Plane_h[0]->checkHorizontal(x0, p)) return 0;
  for(int i=1; i<4; i++) if (!Plane_v[i]->checkVertical  (x0, p)) return 0;
  for(int i=1; i<6; i++) if (!Plane_h[i]->checkHorizontal(x0, p)) return 0;
  return 1;
}

A28::A28()
{
  name = "28msr";
  Contour[0] = new colliContour(565.5+ 0., 54.14+0.  , 42.73+0.  ,88.29);
  Contour[1] = new colliContour(565.5+26., 54.14+2.22, 42.73+1.72,92.03);
  Contour[2] = new colliContour(565.5+54., 54.14+4.89, 42.73+3.83,96.42);
  Contour[3] = new colliContour(565.5+80., 54.14+7.57, 42.73+6.01,100.82);
  init = -1;
}

int 
A28::accepted(double x0[3], FourVector p, double theta, double oop, double)
{
  if (init) {
    for(int i=init=0; i<4; i++) Contour[i]->init(theta-0.09*M_PI/180, oop);
  }
  if (!Contour[0]->checkContour(x0, p)) return 0;
  for (int i=1; i<4; i++) if (!Contour[i]->checkContour(x0, p)) return 0;
  return 1;
}

//////////////////////////////////////////////////////////////////////////////
// at the moment we check only the first collimator plane for B.
// The actual geometry is four planes (horizontal and vertical)
// Planes at 1:0 mm, 2:33 mm, 3:67 mm, 4:100mm
// The angles between the lines 1-2 and 2-3 and 2-3, 3-4:
// horizontal: 0.5°
// vertical:   0.2° 
/*
 * Particle: ->    ___
 *                /   \ 
 *               |_____|
 *               1 2 3 4
 */
 
Bcolli::Bcolli(double hor, double vert) 
{
  if (hor== 0.020 && vert == 0.070) name = "5.6msr";
  else sprintf(name = new char[15] ,"%.2fmsr", hor * vert * 4000);
  
  vertPlane = new colliPlane(3368.0, 0, 3368.0 * tan(vert) );
  horPlane  = new colliPlane(3144.0, 3144.0 * tan(hor),  0 );
  init = 1;
}

Bcolli::Bcolli(double horL, double horR, double vertT, double vertB) 
{
  sprintf(name = new char[50] ,"(%.1f+%.1f)x(%.1f+%.1f)musr", 
	  horL*1000, horR*1000, vertT*1000, vertB*1000);
  
  vertPlane = new colliPlane(3368.0, 0, 0, 
			     3368.0 * tan(vertT), 3368.0 * tan(vertB) );
  horPlane  = new colliPlane(3144.0, 3144.0 * tan(horL), 3144.0 * tan(horR),  
			     0, 0 );
  init = 1;
}

int 
Bcolli::accepted(double x0[3], FourVector p, double theta, double oop, double)
{
  if (init) {
    init = 0;
    vertPlane->init(theta, oop);
    horPlane->init(theta, oop);
  }

  if (!vertPlane->checkVertical(x0, p)) return 0;
  return horPlane->checkHorizontal(x0, p);
}


MXcolli::MXcolli(double hor, double vert, double collidist) 
{
  if (hor== 0.05 && vert == 0.05) name = "MX10.0msr";
  else sprintf(name = new char[15] ,"%.2fmsr", hor * vert * 4000);
  
  vertPlane = new colliPlane(collidist, 0,                        collidist * tan(vert) );
  horPlane  = new colliPlane(collidist, collidist * tan(hor),  0                     );
  init = 1;
}

int 
MXcolli::accepted(double x0[3], FourVector p, double theta, double oop, double)
{
  if (init) {
    init = 0;
    vertPlane->init(theta, oop);
    horPlane->init(theta, oop);
  }

  if (!vertPlane->checkVertical(x0, p)) return 0;
  return horPlane->checkHorizontal(x0, p);
}

HES::HES() 
{
  name = "HES";

  // HES y' at target:   30 ...  90 mrad
  // HES x' at target: -200 ... 200 mrad
  // --> Omega = 24 msr
  // DeltaPhi = +- 11.4 deg
  // DeltaTheta = +- 1.7 deg

  vertPlane = new colliPlane(3368.0, 0, 3368.0 * tan(0.030) );
  horPlane  = new colliPlane(3144.0, 3144.0 * tan(0.200),  0 );
  init = 1;
}

int 
HES::accepted(double x0[3], FourVector p, double theta, double oop, double)
{
  if (init) {
    init = 0;
    vertPlane->init(theta, oop);
    horPlane->init(theta, oop);
  }

  if (!vertPlane->checkVertical(x0, p)) return 0;
  return horPlane->checkHorizontal(x0, p);
}

C225::C225()
{
  name = "22.5msr";
  Plane_h[0] = new colliPlane(453.0 +  0., 36.04 + 0.00, 0.0);
  Plane_h[1] = new colliPlane(453.0 + 10., 36.04 + 0.23, 0.0);
  Plane_h[2] = new colliPlane(453.0 + 20., 36.04 + 0.72, 0.0);
  Plane_h[3] = new colliPlane(453.0 + 30., 36.04 + 1.47, 0.0);
  Plane_h[4] = new colliPlane(453.0 + 40., 36.04 + 2.48, 0.0);
  Plane_h[5] = new colliPlane(453.0 + 50., 36.04 + 3.76, 0.0);

  Plane_v[0] = new colliPlane(453.0 +  0., 0.00, 34.32 + 0.00);
  Plane_v[1] = new colliPlane(453.0 + 16., 0.00, 34.32 + 1.03);
  Plane_v[2] = new colliPlane(453.0 + 34., 0.00, 34.32 + 2.39);
  Plane_v[3] = new colliPlane(453.0 + 50., 0.00, 34.32 + 3.76);
  init = -1;
}

int 
C225::accepted(double x0[3], FourVector p, double theta, double oop, double)
{
  if (init) {
    for(int i=init=0; i<6; i++) Plane_h[i]->init(theta, oop);
    for(int i=0;      i<4; i++) Plane_v[i]->init(theta, oop);
  }
  if (!Plane_v[0]->checkVertical  (x0, p)) return 0;
  if (!Plane_h[0]->checkHorizontal(x0, p)) return 0;
  for(int i=1; i<4; i++) if (!Plane_v[i]->checkVertical  (x0, p)) return 0;
  for(int i=1; i<6; i++) if (!Plane_h[i]->checkHorizontal(x0, p)) return 0;
  return 1;
}

C28::C28()
{
  name = "28msr";
  Contour[0] = new colliContour(453.0+ 0., 43.24+0.  , 34.49+0.  , 70.53);
  Contour[1] = new colliContour(453.0+16., 43.24+1.37, 34.49+1.06, 73.03);
  Contour[2] = new colliContour(453.0+34., 43.24+3.08, 34.49+2.44, 76.06);
  Contour[3] = new colliContour(453.0+50., 43.24+4.73, 34.49+3.78, 78.98);
  init = -1;
}

int 
C28::accepted(double x0[3], FourVector p, double theta, double oop, double)
{
  if (init) {
    for(int i=init=0; i<4; i++) Contour[i]->init(theta, oop);
  }
  if (!Contour[0]->checkContour(x0, p)) return 0;
  for (int i=1; i<4; i++) if (!Contour[i]->checkContour(x0, p)) return 0;
  return 1;
}

D7::D7()
{
  name = "7msr";
  Plane_h[0] = Plane_v[0] = new colliPlane(441. +  0,  8., 42.9);
  Plane_h[1] = Plane_v[1] = new colliPlane(441. + 45, 10., 50.);
  init = -1;
}

int 
D7::accepted(double x0[3], FourVector p, double theta, double oop, double)
{
  if (init) {
    for(int i=init=0; i<2; i++) Plane_h[i]->init(theta, oop);
    for(int i=0;      i<2; i++) Plane_v[i]->init(theta, oop);
  }
  if (!Plane_v[0]->checkVertical  (x0, p)) return 0;
  if (!Plane_h[0]->checkHorizontal(x0, p)) return 0;
  for(int i=1; i<2; i++) if (!Plane_v[i]->checkVertical  (x0, p)) return 0;
  for(int i=1; i<2; i++) if (!Plane_h[i]->checkHorizontal(x0, p)) return 0;
  return 1;
}

D4::D4()
{
  name = "4msr";
  Plane_h[0] = Plane_v[0] = new colliPlane(561. +  0,  8., 42.9);
  Plane_h[1] = Plane_v[1] = new colliPlane(561. + 45, 10., 50.);
  init = -1;
}

int 
D4::accepted(double x0[3], FourVector p, double theta, double oop, double)
{
  if (init) {
    for(int i=init=0; i<2; i++) Plane_h[i]->init(theta, oop);
    for(int i=0;      i<2; i++) Plane_v[i]->init(theta, oop);
  }
  if (!Plane_v[0]->checkVertical  (x0, p)) return 0;
  if (!Plane_h[0]->checkHorizontal(x0, p)) return 0;
  for(int i=1; i<2; i++) if (!Plane_v[i]->checkVertical  (x0, p)) return 0;
  for(int i=1; i<2; i++) if (!Plane_h[i]->checkHorizontal(x0, p)) return 0;
  return 1;
}

D2::D2()
{
  name = "2msr";
  Trapezoid[0] = new colliTrapezoid(561.0+ 0., 46.9, 3.6, 120.6);
  Trapezoid[1] = new colliTrapezoid(561.0+45., 52.0, 4.6, 170.86);
  init = -1;
}

int 
D2::accepted(double x0[3], FourVector p, double theta, double oop, double)
{
  if (init) {
    for(int i=init=0; i<2; i++) Trapezoid[i]->init(theta, oop);
  }
  if (!Trapezoid[0]->checkTrapezoid(x0, p)) return 0;
  for (int i=1; i<2; i++) if (!Trapezoid[i]->checkTrapezoid(x0, p)) return 0;
  return 1;
}

//////////////////////////////////////////////////////////////////////////////

OHIPS_simple::OHIPS_simple(double hor, double ver, double dist) {
  if ((hor==OHIPS_SHORT_HORIZONTAL) && (ver==OHIPS_SHORT_VERTICAL) &&
      (dist==OHIPS_SHORT_DISTANCE)) {
    name = "OHIPS-short";
  } else if ((hor==OHIPS_LONG_HORIZONTAL) && (ver==OHIPS_LONG_VERTICAL) &&
      (dist==OHIPS_LONG_DISTANCE)) {
    name = "OHIPS-long";
  } else sprintf(name = new char[21] ,"OHIPS-%.2fmsr", hor*ver*4000);

  verPlane = new colliPlane(dist, 0, dist*tan(ver));
  horPlane = new colliPlane(dist, dist*tan(hor), 0);
  init = 1;
}

int OHIPS_simple::accepted(double x0[3], FourVector p,
			  double theta, double oop, double)
{
  if (init) {
    init = 0;
    verPlane->init(theta, oop);
    horPlane->init(theta, oop);
  }
  return (verPlane->checkVertical(x0, p) && horPlane->checkHorizontal(x0, p));
}

OOPS_simple::OOPS_simple(double hor, double ver, double dist) {
  if ((hor==OOPS_SHORT_HORIZONTAL) && (ver==OOPS_SHORT_VERTICAL) &&
      (dist==OOPS_SHORT_DISTANCE)) {
    name = "OOPS-short";
  } else if ((hor==OOPS_LONG_HORIZONTAL) && (ver==OOPS_LONG_VERTICAL) &&
      (dist==OOPS_LONG_DISTANCE)) {
    name = "OOPS-long";
  } else sprintf(name = new char[21] ,"OOPS-%.2fmsr", hor*ver*4000);
  
  verPlane = new colliPlane(dist, 0, dist*tan(ver));
  horPlane = new colliPlane(dist, dist*tan(hor), 0);
  init = 1;
}

int OOPS_simple::accepted(double x0[3], FourVector p,
			  double theta, double oop, double)
{
  if (init) {
    init = 0;
    verPlane->init(theta, oop);
    horPlane->init(theta, oop);
  }
  return (verPlane->checkVertical(x0, p) && horPlane->checkHorizontal(x0, p));
}

//////////////////////////////////////////////////////////////////////////////

OHIPS_fancy::OHIPS_fancy(double hor, double ver, double dist) {
  if ((hor==OHIPS_SHORT_HORIZONTAL) && (ver==OHIPS_SHORT_VERTICAL) &&
      (dist==OHIPS_SHORT_DISTANCE)) {
    name = "OHIPS+short";
  } else if ((hor==OHIPS_LONG_HORIZONTAL) && (ver==OHIPS_LONG_VERTICAL) &&
      (dist==OHIPS_LONG_DISTANCE)) {
    name = "OHIPS+long";
  } else sprintf(name = new char[21] ,"OHIPS+%.2fmsr", hor*ver*4000);

  verPlane = new colliPlane(dist, 0, dist*tan(ver));
  horPlane = new colliPlane(dist, dist*tan(hor), 0);
  init = 1;
}

int OHIPS_fancy::accepted(double x0[3], FourVector p,
			  double theta, double oop, double refmom)
{
  if (init) {
    init = 0;
    refMom = refMom;
    verPlane->init(theta, oop);
    horPlane->init(theta, oop);
  }

  if (!verPlane->checkVertical(x0, p) ||
      !horPlane->checkHorizontal(x0, p)) return 0;

  // more tests have to go here
  return 1;
}

OOPS_fancy::OOPS_fancy(double hor, double ver, double dist) {
  if ((hor==OOPS_SHORT_HORIZONTAL) && (ver==OOPS_SHORT_VERTICAL) &&
      (dist==OOPS_SHORT_DISTANCE)) {
    name = "OOPS+short";
  } else if ((hor==OOPS_LONG_HORIZONTAL) && (ver==OOPS_LONG_VERTICAL) &&
      (dist==OOPS_LONG_DISTANCE)) {
    name = "OOPS+long";
  } else sprintf(name = new char[21] ,"OOPS+%.2fmsr", hor*ver*4000);
  
  verPlane = new colliPlane(dist, 0, dist*tan(ver));
  horPlane = new colliPlane(dist, dist*tan(hor), 0);
  init = 1;
}

int OOPS_fancy::accepted(double x0[3], FourVector p,
			  double theta, double oop, double refmom)
{
  if (init) {
    init = 0;
    refMom = refMom;
    verPlane->init(theta, oop);
    horPlane->init(theta, oop);
  }

  if (!verPlane->checkVertical(x0, p) ||
      !horPlane->checkHorizontal(x0, p)) return 0;

  // more tests have to go here
  return 1;
}

KAOScolli::KAOScolli() 
{
  name = "Lower";
  
  Plane1 = new colliPlane(1000-25, 226.5, 126.5, 50, 50);
  Plane2 = new colliPlane(1000+25, 226.5, 126.5, 50, 50);
  init = 1;
}

int 
KAOScolli::accepted(double x0[3], FourVector p, double theta, double oop, double)
{
  if (init) {
    init = 0;
    Plane1->init(theta, oop);
    Plane2->init(theta, oop);
  }

  if (!Plane1->checkVertical(x0, p)) return 0;
  if (!Plane1->checkHorizontal(x0, p)) return 0;
  if (!Plane2->checkVertical(x0, p)) return 0;
  return Plane2->checkHorizontal(x0, p);
}

HKS::HKS() 
{
  name = "HKS";
    
  vertPlane = new colliPlane(3368.0, 0, 3368.0 * tan(0.050) );
  horPlane  = new colliPlane(3144.0, 3144.0 * tan(0.090),  0 );
  init = 1;
}

int 
HKS::accepted(double x0[3], FourVector p, double theta, double oop, double)
{
  if (init) {
    init = 0;
    vertPlane->init(theta, oop);
    horPlane->init(theta, oop);
  }

  if (!vertPlane->checkVertical(x0, p)) return 0;
  return horPlane->checkHorizontal(x0, p);
}



//////////////////////////////////////////////////////////////////////////////

Collimator *ColliAList[] = {
  new A15, new A21, new A28, new allColli, new inPlaneColli,
  new MXcolli(0.05, 0.05, 800), //some quite arbitrary magix assumption; 800mm colli distance, 10msr 
  NULL
};

Collimator *ColliBList[] = {
  new Bcolli(0.020, 0.070), new Bcolli(0.018, 0.070), 
  new Bcolli(0.020, 0.020), new Bcolli(0.018, 0.020), 
  new Bcolli(0.020, 0.015), new Bcolli(0.018, 0.015), 
  new Bcolli(0.020, 0.014),
  new Bcolli(0.020, 0.028),
  new Bcolli(0.020, 0.056),
  new Bcolli(0.021, 0.019, 0.071, 0.069),
  new Bcolli(0.0201, 0.0199, 0.0701, 0.0699),
  new Bcolli(0.020, 0.020, 0.070, 0.070),
  new Bcolli(0.020, 0.020, 0.007, 0.007),
  new Bcolli(0.020, 0.020, 0.70, 0.70),
  new Bcolli(0.00676, 0.00414, 0.00597, 0.00596),
  new Bcolli(0.1, 0.1, 0.25, 0.25), // GSI collider
  new HES, // HES spectrometer at JLAB Hall C (P.A.)
  new allColli, new inPlaneColli, NULL
};

Collimator *ColliCList[] = {
  new C225, new C28, new allColli, new inPlaneColli, NULL
};

Collimator *ColliDList[] = {
  new D7, new D4, new D2, new allColli, new inPlaneColli, NULL
};

Collimator *ColliOhipsList[] = {
  new allColli,
  new OHIPS_simple(OHIPS_SHORT_HORIZONTAL, OHIPS_SHORT_VERTICAL,
		  OHIPS_SHORT_DISTANCE), 
  new OHIPS_simple(OHIPS_LONG_HORIZONTAL, OHIPS_LONG_VERTICAL,
		  OHIPS_LONG_DISTANCE), 
  new OHIPS_fancy(OHIPS_SHORT_HORIZONTAL, OHIPS_SHORT_VERTICAL,
		  OHIPS_SHORT_DISTANCE), 
  new OHIPS_fancy(OHIPS_LONG_HORIZONTAL, OHIPS_LONG_VERTICAL,
		  OHIPS_LONG_DISTANCE), 
  new OHIPS_qspin(OHIPS_SHORT_HORIZONTAL, OHIPS_SHORT_VERTICAL,
		  OHIPS_SHORT_DISTANCE), 
  new OHIPS_qspin(OHIPS_LONG_HORIZONTAL, OHIPS_LONG_VERTICAL,
		  OHIPS_LONG_DISTANCE), 
  NULL
};

Collimator *ColliOopsAList[] = {
  new allColli,
  new OOPS_simple(OOPS_SHORT_HORIZONTAL, OOPS_SHORT_VERTICAL,
		  OOPS_SHORT_DISTANCE), 
  new OOPS_simple(OOPS_LONG_HORIZONTAL, OOPS_LONG_VERTICAL,
		  OOPS_LONG_DISTANCE), 
  new OOPS_fancy(OOPS_SHORT_HORIZONTAL, OOPS_SHORT_VERTICAL,
		  OOPS_SHORT_DISTANCE), 
  new OOPS_fancy(OOPS_LONG_HORIZONTAL, OOPS_LONG_VERTICAL,
		  OOPS_LONG_DISTANCE), 
  new OOPS_qspin(OOPS_SHORT_HORIZONTAL, OOPS_SHORT_VERTICAL,
		  OOPS_SHORT_DISTANCE), 
  new OOPS_qspin(OOPS_LONG_HORIZONTAL, OOPS_LONG_VERTICAL,
		  OOPS_LONG_DISTANCE), 
  NULL
};

Collimator *ColliOopsBList[] = {
  new allColli,
  new OOPS_simple(OOPS_SHORT_HORIZONTAL, OOPS_SHORT_VERTICAL,
		  OOPS_SHORT_DISTANCE), 
  new OOPS_simple(OOPS_LONG_HORIZONTAL, OOPS_LONG_VERTICAL,
		  OOPS_LONG_DISTANCE), 
  new OOPS_fancy(OOPS_SHORT_HORIZONTAL, OOPS_SHORT_VERTICAL,
		  OOPS_SHORT_DISTANCE), 
  new OOPS_fancy(OOPS_LONG_HORIZONTAL, OOPS_LONG_VERTICAL,
		  OOPS_LONG_DISTANCE), 
  new OOPS_qspin(OOPS_SHORT_HORIZONTAL, OOPS_SHORT_VERTICAL,
		  OOPS_SHORT_DISTANCE), 
  new OOPS_qspin(OOPS_LONG_HORIZONTAL, OOPS_LONG_VERTICAL,
		  OOPS_LONG_DISTANCE), 
  NULL
};

Collimator *ColliOopsCList[] = {
  new allColli,
  new OOPS_simple(OOPS_SHORT_HORIZONTAL, OOPS_SHORT_VERTICAL,
		  OOPS_SHORT_DISTANCE), 
  new OOPS_simple(OOPS_LONG_HORIZONTAL, OOPS_LONG_VERTICAL,
		  OOPS_LONG_DISTANCE), 
  new OOPS_fancy(OOPS_SHORT_HORIZONTAL, OOPS_SHORT_VERTICAL,
		  OOPS_SHORT_DISTANCE), 
  new OOPS_fancy(OOPS_LONG_HORIZONTAL, OOPS_LONG_VERTICAL,
		  OOPS_LONG_DISTANCE), 
  new OOPS_qspin(OOPS_SHORT_HORIZONTAL, OOPS_SHORT_VERTICAL,
		  OOPS_SHORT_DISTANCE), 
  new OOPS_qspin(OOPS_LONG_HORIZONTAL, OOPS_LONG_VERTICAL,
		  OOPS_LONG_DISTANCE), 
  NULL
};

Collimator *ColliOopsDList[] = {
  new allColli,
  new OOPS_simple(OOPS_SHORT_HORIZONTAL, OOPS_SHORT_VERTICAL,
		  OOPS_SHORT_DISTANCE), 
  new OOPS_simple(OOPS_LONG_HORIZONTAL, OOPS_LONG_VERTICAL,
		  OOPS_LONG_DISTANCE), 
  new OOPS_fancy(OOPS_SHORT_HORIZONTAL, OOPS_SHORT_VERTICAL,
		  OOPS_SHORT_DISTANCE), 
  new OOPS_fancy(OOPS_LONG_HORIZONTAL, OOPS_LONG_VERTICAL,
		  OOPS_LONG_DISTANCE), 
  new OOPS_qspin(OOPS_SHORT_HORIZONTAL, OOPS_SHORT_VERTICAL,
		  OOPS_SHORT_DISTANCE), 
  new OOPS_qspin(OOPS_LONG_HORIZONTAL, OOPS_LONG_VERTICAL,
		  OOPS_LONG_DISTANCE), 
  NULL
};

Collimator *ColliKAOSList[] = {
  new allColli, new inPlaneColli, new KAOScolli, 
  new HKS, // HKS spectrometer at JLAB Hall C (P.A.)
  NULL
};
