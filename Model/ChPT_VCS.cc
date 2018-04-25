//----------------------------------------------------
// Virtual Compton Scattering
// 
// Kinematical Coefficients (G&vdH)
// and
// HBChPT Model (nucl-th/9910036, Hemmert et al.)
//
// L.Doria,05
//----------------------------------------------------

// Units of measure:
// The input FourVectors are in GeV
// The GPs are given in Fermi^3 or Fermi^4
// The electric and magnetic polarizabilities are given in Fermi^3
// In the FormFactors (G_E(q2),G_M(q2)), q2 is in GeV
// The Structure Functions (P_LL etc.) are given in GeV^(-2)

// Formulas with the comment "VdH" are from: 
// P.Guichon and M.Vanderhaeghen, Progr.Part.Phys, Vol.41 (1998)
// (hep-ph/9806305) 

// Formulas with the comment "Guichon" are from:
// P.Guichon et al.,Nucl.Phys. A 591 (1995) 606-638          

// "Exact" refers to the analytic results from nucl-th/9910036
// "Expansion" refers to the power expansions of the "Exact" formulas


#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "FourVector/FourVector.h"
#include "Cola/Masses.h"
#include "Model/ChPT_VCS.h"

using namespace std;

const double M = m_proton*1e3; //Proton Mass (MeV)
const double m = m_pi0*1e3;    //Pion0 Mass  (MeV)
const double F = 92.4;         //Pion Dacay Constant(may be slightly different)
const double g = 1.267;        //Axial-Vector Coupling Constant
const double hc = 197.326960;  //Conversion Fermi<->MeV 

const double p2f2 = M_PI*M_PI*F*F;
const double pf2 = M_PI*F*F;
const double M2 = M*M;
const double m2 = m*m;
const double m4 = pow(m,4);
const double r2 = M_SQRT2;
const double r3 = 1.732050808;
const double r6 = 2.449489743;
const double g2 = g*g;
const double hc4 = pow(hc,4); 
const double hc3 = pow(hc,3);
const double Alpha = 1/137.035989561;
const double ne = 4*M_PI*Alpha;
const double e6 = pow(4*M_PI*Alpha,3); //(electric charge in natural units)^6



//Accessory Functions
double ChPT_VCS::w0 (FourVector q) //"q zero tilde" MeV
  {
    double mod = pow(q.momentum(),2)*1e6;
    return M - sqrt(M*M+mod);
  }
   
//Related to loop integrals in ChPT
double ChPT_VCS::G (double x) 
{ 
  return asinh(x)/(x*sqrt(1+x*x));
}

//Form Factors
double ChPT_VCS::G_E(double q2)
{

  double Q2 = -q2;

  double F_1s =  ( 9.464/(0.611 + Q2) - 9.054/(1.039 + Q2) - 0.410/(2.560 + Q2))
    * pow(log((9.733 + Q2) / 0.350), -2.148);

  double F_2s = (- 1.549/(0.611 + Q2) + 1.985/(1.039 + Q2) - 0.436/(2.560 + Q2)) 
    * pow(log((9.733 + Q2)/0.350), -2.148);

  double F_1v = ( (1.032   * pow(log((9.733 - 0.500)/0.350), 2.148)
	    + 0.088 * pow(log((9.733 - 0.400)/0.350), 2.148)
	    * pow(1 + Q2/0.318, -2))/(2*(1 + Q2/0.550))
	   - 38.885/(2.103 + Q2) +  425.007/(2.734 + Q2) - 389.742/(2.835+Q2) 
	   ) * pow(log((9.733+Q2)/0.350),-2.148);

  double F_2v = ( (5.782   * pow(log((9.733 - 0.500)/0.350), 2.148)
	     + 0.391 * pow(log((9.733 - 0.400)/0.350), 2.148)
	     / (1 + Q2/0.142)) / ( 2 * (1+Q2/0.536))
	    - 73.535/(2.103+Q2) + 83.211/(2.734+Q2) - 29.467/(2.835+Q2)
	    ) * pow(log((9.733+Q2)/0.350),-2.148);


  double F1 =  F_1s + F_1v; 
  double F2 =  F_2s + F_2v;

  //cout << "G_E Margell = " << F1 + q2/4/pow(m_proton,2) * F2 << "   Dipole = " << 1 / pow(1 - q2/0.71,2) << endl;
  //return 1 / pow(1 - q2/0.71,2); //Dipole

  return  F1 + q2/4/pow(m_proton,2) * F2; //Margell et al.

}

