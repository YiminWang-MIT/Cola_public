//								      -*-c++-*-
// $Id: Focus.h 2216 2008-06-13 21:13:47Z distler $
//
// Wobbler and long target corrections
//

#ifndef __Focus_h__
#define __Focus_h__

#include "TMA/FastTM.h"
#include "rundatabase.h"

const double d01x = 5677.3; //Distance between BPM0X and BPM1X [mm]
const double d01y = 5514.8; //Distance between BPM0Y and BPM1Y [mm]
const double dt1x = 1813.3; //Distance between Target and BPM1X [mm]
const double dt1y = 1893.3; //Distance between Target and BPM1Y [mm]

/** 
  Converts the hardware readout of the BPM to beam position in mm.
  BPM [0] ([1]) is BPM0 (1) in the South Hall. */
class BPM {
protected:
  /// horizontal readout for central position
  double offsH[2];
  /// horizontal conversion factor
  double slopeH[2];
  /// vertical readout for central position
  double offsV[2];
  /// vertical conversion factor
  double slopeV[2];
public:
  /// constructor takes offsets and conversion factors as input
  BPM(struct Bpm *bpm);
  /// calculate horizontal beam position from both BPMs or just BPM1
  double hor(unsigned short pos0, unsigned short pos1);
  double hor(unsigned short pos);
  /// calculate vertical beam position from both BPMs or just BPM1
  double vert(unsigned short pos0, unsigned short pos1);
  double vert(unsigned short pos);
};

class ExtendedTarget {
protected:
  /// OHIPS angle
  double angleO;
  double sinO, cosO;
  /// OOPS angles
  double angleA, angleB, angleC, angleD;
  double sinA, cosA, sinB, cosB, sinC, cosC, sinD, cosD;
public:
  ExtendedTarget(double angle_O, double angle_A, double angle_B,
		 double angle_C, double angle_D);
};

class Focus : public BPM, public ExtendedTarget {
public:
  ///
  Focus(struct Bpm *bpm,
        double angle_O, double angle_A, double angle_B, double angle_C,
	double angle_D);
  ///
  double z_by_O(double horiz, struct TargetCo *resultO);
};

#endif
