//                                                                    -*-c++-*-
// $ Id: $
#include "FourVector/FourVector.h"
#include "FourVector/Spinor.h"

#ifndef __QED_H__
#define  __QED_H__
//const double rad        = M_PI/180.0;
//const double m_electron = 0.000510999;
//const double m_e_sqr    = m_electron*m_electron;
//const double m_muon     = 0.105658389;
//const double m_proton   = 0.93827231;
const double m_p_sqr    = m_proton*m_proton;
//const double alpha      = 1.0/137.036;
//const double e          = sqrt(alpha*4*M_PI);
//const double mubarn     = 389.37966;         // mubarn*GeV^2
//                                             // elementary cross section

//const double cross_CMS = mubarn/64/pow(2*M_PI, 5)/m_proton;

const Complex i_e3 = i * e*e*e;
const Complex   e4 =     e*e*e*e;

const Tensor pID(  m_proton,  0,  0,  0,
		   0,  m_proton,  0,  0,  
		   0,  0,  m_proton,  0,  
		   0,  0,  0,  m_proton);

const Tensor eID(  m_electron,  0,  0,  0,
		   0,  m_electron,  0,  0,  
		   0,  0,  m_electron,  0,  
		   0,  0,  0,  m_electron);

double G_E_p(double Q2);
double G_M_p(double Q2);

Tensor gNN(const FourVector& q, int mu);

struct spin_components {
  double uu, ud, du, dd;
};

struct two_graphs {
  spin_components first;
  spin_components second;
  spin_components ssum;
  double first_sum;
  double second_sum;
  double sum;
};

struct four_graphs {
  spin_components first;
  spin_components second;
  spin_components third;
  spin_components fourth;
  spin_components ssum;
  double first_sum;
  double second_sum;
  double third_sum;
  double fourth_sum;
  double sum;
};

void BREMS_cs0(const FourVector &e_in, const FourVector &e_out,
	       const FourVector &p_in, const FourVector &p_out,
	       const double rcl, four_graphs& result, int lab=0);

void BREMS_2photon_cs0(const FourVector &e_in, const FourVector &e_out,
		       const FourVector &p_in, const FourVector &p_out,
		       const FourVector &q1_out,
		       const double rcl, four_graphs& result, int lab=0);

double the_trans(double th0, double th1, double phi);
double phi_trans(double th0, double th1, double th2);

double bethe_heitler_fit(double ga_th_cm, double ga_ph_cm,
			 double p, double p0, double th_e,
			 double* prm, int fit);
#endif








