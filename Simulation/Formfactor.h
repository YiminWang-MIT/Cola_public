//                                                                    -*-C++-*-
// $Id: Formfactor.h 2663 2015-11-11 23:51:56Z distler $

#include "Cola/Masses.h"
#include "Model/Model.h"

//#include <iostream>
//using namespace std;

class FormfactorBase {
public:
  virtual ~FormfactorBase() { ; }
  inline double mott(double M, double E0, double theta) const {
    double sinTh2 = square(sin(0.5*theta));
    return mubarn
      *square(alpha/(2*E0*sinTh2))*(1-sinTh2)/(1+2*E0/M*sinTh2);
  };
  virtual double CrossSection(double E0, double theta) const = 0;
};

class NucleonFormfactor : public FormfactorBase {
public:
  virtual ~NucleonFormfactor() { ; }
  virtual double CrossSection(double E0, double theta) const { 
    double Eprime = E0/(1 + 2 * E0 / m_proton * sqr(sin(theta/2)));
    double q2     = - 4 * E0 * Eprime * sqr(sin(theta/2));
    double tau    = -q2 / 4 / sqr(m_proton);

     return mott(m_proton, E0, theta) *
          ((sqr(G_E(q2)) + tau * sqr(G_M(q2)))/(1+tau) +
           2*tau*sqr(G_M(q2)*tan(theta/2)));
    // return mott(m_proton, E0, theta) *
    //      ((sqr(G_E(q2)) + tau * sqr(G_M(q2)))/(1+tau)* sqr(cos(theta/2)) +
    //       2*tau*sqr(G_M(q2)*sin(theta/2)));
  };
  virtual double F1(double q2) const  { 
    double tau = -q2/4/sqr(m_proton);
    return (G_E(q2) + tau * G_M(q2))/(1+tau);
  };
  virtual double F2(double q2) const  { 
    double tau = -q2/4/sqr(m_proton);
    return (G_M(q2) - G_E(q2))/(1+tau);
  };

  virtual double G_E(double q2)const{return F1(q2)+q2/4/sqr(m_proton)*F2(q2);};
  virtual double G_M(double q2)const{return F1(q2)+                   F2(q2);};

};

/// P. Mergell, U-G. Meißner, D. Drechsel, Nucl.Phys. A596 (1996) 367-396

class MergellFit : public NucleonFormfactor {
private:
  inline double F_1s(double Q2) const {
    return ( 9.464/(0.611 + Q2) - 9.054/(1.039 + Q2) - 0.410/(2.560 + Q2))
      * pow(log((9.733 + Q2) / 0.350), -2.148);
  };
  inline double F_2s(double Q2) const {
    return (- 1.549/(0.611 + Q2) + 1.985/(1.039 + Q2) - 0.436/(2.560 + Q2)) 
      * pow(log((9.733 + Q2)/0.350), -2.148);
  }; 
  inline double F_1v(double Q2) const {
    return ( (1.032   * pow(log((9.733 - 0.500)/0.350), 2.148)
	      + 0.088 * pow(log((9.733 - 0.400)/0.350), 2.148)
	      * pow(1 + Q2/0.318, -2))/(2*(1 + Q2/0.550))
	     - 38.885/(2.103 + Q2) + 425.007/(2.734 + Q2) - 389.742/(2.835+Q2) 
	     ) * pow(log((9.733+Q2)/0.350),-2.148);
  };
  inline double F_2v(double Q2) const {
    return  ( (5.782   * pow(log((9.733 - 0.500)/0.350), 2.148)
	       + 0.391 * pow(log((9.733 - 0.400)/0.350), 2.148)
	       / (1 + Q2/0.142)) / ( 2 * (1+Q2/0.536))
	      - 73.535/(2.103+Q2) + 83.211/(2.734+Q2) - 29.467/(2.835+Q2)
	      ) * pow(log((9.733+Q2)/0.350),-2.148);
  };
public:
  virtual ~MergellFit() { ; }
  inline double F1 (double q2) const {return F_1s(-q2) + F_1v(-q2); };
  inline double F2 (double q2) const {return F_2s(-q2) + F_2v(-q2); };
};

