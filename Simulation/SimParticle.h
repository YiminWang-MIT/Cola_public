//
// $Id: SimParticle.h 2348 2009-10-09 10:07:29Z merkel $
//

#ifndef __SIMPARTICLE_H__
#define __SIMPARTICLE_H__

#include "../FourVector/FourVector.h"
#include "Matrix3D.h"

// Here should be all livetimes of the particles, given in nanoseconds

class SimParticle{
 public:
  int ID;
  int decaymode;
  double mass;
  double charge;
  double gfactor;
  double livetime;
  int status;
  Vector3D position;
  FourVector fVmomentum;
  Vector3D spin;
  
  // Default construktor
  SimParticle();
  SimParticle(int id, Vector3D lega, FourVector mom, Vector3D vSpin);
  
  //We define operator = for particles
  inline SimParticle& operator = (const SimParticle &simP){
    ID = simP.ID;
    decaymode = simP.decaymode;
    mass = simP.mass;
    charge = simP.charge;
    gfactor = simP.gfactor;
    livetime = simP.livetime;
    status  = simP.status;
    position = simP.position;
    fVmomentum = simP.fVmomentum;
    spin = simP.spin;
    return *this;
  }
};

class SimParticleGroup{
  private:
    unsigned int number;
  public:
    SimParticle simParticle[10];
     
    SimParticleGroup(){
      number = 0;
      
    }
    inline int add(SimParticle simp){
      simParticle[number] = simp;
      number++; // number of elements has increased for one
      return number;
    }
    inline int getNumber(){
      return number;
    }
    
};
 
 
//---------------------------------------------------------------------
//                         lifetimes of particles
//---------------------------------------------------------------------
 
 //leptons
const double tau_electron = 1.0E308; // [ns]
const double tau_positron = 1.0E308; // [ns]
const double tau_mu = 2197.03; // [ns]

//Mesons
const double tau_pion = 26.033;  //[ns]
const double tau_pi0 = 8.4E-8; // [ns]

// decay-time of the Kaon, which decays in first decay mode.
const double tau_kaon1 = 19.52238; // [ns]; 
// decay-time of the Kaon, which decays in second decay mode.
const double tau_kaon2 = 59.20172; // [ns]; 
// decay-time of the Kaon, which decays in third decay mode.
const double tau_kaon3 = 248.694; // [ns];
// decay-time of the Kaon, which decays in fourth decay mode.
const double tau_kaon4 = 373.04216; // [ns];
// decay-time of the Kaon, which decays in fifth decay mode.
const double tau_kaon5 = 704.8947; // [ns];   
// decay-time of the Kaon, which decays in sixth decay mode.
const double tau_kaon6 = 221.55635; // [ns];   

// Hadrons
const double tau_proton = 1.0E308; // [ns]



//---------------------------------------------------------------------
//                         g-factors of particles
//---------------------------------------------------------------------
const double g_electron = 2.0; // g-faktor muona
const double g_positron = 2.0; // g-faktor muona

const double g_muon = 0.0; // g-faktor muona

const double g_pion = 0.0; // g-faktor piona
const double g_kaon = 0.0; // g-faktor kaona


const double g_proton = 5.58569478; // g-faktor piona

#endif
