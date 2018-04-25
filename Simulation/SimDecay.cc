//
// $Id: SimDecay.cc 2640 2015-04-01 14:03:00Z aqua $
//

using namespace std;

#include "Cola/Masses.h"
#include "SimDecay.h"
#include "SimParticle.h"
#include "ran1.h"

long int seme = -1; // This variable is the seed for ran1 method

// This method handles the kinematics of three particle decay
int ThreeParticleDecay(FourVector fvM, double mass_M, double mass_p1, double mass_p2, double mass_p3, FourVector *fvP1, FourVector *fvP2, FourVector *fvP3){ 
      
  FourVector fvM_ACS = fvM;  // Momentum of initial particle of mass M in LAB cs.
  
  #ifdef DEBUG    
    cout<<"fvM ACS: [ "<<fvM_ACS[0]<<", "<<fvM_ACS[1]<<", "<<fvM_ACS[2]<<", "<<fvM_ACS[3]<<" ]"<<endl;
  #endif    
  // Now we use Lorentz transformation to transform momentum into CMS. 
  //FourVector fvM_CMS = Lorentz(fvM_ACS,-fvM_ACS);  // Momentum of muon in  CMS. 
  
  #ifdef DEBUG    
    cout<<"fvM CMS: [ "<<fvM_CMS[0]<<", "<<fvM_CMS[1]<<", "<<fvM_CMS[2]<<", "<<fvM_CMS[3]<<" ]"<<endl;
  #endif    
  // Now we raffle the mass m12 of the virtual joined particle 12 
  double mass_p12 = (mass_p1 + mass_p2) + ran1(&seme)*(mass_M - mass_p3 - mass_p1 - mass_p2);
  
  #ifdef DEBUG
    cout<<"Raffled Mass  of 'virtual particle' p12 is: "<<mass_p12<<endl;
  #endif    

  // Now we can calculate the energy and momentum od particle 3 (nu_mu) 
  double E3 = (mass_M*mass_M - mass_p12*mass_p12 + mass_p3*mass_p3)/(2.0*mass_M);
  double p3 = sqrt((mass_M*mass_M - (mass_p12 + mass_p3)*(mass_p12 + mass_p3))*(mass_M*mass_M - (mass_p12 - mass_p3)*(mass_p12 - mass_p3)))/(2.0*mass_M);
  
  #ifdef DEBUG    
    cout<<"E3: "<<E3<<", P3: "<<p3<<endl;
  #endif
    
  // Now we raffle angles th and ph of particle 3.
  double rphi =  2.0*M_PI*ran1(&seme);  
  double rtheta = acos((-1.0 + 2.0*ran1(&seme)));

  //  We create momentum 4-vector for mu-neutrino
  Vector3D vP3(cos(rphi)*sin(rtheta),sin(rphi)*sin(rtheta),cos(rtheta)); 

  vP3 = p3*vP3;
  FourVector fvP3_CMS(E3, vP3[1], vP3[2], vP3[3]);

  #ifdef DEBUG    
    cout<<"fvP3_CMS: [ "<<fvP3_CMS[0]<<", "<<fvP3_CMS[1]<<", "<<fvP3_CMS[2]<<", "<<fvP3_CMS[3]<<" ]"<<endl;
  #endif    
  
  // We create momentum 4-vector for particle 12
  FourVector fVP12_CMS(mass_M-E3, -vP3[1], -vP3[2], -vP3[3]);
  
  #ifdef DEBUG    
    cout<<"fVP12_CMS: [ "<<fVP12_CMS[0]<<", "<<fVP12_CMS[1]<<", "<<fVP12_CMS[2]<<", "<<fVP12_CMS[3]<<" ]"<<endl;
  #endif    
 
  //Now we transform 4-vector of particle 12 into it's own CS (CMS2)
  //FourVector fVP12_CMS2 = Lorentz(fVP12_CMS,-fVP12_CMS);
  
  #ifdef DEBUG    
    cout<<"fVP12_CMS2: [ "<<fVP12_CMS2[0]<<", "<<fVP12_CMS2[1]<<", "<<fVP12_CMS2[2]<<", "<<fVP12_CMS2[3]<<" ]"<<endl;
  #endif    
  
  // In this CS we now calculate the decay of particle 12 into particles 1 and 2
  // First we calculate momenta of both particles in CMS2
  double E1b = (mass_p12*mass_p12  - mass_p2*mass_p2 + mass_p1*mass_p1)/(2.0*mass_p12);
  double E2b = mass_p12 - E1b;
  double p1b = sqrt((mass_p12*mass_p12 - (mass_p1 + mass_p2)*(mass_p1 + mass_p2))*(mass_p12*mass_p12 - (mass_p1 - mass_p2)*(mass_p1 - mass_p2)))/(2.0*mass_p12);
      
  // Now we raffle angles th and ph of particle 1.
  rphi =  2.0*M_PI*ran1(&seme);  
  rtheta = acos((-1.0 + 2.0*ran1(&seme)));

  // We create the momentum fourvector for mu-neutrino (particle No. 1)
  Vector3D vP1b(cos(rphi)*sin(rtheta), sin(rphi)*sin(rtheta),cos(rtheta));
  vP1b = p1b*vP1b;
  FourVector fvP1_CMS2(E1b, vP1b[1], vP1b[2], vP1b[3]);
      
  #ifdef DEBUG
    cout<<"fvP1_CMS2: [ "<<fvP1_CMS2[0]<<", "<<fvP1_CMS2[1]<<", "<<fvP1_CMS2[2]<<", "<<fvP1_CMS2[3]<<" ]"<<endl;
  #endif    
  
  // Now we need the fourvector for e-neutrino (Particle No. 2)
  FourVector fvP2_CMS2(E2b, -vP1b[1], -vP1b[2], -vP1b[3]);
  
  #ifdef DEBUG    
    cout<<"fvP2_CMS2: [ "<<fvP2_CMS2[0]<<", "<<fvP2_CMS2[1]<<", "<<fvP2_CMS2[2]<<", "<<fvP2_CMS2[3]<<" ]"<<endl;
  #endif    
  
  // Once we have momenta of both particles, we can transform them from CMS2 back to CMS
  FourVector fvP1_CMS = Lorentz(fvP1_CMS2, fVP12_CMS);
  FourVector fvP2_CMS = Lorentz(fvP2_CMS2, fVP12_CMS);
  
  #ifdef DEBUG    
    cout<<"fvP1_CMS: [ "<<fvP1_CMS[0]<<", "<<fvP1_CMS[1]<<", "<<fvP1_CMS[2]<<", "<<fvP1_CMS[3]<<" ]"<<endl;
    cout<<"fvP2_CMS: [ "<<fvP2_CMS[0]<<", "<<fvP2_CMS[1]<<", "<<fvP2_CMS[2]<<", "<<fvP2_CMS[3]<<" ]"<<endl;
  #endif
      
  // In the end we thansform momenta of all three new particles from CMS back into LAB.
  FourVector fvP3_ACS = Lorentz(fvP3_CMS, fvM_ACS);
  FourVector fvP1_ACS = Lorentz(fvP1_CMS, fvM_ACS);
  FourVector fvP2_ACS = Lorentz(fvP2_CMS, fvM_ACS);
  
  #ifdef DEBUG        
    cout<<"fvP3_ACS: [ "<<fvP3_ACS[0]<<", "<<fvP3_ACS[1]<<", "<<fvP3_ACS[2]<<", "<<fvP3_ACS[3]<<" ]"<<endl;  
    cout<<"fvP1_ACS: [ "<<fvP1_ACS[0]<<", "<<fvP1_ACS[1]<<", "<<fvP1_ACS[2]<<", "<<fvP1_ACS[3]<<" ]"<<endl; 
    cout<<"fvP2_ACS: [ "<<fvP2_ACS[0]<<", "<<fvP2_ACS[1]<<", "<<fvP2_ACS[2]<<", "<<fvP2_ACS[3]<<" ]"<<endl; 
  #endif
 
  *fvP1 = fvP1_ACS;
  *fvP2 = fvP2_ACS;
  *fvP3 = fvP3_ACS;

  return 1;
}