class DipoleFit : public NucleonFormfactor {
  inline double dipole_fit(double q2) const { return 1/sqr(1 - q2/0.71);   };
public:
  virtual ~DipoleFit() { ; }
  inline double G_E(double q2) const { return              dipole_fit(q2); };
  inline double G_M(double q2) const { return 2.79284739 * dipole_fit(q2); };
};

//////////////von Jan/////////////////////////

class DipoleGalsterFit : public NucleonFormfactor {
  inline double dipole_fit(double q2) const { return 1/sqr(1 - q2/0.71);   };
public:
  virtual ~DipoleGalsterFit() { ; }
  inline double G_E(double q2) const { 
    double tau=-q2/4/m_neutron/m_neutron;
    return     1.25*tau/(1+18.3*tau)*1.91304273* dipole_fit(q2); };
  inline double G_M(double q2) const { return -1.91304273 * dipole_fit(q2); };
};
/////////////////////////////////////////////

#define ea10 1.041
#define ea11 0.765
#define ea20 -0.041
#define ea21 6.2
#define eab -0.23
#define eqb 0.07
#define esb 0.27

#define ma10 1.002
#define ma11 0.749
#define ma20 -0.002
#define ma21 6.0
#define mab -0.13
#define mqb 0.35
#define msb 0.21

class FWFit : public NucleonFormfactor {
  inline double GSe(double q2) const { return ea10/sqr(1.0-q2/ea11)+ea20/sqr(1.0-q2/ea21) ;};
  inline double GSm(double q2) const { return ma10/sqr(1.0-q2/ma11)+ma20/sqr(1.0-q2/ma21) ;};
  inline double Gbe(double q2) const {return exp(-0.5*sqr((sqrt(-q2)-eqb)/esb))+ exp(-0.5*sqr((sqrt(-q2)+eqb)/esb)) ;};
  inline double Gbm(double q2) const {return exp(-0.5*sqr((sqrt(-q2)-mqb)/msb))+ exp(-0.5*sqr((sqrt(-q2)+mqb)/msb)) ;};

public:
  inline double G_E(double q2) const { return             GSe(q2)-eab*q2*Gbe(q2) ; };
  inline double G_M(double q2) const { return 2.79284739 *  (GSm(q2)-mab*q2*Gbm(q2)); };
};

// J. Arrington, W. Melnitchouk, J. A. Tjon, Phys. Rev. C 76, 035205 (2007)

class ArringtonFit : public NucleonFormfactor {
public:
  inline double G_M(double q2) const {
    const double a1 =-1.465, a2 = 1.260, a3 = 0.262;
    const double b1 = 9.627, b4 =11.179, b5 =13.245;
    double tau = q2 * -0.28397637376725874434;
    return 2.79284739 * (1+tau*(a1+tau*(a2+tau*a3)))/(1+tau*(b1+tau*tau*tau*(b4+tau*b5)));
  }
  inline double G_E(double q2) const {
    const double a1 =  3.439, a2 = -1.602, a3 =  0.068;
    const double b1 = 15.055, b2 = 48.061, b3 = 99.304, b4 =  0.012, b5 =  8.650;
    double tau = q2 * -0.28397637376725874434;
    return (1+tau*(a1+tau*(a2+tau*a3)))
      / (1+tau*(b1+tau*(b2+tau*(b3+tau*(b4+tau*b5)))));
  }
};

// M. A. Belushkin, H.W. Hammer, U.-G. Meißner,  Phys. Rev. C 75, 035202 (2007)

class HammerFit : public NucleonFormfactor {
  static const double gmp[][2],gep[][2];
public:
  inline double G_E(double q2) const {  
    if (q2>5.0) {
      cerr<<" Formfactor range for this parametrization exceeded\n";
    }
    double dx = -q2<1.0 ? 0.01 : 0.1;
    int    o  = -q2<1.0 ? 0 : 90;
    int    i  = -q2/dx;
    double g  = gep[o+i  ][1] * (i+1 + q2/dx)
      -         gep[o+i+1][1] * (i   + q2/dx);
    return g/pow(1-q2/0.71,2);
  }

