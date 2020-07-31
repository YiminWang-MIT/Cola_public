// Ingo Ewald           					-*-c++-*-

#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "FourVector/FourVector.h"
#include "Masses.h"

/** @@name Materials, Energyloss, and Radiationlength

    This group of classes handles materials like elements
    and compounds and their relevant properties like radiation length,
    density etc.
*/
//@@{
/// Abstract class of Materials
class Material 
{ 
private:
  /// Parameter Multiple Scattering
  virtual double ChiAlpha2(double p, double beta, double z) const = 0;
public:
  virtual ~Material() { ; }
  /// Density in \TEX{$g/cm^3$}
  double Density;
  /// Radiation length in \TEX{$g/cm^2$}
  double RadiationLength;
  /// Effective Radiation length in \TEX{$cm$}
  double ThicknessBremsstrahlung;
  /// Parameter for Landau Distribution
  double LandauPar;
  /// Ionisation potential in eV
  double IonisationPotential;
  /// Parameters Density Corr.
  double DC_C, DC_m, DC_x0, DC_x1, DC_a;
  /// Massnumber A
  double Massnumber;
  /// Chargenumber Z
  double Chargenumber;
  /// Numbernumber N of Atoms (for a compound)
  double Numbernumber;
  /// Parameter Multiple Scattering
  int Zs;
  /// Energyloss dE/dx*pathlength in GeV (mean value or most probable value) 
  virtual double  dEdx(Particle p, double pathlength) const = 0;
   /// Light-Output in a Scintilator dL/dx*pathlength 
  virtual double dLdx(Particle p, double pathlength) const = 0;
  /// 
  double theta_MS_g_onestep(Particle p, double old_mom, double x) const;
  /// Multiple Scattering angle in plane (gaussian distribution)
  double theta_MS_gauss(Particle p, double old_mom, double x) const;
  /// Perform Rotation with thetaMS on Particle 
  void MultiScat(Particle &part, double old_mom, double path) const; 
  /// Print properties
  void   print() const;
  ///
  double getZ() const {return Chargenumber;} 
  ///
  double getZaverage() const {return Chargenumber/Numbernumber;} 
  ///
  double getDensity() const {return Density;} 
  /// set density (used for pressurized gas target)
  void   setDensity(double NewDens) {Density = NewDens;} ;
  ///
  double getMassnumber() const {return Massnumber;} 
};

/// Elements
class Element : public Material
{
  friend class Compound;
  private: 
    int    Z;
    double A;
    void   IonisationPot();
    void   Radiationlength();
    void   Thicknessbremsstrahlung();
    void   LandauParameter();
    void   DensCorr();
    double ChiAlpha2(double p, double beta, double z) const;
  public:
    /// Constructor
    Element();
    /// Constructor
    Element(int z, double a, double Dens);
    Element(int z, double a, double Dens,double C, double dca, double m, double x0, double x1);
    double dEdx(Particle p, double pathlength) const; 
    double dLdx(Particle p, double pathlength) const;
    double BetheBloch(Particle p) const;
    double MostProbEloss(Particle p, double pathlength) const;
};

/// Mixtures of materials
class Compound : public Material
{
  private:
    Element e[7];
    double     nmax, nsum, n[7]; //---int
    double  A_sum, Z_sum;
    double  ChiAlpha2(double p, double beta, double z) const;
  public:
    double dEdx(Particle p, double pathlength) const;
     double dLdx(Particle p, double pathlength) const;
    /// Constructor
    Compound(const double Density, 
	     const Element e0, double n0, //--int
	     const Element e1, double n1,//--int
	     const Element e2 = Element(0,0,0), double n2=0, //--int
	     const Element e3 = Element(0,0,0), double n3=0, //--int
	     const Element e4 = Element(0,0,0), double n4=0, //--int
	     const Element e5 = Element(0,0,0), double n5=0, //--int
	     const Element e6 = Element(0,0,0), double n6=0); //--int
};

///
extern  Material * H1;
///
extern  Material * H2HighFlow;
///
extern  Material * H2LowFlow;
///
extern  Material * LH2;
///
extern  Material * LD2;
///
extern  Material * He3;
///
extern  Material * He4;
///
extern  Material * C12;
///
extern  Material * C12pure;
///

extern  Material * N14;

///
extern  Material * O16;

///
extern  Material * Al;
///
extern  Material * Si;
///
extern  Material * Be;
///
extern  Material * Cr;
///
extern  Material * Mn;
///
extern  Material * Fe;
///
extern  Material * Co;
///
extern  Material * Ni;
///
extern  Material * Cu;
///
extern  Material * Mo;
///
extern  Material * Ta;
///
extern  Material * W;

///
extern  Material * H2;
///
extern  Material * N2;
///
extern  Material * O2;
///
extern  Material *Heliumgas;
///
extern  Material *Argongas;
///
extern  Material * Air;
///
extern Material * FrozenAir;
///
extern  Material * Vacuum;
///
extern  Material * Water;
///
extern  Material * Snow;
///
extern  Material * Scintillator;
///
extern  Material * Havar;
///
extern  Material * Kapton;
///
extern  Material * Mylar;
///
extern  Material * CH2;
///
extern  Material * Isobutane;
///
extern  Material * Quartz;
///
extern  Material * Kevlar;
//@@}

#endif /* __MATERIAL_H__ */

