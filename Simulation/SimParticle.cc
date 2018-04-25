//
// $Id: SimParticle.cc 2435 2011-11-03 15:14:08Z patrick $
//

using namespace std;
#include "SimParticle.h"
#include "../Cola/Masses.h"
#include "ran1.h"


long int rannum = -1;  // Needed for random generator


// Default constructor
SimParticle::SimParticle(){  
  ID = P_electron.getID(); 
  mass = P_electron.getMass()*1000.0;
  charge = P_electron.getCharge();
  gfactor = g_electron;
    
  decaymode = 0;
  livetime = 10000000.0; // very very long
  status = 1;
  
  
  position = Vector3D(0.0,0.0,0.0);
  fVmomentum = FourVector(mass, 0.0, 0.0, 0.0);
  spin  = Vector3D(0.0,0.0,0.0);
}


SimParticle::SimParticle(int id, Vector3D lega, FourVector mom, Vector3D vSpin){  
  
  // First we set position, spin, and momentum of the new particle to the given values 
  position = lega;
  fVmomentum = mom;
  spin  = vSpin;
  
  status  = 1;
  
  // First we check if given ID corresponds to the POSITRON
  if (id == P_positron.getID()){
    ID = P_positron.getID();
    mass = P_positron.getMass()*1000.0;
    charge = P_positron.getCharge();
    gfactor = g_positron;

    decaymode = 0;   
    livetime = -fVmomentum[0]*tau_positron/mass*log(ran1(&rannum));
  }
  else if (id == P_proton.getID()){
    ID = P_proton.getID();
    mass = P_proton.getMass()*1000.0;
    charge = P_proton.getCharge();
    gfactor = g_proton;

    decaymode = 0;   
    livetime = -fVmomentum[0]*tau_proton/mass*log(ran1(&rannum));
  }
  else if (id == P_piplus.getID()){
    ID = P_piplus.getID();
    mass = P_piplus.getMass()*1000.0;
    charge = P_piplus.getCharge();
    gfactor = g_pion;
    
    // Pion will decay with 99.98% certainty into muon and neutrino. 
    // Therefore we take into account only this decay mode.
    decaymode = 1;
    // Now we need to calculate livetime of the particle:
    livetime = -fVmomentum[0]*tau_pion/mass*log(ran1(&rannum));
  }
  else if (id == P_piminus.getID()){
    ID = P_piminus.getID();
    mass = P_piminus.getMass()*1000.0;
    charge = P_piminus.getCharge();
    gfactor = g_pion;
    
    // Pion will decay with 99.98% certainty into muon and neutrino. 
    // Therefore we take into account only this decay mode.
    decaymode = 1;
    // Now we need to calculate livetime of the particle:
    livetime = -fVmomentum[0]*tau_pion/mass*log(ran1(&rannum));
  } 
  else if (id == P_muplus.getID()){
    ID = P_muplus.getID();
    mass = P_muplus.getMass()*1000.0;
    charge = P_muplus.getCharge();
    gfactor = g_muon;
    
    // In our conditions, the muon will not decay. But we can still 
    // calculate the living time of the particle 
    decaymode = 1;  // muon has only one relevant decay mode.  
    livetime = -fVmomentum[0]*tau_mu/mass*log(ran1(&rannum));
  }
  else if (id == P_kplus.getID()){
    ID = P_kplus.getID();
    mass = P_kplus.getMass()*1000.0;
    charge = P_kplus.getCharge();
    gfactor = g_kaon;
    
    // With Kaon we will take into account the most probable decays.
    // First we must decide in which decay mode the Kaon will decay:
    double ran_decay = ran1(&rannum);
    if (ran_decay<=0.6344){
      decaymode = 1;  // in this case Kaon will decay into mu and neutrino
      livetime = -fVmomentum[0]*tau_kaon1/mass*log(ran1(&rannum));
    } 
    else if(ran_decay > 0.6344 && ran_decay <= 0.8436){
      decaymode = 2; // K+ decays into Pi0 and Pi+
      livetime = -fVmomentum[0]*tau_kaon2/mass*log(ran1(&rannum));
    } 
    else if(ran_decay > 0.8436 && ran_decay <= 0.8934){
      decaymode = 3; // K+ decays into Pi0, e+, nu_e 
      livetime = -fVmomentum[0]*tau_kaon3/mass*log(ran1(&rannum));
    } 
    else if(ran_decay > 0.8934 && ran_decay <= 0.9266){
      decaymode = 4; // K+ decays into Pi0, mu+, nu_mu
      livetime = -fVmomentum[0]*tau_kaon4/mass*log(ran1(&rannum));
    } 
    else if(ran_decay > 0.9266 && ran_decay <= 0.9441){
      decaymode = 5; // K+ decays into Pi0, Pi0 and Pi+
      livetime = -fVmomentum[0]*tau_kaon5/mass*log(ran1(&rannum));
    } 
    else if(ran_decay > 0.9441 && ran_decay <= 1.0){
      decaymode = 6; // K+ decays into Pi+, Pi- and Pi+
      livetime = -fVmomentum[0]*tau_kaon6/mass*log(ran1(&rannum));
    }
    else{
      decaymode = -1; // in rest of the cases we must end calculation.
      
    }
  }
  else{ // in other cases we take electron
    ID = P_electron.getID(); 
    mass = P_electron.getMass()*1000.0;
    charge = P_electron.getCharge();
    gfactor = g_electron;
    
    decaymode = 0;
    livetime = 10000000.0; // very very long
    
    
  }

  #ifdef DEBUG
    // In the end we print out the properties of the new particle
    cout<<"--------------------------- New Particle ---------------------"<<endl;
    cout<<"ID delca je: "<< ID<<endl;
    cout<<"Status delca: "<<status<<endl;
    cout<<"Masa delca je: "<< mass<<endl;
    cout<<"Naboj delca je: "<<charge<<endl;
    cout<<"Decay mode: "<< decaymode<<endl;
    cout<<"SimParticle bo v danem primeru zivel: "<< livetime<<" ns"<<endl;
    cout<<"Vektor lege je: "<< position <<" mm"<<endl;
    cout<<"Velikost gibalne kolicine je: "<<fVmomentum.momentum()<<" MeV"<<endl;
    cout<<"Vrednost energije je: "<<fVmomentum.energy()<<" MeV"<<endl;
    cout<<"Vektor spina: "<<spin<<endl;
    cout<<"--------------------------------------------------------------"<<endl;
  #endif
 
  
}