double ChPT_VCS::G_M(double q2)
{

  double Q2 = -q2;
  
  double F_1s =  ( 9.464/(0.611 + Q2) - 9.054/(1.039 + Q2) - 0.410/(2.560 + Q2))
    * pow(log((9.733 + Q2) / 0.350), -2.148);
  
  double F_2s = (- 1.549/(0.611 + Q2) + 1.985/(1.039 + Q2) - 0.436/(2.560 + Q2)) 
    * pow(log((9.733 + Q2)/0.350), -2.148);

  double F_1v = ( (1.032   * pow(log((9.733 - 0.500)/0.350), 2.148)
	    + 0.088 * pow(log((9.733 - 0.400)/0.350), 2.148)
	    * pow(1 + Q2/0.318, -2))/(2*(1 + Q2/0.550))
	   - 38.885/(2.103 + Q2) +  425.007/(2.734 + Q2) - 389.742/(2.835+Q2) 
	   ) * pow(log((9.733+Q2)/0.350),-2.148);

  double F_2v = ( (5.782   * pow(log((9.733 - 0.500)/0.350), 2.148)
	     + 0.391 * pow(log((9.733 - 0.400)/0.350), 2.148)
	     / (1 + Q2/0.142)) / ( 2 * (1+Q2/0.536))
	    - 73.535/(2.103+Q2) + 83.211/(2.734+Q2) - 29.467/(2.835+Q2)
	    ) * pow(log((9.733+Q2)/0.350),-2.148);


  double F1 =  F_1s + F_1v; 
  double F2 =  F_2s + F_2v;

  //cout << "G_M Margell = " << F1 + F2 << "   Dipole = " << 2.79284739 / pow(1 - q2/0.71,2) << endl;
  //return 2.79284739 / pow(1 - q2/0.71,2); //Dipole

  return  F1 + F2; //Margell et al.
}

//Spin Dependent--------------------------------------------------------
double ChPT_VCS::P01121(FourVector q)
{
  double q2 = q.momentum()*q.momentum()*1e6;
  double mod = q.momentum()*1e3;
  //double x = (M/1e3 - sqrt(M*M/1e6 + q.momentum()*q.momentum() )) *2*M/1e3;

  return ( (-hc4*g2/24/r2/p2f2/q2)*(1-G(mod/2/m))  ) * scale[4];  //"Exact" O(p3)
  //return -(hc4*g*g*r2)/288/p2f2/m2*(1-q2/5/m2+3*q2*q2/70/m4); //Expansion

  //Exp. Constraint (interno)
  //return  - (23.7/1e6 + 2*sqrt(6)*M*G_E(x)*P01010(q)/hc3 - 3/0.64*G_M(x)*q2/w0(q)*P11111(q)/hc3 ) 
  //*0.64/3/r2/G_M(x)/q2*hc4;
}

double ChPT_VCS::P11021(FourVector q)
{ 
  double q2 = q.momentum()*q.momentum()*1e6;
  double mod = q.momentum()*1e3;
  return (  -(hc4*g2/12/r6/p2f2/q2)*(1-G(mod/2/m))  ) * scale[5];  //"Exact" O(p3)
  //return -hc4*r2*g*g/144/r3/p2f2/m2;
}

double ChPT_VCS::P11001(FourVector q)
{
  double q2 = q.momentum()*q.momentum()*1e6;
  double mod = q.momentum()*1e3;
  return (hc4*g2/12/r3/p2f2)*(2 - (2+3*q2/4/m2)*G(mod/2/m)  );//"Exact" O(p3)
}

double ChPT_VCS::P0111(FourVector q)
{
  double q2 = q.momentum()*q.momentum()*1e6;
  double mod = sqrt(q.momentum()*q.momentum())*1e3;
  return (hc4*g2/24/r6/p2f2/q2)*(3-(3+q2/m2)*G(mod/2/m));  //"Exact" O(p3)
  //return -hc4*g*g/48/r6/p2f2/m2;
}

//Electric&Magnetic Polarizabilities---------------------------------------

double ChPT_VCS::Alpha(FourVector q)
{
  //Expansion
  //return (hc3*5*0.091*g*g/384/p2f2/m) * (1-7*q2/50/m2+81*q2*q2/2800/m4);
  
  double q2 = q.momentum()*q.momentum()*1e6;
  double mod = q.momentum()*1e3;
  
  return (hc3*ne*g*g*m/64/p2f2)*
    (4+(2*q2/m2) - (8-(2*q2/m2)-(q2*q2/m4))*(m/mod)*atan(mod/m/2) ) /
    (q2*(4+q2/m2)); //"Exact" O(p3)
}

