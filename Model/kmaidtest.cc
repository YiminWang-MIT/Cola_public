#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include "Model/Model.h"
#include "HMBook/hmbook.h"
#include "HMBook/hmplot.h"

int main(int argc, char *argv[])
{
  if (argc!=6) {
    std::cout << "Usage: kmaidtest W (GeV) Q2 (in GeV^2/c^2) eps (0<eps<1) theta_cms (in deg) chn (1=Lambda; 3=Sigma)"
	      << std::endl;
    exit(0);
  }

  double Wo   = atof(argv[1]);
  double q2   = atof(argv[2]);
  double eps  = atof(argv[3]);
  double thcms= atof(argv[4]);
  int    chn  = atof(argv[5]);

  std::cout << "W (GeV): " << Wo << " Q2 (in GeV^2/c^2): " << q2 
	    << " eps (0<eps<1): " << eps << " theta_cms (in deg): " 
	    << thcms << " chn (1=Lambda; 3=Sigma): " << chn
	    << std::endl;

  kmaid KMaid(chn);

  double m_hyp;
  if (chn == 1) m_hyp = m_Lambda;
  else if (chn == 3) m_hyp = m_Sigma0;
  else return 0;

  double thr = (m_hyp + m_kaon)*1e3;

  HIST *s0   = HMBook1("[s]_o!", "W", "d[s]_o!/d[W]", "MeV", "[m]b/sr", 100, 
		       (m_hyp + m_kaon)*1e3,2000);
  HIST *sT   = HMBook1("[s]_T!", "W", "d[s]_T!/d[W]", "MeV", "[m]b/sr", 100, 
		       (m_hyp + m_kaon)*1e3,2000);
  HIST *sL   = HMBook1("[s]_L!", "W", "d[s]_L!/d[W]", "MeV", "[m]b/sr", 100, 
		       (m_hyp + m_kaon)*1e3,2000);
  HIST *sTT  = HMBook1("[s]_TT!", "W", "d[s]_TT!/d[W]", "MeV", "[m]b/sr", 100, 
		       (m_hyp + m_kaon)*1e3,2000);
  HIST *sTL  = HMBook1("[s]_TL!", "W", "d[s]_TL!/d[W]", "MeV", "[m]b/sr", 100, 
		       (m_hyp + m_kaon)*1e3,2000);
  HIST *sTLp = HMBook1("[s]_TLp!", "W", "d[s]_TLp!/d[W]", "MeV", "[m]b/sr", 
		       100, (m_hyp + m_kaon)*1e3,2000);

  HIST *dsdth= HMBook1("d[s]_o!dcos[Q]", "cos [Q]^*!_K!", 
		       "d[s]_o!/dcos [Q]^*!_K!", "", "[m]b/sr", 100, -0.1, 0.9);

  HIST *dsdQ2= HMBook1("d[s]_o!dQ^2!", "Q^2!", 
		       "d[s]_o!/dQ^2!", "GeV^2", "[m]b/sr", 100, 0, 0.08);

  for(double W= thr+(2000-thr)/200; W<2000; W+=(2000-thr)/100){
    HMFill(s0 , W, 1, KMaid.sigma_0(  W/1e3, q2, eps, thcms/180*M_PI));
    HMFill(sT , W, 1, KMaid.sigma_T(  W/1e3, q2, eps, thcms/180*M_PI));
    HMFill(sL , W, 1, KMaid.sigma_L(  W/1e3, q2, eps, thcms/180*M_PI));
    HMFill(sTT, W, 1, KMaid.sigma_TT( W/1e3, q2, eps, thcms/180*M_PI));
    HMFill(sTL, W, 1, KMaid.sigma_TL( W/1e3, q2, eps, thcms/180*M_PI));
    HMFill(sTLp,W, 1, KMaid.sigma_TLp(W/1e3, q2, eps, thcms/180*M_PI));
  }

  for(double cth= -0.095; cth<0.9; cth+=0.01){
    HMFill(dsdth, cth, 1, KMaid.sigma_0( Wo, q2, eps, acos(cth))); }

  for(double Q2= 0.0004; Q2<0.08; Q2+=0.0008){
    HMFill(dsdQ2, Q2, 1, KMaid.sigma_0( Wo, Q2, eps, thcms/180*M_PI)); }

  char name[30]; 
  sprintf(name, "kmaidtest-chn-%s.his", argv[5]);
  HMWriteAll(name);

  sprintf(name, "kmaidtest-W-chn-%s.ps", argv[5]);
  HMPlotFile(name);
  HMTitle("KMaid [g] p [\256] K^+!Y");
  HMSetOption( 9, 2. ); // LAB_X
  HMSetOption(10, 1.5); // LAB_Y
  HMSetOption(15, 0.8); // FONTSIZE_COMMENT
  HMSetOption(13, 0.5); // FONTSIZE_AXIS
  HMSetOption(14, 0.8); // FONTSIZE_LABEL
  HMSetOption(37, 0.1); // LINE_WIDTH
  HMMinimumY(-0.2); HMMaximumY(0.8);
  HMFunction(s0); HMExplain("[s]_0!"); HMSame();
  HMFunction(sT); HMExplain("[s]_T!"); HMSame(); 
  HMFunction(sL); HMExplain("[s]_L!"); HMSame(); 
  HMFunction(sTT);HMExplain("[s]_TT!");HMSame(); 
  HMFunction(sTL);HMExplain("[s]_TL!");
  HMClose();

  sprintf(name, "kmaidtest-th-chn-%s.ps", argv[5]);
  HMPlotFile(name);
  HMTitle("KMaid [g] p [\256] K^+!Y");
  HMSetOption( 9, 2. ); // LAB_X
  HMSetOption(10, 1.5); // LAB_Y
  HMSetOption(15, 0.8); // FONTSIZE_COMMENT
  HMSetOption(13, 0.5); // FONTSIZE_AXIS
  HMSetOption(14, 0.8); // FONTSIZE_LABEL
  HMSetOption(37, 0.1); // LINE_WIDTH
  HMMinimumY(0); HMMaximumY(0.8);
  HMFunction(dsdth);
  HMClose();

  sprintf(name, "kmaidtest-Q2-chn-%s.ps", argv[5]);
  HMPlotFile(name);
  HMTitle("KMaid [g] p [\256] K^+!Y");
  HMSetOption( 9, 2. ); // LAB_X
  HMSetOption(10, 1.5); // LAB_Y
  HMSetOption(15, 0.8); // FONTSIZE_COMMENT
  HMSetOption(13, 0.5); // FONTSIZE_AXIS
  HMSetOption(14, 0.8); // FONTSIZE_LABEL
  HMSetOption(37, 0.1); // LINE_WIDTH
  HMMinimumY(0); HMMaximumY(0.8);
  HMMinimumX(0.002);
  HMFunction(dsdQ2);
  HMClose();

  return 0;
}

