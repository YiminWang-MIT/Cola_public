//                                                                    -*-C++-*-

#ifndef __Colli_h__
#define __Colli_h__

#include "FourVector/FourVector.h"

/// Abstract Base class for collimator
class Collimator {
protected:
  char *name;
public:
  virtual ~Collimator() { ; }
  /// Check acceptance
  virtual int accepted(double target[3], FourVector p,
		       double theta, double oop, double refmom) = 0;
  /// Get the name of this collimator (as in run.db) for initialization
  inline char *getName() { return name;}; 
};

/// Utility class for check of a single collimator plane
class colliPlane {
protected:
  double cutH, cutV, cutH2, cutV2;
  double l;
  Momentum normal, x;
public:
  /// Constructor: distance to the target, cut in horizontal and vertical 
  colliPlane(double length, double cutHorizontal, double cutVertical) {
    cutH  =  cutHorizontal; 
    cutH2 = -cutHorizontal; 
    cutV =   cutVertical;
    cutV2 = -cutVertical;
    l = length;
  };
  /// Constructor: distance to the target, 
  /// cut in horizontal (left/right) and vertical (top/bottom)
  colliPlane(double length, double cutLeft, double cutRight,
	     double cutTop, double cutBottom ) {
    cutH  =  cutLeft;
    cutH2 = -cutRight;
    cutV  =  cutTop; 
    cutV2 = -cutBottom;
    l = length;
  };
  virtual ~colliPlane() { ; }
  /// initalize with that spectrometer angle (just a rotation)
  void init(double theta, double oop = 0) {
    x = (normal = 
	 Momentum(sin(theta)*cos(oop), sin(oop), cos(theta)*cos(oop))) * l;
  };
  /// horizontal check with target position and direction of the particle
  int checkHorizontal(double x0[], FourVector p) {
    Momentum d = x - (Momentum) x0;
    if ((Momentum) p*normal <0) return false; // fix by JCB,
					      // only forward dir allowed!
    double   a = d * normal / ((Momentum) p * normal);
    double xx = (a*p[1]-d[0]) * normal[2] - (a*p[3]-d[2]) * normal[0];
    return (xx < cutH && xx > cutH2);
  }
  /// vertical check with target position and direction of the particle
  int checkVertical(double x0[], FourVector p) {
    Momentum d = x - (Momentum) x0; 
    if ((Momentum) p*normal <0) return false; // fix by JCB,
					      // only forward dir allowed!
    double xx = d * normal / ((Momentum) p * normal) * p[2] - d[1];
    return (xx < cutV && xx > cutV2);
  };
  //only used for old ndet at the moment (2017):
  double posVertical(double x0[], FourVector p) {
    Momentum d = x - (Momentum) x0; 
    double h = d * normal / ((Momentum) p * normal) * p[2] - d[1];
    return (h);
  };
  //only used for old ndet at the moment (2017):
  double posHorizontal(double x0[], FourVector p) {
    Momentum d = x - (Momentum) x0;
    double   a = d * normal / ((Momentum) p * normal);
    double h = (a*p[1]-d[0]) * normal[2] - (a*p[3]-d[2]) * normal[0];
    return (h);
  };
  Momentum getx() { return x; }
};

/// Utility class for check of a single contour plane
class colliContour {
  double cutH, cutV, cutC;
  double l;
  Momentum x;
public:
  /// Constructor: distance to the target, cut in contour 
  colliContour(double length, double cutHorizontal, 
	       double cutVertical, double cutContour) {
    l = length;
    cutH = cutHorizontal; 
    cutV = cutVertical;
    cutC = cutContour;
  };
  virtual ~colliContour() { ; }
  /// initalize with that spectrometer angle (just a rotation)
  void init(double theta, double oop) {
    x = Momentum(sin(theta)*cos(oop), sin(oop), cos(theta)*cos(oop)) * l;
  };
  /// contour check with target position and direction of the particle
  int checkContour(double x0[], FourVector p) {
    Momentum d = x - (Momentum) x0;
    double p_times_x = ((Momentum) p * x);
    if (acos(p_times_x / p.momentum() / l ) > 0.3) return 0;
    double a = d * x / p_times_x;
    double hx = a*p[1]-d[0];
    double hz = a*p[3]-d[2];
    double h = sqrt(hx*hx + hz*hz);
    double v = a * p[2] - d[1];
    return (fabs(h) < cutH && fabs(v) < cutV && fabs(v)+fabs(h) < cutC);
  };
};

