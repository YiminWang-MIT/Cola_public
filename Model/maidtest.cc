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
    std::cout << "Usage: maidtest q2(in GeV^2/c^2), prints multipoles"
    << std::endl;
    exit(0);
  }
  double q2 = atof(argv[1]);
  maid2000 Maid(1);

  double thr = (m_proton + m_pi0)*1e3;
  HIST *E0pr =HMBook1("E_0+!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *E0pi =HMBook1("Im E_0+!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *L0pr =HMBook1("L_0+!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *L0pi =HMBook1("Im L_0+!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);

  HIST *E1pr =HMBook1("E_1+!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *E1pi =HMBook1("Im E_1+!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *M1pr =HMBook1("M_1+!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *M1pi =HMBook1("Im M_1+!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *M1mr =HMBook1("M_1-!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *M1mi =HMBook1("Im M_1-!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *L1pr =HMBook1("L_1+!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *L1pi =HMBook1("Im L_1+!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *L1mr =HMBook1("L_1-!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *L1mi =HMBook1("Im L_1-!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);

  HIST *E2pr =HMBook1("E_2+!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *E2pi =HMBook1("Im E_2+!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *E2mr =HMBook1("E_2-!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *E2mi =HMBook1("Im E_2-!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *M2pr =HMBook1("M_2+!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *M2pi =HMBook1("Im M_2+!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *M2mr =HMBook1("M_2-!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *M2mi =HMBook1("Im M_2-!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *L2pr =HMBook1("L_2+!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *L2pi =HMBook1("Im L_2+!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *L2mr =HMBook1("L_2-!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);
  HIST *L2mi =HMBook1("Im L_2-!","W","","MeV","10^-3!/m_[p]+!",100,thr,2000);

  HIST *P1r = HMBook1("P_1!","W","","MeV","10^-3!/m_[p]!^2!",100,thr,thr+20);
  HIST *P2r = HMBook1("P_2!","W","","MeV","10^-3!/m_[p]!^2!",100,thr,thr+20);
  HIST *P3r = HMBook1("P_3!","W","","MeV","10^-3!/m_[p]!^2!",100,thr,thr+20);
  HIST *P4r = HMBook1("P_4!","W","","MeV","10^-3!/m_[p]!^2!",100,thr,thr+20);
  HIST *P5r = HMBook1("P_5!","W","","MeV","10^-3!/m_[p]!^2!",100,thr,thr+20);
  HIST *P23 = HMBook1("P_23!^2!","W","","MeV","(10^-3!/m_[p]!^2!)^2!",100,thr,thr+20);

  HIST *sT  = cs("[s]_T!",  "d[s]_T!(90°)/d[W]");
  HIST *sL  = cs("[s]_L!",  "d[s]_L!(90°)/d[W]");
  HIST *sTT = cs("[s]_TT!", "d[s]_TT!(90°)/d[W]");
  HIST *sTL = cs("[s]_TL!", "d[s]_TL!(90°)/d[W]");
  HIST *sTLp = cs("[s]_TL'!", "d[s]_TL'!(90°)/d[W]");

  for(double W=thr+(2000-thr)/200; W<2000;W+=(2000-thr)/100){
    Maid.ps((W-thr)/1e3, q2, 90.0/180*M_PI);

    HMFill(sT ,W,0,Maid.sigma_T ((W-thr)/1e3, q2, 0.0,90.0/180*M_PI));
    HMFill(sL ,W,0,Maid.sigma_L ((W-thr)/1e3, q2, 0.0,90.0/180*M_PI));
    HMFill(sTT,W,0,Maid.sigma_TT((W-thr)/1e3, q2, 0.0,90.0/180*M_PI));
    HMFill(sTL,W,0,Maid.sigma_TL((W-thr)/1e3, q2, 0.0,90.0/180*M_PI));
    HMFill(sTLp,W,0,Maid.sigma_TLp((W-thr)/1e3, q2, 0.0,90.0/180*M_PI));

    HMFill(E0pr,W,0,Maid.Ep(0).real()); HMFill(E0pi,W,0,Maid.Ep(0).imag());
    HMFill(L0pr,W,0,Maid.Lp(0).real()); HMFill(L0pi,W,0,Maid.Lp(0).imag());
    HMFill(E1pr,W,0,Maid.Ep(1).real()); HMFill(E1pi,W,0,Maid.Ep(1).imag());
    HMFill(M1pr,W,0,Maid.Mp(1).real()); HMFill(M1pi,W,0,Maid.Mp(1).imag());
    HMFill(M1mr,W,0,Maid.Mm(1).real()); HMFill(M1mi,W,0,Maid.Mm(1).imag());
    HMFill(L1pr,W,0,Maid.Lp(1).real()); HMFill(L1pi,W,0,Maid.Lp(1).imag());
    HMFill(L1mr,W,0,Maid.Lm(1).real()); HMFill(L1mi,W,0,Maid.Lm(1).imag());
    HMFill(E2pr,W,0,Maid.Ep(2).real()); HMFill(E2pi,W,0,Maid.Ep(2).imag());
    HMFill(E2mr,W,0,Maid.Em(2).real()); HMFill(E2mi,W,0,Maid.Em(2).imag());
    HMFill(M2pr,W,0,Maid.Mp(2).real()); HMFill(M2pi,W,0,Maid.Mp(2).imag());
    HMFill(M2mr,W,0,Maid.Mm(2).real()); HMFill(M2mi,W,0,Maid.Mm(2).imag());
    HMFill(L2pr,W,0,Maid.Lp(2).real()); HMFill(L2pi,W,0,Maid.Lp(2).imag());
    HMFill(L2mr,W,0,Maid.Lm(2).real()); HMFill(L2mi,W,0,Maid.Lm(2).imag());

  }
  for(double W=thr+20.0/200; W<thr+20;W+=20.0/100){
    Maid.ps((W-thr)/1e3, q2, 90.0/180*M_PI);
    double s = pow(W/1e3,2);
    double q = sqrt((s-pow(m_pi0+m_proton,2))*(s-pow(m_pi0-m_proton,2))/4/s); 
    double scale = m_pion /q;
    HMFill(P1r,W,0,Maid.P1().real()*scale); 
    HMFill(P2r,W,0,Maid.P2().real()*scale); 
    HMFill(P3r,W,0,Maid.P3().real()*scale); 
    HMFill(P4r,W,0,Maid.P4().real()*scale); 
    HMFill(P5r,W,0,Maid.P5().real()*scale); 
    HMFill(P23, W, 0, 
	   (sqr(Maid.P2().real()) + sqr(Maid.P3().real()))/2*scale*scale); 
  }
  Maid.ps(0.000001, q2, 90.0/180*M_PI);
  double s = pow(thr/1e3+0.00005,2);
  double q = sqrt((s-pow(m_pi0+m_proton,2))*(s-pow(m_pi0-m_proton,2))/4/s); 
  double scale = m_pion /q;
 
  std::cout << "E0: " << std::setw(6) << std::setprecision(2)
	    << Maid.E0p().real() << std::endl;
  std::cout << "L0: " << std::setw(6) << std::setprecision(2)
	    << Maid.L0p().real() << std::endl;
  std::cout << "P1: " << std::setw(6) << std::setprecision(2)
	    << Maid.P1().real()*scale << std::endl;
  std::cout << "P2: " << std::setw(6) << std::setprecision(2)
	    << Maid.P2().real()*scale << std::endl;
  std::cout << "P3: " << std::setw(6) << std::setprecision(2)
	    << Maid.P3().real()*scale << std::endl;
  std::cout << "P4: " << std::setw(6) << std::setprecision(2)
	    << Maid.P4().real()*scale << std::endl;
  std::cout << "P5: " << std::setw(6) << std::setprecision(2)
	    << Maid.P5().real()*scale << std::endl;
  std::cout << "P23: " << std::setw(6) << std::setprecision(2)
	    << (sqr(Maid.P2().real())
		+ sqr(Maid.P3().real()))/2*scale*scale << std::endl;

  HMWriteAll("Multipoles.his");
  HMPlotFile("Multipoles.ps");
  HMTitle("MAID Multipoles [g]p [\256] [p]^0!p   l=0");
  HMZone(1,2,1);
  HMMinimumY(-10); HMMaximumY(10);HMFunction(E0pr);HMSame();HMFunction(E0pi);
  HMMinimumY(-10); HMMaximumY(10);HMFunction(L0pr);HMSame();HMFunction(L0pi);
  HMNewPage();
  HMZone(2,3,2);
  HMTitle("MAID Multipoles [g]p [\256] [p]^0!p   l=1");
  HMMinimumY( -5); HMMaximumY( 5);HMFunction(E1pr);HMSame();HMFunction(E1pi);
  HMMinimumY( -5); HMMaximumY( 5);HMFunction(M1mr);HMSame();HMFunction(M1mi);
  HMMinimumY(-15); HMMaximumY(30);HMFunction(M1pr);HMSame();HMFunction(M1pi);
  HMMinimumY( -5); HMMaximumY( 5);HMFunction(L1mr);HMSame();HMFunction(L1mi);
  HMMinimumY( -5); HMMaximumY( 5);HMFunction(L1pr);HMSame();HMFunction(L1pi);
  HMNewPage();
  HMZone(2,3,1);
  HMTitle("MAID Multipoles [g]p [\256] [p]^0!p   l=2");
  HMMinimumY(-10); HMMaximumY(10);HMFunction(E2mr);HMSame();HMFunction(E2mi);
  HMMinimumY( -1); HMMaximumY( 1);HMFunction(E2pr);HMSame();HMFunction(E2pi);
  HMMinimumY( -5); HMMaximumY( 5);HMFunction(M2mr);HMSame();HMFunction(M2mi);
  HMMinimumY( -1); HMMaximumY( 2);HMFunction(M2pr);HMSame();HMFunction(M2pi);
  HMMinimumY( -5); HMMaximumY( 5);HMFunction(L2mr);HMSame();HMFunction(L2mi);
  HMMinimumY( -1); HMMaximumY( 2);HMFunction(L2pr);HMSame();HMFunction(L2pi);
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
  HMClose();
  return 0;
}