double ChPT_VCS::Beta(FourVector q)
{
  //Expansion
  //return (hc3*ne*g*g/768/p2f2/m) * (1+q2/5/m2-39*q2*q2/560/m4);
  
  double q2 = q.momentum()*q.momentum()*1e6;
  double mod = q.momentum()*1e3;
  
  return (hc3*ne*g*g*m/128/p2f2)*
    (-4-(2*q2/m2) + (8+(6*q2/m2)+(q2*q2/m4))*(m/mod)*atan(mod/m/2) ) /
    (q2*(4+q2/m2)); //"Exact" O(p3)
  
}

double ChPT_VCS::P01010(FourVector q)
{
  //Expansion
  //return (-4*M_PI*r2/r3)*(hc3*5*g*g/384/p2f2/m) * 
  //(1-7*q2/50/m2+81*q2*q2/2800/m4); 
  
  return -r2*4*M_PI*Alpha(q)/r3/ne * scale[0] ; //"Exact" O(p3)
}


double ChPT_VCS::P11110(FourVector q)
{  
  //Expansion
  //return (-4*M_PI*2.82842/r3)*(hc3*g*g/768/p2f2/m) * 
  //(1+q2/5/m2-39*q2*q2/560/m4);
  
  return -4*M_PI*Beta(q)/ne*2.828427125/r3 * scale[1]; //"Exact" O(p3)
}

//Auxiliary P(01,1)1
double ChPT_VCS::PA0111(FourVector q)
{
  double q2 = q.momentum()*q.momentum()*1e6;
  return -(hc4*11*g*g/576/pf2/M/m)*(1-6*q2/55/m2+123*q2*q2/560/m4);
}


//From Charge Conjugation: P(01,01)1 and P(11,11)1-------------------------

double ChPT_VCS::P01011(FourVector q)
{
  //With combinations
  //return (w0(q)/3/q2/hc)*(r2*P11001(q2)+(r3/r2)*q2*P11021(q2));
  //Expansion
  //return (hc3*g*g/144/p2f2/M)*(q2/m2-(3/20+m2/M/M/4)*q2*q2/m4);
  
  double q2 = q.momentum()*q.momentum()*1e6;
  double mod = q.momentum()*1e3;
  
  //"Exact" O(p3)
  return (  (hc3*g*g*w0(q)/24/p2f2/q2) * (1-(1+q2/2/m2)*G(mod/2/m)) ) *scale[3];

  //Exp. Constraint (interno)

  //double mq = q.momentum()*1e3;
  //double x = (M/1e3 - sqrt(M*M/1e6 + q.momentum()*q.momentum() )) *2*M/1e3;

  //return ( -5*hc3/1e6 - r3/r2*M*mq*G_E(x)*P11110(q)/sqrt(-w0(q)*2*M) ) 
  //*2*w0(q)/3/sqrt(-w0(q)*2*M)/mq/G_M(x);
}

  
double ChPT_VCS::P11111(FourVector q)
{    

  double q2 = q.momentum()*q.momentum()*1e6;
  double mod = q.momentum()*1e3;

  //Expansion
  //return -(g*g/288/p2f2/M)*(q2/m2 - (0.1 - m*m/M/M/4)*q2*q2/m2/m2    ) * scale[0];

  //"Exact" O(p3)
  return ( -(hc3*g*g*w0(q)*M2/24/p2f2/q2/q2)*
	   ( (2*w0(q)/M+3*q2/M2) - ( (3*m2+q2)*(q2/M2/m2) + (2+q2/m2)*w0(q)/M) 
	     * G(mod/2/m) )   ) * scale[2];
}


//Structure Functions

void ChPT_VCS::setScale(double scale[]) {
  for (int i=0;i<6;i++) ChPT_VCS::scale[i] = scale[i];
  //for (int i=0;i<4;i++) ChPT_VCS::scale[i] = scale[i];
}

void ChPT_VCS::setStruct(double Struct[]) {
  for (int i=0;i<2;i++) ChPT_VCS::Struct[i] = Struct[i];
}



double ChPT_VCS::P_LL(FourVector q)
{ 
  double x = (M/1e3 - sqrt(M*M/1e6 + q.momentum()*q.momentum() )) *2*M/1e3;

  return -2*M*r6*G_E(x)*P01010(q)/hc3*1e6; 
}


double ChPT_VCS::P_TT(FourVector q)
{
  double q2 = q.momentum()*q.momentum()*1e6;//-q.square()*1e6;
  double x = (M/1e3 - sqrt(M*M/1e6 + q.momentum()*q.momentum() )) *2*M/1e3;
  
  //VdH
  return -3. * G_M(x) * (q2/w0(q)) * 
    ( P11111(q)/hc3 - r2*w0(q)*P01121(q)/hc4 ) *1e6;
  
  //Guichon
  //return  (3/2.)*G_M(x) *
  //( 2*w0(q)*P01011(q2)/hc3 + r2*q2*(r3*P0111(q2)/hc4 + P01121(q2)/hc4) );
}  

