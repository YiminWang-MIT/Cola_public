//
// Created 1994 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Id: Focus.cc 2640 2015-04-01 14:03:00Z aqua $
//
// Vertex in extended targets using BPMs
//

#define _DEFAULT_SOURCE 1
#define _XOPEN_SOURCE 1
#include <stdio.h>
#include <math.h>
#include "Focus.h"

inline double square(double x)  { return x * x; }

BPM::BPM(struct Bpm *bpm)
{
  for (int i=0; i<2; i++) {
    offsH[i]  = bpm[i].x0;
    slopeH[i] = bpm[i].dx;
    offsV[i]  = bpm[i].y0;
    slopeV[i] = bpm[i].dy;
  }
}

double
BPM::hor(unsigned short pos0, unsigned short pos1)
{
  double x0 = slopeH[0]*pos0 + offsH[0];
  double x1 = slopeH[1]*pos1 + offsH[1];

  return x1 + dt1x*(x1-x0)/d01x; // x=tan(x) approximation
}

double
BPM::hor(unsigned short pos)
{
  return slopeH[1]*pos + offsH[1];
}

double
BPM::vert(unsigned short pos0, unsigned short pos1)
{
  double y0 = slopeV[0]*pos0 + offsV[0];
  double y1 = slopeV[1]*pos1 + offsV[1];

  return y1 + dt1y*(y1-y0)/d01y; // y=tan(y) approximation
}

double
BPM::vert(unsigned short pos)
{
  return slopeV[1]*pos + offsV[1];
}

ExtendedTarget::ExtendedTarget(double angle_O, double angle_A, 
                               double angle_B, double angle_C,
			       double angle_D) {
  angleO = angle_O;
  angleA = angle_A;
  angleB = angle_B;
  angleC = angle_C;
  angleD = angle_D;
  sinO	 = sin(angle_O);
  cosO	 = cos(angle_O);
  sinA	 = sin(-angle_A);
  cosA	 = cos(-angle_A);
  sinB	 = sin(-angle_B);
  cosB	 = cos(-angle_B);
  sinC	 = sin(-angle_C);
  cosC	 = cos(-angle_C);
  sinD	 = sin(-angle_D);
  cosD	 = cos(-angle_D);
}

Focus::Focus(struct Bpm *bpm, 
	     double angle_O, double angle_A, double angle_B, double angle_C,
	     double angle_D) : BPM(bpm), 
  ExtendedTarget(angle_O, angle_A, angle_B, angle_C, angle_D) { ; }

double
Focus::z_by_O(double horiz, struct TargetCo *resultO)
{
  double y0  = resultO->y0*10.0;
  double ph0 = 0.001*resultO->ph;

  return -y0 * cos(ph0) / sin(angleO+ph0) - horiz / tan (angleO+ph0);
}
