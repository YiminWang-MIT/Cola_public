#include "Model/Model.h"
#include <iostream>
#include "IntL.h"

void
ChPT::setSwaves(double E0, double L0, double q2)
{ setKinematics(0, q2);
  E0_set = E0/1E3/m_pion 
    + (e*g_piN*sqr(wc))/(32*sqr(M_PI)*m_proton *sqr(F_pi))*
    (1 - 5*wc/2/m_proton) * real(sqrt_y_1(y));

  L0_set = L0/1E3/m_pion 
    + (e*g_piN*sqr(wc))/(32*sqr(M_PI)*m_proton *sqr(F_pi))*
    1.0/(2.0+rho)*(1 - wc/2/m_proton*(10+6*rho+rho*rho)/(2+rho))*
    real(sqrt_y_1(y)); 
}

double_complex   
ChPT::E0p()
{
  if (E0_set!=0 || L0_set!=0)
    return (E0_set - (e*g_piN*sqr(wc))/(32*sqr(M_PI)*m_proton *sqr(F_pi))*
	    (1 - 5*wc/2/m_proton) * sqrt_y_1(y)) * 1E3 * m_pion;
  return (E0p(k2) - (e*g_piN*sqr(wc))/(32*sqr(M_PI)*m_proton *sqr(F_pi))*
	  (1 - 5*wc/2/m_proton) * sqrt_y_1(y)) * 1E3 * m_pion;//ok!
}

double_complex   
ChPT::L0p()
{
  if (E0_set!=0 || L0_set!=0)
    return (L0_set - (e*g_piN*sqr(wc))/(32*sqr(M_PI)*m_proton *sqr(F_pi))*
      1.0/(2.0+rho)*(1-wc/2/m_proton*(10+6*rho+rho*rho)/(2+rho))*sqrt_y_1(y)) 
      * 1E3 * m_pion;

  return (L0p(k2) - (e*g_piN*sqr(wc))/(32*sqr(M_PI)*m_proton *sqr(F_pi))*
    1.0/(2.0+rho)*(1 - wc/2/m_proton*(10+6*rho+rho*rho)/(2+rho))*sqrt_y_1(y)) 
    * 1E3 * m_pion; // this subroutine is heavily checked and is ok!
}

double intpol(double rho, int i) {
  int d = rho/0.01;
  return CHPTint[d+1][i] + (CHPTint[d+1][i]-CHPTint[d][i])
    /(CHPTint[d+1][0]-CHPTint[d][0])*(rho-CHPTint[d+1][0]);
}

double_complex   
ChPT::E0p(double k2)
{
  // Born term
  double_complex E = 
    e*g_piN/(8*M_PI*sqr(m_proton))*
    (- m_pi0 
     + 1/(2*m_proton) * ((3+kappa)*sqr(m_pi0)-(1+kappa)*k2)
     + m_pi0/(8*sqr(m_proton))*((5+4*kappa)*k2
				-3*(5+2*kappa)*sqr(m_pi0)));
  
  // q^3 loop
  E +=  e*g_piN*sqr(wc)/(128*sqr(M_PI)*m_proton *sqr(F_pi)) *
    (rho/(1+rho)
     +sqr(2+rho)/2/pow(1+rho,1.5)*acos(-rho/(2+rho)));

  // g_a^3 - diagrams
  // double integral = 0;
  // const int n=400;
  //for (double x=0.5/n; x<1; x+=1.0/n) 
  //  integral += sqrt(1-x*x+rho*x*(1-x))*acos(x/sqrt(1+rho*x*(1-x)))/n;

  E += e * pow(g_A * wc/(M_PI*F_pi),3)/(128 * m_proton) *
    ((8.0/3.0+7*rho/6)*log(wc/lambda)-26.0/9-59*rho/36+M_PI*(5.0/3+rho)
     +(8.0/3+7*rho/6)*sqrt(1+4/rho)*log((sqrt(4+rho)+sqrt(rho))/2)
     -2*(2+rho)*intpol(rho,1));

  // g_a - diagrams
  //  integral=0;
  // for (double x=0.5/n; x<1; x+=1.0/n) 
  // integral += (4*(1+rho)*x*x-(4+6*rho)*x-4)/
  //   sqrt(1-x*x+rho*x*(1-x))*asin(x/sqrt(1+rho*x*(1-x)))/n;

  E +=e * g_A * pow(wc/(M_PI*F_pi),3)/(128 * m_proton) *
    ((10 + 11*rho/6) * log(wc/lambda)
     -7.0/3
     -31*rho/36
     +(4.0/3+11*rho/6)*sqrt(1+4/rho)*log((sqrt(4+rho)+sqrt(rho))/2)
     -M_PI*rho/(1+rho)-M_PI*sqr(2+rho)/2/pow(1+rho,1.5)*acos(-rho/(2+rho))
     +intpol(rho,3));
  
  //counter terms
  E += e*m_pi0*((a1+a2)*sqr(m_pi0)-a3*k2);

  //Form Factor
  E += -e*g_piN*m_pi0*k2/(48*M_PI*sqr(m_proton))*delta_r1p;

  return E;
}