double ChPT_VCS::P01010ScaleBoundaryCondition(FourVector q, double PLLminusPTToverEps, double eps)
{ 
  scale[0] = 1;
  return (PLLminusPTToverEps + P_TT(q)/eps)/P_LL(q);
}  

double ChPT_VCS::P_LT(FourVector q)
{
    
  double mq = q.momentum()*1e3;
  double x = (M/1e3 - sqrt(M*M/1e6 + q.momentum()*q.momentum() )) *2*M/1e3;
  
  //VdH
  return  (r3/r2) * M * (mq / sqrt(-w0(q)*2*M)) * G_E(x) 
    * P11110(q)/hc3 *1e6 + (3/2.)*mq*sqrt(-w0(q)*2*M) * 
    G_M(x)*P01011(q) /hc3/ w0(q) *1e6;
  
  //Guichon 
  //return (r3/r2) * M * sqrt(q2) * G_E(x) * P11110(q2) /hc3/ 
  //sqrt(-w0(q)*2*M)  + (r3/2.)/mq*sqrt(-w0(q)*2*M) *G_M(x)*
  //( P11001(q2)/hc4 + q2/r2*P11021(q2)/hc4 );
}

double ChPT_VCS::P11110ScaleBoundaryCondition(FourVector q, double PLT)
{
    
  double mq = q.momentum()*1e3;
  double x = (M/1e3 - sqrt(M*M/1e6 + q.momentum()*q.momentum() )) *2*M/1e3;
  scale[1] = 1;
  return (PLT - (3/2.)*mq*sqrt(-w0(q)*2*M) * G_M(x)*P01011(q) /hc3/ w0(q) *1e6)
    /((r3/r2) * M * (mq / sqrt(-w0(q)*2*M)) * G_E(x) * P11110(q)/hc3 *1e6);
 
}

double ChPT_VCS::P_LTz(FourVector q)
{
  
  double mq = q.momentum()*1e3;
  double x = (M/1e3 - sqrt(M*M/1e6 + q.momentum()*q.momentum() )) *2*M/1e3;
  
  return (3/2.)*mq*sqrt(-w0(q)*2*M) *G_M(x)*P01011(q) /hc3/ w0(q) *1e6
         -3*M*mq*G_E(x)*P11111(q)/sqrt(-w0(q)*2*M)/hc3 *1e6;
}
    
double ChPT_VCS::P_LTz1(FourVector q)
{
  
  double x = (M/1e3 - sqrt(M*M/1e6 + q.momentum()*q.momentum() )) *2*M/1e3;
  
  return -(3/2.)*sqrt(-w0(q)*2*M)*G_M(x)*P01011(q)/hc3 *1e6 +
    3*M*q.momentum()*q.momentum()*G_E(x)*P11111(q)
    /hc3/sqrt(-w0(q)*2*M)/w0(q) *1e6;  
}

double ChPT_VCS::P_LTo1(FourVector q)
{
  double mq = q.momentum()*1e3;
  double x = (M/1e3 - sqrt(M*M/1e6 + q.momentum()*q.momentum() )) *2*M/1e3;
  
  return 3*mq*sqrt(-w0(q)*2*M)*G_M(x)/2/w0(q) *
    ( P01011(q)/hc3 - (r3/r2)*w0(q)*P11021(q)/hc4  ) *1e6;
}

double ChPT_VCS::P_LTo(FourVector q)
{
  double mp = m_proton;
  double mq = q.momentum();
  double W0 = mp - sqrt(mp*mp-mq*mq);
  double R = 2*mp / sqrt(W0*2*mp);
  double x = (M/1e3 - sqrt(M*M/1e6 + q.momentum()*q.momentum() )) *2*M/1e3;
  
  return R*G_E(x)/2/G_M(x)*P_TT(q) - G_M(x)/2/R/G_E(x)*P_LL(q);
}

double ChPT_VCS::P_TTo(FourVector q)
  {
    double mq = q.momentum()*1e3;
    double x = (M/1e3 - sqrt(M*M/1e6 + q.momentum()*q.momentum() )) *2*M/1e3;
    
    return -mq/2*G_M(x)*(3*P11111(q)/hc3+(r3/r2)*P11110(q)
           /hc3) *1e6;
  }