  inline double G_M(double q2) const {  
    if (q2>5.0) {
      cerr<<" Formfactor range for this parametrization exceeded\n";
    }
    double dx = -q2<1.0 ? 0.01 : 0.1;
    int    o  = -q2<1.0 ? 0 : 90;
    int    i  = -q2/dx;
    double g  = gmp[o+i  ][1] * (i+1 + q2/dx)
      -         gmp[o+i+1][1] * (i   + q2/dx);
    return 2.79284739 * g/pow(1-q2/0.71,2);
  }

};

static const double hbarc = 0.1973269718; // GeV fm

// units: [Q] = 1/fm    [E0] = GeV

class DeuteronFormfactor : public FormfactorBase {
public:
  virtual ~DeuteronFormfactor() { ; }
  virtual double CrossSection(double E0, double theta) const { 
    return mott(m_deuteron, E0, theta) * F(E0, theta);
  };
  virtual double A(double Q) const  {
    double eta = square(0.5*hbarc*Q/m_deuteron);
    return square(G_C(Q)) + 8.0/9.0*square(eta*G_Q(Q)) + 2.0/3.0*eta*square(G_M(Q));
  };
  virtual double B(double Q) const  { 
    double eta = square(0.5*hbarc*Q/m_deuteron);
    return 4.0/3.0*eta*(1+eta)*square(G_M(Q));
  };
  virtual double F(double E0, double theta) const  { 
    double cosm1 = 1-cos(theta);
    double Q = sqrt((2*square(E0/hbarc)*cosm1)/(1+E0/m_deuteron*cosm1));
    return A(Q)+B(Q)*square(tan(0.5*theta));
  };

  virtual double G_C(double Q) const = 0;
  virtual double G_Q(double Q) const = 0;
  virtual double G_M(double Q) const = 0;

};


// Coding of the three parametrisations given in:
// Abbott, D. and others: Phenomenology of the deuteron electromagnetic
// form-factors, Eur.Phys.J. A7 (2000) 421-427

// Sum of Gaussians as described in
// I.Sick: Model-independent nuclear charge densities from
// elastic electron scattering, Nucl.Phys.A218 (1974) 509-541.

class AbbottFit3 : public DeuteronFormfactor {
  static const double SoGR[24];
  static const double SoGC[24];
  static const double SoGQ[24];
  static const double SoGM[24];
  static const double gamma;
  inline double SoG(double Q, const double *const R, const double *const G) const {
    double total = G[0];
    for (int i=1; i<24; i++) {
      double Rgamma2 = square(R[i]/gamma);
      total += G[i]/(1+2*Rgamma2)*(cos(Q*R[i])+2*Rgamma2*sin(Q*R[i])/(Q*R[i]));
    }
    return total;
  }
public:
  virtual ~AbbottFit3() {}

  inline double G_C(double Q) const {
    return exp(-square(0.5*Q*gamma)) * SoG(Q, SoGR, SoGC);
  }
  inline double G_Q(double Q) const {
    return 25.83 * exp(-square(0.5*Q*gamma)) * SoG(Q, SoGR, SoGQ);
  }
  inline double G_M(double Q) const {
    return 1.714 * exp(-square(0.5*Q*gamma)) * SoG(Q, SoGR, SoGM);
  }
};


//__________________________________________________________________________
//
//
// Class for the Spline interpolation of the J.C. Bernauer data
//
//__________________________________________________________________________


class BernauerSplineFit : public NucleonFormfactor {
private:
  static const int NumberOfPoints;
  static const double BernauerPointsQ2[1000+1];
  static const double BernauerPointsGEp[1000+1];
  static const double BernauerPointsGMp[1000+1];
  double GEp2ndDerivatives[1000+1];
  double GMp2ndDerivatives[1000+1];
  inline double dipole_fit(double q2) const { return 1/pow(1 - q2/0.71,2); };
public:
  virtual ~BernauerSplineFit() { ; };
  BernauerSplineFit();
  double GEp(double q2) const;
  double GMp(double q2) const;
  inline double G_E(double q2) const { return GEp(q2); };
  inline double G_M(double q2) const { return 2.79284739 * GMp(q2); };
  
};

