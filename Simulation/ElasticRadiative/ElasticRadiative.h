#ifndef __ElasticRadiative_h__
#define __ElasticRadiative_h__

/*
 * This file is imported by Yimin Wang from Olympus radiative generator.
 * 2019/01/12
 */

#include "../Generate.h"
#include <TFile.h>

#include "Math/GSLIntegrator.h"
#include "Math/GSLMCIntegrator.h"
#include "Math/Interpolator.h"

#include "radiative_helpers.hpp"

class OneIndex;
class TLorentzVector;

class generateElasticRadiative : public eventGenerator{
  // for a test
  PseudoRandom prndm;
protected:
  double BHmin, BHmax, RadCutOff;
private:
  double cosThetaMin,cosThetaDelta,thetaMin,thetaMax; // Lepton angle range
  double phiRange; 
  double phaseweight;
  void recalcWeight();
  double mP,me,alpha,muP,cmSqMeVSq,alphaCubedOver64PiSq,qSqDipole;

  // Cuts accessible to the user
  bool useDeltaECut;
  double deltaECut;
  double softFraction;
  bool pushPhoton;
  int thetaDistribution; // 0 for flat, 1 for 1/(1-x)**2 (Rutherford)

  bool usePointProtonFF;
  bool useKellyFF;

  // Classes needed for the five-fold differential cross section
  Gamma * pGamma;
  IFourMat * pI4;
  Sigma * pSigma;
  
  // Objects needed for every event
  TLorentzVector p1,p2,p3,p4,k,kMod;
  //ElasticKinematics intElKin; // For the integration of the soft region
  double interTheta;
  double inter_maxDeltaE;
  double inter_t;
  double inter_elE;

  double k_cut;
  double method3_deltaE_cut;

  double d_p1_p1();
  double d_p1_p2();
  double d_p1_p3();
  double d_p1_p4();
  double d_p2_p2();
  double d_p2_p3();
  double d_p2_p4();
  double d_p3_p3(double E3);
  double d_p3_p4();
  double d_p4_p4(double E4);

  // Integrators
  ROOT::Math::GSLIntegrator *i_p1_p2;
  ROOT::Math::GSLIntegrator *i_p1_p3;
  ROOT::Math::GSLIntegrator *i_p1_p4;
  ROOT::Math::GSLIntegrator *i_p2_p3;
  ROOT::Math::GSLIntegrator *i_p2_p4;
  ROOT::Math::GSLIntegrator *i_p3_p4;
  ROOT::Math::GSLMCIntegrator *i_sample;

  // Interpolators
  ROOT::Math::Interpolator *inter_vpol;
  ROOT::Math::Interpolator *inter_brem_ee;
  ROOT::Math::Interpolator *inter_brem_ep;
  ROOT::Math::Interpolator *inter_brem_pp;
  ROOT::Math::Interpolator *inter_virt;
  ROOT::Math::Interpolator *inter_prime;
  ROOT::Math::Interpolator *inter_sample;

  // Interpolators for Jan's form factor fits
  ROOT::Math::Interpolator *inter_splineGE;
  ROOT::Math::Interpolator *inter_splineGEupper;
  ROOT::Math::Interpolator *inter_splineGElower;
  ROOT::Math::Interpolator *inter_splineGM;
  ROOT::Math::Interpolator *inter_splineGMupper;
  ROOT::Math::Interpolator *inter_splineGMlower;

  class Btilde {
    private:
      const GeneratorRadiative& g;
      const TLorentzVector& v1;
      const TLorentzVector& v2;
    public:
      Btilde(const GeneratorRadiative& parent, 
          const TLorentzVector& i1, const TLorentzVector& i2) 
        : g(parent), v1(i1), v2(i2) {};
      double operator() (double x) const;
  };

  class intSampletoKcut {
    private:
      const GeneratorRadiative& g;
    public:
      intSampletoKcut(const GeneratorRadiative& parent)
        : g(parent) {};
      double operator() (const double *x) const;
  };