double ChPT_VCS::P_TTo1(FourVector q)
{
  double mq = q.momentum()*1e3; 
  double x = (M/1e3 - sqrt(M*M/1e6 + q.momentum()*q.momentum() )) *2*M/1e3;
  
  return mq/2*G_M(x)*( 3*mq/w0(q)*P11111(q)/hc3+ r3/r2*P11110(q)
         /hc3*w0(q)/mq ) *1e6;
}

  

//CrossSections

double ChPT_VCS::K2(FourVector q, double pol)
{
  double mp = m_proton;
  double mq = q.momentum();
  double Eq = sqrt(mp*mp+mq*mq);
  double W0 = mp - sqrt(mp*mp+mq*mq);
  double Q  = -W0*2*mp;
  
  return e6 * 2*mq*mp/Q/(1-pol)*sqrt(2*Eq/(Eq+mp));
}

double ChPT_VCS::kT (double theta, double phi, double pol, FourVector P_in)
{

  double q = P_in.momentum();
  
  double st = sin(theta);
  double ct = cos(theta);
  double mel = 0.001;
  FourVector p_in(energy(m_proton, q), 0, 0, -q);
  FourVector p_out_el(energy(m_proton, mel), 0, -st*mel, -ct*mel);
  FourVector q_out_el(mel,                   0,  st*mel,  ct*mel);
  FourVector q_in_el = p_out_el + q_out_el - p_in;
  double qi=q_in_el.momentum();
  double kiko = sqrt((1+pol)/(1-pol))*qi;
  double ki = (kiko+q_in_el.energy())/2;
  double ko =  kiko-ki;
  double tk=acos((qi*qi+ki*ki-ko*ko)/(2*qi*ki));
  FourVector e_in_el(energy(m_electron, ki), ki*sin(phi), ki*sin(tk)*cos(phi), ki*cos(tk)*cos(phi));
  //FourVector e_out_el = e_in_el - q_in_el;
  double Q2_el = -q_in_el.square();
  double Q_el  = sqrt(Q2_el);

  return Q_el*sqrt(pol/2/(1-pol));
}
  
  
double ChPT_VCS::w(double theta, double phi, double pol, FourVector P_in)
{

  double q = P_in.momentum();

  double st=sin(theta);
  double ct=cos(theta);
  double mel=0.001;

  FourVector p_in(energy(m_proton, q), 0, 0, -q);
  FourVector p_out_el(energy(m_proton, mel), 0, -st*mel, -ct*mel);
  FourVector q_out_el(mel,                   0,  st*mel,  ct*mel);
  FourVector q_in_el = p_out_el + q_out_el - p_in;

  double qi = q_in_el.momentum();
  double kiko = sqrt((1+pol)/(1-pol))*qi;
  double ki = (kiko+q_in_el.energy())/2;
  double ko =  kiko-ki;
  double tk = acos((qi*qi+ki*ki-ko*ko)/(2*qi*ki));

  FourVector e_in_el(energy(m_electron, ki), ki*sin(phi), ki*sin(tk)*cos(phi), ki*cos(tk)*cos(phi));
  //  FourVector e_out_el = e_in_el - q_in_el;

  return - mel * ( 1/( p_in*q_out_el) + 1/(e_in_el*q_out_el) );

}


// OOP TEST
double ChPT_VCS::wphi(double theta, double phi,double pol, FourVector P_in)
{
  double q = P_in.momentum();
  
  double st=sin(theta);

  double ct=cos(theta);
  double mel=0.001;
  
  FourVector p_in(energy(m_proton, q), 0, 0, -q);
  FourVector p_out_el(energy(m_proton, mel), 0, -st*mel, -ct*mel);
  FourVector q_out_el(mel,                   0,  st*mel,  ct*mel);
  FourVector q_in_el = p_out_el + q_out_el - p_in;
  
  double qi=q_in_el.momentum();
  double kiko = sqrt((1+pol)/(1-pol))*qi;
  double ki = (kiko+q_in_el.energy())/2;
  double ko =  kiko-ki;
  double tk=acos((qi*qi+ki*ki-ko*ko)/(2*qi*ki));

  FourVector e_in_el(energy(m_electron, ki), ki*sin(phi), ki*sin(tk)*cos(phi), ki*cos(tk)*cos(phi));
  FourVector e_out_el = e_in_el - q_in_el;


  cout << theta*180/M_PI << " " << phi*180/M_PI << " " 
       << -mel * ( 1/( p_in*q_out_el) + 1/(e_in_el*q_out_el) ) << " "
       << mel * ( 1/(e_out_el*q_out_el) - 1/(e_in_el*q_out_el) ) << endl;

  return - mel * ( 1/( p_in*q_out_el) + 1/(e_in_el*q_out_el) );
}