/// Utility class for check off a single trapezoid-shaped plane 
/// with a constant vertical distance of the sides and a varying 
/// horizontal distance
class colliTrapezoid {
  double cutV, cutHa, cutVa;
  double l;
  Momentum normal, x;
public:
  /// Constructor: distance to the target, cut in shape: 
  /// cutVertical is half the height of the opening,
  /// cutHorizontalAxis is half the width of the opening in the middle,
  /// cutVerticalAxis is the point where the two sides with 45° slope
  /// of the Colli would meet
  colliTrapezoid(double length, double cutVertical, 
	       double cutHorizontalAxis, double cutVerticalAxis) {
    l = length;
    cutV = cutVertical;
    cutHa = cutHorizontalAxis;
    cutVa = cutVerticalAxis;
  };
  virtual ~colliTrapezoid() { ; }
  /// initalize with spectrometer angle (just a rotation)
  void init(double theta, double oop) {
    x = Momentum(sin(theta)*cos(oop), sin(oop), cos(theta)*cos(oop)) * l;
  };
  /// contour check with target position and direction of the particle
  int checkTrapezoid(double x0[], FourVector p) {
    Momentum d = x - (Momentum) x0;
    double a = d * x / ((Momentum) p * x);
    double hx = a*p[1]-d[0];
    double hz = a*p[3]-d[2];
    double h = sqrt(hx*hx + hz*hz);
    double v = a * p[2] - d[1];
    return (fabs(v) < cutV && v - cutVa/cutHa*fabs(h) > -cutVa);
  };
};


/// The 4 pi collimator, used for Simulation, accepts everything
class allColli : public Collimator {
public:
  ///
  allColli() { name = "4pi"; };
  virtual ~allColli() { ; }
  /// Check if particle is accepted by this collimator
  int accepted(double *, FourVector, double, double, double) { return -1;};
};

/// The in plane collimator, used for Simulation, accepts +/- 70mrad OutOfPlane
class inPlaneColli : public Collimator {
public:
  ///
  inPlaneColli() { name = "inPlane"; };
  virtual ~inPlaneColli() { ; }
  /// Check if particle is accepted by this collimator
  int accepted(double [3], FourVector p, double, double, double){ 
    return fabs(atan2(p[2], sqrt(p[1]*p[1] + p[3]*p[3]))) < 0.070;
  };
};

/// 15msr bismuth collimator for spectrometer A
class A15 : public Collimator { 
private:
  int init;
  colliPlane *Plane_h[6];
  colliPlane *Plane_v[4];
public:
  ///
  A15();
  virtual ~A15() { ; }
  /// Check if particle is accepted by this collimator
  int accepted(double target[3], FourVector p,
	       double theta, double oop, double);
};

/// 21msr bismuth collimator for spectrometer A
class A21 : public Collimator { 
private:
  int init;
  colliPlane *Plane_h[6];
  colliPlane *Plane_v[4];
public:
  ///
  A21();
  virtual ~A21() { ; }
  /// Check if particle is accepted by this collimator
  int accepted(double target[3], FourVector p,
	       double theta, double oop, double);
};

/// modified 28msr colimator for spectrometer A
class A28 : public Collimator { 
private:
  int init;
  colliContour *Contour[4];
public:
  ///
  A28();
  virtual ~A28() { ; }
  /// Check if particle is accepted by this collimator
  int accepted(double target[3], FourVector p,
	       double theta, double oop, double);
};

