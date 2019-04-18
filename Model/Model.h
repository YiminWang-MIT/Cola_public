//                                                                    -*-c++-*-
// $Id: Model.h 2689 2017-05-31 10:21:51Z schlimme $
//

// This file contains three models derived from class model:
//
// - class ChPT        V. Bernard et al.,  Nucl. Phys. A607 (1996) 379
//                                         Nucl. Phys. A633 (1998) 695
// - class maid2000    D. Drechsel et al., Nucl. Phys. A645 (1999) 145
// - class SPwaves     7 Parameters: Const s waves, P waves like q

#ifndef __MODEL_H__
#define __MODEL_H__

#include <math.h>
#include <complex>
#include <iostream>
#include <fstream>
#include <ext/stdio_filebuf.h>

#include "Cola/Masses.h"

using namespace std;
using namespace __gnu_cxx;


typedef std::complex<double> double_complex;

enum modeltype {
  Isotropic,   PWIA,         BoundDelta,    Pi0Threshold,
  TripleLab,   TripleLabPP,  ElasticProton, ElasticDeuteron, ElasticDipole,
  ElasticNucleus,QuasiElasticNucleus,
  Elastic,     Maid,         ChPTh,         VCS,
  TwoBodyPWIA, TwoBodyHe3,   ThreeBodyPWIA, ThreeBodyHe3,
  Iso6D,       DBreakup,     He3eepn,       EtaMaid,
  He3nr,       TripleLabHe3, He3fast,       TriplePol,
  Inclusive,   kMaid,        DMBoson,       DMQEDBackground,ElasticCryogens,
  DMProton,    Mott,         ElasticNitrogen, ElasticNuclearPA, ElasticHavar,
  ElasticPolElectronPolProton, ElasticRadiative
};

extern "C" double mymaid_(int *iso, double *w, double *q2, double *costheta,
			  double h[6][2],  double f[6][2],
			  double ep[10][2], double em[10][2],
			  double mp[10][2], double mm[10][2],
			  double lp[10][2], double lm[10][2]);

extern "C" double kmaid_(int *ichan, double *QP2, double *W, double *TH,
			  double *ST, double *SL, double *STL,
			  double *STT, double *STLP);
// Definition for:      SUBROUTINE K_MAID(ICHAN,QP2,W,TH,ST,SL,STL,STT,STLP)

static const double alpha =  1.0/137.035989561; // fine-structure constant 
static const double mubarn = 389.37966;       // (h_bar c)^2 in  GeV^2 * mubarn
static const double e = sqrt(alpha * 4 * M_PI); // e in sqrt(h_bar*c)

//This is not necessary anymore because of the Cola/Masses.h include. There are these masses already defined
/*
#ifndef _MASSES_H_
static const double m_electron =  0.00051099907;
static const double m_eta =       0.54775;
static const double m_pi0 =       0.1349764;
static const double m_pion =      0.13956995;
static const double m_proton =    0.93827231;
static const double m_neutron =   0.93956563;
static const double m_Lambda =    1.115684;
static const double m_Sigma0 =    1.19255;

static const double m_kaon =      0.493677;
#endif
*/
inline double sqr(double a) { return a*a; }

class model {
protected:
public:
  char *name;
  static const double wc;        // pi+ threshold
  double W;
  double omega;
  double q;
  double rho;
  double y;
  double k2;
  double k0;
  double epsL;

  virtual ~model() { ; }

  //  virtual double_complex E0p()=0;
  //  virtual double_complex L0p()=0;
  virtual double_complex E0p() {return 0;};
  virtual double_complex L0p() {return 0;};
  virtual double_complex S0p() {return L0p()/k0*sqrt(k0*k0-k2);}

  // child should define only P ***OR*** E/M/L p wave multipoles
  virtual double_complex P1()  { return 3.0 * E1p() + M1p() - M1m(); };
  virtual double_complex P2()  { return 3.0 * E1p() - M1p() + M1m(); };
  virtual double_complex P3()  { return 2.0 * M1p() + M1m();         };
  virtual double_complex P4()  { return 4.0 * L1p() + L1m();         };
  virtual double_complex P5()  { return L1m() - 2.0 * L1p();         };