double_complex   
ChPT::L0p(double k2)
{
  if (rho>11) cerr<<"ChPT Model out of range";

  // Born terms /////////////////////////////////////////////////*/////////////

  double_complex L =
    e * g_piN / (8.0 * M_PI * sqr(m_proton)) *
    (- m_pi0 + (3 * sqr(m_pi0) - k2) / (2.0 * m_proton)
     + m_pi0 / (8 * sqr(m_proton)) * ((5 - 2 * kappa) * k2 - 15 * sqr(m_pi0)));

  // q^3 loop ///////////////////////////////////////////////////*//////////// 

  L += e * g_piN * sqr(wc) / (128 * sqr(M_PI) * m_proton * sqr(F_pi)) *
    (2.0 / (1.0 + rho) + rho/pow(1 + rho, 1.5) * acos(-rho / (2 + rho)));

  // g_a^3 - diagrams ////////////////////////////////////////////////////////

  //double integral = CHPTint[(int)(rho/0.01)][2];

  L += e * pow((g_A * wc)/(M_PI * F_pi), 3)/(128 * m_proton) *
    ((2.0/3-5*rho/6)*log(wc/lambda)-8.0/9+13*rho/36+M_PI*(1.0/6-rho/2)
     +(5.0/3+rho/6)*sqrt(1+4/rho)*log((sqrt(4+rho)+sqrt(rho))/2)
     +intpol(rho,2));

  // g_a - diagrams  /////////////////////////////////////////////////////////

  L +=  e * g_A * pow(wc/(M_PI*F_pi),3)/(128 * m_proton) * (1+rho) * 
     (-2 * log(wc/lambda) - 3 + intpol(rho,4));

  //  cout << " ga 1: "<< (L-Lb).real();Lb=L;

  L +=  e * g_A * pow(wc/(M_PI*F_pi),3)/(128 * m_proton) *
    ((10 + 11*rho/6) * log(wc/lambda)
     -7.0/3 - 31*rho/36
     +(4.0/3+11*rho/6)*sqrt(1+4/rho)*log((sqrt(4+rho)+sqrt(rho))/2)
    -M_PI*rho/(1+rho)-M_PI*sqr(2+rho)/2/pow(1+rho,1.5)*acos(-rho/(2+rho))
     + intpol(rho,3));
     
  //counter terms
  
  L += e * m_pi0 * ((a1+a2) * sqr(m_pi0) - a3 * k2 + a4 * (sqr(m_pi0)-k2))
    - e * sqr(m_pi0) * k2 * a5;
  
  //Form Factor
  L -=  e * g_piN * m_pi0 * k2 / (48 * M_PI * sqr(m_proton)) * delta_r1p;
  return L;///1.05;
}





