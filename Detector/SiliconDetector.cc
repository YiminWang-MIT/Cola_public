//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 2004-2008
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: SiliconDetector.cc 2357 2009-11-29 10:20:26Z makek $
//

#include "Aqua/AquaTree.h"
#include "SiliconDetector.h"
#include "Cola/Targetlength.h"
#include <cmath>
#include <fstream>
#include <iostream>

SiliconDetector::SiliconDetector(AquaTree *aquatree, AquaTree *outtree, int use_fadc)
  : detectorBase(aquatree, outtree)
{
  AquaNode **usn = (AquaNode **)adc;
  atree->multifind(usn, 0, 47, ".si.adc[%d]");

  AquaNode **aaa = (AquaNode **)msx;
  atree->multifind(aaa, 0, 5, ".si.msx[%d]");
  
  AquaNode **ttt = (AquaNode **)tdc;
  atree->multifind(ttt, 6, 10, ".si.msx[%d]");
  
  AquaNode **msc = (AquaNode **)misc;
  atree->multifind(msc, 0, 0, ".si.misc[%d]");
  
  AquaNode **bb2_a = (AquaNode **)E_A;
  online->multifind(bb2_a, 0, 23, "online.si.E_A[%d]");

  AquaNode **bb2_b = (AquaNode **)E_B;
  online->multifind(bb2_b, 0, 23, "online.si.E_B[%d]");

  AquaNode **bbb = (AquaNode **)E_MSX;
  online->multifind(bbb, 0, 4, "online.si.E_MSX[%d]");

  AquaNode **rec = (AquaNode **)E_rec;
  online->multifind(rec, 0, 2, "online.si.E_rec[%d]");

  si_theta = (AquaNodeDouble *)online->find("online.si.si_theta");
  si_phi = (AquaNodeDouble *)online->find("online.si.si_phi");
  E_scint = (AquaNodeDouble *)online->find("online.si.E_scint");
  E_bb2a = (AquaNodeDouble *)online->find("online.si.E_bb2a");
  E_bb2b = (AquaNodeDouble *)online->find("online.si.E_bb2b");
  E_VETO = (AquaNodeDouble *)online->find("online.si.E_VETO");
  
  EnergyParticle = (AquaNodeDouble *)online->find("online.si.EnergyParticle");
  MomentumParticle = (AquaNodeDouble *)online->find("online.si.MomentumParticle");
  //ThetaParticle = (AquaNodeDouble *)online->find("online.si.ThetaParticle");
  //PhiParticle = (AquaNodeDouble *)online->find("online.si.PhiParticle");

  OK = (AquaNodeUShort *)online->find("online.si.OK");

  // --- Determine fadc type, if any ---
  setFADC(use_fadc);

  switch (USE_FADC){
      case 1:
      {	  AquaNode **sn = (AquaNode **)fadc_n1728;
	  atree->multifind(sn, 1, fadcChannels, ".si.fadc[%d].energy");
      }
      break;
      case 2:
      {   AquaNode **mmm = (AquaNode **)maximum;
	  aquatree->multifind(mmm, 0, AquaUnit08A8channels-1, ".si.V1724.chan[%d].maximum"); 
	  
	  AquaNode **eng = (AquaNode **)fadc_v1724;
	  online->multifind(eng, 0, fadcChannels-1, "online.si.v1724energy[%d]");
      }
      break;
      default:
	  break;
  }
  
  if (USE_FADC !=0){
      AquaNode **ff = (AquaNode **)E_fADC;
      online->multifind(ff, 0, fadcChannels-1, "online.si.E_fADC[%d]");

      EnergyP_fadc = (AquaNodeDouble *)online->find("online.si.EnergyP_fadc");
      MomentumP_fadc = (AquaNodeDouble *)online->find("online.si.MomentumP_fadc");
  }
  // -------------------------------------

  init();
}

SiliconDetector::~SiliconDetector()
{
  ;
}
double theta, phi;

/*** Detector layer ***/
Layer::Layer(Material *mat, double thickness, double steps){
    LayerSteps = steps;
    LayerThickness = thickness;
    material = mat;
}
double Layer::pathlength(double th, double ph){
    path = this->thickness()/cos(th)/cos(ph); 
    return path; //[cm]
}
double Layer::Eloss(Particle part){
    initial=part.energy();
    if (part.energy() - part.getMass() > 0.0){
	for (int i=0; i < this->steps(); i++){	
	    part += material->dEdx(part,this->pathlength(theta,phi)/this->steps());
	}
    }
    return initial - part.energy();
}
double Layer::Egain(Particle part){
  initial=part.energy();
  if (part.energy() - part.getMass() > 0.0){
      for (int i=0; i < this->steps(); i++){
	  part -= material->dEdx(part,this->pathlength(theta,phi)/this->steps());
      }
  }
  return part.energy()-initial;
}


