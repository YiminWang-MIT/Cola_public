#include "Model/Model.h"

const double ChPT::delta_r1p = 6.60;    // proton radius
const double ChPT::lambda = m_proton;   // suspicious regularization parameter
const double ChPT::g_piN = 13.4;        // strong pion-nucleon coupling
const double ChPT::F_pi  = 0.093;       // pion decay constant
const double ChPT::kappa = 1.79284739;  // proton anomalous magnetic moment 
const double ChPT::g_A = g_piN*F_pi/m_proton;  // axial coulping

double ChPT::a1    =  7.85; // Low Energy Constants (LEC) in GeV^-4
double ChPT::a2    =  0.00; // GeV^-4 only a1+a2 is needed
double ChPT::a3    = -1.37; // GeV^-4
double ChPT::a4    = -0.22; // GeV^-4
double ChPT::a5    =  0.00;    // GeV^-5
double ChPT::bp    =  13.0; // GeV^-3

double_complex 
ChPT::P1()
{
  double_complex p = e * g_piN * q / (8 * M_PI * sqr(m_proton) * omega) *   
    ( (1 + kappa) * sqrt(sqr(omega) - k2) 
      + 1 / (10 * m_proton * omega * sqrt(sqr(omega) - k2)) * 
      ( 17 * sqr(omega) * k2 - 12 * pow(omega,4) +
        2 * sqr(omega*m_pi0) -	7 * k2 * sqr(m_pi0) + 
        5 * kappa * (2 * sqr(omega) * k2 - k2 * sqr(m_pi0) - pow(omega,4))));

  p += (e * pow(g_piN, 3) * wc * q) /
    (32 * M_PI*M_PI * pow(m_proton,3))  
    / sqrt(y * pow(y+rho,3)) *
    (rho/3 + 3*rho*rho/8 + y/3 + 3*rho*y/4 + y*y/2 - 
     sqrt_y_1(y) * (rho/3 + rho*rho/8 + y/3 + rho*y/6 + y*y/6) -
     (rho+2*y)*(4*rho+rho*rho+4*y)/16/sqrt(y+rho)*H(y,rho));

  return p * 1E3 * m_pion;
}

double_complex  
ChPT::P2()
{
  double_complex p = e * g_piN * q / (8 * M_PI * sqr(m_proton) * omega) *   
    ( -(1 + kappa) * sqrt(sqr(omega) - k2) 
      + 1 / (10 * m_proton * omega * sqrt(sqr(omega) - k2)) * 
      ( 13 * pow(omega,4) - 18 * sqr(omega)*k2 +
        2 * sqr(omega*m_pi0) - 3 * k2 * sqr(m_pi0) - 
        5 * kappa * (2 * sqr(omega) * k2 - k2 * sqr(m_pi0) - pow(omega,4))));
  
  p += (e * pow(g_piN, 3) * wc * q) /
    (32 * M_PI*M_PI * pow(m_proton,3))  
    / sqrt(y * pow(y+rho,3)) *
    (rho/3 - 3*rho*rho/8 + y/3 - rho*y - y*y/2 + 
     sqrt_y_1(y) * (-rho/3 + rho*rho/8 -y/3 +7*rho*y/12 + y*y/3) +
     rho*(4*rho+rho*rho+4*y)/16/sqrt(y+rho) * H(y, rho));

 return p * 1E3 * m_pion;
}

double_complex  
ChPT::P3()
{
  return e * q * (g_piN/(16*M_PI*pow(m_proton,3))+bp) * 
    sqrt(sqr(omega) - k2) * 1E3 * m_pion;
}

double_complex  
ChPT::P4()
{
  double_complex p =  e * g_piN * q/(40.0*M_PI*pow(m_proton,3)) * 
    (2.0+3.0*sqr(m_pi0/omega)) * sqrt(sqr(omega) - k2);

  p += (e * pow(g_piN, 3) * wc * q) /
    (32.0 * M_PI*M_PI * pow(m_proton,3))  
    / sqrt(y * pow(y+rho,3)) *
    (rho/3.0 + y/3.0 - rho*y/4.0 - y*y/2.0 + 
     sqrt_y_1(y)/3.0 
      * (-rho - y + rho*y/4.0 + y*y) -
     (rho+2.0*y)*rho*y/8.0/sqrt(y+rho)*H(y,rho));

  return p * 1E3 * m_pion;
}

double_complex  
ChPT::P5()
{
  double_complex p =  e * g_piN * q/(80*M_PI*pow(m_proton,3)) * 
    (3 + 2*sqr(m_pi0/omega)) * sqrt(sqr(omega) - k2);
  
  p += (e * pow(g_piN, 3) * wc * q) /
    (32.0 * M_PI*M_PI * pow(m_proton,3))  
    / sqrt(y * pow(y+rho,3)) *
    (rho/3.0 + y/3.0 - rho*y/8.0  + 
     sqrt_y_1(y)/3.0 
     * (-rho - y + 5.0*rho*y/8.0 + y*y/4.0) -
     y*(4.0*rho+rho*rho+4.0*y)/16.0/sqrt(y+rho)*H(y,rho));
  
  return p * 1E3 * m_pion;
}
















