// "npol.cc"
// Data reconstruction for the neutron polarimeter

//$Id: npol.cc 2216 2008-06-13 21:13:47Z distler $
// updated 2002/04/03 M. Seimetz

/*
#include <math.h>          // -> Constants and math. functions
#include <fstream.h>       // -> File handling
#include <stdio.h>         // -> cout / cerr
#include <stdlib.h>        // -> String handling

#include "Cindy/Cindy++.h" // -> Class CindyTree
#include "online.h"        // -> Struct Online, Snpol, ... 
#include ABC_H             // -> Struct abcn (Raw data)
#include "Masses.h"        // -> Class Particle
#include "rundatabase.h"   // -> Struct rundb
*/
#include "fstream"
#include "npol.h"          // -> Class Cnpol, ...
#include "npolPrint.cc"    // -> Rund.db check and saving configuration infos

// Angle of npol [rad]
#define alpha rundb.npol.alpha * M_PI / 180 

// Speed of Light in vacuum [cm/ns]
#define c_vak 29.9792458

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//:::Initialisation of static elements of CnpolPaddle:::::::::::::::::::::::://
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
int CnpolPaddle::padnr = 0;
CnpolConfigFiles *CnpolPaddle::conf = NULL;
double CnpolPaddle::startTime = 0;
double CnpolPaddle::ADCjitter = errorVal;
double CnpolPaddle::interruptrate = 0;


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//:::Stuff for Aqua:::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
void packAllEventData(struct SnpolPaddle *onl, AquaTree *out)
{
  out ->packEventData(&onl->allOk,1);
  out ->packEventData(&onl->adc1_cut,1);
  out ->packEventData(&onl->adc2_cut,1);
  out ->packEventData(&onl->pulsh_cut,1);
  out ->packEventData(&onl->tdc1_cut,1);
  out ->packEventData(&onl->tdc2_cut,1);
  out ->packEventData(&onl->time_cut,1);
  out ->packEventData(&onl->hit_cut,1);
  out ->packEventData(&onl->hitpaddle_cut,1);
  out ->packEventData(&onl->adc1,1);
  out ->packEventData(&onl->adc2,1);
  out ->packEventData(&onl->tdc1,1);
  out ->packEventData(&onl->tdc2,1);
  out ->packEventData(&onl->pulsh,1);
  out ->packEventData(&onl->time,1);
  out ->packEventData(&onl->diff,1);
  out ->packEventData(&onl->tof,1);
  out ->packEventData(&onl->beta,1);
  out ->packEventData(&onl->npolCoord[0],1);
  out ->packEventData(&onl->npolCoord[1],1);
  out ->packEventData(&onl->npolCoord[2],1); 
  out ->packEventData(&onl->labCoord[0],1);
  out ->packEventData(&onl->labCoord[1],1);
  out ->packEventData(&onl->labCoord[2],1);
  out ->packEventData(&onl->line,1);
  out ->packEventData(&onl->scaler,1);
  if (onl->lambda) out ->packEventData(&onl->lambda,1);
  if (onl->c_eff)  out ->packEventData(&onl->c_eff,1);
  
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//:::Read in for config. files::::::::::::::::::::::::::::::::::::::::::::::://
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
int CnpolConfig::readConfigFromFile(char filename[256],short anzahl, int *verbose) { 
  
  int number;
  char str[126];
  short problemo=1;
  if (strlen(filename)==0) return errorVal;
  if (*verbose) std::cerr << "Load file: " << filename <<std::endl;

  std::ifstream infile (filename, std::ios::in); //  | std::ios::nocreate
  if (!infile) {
    std::cerr << "ERROR: Cannot open file " << filename
	      << ". No such file !" << std::endl;
    return 0;
  }
  
  while (1) {
    if ((infile >> number) && (number >=0) && (number <=80)) {
      number--;    
      switch (anzahl) {
      case 1: 
	problemo=!(infile >> str >> col1[number]);
      break;
      case 2:
        problemo=!(infile >> str >> col1[number]  >> col2[number]);
      break;
      case 3:
        problemo=!(infile >> str >> col1[number]  >> col2[number]
                          >> col3[number]);
      break;
      case 4:
        problemo=!(infile >> str >> col1[number]  >> col2[number]
                          >> col3[number] >> col4[number]);
      break;
      }
      if (!problemo) {   
        infile.getline(str,125);
      }
      else {
        std::cerr << "ERROR in file: "<< filename << std::endl
		  << "Konfuzius says: \"There's something wrong with the "
		  << "format  of this file !\"" << std::endl; 
        infile.close();
        return 0;
      }                    
    }
    else {
      infile.clear();  // 0
      if (!infile.getline(str,125)) { infile.close();  return 1;}    
    }
  }
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//:::Correction of adc oscillation::::::::::::::::::::::::::::::::::::::::::://
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
short CnpolPaddle::TDCandADC(AquaTree *atree) {
  return atree->itemOK(tdc1) * atree->itemOK(tdc2) *
         atree->itemOK(adc1) * atree->itemOK(adc2);
}

short CnpolPaddle::onlyADC(AquaTree *atree) {
  return !(atree->itemOK(tdc1)) * !(atree->itemOK(tdc2)) *
         atree->itemOK(adc1) * atree->itemOK(adc2);
}

///////////////////////////////////////////
// Estimate amplitude of adc oscillation //
///////////////////////////////////////////
double CnpolWall1::getADCjitter(AquaTree *atree) {
  double sum=0;
  int  num=0;
  double delta1[2],delta2[2];
//Calculation is only done if "nPol.adc.maxjitter" (in run.db) > 0 
  if (*maxjitter > 0) {
    for (int i=1; i<16; i++) {
      if (ndet[i]->onlyADC(atree) && 
          ndet[15+i]->onlyADC(atree) ) {
        delta1[0]=*ndet[i]->adc1 - ndet[i]->pedlow1;
        delta1[1]=*ndet[i]->adc2 - ndet[i]->pedlow2;
        delta2[0]=*ndet[i+15]->adc1 - ndet[i+15]->pedlow1;
        delta2[1]=*ndet[i+15]->adc2 - ndet[i+15]->pedlow2;
        for (int n=0; n<2; n++) {
	  if (fabs(delta1[n]) < *maxjitter) { sum  += delta1[n]; num++ ;};
          if (fabs(delta2[n]) < *maxjitter) { sum  += delta2[n]; num++ ;};
        }
      }
    }
  }
  if (num > 0)  return (sum/num);
  else return errorVal;
}
 

//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//:::Reconstruction for each scintillator paddle::::::::::::::::::::::::::::://
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
int CnpolPaddle::reconstruct(AquaTree *atree, AquaTree *out)
{   
////////////////////
// Initialisation //
////////////////////
//Set explicit all Cuts to zero at the beginning of reconstruction
  onl->adc1_cut=0;      out->packEventData(&onl->adc1_cut,1);
  onl->adc2_cut=0;      out->packEventData(&onl->adc2_cut,1);
  onl->pulsh_cut=0;     out->packEventData(&onl->pulsh_cut,1);
  onl->tdc1_cut=0;      out->packEventData(&onl->tdc1_cut,1);
  onl->tdc2_cut=0;      out->packEventData(&onl->tdc2_cut,1);
  onl->time_cut=0;      out->packEventData(&onl->time_cut,1);
  onl->hit_cut=0;       out->packEventData(&onl->hit_cut,1);   
  onl->hitpaddle_cut=0; out->packEventData(&onl->hitpaddle_cut,1);
//Set in any case the run.db (default) positions 
  onl->npolCoord[0] = x; out->packEventData(&onl->npolCoord[0],1);
  onl->npolCoord[1] = y; out->packEventData(&onl->npolCoord[1],1);  
  onl->npolCoord[2] = z; out->packEventData(&onl->npolCoord[2],1);
//Copy run.db values in online structure
  if (SoL)    { onl->c_eff=SoL;     out->packEventData(&onl->c_eff,1);}
  if (lambda) { onl->lambda=lambda; out->packEventData(&onl->lambda,1);}

////////////////////////////////
// Reconstruction for Scalers //
////////////////////////////////
// Calculate scaler rate in Hz
  if (atree->itemOK(scaler)) {
     onl->scaler = *scaler * interruptrate / 128;
     out->packEventData(&onl->scaler,1);
/* Read out of the scalers (by DAQ) is done regulary after 128 interrups. */
  }

/////////////////////////////////
//==>Reconstruction for ADC's  //
/////////////////////////////////
// 1st CHECK for ADC's --- Both ADC's in data ? 
  onl->allOk =    atree->itemOK(adc1) + 
              2 * atree->itemOK(adc2);\
  out->packEventData(&onl->allOk,1);
  if (onl->allOk != 3)  return onl->hit_cut;
// 2nd CHECK for ADC's --- Both ADC's not in overflow ? 
  onl->allOk = 4 + (*adc1 < 8191 ) +
               2 * (*adc2 < 8191 );
  out->packEventData(&onl->allOk,1);
  if (onl->allOk != 7)  return onl->hit_cut;
// Pedestal (and jitter) correction 
  double ph1 = *adc1; 
  double ph2 = *adc2;
  short hrange[2] = {(ph1 > 4095) , (ph2 > 4095)};
  if (hrange[0])                                        
    ph1 = (ph1-4096) * 8 - pedup1;
  else
    ph1 -= (pedlow1 + (ADCjitter != errorVal) * ADCjitter);
  if (hrange[1]) 
    ph2 = (ph2-4096) * 8 - pedup2;
  else
    ph2 -= (pedlow2 + (ADCjitter != errorVal) * ADCjitter);
// 3rd CHECK for ADCs --- Pedestal (underflow) check
  onl->allOk = 7 + (ph1 > *minADC) + 
               2 * (ph2 > *minADC);
  out->packEventData(&onl->allOk,1);
  if (onl->allOk != 10)  return onl->hit_cut;  
// Do reconstruction vor ADC's
  onl->adc1=ph1;                                       // ADC 1  
  onl->adc2=ph2;                                       // ADC 2
  onl->adc1_cut  = (onl->adc1 > adc1L) *               // ADC cuts
                   (onl->adc1 < adc1H);
  onl->adc2_cut  = (onl->adc2 > adc2L) * 
                   (onl->adc2 < adc2H);
  onl->pulsh = sqrt(ph1 * ph2);                        // Pulsheigh
  onl->pulsh_cut = (onl->pulsh > pulshL) *             // Pulsheigh cut
                   (onl->pulsh < pulshH);
  onl->line = lambda/2 * (log(ph1/ph2) - deltaQ);      // Line
  out->packEventData(&onl->adc1,1);     out->packEventData(&onl->adc2,1);
  out->packEventData(&onl->adc1_cut,1); out->packEventData(&onl->adc2_cut,1);
  out->packEventData(&onl->pulsh,1);    out->packEventData(&onl->pulsh_cut,1);
  out->packEventData(&onl->line,1);
  if (detector==1) {                                   // special version
    onl->hit_cut = 10;                                 // of hit_cut for v.1st
    out->packEventData(&onl->hit_cut,1);               // (if TDC's not ok) 
  } 
/* "onl->hit_cut = 10" is equal to                          */
/* "onl->hit_cut = (ph1 > *minADC) * (ph2 > *minADC) * 10"   */
 
////////////////////////////////
//==>Reconstruction for TDC's //
////////////////////////////////
// 1st CHECK for TDC's --- Both TDC's in data  ?
  onl->allOk = 10 + 
                   atree->itemOK(tdc1) + 
               2 * atree->itemOK(tdc2);
  out->packEventData(&onl->allOk,1);
  if (onl->allOk != 13)  return onl->hit_cut;
// 2nd CHECK for TDC's --- Both TDC's not in overflow ?
  onl->allOk = 14 + (*tdc1 < 1023 )  + 
                2 * (*tdc2 < 1023 );
  out->packEventData(&onl->allOk,1);
  if (onl->allOk != 17)  return onl->hit_cut;
//Corrections for TDC's  
  double time1 = *tdc1 + startTime / *tdcScale;         // Start time corr.
  double time2 = *tdc2 + startTime / *tdcScale; 
  if (detector < 3) {       // 1st wall
    if (ph1 > 0)                                        // Walk correction
      time1 -= walk1 * (sqrt(1/ph1));
    if (ph2 > 0) 
      time2 -= walk2 * (sqrt(1/ph2));
  }
  else {                     // 2nd wall and trigger
    double rawdiff = (time1 - time2) * (*tdcScale) - Diff0;
    double rawx = x - 0.5 * SoL * c_vak * rawdiff;      // Space dependance
    if (ph1 > 0)
      time1 -= walk1 * exp(rawx/ (*mu)) * (sqrt(1/ph1));
    if (ph2 > 0) 
      time2 -= walk2 * exp(-rawx/ (*mu)) * (sqrt(1/ph2));
  }
// 3rd CHECK for TDC's --- Underflow check 
  onl->allOk = 17 + (time1 > 0) + 
                2 * (time2 > 0);
  out->packEventData(&onl->allOk,1);
  if (onl->allOk != 20)  return onl->hit_cut;
// DO reconstruction for TDC's 
  onl->tdc1 = time1;
  onl->tdc2 = time2;
  onl->tdc1_cut  = (onl->tdc1 > tdc1L) * 
                  (onl->tdc1 < tdc1H);
  onl->tdc2_cut  = (onl->tdc2 > tdc2L) * 
                  (onl->tdc2 < tdc2H);
  onl->time = (time1 + time2)/2 * (*tdcScale) - Time0;  // TDC_sum (Time)
  onl->diff = (time1 - time2) * (*tdcScale) - Diff0;    // TDC_diff (Diff)
  onl->time_cut  = (onl->time > timeL) *                // Time cut
                   (onl->time < timeH);
  if (startTime != 0 ) {
    onl->tof  = onl->time;                              // Time of Flight
    out->packEventData(&onl->tof,1);
  }
  out->packEventData(&onl->tdc1,1);      out->packEventData(&onl->tdc2,1);
  out->packEventData(&onl->tdc1_cut,1);  out->packEventData(&onl->tdc2_cut,1);
  out->packEventData(&onl->time,1);      out->packEventData(&onl->time_cut,1);  
  out->packEventData(&onl->diff,1);      out->packEventData(&onl->tof,1);

///////////////////////////////////////   
//==> Reconstruction of hit position //
///////////////////////////////////////
// npol coordinates (cartesian coord: x,y,z)
  if (detector < 3) {       // 1st wall
    onl->npolCoord[1] = y - 0.5 * SoL * c_vak *  onl->diff;
    out->packEventData(&onl->npolCoord[1],1);  
  }
  else {                     // 2nd wall and trigger
    onl->npolCoord[0] = x - 0.5 * SoL * c_vak *  onl->diff;
    out->packEventData(&onl->npolCoord[0],1);
  }
// Transformation in lab coordinates (spherical coord.: r,theta,phi) 
#define xn onl->npolCoord[0]
#define yn onl->npolCoord[1]
#define zn onl->npolCoord[2] 
  onl->labCoord[0] = sqrt(xn*xn + yn*yn + zn*zn);
  if (onl->labCoord[0] != 0) {
    onl->labCoord[1] = acos((xn*sin(alpha)+zn*cos(alpha))/onl->labCoord[0] );
    out->packEventData(&onl->labCoord[1],1);
  }
  onl->labCoord[2] = atan2(-yn,(-xn*cos(alpha)+zn*sin(alpha)));
#undef xn
#undef yn
#undef zn
  out->packEventData(&onl->labCoord[0],1);
  out->packEventData(&onl->labCoord[2],1);                   
// Reconstruction of beta   
  if (onl->tof != 0) {
    onl->beta = (onl->labCoord[0] / onl->tof) / c_vak;        
    out->packEventData(&onl->beta,1);
  }

////////////
// HitCut //
////////////
  if (detector==1) onl->hit_cut=(onl->tdc1_cut || onl->tdc2_cut); // v.1st  
  else 
  onl->hit_cut = onl->pulsh_cut * onl->time_cut;
  /*
    onl->hit_cut = onl->adc1_cut * onl->adc2_cut *              
    onl->tdc1_cut * onl->tdc2_cut;  // old version
  */
   out->packEventData(&onl->hit_cut,1);
  return onl->hit_cut;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//:::Reconstruction for subframes 2nd wall::::::::::::::::::::::::::::::::::://
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
int CnpolFrame::reconstruct(AquaTree *atree, AquaTree *out, int charged) 
{
//Do reconstruction and hit_cut calculation for each paddle in this frame
  short  vetohits[5];
  short  ndethits[13];
  for (int i=1; i<5; i++) vetohits[i]=veto[i]->reconstruct(atree, out);
  for (int i=1; i<13; i++) ndethits[i]=ndet[i]->reconstruct(atree, out);

////////////////////////////////////////////////
//Combine ndet hit_cut with veto information // 
///////////////////////////////////////////////
/* ndethits[i]=0 for no hit(_cut) in ndet                             */ 
/* ndethits[i]=1 for Anti-Coincedence between veto and ndet (neutron) */
/* ndethits[i]=2 for Coincedence between veto and ndet (proton)       */
  if (*vetoUse) {
    if (!(*neighbourVetoUse)) {
      for (int i=1; i<5;i++)  ndethits[i]=(1 + vetohits[i]) * ndethits[i];
      for (int i=5; i<9;i++)  ndethits[i]=(1 + vetohits[i-4]) * ndethits[i];
      for (int i=9; i<13;i++) ndethits[i]=(1 + vetohits[i-8]) * ndethits[i];
    }
    else  {
      for (int i=1; i<5;i++) {
        int il = (i >  1 ? i-1 :  1);      // left neighbour
        int ir = (i < 4 ? i+1 : 4);        // right neighbour
        ndethits[i] = (1 +(vetohits[i] || vetohits[il] || vetohits[ir]))
	  * ndethits[i]; } 
      for (int i=5; i<9;i++) {
        int il = (i > 5 ? i-1 : 5);
        int ir = (i < 8 ? i+1 : 8);
        ndethits[i] = (1 + (vetohits[i-4] || vetohits[il-4] || vetohits[ir-4]))
	  * ndethits[i]; }  
      for (int i=9; i<13;i++) {
        int il = (i > 9 ? i-1 : 9);
        int ir = (i < 12 ? i+1 : 12);
        ndethits[i] = (1 + (vetohits[i-8] || vetohits[il-8] || vetohits[ir-8])) 
	  * ndethits[i]; }	
    }
  }

//////////////////////////
//Veto hits in subframe // 
//////////////////////////
  onl->vetomulti=0;         
  onl->rawvetomulti=0;
  for (int i=1; i<5; i++) {
    onl->vetomulti +=  (vetohits[i] > 0);          // HitCut - Multiplicity
    onl->rawvetomulti += veto[i]->TDCandADC(atree);// Data complete - Multipl. 
    if (vetohits[i]) {                             // Store # of paddle        
      onl->vetohit[onl->vetomulti]=i;              // in online structure 
      out->packEventData(&onl->vetohit[onl->vetomulti],1);
    }
  }
  out->packEventData(&onl->vetomulti,1);
  out->packEventData(&onl->rawvetomulti,1);
// Reconstruction of hitpaddle 
/* Take accidental paddle if more than one hit. */
  int random=1; 
  if (onl->vetomulti > 0) {
    if (onl->vetomulti > 1)
      random = int(rint(ran() * (onl->vetomulti-1)) + 1);        
      // random number (integer) between 1 .. onl->vetomulti
    int vhit=onl->vetohit[random];
    onl->veto[vhit].hitpaddle_cut = 1;      // Set hitpaddle_cut for hit_paddle
    onl->veto[0]=onl->veto[vhit];           // Copy hitpaddle in virtual paddle
    onl->vetohit[0]=vhit;                   // Store # of hitpaddle
    packAllEventData(&onl->veto[0], out);
    out->packEventData(&onl->veto[vhit].hitpaddle_cut, 1);
    out->packEventData(&onl->vetohit[0],1);
  }

//////////////////////////
//Ndet hits in subframe // 
//////////////////////////
  onl->ndetmulti[0]=onl->ndetmulti[1]=onl->ndetmulti[2]=0; 
  onl->rawndetmulti[0]=onl->rawndetmulti[1]=onl->rawndetmulti[2]=0;
  for (int j=0; j<3; j++) {
    for (int i=1; i<5; i++) {
      onl->triggermulti[j] += 
                     charged * (ndethits[i+j*4] > (*vetoUse))+ // "Good hits"
                    !charged * (ndethits[i+j*4] == 1); 
      onl->ndetmulti[j] +=  (ndethits[i+j*4] > 0);    // HitCut - Multiplicity
      onl->rawndetmulti[j] +=ndet[i+j*4]->TDCandADC(atree); // Data complete -
    }                                                      // Multiplicity 
  }
  out->packEventData(&onl->triggermulti[0],1);
  out->packEventData(&onl->triggermulti[1],1);
  out->packEventData(&onl->triggermulti[2],1);
  out->packEventData(&onl->ndetmulti[0],1);
  out->packEventData(&onl->ndetmulti[1],1);
  out->packEventData(&onl->ndetmulti[2],1);
  out->packEventData(&onl->rawndetmulti[0],1);
  out->packEventData(&onl->rawndetmulti[1],1);
  out->packEventData(&onl->rawndetmulti[2],1);

  int   trigger[13];
  short totalmulti = 0;
  short totaltriggermulti=0;
  for (int i=1; i<13; i++) {
    if (ndethits[i]) {
      totalmulti++; 
      onl->ndet[i].hit_cut=ndethits[i];             //Store hit type in hit_Cut
      out->packEventData(&onl->ndet[i].hit_cut,1);
      onl->ndethit[totalmulti]=i;                       //Store # of paddle 
      out->packEventData(&onl->ndethit[totalmulti],1);  //in online structure
// Special for hitpaddle reconstruction
      totaltriggermulti +=  charged * (ndethits[i] > (*vetoUse)) +
                           !charged * (ndethits[i] == 1); 
      if (charged && (ndethits[i] > (*vetoUse))) trigger[totaltriggermulti]=i;  
      if (!charged && (ndethits[i] == 1)) trigger[totaltriggermulti]=i; 
    }
  }
// Reconstruction of ndet hitpaddle, dependent on rund.db reaction
/* If only one hit in first layer then take this paddle as hitpaddle */
/* else take accidental hitpaddle */
  random=1;
  if (totaltriggermulti > 0) {        
    if (onl->triggermulti[0] != 1) 
      random = int(rint(ran() * (totaltriggermulti-1)) + 1);        
      // random number (integer) between 1 .. triggermulti
    int nhit=trigger[random];
    onl->ndet[nhit].hitpaddle_cut = 1;    // Set hitpaddle_cut for hit_paddle
    onl->ndet[0]=onl->ndet[nhit];         // Copy hitpaddle in virtual paddle
    onl->ndethit[0]=nhit;                 // Store # of hitpaddle
    packAllEventData(&onl->ndet[0], out);
    out->packEventData(&onl->ndet[nhit].hitpaddle_cut, 1);
    out->packEventData(&onl->ndethit[0],1);
    return 1;
  }
  else return 0;
} 


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//:::Reconstruction for total 2nd wall::::::::::::::::::::::::::::::::::::::://
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
int CnpolWall2::reconstruct(AquaTree *atree, AquaTree *out, int charged) 
{
// Do reconstruction for both subframes // 
  onl->eventpattern =    top->reconstruct(atree, out, charged) +
                     2 * bottom->reconstruct(atree, out, charged);
  out->packEventData(&onl->eventpattern,1);

///////////////////////////////////////
// Veto hitpaddle for total 2nd wall // 
///////////////////////////////////////
  short vhit = (onl->top.vetomulti > 0)+ 2 * (onl->bottom.vetomulti > 0); 
  switch (vhit) {
    case 1 : onl->vhitpaddle=onl->top.veto[0];
             packAllEventData(&onl->vhitpaddle, out);
             onl->vetohit=onl->top.vetohit[0];
             out->packEventData(&onl->vetohit,1);
             break; 
    case 2 : onl->vhitpaddle=onl->bottom.veto[0];
             packAllEventData(&onl->vhitpaddle, out);
             onl->vetohit=onl->bottom.vetohit[0] + 4;
             out->packEventData(&onl->vetohit,1);
             break; 
    case 3 : if ( ran() <= 0.5) {
               onl->vhitpaddle=onl->top.veto[0];
               onl->vetohit=onl->top.vetohit[0];
             }
             else {
               onl->vhitpaddle=onl->bottom.veto[0];
               onl->vetohit=onl->bottom.vetohit[0] + 4;
             }
             packAllEventData(&onl->vhitpaddle, out);
             out->packEventData(&onl->vetohit,1);  
             break; 
  }

///////////////////////////////////////
// Ndet hitpaddle for total 2nd wall // 
///////////////////////////////?///////
  switch (onl->eventpattern) {
    case 1 : onl->nhitpaddle=onl->top.ndet[0];
             packAllEventData(&onl->nhitpaddle, out);
             onl->ndethit=onl->top.ndethit[0];
             out->packEventData(&onl->ndethit,1);
             break; 
    case 2 : onl->nhitpaddle=onl->bottom.ndet[0];
             packAllEventData(&onl->nhitpaddle, out);
             onl->ndethit=onl->bottom.ndethit[0] + 12;
             out->packEventData(&onl->ndethit,1);
             break; 
    case 3 : if ( ran() <= 0.5) {
               onl->nhitpaddle=onl->top.ndet[0];
               onl->ndethit=onl->top.ndethit[0];
             }
             else {
               onl->nhitpaddle=onl->bottom.ndet[0];
               onl->ndethit=onl->bottom.ndethit[0] + 12;
             }
             packAllEventData(&onl->nhitpaddle, out);
             out->packEventData(&onl->ndethit,1);
             break; 
  }
  return onl->eventpattern;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//:::Reconstruction for 1st wall::::::::::::::::::::::::::::::::::::::::::::://
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
int CnpolWall1::reconstruct(AquaTree *atree, AquaTree *out, int charged) 
{
//Start reconstruction and hit_cut calculation for each paddle in 1st wall
  short vetohits[16];
  short ndethits[32];
  for (int i=1; i<31;i++) ndethits[i]=ndet[i]->reconstruct(atree, out);
  for (int i=1; i<16;i++) vetohits[i]=veto[i]->reconstruct(atree, out);

///////////////////////////////////////////////
//Combine ndet hit_cut with veto information // 
//////////////////////////////////?////////////
/* ndethits[i]=0 for no hit(_cut) in ndet                             */
/* ndethits[i]=1 for Anti-Coincedence between veto and ndet (neutron) */
/* ndethits[i]=2 for Coincedence between veto and ndet      (proton)  */
/* ndethits[i]=11 for special veto hit_cut (only ADC's)     (proton?) */
  if (*vetoUse) {
    if (!(*neighbourVetoUse)) {
      for (int i=1; i<16;i++)  ndethits[i]=(1 + vetohits[i]) * ndethits[i];
      for (int i=16; i<31;i++) ndethits[i]=(1 + vetohits[i-15]) * ndethits[i];
    }
    else {
      short vhittype;
      for (int i=1; i<16;i++) {
        int il = (i >  1 ? i-1 :  1);      // left neighbour
        int ir = (i < 15 ? i+1 : 15);      // right neighbour  
        vhittype= ((vetohits[i] + vetohits[il] + vetohits[ir])  > 0) *
	(1 +  9 * (vetohits[i]!=1)*(vetohits[il]!=1)*(vetohits[ir]!=1));
        ndethits[i] = (1 + vhittype) * ndethits[i]; }
      for (int i=16; i<31;i++) {
        int il = (i > 16 ? i-1 : 16);
        int ir = (i < 30 ? i+1 : 30);
        vhittype= ((vetohits[i-15] + vetohits[il-15] + vetohits[ir-15])  > 0) *
      (1 +  9 * (vetohits[i-15]!=1)*(vetohits[il-15]!=1)*(vetohits[ir-15]!=1));
        ndethits[i]=(1 + vhittype) * ndethits[i]; }   
    }
  }
//////////////////////////
//Veto hits in 1st wall // 
//////////////////////////
  onl->vetomulti=0;         
  onl->rawvetomulti=0;
  onl->vetoPhits=0;
  for (int i=1; i<16; i++) {
    onl->vetomulti +=  (vetohits[i] > 0);         // HitCut - Multiplicity
    onl->rawvetomulti += veto[i]->TDCandADC(atree);// Data complete - Multipl. 
    onl->vetoPhits += (ndethits[i] > 1) * (ndethits[i+15] > 1);  
    if (vetohits[i]) {                              // Store # of paddle     
      onl->vetohit[onl->vetomulti]=i;               // in online structure 
      out->packEventData(&onl->vetohit[onl->vetomulti],1);
    }
  }
  out->packEventData(&onl->vetomulti,1);
  out->packEventData(&onl->rawvetomulti,1);
  out->packEventData(&onl->vetoPhits,1);
// Reconstruction of hitpaddle 
/* Take accidental paddle if more than one hit. */
  int random=1;
  if (onl->vetomulti > 0) {
    if (onl->vetomulti > 1) 
      random = int(rint(ran() * (onl->vetomulti-1)) + 1);
      // random number (integer) between 1 .. onl->vetomulti   
    int vhit=onl->vetohit[random];        
    onl->veto[vhit].hitpaddle_cut = 1;      // Set hitpaddle_cut for hit_paddle
    onl->veto[0]=onl->veto[vhit];           // Copy hitpaddle in virtual paddle
    onl->vetohit[0]=vhit;                   // Store # of hitpaddle
    packAllEventData(&onl->veto[0], out);
    out->packEventData(&onl->veto[vhit].hitpaddle_cut, 1);
    out->packEventData(&onl->vetohit[0],1);
  }

//////////////////////////
//Ndet hits in 1st wall // 
/////////////////?////////
  onl->ndetmulti[0]=onl->ndetmulti[1]=0; 
  onl->rawndetmulti[0]=onl->rawndetmulti[1]=0;
  for (int j=0; j<2; j++) {
    for (int i=1; i<16; i++) {
      onl->triggermulti[j] += 
                    charged * (ndethits[i+j*15] > (*vetoUse)) +// "Good hits" 
	           !charged * (ndethits[i+j*15] == 1);                         
      onl->ndetmulti[j] +=  (ndethits[i+j*15] > 0);   // HitCut - Multiplicity
      onl->rawndetmulti[j] +=ndet[i+j*15]->TDCandADC(atree);// Data complete - 
    }                                                    // Multiplicity
  }
  out->packEventData(&onl->triggermulti[0],1);
  out->packEventData(&onl->triggermulti[1],1);
  out->packEventData(&onl->ndetmulti[0],1);
  out->packEventData(&onl->ndetmulti[1],1);
  out->packEventData(&onl->rawndetmulti[0],1);
  out->packEventData(&onl->rawndetmulti[1],1);

  int   trigger[31];
  short totalmulti = 0;
  short totaltriggermulti=0;
  for (int i=1; i<31; i++) {
    if (ndethits[i]) {
      totalmulti++; 
      onl->ndet[i].hit_cut=ndethits[i];             //Store hit type in hit_Cut
      out->packEventData(&onl->ndet[i].hit_cut,1);
      onl->ndethit[totalmulti]=i;                      //Store # of paddle 
      out->packEventData(&onl->ndethit[totalmulti],1); //in online structure
// Special for hitpaddle reconstruction
      totaltriggermulti +=  charged * (ndethits[i] > (*vetoUse)) + 
                           !charged * (ndethits[i] == 1);
      if (charged && (ndethits[i] > (*vetoUse))) trigger[totaltriggermulti]=i;  
      if (!charged && (ndethits[i] == 1)) trigger[totaltriggermulti]=i; 
    }
  }
// Reconstruction of ndet hitpaddle, dependent on rund.db reaction
/* If in first layer only one hit then take this as hitpaddle */
/* else take accidental hitpaddle */
  random=1;
  if (totaltriggermulti > 0) {
    if (onl->triggermulti[0] != 1) 
      random = int(rint(ran() * (totaltriggermulti-1)) + 1);  
      // random number (integer) between 1 .. triggermulti
    int nhit=trigger[random];         
    onl->ndet[nhit].hitpaddle_cut = 1;      // Set hitpaddle_cut for hit_paddle
    onl->ndet[0]=onl->ndet[nhit];           // Copy hitpaddle in virtual paddle
    onl->ndethit[0]=nhit;                   // Store # of hitpaddle
    packAllEventData(&onl->ndet[0], out);
    out->packEventData(&onl->ndet[nhit].hitpaddle_cut, 1);
    out->packEventData(&onl->ndethit[0],1);
    onl->eventpattern = 1;  
  }
  else onl->eventpattern = 0;
  out->packEventData(&onl->eventpattern,1);
  return onl->eventpattern; 
} 


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//:::Reconstruction for total npol::::::::::::::::::::::::::::::::::::::::::://
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
int Cnpol::reconstruct(AquaTree *atree, AquaTree *out, Particle &particle, 
                       double tStart) 
{
////////////////////////////////////
// Initialisation and other stuff //
////////////////////////////////////
  if (!npolInReaction) { 
    std::cout << "You are analysing npol data, but NPOL ist not specified" 
	      <<  "in the run.db reaction." << std::endl;
   return errorVal;
  }
// Correction for pedestal oscillation
  onl->wall1.ADCjitter = wall1->getADCjitter(atree);
  out->packEventData(&onl->wall1.ADCjitter,1);
  CnpolPaddle::ADCjitter = onl->wall1.ADCjitter;
// Starttime for ToF (tstart = tTimeAtTarget is given by specA)
  CnpolPaddle::startTime=tStart;
// Calculating interrupt rate
  if ((Interrupts++ % 128) == 0) {
    CnpolPaddle::interruptrate = 128 * 1000 / (*TimeStamp - OldTimeStamp); 
    OldTimeStamp = *TimeStamp;
  }
  if ((Interrupts % 5000) == 0 && ColaStartTime != errorVal) {
    time_t now=time(NULL);
    std::cout << "Event " << Interrupts << ": "
	      <<  (now - ColaStartTime) << " s ("  
	      << 5000 / (now - lastTime) << " Hz)" << std::endl;
    lastTime=now;   
  }
  onl->interruptrate = CnpolPaddle::interruptrate;
  out->packEventData(&onl->interruptrate,1);
//////////////////////////////////////////////////////////////
//Reconstruction for 1st and 2nd wall and trigger detectors // 
//////////////////////////////////////////////////////////////
  int charge = (particle.getCharge() != 0);
  onl->eventpattern =  wall1->reconstruct(atree, out, charge) +
                     2 *  wall2->reconstruct(atree, out, charge);
  out->packEventData(&onl->eventpattern,1);
//==> Reconstruction for trigger detectors
  for (int i=1; i<4; i++) { trigger[i]->reconstruct(atree, out); }

///////////////////////////////////
//If hit(s) in ndets of 1st wall // 
///////////////////////////////////
//    Using information of the hit in ndets 1st wall for energy reconstruction
//    and for reconstruction of the fieldintegral.
  if (onl->wall1.eventpattern) {
    double beta = onl->wall1.ndet[0].beta;
    if (beta > 0 && beta < 1) { 
      double energy   = particle.getMass() / sqrt(1 - beta*beta);
      double momentum = particle.getMass() / sqrt(1/(beta*beta)-1);
      particle.initPolar(energy,momentum, onl->wall1.ndet[0].labCoord[1],
                       onl->wall1.ndet[0].labCoord[2]);
    }
    magnet->getMagnetCoords(out,onl->wall1.ndet[0].npolCoord); 
// Calculating the fieldintegral only when magnet is on (current !=0) ;-)
    if (magnet->RundbCurrent !=0) magnet->getFieldIntegral(out);
/////////////////////////////////////////
//If hits in ndets of 1st AND 2nd wall // 
/////////////////////////////////////////
//    Using the infos from hits in ndets of 1st and 2nd wall for reconstruction
//    of the scatterangles in 1st wall and for ToF between 1st and 2nd wall.
#define r1 onl->wall1.ndet[0].labCoord[0]
#define t1 onl->wall1.ndet[0].labCoord[1]
#define p1 onl->wall1.ndet[0].labCoord[2]
#define r2 onl->wall2.nhitpaddle.labCoord[0]
#define t2 onl->wall2.nhitpaddle.labCoord[1]
#define p2 onl->wall2.nhitpaddle.labCoord[2] 
    if(onl->wall2.eventpattern) {
      double xbar,ybar,zbar ;
      xbar = r2 *(cos(t2)*sin(t1)-sin(t2)*cos(t1)*cos(p1-p2) );
      ybar = r2 *( sin(t2)*sin(p1-p2) );
      zbar = r2 *( cos(t1)*cos(t2)+sin(t1)*sin(t2)*cos(p1-p2) ) - r1;
#undef r1
#undef t1
#undef p1
#undef r2
#undef t2
#undef p2
      onl->scatterCoord[0] = sqrt(xbar*xbar + ybar*ybar + zbar*zbar);
      onl->scatterCoord[1] = acos( zbar / onl->scatterCoord[0] );
      onl->scatterCoord[2] = atan2(ybar,xbar);  
      onl->tof12 = onl->wall2.nhitpaddle.tof - onl->wall1.ndet[0].tof;
      if (onl->tof12 != 0) {
        onl->beta12 = (onl->scatterCoord[0]/onl->tof12) / c_vak;
        out->packEventData(&onl->beta12,1);
      }
      out->packEventData(&onl->scatterCoord[0],1);
      out->packEventData(&onl->scatterCoord[1],1);
      out->packEventData(&onl->scatterCoord[2],1);
      out->packEventData(&onl->tof12,1);
    }
  }
  return onl->eventpattern;
}


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//:::Magnet:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://

//////////////////////////////////////////////////////
//Autocomplete for filename of magnet config. file  // 
//////////////////////////////////////////////////////
void CnpolMagnet::getfilename(char path[256]) {
  
  char buffer[10];
  strcpy(filename,path);
  strcat(filename,"magnetI");
  strcat(filename,gcvt(rint(RundbCurrent),4,buffer));
  strcat(filename,"A.list");
}

////////////////////////////////////
//Read in for magnet config file  // 
////////////////////////////////////
short CnpolMagnet::readMagnetDataFromFile(int *verbose) {

  char dummy[80];

  if (*verbose) std::cerr << "Load file: "<< filename << std::endl;
  std::ifstream infile (filename, std::ios::in); //  | ios::nocreate
  if (infile.fail()) {
    std::cerr << "ERROR: Cannot open file "
	      << filename << " No such file !" << std::endl;
    return 0;
  }
    
  double x0,y0;
  int indexControl[2*npm_xdim+1][2*npm_ydim+1];  // control array
  for (int(i1)=0; i1<=2*npm_xdim; i1++)
    for (int(i2)=0; i2<=2*npm_ydim; i2++)
      indexControl[i1][i2] = 0;
  /*
  for (int(i1)=0; i1<=2*npm_xdim; i1++)
    for (int(i2)=0; i2<=2*npm_ydim; i2++)
      std::cerr << indexControl[i1][i2];
  */

  while (1) {
    if (infile >> x0 >> y0) {
      if (int(x0)%10 == 0)
	x0=x0/10;
      if (int(y0)%10 == 0)
	y0=y0/10;
      if ((fabs(x0) > npm_xdim) || (fabs(y0) > npm_ydim)) {
        std::cerr << "ERROR: The file " << filename << "contains coordinates "
		  << "which are incompatible with the Cola++ routine"
		  << std::endl;
        infile.close();
        return 0;
      }
      if (infile >> integral[getXindex(x0)][getYindex(y0)]) {
        infile.getline(dummy,79); 
	/*
	std::cerr << "x0: " << x0 << " y0: " << y0 << " Byint: " << 
	integral[getXindex(x0)][getYindex(y0)] << std::endl;
	*/
	if (indexControl[getXindex(x0)][getYindex(y0)] == 0){
	  indexControl[getXindex(x0)][getYindex(y0)] = 1;
	  // std::cerr << indexControl[getXindex(x0)][getYindex(y0)];
	}
	else {
	  std::cerr << "Magnet field index already occupied!" << std::endl;
	  return 0;
	}
      }
      else {
        std::cerr << "ERROR in file: "<< filename << std::endl
		  << "Konfuzius says: \"There's something wrong with "
		  << "the format of this file !\"" << std::endl; 
        infile.close();
        return 0;
      }
    }
    else {
      infile.clear(); // 0
      /*
      for (int(j1)=0; j1<=2*npm_xdim; j1++)
	for (int(j2)=0; j2<=2*npm_ydim; j2++)
	  std::cerr << indexControl[j1][j2];
      */
      if (!infile.getline(dummy,79)) {
	infile.close(); 
	for (int(i1)=0; i1<=2*npm_xdim; i1++)
	  for (int(i2)=0; i2<=2*npm_ydim; i2++)
	    if (indexControl[i1][i2] != 1){
	      std::cerr << "ERROR in file: "<< filename << std::endl
		   << "Field indices not consistent with Cola++ default format"
		   << std::endl;
	      return 0;
	    }
	return 1;
      }    
    }
  }
}

////////////////////////////////////////////////////////
//Calculate hit coordinats at the front of the magnet // 
////////////////////////////////////////////////////////
void CnpolMagnet::getMagnetCoords(AquaTree *out,double firstWpos[3]) {
  if (firstWpos[2] != 0) {
    onl->frontCoord[0]=firstWpos[0] * position[2]/firstWpos[2] - position[0];
    onl->frontCoord[1]=firstWpos[1] * position[2]/firstWpos[2] - position[1];
  }
  else {onl->frontCoord[0]=errorVal; onl->frontCoord[1]=errorVal;}
  out->packEventData(&onl->frontCoord[0],1);
  out->packEventData(&onl->frontCoord[1],1);
}

///////////////////////////////////////////////////////
//Get corresponding array index for a given position // 
///////////////////////////////////////////////////////
int CnpolMagnet::getXindex(double x0) {
  return int(rint(x0) + npm_xdim);
}

int CnpolMagnet::getYindex(double y0) {
  return int(rint(y0) + npm_ydim); 
}

////////////////////////////////////////////////////////
//Get fieldintegral using hit position in first wall  // 
////////////////////////////////////////////////////////
void CnpolMagnet::getFieldIntegral(AquaTree *out) {
  //double drift: fabs(RundbCurrent-*MezzoCurrent)/RundbCurrent;
  //if (drift> 0.1)
  //  std::cout << "WARNING: The actual magnet current drifts " << drift*100
  //            << "% from the specified run.db value." << std::endl;
  if ((fabs(onl->frontCoord[0]) <= npm_xdim) && 
      (fabs(onl->frontCoord[1]) <= npm_ydim)) {
    onl->fieldintegral=
    integral[getXindex(onl->frontCoord[0])][getYindex(onl->frontCoord[1])];
    out->packEventData(&onl->fieldintegral,1);
  }
}

#undef alpha
#undef c_vak
#undef errorVal
///////THE END/////////////////////////////////////////////////////////////////