// This method handles the kinematics of two-particle decay
int twoParticleDecay(FourVector fvM, double mass_M, double mass_p1, double mass_p2, FourVector *fvP1, FourVector *fvP2){
  
  FourVector fvM_ACS = fvM;
  
  #ifdef DEBUG    
    cout<<"fvM_ACS: [ "<<fvM_ACS[0]<<", "<<fvM_ACS[1]<<", "<<fvM_ACS[2]<<", "<<fvM_ACS[3]<<" ]"<<endl;
  #endif

  // Now we use Lorentz transformation to transform momentum into CMS. 
  //FourVector fvM_CMS = Lorentz(fvM_ACS,-fvM_ACS);
  
  #ifdef DEBUG    
  cout<<"fvM_CMS: [ "<<fvM_CMS[0]<<", "<<fvM_CMS[1]<<", "<<fvM_CMS[2]<<", "<<fvM_CMS[3]<<" ]"<<endl;
  #endif

  // The particle M decays into P1 and P2
  // Now we calculate energies of new particles 
  double E_P1 = (mass_M*mass_M - mass_p2*mass_p2 + mass_p1*mass_p1)/(2.0*mass_M);
  double E_P2 = mass_M - E_P1;
  
  // Now we calculate the absolute value of three-momentum
  double p1 = sqrt((mass_M*mass_M - (mass_p1 + mass_p2)*(mass_p1 + mass_p2))*(mass_M*mass_M - (mass_p1 - mass_p2)*(mass_p1 - mass_p2)))/(2.0*mass_M);
  
  // Now we raffle spherical agles, which determine directions of we particles
  double rphi =  2.0*M_PI*ran1(&seme);  
  double rtheta = acos((-1.0 + 2.0*ran1(&seme)));

  // Once we have angles, we can calculate the momentum fourvector of Neutrino
  Vector3D vP1(cos(rphi)*sin(rtheta), sin(rphi)*sin(rtheta), cos(rtheta)); 

  vP1 = p1*vP1;
  FourVector fvP1_CMS(E_P1, vP1[1], vP1[2], vP1[3]);

  // Now we make momentum fourvector of Muon in CMS of PION
  FourVector fvP2_CMS(E_P2,-vP1[1],-vP1[2],-vP1[3]);

  // Now we transform momentum fourvectors of both particles back to ACS 
  // coordinate system.
  FourVector fvP1_ACS= Lorentz(fvP1_CMS,fvM_ACS);
  FourVector fvP2_ACS= Lorentz(fvP2_CMS,fvM_ACS);
  
  #ifdef DEBUG    
    cout<<"fvP1_ACS: [ "<<fvP1_ACS[0]<<", "<<fvP1_ACS[1]<<", "<<fvP1_ACS[2]<<", "<<fvP1_ACS[3]<<" ]"<<endl;
    cout<<"fvP2_ACS: [ "<<fvP2_ACS[0]<<", "<<fvP2_ACS[1]<<", "<<fvP2_ACS[2]<<", "<<fvP2_ACS[3]<<" ]"<<endl;
  #endif

  *fvP1 = fvP1_ACS;
  *fvP2 = fvP2_ACS;
  
  return 1;
}