  virtual double_complex M1p() { return (P3() + (P1() - P2())/2.0)/3.0; };
  virtual double_complex M1m() { return (P3() + P2()  - P1())/3.0;      };
  virtual double_complex E1p() { return (P1() + P2())/3.0;              };
  virtual double_complex L1p() { return (P4() - P5())/6.0;              };
  virtual double_complex L1m() { return (P4() + 2.0*P5())/3.0;          };
  
  virtual double_complex P1(double deltaW, double k2) { 
    setKinematics(deltaW, k2);  return P1(); }
  virtual double_complex P2(double deltaW, double k2) { 
    setKinematics(deltaW, k2);  return P2(); }
  virtual double_complex P3(double deltaW, double k2) { 
    setKinematics(deltaW, k2);  return P3(); }
  virtual double_complex P4(double deltaW, double k2) { 
    setKinematics(deltaW, k2);  return P4(); }
  virtual double_complex P5(double deltaW, double k2) { 
    setKinematics(deltaW, k2);  return P5(); }
  virtual double_complex E0p(double deltaW, double k2) { 
    setKinematics(deltaW, k2);  return E0p(); }
  virtual double_complex L0p(double deltaW, double k2) { 
    setKinematics(deltaW, k2);  return L0p(); }
  virtual double_complex M1p(double deltaW, double k2) { 
    setKinematics(deltaW, k2);  return M1p(); }
  virtual double_complex M1m(double deltaW, double k2) { 
    setKinematics(deltaW, k2);  return M1m(); }
  virtual double_complex E1p(double deltaW, double k2) { 
    setKinematics(deltaW, k2);  return E1p(); }

  void setKinematics(double dW,double q2);
  virtual double sigma_0(double dW, double q2, double eps, double theta);
  virtual double sigma_T(double dW, double q2, double eps, double theta);
  virtual double sigma_L(double dW, double q2, double eps, double theta);
  virtual double sigma_TL(double dW, double q2, double eps, double theta);
  virtual double sigma_TLp(double dW, double q2, double eps, double theta);
  virtual double sigma_TT(double dW, double q2, double eps, double theta);
  virtual double R_T(double dW, double q2, double theta);
  virtual double R_L(double dW, double q2, double theta);
  virtual double R_TT(double dW, double q2, double theta);
  virtual double R_TL(double dW, double q2, double theta);
  virtual double R_TLp(double dW, double q2, double theta);
  virtual double CrossSection(double dW, double q2, double eps, 
			      double theta, double phi, double h=0);
  virtual void P_CMS(double P[3],double dW, double q2, double eps, 
		     double theta, double phi, double h) {P[0]=P[1]=P[2]=0;}
};

///////////////////////////////////////////////////////////////////////////////
// Model of  V. Bernard et. al. Nucl. Phys. A607 (1996) 379-401
///////////////////////////////////////////////////////////////////////////////

class ChPT : public model {
private:
  static const double delta_r1p; // proton radius
  static const double lambda;    // suspicious regularization parameter
  static const double g_piN;     // strong pion-nucleon coupling
  static const double F_pi;      // pion decay constant
  static const double kappa;     // proton anomalous magnetic moment 
  static const double g_A;       // axial coulping

  static double a1; // Low Energy Constants (LEC) in GeV^-4
  static double a2; // GeV^-4 only a1+a2 is needed
  static double a3; // GeV^-4
  static double a4; // GeV^-4
  static double a5; // GeV^-5
  static double bp; // GeV^-3

  double E0_set, L0_set;

  double_complex sqrt_y_1(double y) {
    return (y<1 ? double_complex(sqrt(1-y),0) :
	    double_complex(0,-sqrt(y-1)));
    //  return sqrt((double_complex)(1-y)); Warum geht das nicht?!?!?!?
  }
  