/// The collimator for spectrometer B
class Bcolli : public Collimator { 
private:
  colliPlane *vertPlane, *horPlane;
  int    init;
public:
  virtual ~Bcolli() { delete vertPlane; delete horPlane; };
  /// Constructor with acceptances in mrad, e.g. Bcolli(0.02,0.07) is 5.6msr
  Bcolli(double horizontal, double vertical);
  /// Constructor with acceptances in mrad (all four values independently),
  /// e.g. Bcolli(0.02, 0.00, 0.07, 0.07) is 2.8msr
  Bcolli(double horiLeft, double horiRight, 
	 double vertiTop, double vertiBottom);
  /// Check if particle is accepted by this collimator
  int accepted(double target[3], FourVector p,
	       double theta, double oop, double);
};

/// Experimental - simple - collimator for magix spectrometers, based on Bcolli, but mainly different colli-target distance
class MXcolli : public Collimator { 
private:
  colliPlane *vertPlane, *horPlane;
  int    init;
public:
  virtual ~MXcolli() { delete vertPlane; delete horPlane; };
  /// Constructor with acceptances in mrad, e.g. MXcolli(0.02,0.07) is 5.6msr
  MXcolli(double horizontal, double vertical, double collidist);
  /// Check if particle is accepted by this collimator
  int accepted(double target[3], FourVector p,
	       double theta, double oop, double);
};

/// 24 msr collimator for HES spectrometer @ JLAB
class HES : public Collimator { 
private:
  int init;
  colliPlane *vertPlane, *horPlane;
public:
  ///
  HES();
  virtual ~HES() { ; }
  /// Check if particle is accepted by this collimator
  int accepted(double target[3], FourVector p,
	       double theta, double oop, double);
};

/// 18 msr collimator for HKS spectrometer @ JLAB 
class HKS : public Collimator { 
private:
  int init;
  colliPlane *vertPlane, *horPlane;
public:
  ///
  HKS();
  virtual ~HKS() { ; }
  /// Check if particle is accepted by this collimator
  int accepted(double target[3], FourVector p,
	       double theta, double oop, double);
};

/// 22.5msr heavy-metal collimator for spectrometer C
class C225 : public Collimator { 
private:
  int init;
  colliPlane *Plane_h[6];
  colliPlane *Plane_v[4];
public:
  ///
  C225();
  virtual ~C225() { ; }
  /// Check if particle is accepted by this collimator
  int accepted(double target[3], FourVector p,
	       double theta, double oop, double);
};

/// modified 28msr colimator for spectrometer C
class C28 : public Collimator { 
private:
  int init;
  colliContour *Contour[4];
public:
  virtual ~C28() { ; }
  ///
  C28();
  /// Check if particle is accepted by this collimator
  int accepted(double target[3], FourVector p,
	       double theta, double oop, double);
};

/// collimator for pion spectrometer at 54 cm distance to target
class D7 : public Collimator { 
private:
  int init;
  colliPlane *Plane_h[2];
  colliPlane *Plane_v[2];
public:
  virtual ~D7() { ; }
  ///
  D7();
  /// Check if particle is accepted by this collimator
  int accepted(double target[3], FourVector p,
	       double theta, double oop, double);
};

/// collimator for pion spectrometer at 66 cm distance to target
class D4 : public Collimator { 
private:
  int init;
  colliPlane *Plane_h[2];
  colliPlane *Plane_v[2];
public:
  virtual ~D4() { ; }
  ///
  D4();
  /// Check if particle is accepted by this collimator
  int accepted(double target[3], FourVector p,
	       double theta, double oop, double);
};

/// collimator for pion spectrometer at 66 cm distance to target, angles up to 35 deg
class D2 : public Collimator { 
private:
  int init;
  colliTrapezoid *Trapezoid[2];
public:
  virtual ~D2() { ; }
  ///
  D2();
  /// Check if particle is accepted by this collimator
  int accepted(double target[3], FourVector p,
	       double theta, double oop, double);
};