  // Arrays for interpolation:
  double s[10000];
  double rep[10000];
public:
  virtual ~generateElasticRadiative() { ; }
  generateElasticRadiative(reaction *r, simDetectorBase *Sime, 
			 simDetectorBase *Sim1, SIMUL *rundb) {
    //ignore skip and seed at this moment
    Label      = "d[W]'";
    Unit       = "[m]b";
    Reaction   = r; sime=Sime; sim1=Sim1;
    targetmass = Reaction->getTarget()->getMass();
    dcte       = sin(sime->getDtheta());
    dphie      = calc_dphie();
    Ex         = rundb->excitation;
    Gamma      = 1;
    BHmin      = rundb->BHmin;
    BHmax      = rundb->BHmax;
    RadCutOff  = rundb->RadiationCutOff;

    
    //need to change the next two lines
    //also look for generator base file
    mP = m_proton; //getMass("proton");
    me = m_electron; //getMass("e-");
    alpha = 7.2973525698E-3;
    muP = 2.7928456;
    alphaCubedOver64PiSq = alpha * alpha * alpha / (64* M_PI * M_PI);
    cmSqMeVSq = 3.8937966E-22;
    qSqDipole = 710000.; // MeV^2
    usePointProtonFF = false;
    useKellyFF = false;

    pGamma = new Gamma();
    pI4 = new IFourMat();
    pSigma = new Sigma();
  };
  
  double generateEvent(double helicity);
  double integrationVolume() { return 4 * sime->getDphi() * dcte; }

  // from cooker
  void Initialize();
  //virtual int generateEvent(GeneratorEvent *eventinfo); //eventinfo is supplied from the outside (to write to trees directly). Return : positive: Number of tracks. Negative: Error.  This should be overloaded by the real generators.
  double bornCrossSection(const ElasticKinematics &e, int FFTypeE, int FFTypeM); // in units of cm^2 / sr
  void bremMatrixElement(double &lepTerm, double &mixTerm, double &hadTerm, int FFTypeE, int FFTypeM); // This is a modified matrix element, without two powers of k in the denom.
  double calcElasticCorr(const ElasticKinematics &e); // Calculate Maximon and Tjon correction that doesn't depend on deltaE
  double getMTjDelta(const ElasticKinematics &e, double deltaE); // Calculate whole Maximon and Tjon correction
  double calcElasticCorrMoTsai(const ElasticKinematics &e); // Calculate Mo and Tsai correction that doesn't depend on deltaE
  double calcElasticCorrMeisYen(const ElasticKinematics &e); // Calculate Mo and Tsai correction that doesn't depend on deltaE

  // Functions needed for converting between k and deltaE
  double photonMom(double cosThetaE, double cosThetaK, double cosThetaEK, double mom3, double E3);
  double jacKDeltaE(double cosThetaK, double cosThetaEK, double Eel, double E3);

  // Functions needed to generate the angle of the photon
  double photonDirFcn(double cosTheta, double lEnergy) const;
  double getLeptonSlowness(double lEnergy) const;
  double photonDirCumDist(double cosTheta, double lEnergy) const;
  double cosThetaKfromRel(double thetaE, double thetaEK, double phiEK) const;
  double phiKfromRel(double thetaE, double phiE, double thetaEK, double phiEK) const;
  double cosThetaEKfromGlob(double thetaE, double phiE, double thetaK, double phiK) const;
  double phiEKfromGlob(double thetaE, double phiE, double thetaK, double phiK) const;
  double genPhotonCosTh(double lEnergy, double real) const;

  // Functions needed to generate the five-fold differential cross section
  void fillProtonCurrent(OneIndex &current, const TLorentzVector &photon, int FFTypeE, int FFTypeM);
  double diracFormFactor(double QSq, int FFTypeE, int FFTypeM);
  double pauliFormFactor(double QSq, int FFTypeE, int FFTypeM);
  double dipoleFormFactor(double QSq);
  double electricFormFactor(double QSq, int formFactorType);
  double magneticFormFactor(double QSq, int formFactorType);

  // User functions
  void setThetaRange(double thetamin,double thetamax); //in degrees
  void setPhiRange(double phirange);  // in degrees, phi range will be  0+-range and 180+-range.
  void setDeltaECut(bool use, double cut);
  void setSoftFraction(double frac);
  void setPushPhoton(bool push);
  void setThetaDistribution(int d);
  void addPointProtonFFCalc(bool b);
  void addKellyFFCalc(bool b);

  double getVpolLep(double Q2, double mass); // vacuum polarization from a lepton

  // Test Functions
  void test1();
  void test2();
  double test_integral(double thetaE, double phiE, double deltaE, ElasticKinematics &el); 
  // returns d\sigma / d\Omega_e d\Delta_e in units of cm^2 / sr / MeV
  void test3();
  double test_born(double thetaE); // returns d\sigma / d\Omega_e in units of cm^s / sr
  void test4();
  void test5();
};

#endif