  double_complex H(double y, double r) { 
    return atan2(r, 2 * sqrt(y+r)) + 
      (y < 1 ? double_complex(asin((r+2*y) / sqrt(4*r+r*r+4*y)), 0) :
       double_complex(M_PI_2,
		      log((r+2*y+2*sqrt((y+r)*(y-1))) / sqrt(4*r+r*r+4*y)))); 
  }
public:
  ChPT() {E0_set=0;L0_set=0;}
  virtual ~ChPT() { ; }
  void setSwaves(double E0, double L0, double q2);
  void setPwaves(double b){ bp = b; }
  void setModel (double a1, double a2, double a3, double a4, 
		 double a5, double bp) {
    this->a1 = a1;    this->a2 = a2;    this->a3 = a3;    this->a4 = a4;
    this->a5 = a5;    this->bp = bp;
  }

  double_complex P1();
  double_complex P2();
  double_complex P3();
  double_complex P4();
  double_complex P5();
  double_complex E0p();
  double_complex L0p();
  double_complex E0p(double k2);
  double_complex L0p(double k2);
};

///////////////////////////////////////////////////////////////////////////////
// 7 parameter model: constant swaves, pwaves proportional to pion cm momentum

class SPwaves : public model {
private:
  
  double p1;
  double p2;
  double p3;
  double p4;
  double p5;

  double_complex E0, L0;
public:
  SPwaves(const double p[7]) {
    p1 = p[0];    p2 = p[1];    p3 = p[2];    p4 = p[3];    p5 = p[4];
    E0 = p[5];    L0 = p[6];
  }

  SPwaves(const double_complex p[7]) {
    p1 = p[0].real();    
    p2 = p[1].real();    
    p3 = p[2].real();    
    p4 = p[3].real();    
    p5 = p[4].real();
    E0 = p[5];    L0 = p[6];
  }
  virtual ~SPwaves() { ; }

  double_complex P1() { return q * p1;}
  double_complex P2() { return q * p2;}
  double_complex P3() { return q * p3;}
  double_complex P4() { return q * p4;}
  double_complex P5() { return q * p5;}
  double_complex E0p(){ return E0;}
  double_complex L0p(){ return L0;}
};

///////////////////////////////////////////////////////////////////////////////
class maid2000 : public model {
protected:
  int isospin;
  double m_initial, m_final, m_meson;
  double ep[10][2], em[10][2];
  double mp[10][2], mm[10][2];
  double lp[10][2], lm[10][2];
  double_complex h1, h2, h3, h4, h5, h6;
  double_complex f1, f2, f3, f4, f5, f6;
  
public: 

  maid2000(int isospin = 1) {
    maid2000::isospin = isospin;
    m_initial = isospin==1 || isospin==3 ? m_proton : m_neutron;
    m_final   = isospin==1 || isospin==4 ? m_proton : m_neutron;
    m_meson   = isospin==1 || isospin==2 ? m_pi0    : m_pion;
    name = "Maid";
  };
  virtual ~maid2000() { ; }

