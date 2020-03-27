//
// Created 1994 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Id: Focus.cc 2640 2015-04-01 14:03:00Z aqua $
//
// Wobbler and long target corrections
//

#define YWdebug 0

#define _DEFAULT_SOURCE 1
#define _XOPEN_SOURCE 1
#include <stdio.h>
#include <math.h>
#include "Focus.h"
#include "rundatabase.h"

// These factors are from K. I. Blomqvist et al.,  
// Nucl. Instr. and Meth. A 403 (1998) 263-301
// Page 13 Table 2
// deltaA=(x|x)/(x|dP) (mm!)
// deltaT=(th|x)/(th|th) (mm!)
const double FAKTOR_DELTA_A = -0.532 / 56.5;
const double FAKTOR_THETA_A = -0.992 / -1.88;
const double FAKTOR_DELTA_B = -0.853 / 82.2;
const double FAKTOR_THETA_B = -0.316/-1.17;
const double FAKTOR_DELTA_C = -0.526 / 44.1;
const double FAKTOR_THETA_C = -1.270 / -1.90;


inline double square(double x)  { return x * x; }

WobblerA::WobblerA(double oHA, double factorHA, double oVA, double factorVA)
{
  offsHA = oHA;
  factHA = factorHA;
  offsVA = oVA;
  factVA = factorVA;
}

double
WobblerA::horA(unsigned short position)
{
  return factHA * (offsHA + (position >> 8 & 0xff));
}

double
WobblerA::vertA(unsigned short position)
{
  return factVA * (offsVA + (position & 0xff));
}

WobblerB::WobblerB(double oHB, double factorHB, double oVB, double factorVB)
{
  offsHB = oHB;
  factHB = factorHB;
  offsVB = oVB;
  factVB = factorVB;
}

double
WobblerB::horB(unsigned short position)
{
#if YWdebug
  std::cout << "horB\t" << (position >> 8 & 0xff) << '\t' << factHB << '\t' << offsHB << std::endl;
#endif
  return factHB * (offsHB + (position >> 8 & 0xff));
}

double
WobblerB::vertB(unsigned short position)
{
#if YWdebug
  std::cout << "vertB\t" << (position & 0xff) << '\t' << factVB << '\t' << offsVB << std::endl;
#endif
  return factVB * (offsVB + (position & 0xff));
}

WobblerC::WobblerC(double oHC, double factorHC, double oVC, double factorVC)
{
  offsHC = oHC;
  factHC = factorHC;
  offsVC = oVC;
  factVC = factorVC;
}

double
WobblerC::horC(unsigned short position)
{
  return factHC * (offsHC + (position >> 8 & 0xff));
}