///////////////////////////////////////////////////////////////////////////////
//
// Class for the Spline fit to the Mainz+World+pol data
// J.C. Bernauer et al.: Phys.Rev. C90 (2014) 015206
//
// this parametrization is valid for 0 <= Q^2 < 40 (GeV/c)^2
//
///////////////////////////////////////////////////////////////////////////////

class SplinesWithVariableKnots : public NucleonFormfactor {
  static const double knots[];
public:
  static double *bsplineCoeff(int p, double u);
public:
  virtual ~SplinesWithVariableKnots() { ; };
  double GEp(double q2) const;
  double GMp(double q2) const;
  inline double G_E(double q2) const { return GEp(q2); };
  inline double G_M(double q2) const { return 2.79284739 * GMp(q2); };
};

///////////////////////////////////////////////////////////////////////////////

class NuclearFormFactor {
public:
  virtual ~NuclearFormFactor() { ; }
  inline double mott(double Z, double M, double E0, double theta) const {
    double sinTh2 = square(sin(0.5*theta));
    return mubarn
      *square(Z*alpha/(2*E0*sinTh2))*(1-sinTh2)/(1+2*E0/M*sinTh2);
  };
  virtual double CrossSection(double E0, double theta) const = 0; 
  virtual double F(double E0, double theta) const = 0; // this is actually the square of FF

};   


//______________________________________________________________________________
//
// Helm's Model:
//
// Helms model is an easy model to parameterize form-factors
// of heavy nuclei. The charge distribution is a simple convolution
// of a uniform sphere and a gaussian tail. 
// See:
// Friedrich et al., Nuclear Physics A373 (1982) 192
//______________________________________________________________________________


class HelmsModel : public NuclearFormFactor {
private:
  double M;
  double Z;
  double R;
  double a;
public:
  HelmsModel(double ZZ, double MM, double RR, double aa) { Z = ZZ; M = MM; R = RR; a = aa;}
  virtual ~HelmsModel() {}

  double F(double E0, double theta) const {
    double Eprime = E0/(1 + 2 * E0 / M * pow(sin(theta/2),2));
    double q2 = 4 * E0 * Eprime * pow(sin(theta/2),2);
    double qfm = sqrt(q2)/hbarc; 
    double F_gaus = exp(-0.5*pow(qfm*a,2));
    double F_sphere = qfm<1e-4 ? 1 : 3./pow(qfm*R,2)*(sin(qfm*R)/(qfm*R)-cos(qfm*R));
    double F = F_sphere*F_gaus;
    return pow(F,2);
  } 
  double CrossSection(double E0, double theta) const { 
    return mott(Z, M, E0, theta)*F(E0, theta);

  };
  
};

//______________________________________________________________________________
//
// Tantal FF Parameterization:
//
// Parameterization taken from:
// B. Povh Particles and Nuclei &&
// PHYSICAL REVIEW C VOLUME 26, 3 (1982)
//______________________________________________________________________________

class TantalumFormfactor : public NuclearFormFactor {
public:
  virtual ~TantalumFormfactor() { ; }
  inline double CrossSection(double E0, double theta) const { 
    return mott(73, m_Ta181, E0, theta)*F(E0, theta);
  };
  virtual double F(double E0, double theta) const = 0; // this is actually the square of FF
};

class TantalumUniformFit : public TantalumFormfactor {
private:
  static const double R = 7.70;// Equivalent Tantalum radius [fm], Stanford; 
public:
  virtual ~TantalumUniformFit() { ; }
  double F(double E0, double theta) const {
    double Eprime = E0/(1 + 2 * E0 / m_Ta181 * pow(sin(theta/2),2));
    double q2 = 4 * E0 * Eprime * pow(sin(theta/2),2);
    double qfm = sqrt(q2)/hbarc; 
    double F_sphere = qfm<1e-4 ? 1 : 3/pow(qfm*R,2)*(sin(qfm*R)/(qfm*R)-cos(qfm*R));
    return pow(F_sphere,2);
  } 
};

//______________________________________________________________________________
//
// NITROGEN FF Parameterization:
//
// Parameterization taken from:
//
// PHYSICAL REVIEW C VOLUME 2, NUMBER 6 DECEMBER 1970
// Scattering of High-Energy Electrons by Nitrogen-14 and -15*
// E. B. Dally
//______________________________________________________________________________

