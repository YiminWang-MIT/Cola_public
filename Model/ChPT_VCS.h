//----------------------------------------------------
// Virtual Compton Scattering
// HBChPT Model from nucl-th/9910036, Hemmert et al.
//----------------------------------------------------

// Units of measure:
// The input FourVectors are in GeV
// The P-functions are given in Fermi^3 or Fermi^4
// The electric and magnetic polarizabilities are givern in Fermi^3
// In the FormFactors (G_E(q2),G_M(q2)), q2 is in GeV
// The Structure Functions (P_LL etc.) are given in GeV^(-2)

// Formulas with the comment "VdH" are from: 
// P.Guichon and M.Vanderhaeghen, Progr.Part.Phys, Vol.41 (1998)
// (hep-ph/9806305) 

// Formulas with the comment "Guichon" are from:
// P.Guichon et al.,Nucl.Phys. A 591 (1995) 606-638          

// "Exact" refers to the analytic results from nucl-th/9910036
// "Expansion" refers to the power expansions of the "Exact" formulas

#ifndef ChPT_VCS_H
#define ChPT_VCS_H 

 
class ChPT_VCS
{
  static double scale[6];
  static double Struct[2];

 private:

  double w0 (FourVector q);
  double G (double x);

  //Electric and Magnetic Form Factors
  double G_E(double q2);
  double G_M(double q2);
 public:  
  //Generalized Polarizabilities
  double P01121(FourVector q);
  double P11021(FourVector q);
  double P11001(FourVector q);
  double P0111(FourVector q);
  double P01010(FourVector q);
  double P11110(FourVector q);
  double PA0111(FourVector q);
  double P01011(FourVector q);
  double P11111(FourVector q);

  //Electric and Magnetic Polarizabilities
  double Alpha(FourVector q);
  double Beta(FourVector q);
  

  static void setScale(double scale[]);
  static void setStruct(double scale[]);
  double P01010ScaleBoundaryCondition(FourVector q, double PLLminusPTToverEps, double eps);
  double P11110ScaleBoundaryCondition(FourVector q, double PLT);

  //Structure Functions
  double P_LL(FourVector q);
  double P_TT(FourVector q);
  double P_LT(FourVector q);
  double P_LTz(FourVector q);
  double P_LTz1(FourVector q);
  double P_LTo1(FourVector q);
  double P_LTo(FourVector q);
  double P_TTo(FourVector q);
  double P_TTo1(FourVector q);
  
  double K2(FourVector q, double pol);
  double kT (double theta, double phi, double pol, FourVector P_in);
  double w(double theta, double phi, double pol, FourVector P_in);
  double w1(double theta, double phi, double pol, FourVector P_in);
  double w2(double theta, double phi, double pol, FourVector P_in);

  double wphi(double theta, double phi,double pol, FourVector P_in);
  
  //Cross Sections
  double M0(FourVector photon,FourVector p_in, FourVector e_in,
	    FourVector e_out,FourVector q,double theta,double phi,
	    double pol);
  double Mz(FourVector photon,FourVector p_in, FourVector e_in,
	    FourVector e_out,FourVector q,double theta,double phi,
	    double pol,double hel);
  double Mx(FourVector photon,FourVector p_in, FourVector e_in,
	    FourVector e_out,FourVector q,double theta,double phi,
	    double pol,double hel);
  double My(FourVector photon,FourVector p_in, FourVector e_in,
	    FourVector e_out,FourVector q,double theta,double phi,
	    double pol,double hel);

  void Coefficients (FourVector photon,FourVector p_in, FourVector e_in,
		       FourVector e_out,FourVector q,double theta,double phi,
		       double pol);
  
  double CoeffFit1(FourVector photon,FourVector p_in, FourVector e_in,
		   FourVector e_out,FourVector q,double theta,double phi,
		   double pol);
  
  
  double CoeffFit2(FourVector photon,FourVector p_in, FourVector e_in,
		   FourVector e_out,FourVector q,double theta,double phi,
		   double pol);
  
};


#endif