  double ps(double dW, double q2, double theta)
  {  
    double H[6][2];
    double F[6][2];
    double costheta = cos(theta);
    W = dW + m_final + m_meson;
    k0   = (W * W - sqr(m_initial) + q2) / 2 / W;
    double Q2 = -q2;
    mymaid_(&isospin, &W, &Q2, &costheta, H, F, ep,em,mp,mm,lp,lm);
    double k_gamma_eq  = (W*W - pow(m_initial,2))/2/W;
    double E_pion_cms  = (W*W + pow(m_meson,2) - pow(m_final,2))/2/W;
    double p_pion_cms  = sqrt(pow(E_pion_cms,2) - pow(m_meson,2));
    
    h1=double_complex(H[0][0],H[0][1]); h2=double_complex(H[1][0],H[1][1]);
    h3=double_complex(H[2][0],H[2][1]); h4=double_complex(H[3][0],H[3][1]);
    h5=double_complex(H[4][0],H[4][1]); h6=double_complex(H[5][0],H[5][1]);
    f1=double_complex(F[0][0],F[0][1]); f2=double_complex(F[1][0],F[1][1]);
    f3=double_complex(F[2][0],F[2][1]); f4=double_complex(F[3][0],F[3][1]);
    f5=double_complex(F[4][0],F[4][1]); f6=double_complex(F[5][0],F[5][1]);
    return mubarn * 1e6 * p_pion_cms / k_gamma_eq;
  };
  double sigma_0(double dW, double q2, double eps, double theta) {
    double phasespace=ps(dW, q2, theta);
    return phasespace * ((norm(h1) + norm(h2) + norm(h3)+norm(h4))/2
		 + eps * -q2/sqr(k0) * (norm(h5) + norm(h6)));
  };
  double sigma_T(double dW, double q2, double eps, double theta)  {
    return ps(dW, q2, theta) * (norm(h1) + norm(h2) + norm(h3)+norm(h4))/2;
  };
  double sigma_L(double dW, double q2, double eps, double theta)  {
    return ps(dW, q2, theta) * (norm(h5) + norm(h6));
  };
  double sigma_TT(double dW, double q2, double eps, double theta) {
    return ps(dW, q2, theta) * real(conj(h2)*h3 - conj(h1)*h4);
  };
  // double sigma_TL(double dW, double q2, double eps, double theta) {
  //  return ps(dW, q2, theta)*real((h1-h4)*conj(h5)+(h2+h3)*conj(h6))/M_SQRT2;
  //};
  //  double sigma_TLp(double dW, double q2, double eps, double theta){
  //  return ps(dW, q2, theta)*-imag((h1-h4)*conj(h5)+(h2+h3)*conj(h6))/M_SQRT2;
    // ps(dW, q2, theta)*-sin(theta)*
    // imag(conj(f2+f3+cos(theta)*f4)*f5+conj(f1+f4+cos(theta)*f3)*f6);
    
  // };

  double_complex Ep(int l) { return double_complex(ep[l][0],ep[l][1]);}
  double_complex Em(int l) { return double_complex(em[l][0],em[l][1]);}
  double_complex Mp(int l) { return double_complex(mp[l][0],mp[l][1]);}
  double_complex Mm(int l) { return double_complex(mm[l][0],mm[l][1]);}
  double_complex Lp(int l) { return double_complex(lp[l][0],lp[l][1]);}
  double_complex Lm(int l) { return double_complex(lm[l][0],lm[l][1]);}

  double_complex M1p() { return Mp(1);}
  double_complex M1m() { return Mm(1);}
  double_complex E1p() { return Ep(1);}
  double_complex L1p() { return Lp(1);}
  double_complex L1m() { return Lm(1);}
  double_complex E0p() { return Ep(0);}
  double_complex L0p() { return Lp(0);}
};

///////////////////////////////////////////////////////////////////////////////
class etaMaid : public model {
protected:
  int isospin,m,n;
  double dx, min, max, angle, amin, amax, dy, Q2;
  double m_initial, m_final, m_meson;
  double ep[10][2], em[10][2];
  double mp[10][2], mm[10][2];
  double lp[10][2], lm[10][2];
  double_complex h1, h2, h3, h4, h5, h6;
  struct maiddata { double W, Hre[6], Him[6];} *data;
  
public: 

  etaMaid(char *filename);
  virtual ~etaMaid() { delete[] data; }