class NitrogenFormfactor : public NuclearFormFactor {
public:
  virtual ~NitrogenFormfactor() { ; }
  inline double CrossSection(double E0, double theta) const { 
    return mott(7, m_N14, E0, theta)*F(E0, theta);
  };

  virtual double F(double E0, double theta) const = 0;

};

class DallyFit : public NitrogenFormfactor {
private:
  static const double aa=1.75; //[fm]
  static const double mu = 30; //[in units of mu_N]
  static const double alpha = 0.44;
  static const double J = 1; // Ground state is 1+
  static const double ap = 0.63;
  static const double Z = 7;
  static const double A = 14;
  static const double Q = 1.52; //[fm**2]
public:
  virtual ~DallyFit() {}

  inline double FC0sq(double q2) const { //(q is in units of  [1/fm])
    double x = 1.0/4.0*q2*pow(aa,2);
    double d = 1.0/4.0 *q2*(pow(ap,2) - pow(aa,2)/A);
    double val = (1.0 - 2.0/3.0*(Z-2)/Z*x)/exp(x+d);
    return pow(val,2);
  }

  inline double FC2sq(double q2) const {//(q is in units of  [1/fm])
    double x = 1.0/4.0*q2*pow(aa,2);
    double d = 1.0/4.0 *q2*(pow(ap,2) - pow(aa,2)/A);
    double val = pow(q2,2)/180.0*(J+1)*(2.*J+3.)/J/(2.*J-1)*pow(Q/Z,2)/exp(2*(x+d));
    return val;
  }

  inline double FM1sq(double q2) const {//(q is in units of  [1/fm])
    double x = 1.0/4.0*q2*pow(aa,2);
    double d = 1.0/4.0 *q2*(pow(ap,2) - pow(aa,2)/A);
    double mp = m_proton/hbarc; //[GeV-> 1/fm]. 
    double val = 2./3.*(J+1)/J*x/pow(aa*mp,2)*pow(mu/Z,2)*pow(1-2./3.*x+alpha*2./3.*x,2)/exp(2*(x+d));
    return val;
  }

  inline double FLsq(double q2) const {//(q is in units of  [1/fm])
    return FC0sq(q2)+FC2sq(q2);
  }

  inline double FTsq(double q2) const {//(q is in units of  [1/fm])
    return FM1sq(q2);
  }

  double F(double E0, double theta) const {
    double Eprime = E0/(1 + 2 * E0 / m_N14 * sqr(sin(theta/2)));
    double q2     = 4 * E0 * Eprime * sqr(sin(theta/2));
    double qfm = sqrt(q2)/hbarc; //[1/fm];
    //std::cout<<"I am calculating nitrogen!  "<<qfm<<"  "<<theta<<std::endl;
    return FLsq(qfm*qfm) + (1./2. + pow(tan(1./2.*theta),2))*FTsq(qfm*qfm);
  }
};

//______________________________________________________________________________
//
// CARBON FF Parameterization
//
// Parameterization taken from:
//
// PHYSICAL REVIEW C VOLUME 26, 3 (1982)
// Reuter
//______________________________________________________________________________

class CarbonFormfactor : public NuclearFormFactor {
public:
  virtual ~CarbonFormfactor() { ; }
  inline double CrossSection(double E0, double theta) const { 
    return mott(6, m_C12, E0, theta)*F(E0, theta);
  };

  virtual double F(double E0, double theta) const = 0;
};

class HofstadterFit : public CarbonFormfactor {
private:
  static const double a = 2.46;// carbon radius [fm], Stanford; 
  static const double alp = 4.0/3.0;
public:
  virtual ~HofstadterFit() {}

  double F(double E0, double theta) const {
    double k = sqrt(3*(2+5*alpha)/2/(2+3*alpha));
    
    double Eprime = E0/(1 + 2 * E0 / m_C12 * pow(sin(theta/2),2));
    double q2 = 4 * E0 * Eprime * pow(sin(theta/2),2);
    double qfm = sqrt(q2)/hbarc; //[1/fm];
    double x = a*qfm; 
    return pow((1-alp*x*x/(2*k*k*(2+3*alp)))*exp(-x*x/4/k/k),2);
  }
};  