/// Collimator for the Hadron3 Detector
class H3colli : public colliPlane { 
private:
  Momentum impact;
  double vertical, horizontal;
public:
  virtual ~H3colli() { ; }
  H3colli(double length, double cutHorizontal, double cutVertical)
    : colliPlane(length, cutHorizontal, cutVertical)
  { impact.set(0,0,0); };
  H3colli(double length, double cutLeft, double cutRight, double cutTop,
	  double cutBottom)
    : colliPlane(length, cutLeft, cutRight, cutTop, cutBottom)
  { impact.set(0,0,0); };
  /// Check that particle does not fall into a specular H3
  int accepted(FourVector p) {
    return ( acos( ( (Momentum) p * normal) / p.abs() ) < 1.57 );
  };
  /// Make impact vector in the H3 plane
  int makeImpact(double x0[], FourVector p) {
    Momentum proton = (Momentum) p, vertex = (Momentum) x0;
    double theta = x.varTheta(); //, phi = x.varPhi();
    proton.rot_theta(-theta); // proton.rot_phi(-phi);
    vertex.rot_theta(-theta); // vertex.rot_phi(-phi);
    impact = ((l-vertex[2])/proton[2]) * proton + vertex;
    return (impact[2] - l < 1); // difference smaller than 1 mm
  }
  /// horizontal check with target position and direction of the particle
  /// in the same way as the grid is generated
  int checkHorizontal(double x0[], FourVector p) {
    horizontal = impact[0];
    return (horizontal < cutH && horizontal > cutH2);
  }
  /// vertical check with target position and direction of the particle
  /// in the same way as the grid is generated
  int checkVertical(double x0[], FourVector p) {
    vertical = impact[1];
    return (vertical < cutV && vertical > cutV2);
  };
  /// Get distance from target to center of H3
  double getLength() { return l; };
  double getH() { return horizontal; };
  double getV() { return vertical; };
};

// Collimators for MIT-Bates South Hall (OHIPS and OOPS) /////////////////////

/* Configuration    Drift     h/2     v/2    ph/2     th/2
   --------------------------------------------------------------------------
   OHIPS short     1169.3    37.5  86.444  32.059   73.794 (front collimator)
                   1361.85   43.0  97.563  31.564   71.518 (rear collimator!)

   OHIPS long      1569.3    37.5  86.444  23.891   55.029 (front collimator!)
                   1569.3    43.0  97.563  24.401   55.319 (rear collimator)

   OOPS short      1296.3  15.634  41.656  12.060   32.123 (front collimator!)

   OOPS long       2396.3  15.634  41.656   6.5241  17.381 (front collimator!)

   [ all linear dimensions in mm, angles in mrad ] 
   [ ! == defines the acceptance and is used in the code ] */

// const double OHIPS_SHORT_DISTANCE   =  1169.30; // front
// const double OHIPS_SHORT_HORIZONTAL = 0.032059; // front
// const double OHIPS_SHORT_VERTICAL   = 0.073794; // front
const double OHIPS_SHORT_DISTANCE   =  1361.85; // rear
const double OHIPS_SHORT_HORIZONTAL = 0.031564; // rear
const double OHIPS_SHORT_VERTICAL   = 0.071581; // rear

// const double OHIPS_LONG_DISTANCE   =  1761.85; // rear
// const double OHIPS_LONG_HORIZONTAL = 0.024401; // rear
// const double OHIPS_LONG_VERTICAL   = 0.055319; // rear
const double OHIPS_LONG_DISTANCE   =  1569.30; // front
const double OHIPS_LONG_HORIZONTAL = 0.023891; // front
const double OHIPS_LONG_VERTICAL   = 0.055029; // front

const double OOPS_SHORT_DISTANCE   = 1296.3;
const double OOPS_SHORT_HORIZONTAL = 0.012060;
const double OOPS_SHORT_VERTICAL   = 0.032123;

const double OOPS_LONG_DISTANCE   = 2396.3;
const double OOPS_LONG_HORIZONTAL = 0.0065241;
const double OOPS_LONG_VERTICAL   = 0.017381;

/// The collimator for OHIPS spectrometer
/// only a simple front collimator is used
class OHIPS_simple : public Collimator { 
private:
  colliPlane *verPlane, *horPlane;
  int init;
public:
  /// Constructor, acceptances in mrad, distance in mm
  OHIPS_simple(double hor, double ver, double dist);
  /// Destructor
  virtual ~OHIPS_simple() { delete verPlane; delete horPlane; };
  /// Check if particle is accepted by this collimator
  int accepted(double target[3], FourVector p,
	       double theta, double oop, double);
};

