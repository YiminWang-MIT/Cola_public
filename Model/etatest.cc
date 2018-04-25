#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include "Model/Model.h"
#include "HMBook/hmbook.h"
#include "HMBook/hmplot.h"

double maxi = 1700;

inline HIST *cs(char *title, char*label) {
  return HMBook1(title,"W", label, "MeV", "[m]b/sr", 100, 
		 (m_proton + m_eta)*1e3,maxi);
}

int main(int argc, char *argv[])
{
  if (argc !=5) {cout << argv[0]<< " q2 eps theta phi\n";return -1;}
  double q2  = atof(argv[1]);
  double eps = atof(argv[2]);
  double th  = atof(argv[3])/180*M_PI;
  double ph  = atof(argv[4])/180*M_PI;
  etaMaid Maid("maid.dat");

  double thr = (m_proton + m_eta)*1e3;

  HIST *h[] =  { cs("[s]_T!",  "d[s]_T!(90°)/d[W]"),
		 cs("[s]_L!",  "d[s]_L!(90°)/d[W]"),
		 cs("[s]_TT!", "d[s]_TT!(90°)/d[W]"),
		 cs("[s]_TL!", "d[s]_TL!(90°)/d[W]"),
		 cs("[s]_TL'!", "d[s]_TL'!(90°)/d[W]"),
		 cs("[s]_T!^y'0!", "d[s](90°)/d[W]"),
		 cs("[s]_sTL!^x'0!", "d[s](90°)/d[W]"),
		 cs("[s]_sTL!^z'0!", "d[s](90°)/d[W]"),
		 cs("[s]_sTT!^x'0!", "d[s](90°)/d[W]"),
		 cs("[s]_sTT!^z'0!", "d[s](90°)/d[W]"),
		 cs("[s]_cTL'!^x'0!", "d[s](90°)/d[W]"),
		 cs("[s]_cTL'!^z'0!", "d[s](90°)/d[W]"),
		 cs("[s]_TT'!^x'0!", "d[s](90°)/d[W]"),
		 cs("[s]_TT'!^z'0!", "d[s](90°)/d[W]"),
		 cs("[s]_L!^y'0!", "d[s](90°)/d[W]"),
		 cs("[s]_cTL!^y'0!", "d[s](90°)/d[W]"),
		 cs("[s]_cTT!^y'0!", "d[s](90°)/d[W]"),
		 cs("[s]_sTL'!^y'0!", "d[s](90°)/d[W]"),
		 cs("P_x!", "%"),
		 cs("P_x!(h)", "%"),
		 cs("P_y!", "%"),
		 cs("P_y!(h)", "%"),
		 cs("P_z!", "%"),
		 cs("P_z!(h)", "%"),
		 cs("P_x!^CM!", "%"),
		 cs("P_y!^CM!", "%"),
		 cs("P_z!^CM!", "%"),
		 cs("P_x!^CM!(h)", "%"),
		 cs("P_y!^CM!(h)", "%"),
		 cs("P_z!^CM!(h)", "%"),
		 cs("P_x!^CM!(h2)", "%"),
		 cs("P_y!^CM!(h2)", "%"),
		 cs("P_z!^CM!(h2)", "%")};
  
  for(double W=thr+(maxi-thr)/200; W<maxi;W+=(maxi-thr)/100){
    HMFill(h[ 0], W,0,Maid.sigma_T     ((W-thr)/1e3, q2, th, ph));
    HMFill(h[ 1], W,0,Maid.sigma_L     ((W-thr)/1e3, q2, th, ph));
    HMFill(h[ 2], W,0,Maid.sigma_TT    ((W-thr)/1e3, q2, th, ph));
    HMFill(h[ 3], W,0,Maid.sigma_TL    ((W-thr)/1e3, q2, th, ph));
    HMFill(h[ 4], W,0,Maid.sigma_TLp   ((W-thr)/1e3, q2, th, ph));
    HMFill(h[ 5], W,0,Maid.sigma_T_y   ((W-thr)/1e3, q2, th, ph));
    HMFill(h[ 6], W,0,Maid.sigma_sTL_x ((W-thr)/1e3, q2, th, ph));
    HMFill(h[ 7], W,0,Maid.sigma_sTL_z ((W-thr)/1e3, q2, th, ph));
    HMFill(h[ 8], W,0,Maid.sigma_sTT_x ((W-thr)/1e3, q2, th, ph));
    HMFill(h[ 9], W,0,Maid.sigma_sTT_z ((W-thr)/1e3, q2, th, ph));
    HMFill(h[10], W,0,Maid.sigma_cTLp_x((W-thr)/1e3, q2, th, ph));
    HMFill(h[11], W,0,Maid.sigma_cTLp_z((W-thr)/1e3, q2, th, ph));
    HMFill(h[12], W,0,Maid.sigma_TTp_x ((W-thr)/1e3, q2, th, ph));
    HMFill(h[13], W,0,Maid.sigma_TTp_z ((W-thr)/1e3, q2, th, ph));
    HMFill(h[14], W,0,Maid.sigma_L_y   ((W-thr)/1e3, q2, th, ph));
    HMFill(h[15], W,0,Maid.sigma_cTL_y ((W-thr)/1e3, q2, th, ph));
    HMFill(h[16], W,0,Maid.sigma_cTT_y ((W-thr)/1e3, q2, th, ph));
    HMFill(h[17], W,0,Maid.sigma_sTLp_y((W-thr)/1e3, q2, th, ph));
    HMFill(h[18], W,0,Maid.Px ((W-thr)/1e3, q2, eps, th,ph));
    HMFill(h[19], W,0,Maid.Pxh((W-thr)/1e3, q2, eps, th,ph));
    HMFill(h[20], W,0,Maid.Py ((W-thr)/1e3, q2, eps, th,ph));
    HMFill(h[21], W,0,Maid.Pyh((W-thr)/1e3, q2, eps, th,ph));
    HMFill(h[22], W,0,Maid.Pz ((W-thr)/1e3, q2, eps, th,ph));
    HMFill(h[23], W,0,Maid.Pzh((W-thr)/1e3, q2, eps, th,ph));

    double P[3]; 
    Maid.P_CMS(P, (W-thr)/1e3, q2, eps, th, ph,1);
    HMFill(h[24], W, 0, P[0]);
    HMFill(h[25], W, 0, P[1]);
    HMFill(h[26], W, 0, P[2]);
    double P2[3]; 
    Maid.P_CMS(P2, (W-thr)/1e3, q2, eps, th, ph,0);
    HMFill(h[27], W, 0, P[0]-P2[0]);
    HMFill(h[28], W, 0, P[1]-P2[1]);
    HMFill(h[29], W, 0, P[2]-P2[2]);

    double P3[3],P4[3]; 
    Maid.P_CMS(P3, (W-thr)/1e3, q2, eps, th, ph, 1);
    Maid.P_CMS(P4, (W-thr)/1e3, q2, eps, th, ph,-1);
    HMFill(h[30], W, 0, (P3[0]-P4[0])/2);
    HMFill(h[31], W, 0, (P3[1]-P4[1])/2);
    HMFill(h[32], W, 0, (P3[2]-P4[2])/2);
  }
  HMWriteAll("Multipoles.his");
  HMPlotFile("Multipoles.ps");
  char tit[255];
  sprintf(tit,"[h]-MAID [q]=%s^o! q^2!=%s",argv[3],argv[1]);
  HMZone(2,3,1);
  for (int i=0;i<33;i++)  {
    if (i<18) { HMMaximumY(2.0);
    HMMinimumY(-2.0); } else  { HMMaximumY(NOMINIMUM);
    HMMinimumY(NOMINIMUM); }
    HMFunction(h[i]);
    if (i%6==0) HMTitle(tit);
  }  
  HMClose();
  return 0;
}