//______________________________________________________________________________
//
// Be9 FF Parameterization:
//
// Parameterization taken from:
// Hofstadter  Rev. Mod. Phys. 28 214 
//______________________________________________________________________________

class BerylliumFormfactor : public NuclearFormFactor {
public:
  virtual ~BerylliumFormfactor() { ; }
  inline double CrossSection(double E0, double theta) const { 
    return mott(4, m_Be9, E0, theta)*F(E0, theta);
  };
  virtual double F(double E0, double theta) const = 0; // this is actually the square of FF
};

class BerylliumUniformFit : public BerylliumFormfactor {
private:
  static const double R = 1.89;// Equivalent radius [fm]; 
public:
  virtual ~BerylliumUniformFit() { ; }
  double F(double E0, double theta) const {
    double Eprime = E0/(1 + 2 * E0 / m_Be9 * pow(sin(theta/2),2));
    double q2 = 4 * E0 * Eprime * pow(sin(theta/2),2);
    double qfm = sqrt(q2)/hbarc; 
    double F_sphere = qfm<1e-4 ? 1 : 3/pow(qfm*R,2)*(sin(qfm*R)/(qfm*R)-cos(qfm*R));
    return pow(F_sphere,2);
  } 
};

class BerylliumModifiedExpFit : public BerylliumFormfactor {
private:
  static const double R = 3.04;// Equivalent radius [fm]; 
public:
  virtual ~BerylliumModifiedExpFit() { ; }
  double F(double E0, double theta) const {
    double Eprime = E0/(1 + 2 * E0 / m_Be9 * pow(sin(theta/2),2));
    double q2 = 4 * E0 * Eprime * pow(sin(theta/2),2);
    double qfm = sqrt(q2)/hbarc; 
    double F_modexp = qfm<1e-4 ? 1 : pow(1+pow(R*qfm,2)/18.0,-3);
    return pow(F_modexp,2);
  } 
};

// Ottermann, C.R. et al.: Nucl.Phys. A436 (1985) 688-698

class OttermannHe3 : public FormfactorBase {
public:
  virtual ~OttermannHe3() { ; }
  virtual double CrossSection(double E0, double theta) const {
    const double mu = -2.12755;
    const double Z = 2;
    double Eprime = E0/(1 + 2 * E0 / m_He3 * sqr(sin(theta/2)));
    double q2     = 4 * E0 * Eprime * sqr(sin(theta/2));   
    double tau    = 1+q2 / 4 / sqr(m_He3);

    return mott(m_He3, E0, theta) *
      (sqr(Z*G_E(q2))/tau +
       (1/2/tau+sqr(tan(theta/2)))*2*sqr(mu*G_M(q2))*q2/sqr(2*m_proton));
  }
  virtual double G_E(double q2) const {
    const double A = 0.462/hbarc/hbarc;
    const double B = 0.176/hbarc/hbarc;
    const double C = 0.812/hbarc/hbarc;
    return exp(-A*q2)-B*q2*exp(-C*q2);
  }
  virtual double G_M(double q2) const {
    const double A = 0.48/hbarc/hbarc;
    const double B = 0.18/hbarc/hbarc;
    const double C = 0.85/hbarc/hbarc;
    return exp(-A*q2)-B*q2*exp(-C*q2);
  }
};

class OttermannHe4 : public FormfactorBase {
public:
  virtual ~OttermannHe4() { ; }
  virtual double CrossSection(double E0, double theta) const {
    const double Z = 2;
    double Eprime = E0/(1 + 2 * E0 / m_He4 * sqr(sin(theta/2)));
    double q2     = 4 * E0 * Eprime * sqr(sin(theta/2));    // *-1.0 (?)
    double tau    = 1+q2 / 4 / sqr(m_He4);                  // 1-q2 (?)
    return mott(m_He4, E0, theta) * sqr(Z*G_E(q2))/tau;
  }
  virtual double G_E(double q2) const {
    const double a = 0.316/hbarc;
    const double b = 0.675/hbarc;
    return (1-pow(sqr(a)*q2,6))*exp(-sqr(b)*q2);
  }
  virtual double G_M(double q2) const { return 0; }
};