/// The collimator for OOPS spectrometer
/// only a simple front collimator is used
class OOPS_simple : public Collimator { 
private:
  colliPlane *verPlane, *horPlane;
  int init;
public: 
  /// Constructor, acceptances in mrad, distance in mm
  OOPS_simple(double hor, double ver, double dist);
  /// Destructor
  virtual ~OOPS_simple() { delete verPlane; delete horPlane; };
  /// Check if particle is accepted by this collimator
  int accepted(double target[3], FourVector p,
	       double theta, double oop, double);
};

/// The collimator for OHIPS spectrometer
/// a more fancy acceptance determination is used
class OHIPS_fancy : public Collimator { 
private:
  colliPlane *verPlane, *horPlane;
  double refMom;
  int init;
public:
  /// Constructor, acceptances in mrad, distance in mm
  OHIPS_fancy(double hor, double ver, double dist);
  /// Destructor
  virtual ~OHIPS_fancy() { delete verPlane; delete horPlane; };
  /// Check if particle is accepted by this collimator
  int accepted(double target[3], FourVector p,
	       double theta, double oop, double refmom);
};

/// The collimator for OOPS spectrometer
/// a more fancy acceptance determination is used
class OOPS_fancy : public Collimator { 
private:
  colliPlane *verPlane, *horPlane;
  double refMom;
  int init;
public: 
  /// Constructor, acceptances in mrad, distance in mm
  OOPS_fancy(double hor, double ver, double dist);
  /// Destructor
  virtual ~OOPS_fancy() { delete verPlane; delete horPlane; };
  /// Check if particle is accepted by this collimator
  int accepted(double target[3], FourVector p,
	       double theta, double oop, double refmom);
};

/// The collimator for OHIPS spectrometer
/// a qspin simulation determines the acceptance
class OHIPS_qspin : public Collimator { 
private:
  colliPlane *verPlane, *horPlane;
  double refMom;
  int init;
public:
  /// Constructor, acceptances in mrad, distance in mm
  OHIPS_qspin(double hor, double ver, double dist);
  /// Destructor
  virtual ~OHIPS_qspin() { delete verPlane; delete horPlane; };
  /// Check if particle is accepted by this collimator
  int accepted(double target[3], FourVector p,
	       double theta, double oop, double refmom);
};

/// The collimator for OOPS spectrometer
/// a qspin simulation determines the acceptance
class OOPS_qspin : public Collimator { 
private:
  colliPlane *verPlane, *horPlane;
  double refMom;
  int init;
public: 
  /// Constructor, acceptances in mrad, distance in mm
  OOPS_qspin(double hor, double ver, double dist);
  /// Destructor
  virtual ~OOPS_qspin() { delete verPlane; delete horPlane; };
  /// Check if particle is accepted by this collimator
  int accepted(double target[3], FourVector p,
	       double theta, double oop, double refmom);
};


/// The collimator for KAOS
class KAOScolli : public Collimator { 
private:
  colliPlane *Plane1, *Plane2;
  int    init;
public:
  virtual ~KAOScolli() { delete Plane1; delete Plane2; };

  KAOScolli();

  /// Check if particle is accepted by this collimator
  int accepted(double target[3], FourVector p,
	       double theta, double oop, double);
};



/// list of known collimators for spectrometer A
extern Collimator *ColliAList[];

/// list of known collimators for spectrometer B
extern Collimator *ColliBList[];

/// list of known collimators for spectrometer C
extern Collimator *ColliCList[];

/// list of known collimators for spectrometer D
extern Collimator *ColliDList[];

/// list of known collimators for OHIPS
extern Collimator *ColliOhipsList[];

/// list of known collimators for OOPSA
extern Collimator *ColliOopsAList[];

/// list of known collimators for OOPSB
extern Collimator *ColliOopsBList[];

/// list of known collimators for OOPSC
extern Collimator *ColliOopsCList[];

/// list of known collimators for OOPSD
extern Collimator *ColliOopsDList[];

/// list of known collimators for KAOS
extern Collimator *ColliKAOSList[];

#endif /* __Colli_h__ */