double ChPT_VCS::w1(double theta, double phi, double pol, FourVector P_in)
{
  double q = P_in.momentum();

  double st=sin(theta);
  double ct=cos(theta);
  double mel=0.001;

  FourVector p_in(energy(m_proton, q), 0, 0, -q);
  FourVector p_out_el(energy(m_proton, mel), 0, -st*mel, -ct*mel);
  FourVector q_out_el(mel,                   0,  st*mel,  ct*mel);
  FourVector q_in_el = p_out_el + q_out_el - p_in;

  double qi=q_in_el.momentum();
  double kiko = sqrt((1+pol)/(1-pol))*qi;
  double ki = (kiko+q_in_el.energy())/2;
  double ko =  kiko-ki;
  double tk=acos((qi*qi+ki*ki-ko*ko)/(2*qi*ki));

  FourVector e_in_el(energy(m_electron, ki), ki*sin(phi), ki*sin(tk)*cos(phi), ki*cos(tk)*cos(phi));
  FourVector e_out_el = e_in_el - q_in_el;

  return mel * ( 1/(e_out_el*q_out_el) - 1/(e_in_el*q_out_el) );

}

double ChPT_VCS::w2(double theta, double phi, double pol, FourVector P_in)
{

  double q = P_in.momentum();

  double st=sin(theta);
  double ct=cos(theta);
  double mel=0.001;

  FourVector p_in(energy(m_proton, q), 0, 0, -q);
  FourVector p_out_el(energy(m_proton, mel), 0, -st*mel, -ct*mel);
  FourVector q_out_el(mel,                   0,  st*mel,  ct*mel);
  FourVector q_in_el = p_out_el + q_out_el - p_in;

  double qi=q_in_el.momentum();
  double kiko = sqrt((1+pol)/(1-pol))*qi;
  double ki = (kiko+q_in_el.energy())/2;
  double ko =  kiko-ki;
  double tk=acos((qi*qi+ki*ki-ko*ko)/(2*qi*ki));

  FourVector e_in_el(energy(m_electron, ki), ki*sin(phi), ki*sin(tk)*cos(phi), ki*cos(tk)*cos(phi));
  //  FourVector e_out_el = e_in_el - q_in_el;

  double Q2_el = -q_in_el.square();
  double Q_el  = sqrt(Q2_el);
  double kT = Q_el*sqrt(pol/2/(1-pol));

  //return w(theta,phi,pol,P_in)*q - w1(theta,phi,pol,P_in)*sqrt(ko*ko-kT*kT); //ERROR??
  
  return w(theta,phi,pol,P_in)*q_in_el.momentum() - w1(theta,phi,pol,P_in)*sqrt(ko*ko-kT*kT);

}