double
WobblerC::vertC(unsigned short position)
{
  return factVC * (offsVC + (position & 0xff));
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

ExtendedTarget::ExtendedTarget(double anglespecA, double anglespecB, 
                               double anglespecC, double oopangB)
{
  angleA = anglespecA;
  angleB = anglespecB;
  oopB   = oopangB;
  angleC = anglespecC;
  sinA	 = sin(anglespecA);
  cosA	 = cos(anglespecA);
  sinB	 = sin(-anglespecB);
  cosB	 = cos(-anglespecB);
  sinC	 = sin(-anglespecC);
  cosC	 = cos(-anglespecC);
}

void
ExtendedTarget::focusR_A(double horiz, double verti, struct TargetCo *resultA)
{
  double y0  = resultA->y0*10.0;
  double ph0 = 0.001*resultA->ph;
  double th0 = 0.001*resultA->th;
  zR_A = -(y0 * cosA + horiz) * cos(ph0) / sin(angleA + ph0);
  xR_A = verti - zR_A * tan(th0);

  // Focus correction for spec A is heavily checked and is OK! Don't touch these lines
  // without contacting me, even if you suspect an error! Harald Merkel

}

void
ExtendedTarget::focusR_B(double hor, double vert, struct TargetCo *resultB)
{

  //  double HFmaxy0 =  fabs(25.*sinB) + (rundb.beam.offset.x + 2.)*cosB;
  double y0  = resultB->y0*10.0;
  double ph0 = 0.001*resultB->ph;
  double th0 = 0.001*resultB->th;
  // zR_B = -(y0 * cosB + horiz) * cos(ph0) / sin(-angleB + ph0);
  // yR_B =  (y0 + horiz) * sinB * cos(ph0) / sin(-angleB + ph0) - horiz;
  // xR_B = verti - zR_B * tan(th0);

  zR_B = -((hor*cos(oopB) + cosB*y0*cos(oopB) + sinB*vert*sin(oopB))/
	   (sinB + cosB*cos(oopB)*tan(ph0)));
  xR_B = (sinB*vert*cos(oopB) - hor*sin(oopB) + 
	  cosB*(-(y0*sin(oopB)) + vert*tan(ph0)))/
    (sinB + cosB*cos(oopB)*tan(ph0));
  
  // xR_B is the wrong name: this should be called x0:
  xR_B = xR_B - zR_B * tan(th0);
}

void
ExtendedTarget::focusR_C(double horiz, double verti, struct TargetCo *resultC)
{
  double y0  = resultC->y0*10.0;
  double ph0 = 0.001*resultC->ph;
  double th0 = 0.001*resultC->th;

  zR_C = -(y0 *cosC + horiz)  * cos(ph0) / sin(-angleC+ph0);
  xR_C = verti - zR_C * tan(th0);
}

void
ExtendedTarget::corrA(double x0, struct TargetCo *resultA)
{
  resultA->dp += FAKTOR_DELTA_A * x0;
  resultA->th -= FAKTOR_THETA_A * x0;
}

void
ExtendedTarget::corrB(double x0, struct TargetCo *resultB)
{
  resultB->dp += FAKTOR_DELTA_B * x0;
  resultB->th -= FAKTOR_THETA_B * x0;
}

void
ExtendedTarget::corrC(double x0, struct TargetCo *resultC)
{
  resultC->dp += FAKTOR_DELTA_C * x0;
  resultC->th -= FAKTOR_THETA_C * x0;
}


//presently not in use:
double
ExtendedTarget::y(struct TargetCo *resultA, struct TargetCo *resultB)
{
  focusA(resultA);
  focusB(resultB);

  return (stA * yA + zB - zA - stB * yB) / (stA - stB);
}

void
ExtendedTarget::focusA(struct TargetCo *resultA)
{
  double y0 = resultA->y0*10.0;

  yA  = y0  * cosA;
  zA  = -y0 * sinA;
  stA = tan(M_PI_2 - angleA - 0.001 * resultA->ph);
}

void
ExtendedTarget::focusB(struct TargetCo *resultB)
{
  double y0 = resultB->y0*10.0;

  yB  = y0*cosB;
  zB  = y0*sinB;
  stB = -tan(M_PI_2-angleB+0.001*resultB->ph);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Focus::Focus(double offsetHA, double factorHA, 
             double offsetVA, double factorVA,
             double offsetHB, double factorHB, 
             double offsetVB, double factorVB,
             double offsetHC, double factorHC, 
             double offsetVC, double factorVC,
	     double anglespecA, double anglespecB, double anglespecC, 
	     double oopangB)
     : WobblerA(offsetHA, factorHA, offsetVA, factorVA)
     , WobblerB(offsetHB, factorHB, offsetVB, factorVB)     
     , WobblerC(offsetHC, factorHC, offsetVC, factorVC)     
     , ExtendedTarget(anglespecA, anglespecB, anglespecC, oopangB) { ; }

double
Focus::z_by_A(double horiz, struct TargetCo *resultA)
{
  double y0  = resultA->y0*10.0;
  double ph0 = 0.001*resultA->ph;

  return -y0 * cos(ph0) / sin(angleA+ph0) - horiz / tan (angleA+ph0);
}

double
Focus::l_R_A(double horiz, double verti, struct TargetCo *resultA)
{
  double ph0 = 0.001*resultA->ph;
  focusR_A(horiz, verti, resultA);
  return zR_A / cos(ph0);
}

double
Focus::z_by_B(double horiz, double verti, struct TargetCo *resultB)
{
  double y0  = resultB->y0*10.0;
  double ph0 = 0.001*resultB->ph;
  // Old line, equivalent for oopB==0!
  //  return -y0 * cos(ph0) / sin(-angleB+ph0) - horiz / tan (-angleB+ph0);

  return 
    -((horiz*cos(-angleB) + y0 - (horiz*cos(oopB)*sin(-angleB) 
			       +  verti*sin(oopB))*tan(ph0))/
      (sin(-angleB) +  cos(-angleB) * cos(oopB) * tan(ph0)));
}

double
Focus::l_R_B(double horiz, double verti, struct TargetCo *resultB)
{
  double ph0 = 0.001*resultB->ph;
  focusR_B(horiz, verti, resultB);
  return zR_B / cos(ph0);
}

double
Focus::z_by_C(double horiz, struct TargetCo *resultC)
{
  double y0  = resultC->y0*10.0;
  double ph0 = 0.001*resultC->ph;

  return -y0 * cos(ph0) / sin(-angleC+ph0) - horiz / tan (-angleC+ph0);
}

double
Focus::l_R_C(double horiz, double verti, struct TargetCo *resultC)
{
  double ph0 = 0.001*resultC->ph;
  focusR_C(horiz, verti, resultC);
  return zR_C / cos(ph0);
}

void
Focus::focuscorrA(double horiz, double verti, struct TargetCo *resultA)
{
  focusR_A(horiz, verti, resultA);
  corrA(xR_A,resultA);
}

void
Focus::focuscorrB(double horiz, double verti, struct TargetCo *resultB)
{
  focusR_B(horiz, verti, resultB);
  corrB(xR_B,resultB);
}

void
Focus::focuscorrC(double horiz, double verti, struct TargetCo *resultC)
{
  focusR_C(horiz, verti, resultC);
  corrC(xR_C,resultC);
}



//presently not in use:
double
Focus::z(double horiz, struct TargetCo *resultB)
{
  focusB(resultB);
  return stB * (horiz - yB) + zB;
}

double
Focus::z_A(double horiz, struct TargetCo *resultA)
{
  focusA(resultA);
  return stA * (horiz - yA) + zA;
}

void
Focus::corr(double horiz, double verti, struct TargetCo *resultA,
		 struct TargetCo *resultB)
{
  double x0 = verti;
  double y0 = horiz;
  double z0 = z(horiz, resultB);
  focusA(resultA);

  double distA = sqrt(square(y0-yA)+square(z0-zA));
  corrA(x0+distA*sin(0.001*resultA->th),resultA);
  
  double distB = sqrt(square(y0-yB)+square(z0-zB));
  corrB(x0+distB*sin(0.001*resultB->th),resultB);
}

double
Focus::z_B(double horiz, struct TargetCo *resultB)
{
  focusB(resultB);
  return stB * (horiz - yB) + zB;
}

double
Focus::z_C(double horiz, struct TargetCo *resultC)
{
  //C wird hier noch wie B (=rechts stehend) behandelt.
  focusB(resultC);
  return stB * (horiz - yB) + zB;
}