  double ps(double dW, double q2, double theta)
  {  
    q2 = -Q2;
    W = dW + m_final + m_meson;
    k0   = (W * W - sqr(m_initial) + q2) / 2 / W;
    
    if (W*1000>max || W*1000<min) return 0;
    double th = theta/M_PI*180;
    int j = (int) ((th-amin)/dy);
    int i= n*j + (int) ((W-min/1000)/(dx/1000));
    
    double k_gamma_eq  = (W*W - pow(m_initial,2))/2/W;
    double E_pion_cms  = (W*W + pow(m_meson,2) - pow(m_final,2))/2/W;
    double p_pion_cms  = sqrt(pow(E_pion_cms,2) - pow(m_meson,2));
    
    double_complex ah1=double_complex(data[i].Hre[0], data[i].Him[0]) * 1e-3 / m_pion; 
    double_complex ah2=double_complex(data[i].Hre[1], data[i].Him[1]) * 1e-3 / m_pion;
    double_complex ah3=double_complex(data[i].Hre[2], data[i].Him[2]) * 1e-3 / m_pion; 
    double_complex ah4=double_complex(data[i].Hre[3], data[i].Him[3]) * 1e-3 / m_pion;
    double_complex ah5=double_complex(data[i].Hre[4], data[i].Him[4]) * 1e-3 / m_pion; 
    double_complex ah6=double_complex(data[i].Hre[5], data[i].Him[5]) * 1e-3 / m_pion;
    double_complex bh1=double_complex(data[i+n].Hre[0], data[i+n].Him[0]) * 1e-3 / m_pion; 
    double_complex bh2=double_complex(data[i+n].Hre[1], data[i+n].Him[1]) * 1e-3 / m_pion;
    double_complex bh3=double_complex(data[i+n].Hre[2], data[i+n].Him[2]) * 1e-3 / m_pion; 
    double_complex bh4=double_complex(data[i+n].Hre[3], data[i+n].Him[3]) * 1e-3 / m_pion;
    double_complex bh5=double_complex(data[i+n].Hre[4], data[i+n].Him[4]) * 1e-3 / m_pion; 
    double_complex bh6=double_complex(data[i+n].Hre[5], data[i+n].Him[5]) * 1e-3 / m_pion;
    
    h1 = ah1*(1-fmod(th,dy)/dy) + bh1 * fmod(th,dy)/dy;
    h2 = ah2*(1-fmod(th,dy)/dy) + bh2 * fmod(th,dy)/dy;
    h3 = ah3*(1-fmod(th,dy)/dy) + bh3 * fmod(th,dy)/dy;
    h4 = ah4*(1-fmod(th,dy)/dy) + bh4 * fmod(th,dy)/dy;
    h5 = ah5*(1-fmod(th,dy)/dy) + bh5 * fmod(th,dy)/dy;
    h6 = ah6*(1-fmod(th,dy)/dy) + bh6 * fmod(th,dy)/dy;
    return (p_pion_cms / k_gamma_eq) * mubarn;
  };
  double sigma_0(double dW, double q2, double eps, double theta) {
    double phasespace=ps(dW, q2, theta);
    return phasespace * ((norm(h1) + norm(h2) + norm(h3)+norm(h4))/2
			 + eps * -q2/sqr(k0) * (norm(h5) + norm(h6)));
  };
  double sigma_T     (double dW, double q2, double eps, double theta) {
    return ps(dW, q2, theta) * (norm(h1) + norm(h2) + norm(h3)+norm(h4))/2;  };
  double sigma_L     (double dW, double q2, double eps, double theta) { return ps(dW, q2, theta) * (norm(h5) + norm(h6));  };
  double sigma_TT    (double dW, double q2, double eps, double theta) { return ps(dW, q2, theta) * real( conj(h2)*h3 - conj(h1)*h4);  };
  double sigma_TL    (double dW, double q2, double eps, double theta) { return ps(dW, q2, theta) * real((h1-h4)*conj(h5)+(h2+h3)*conj(h6))/M_SQRT2;  };
  double sigma_TLp   (double dW, double q2, double eps, double theta) { return ps(dW, q2, theta) *-imag((h1-h4)*conj(h5)+(h2+h3)*conj(h6))/M_SQRT2;  };
  double sigma_T_y   (double dW, double q2, double eps, double theta) { return ps(dW, q2, theta) * imag( conj(h1)*h3+conj(h2)*h4);  };
  double sigma_sTL_x (double dW, double q2, double eps, double theta) { return ps(dW, q2, theta) * imag( conj(h5)*(h2-h3)-conj(h6)*(h1+h4))/M_SQRT2;  };
  double sigma_sTL_z (double dW, double q2, double eps, double theta) { return ps(dW, q2, theta) * imag(-conj(h5)*(h2+h4)-conj(h6)*(h2+h3))/M_SQRT2;  };
  double sigma_sTT_x (double dW, double q2, double eps, double theta) { return ps(dW, q2, theta) * imag( conj(h1)*h2-conj(h3)*h4);  };
  double sigma_sTT_z (double dW, double q2, double eps, double theta) { return ps(dW, q2, theta) * imag(-conj(h1)*h4+conj(h2)*h3);  };
  double sigma_cTLp_x(double dW, double q2, double eps, double theta) { return ps(dW, q2, theta) * real(-conj(h5)*(h2-h3)+conj(h6)*(h1+h4))/M_SQRT2;  };
  double sigma_cTLp_z(double dW, double q2, double eps, double theta) { return ps(dW, q2, theta) * real(-conj(h5)*(h1+h4)+conj(h6)*(h2-h3))/M_SQRT2;  };
  double sigma_TTp_x (double dW, double q2, double eps, double theta) { return ps(dW, q2, theta) * real( conj(h1)*h3+conj(h2)*h4);  };
  double sigma_TTp_z (double dW, double q2, double eps, double theta) { return ps(dW, q2, theta) * (norm(h1)+norm(h2)-norm(h3)-norm(h4))/2;  };
  double sigma_L_y   (double dW, double q2, double eps, double theta) { return ps(dW, q2, theta) * 2 * imag(conj(h5)*h6);};
  double sigma_cTL_y (double dW, double q2, double eps, double theta) { return ps(dW, q2, theta) * imag(conj(h5)*(h2+h3)-conj(h6)*(h1-h4))/M_SQRT2;  };
  double sigma_cTT_y (double dW, double q2, double eps, double theta) { return ps(dW, q2, theta) * imag(conj(h1)*h2+conj(h3)*h4);  };
  double sigma_sTLp_y(double dW, double q2, double eps, double theta) { return ps(dW, q2, theta) * real(conj(h5)*(h2+h3)-conj(h6)*(h1-h4))/M_SQRT2;  };