int SiliconDetector::handle(){

  OK->handle(0);
  
  double EnergyBB2a = 0;
  double EnergyBB2b = 0;
  double si_theta_cnt = -4096.0;
  double si_phi_cnt = -4096.0;
  double E_Atmp[24];
  double E_Btmp[24];
  double E_rectmp[3];
  int max_a=0; int max_b=0; 
  double fadc_E_corr=0.0; 

  /*** BB2 - vertical strips ***/
  for (int i=0; i<24; i++){
      E_Atmp[i]=0;
      if (adc[i]->ok()){
	  E_Atmp[i]=((*adc[i])* a_A[i] + b_A[i]); 
	  if (E_Atmp[i] > si_theta_cnt) {
	      si_theta_cnt = E_Atmp[i];
	      max_a = i;
	  }
      }
      //else return 0; //exit if theta cannot be reconstructed
      E_A[i]->handle(E_Atmp[i]); //BB2A Energy corrected
  }
  //E_A[max_a]->handle(E_Atmp[max_a]); //BB2A Energy corrected
  theta = atan((max_a+0.5-12.0)/distance);
  si_theta->handle(theta); //Theta in Silicon coordinates
  
  /*** BB2 - horizontal strips ***/
  for (int j=0; j<24; j++){
      E_Btmp[j]=0.0;
      if (adc[j+24]->ok()){
	  E_Btmp[j]=((*adc[j+24])*a_B[j] + b_B[j]);
	  if (E_Btmp[j] > si_phi_cnt){
	      si_phi_cnt = E_Btmp[j];    
	      max_b = j;
	  }
      }
      //else return 0; //exit if phi cannot be reconstructed
      E_B[j]->handle(E_Btmp[j]); //BB2B Energy corrected
  }
  //E_B[max_b]->handle(E_Btmp[max_b]); //BB2B Energy corrected
  phi = atan((max_b+0.5-12.0)/distance);   
  si_phi->handle(phi); //Phi in Silicon coordinates
   
  //BB2a and BB2b energies
  EnergyBB2a=*E_A[max_a] * 0.001; //[GeV]
  EnergyBB2b=*E_B[max_b] * 0.001; //[GeV]

  E_bb2a->handle(EnergyBB2a);    
  E_bb2b->handle(EnergyBB2b);      
 
  /*** Handle Angles ***/
  //ThetaParticle->handle(theta);
  //PhiParticle->handle(phi); 

  /*** Scintillator energy ***/
  double EnergyScint=0.0;
  if (misc[0]->ok()){
      EnergyScint=(*misc[0])*a_scint + b_scint;
      EnergyScint*=0.001; //[GeV]
      E_scint->handle(EnergyScint);   
  }
  ////else return 0; //this should be left commented for backward compatibility

  /*** MSX energies ***/
  double EnergyMSX[5];
  for (int i=0; i<5; i++){
    EnergyMSX[i]=0; 
    if (msx[i]->ok()){
      EnergyMSX[i]=(*msx[i])*a_MSX[i] + b_MSX[i];
      EnergyMSX[i]*=0.001; //[GeV]
      E_MSX[i]->handle(EnergyMSX[i]);
    }
    //else return 0;
  }
  
  /*** VETO energy ***/
  double EnergyVETO=0;
  if (msx[5]->ok()){ //in the second test VETO was msx[6] otherwise it's msx[5]
    EnergyVETO=((*msx[5])*a_MSX[5] + b_MSX[5])*0.001;
    E_VETO->handle(EnergyVETO);
  }
  //else return 0;
 
  /*** Detector layers ***/
  //Layer *Str = new Layer(Fe,0.04,500);
  //Layer *Scint = new Layer(Scintillator, thick[7]<0.3 ? 0.306 : thick[7], 30);//Thickness is Scintillator (0.3cm) + Tape + Foil (0.006cm) ~ 0.306 cm
  Layer *Scint = new Layer(Scintillator,thick[7],30);//Thickness is Scintillator (0.3cm) + Tape + Foil (0.025cm) ~ 0.325 cm
  Layer *Absorber = new Layer(Al,thick[0],30);
  Layer *BB2 = new Layer(Si,thick[1],30);
  Layer *MSX1 = new Layer(Si,thick[2],100);
  Layer *MSX2 = new Layer(Si,thick[3],100);
  Layer *MSX3 = new Layer(Si,thick[4],100);
  Layer *MSX4 = new Layer(Si,thick[5],100);
  Layer *MSX5 = new Layer(Si,thick[6],100);
  
  /*** Proton - shapers ***/
  Particle P = P_proton;
  P.setMomentum(0,0,0);
  double ParticleMomentum = 0.0;
  double ParticleEnergy = 0.0;
 
  int ESHAPER = 1;
  
  switch(ESHAPER){
      case 0:
	  //Default/////////////////////////////////////////////////////////////////////////////////////////////////
	  ParticleEnergy = EnergyMSX[0] + EnergyMSX[1] + EnergyMSX[2] + EnergyMSX[3] + EnergyMSX[4] + P.getMass();
	  break;
	  //Default//////////////////////////////////////////////////////////////////////////////////////////////////
      case 1:
	  //Without 1st layer///////////////////////////////////////////////////////////////////////////////////////
	  ParticleEnergy = EnergyMSX[1] + EnergyMSX[2] + EnergyMSX[3] + EnergyMSX[4] + P.getMass();
	  break;
	  //Without 1st layer///////////////////////////////////////////////////////////////////////////////////////
      default:
	  break;
  }

  if (ParticleEnergy>P.getMass()){ 
    ParticleMomentum = sqrt(pow(ParticleEnergy,2) - pow(P.getMass(),2));
    P.setMomentum(ParticleMomentum,0,0);
    
    
    /*** Inverse Bethe-Bloch in MSX1 ***/
    P += MSX1->Egain(P);

    /*** Inverse Bethe-Bloch in BB2 ***/
    P += BB2->Egain(P);
    
    /*** Inverse Bethe-Bloch in Scintillator ***/
    if (thick[7]!=0) P += Scint->Egain(P);
    
    /*** Inverse Bethe-Bloch in Absorber ***/
    P += Absorber->Egain(P);
    
    /*** Handle Particle stuff ***/
    EnergyParticle->handle(P.energy());
    MomentumParticle->handle(P.momentum());
    //ThetaParticle->handle(theta);
    //PhiParticle->handle(phi); 
  }

  // --- in case we use fadc ---
  if (USE_FADC !=0){
      
      /*** fADC energies ***/
      double EnergyfADC[fadcChannels];
      switch (USE_FADC){
	  case 1:
	  {
	      for (int i=0; i<fadcChannels; i++){
		  EnergyfADC[i]=0; 
		  if (fadc_n1728[i]->ok()){
		      EnergyfADC[i]=-(*fadc_n1728[i]);//-sign for negative signals
		      EnergyfADC[i]*=a_fADC[i];
		      EnergyfADC[i]+=b_fADC[i]; 
		      EnergyfADC[i]*=0.001; //This puts Energy into GeV
		  }
		  //else return 0;
	      }
	  }
	  break;
	  case 2:
	  {
	      for (int i=0; i<fadcChannels; i++){
		  EnergyfADC[i]=0; 
		  if (fadc_v1724[i]->ok()){
		      //Energy from trapezoid
		      EnergyfADC[i]=-(*fadc_v1724[i]);//-sign for negative signals
		      EnergyfADC[i]*=a_fADC[i];
		      EnergyfADC[i]+=b_fADC[i]; 
		      EnergyfADC[i]*=0.001; //This puts Energy into GeV
		  }
		  //else return 0;
	      }
	  }
	  break;
	  default:
	      break;
      }
      //*** Handle fADC energies stuff ***//
      for (int j=0; j<fadcChannels; j++) E_fADC[j]->handle(EnergyfADC[j]);
  
      /*** Proton - fADC ***/
      Particle P_fadc = P_proton;
      P_fadc.setMomentum(0,0,0);
      double P_fadcMomentum = 0.0;
      double P_fadcEnergy = 0.0;
      
      switch(USE_FADC){
	  case 1:
	      P_fadcEnergy = EnergyfADC[0] + EnergyfADC[1] + EnergyfADC[2] + EnergyfADC[3];
	      break;
	  case 2:
	      P_fadcEnergy = EnergyfADC[0] + EnergyfADC[1] + EnergyfADC[2] + EnergyfADC[3] + EnergyfADC[4];
	      break;
      }
      //---------------------------------------------------------------
      // --- Only for offline analysis ---------------
      
      //#define BANANA_CUT_BB2_FADC
      #define FADC_ENERGY_CORRECTION
      #include "SiDetectorProton.cc" //Proton identification and energy correction
      
      int EMODE = 0;
      
      switch(EMODE){
	  case 0:
	      //Default/////////////////////////////////////////////////////////////////////////////////////////////////
	      if (STOP1==true) P_fadcEnergy = EnergyfADC[0]+EnergyfADC[1];
	      else if (STOP2==true) P_fadcEnergy = EnergyfADC[0]+EnergyfADC[1]+EnergyfADC[2];
	      else if (STOP3==true) P_fadcEnergy = EnergyfADC[0]+EnergyfADC[1]+EnergyfADC[2]+EnergyfADC[3];
	      else if (STOP4==true) P_fadcEnergy = EnergyfADC[0]+EnergyfADC[1]+EnergyfADC[2]+EnergyfADC[3]+EnergyfADC[4];
	      else if (STOP5==true) P_fadcEnergy = EnergyfADC[0]+EnergyfADC[1]+EnergyfADC[2]+EnergyfADC[3]+EnergyfADC[4]+EnergyfADC[5];
	      else if (STOP6==true) P_fadcEnergy = EnergyfADC[0]+EnergyfADC[1]+EnergyfADC[2]+EnergyfADC[3]+EnergyfADC[4]+EnergyfADC[5];
	      break;
	      //Default//////////////////////////////////////////////////////////////////////////////////////////////////
	  case 1:
	      //Without 1st layer///////////////////////////////////////////////////////////////////////////////////////
	      if (STOP1==true) return 0;
	      else if (EnergyfADC[1]<0.0025) return 0;
	      else if (STOP2==true) P_fadcEnergy = EnergyfADC[1]+EnergyfADC[2];
	      else if (STOP3==true) P_fadcEnergy = EnergyfADC[1]+EnergyfADC[2]+EnergyfADC[3];
	      else if (STOP4==true) P_fadcEnergy = EnergyfADC[1]+EnergyfADC[2]+EnergyfADC[3]+EnergyfADC[4];
	      else if (STOP5==true) P_fadcEnergy = EnergyfADC[1]+EnergyfADC[2]+EnergyfADC[3]+EnergyfADC[4];
	      break;
	      //Without 1st layer///////////////////////////////////////////////////////////////////////////////////////
	  case 2:
	      //Each layer separately//////////////////////////////////////////////////////////////////////////////////
	      if (STOP1==true) P_fadcEnergy = EnergyfADC[0];
	      else if (STOP2==true) P_fadcEnergy = EnergyfADC[0]+EnergyfADC[1];
	      else if (STOP3==true) P_fadcEnergy = EnergyfADC[1]+EnergyfADC[2];
	      else if (STOP4==true) P_fadcEnergy = EnergyfADC[2]+EnergyfADC[3];
	      else if (STOP5==true) P_fadcEnergy = EnergyfADC[3]+EnergyfADC[4];
	      //Each layer separately///////////////////////////////////////////////////////////////////////////////////
	  case 3:
	      //1st layer separately////////////////////////////////////////////////////////////////////////////////////
	      if (STOP1==true) P_fadcEnergy = EnergyfADC[0];
	      else if (STOP2==true && EnergyfADC[1]>0.0035) P_fadcEnergy = EnergyfADC[1]+EnergyfADC[2];
	      else if (STOP3==true && EnergyfADC[1]>0.0025) P_fadcEnergy = EnergyfADC[1]+EnergyfADC[2]+EnergyfADC[3];
	      else if (STOP4==true && EnergyfADC[1]>0.0025) P_fadcEnergy = EnergyfADC[1]+EnergyfADC[2]+EnergyfADC[3]+EnergyfADC[4];
	      else if (STOP5==true && EnergyfADC[1]>0.0025) P_fadcEnergy = EnergyfADC[1]+EnergyfADC[2]+EnergyfADC[3]+EnergyfADC[4];
	      break;
	      //1st layer separately////////////////////////////////////////////////////////////////////////////////////
	  case 4:
	      //Calibration////////////////////////////////////////////////////////////////////////////////////
	      if (STOP1==true) P_fadcEnergy = EnergyfADC[0];
	      else if (STOP2==true) P_fadcEnergy = EnergyfADC[1];
	      else if (STOP3==true) P_fadcEnergy = EnergyfADC[2];
	      else if (STOP4==true) P_fadcEnergy = EnergyfADC[3];
	      else if (STOP5==true) P_fadcEnergy = EnergyfADC[4];
	      else P_fadcEnergy=0;
	      break;
	      //Calibration////////////////////////////////////////////////////////////////////////////////////
	  case 5:
	      //Calibration////////////////////////////////////////////////////////////////////////////////////
	      if (STOP2==true) P_fadcEnergy = EnergyfADC[1]+EnergyfADC[0];
	      else if (STOP3==true) P_fadcEnergy = EnergyfADC[2]+EnergyfADC[1];
	      else if (STOP4==true) P_fadcEnergy = EnergyfADC[3]+EnergyfADC[2];
	      else if (STOP5==true) P_fadcEnergy = EnergyfADC[4]+EnergyfADC[3];
	      else if (STOP6==true) P_fadcEnergy = EnergyfADC[5]+EnergyfADC[4];
	      else P_fadcEnergy=0;
	      break;
	      //Calibration////////////////////////////////////////////////////////////////////////////////////
	  case 6:
	      //Calibration////////////////////////////////////////////////////////////////////////////////////
	      if (STOP1==true) P_fadcEnergy = EnergyfADC[0];
	      else if (STOP2==true) P_fadcEnergy = EnergyfADC[1];
	      else if (STOP3==true) P_fadcEnergy = EnergyfADC[2];
	      else if (STOP4==true) P_fadcEnergy = EnergyfADC[3];
	      else if (STOP5==true) P_fadcEnergy = EnergyfADC[4];
	      else if (STOP6==true) P_fadcEnergy = EnergyfADC[5];
	      else P_fadcEnergy=0;
	      break;
	      //Calibration////////////////////////////////////////////////////////////////////////////////////
	  case 7:
	      //Calibration////////////////////////////////////////////////////////////////////////////////////
	      if (STOP1==true) P_fadcEnergy = EnergyfADC[0];
	      else if (STOP2==true) P_fadcEnergy = EnergyfADC[1];
	      else if (STOP3==true) P_fadcEnergy = EnergyfADC[2];
	      else if (STOP4==true) P_fadcEnergy = EnergyfADC[3];
	      else if (STOP5==true) P_fadcEnergy = EnergyfADC[4];
	      else if (STOP6==true) P_fadcEnergy = EnergyfADC[5];
	      break;
	      //Calibration////////////////////////////////////////////////////////////////////////////////////
	  case 9:
	      //Default/////////////////////////////////////////////////////////////////////////////////////////////////
	      if (STOP1==true) P_fadcEnergy = EnergyfADC[0]+EnergyfADC[1];
	      else if (STOP2==true) P_fadcEnergy = EnergyfADC[1]+EnergyfADC[2];
	      else if (STOP3==true) P_fadcEnergy = EnergyfADC[1]+EnergyfADC[2]+EnergyfADC[3];
	      else if (STOP4==true) P_fadcEnergy = EnergyfADC[1]+EnergyfADC[2]+EnergyfADC[3]+EnergyfADC[4];
	      else if (STOP5==true) P_fadcEnergy = EnergyfADC[1]+EnergyfADC[2]+EnergyfADC[3]+EnergyfADC[4]+EnergyfADC[5];
	      else if (STOP6==true) P_fadcEnergy = EnergyfADC[1]+EnergyfADC[2]+EnergyfADC[3]+EnergyfADC[4]+EnergyfADC[5];
	      break;
	      //Default//////////////////////////////////////////////////////////////////////////////////////////////////
	  default:
	      break;
      }

      //--- Only for offline analysis
      //---------------------------------------------------------------
  
      P_fadcEnergy += P_fadc.getMass();
      P_fadcEnergy += fadc_E_corr;
      
      if (P_fadcEnergy>P_fadc.getMass()){ 
	  P_fadcMomentum = sqrt(pow(P_fadcEnergy,2) - pow(P_fadc.getMass(),2));
	  P_fadc.setMomentum(P_fadcMomentum,0,0);
	  
	  switch (EMODE){
	      case 1:
		  /*** Inverse Bethe-Bloch in MSX1 ***/
		  P_fadc += MSX1->Egain(P_fadc);
		  break;
	      case 2:
		  /*** Inverse Bethe-Bloch in MSX ***/
		  if (STOP5==true) P_fadc += MSX3->Egain(P_fadc);
		  if (STOP5==true || STOP4==true) P_fadc += MSX2->Egain(P_fadc);
		  if (STOP5==true || STOP4==true || STOP3==true) P_fadc += MSX1->Egain(P_fadc);
		  break;
	      case 3:
		  /*** Inverse Bethe-Bloch in MSX1 ***/
		  if (STOP1!=true) P_fadc += MSX1->Egain(P_fadc);
		  break;
	      case 4:
		  /*** Inverse Bethe-Bloch in MSX1 ***/
		  if (STOP2==true){
		      double etmp1=MSX1->Egain(P_fadc);
		      E_MSX[0]->handle(etmp1); 
		  }
		  else if (STOP3==true){
		      double etmp2=MSX2->Egain(P_fadc);
		      E_MSX[1]->handle(etmp2); 
		  }
		  else if (STOP4==true){
		      double etmp3=MSX3->Egain(P_fadc);
		      E_MSX[2]->handle(etmp3); 
		  }
		  else if (STOP4==true){
		      double etmp4=MSX4->Egain(P_fadc);
		      E_MSX[3]->handle(etmp4); 
		  }
		  break;
	      case 5:
		  /*** Inverse Bethe-Bloch in MSX1 ***/
		  if (STOP2==true){
		      double etmp1=MSX1->Eloss(P_fadc);
		      P_fadc -= etmp1;
		      E_MSX[0]->handle(etmp1); 
		      E_MSX[1]->handle(P_fadc.energy()-P_fadc.getMass()); 
		  }
		  else if (STOP3==true){
		      double etmp2=MSX2->Eloss(P_fadc);
		      P_fadc -= etmp2;
		      E_MSX[1]->handle(etmp2); 
		      E_MSX[2]->handle(P_fadc.energy()-P_fadc.getMass()); 
		  }
		  else if (STOP4==true){
		      double etmp3=MSX3->Eloss(P_fadc);
		      P_fadc -= etmp3;
		      E_MSX[2]->handle(etmp3); 
		      E_MSX[3]->handle(P_fadc.energy()-P_fadc.getMass()); 
		  }
		  else if (STOP5==true){
		      double etmp4=MSX4->Eloss(P_fadc);
		      P_fadc -= etmp4;
		      E_MSX[3]->handle(etmp4); 
		      E_MSX[4]->handle(P_fadc.energy()-P_fadc.getMass()); 
		  }
		  else if (STOP6==true){
		      double etmp5=MSX5->Eloss(P_fadc);
		      P_fadc -= etmp5;
		      E_MSX[4]->handle(etmp5); 
		      E_VETO->handle(P_fadc.energy()-P_fadc.getMass());
		  } 
		  break;
	      case 6:
		  if (STOP2==true){
		      double x = EnergyfADC[1];
		      double y = 0;
		      for (int i=0; i<5; i++) y += pow(x,i)*dE1003_Erest[i]; 
		      E_MSX[0]->handle(y); 
		  }
		  else if (STOP3==true){
		      double x = EnergyfADC[2];
		      double y = 0;
		      for (int i=0; i<5; i++) y += pow(x,i)*dE1003_Erest[i]; 
		      E_MSX[1]->handle(y); 
		  }
		  else if (STOP4==true){
		      double x = EnergyfADC[3];
		      double y = 0;
		      for (int i=0; i<5; i++) y += pow(x,i)*dE1003_Erest[i]; 
		      E_MSX[2]->handle(y); 
		  }
		  else if (STOP5==true){
		      double x = EnergyfADC[4];
		      double y = 0;
		      for (int i=0; i<5; i++) y += pow(x,i)*dE1003_Erest[i]; 
		      E_MSX[3]->handle(y); 
		  }
		  else if (STOP6==true){
		      double x = EnergyfADC[5];
		      double y = 0;
		      for (int i=0; i<5; i++) y += pow(x,i)*dE1003_Erest[i]; 
		      E_MSX[4]->handle(y); 
		  }
		  break;
	      case 7:
		  if (STOP2==true){
		      double x = EnergyfADC[0];
		      double y = 0;
		      for (int i=0; i<5; i++) y += pow(x,i)*Erest_dE1003[i]; 
		      E_MSX[1]->handle(y); 
		  }
		  else if (STOP3==true){
		      double x = EnergyfADC[1];
		      double y = 0;
		      for (int i=0; i<5; i++) y += pow(x,i)*Erest_dE1003[i]; 
		      E_MSX[2]->handle(y); 
		  }
		  else if (STOP4==true){
		      double x = EnergyfADC[2];
		      double y = 0;
		      for (int i=0; i<5; i++) y += pow(x,i)*Erest_dE1003[i]; 
		      E_MSX[3]->handle(y); 
		  }
		  else if (STOP5==true){
		      double x = EnergyfADC[3];
		      double y = 0;
		      for (int i=0; i<5; i++) y += pow(x,i)*Erest_dE1003[i]; 
		      E_MSX[4]->handle(y); 
		  }
		  else if (STOP6==true){
		      double x = EnergyfADC[4];
		      double y = 0;
		      for (int i=0; i<5; i++) y += pow(x,i)*Erest_dE1003[i]; 
		      E_VETO->handle(y); 
		  }
		  break;
	      case 9:
		  if (STOP1==true) break;
		  else P_fadc += MSX1->Egain(P_fadc);
		  break;
	  }
	  
      	  /*** Inverse Bethe-Bloch in BB2 ***/
	  E_rectmp[2]=BB2->Egain(P_fadc);
	  P_fadc += E_rectmp[2];
	  E_rec[2]->handle(E_rectmp[2]);

	  /*** Inverse Bethe-Bloch in Scintillator ***/
	  E_rectmp[1]=Scint->Egain(P_fadc);
	  P_fadc += E_rectmp[1];
	  E_rec[1]->handle(E_rectmp[1]);

	  /*** Inverse Bethe-Bloch in Absorber ***/
	  E_rectmp[0]=Absorber->Egain(P_fadc);
	  P_fadc += E_rectmp[0];
	  E_rec[0]->handle(E_rectmp[0]);
	  
	  /*** Handle fADC Particle stuff ***/
	  EnergyP_fadc->handle(P_fadc.energy());
	  MomentumP_fadc->handle(P_fadc.momentum());
      }
  }
  
  OK->handle(1);

  delete Scint;
  delete Absorber;
  delete BB2;
  
  return 0;
}    

