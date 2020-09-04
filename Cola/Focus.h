//								      -*-c++-*-
// $Id: Focus.h 2216 2008-06-13 21:13:47Z distler $
//
// Wobbler and long target corrections
//

#ifndef __Focus_h__
#define __Focus_h__

#include <iostream>
#include <fstream>


extern std::ostream * fdeb;

#include "TMA/FastTM.h"

/** 
  This class converts the hardware readout of the wobbler current amplitude
  to the beam position in mm. The hardware readout is a 16bit word with the
  horizontal position in the low byte and the vertical position in the high
  byte. 
  */
class WobblerA {
protected:
  /// horizontal readout for central position (negativ value, bits)
  double offsHA;
  /// horizontal conversion factor (mm/bit)
  double factHA;
  /// vertical readout for central position (negativ value, bits)
  double offsVA;
  /// vertical conversion factor (mm/bit)
  double factVA;
public:
  /// constructor takes offsets and conversion factors as input
  WobblerA(double offsetHA, double factorHA, double offsetVA, double factorVA);
  /// calculate horizontal beam position
  double horA(unsigned short position);
  /// calculate vertical beam position
  double vertA(unsigned short position);
};

class WobblerB {
protected:
  /// horizontal readout for central position (negativ value, bits)
  double offsHB;
  /// horizontal conversion factor (mm/bit)
  double factHB;
  /// vertical readout for central position (negativ value, bits)
  double offsVB;
  /// vertical conversion factor (mm/bit)
  double factVB;
public:
  /// constructor takes offsets and conversion factors as input
  WobblerB(double offsetHB, double factorHB, double offsetVB, double factorVB);
  /// calculate horizontal beam position
  double horB(unsigned short position);
  /// calculate vertical beam position
  double vertB(unsigned short position);
};

class WobblerC {
protected:
  /// horizontal readout for central position (negativ value, bits)
  double offsHC;
  /// horizontal conversion factor (mm/bit)
  double factHC;
  /// vertical readout for central position (negativ value, bits)
  double offsVC;
  /// vertical conversion factor (mm/bit)
  double factVC;
public:
  /// constructor takes offsets and conversion factors as input
  WobblerC(double offsetHC, double factorHC, double offsetVC, double factorVC);
  /// calculate horizontal beam position
  double horC(unsigned short position);
  /// calculate vertical beam position
  double vertC(unsigned short position);
};

/**
  This class is used to calculate the vertex position of the reaction within 
  an extended target.
  */
class ExtendedTarget {
protected:
  /// angle of spectrometer A
  double angleA;
  /// angle of spectrometer B
  double angleB;
  double oopB;
  /// angle of spectrometer C
  double angleC;
  double sinA, cosA, sinB, cosB, sinC, cosC;
  /// slope of the particle trajectory to spectrometer A
  double stA;
  ///
  double yA;
  ///
  double zA;
  /// slope of the particle trajectory to spectrometer B
  double stB;
  ///
  double yB;
  ///
  double zB;
  ///
  void focusA(struct TargetCo *resultA);
  ///
  void focusB(struct TargetCo *resultB);
  /// Vertex coordinates in spectrometer A frame
  double xR_A;
  double zR_A;
  ///
  void focusR_A(double horiz, double verti, struct TargetCo *resultA);
  /// Vertex coordinates in spectrometer B frame
  double xR_B;
  double zR_B;
  ///
  void focusR_B(double horiz, double verti, struct TargetCo *resultB);
  /// Vertex coordinates in spectrometer C frame
  double xR_C;
  double zR_C;
  ///
  void focusR_C(double horiz, double verti, struct TargetCo *resultC);
public:
  ///
  ExtendedTarget(double angleSpekA, double angleSpekB, double anglespecC,
		 double oopangB=0.0);
  ///
  double y(struct TargetCo *resultA, struct TargetCo *resultB);
  /// corr for Cola
  /// antiCorr for Simul
  void corrA(double x0, struct TargetCo *resultA);
  void antiCorrA(double hor, double vert, struct TargetCo *resultA);
  ///
  void corrB(double x0, struct TargetCo *resultB);
  void antiCorrB(double hor, double vert, struct TargetCo *resultB);
  ///
  void corrC(double x0, struct TargetCo *resultC);
  void antiCorrC(double hor, double vert, struct TargetCo *resultC);
  ///
};

///
class Focus : public WobblerA, public WobblerB, public WobblerC, 
              public ExtendedTarget {
public:
  ///
  Focus(double offsetHA, double factorHA, double offsetVA, double factorVA,
	double offsetHB, double factorHB, double offsetVB, double factorVB,
        double offsetHC, double factorHC, double offsetVC, double factorVC,
        double angleSpekA, double angleSpekB, double anglespecC,
	double oopangB=0.0);
  ///
  double z(double horiz, struct TargetCo *resultB);
  ///
  double z_A(double horiz, struct TargetCo *resultA);
  ///
  double z_B(double horiz, struct TargetCo *resultB);
  ///
  double z_C(double horiz, struct TargetCo *resultC);
  ///
  double z_by_A(double horiz, struct TargetCo *resultA);
  ///
  double z_by_B(double horiz, double verti, struct TargetCo *resultB);
  ///
  double z_by_C(double horiz, struct TargetCo *resultC);
  /// extra path length due to extended target
  double l_R_A(double horiz, double verti, struct TargetCo *resultA);
  ///
  double l_R_B(double horiz, double verti, struct TargetCo *resultB);
  ///
  double l_R_C(double horiz, double verti, struct TargetCo *resultC);
  ///
  void corr(double horiz, double verti, struct TargetCo *resultA,
	    struct TargetCo *resultB);
  /// correction of delta and theta of SpecA
  void focuscorrA(double horiz, double verti, struct TargetCo *resultA);
  /// correction of delta and theta of SpecB
  void focuscorrB(double horiz, double verti, struct TargetCo *resultB);
  /// correction of delta and theta of SpecC
  void focuscorrC(double horiz, double verti, struct TargetCo *resultC);

  /// correction of delta and theta of SpecA
  void focusanticorrA(double horiz, double verti, struct TargetCo *resultA);
  /// correction of delta and theta of SpecB
  void focusanticorrB(double horiz, double verti, struct TargetCo *resultB);
  /// correction of delta and theta of SpecC
  void focusanticorrC(double horiz, double verti, struct TargetCo *resultC);
};

#endif