  double Px(double dW, double q2, double eps, double theta, double phi) {
    double epsL = eps * -q2/k0/k0;
    return (   sqrt(2*epsL*(1+eps)) * sigma_sTL_x(dW, q2, eps, theta) * sin(phi) 
      +    eps                      * sigma_sTT_x(dW, q2, eps, theta) * sin(2*phi)) / CrossSection(dW, q2, eps, theta, phi);
  };
  double Pxh(double dW, double q2, double eps, double theta, double phi) {
    double epsL = eps * -q2/k0/k0;
    return (sqrt(2*epsL*(1-eps)) * sigma_cTLp_x(dW, q2, eps, theta) * cos(phi)
      +     sqrt(1-eps*eps)      * sigma_TTp_x(dW, q2, eps, theta)) / CrossSection(dW, q2, eps, theta, phi);
  };

  double Py(double dW, double q2, double eps, double theta, double phi) {
    double epsL = eps * -q2/k0/k0;
    return (                         sigma_T_y(dW, q2, eps, theta) 
	    + epsL                 * sigma_L_y(dW, q2, eps, theta)
	    + sqrt(2*epsL*(1+eps)) * sigma_cTL_y(dW, q2, eps, theta) * cos(phi) 
	    + eps                  * sigma_cTT_y(dW, q2, eps, theta) * cos(2*phi)) / CrossSection(dW, q2, eps, theta, phi);
  };
  double Pyh(double dW, double q2, double eps, double theta, double phi) {
    double epsL = eps * -q2/k0/k0;
    return (sqrt(2*epsL*(1-eps)) * sigma_sTLp_y(dW, q2, eps, theta) * sin(phi)) / CrossSection(dW, q2, eps, theta, phi);
  };