int SiliconDetector::print()
{
  int i;
  for (i=0; i<48; i++) std::cout << threshold[i] << ((i%8)==7 ? '\n' : '\t');
  std::cout << std::endl;
  for (i=0; i<6; i++) std::cout << a_MSX[i] << '\t' << b_MSX[i] << '\n';
  std::cout << std::endl;
  for (i=0; i<fadcChannels; i++) std::cout << a_fADC[i] << '\t' << b_fADC[i] << '\n';
  std::cout << std::endl;
  for (i=0; i<24; i++) std::cout << a_A[i] << '\t' << b_A[i] << '\n';
  std::cout << std::endl;
  for (i=0; i<24; i++) std::cout << a_B[i] << '\t' << b_B[i] << '\n';
  return 0;
}

void SiliconDetector::setFADC(int use_fadc_){
    USE_FADC = use_fadc_;
    switch (USE_FADC){
	case 1:
	    fadcChannels = 4;
	    break;
	case 2: 
	    fadcChannels = 6;
	    break;
	default:
	    fadcChannels = 0;
	    break;
    }
}

int SiliconDetector::init(const char *filename, double angle_, double distance_)
{
  std::ifstream *in = NULL;
  angle = angle_;
  distance = 10 * distance_; //[mm]

  if (filename && (in = new std::ifstream(filename)) && in->is_open()) {
    int i;
    for (i=0; i<54; i++) *in >> threshold[i];
    *in >> a_scint >> b_scint;
    for (i=0; i<6; i++)  *in >> a_MSX[i] >> b_MSX[i];
    for (i=0; i<fadcChannels; i++)  *in >> a_fADC[i] >> b_fADC[i];
    for (i=0; i<24; i++) *in >> a_A[i] >> b_A[i];
    for (i=0; i<24; i++) *in >> a_B[i] >> b_B[i];
    for (i=0; i<8; i++) *in>> thick[i];
    in->close(); in = NULL;
  } else {
    if (filename)
      std::cerr << "SiliconDetector::init:\tfile '" << filename
		<< "'could not be read.\n\t\t\t Using default parameters."
		<< std::endl;
    threshold[0] = 0;
    threshold[1] = 0;
    threshold[2] = 0;
    threshold[3] = 0;
    threshold[4] = 0;
    threshold[5] = 0;
    threshold[6] = 0;
    threshold[7] = 0;
    threshold[8] = 0;
    threshold[9] = 0;
    threshold[10] = 0;
    threshold[11] = 0;
    threshold[12] = 0;
    threshold[13] = 0;
    threshold[14] = 0;
    threshold[15] = 0;
    threshold[16] = 0;
    threshold[17] = 0;
    threshold[18] = 0;
    threshold[19] = 0;
    threshold[20] = 0;
    threshold[21] = 0;
    threshold[22] = 0;
    threshold[23] = 0;
    threshold[24] = 0;
    threshold[25] = 0;
    threshold[26] = 0;
    threshold[27] = 0;
    threshold[28] = 0;
    threshold[29] = 0;
    threshold[30] = 0;
    threshold[31] = 0;
    threshold[32] = 0;
    threshold[33] = 0;
    threshold[34] = 0;
    threshold[35] = 0;
    threshold[36] = 0;
    threshold[37] = 0;
    threshold[38] = 0;
    threshold[39] = 0;
    threshold[40] = 0;
    threshold[41] = 0;
    threshold[42] = 0;
    threshold[43] = 0;
    threshold[44] = 0;
    threshold[45] = 0;
    threshold[46] = 0;
    threshold[47] = 0;
    
    threshold[48] = 0;
    threshold[49] = 0;
    threshold[50] = 0;
    threshold[51] = 0;
    threshold[52] = 0;
    threshold[53] = 0;
    
    a_scint = 1.0; b_scint = 0.0;
 
    a_MSX[0] = 0.00293; b_MSX[0] = 0.08;
    a_MSX[1] = 0.00302; b_MSX[1] = 0.00;
    a_MSX[2] = 0.00287; b_MSX[2] = 0.00;
    a_MSX[3] = 0.00277; b_MSX[3] = 0.00;
    a_MSX[4] = 0.00259; b_MSX[4] = 0.019;
    a_MSX[5] = 0.00; b_MSX[5] = 0.00;

    for (int k=0; k<fadcChannels; k++){
	a_fADC[k] = 1.00; b_fADC[k] = 0.00;
    }

    a_A[0] = 0.00180; b_A[0] = -0.113;
    a_A[1] = 0.00194; b_A[1] = -0.338;
    a_A[2] = 0.00197; b_A[2] = -0.247;
    a_A[3] = 0.00168; b_A[3] =  0.008;
    a_A[4] = 0.00164; b_A[4] =  0.107;
    a_A[5] = 0.00175; b_A[5] = -0.073;
    a_A[6] = 0.00201; b_A[6] = -0.222;
    a_A[7] = 0.00245; b_A[7] = -0.678;
    a_A[8] = 0.00186; b_A[8] = -0.558;
    a_A[9] = 0.00193; b_A[9] = -0.535;
    a_A[10] = 0.00155; b_A[10] = -0.269;
    a_A[11] = 0.00170; b_A[11] = -0.334;
    a_A[12] = 0.00145; b_A[12] = -0.088;
    a_A[13] = 0.00133; b_A[13] = -0.265;
    a_A[14] = 0.00159; b_A[14] = -0.541;
    a_A[15] = 0.00171; b_A[15] = -0.525;
    a_A[16] = 0.00175; b_A[16] = -0.374;
    a_A[17] = 0.00154; b_A[17] = -0.187;
    a_A[18] = 0.00162; b_A[18] = -0.187;
    a_A[19] = 0.00190; b_A[19] = -0.438;
    a_A[20] = 0.00167; b_A[20] = -0.407;
    a_A[21] = 0.00179; b_A[21] = -0.680;
    a_A[22] = 0.00138; b_A[22] = -0.220;
    a_A[23] = 0.00138; b_A[23] = -0.737;
    
    a_B[0] = 0.00188; b_B[0] = -0.162;
    a_B[1] = 0.00192; b_B[1] = -0.186;
    a_B[2] = 0.00201; b_B[2] = -0.183;
    a_B[3] = 0.00192; b_B[3] = -0.200;
    a_B[4] = 0.00183; b_B[4] = -0.065;
    a_B[5] = 0.00196; b_B[5] = -0.190;
    a_B[6] = 0.00175; b_B[6] = -0.129;
    a_B[7] = 0.00185; b_B[7] = -0.172;
    a_B[8] = 0.00191; b_B[8] = -0.219;
    a_B[9] = 0.00200; b_B[9] = -0.126;
    a_B[10] = 0.00205; b_B[10] = -0.237;
    a_B[11] = 0.00185; b_B[11] = -0.106;
    a_B[12] = 0.00159; b_B[12] = -0.350;
    a_B[13] = 0.00162; b_B[13] = -0.447;
    a_B[14] = 0.00150; b_B[14] = -0.313;
    a_B[15] = 0.00152; b_B[15] = -0.323;
    a_B[16] = 0.00194; b_B[16] = -0.676;
    a_B[17] = 0.00187; b_B[17] = -0.566;
    a_B[18] = 0.00188; b_B[18] = -0.686;
    a_B[19] = 0.00180; b_B[19] = -0.466;
    a_B[20] = 0.00270; b_B[20] = -0.408;
    a_B[21] = 0.00247; b_B[21] = -0.646;
    a_B[22] = 0.00227; b_B[22] = -0.451;
    a_B[23] = 0.00207; b_B[23] = -0.703;

    thick[0] = 0.1; //Absorber  
    thick[1] = 0.03; //BB2
    thick[2] = 0.1003; //MSX1...
    thick[3] = 0.1003;
    thick[4] = 0.1003;
    thick[5] = 0.1003;
    thick[6] = 0.1003;
    thick[7] = 0.3250; //Scintillator
  }

  //debug output
  //print();

  return 0;
}
