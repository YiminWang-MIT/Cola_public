//                                                                    -*-C++-*-
// $Id: BetheHeitler.cc 2216 2008-06-13 21:13:47Z distler $

#include "Simulation/Generate.h"
#include "Simulation/QED.h"
#include "Simulation/Formfactor.h"
#include "FourVector/FourVector.h"
#include "HMBook/hmbook.h"
#include "Cola/ColaNtuple.h"


#include <stdlib.h>
#include <fstream>
#include <iostream>

using namespace std;

// Ntuple stuff, just for linkage => ignore
struct {  int hmemor[HBOOKMEM];} pawc_;
char *ntvarn[MAX_NT_VAR];   // Scalar name related to ntvar
char *ntdes[MAX_NT_VAR];    // NTvariable name
char *ntbck[MAX_NT_VAR];    // Block name for each ntvar
char *nttitle;              // Ntuple Title
int  ntiden, ntnvar;
int  iselastic=false;
SobolSequence sobol;
int  verbose =1;
//////////

int main(int, char *[]) 
{
  //vcsfpp45
  // double E0    = 0.76638;        // Beam energy E0 GeV
  // double Ef    = 0.52728;        // outgoing electron energy Ef GeV
  // double theta = 54.4*M_PI/180;  // outgoing electron angle theta

  double E0    = 0.8525;          // Beam energy E0 GeV
  double Ef    = sqrt( pow(0.565,2)+ m_e_sqr ); // outgoing electron energy Ef GeV
  double theta = 50.7*M_PI/180;  // outgoing electron angle theta


  //Check (from Proposal)
  //double E0    = 0.855;        // Beam energy E0 GeV
  //double Ef    = 0.539;        // outgoing electron energy Ef GeV
  //double theta = 52.2*M_PI/180;  // outgoing electron angle theta


  FourVector ein  = FourVector(E0, 0, 0, sqrt(E0*E0 - m_e_sqr));
  FourVector eout = FourVector(Ef, 0, 0, sqrt(Ef*Ef - m_e_sqr));
  eout.rot_theta(theta);
  FourVector phvir = ein - eout;

  // The virtual photon defines the Z direction
  ein  = ein.rotate(phvir);
  eout = eout.rotate(phvir);
  
  FourVector CM = ein - eout + FourVector(m_proton, 0, 0, 0);
  double s = CM.square(); 

  Momentum spinCM;
  Momentum spin;

  HIST *id  = HMBook1("Cross Section","[Q]_[gg]!","","^o!","[m]barn/GeV sr^2!",
		     360, -180, 180);
  HIST *idx = HMBook1("Polarization x","[Q]_[gg]!","P_x!","^o!","%",
		     360, -180, 180);
  HIST *idy = HMBook1("Polarization y","[Q]_[gg]!","P_y!","^o!","%",
		     360, -180, 180);
  HIST *idz = HMBook1("Polarization z","[Q]_[gg]!","P_z!","^o!","%",
		     360, -180, 180);
  
    ein    = ein.rotateTo(phvir);
    eout   = eout.rotateTo(phvir);
  for (double thgg=-179.5;thgg<180;thgg+=360.0/360) {
     
    FourVector photon; 
    photon.initPolar(1, 1, thgg*M_PI/180, M_PI);   
    double k = ( s-m_proton*m_proton )/2/sqrt(s);
    photon = photon*k;

    Particle proton(CM - photon);
    proton = proton.Lorentz(-CM).setSpin(spin).Lorentz(CM);   
    photon = photon.Lorentz(CM);
   
    photon = photon.rotateTo(phvir);
    double weight = ElasticCrossSection(ein,eout,photon,1,0,&spin,&spinCM);

   
    HMFill(id,  thgg, 0, weight);
    HMFill(idx, thgg, 0, spinCM[0]);
    HMFill(idy, thgg, 0, spinCM[1]);
    HMFill(idz, thgg, 0, spinCM[2]);
  }

  /*ifstream in("/users/kpha1/merkel/VCS/VCSMarcVanderhaeghen/exe/compton/"
    "vcs_diff1_bhborn.dat");*/

  //Temporaneo!!
  ifstream in("vcs_recoilpolx_kin766_kout527_kth54_bhborn.dat"); 

  if (in.good()) {
    HIST *idvdh = HMBook1("Cross Section (Vanderaheghen)",
			  "","","","",73,-182.5,182.5);

    for (double th=-180;th<=180; th+=5) {
      double theta, cs;
 
      in >> theta >> cs;
      HMFill(idvdh, theta, 0, cs/1e3);
    }
  }

  ifstream in1("vcs_recoilpolx_kin766_kout527_kth54_bhborn.dat");  
  ifstream in2("vcs_recoilpolz_kin766_kout527_kth54_bhborn.dat");  
  //ifstream in1("vcs_recoilpolx_kin855_kout539_kth52_bhborn.dat");  
  //ifstream in2("vcs_recoilpolz_kin855_kout539_kth52_bhborn.dat");  
  HIST *id1=HMBook1("MV Polarization X","[q]_[gg]!","P_x!","","",72,-180,180);
  HIST *id2=HMBook1("MV Polarization Z","[q]_[gg]!","P_z!","","",72,-180,180);


  if (in1.good() && in2.good()) {
    for (double th=-180;th<=180; th+=5) {
      double theta, nuu,ndd,nud,ndu;
      in1 >> theta>> nuu>>ndd>>nud>>ndu;
      //cout<<" "<<theta<<" "<<nuu<<" "<<ndd<<" "<<nud<<" "<<ndu<<" "<<endl;
      HMFill(id1,theta,0,100 * (nuu-ndd)/(nuu+ndd));


      in2 >> theta>> nuu>>ndd>>nud>>ndu;
      HMFill(id2,theta,0,100 * (nuu-ndd)/(nuu+ndd));
    }
  }
  HMWriteAll("BetheHeitler.his");
}



 