//------- This method handles the decay of various particles-----------;
int simParticleDecay(SimParticle *simParticle1, SimParticleGroup *simParticleGroup1)
{
  if ((int) simParticle1->ID == P_piplus.getID()){  
    // This part of the method handles decay of the pion
    if(simParticle1->decaymode == 1){
      #ifdef DEBUG     	
        cout<<"=================== PION+ MODE 1 =================="<<endl;
      #endif
      // This particle handles decay of the pi+ into mu+ and neutrino 
      double mass_pion = m_pion *1000.0;
      double mass_muon = m_mu*1000.0;
      double mass_neutrino = 0.0;

      FourVector fvP_ACS = simParticle1->fVmomentum;
      FourVector fvP_muon_ACS;
      FourVector fvP_neutrino_ACS;
      
     
      // now we raffle which particle will have index 1 and which index 2. 
      if (ran1(&seme)<0.5){
        twoParticleDecay(fvP_ACS, mass_pion, mass_muon, mass_neutrino, &fvP_muon_ACS, &fvP_neutrino_ACS);
      }
      else{
        twoParticleDecay(fvP_ACS, mass_pion, mass_neutrino, mass_muon, &fvP_neutrino_ACS, &fvP_muon_ACS);
      }
   
      #ifdef DEBUG    
        cout<<"-->fvP_muon_ACS: [ "<<fvP_muon_ACS[0]<<", "<<fvP_muon_ACS[1]<<", "<<fvP_muon_ACS[2]<<", "<<fvP_muon_ACS[3]<<" ]"<<endl;
        cout<<"-->fvP_neutrino_ACS: [ "<<fvP_neutrino_ACS[0]<<", "<<fvP_neutrino_ACS[1]<<", "<<fvP_neutrino_ACS[2]<<", "<<fvP_neutrino_ACS[3]<<" ]"<<endl;
      #endif

      // In the end we change properties of the particles. Now we have Muon.
      Vector3D position = simParticle1->position;
      Vector3D spin = simParticle1->spin;  // Nicht richtig !!!
      SimParticle simP(P_muplus.getID(),position,fvP_muon_ACS, spin);
      
      simParticle1-> ID = simP.ID;
      simParticle1-> decaymode = simP.decaymode;
      simParticle1-> mass = simP.mass;
      simParticle1-> charge = simP.charge;
      simParticle1-> gfactor = simP.gfactor;
      simParticle1-> livetime = simP.livetime;
      simParticle1-> position = simP.position;
      simParticle1-> fVmomentum = simP.fVmomentum;
      simParticle1-> spin = simP.spin;
      simParticle1-> status = simP.status;
      
      #ifdef DEBUG     
        cout<<"=================== END PION+ MODE 1 =================="<<endl;
      #endif

      return 1;
    }
  }
  
  else if ((int)simParticle1->ID == P_piminus.getID()){  
    // This part of the method handles decay of the pion
    if(simParticle1->decaymode == 1){
      #ifdef DEBUG    
        cout<<"=================== PION- MODE 1 =================="<<endl;
      #endif  
      // This particle handles decay of the pi- into mu- and neutrino 
      double mass_pion = m_pion *1000.0;
      double mass_muon = m_mu*1000.0;
      double mass_neutrino = 0.0;

      FourVector fvP_ACS = simParticle1->fVmomentum;
      FourVector fvP_muon_ACS;
      FourVector fvP_neutrino_ACS;
      

       // now we raffle which particle will have index 1 and which index 2.
      if (ran1(&seme)<0.5){
        twoParticleDecay(fvP_ACS, mass_pion, mass_muon, mass_neutrino, &fvP_muon_ACS, &fvP_neutrino_ACS);
      }
      else{
        twoParticleDecay(fvP_ACS, mass_pion, mass_neutrino, mass_muon, &fvP_neutrino_ACS, &fvP_muon_ACS);
      }
      
      
      #ifdef DEBUG    
        cout<<"-->fvP_muon_ACS: [ "<<fvP_muon_ACS[0]<<", "<<fvP_muon_ACS[1]<<", "<<fvP_muon_ACS[2]<<", "<<fvP_muon_ACS[3]<<" ]"<<endl;
        cout<<"-->fvP_neutrino_ACS: [ "<<fvP_neutrino_ACS[0]<<", "<<fvP_neutrino_ACS[1]<<", "<<fvP_neutrino_ACS[2]<<", "<<fvP_neutrino_ACS[3]<<" ]"<<endl;
      #endif
 
      // In the end we change properties of the particles. Now we have Muon.
      Vector3D position = simParticle1->position;
      Vector3D spin = simParticle1->spin;  // Nicht richtig !!!
      SimParticle simP(P_muminus.getID(),position,fvP_muon_ACS, spin);
      
      simParticle1-> ID = simP.ID;
      simParticle1-> decaymode = simP.decaymode;
      simParticle1-> mass = simP.mass;
      simParticle1-> charge = simP.charge;
      simParticle1-> gfactor = simP.gfactor;
      simParticle1-> livetime = simP.livetime;
      simParticle1-> position = simP.position;
      simParticle1-> fVmomentum = simP.fVmomentum;
      simParticle1-> spin = simP.spin;
      simParticle1-> status = simP.status;

      #ifdef DEBUG          
        cout<<"=================== END PION- MODE 1 =================="<<endl;
      #endif
      return 1;
    }
  }
  else if((int)simParticle1->ID == P_kplus.getID()){
    // This part handles the decay of kaon
    if(simParticle1->decaymode == 1){
      #ifdef DEBUG    
        cout<<"=================== KAON MODE 1 =================="<<endl;
      #endif
      // In this case K+ decays into  mu+ and neutrino
      
      // This particle handles decay of the K+ into mu+ and neutrino 
      double mass_kaon = m_kaon *1000.0;
      double mass_muon = m_mu*1000.0;
      double mass_neutrino = 0.0;

      FourVector fvP_ACS = simParticle1->fVmomentum;
      FourVector fvP_muon_ACS;
      FourVector fvP_neutrino_ACS;
      
      // now we raffle which particle will have index 1 and which index 2. 
      if (ran1(&seme)<0.5){
        twoParticleDecay(fvP_ACS, mass_kaon, mass_muon, mass_neutrino, &fvP_muon_ACS, &fvP_neutrino_ACS);
      }
      else{
        twoParticleDecay(fvP_ACS, mass_kaon, mass_neutrino, mass_muon, &fvP_neutrino_ACS, &fvP_muon_ACS);
      }
      
      // In the end we change properties of the particles. Now we have Muon.
      Vector3D position = simParticle1->position;
      Vector3D spin = simParticle1->spin;  // Nicht richtig !!!
      SimParticle simP(P_muplus.getID(),position,fvP_muon_ACS, spin);
      
      simParticle1-> ID = simP.ID;
      simParticle1-> decaymode = simP.decaymode;
      simParticle1-> mass = simP.mass;
      simParticle1-> charge = simP.charge;
      simParticle1-> gfactor = simP.gfactor;
      simParticle1-> livetime = simP.livetime;
      simParticle1-> position = simP.position;
      simParticle1-> fVmomentum = simP.fVmomentum;
      simParticle1-> spin = simP.spin;
      simParticle1-> status = simP.status;
      
      #ifdef DEBUG    
        cout<<"=================== END KAON MODE 1 =================="<<endl;
      #endif

      return 1;
    }
    else if(simParticle1->decaymode == 2){
      #ifdef DEBUG    
        cout<<"=================== KAON MODE 2 =================="<<endl;
      #endif
      // In this case K+ decays into Pi+ and Pi0. Then Pi0 in the moment
      // decays into two photons. Therefore we only need to folow Pi+ particle.
      
      // This particle handles decay of the K+ into pi+ and pi0 
      double mass_kaon = m_kaon *1000.0;
      double mass_pi = m_pion*1000.0;
      double mass_pi0 = m_pi0*1000.0;
      

      FourVector fvP_ACS = simParticle1->fVmomentum;
      FourVector fvP_pi_ACS;
      FourVector fvP_pi0_ACS;
      
 
       // now we raffle which particle will have index 1 and which index 2. 	
      if (ran1(&seme)<0.5){
        twoParticleDecay(fvP_ACS, mass_kaon, mass_pi, mass_pi0, &fvP_pi_ACS, &fvP_pi0_ACS);
      }
      else{
        twoParticleDecay(fvP_ACS, mass_kaon, mass_pi0, mass_pi, &fvP_pi0_ACS, &fvP_pi_ACS);
      }
      
      Vector3D position = simParticle1->position;
      Vector3D spin = simParticle1->spin;  // Nicht richtig !!!
      SimParticle simP(P_piplus.getID(),position,fvP_pi_ACS, spin ); 
      
      simParticle1-> ID = simP.ID;
      simParticle1-> decaymode = simP.decaymode;
      simParticle1-> mass = simP.mass;
      simParticle1-> charge = simP.charge;
      simParticle1-> gfactor = simP.gfactor;
      simParticle1-> livetime = simP.livetime;
      simParticle1-> position = simP.position;
      simParticle1-> fVmomentum = simP.fVmomentum;
      simParticle1-> spin = simP.spin;
      simParticle1-> status = simP.status;
      
      #ifdef DEBUG    
        cout<<"=================== END KAON MODE 2 =================="<<endl;
      #endif

      return 1;
    }
    else if(simParticle1->decaymode == 3){
      // In this decay mode, K+ decays into Pi0, e+ and nu_e
      #ifdef DEBUG    	
        cout<<"=================== KAON MODE 3 =================="<<endl;
      #endif

      // First we calculate masses of all particles.
      double mass_M = m_kaon *1000.0; // Mass of the particle that decays - That's K+
      double mass_p3 = m_pi0*1000.0; // Mass of Pi0
      double mass_p2 = 0.0; // Mass of nu_e
      double mass_p1 = m_electron *1000.0; // Mass of e+
      
      FourVector fvKaon_ACS = simParticle1->fVmomentum;
      FourVector fvP_pi0_ACS;
      FourVector fvP_neutrino_ACS;
      FourVector fvP_electron_ACS;
      

       // now we raffle which particle will have index 1, which index 2 and whick index 3.
      if (ran1(&seme)<=1.0/6.0){
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p1, mass_p2, mass_p3, &fvP_electron_ACS, &fvP_neutrino_ACS, &fvP_pi0_ACS);
      }
      else if (ran1(&seme)<=2.0/6.0){
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p1, mass_p3, mass_p2, &fvP_electron_ACS, &fvP_pi0_ACS, &fvP_neutrino_ACS);
      }
      else if (ran1(&seme)<=3.0/6.0){
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p3, mass_p1, mass_p2, &fvP_pi0_ACS, &fvP_electron_ACS, &fvP_neutrino_ACS);
      }
      else if (ran1(&seme)<=4.0/6.0){
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p3, mass_p2, mass_p1, &fvP_pi0_ACS, &fvP_neutrino_ACS, &fvP_electron_ACS);
      }
      else if (ran1(&seme)<=5.0/6.0){
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p2, mass_p1, mass_p3, &fvP_neutrino_ACS, &fvP_electron_ACS, &fvP_pi0_ACS);
      }
      else{
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p2, mass_p3, mass_p1, &fvP_neutrino_ACS, &fvP_pi0_ACS, &fvP_electron_ACS);
      }
      
      Vector3D position = simParticle1->position;
      Vector3D spin = simParticle1->spin;  // Nicht richtig !!!
      SimParticle simP(P_positron.getID(),position,fvP_electron_ACS, spin ); 
      
      simParticle1-> ID = simP.ID;
      simParticle1-> decaymode = simP.decaymode;
      simParticle1-> mass = simP.mass;
      simParticle1-> charge = simP.charge;
      simParticle1-> gfactor = simP.gfactor;
      simParticle1-> livetime = simP.livetime;
      simParticle1-> position = simP.position;
      simParticle1-> fVmomentum = simP.fVmomentum;
      simParticle1-> spin = simP.spin;
      simParticle1-> status = simP.status;

      #ifdef DEBUG          
        cout<<"=================== END KAON MODE 3 =================="<<endl;
      #endif

      return 1;
    }
    else if(simParticle1->decaymode == 4){
      // In this decay mode, K+ decays into Pi0, mu+ and nu_mu
      #ifdef DEBUG    
        cout<<"=================== KAON MODE 4 =================="<<endl;
      #endif 
      // First we calculate masses of all particles
      double mass_M = m_kaon *1000.0; // Mass of the particle that decays
      double mass_p3 = m_pi0*1000.0; // Mass of  Pi0
      double mass_p2 = 0.0; // Mass of nu_mu
      double mass_p1 = m_mu *1000.0; // Mass of muon
      
      FourVector fvKaon_ACS = simParticle1->fVmomentum;
      FourVector fvP_pi0_ACS;
      FourVector fvP_neutrino_ACS;
      FourVector fvP_mu_ACS;
       
       // now we raffle which particle will have index 1, which index 2 and whick index 3.
      if (ran1(&seme)<=1.0/6.0){
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p1, mass_p2, mass_p3, &fvP_mu_ACS, &fvP_neutrino_ACS, &fvP_pi0_ACS);
      }
      else if (ran1(&seme)<=2.0/6.0){
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p1, mass_p3, mass_p2, &fvP_mu_ACS, &fvP_pi0_ACS, &fvP_neutrino_ACS);
      }
      else if (ran1(&seme)<=3.0/6.0){
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p3, mass_p1, mass_p2, &fvP_pi0_ACS, &fvP_mu_ACS, &fvP_neutrino_ACS);
      }
      else if (ran1(&seme)<=4.0/6.0){
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p3, mass_p2, mass_p1, &fvP_pi0_ACS, &fvP_neutrino_ACS, &fvP_mu_ACS);
      }
      else if (ran1(&seme)<=5.0/6.0){
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p2, mass_p1, mass_p3, &fvP_neutrino_ACS, &fvP_mu_ACS, &fvP_pi0_ACS);
      }
      else{
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p2, mass_p3, mass_p1, &fvP_neutrino_ACS, &fvP_pi0_ACS, &fvP_mu_ACS);
      }

      
      Vector3D position = simParticle1->position;
      Vector3D spin = simParticle1->spin;  // Nicht richtig !!!
      SimParticle simP(P_muplus.getID(),position,fvP_mu_ACS, spin ); 
      
      simParticle1-> ID = simP.ID;
      simParticle1-> decaymode = simP.decaymode;
      simParticle1-> mass = simP.mass;
      simParticle1-> charge = simP.charge;
      simParticle1-> gfactor = simP.gfactor;
      simParticle1-> livetime = simP.livetime;
      simParticle1-> position = simP.position;
      simParticle1-> fVmomentum = simP.fVmomentum;
      simParticle1-> spin = simP.spin;
      simParticle1-> status = simP.status;
      
      #ifdef DEBUG    
        cout<<"=================== END KAON MODE 4 =================="<<endl;
      #endif

      return 1;
    }
    else if(simParticle1->decaymode == 5){
      // In this decay mode, K+ decays into Pi0 Pi0 and Pi+
      #ifdef DEBUG    	
        cout<<"=================== KAON MODE 5 =================="<<endl;
      #endif

      double mass_M = m_kaon *1000.0; 
      double mass_p3 = m_pi0*1000.0; 
      double mass_p2 = m_pi0*1000.0;
      double mass_p1 = m_pion *1000.0;
      
      FourVector fvKaon_ACS = simParticle1->fVmomentum; 
      FourVector fvP_pi0a_ACS;
      FourVector fvP_pi0b_ACS;
      FourVector fvP_pion_ACS;
       
       // now we raffle which particle will have index 1, which index 2 and whick index 3.
      if (ran1(&seme)<=1.0/6.0){
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p1, mass_p2, mass_p3, &fvP_pion_ACS, &fvP_pi0a_ACS, &fvP_pi0b_ACS);
      }
      else if (ran1(&seme)<=2.0/6.0){
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p1, mass_p3, mass_p2, &fvP_pion_ACS, &fvP_pi0b_ACS, &fvP_pi0a_ACS);
      }
      else if (ran1(&seme)<=3.0/6.0){
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p3, mass_p1, mass_p2, &fvP_pi0b_ACS, &fvP_pion_ACS, &fvP_pi0a_ACS);
      }
      else if (ran1(&seme)<=4.0/6.0){
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p3, mass_p2, mass_p1, &fvP_pi0b_ACS, &fvP_pi0a_ACS, &fvP_pion_ACS);
      }
      else if (ran1(&seme)<=5.0/6.0){
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p2, mass_p1, mass_p3, &fvP_pi0a_ACS, &fvP_pion_ACS, &fvP_pi0b_ACS);
      }
      else{
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p2, mass_p3, mass_p1, &fvP_pi0a_ACS, &fvP_pi0b_ACS, &fvP_pion_ACS);
      }

      Vector3D position = simParticle1->position;
      Vector3D spin = simParticle1->spin;  // Nicht richtig !!!
      SimParticle simP(P_piplus.getID(),position,fvP_pion_ACS, spin ); 
      
      simParticle1-> ID = simP.ID;
      simParticle1-> decaymode = simP.decaymode;
      simParticle1-> mass = simP.mass;
      simParticle1-> charge = simP.charge;
      simParticle1-> gfactor = simP.gfactor;
      simParticle1-> livetime = simP.livetime;
      simParticle1-> position = simP.position;
      simParticle1-> fVmomentum = simP.fVmomentum;
      simParticle1-> spin = simP.spin;
      simParticle1-> status = simP.status;
      
      #ifdef DEBUG    
        cout<<"=================== END KAON MODE 5 =================="<<endl;
      #endif

      return 1;
    }
    else if(simParticle1->decaymode == 6){
      // In this decay mode, K+ decays into Pi+ Pi- and Pi+
      #ifdef DEBUG      	
        cout<<"=================== KAON MODE 6 =================="<<endl;
      #endif

      double mass_M = m_kaon*1000.0; 
      double mass_p3 = m_pion*1000.0;
      double mass_p2 = m_pion*1000.0;
      double mass_p1 = m_pion*1000.0;
      
      FourVector fvKaon_ACS = simParticle1->fVmomentum;
      FourVector fvP_piplusA_ACS;
      FourVector fvP_piminus_ACS;
      FourVector fvP_piplusB_ACS;
       
       // now we raffle which particle will have index 1, which index 2 and whick index 3.
      if (ran1(&seme)<=1.0/6.0){
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p1, mass_p2, mass_p3, &fvP_piplusA_ACS, &fvP_piminus_ACS, &fvP_piplusB_ACS);
      }
      else if (ran1(&seme)<=2.0/6.0){
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p1, mass_p3, mass_p2, &fvP_piplusA_ACS, &fvP_piplusB_ACS, &fvP_piminus_ACS);
      }
      else if (ran1(&seme)<=3.0/6.0){
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p3, mass_p1, mass_p2, &fvP_piplusB_ACS, &fvP_piplusA_ACS, &fvP_piminus_ACS);
      }
      else if (ran1(&seme)<=4.0/6.0){
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p3, mass_p2, mass_p1, &fvP_piplusB_ACS, &fvP_piminus_ACS, &fvP_piplusA_ACS);
      }
      else if (ran1(&seme)<=5.0/6.0){
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p2, mass_p1, mass_p3, &fvP_piminus_ACS, &fvP_piplusA_ACS, &fvP_piplusB_ACS);
      }
      else{
        ThreeParticleDecay(fvKaon_ACS, mass_M, mass_p2, mass_p3, mass_p1, &fvP_piminus_ACS, &fvP_piplusB_ACS, &fvP_piplusA_ACS);
      }
      
 
      Vector3D position = simParticle1->position;
      Vector3D spin = simParticle1->spin;  // Nicht richtig !!!
      
      // Now we create two new particles that we want to follow
      SimParticle simP(P_piplus.getID(),position,fvP_piplusA_ACS, spin );
      SimParticle simP1(P_piminus.getID(),position,fvP_piminus_ACS, spin ); 
      SimParticle simP2(P_piplus.getID(),position,fvP_piplusB_ACS, spin );
      
      //First we uprade initial particle
      simParticle1-> ID = simP.ID;
      simParticle1-> decaymode = simP.decaymode;
      simParticle1-> mass = simP.mass;
      simParticle1-> charge = simP.charge;
      simParticle1-> gfactor = simP.gfactor;
      simParticle1-> livetime = simP.livetime;
      simParticle1-> position = simP.position;
      simParticle1-> fVmomentum = simP.fVmomentum;
      simParticle1-> spin = simP.spin;
      simParticle1-> status = simP.status;
      
      // Now we add another particle into the group
      simParticleGroup1->add(simP1);
      simParticleGroup1->add(simP2);
      
      #ifdef DEBUG    
        cout<<"Number of particles in the group: "<<simParticleGroup1->getNumber()<<endl;     
        cout<<"=================== END KAON MODE 6 =================="<<endl;
      #endif

      return 1;
    }
    else{
      return -1;
    }
  }
  else if((int)simParticle1->ID == P_muplus.getID()){
    // muon has only one relevant decay mode
    if(simParticle1->decaymode == 1){
      #ifdef DEBUG     	
        cout<<"===================Muon bo razpadel:=================="<<endl;
      #endif

      double mass_M = m_mu *1000.0; 
      double mass_p3 = 0.0; 
      double mass_p2 = 0.0; 
      double mass_p1 = m_electron *1000.0;
      
      FourVector fvMuon_ACS = simParticle1->fVmomentum;
      FourVector fvP_electron_ACS;
      FourVector fvP_neutrinoE_ACS;
      FourVector fvP_neutrinoM_ACS;

       // now we raffle which particle will have index 1, which index 2 and whick index 3.
      if (ran1(&seme)<=1.0/6.0){
        ThreeParticleDecay(fvMuon_ACS, mass_M, mass_p1, mass_p2, mass_p3, &fvP_electron_ACS, &fvP_neutrinoE_ACS, &fvP_neutrinoM_ACS);
      }
      else if (ran1(&seme)<=2.0/6.0){
        ThreeParticleDecay(fvMuon_ACS, mass_M, mass_p1, mass_p3, mass_p2, &fvP_electron_ACS, &fvP_neutrinoM_ACS, &fvP_neutrinoE_ACS);
      }
      else if (ran1(&seme)<=3.0/6.0){
        ThreeParticleDecay(fvMuon_ACS, mass_M, mass_p3, mass_p1, mass_p2, &fvP_neutrinoM_ACS, &fvP_electron_ACS, &fvP_neutrinoE_ACS);
      }
      else if (ran1(&seme)<=4.0/6.0){
        ThreeParticleDecay(fvMuon_ACS, mass_M, mass_p3, mass_p2, mass_p1, &fvP_neutrinoM_ACS, &fvP_neutrinoE_ACS, &fvP_electron_ACS);
      }
      else if (ran1(&seme)<=5.0/6.0){
        ThreeParticleDecay(fvMuon_ACS, mass_M, mass_p2, mass_p1, mass_p3, &fvP_neutrinoE_ACS, &fvP_electron_ACS, &fvP_neutrinoM_ACS);
      }
      else{
        ThreeParticleDecay(fvMuon_ACS, mass_M, mass_p2, mass_p3, mass_p1, &fvP_neutrinoE_ACS, &fvP_neutrinoM_ACS, &fvP_electron_ACS);
      }

      
      Vector3D position = simParticle1->position;
      Vector3D spin = simParticle1->spin;  // Nicht richtig !!!
      SimParticle simP(P_positron.getID(),position,fvP_electron_ACS, spin); 
      
      simParticle1-> ID = simP.ID;
      simParticle1-> decaymode = simP.decaymode;
      simParticle1-> mass = simP.mass;
      simParticle1-> charge = simP.charge;
      simParticle1-> gfactor = simP.gfactor;
      simParticle1-> livetime = simP.livetime;
      simParticle1-> position = simP.position;
      simParticle1-> fVmomentum = simP.fVmomentum;
      simParticle1-> spin = simP.spin;
      simParticle1-> status = simP.status;
      
      #ifdef DEBUG    
        cout<<"===================END MUON:=================="<<endl;
      #endif
      return 1;
      

      
    }
    else{
        return -1;
    }
    
  } 
  else{
    #ifdef DEBUG    
      std::cout<<"Decay for particle with ID: "<<simParticle1->ID<<" is not yet implemented!\a"<<std::endl;
    #endif
    return -1;
  }  
  return 0;
}
