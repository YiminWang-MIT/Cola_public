#include "Model/Model.h"
#include "config.h"

const double model::wc    = 0.14011;     // pi+ threshold

extern "C" int MAIN__() { return 0;}; // bug in libf2c

void model::setKinematics(double deltaW, double q2)
{
  k2    = q2;
  W     = m_proton + m_pi0 + deltaW;
  omega = (W*W - sqr(m_proton) + sqr(m_pi0))/(2*W);
  q     = sqrt(sqr(omega)-sqr(m_pi0));
  rho   = - k2 / sqr(wc);
  y     = sqr(omega/wc);
  k0   = (W * W - sqr(m_proton) + k2) / 2 / W;
}

double model::sigma_0(double dW, double q2, double eps, double theta) 
{
  setKinematics(dW,q2);
  epsL = -k2 / sqr(k0) * eps;    
  return  2 * W * q / (sqr(W) - sqr(m_proton)) * 
    ( R_T(dW, q2, theta) +  epsL * R_L(dW, q2, theta))
    * mubarn / sqr(1e3 * m_pion);
}

double model::sigma_T(double dW, double q2, double eps, double theta) 
{
  setKinematics(dW,q2);
  epsL = -k2 / sqr(k0) * eps;    
  return  2 * W * q / (sqr(W) - sqr(m_proton)) * 
    ( R_T(dW, q2, theta) )
    * mubarn / sqr(1e3 * m_pion);
}

double model::sigma_L(double dW, double q2, double eps, double theta) 
{
  setKinematics(dW,q2);
  epsL = -k2 / sqr(k0) * eps;    
  return  2 * W * q / (sqr(W) - sqr(m_proton)) * 
    ( R_L(dW, q2, theta))
    * mubarn / sqr(1e3 * m_pion);
}

double model::sigma_TL(double dW, double q2, double eps, double theta) 
{
  setKinematics(dW,q2);
  return  2 * W * q / (sqr(W) - sqr(m_proton)) *   R_TL(dW, q2, theta)
    * mubarn / sqr(1e3 * m_pion);
}

double model::sigma_TLp(double dW, double q2, double eps, double theta) 
{
  setKinematics(dW,q2);
  return  2 * W * q / (sqr(W) - sqr(m_proton)) *   R_TLp(dW, q2, theta)
    * mubarn / sqr(1e3 * m_pion);
}

double model::sigma_TT(double dW, double q2, double eps, double theta) 
{
  setKinematics(dW,q2);
  return  2 * W * q / (sqr(W) - sqr(m_proton)) * 
    R_TT(dW, q2, theta) * mubarn / sqr(1e3 * m_pion);
}

double model::R_T(double dW, double q2, double theta) 
{
  setKinematics(dW,q2);
  return norm(E0p() + cos(theta)*P1())
    + pow(sin(theta),2)/2 * (norm(P2()) + norm(P3()));
}

double model::R_L(double dW, double q2, double theta) 
{
  setKinematics(dW,q2);
  return norm(L0p() + cos(theta)*P4()) + pow(sin(theta),2) * norm(P5()); 
}

double model::R_TT(double dW, double q2, double theta) 
{
  setKinematics(dW,q2);
  return pow(sin(theta),2)/2 * (norm(P2()) - norm(P3()));
}

double model::R_TL(double dW, double q2, double theta) 
{
  setKinematics(dW,q2);
  return - sin(theta) * real((E0p() + cos(theta)*P1()) * conj(P5()) +
			     (L0p() + cos(theta)*P4()) * conj(P2()));
}

double model::R_TLp(double dW, double q2, double theta) 
{
  setKinematics(dW,q2);
  return - sin(theta) * imag((E0p() + cos(theta)*P1()) * conj(P5()) +
			     (L0p() + cos(theta)*P4()) * conj(P2()));
}

double model::CrossSection(double dW, double q2, double eps, 
			   double theta, double phi, double h) 
{
  setKinematics(dW,q2);
  epsL = -k2 / sqr(k0) * eps;    
  return                       sigma_0(dW,q2,eps,theta) + 
    sqrt(2*epsL*(1+eps))     * sigma_TL(dW,q2,eps,theta)*cos(phi) +
    h * sqrt(2*epsL*(1-eps)) * sigma_TLp(dW,q2,eps,theta)*sin(phi) +
    eps                      * sigma_TT(dW,q2,eps,theta)*cos(2*phi);
}