double ChPT_VCS::M0(FourVector photon,FourVector p_in, FourVector e_in,
		    FourVector e_out,FourVector q,double theta,double phi,
		    double pol)
{ 

  double mq = q.momentum();
  double mp = m_proton;
  double W0 = mp - sqrt(mp*mp+mq*mq);
  
  double W1 = w1(theta,phi,pol,p_in);
  double W2 = w2(theta,phi,pol,p_in);

  double v1 = sin(theta)*(W2*sin(theta) - 
			  kT(theta,phi,pol,p_in)*W1*cos(theta)*cos(phi) );
  double v2 = -( W2*sin(theta)*cos(phi) - 
		 kT(theta,phi,pol,p_in)*W1*cos(theta));
  double v3 = -( W2*sin(theta) *
		 cos(theta)*cos(phi)
		 - kT(theta,phi,pol,p_in)* W1*(1-sin(theta) *
		 sin(theta)*cos(phi)*cos(phi)));

  return 2*K2(q,pol)*
    ( v1*(pol*P_LL(q) - P_TT(q) ) +
      ( v2 - W0*v3/mq ) * sqrt(2*pol*(1+pol))*P_LT(q) );

  //Likelihood fit
  //return 2*K2(q,pol)*( v1*Struct[0] + ( v2 - W0*v3/mq ) * sqrt(2*pol*(1+pol))*Struct[1] );
  
}
  
  
double ChPT_VCS::Mz(FourVector photon,FourVector p_in, FourVector e_in,
		    FourVector e_out,FourVector q,double theta,double phi,
			   double pol,double hel)
{

  double W1 = w1(theta,phi,pol,p_in);
  double W2 = w2(theta,phi,pol,p_in);
  
  double v1 = sin(theta)*(W2*sin(theta)-kT(theta,phi,pol,p_in)*W1*cos(theta)*cos(phi) );
  double v2 = -( W2*sin(theta)*cos(phi) - kT(theta,phi,pol,p_in)*W1*cos(theta));
  double v3 = -( W2*sin(theta)*cos(theta)*cos(phi)- kT(theta,phi,pol,p_in)* 
		 W1*(1-sin(theta)*sin(theta)*cos(phi)*cos(phi)));

  return 4*hel*K2(q,pol)*
    ( -v1*sqrt(1-pol*pol)*P_TT(q) +
      v2*sqrt(2*pol*(1-pol))*P_LTz(q) +
      v3*sqrt(2*pol*(1-pol))*P_LTz1(q) );
}

  
double ChPT_VCS::Mx(FourVector photon,FourVector p_in, FourVector e_in,
		    FourVector e_out,FourVector q,double theta,double phi,
		    double pol,double hel)
{
  
  double W1 = w1(theta,phi,pol,p_in);
  double W2 = w2(theta,phi,pol,p_in);

  double vx1 = sin(theta)*cos(phi)*(W2*
	       sin(theta)-kT(theta,phi,pol,p_in)*W1*cos(theta)*
	       cos(phi));
  double vx2 = -W2*sin(theta) + kT(theta,phi,pol,p_in)*
                W1*cos(theta)*cos(phi);
  double vx3 = -cos(theta)*(W2*
	       sin(theta)-kT(theta,phi,pol,p_in)*W1*cos(theta)*
	       cos(phi));
  double vx4 = kT(theta,phi,pol,p_in)*W1*sin(theta)*sin(phi)*sin(phi);
  
  
  return 4*hel*K2(q,pol)*
    (vx1*sqrt(2*pol*(1-pol))*P_LTo(q) +
     vx2*sqrt(1-pol*pol)*P_TTo(q) +
     vx3*sqrt(1-pol*pol)*P_TTo1(q) +
     vx4*sqrt(2*pol*(1-pol))*P_LTo1(q) );
}

double ChPT_VCS::My(FourVector photon,FourVector p_in, FourVector e_in,
		    FourVector e_out,FourVector q,double theta,double phi,
		    double pol,double hel)
{
  
  double W1 = w1(theta,phi,pol,p_in);
  double W2 = w2(theta,phi,pol,p_in);

  double vy1 = sin(theta)*sin(phi)*(W2*
	       sin(phi) - kT(theta,phi,pol,p_in)*W1*cos(theta)*
	       cos(phi));
  double vy2 = kT(theta,phi,pol,p_in)*W1*cos(theta)*sin(phi);
  double vy3 = kT(theta,phi,pol,p_in)*W1*sin(phi);
  double vy4 = -kT(theta,phi,pol,p_in)*W1*sin(theta)*sin(phi)*cos(phi);
  
  
  return 4*hel*K2(q,pol)*
    (vy1*sqrt(2*pol*(1-pol))*P_LTo(q) +
     vy2*sqrt(1-pol*pol)*P_TTo(q) +
     vy3*sqrt(1-pol*pol)*P_TTo1(q) +
     vy4*sqrt(2*pol*(1-pol))*P_LTo1(q) );
}


