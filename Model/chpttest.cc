#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "Model/Model.h"
#include "HMBook/hmbook.h"
#include "HMBook/hmplot.h"

inline HIST *cs(char *title, char*label) {
  return HMBook1(title,"W", label, "MeV", "nb/sr", 100, 
		 (m_proton + m_pion)*1e3,2000);
}

int main(int argc, char *argv[])
{
  if (argc!=2) {
    std::cout << "Usage: chpttest q2(in GeV^2/c^2), prints multipoles"
    << std::endl;
    exit(0);
  }
  double q2 = atof(argv[1]);
  ChPT chpt;

  double thr = (m_proton + m_pi0)*1e3;
  HIST *E0pr =HMBook1("E_0+!","W","","MeV","10^-3!/m_[p]+!",100,thr,thr+15);
  HIST *E0pi =HMBook1("Im E_0+!","W","","MeV","10^-3!/m_[p]+!",100,thr,thr+15);
  HIST *L0pr =HMBook1("L_0+!","W","","MeV","10^-3!/m_[p]+!",100,thr,thr+15);
  HIST *L0pi =HMBook1("Im L_0+!","W","","MeV","10^-3!/m_[p]+!",100,thr,thr+15);
  HIST *P1r = HMBook1("P_1!","W","","MeV","10^-3!/m_[p]!^2!",100,thr,thr+15);
  HIST *P2r = HMBook1("P_2!","W","","MeV","10^-3!/m_[p]!^2!",100,thr,thr+15);
  HIST *P3r = HMBook1("P_3!","W","","MeV","10^-3!/m_[p]!^2!",100,thr,thr+15);
  HIST *P4r = HMBook1("P_4!","W","","MeV","10^-3!/m_[p]!^2!",100,thr,thr+15);
  HIST *P5r = HMBook1("P_5!","W","","MeV","10^-3!/m_[p]!^2!",100,thr,thr+15);
  HIST *P23 = HMBook1("P_23!^2!","W","","MeV","(10^-3!/m_[p]!^2!)^2!",100,thr,thr+15);

  HIST *a0e  = HMBook1("a_0!","Q^2!","","","[m]b/sr",100,0.0,0.12);
  HIST *a0l  = HMBook1("a_0!","Q^2!","","","[m]b/sr",100,0.0,0.12);
  HIST *a01  = HMBook1("a_0!","Q^2!","","","[m]b/sr",100,0.0,0.12);
  HIST *a02  = HMBook1("a_0!","Q^2!","","","[m]b/sr",100,0.0,0.12);
  HIST *a03  = HMBook1("a_0!","Q^2!","","","[m]b/sr",100,0.0,0.12);

  HIST *sT  = cs("[s]_T!",  "d[s]_T!(90°)/d[W]");
  HIST *sL  = cs("[s]_L!",  "d[s]_L!(90°)/d[W]");
  HIST *sTT = cs("[s]_TT!", "d[s]_TT!(90°)/d[W]");
  HIST *sTL = cs("[s]_TL!", "d[s]_TL!(90°)/d[W]");
  HIST *sTLp = cs("[s]_TL'!", "d[s]_TL'!(90°)/d[W]");

  for(double W=thr+(2000-thr)/200; W<2000;W+=(2000-thr)/100){
    HMFill(sT ,W,0,chpt.sigma_T ((W-thr)/1e3, q2, 0.0,90.0/180*M_PI));
    HMFill(sL ,W,0,chpt.sigma_L ((W-thr)/1e3, q2, 0.0,90.0/180*M_PI));
    HMFill(sTT,W,0,chpt.sigma_TT((W-thr)/1e3, q2, 0.0,90.0/180*M_PI));
    HMFill(sTL,W,0,chpt.sigma_TL((W-thr)/1e3, q2, 0.0,90.0/180*M_PI));
    HMFill(sTLp,W,0,chpt.sigma_TLp((W-thr)/1e3, q2, 0.0,90.0/180*M_PI));

  }
  for(double Q2=0+0.12/200;Q2<0.12;Q2+=0.12/100) {
    chpt.sigma_T (0.000001, -Q2, 0.67,0);
    HMFill(a0e,Q2,0,mubarn/ sqr(1e3 * m_pion)*norm(chpt.E0p()));
    chpt.sigma_L (0.000001, -Q2, 0.67,0);    
    HMFill(a0l,Q2,0,mubarn/ sqr(1e3 * m_pion)*norm(chpt.L0p())*chpt.epsL);
    HMFill(a01,Q2,0,mubarn/ sqr(1e3 * m_pion)*(norm(chpt.E0p())+norm(chpt.L0p())*chpt.epsL));
    chpt.sigma_L (0.000001, -Q2, 0.58,0);    
    HMFill(a02,Q2,0,mubarn/ sqr(1e3 * m_pion)*(norm(chpt.E0p())+norm(chpt.L0p())*chpt.epsL));
    chpt.sigma_L (0.000001, -Q2, 0.79,0);    
    HMFill(a03,Q2,0,mubarn/ sqr(1e3 * m_pion)*(norm(chpt.E0p())+norm(chpt.L0p())*chpt.epsL));

  } 

  chpt.sigma_L (0.000001, -0.06, 0.67,0);
  double d1 = mubarn/ sqr(1e3 * m_pion)*norm(chpt.L0p())*chpt.epsL;
  chpt.sigma_L (0.000001, -0.12, 0.67,0);
  double d2 = mubarn/ sqr(1e3 * m_pion)*norm(chpt.L0p())*chpt.epsL;
  cout <<"d2/d1= "<<d2/d1<<endl;

  for(double W=thr+15.0/200; W<thr+20;W+=15.0/100){
    // double s = pow(W/1e3,2);
    //    double q = sqrt((s-pow(m_pi0+m_proton,2))*(s-pow(m_pi0-m_proton,2))/4/s); 
    double scale = 1;//m_pion /q;
    chpt.sigma_T ((W-thr)/1e3, q2, 0.0,90.0/180*M_PI);
    HMFill(E0pr, W, 0, chpt.E0p().real()*scale); 
    HMFill(E0pi, W, 0, chpt.E0p().imag()*scale); 
    HMFill(L0pr, W, 0, chpt.L0p().real()*scale); 
    HMFill(L0pi, W, 0, chpt.L0p().imag()*scale); 
    HMFill(P1r, W, 0, chpt.P1().real()*scale); 
    HMFill(P2r, W, 0, chpt.P2().real()*scale); 
    HMFill(P3r, W, 0, chpt.P3().real()*scale); 
    HMFill(P4r, W, 0, chpt.P4().real()*scale); 
    HMFill(P5r, W, 0, chpt.P5().real()*scale); 
    HMFill(P23, W, 0, 
	   (sqr(chpt.P2().real()) + sqr(chpt.P3().real()))/2*scale*scale); 
  }
  double s = pow(thr/1e3+0.00005,2);
  double q = sqrt((s-pow(m_pi0+m_proton,2))*(s-pow(m_pi0-m_proton,2))/4/s); 
  double scale = m_pion /q;
 
  std::cout << "E0: " << std::setw(6) << std::setprecision(2)
	    << chpt.E0p().real() << std::endl;
  std::cout << "L0: " << std::setw(6) << std::setprecision(2)
	    << chpt.L0p().real() << std::endl;
  std::cout << "P1: " << std::setw(6) << std::setprecision(2)
	    << chpt.P1().real()*scale << std::endl;
  std::cout << "P2: " << std::setw(6) << std::setprecision(2)
	    << chpt.P2().real()*scale << std::endl;
  std::cout << "P3: " << std::setw(6) << std::setprecision(2)
	    << chpt.P3().real()*scale << std::endl;
  std::cout << "P4: " << std::setw(6) << std::setprecision(2)
	    << chpt.P4().real()*scale << std::endl;
  std::cout << "P5: " << std::setw(6) << std::setprecision(2)
	    << chpt.P5().real()*scale << std::endl;
  std::cout << "P23: " << std::setw(6) << std::setprecision(2)
	    << (sqr(chpt.P2().real())
		+ sqr(chpt.P3().real()))/2*scale*scale << std::endl;

  HMWriteAll("Multipoles.his");
  HMPlotFile("Multipoles.ps");
  HMTitle("MAID Multipoles [g]p [\256] [p]^0!p   l=0");
  HMZone(1,2,1);
  HMMinimumY(0); HMMaximumY(3.5);HMFunction(E0pr);HMSame();HMFunction(E0pi);
    HMMinimumY(-1.6); HMMaximumY(-1.2);
  HMFunction(L0pr);HMSame();HMFunction(L0pi);
  HMNewPage();
  HMZone(2,3,1);
  HMFunction(sT);
  HMFunction(sL);
  HMFunction(sTT);
  HMFunction(sTL);
  HMFunction(sTLp);
  HMNewPage();
  HMZone(2,3,1);
  HMFunction(P1r);
  HMFunction(P2r);
  HMFunction(P3r);
  HMFunction(P4r);
  HMFunction(P5r);
  HMFunction(P23);
  HMNewPage();
  HMMinimumY(0);
  HMMaximumY(0.7);
  HMFunction(a0l);
  HMSame();
  HMFunction(a0e);
  HMSame();
  HMFunction(a01);
  HMSame();
  HMFunction(a02);
  HMSame();
  HMFunction(a03);
  
  HMZone(2,3,3);
  HMMinimumX(NOMINIMUM);
  HMMaximumX(NOMINIMUM);
  HMMinimumY(-0.2); HMMaximumY(1.6);
  for(double q2=-0.06;q2<-0.03;q2+=0.01) {
    int i=0;
    for(double W=thr+15.0/200; W<thr+15;W+=15.0/100){
      chpt.sigma_T ((W-thr)/1e3, q2, 0.0,90.0/180*M_PI);
      E0pr->data[i++] = chpt.E0p().real();   
    }
    
    HMFunction(E0pr);
    if (q2<-0.045) HMSame();
  }
  
  HMMinimumY(-1.6); HMMaximumY(-1.2);
  for(q2=-0.06;q2<-0.03;q2+=0.01) {
    int i=0;
    for(double W=thr+15.0/200; W<thr+15;W+=15.0/100){
      chpt.sigma_T ((W-thr)/1e3, q2, 0.0,90.0/180*M_PI);
      L0pr->data[i++] = chpt.L0p().real(); 
    }
    HMFunction(L0pr);
    if (q2<-0.045) HMSame();
  }
  
  HMClose();
  return 0;
}