etaMaid::etaMaid(char *filename) {
  char command[255];
  sprintf(command, GZIP " -c -d %-200s",filename);
  FILE * gzippipe = popen(command,"r");
  stdio_filebuf<char> fb(gzippipe,ios::in); 
  istream in(&fb);    
  
  in >> Q2 >> amin >> amax >> dy >> min >> max >> dx;
  m = (int) ((amax - amin)/dy) + 1;
  n = (int) ((max - min)/dx) + 1;
  data = new maiddata[m*n];
  
  for(int j=0;j<m;j++) {
    in >> Q2 >> angle >> min >> max >> dx;
    for (int i=j*n;i<j*n+n;i++)
      in >> data[i].W 
	 >> data[i].Hre[0] >> data[i].Him[0]
	 >> data[i].Hre[1] >> data[i].Him[1] 
	 >> data[i].Hre[2] >> data[i].Him[2]
	 >> data[i].Hre[3] >> data[i].Him[3]
	 >> data[i].Hre[4] >> data[i].Him[4]
	 >> data[i].Hre[5] >> data[i].Him[5];
  }
  pclose(gzippipe);
  m_initial=m_proton;m_final=m_proton;m_meson=m_eta;
}

kmaid::kmaid(int Aichan) {
  f_ichan = Aichan;
  name    = "kMaid";
}

double 
kmaid::CrossSection(double W, double Q2, double eps, 
		    double theta, double phi, double h) 
{
  double ST, SL, STL, STT, STLP;
  double MeVQ2= Q2*1e6;
  double MeVW = W*1e3;

  //  cout << "kMaid input: " << MeVW << " " <<MeVQ2 << " " << eps 
  //       << " " << theta << " " << phi << " " << h << endl;
  
  kmaid_(&f_ichan, &MeVQ2, &MeVW, &theta,
	  &ST, &SL, &STL, &STT, &STLP);
  double sum = ST + eps*SL+sqrt(2*eps*(1+eps))*STL*cos(phi)
    +eps*STT*cos(2*phi)+h*sqrt(2*eps*(1-eps))*STLP*sin(phi);
  /*  cout << "kMaid return (ST SL STL STT STLP 2*eps*(1-eps) sum):  " 
       << ST << " " << SL << " " 
       << STL << " " << STT << " " 
       << STLP << " " << 2*eps*(1-eps)  << " " 
       << sum << endl;  */
  return sum;
}

double 
kmaid::sigma_0(double W, double q2, double eps, double theta)
{
  double ST, SL, STL, STT, STLP;
  double MeVQ2=q2*1e6;
  double MeVW=  W*1e3;
  double phi = 0;
  double h   = 0;
  
  kmaid_(&f_ichan, &MeVQ2, &MeVW, &theta,
	  &ST, &SL, &STL, &STT, &STLP);
  double sum = ST + eps*SL+sqrt(2*eps*(1+eps))*STL*cos(phi)
    +eps*STT*cos(2*phi)+h*sqrt(2*eps*(1-eps))*STLP*sin(phi);
  return sum;
}

double 
kmaid::sigma_T(double W, double q2, double eps, double theta)
{
  double ST, SL, STL, STT, STLP;
  double MeVQ2=q2*1e6;
  double MeVW = W*1e3;
  kmaid_(&f_ichan, &MeVQ2, &MeVW, &theta,
	  &ST, &SL, &STL, &STT, &STLP);
  return ST;
}

double kmaid::sigma_TT(double W, double q2, double eps, double theta) 
{
  double ST, SL, STL, STT, STLP;
  double MeVQ2=q2*1e6;
  double MeVW = W*1e3;

  kmaid_(&f_ichan, &MeVQ2, &MeVW, &theta,
	  &ST, &SL, &STL, &STT, &STLP);
  return STT;
}

double 
kmaid::sigma_L(double W, double q2, double eps, double theta)
{
  double ST, SL, STL, STT, STLP;
  double MeVQ2=q2*1e6;
  double MeVW = W*1e3;

  kmaid_(&f_ichan, &MeVQ2, &MeVW, &theta,
	  &ST, &SL, &STL, &STT, &STLP);
  return SL;
}  

double kmaid::sigma_TL(double W, double q2, double eps, double theta) 
{
  double ST, SL, STL, STT, STLP;
  double MeVQ2=q2*1e6;
  double MeVW=  W*1e3;
  
  kmaid_(&f_ichan, &MeVQ2, &MeVW, &theta,
	  &ST, &SL, &STL, &STT, &STLP);
  return STL;
}

double kmaid::sigma_TLp(double W, double q2, double eps, double theta) 
{
  double ST, SL, STL, STT, STLP;
  double MeVQ2=q2*1e6;
  double MeVW = W*1e3;

  kmaid_(&f_ichan, &MeVQ2, &MeVW, &theta,
	  &ST, &SL, &STL, &STT, &STLP);
  return STLP;
}