void ChPT_VCS::Coefficients (FourVector photon,FourVector p_in,
                               FourVector e_in,FourVector e_out,FourVector q,
                               double theta,double phi,double pol)
{

  double W0 = w(theta,phi,pol,p_in);
  double W1 = w1(theta,phi,pol,p_in);  
  double W2 = w2(theta,phi,pol,p_in);

  
  //X
  double vx1 = sin(theta)*cos(phi)*(w2(theta,phi,pol,p_in)*
	       sin(theta)-kT(theta,phi,pol,p_in)*w1(theta,phi,pol,p_in)*cos(theta)*
	       cos(phi));
  double vx2 = -w2(theta,phi,pol,p_in)*sin(theta) + kT(theta,phi,pol,p_in)*
                w1(theta,phi,pol,p_in)*cos(theta)*cos(phi);
  double vx3 = -cos(theta)*(w2(theta,phi,pol,p_in)*
		sin(theta)-kT(theta,phi,pol,p_in)*w1(theta,phi,pol,p_in)*cos(theta)*
		cos(phi));
  double vx4 = kT(theta,phi,pol,p_in)*w1(theta,phi,pol,p_in)*sin(theta)*sin(phi)*sin(phi);


  //Y
  double vy1 = sin(theta)*sin(phi)*(w2(theta,phi,pol,p_in)*
	       sin(phi) - kT(theta,phi,pol,p_in)*w1(theta,phi,pol,p_in)*cos(theta)*
	       cos(phi));
  double vy2 = kT(theta,phi,pol,p_in)*w1(theta,phi,pol,p_in)*cos(theta)*sin(phi);
  double vy3 = kT(theta,phi,pol,p_in)*w1(theta,phi,pol,p_in)*sin(phi);
  double vy4 = -kT(theta,phi,pol,p_in)*w1(theta,phi,pol,p_in)*sin(theta)*sin(phi)*cos(phi);

  //Z
  double v1 = sin(theta)*(W2*sin(theta)-kT(theta,phi,pol,p_in)*W1*cos(theta)*cos(phi) );
  double v2 = -( W2*sin(theta)*cos(phi) - kT(theta,phi,pol,p_in)*W1*cos(theta));
  double v3 = -( W2*sin(theta)*cos(theta)*cos(phi)- kT(theta,phi,pol,p_in)* 
		 W1*(1-sin(theta)*sin(theta)*cos(phi)*cos(phi)));

  cout << theta*180/M_PI << " " 
       << vx1 << " "<< vx2 << " "<< vx3 << " "<< vx4 << " " 
       << vy1 << " "<< vy2 << " "<< vy3 << " "<< vy4 << " " 
       << v1 << " "<< v2 << " " << v3 << " " 
       << kT(theta,phi,pol,p_in) << " "
       << W0 << " "
       << W1 << " "
       << W2
       << endl;
 
}


double ChPT_VCS::CoeffFit1(FourVector photon,FourVector p_in, FourVector e_in,
			  FourVector e_out,FourVector q,double theta,double phi,
			  double pol)
{ 

  double mq = q.momentum();
  double mp = m_proton;
  double W0 = mp - sqrt(mp*mp+mq*mq);
  
  /*double v1 = sin(theta)*(w2(photon,p_in,e_in,e_out,q,pol)*sin(theta) - 
			  kT(theta,phi,pol,p_in)*w1(photon,e_in,e_out)*cos(theta) * 
			  cos(phi) );*/
  double v2 = -( w2(theta,phi,pol,p_in)*sin(theta)*cos(phi) - 
		 kT(theta,phi,pol,p_in)*w1(theta,phi,pol,p_in)*cos(theta));
  double v3 = -( w2(theta,phi,pol,p_in)*sin(theta) *
		 cos(theta)*cos(phi)
		 - kT(theta,phi,pol,p_in)* w1(theta,phi,pol,p_in)*(1-sin(theta) *
		 sin(theta)*cos(phi)*cos(phi)));
  
  return 2*K2(q,pol) * ( v2 - W0*v3/mq ) * sqrt(2*pol*(1+pol)) ;

  
}
     

double ChPT_VCS::CoeffFit2(FourVector photon,FourVector p_in, FourVector e_in,
			  FourVector e_out,FourVector q,double theta,double phi,
			  double pol)
{ 

  phi = phi*M_PI/180;

  double mq = q.momentum();
  double mp = m_proton;
  double W0 = mp - sqrt(mp*mp+mq*mq);

  double v1 = sin(theta)*(w2(theta,phi,pol,p_in)*sin(theta) - 
			  kT(theta,phi,pol,p_in)*w1(theta,phi,pol,p_in)*cos(theta) * 
			  cos(phi) );

  double v2 = -( w2(theta,phi,pol,p_in)*sin(theta)*cos(phi) - 
		 kT(theta,phi,pol,p_in)*w1(theta,phi,pol,p_in)*cos(theta));

  double v3 = -( w2(theta,phi,pol,p_in)*sin(theta) *
		 cos(theta)*cos(phi)
		 - kT(theta,phi,pol,p_in)* w1(theta,phi,pol,p_in)*(1-sin(theta) *
							   sin(theta)*cos(phi)*cos(phi)));

  // vLL
  double vLL = 2*K2(q,pol) * v1 * pol ;

  //vLT
  double vLT = 2*K2(q,pol) * ( v2 - W0*v3/mq ) * sqrt(2*pol*(1+pol));

  

  //cout << theta*180/M_PI <<" "<<phi*180/M_PI<<" "<<2*K2(q,pol) * v1 * pol<<" "<<
  //2*K2(q,pol) * ( v2 - W0*v3/mq ) * sqrt(2*pol*(1+pol)) << " " << 
  //vLL/vLT << endl;

  return vLL/vLT;

} 



double ChPT_VCS::scale[6]={1,1,1,1,1,1};
//double ChPT_VCS::scale[4]={1,1,1,1};
double ChPT_VCS::Struct[2]={20,-5};
 