  double Pz(double dW, double q2, double eps, double theta, double phi) {
    double epsL = eps * -q2/k0/k0;
    return (   sqrt(2*epsL*(1+eps)) * sigma_sTL_z(dW, q2, eps, theta) * sin(phi) 
      +    eps                      * sigma_sTT_z(dW, q2, eps, theta) * sin(2*phi)) / CrossSection(dW, q2, eps, theta, phi);
  };
  double Pzh(double dW, double q2, double eps, double theta, double phi) {
    double epsL = eps * -q2/k0/k0;
    return (sqrt(2*epsL*(1-eps)) * sigma_cTLp_z(dW, q2, eps, theta) * cos(phi)
      +     sqrt(1-eps*eps)      * sigma_TTp_z(dW, q2, eps, theta)) / CrossSection(dW, q2, eps, theta, phi);
  };
  void P_CMS(double P[3],double dW, double q2, double eps, double theta, double phi, double h) {
    double p[]  = { Px(dW, q2, eps, theta, phi) + h * Pxh(dW, q2, eps, theta, phi),
    		    Py(dW, q2, eps, theta, phi) + h * Pyh(dW, q2, eps, theta, phi), 
    		    Pz(dW, q2, eps, theta, phi) + h * Pzh(dW, q2, eps, theta, phi)};
    //double p[]  = { h * Pxh(dW, q2, eps, theta, phi),
    //         	      h * Pyh(dW, q2, eps, theta, phi), 
    //	              h * Pzh(dW, q2, eps, theta, phi)};
    double p2[] = { p[0] * cos(phi) - p[1] * sin(phi), 
		    p[1] * cos(phi) + p[0] * sin(phi),
		    p[2]};
    P[0] =          p2[0] * cos(theta) + p2[2] * sin(theta);
    P[1] =          p2[1];
    P[2] =          p2[2] * cos(theta) - p2[0] * sin(theta);
  }

  double_complex Ep(int l) { return double_complex(ep[l][0],ep[l][1]);}
  double_complex Em(int l) { return double_complex(em[l][0],em[l][1]);}
  double_complex Mp(int l) { return double_complex(mp[l][0],mp[l][1]);}
  double_complex Mm(int l) { return double_complex(mm[l][0],mm[l][1]);}
  double_complex Lp(int l) { return double_complex(lp[l][0],lp[l][1]);}
  double_complex Lm(int l) { return double_complex(lm[l][0],lm[l][1]);}

  double_complex M1p() { return Mp(1);}
  double_complex M1m() { return Mm(1);}
  double_complex E1p() { return Ep(1);}
  double_complex L1p() { return Lp(1);}
  double_complex L1m() { return Lm(1);}
  double_complex E0p() { return Ep(0);}
  double_complex L0p() { return Lp(0);}
};


class kmaid : public model {

protected:
  int f_ichan;
public:
  kmaid(int Aichan=1);

  virtual double CrossSection(double W, double Q2, double eps, 
			      double theta, double phi, double h=0);

  // needed for kmaidtest:
  double sigma_0(double W, double q2, double eps, double theta);
  double sigma_T(double W, double q2, double eps, double theta);
  double sigma_L(double W, double q2, double eps, double theta);
  double sigma_TT(double W, double q2, double eps, double theta); 
  double sigma_TL(double W, double q2, double eps, double theta); 
  double sigma_TLp(double W, double q2, double eps, double theta); 
};

void sigaren(int& initflag, double k_i, double omega, double cos_th_e,
	     double q2, double qnu2, double jacobian,
	     double th_pq_cm_1, double ph_pq, double t_cm,
	     double q2_cm, int asym_flag, char nuc_type,
	     double& sig_mott,
	     double& v_l, double& v_t, double& v_lt, double& v_tt,
	     double& w_l, double& w_t, double& w_lt, double& w_tt);


#endif /* __MODEL_H__ */
//@
