//
// Created 1993 by	Michael O. Distler,
//                      Harald Merkel
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Cola/SCINTraw.cc,v 2.4 2002-12-12 12:23:55 distler Exp $
//

#include "Aqua_abcn.h"
#include "Aqua_online.h"
#include "rundatabase.h"
#include <math.h>
#include <iostream>

/////////////////////////////////////////
//  NEW VERSION 2014  //bss//
// includes software pedestal subtraction, needed for ISR2013, deuteron 2014, ...
// also further changes like: scale ADC values first, then look for largest signal for the hit pattern, ...
//  OLD ONE BELOW, IS STILL USED AS DEFAULT, to use new one:
// in run.db: A.scint.UseNewHandle = 1 and so on
/////////////////////////////////////////
int scintillator_2014(AquaTree  *atree,
		      AquaTree  *out,
		      struct Spectrometer &onl, 
		      struct SPECTROMETER &rund,
		      int num_dEpad, int num_ToFpad,
		      struct MpsPaddle *dEpad, 
		      struct MpsPaddle *ToFpad,
		      struct MpsPm *dE_B,
		      short *de_tof_time, 
		      char SpecName);
int scintillator_OLD(AquaTree  *atree,
		 AquaTree  *out,
		 struct Spectrometer &onl, 
                 struct SPECTROMETER &rund,
		 int num_dEpad, int num_ToFpad,
                 struct MpsPaddle *dEpad, 
                 struct MpsPaddle *ToFpad,
                 struct MpsPm *dE_B,
		     short *de_tof_time);


/////////////////////////////////////////
//THE VERSION TO BE USED IS CHOSEN HERE:
/////////////////////////////////////////

int scintillator(AquaTree  *atree,
		 AquaTree  *out,
		 struct Spectrometer &onl, 
                 struct SPECTROMETER &rund,
		 int num_dEpad, int num_ToFpad,
                 struct MpsPaddle *dEpad, 
                 struct MpsPaddle *ToFpad,
                 struct MpsPm *dE_B,
                 short *de_tof_time, 
		 char SpecName)
{
  if (rund.scint.UseNewHandle != 0)
    return scintillator_2014(atree, out, onl, rund, num_dEpad, num_ToFpad, dEpad, ToFpad, dE_B, de_tof_time, SpecName);
  else return scintillator_OLD(atree, out, onl, rund, num_dEpad, num_ToFpad, dEpad, ToFpad, dE_B, de_tof_time);
}


int scintillator_2014(AquaTree  *atree,
		 AquaTree  *out,
		 struct Spectrometer &onl, 
                 struct SPECTROMETER &rund,
		 int num_dEpad, int num_ToFpad,
                 struct MpsPaddle *dEpad, 
                 struct MpsPaddle *ToFpad,
                 struct MpsPm *dE_B,
                 short *de_tof_time, 
		      char SpecName)
{
  //bssdel:  
  //  std::cerr << "Creating spek" << SpecName << std::endl;

  int pattern = 0, i;
  int counter = 0;
  int dummy_counter = 0;

  onl.ToF.paddle      = onl.dE.paddle = -1;
  onl.ToF.scint       = onl.dE.scint  = 0;
  onl.ToF.Energy.left = onl.ToF.Energy.right = 0;
  onl.dE.Energy.left  = onl.dE.Energy.right = 0;

  double maxenergy = 0;
  if (!onl.trigger)  return 0;

  ///////////////////////
  // ToF A, B and C
  ///////////////////////

  int number = 1;
  for (i=0; i<num_ToFpad; i++) {
    if (atree->itemOK(&ToFpad[i].left.energy) && atree->itemOK(&ToFpad[i].right.energy)) {
      
      onl.ToF.AdcPedCorr_left[i] = onl.ToF.AdcPedCorr_right[i] = 0.;
      onl.ToF.AdcScaled_left[i] = onl.ToF.AdcScaled_right[i] = 0.;

      double temp_Ped=0;

      //bss 2013-12-30 the hardware pedestal subtraction has a minimum ADC value above pedestal, 
      //i.e. ADC > 5 ? ADC : 0; emulate this in case hardware subtraction is replaced by software subtr.
      double PedCorr = ToFpad[i].left.energy + rund.scint.ToF_corr_left_offset[i];
      //std::cout << i << "\t" << PedCorr << "\t";
      temp_Ped+=PedCorr;
      if ( PedCorr >= rund.scint.MinAdcOverThresholdValue) {
        onl.ToF.AdcPedCorr_left[i] = PedCorr;
        onl.ToF.AdcScaled_left[i] = PedCorr * rund.scint.ToF_corr_left_scale[i];
      }
      PedCorr = ToFpad[i].right.energy + rund.scint.ToF_corr_right_offset[i];
      //std::cout << PedCorr << std::endl;
      temp_Ped+=PedCorr;
      if ( PedCorr >= rund.scint.MinAdcOverThresholdValue) {
        onl.ToF.AdcPedCorr_right[i] = PedCorr;
        onl.ToF.AdcScaled_right[i] = PedCorr * rund.scint.ToF_corr_right_scale[i];
      }
      out->packEventData(&onl.ToF.AdcPedCorr_left[i], 1); 
      out->packEventData(&onl.ToF.AdcScaled_left[i], 1); 
      out->packEventData(&onl.ToF.AdcPedCorr_right[i], 1); 
      out->packEventData(&onl.ToF.AdcScaled_right[i], 1); 
      
      if (temp_Ped>0) dummy_counter+=1;

      double thisenergy = onl.ToF.AdcScaled_left[i] * onl.ToF.AdcScaled_right[i]; //should usually have isolated peak at zero
      if (thisenergy>0) { //implicitly fulfilled: ADC+Offset>=minADC for both left and right
	counter++;
	pattern |= number;
      }
 
      if (thisenergy>maxenergy) { 
	maxenergy = thisenergy;
	onl.ToF.Energy.left       = onl.ToF.AdcPedCorr_left[i]; //ped corrected, for walk correction
	onl.ToF.Energy.left_corr  = onl.ToF.AdcScaled_left[i];  //scaled, for particle ID
	onl.ToF.Energy.right       = onl.ToF.AdcPedCorr_right[i]; //ped corrected, for walk correction
	onl.ToF.Energy.right_corr  = onl.ToF.AdcScaled_right[i];  //scaled, for particle ID
	onl.ToF.paddle = i + 1;
	onl.ToF.scint = rund.scint.ToF[onl.ToF.paddle - 1];
	out->packEventData(&onl.ToF.Energy.left, 4);
	out->packEventData(&onl.ToF.paddle, 1);
	out->packEventData(&onl.ToF.scint, 1);
      }
    }
    number <<= 1;
  }
  
  onl.ToF.hits = counter;
  out->packEventData(&onl.ToF.hits, 1);
  onl.ToF.dummy_hits = dummy_counter;
  out->packEventData(&onl.ToF.dummy_hits, 1);
  onl.ToF.pattern = pattern;
  out->packEventData(&onl.ToF.pattern, 1);


  ///////////////////////
  // reset
  ///////////////////////

  number = 1;
  maxenergy = 0;
  counter = 0;

  ///////////////////////
  // dE A and C
  ///////////////////////

  if (!dE_B) {
    for (i=0; i<num_dEpad; i++)  {
      if (atree->itemOK(&dEpad[i].left.energy) && atree->itemOK(&dEpad[i].right.energy)) {
	onl.dE.AdcPedCorr_left[i] = onl.dE.AdcPedCorr_right[i] = 0.;
	onl.dE.AdcScaled_left[i] = onl.dE.AdcScaled_right[i] = 0.;


	double PedCorr = dEpad[i].left.energy + rund.scint.dE_corr_left_offset[i];
	if ( PedCorr >= rund.scint.MinAdcOverThresholdValue) {
	  onl.dE.AdcPedCorr_left[i] = PedCorr;
	  onl.dE.AdcScaled_left[i] = PedCorr * rund.scint.dE_corr_left_scale[i];
	}
	PedCorr = dEpad[i].right.energy + rund.scint.dE_corr_right_offset[i];
	if ( PedCorr >= rund.scint.MinAdcOverThresholdValue) {
	  onl.dE.AdcPedCorr_right[i] = PedCorr;
	  onl.dE.AdcScaled_right[i] = PedCorr * rund.scint.dE_corr_right_scale[i];
	}
	out->packEventData(&onl.dE.AdcPedCorr_left[i], 1); 
	out->packEventData(&onl.dE.AdcScaled_left[i], 1); 
	out->packEventData(&onl.dE.AdcPedCorr_right[i], 1); 
	out->packEventData(&onl.dE.AdcScaled_right[i], 1); 
	

	double thisenergy = onl.dE.AdcScaled_left[i] * onl.dE.AdcScaled_right[i]; //should usually have isolated peak at zero
	if (thisenergy>0) { //implicitly fulfilled: ADC+Offset>=minADC for both left and right
	  counter++;
	  pattern |= number;
	}


	if  (thisenergy>maxenergy) {
	  maxenergy = thisenergy;
	  onl.dE.Energy.left       = onl.dE.AdcPedCorr_left[i]; //ped corrected, for walk correction
	  onl.dE.Energy.left_corr  = onl.dE.AdcScaled_left[i];  //scaled, for particle ID
	  onl.dE.Energy.right       = onl.dE.AdcPedCorr_right[i]; //ped corrected, for walk correction
	  onl.dE.Energy.right_corr  = onl.dE.AdcScaled_right[i];  //scaled, for particle ID
	  

	  onl.dE.paddle = i + 1;
	  onl.dE.scint = rund.scint.dE[onl.dE.paddle - 1];
	  out->packEventData(&onl.dE.Energy.left, 4);
	  out->packEventData(&onl.dE.paddle, 1);
	  out->packEventData(&onl.dE.scint, 1);
	}
      }
      number <<= 1;
    }

  ///////////////////////
  // dE B
  ///////////////////////

  } else { // Special case for dE layer of Spectrometer B ! That's odd.
    for (i=0; i<num_dEpad; i++)  {
      if (atree->itemOK(&dE_B[i].energy))  {
	onl.dE.AdcPedCorr_left[i] = onl.dE.AdcPedCorr_right[i] = 0.;
	onl.dE.AdcScaled_left[i] = onl.dE.AdcScaled_right[i] = 0.;

	//using "right"- values only
	double PedCorr = dE_B[i].energy + rund.scint.dE_corr_right_offset[i];
	if ( PedCorr >= rund.scint.MinAdcOverThresholdValue) {
	  onl.dE.AdcPedCorr_right[i] = PedCorr;
	  onl.dE.AdcScaled_right[i] = PedCorr * rund.scint.dE_corr_right_scale[i];
	}
	out->packEventData(&onl.dE.AdcPedCorr_right[i], 1); 
	out->packEventData(&onl.dE.AdcScaled_right[i], 1); 


	double thisenergy = onl.dE.AdcScaled_right[i];
	if (thisenergy>0) { //implicitly fulfilled: ADC+Offset>=minADC
	  counter++;
	  pattern |= number;
	}



	if  (thisenergy>maxenergy) {
	  maxenergy = thisenergy;
	  //has only one PMT for each scintillator, two values are assigned here since in some .col files you find uncool things like
	  //Scalar spc(dE.Energy)    = sqrt(spec.dE.Energy.left*spec.dE.Energy.right)

	  onl.dE.Energy.right = onl.dE.Energy.left = onl.dE.AdcPedCorr_right[i]; //ped corrected, for walk correction;
	  onl.dE.Energy.right_corr = onl.dE.Energy.left_corr = onl.dE.AdcScaled_right[i];

	  onl.dE.paddle = i + 1;
	  onl.dE.scint = rund.scint.dE[onl.dE.paddle - 1];
	  out->packEventData(&onl.dE.Energy.left, 4);
	  out->packEventData(&onl.dE.paddle, 1);
	  out->packEventData(&onl.dE.scint, 1);
	}
      }
      number <<= 1;
    }
  }
  
  onl.dE.hits = counter;
  out->packEventData(&onl.dE.hits, 1);
  onl.dE.pattern = pattern;
  out->packEventData(&onl.dE.pattern, 1);
 
  if ( atree->itemOK(de_tof_time) && 
       out->itemOK(&onl.dE.paddle) && 
       out->itemOK(&onl.ToF.paddle)){
    //onl.raw_dE_ToF = *de_tof_time*rundb.tdc_resolution + rund.scint.walk[0];

    ////////////////////////
    // calling this raw_... can be very misleading somehow
    ////////////////////////

	onl.raw_dE_ToF = *de_tof_time*rund.de_tof_tdc_resolution + rund.scint.walk[0];
    //    cout << " resol = " << rund.de_tof_tdc_resolution << endl; 
    if (rund.scint.use_dE) onl.raw_dE_ToF *= -1;
    onl.raw_dE_ToF +=	rund.scint.ToF[onl.ToF.paddle - 1];
    onl.raw_dE_ToF -= rund.scint.dE [onl.dE.paddle  - 1];
    if (onl.ToF.Energy.right>0 && onl.dE.Energy.right>0) {
      onl.dE_ToF = onl.raw_dE_ToF-rund.scint.walk[1]/sqrt(onl.ToF.Energy.right)
	+ rund.scint.walk[2]/sqrt(onl.dE.Energy.right);
      out->packEventData(&onl.dE_ToF,     1);
    }
    out->packEventData(&onl.raw_dE_ToF, 1);
  }
  
  return pattern;
}
 
  
// Speed of light in Scintillator (in mm/ns)
// this is NOT consistent with  n=1.58 (index of refraction for scintillator)

const double C_SCINT    = 133.3; 
const double C_SCINT_DE	= 112.5;  // instead of 125.0
//const double C_C_SCINT	=  90.0;  // Spectrometer C seems to be strange
const double C_C_SCINT	= 133.3;  // Spectrometer C like A! (test)
   
double qsddaTimeShift(double y, struct online *onl)
{ if (rundb.A.scint.use_dE==0)
    return rundb.A.scint.ToF[onl->A.ToF.paddle-1] - y/C_SCINT;
  else  
    return rundb.A.scint.dE[onl->A.dE.paddle-1]  - y/C_SCINT_DE;
}

double qsddaLeftShift(double y, struct online *onl)
{ if (rundb.A.scint.use_dE==0)
    return rundb.A.scint.ToF_Left[onl->A.ToF.paddle-1] + 2*y/C_SCINT;
  else  
    return rundb.A.scint.dE_Left[onl->A.dE.paddle-1] + 2*y/C_SCINT_DE;
}

double bclamTimeShift(double y, struct online *onl)
{
  if (rundb.B.scint.use_dE==0)
    return rundb.B.scint.ToF[onl->B.ToF.paddle-1] - y/C_SCINT;
  else
    return rundb.B.scint.dE[onl->B.dE.paddle-1] - y/C_SCINT;
}

double bclamLeftShift(double y, struct online *onl)
{
  return rundb.B.scint.ToF[onl->B.ToF.paddle-1] 
    + rundb.B.scint.ToF_Left[onl->B.ToF.paddle-1] + 2*y/C_SCINT;
}

double qsddcTimeShift(double y, struct online *onl)
{
  if (rundb.C.scint.use_dE==0)
    return rundb.C.scint.ToF[onl->C.ToF.paddle-1] - y/C_C_SCINT;
  else  
    return rundb.C.scint.dE[onl->C.dE.paddle-1]  - y/C_SCINT_DE;
}

double qsddcLeftShift(double y, struct online *onl)
{
  if (rundb.C.scint.use_dE==0)
    return rundb.C.scint.ToF_Left[onl->C.ToF.paddle-1] + 2*y/C_C_SCINT;
  else  
    return rundb.C.scint.dE_Left[onl->C.dE.paddle-1]  + 2*y/C_SCINT_DE;
}

int qsddaScintIndex(int pattern)
{
  int i, min_index = 0;
  double min_offset = 1.0e99;

  if (rundb.A.scint.use_dE==0) {
    for(i=0; pattern; i++) {
      if ((pattern & 0x1) && (rundb.A.scint.ToF[i]<min_offset)) {
	min_offset = rundb.A.scint.ToF[i];
	min_index = i + 1;
      }
      pattern >>= 1;
    }
  }
  else {
    for(i=0; pattern; i++) {
      if ((pattern & 0x1) && (rundb.A.scint.dE[i]<min_offset)) {
	min_offset = rundb.A.scint.dE[i];
	min_index = i + 1;
      }
      pattern >>= 1;
    }
  }
  return min_index;
}

int qsddaScintIndex ( double x )
{
  if (rundb.A.scint.use_dE==0) {
    double pad = 1.0 + ( x + 188.6 ) / 158.3 + 1.0;
    if ( (pad<0.0) || (15.0<=pad) ) pad = 0.0;
    return (int)pad;
  }
  //dE has to be checked
  else {
    double pad = 4 + ( x - 70 ) / 159.3;
    if ( (pad<0.0) || (15.0<=pad) ) pad = 0.0;
    return (int)pad;
  }
}

int bclamScintIndex(int pattern)
{
  int i, min_index = 0;
  double min_offset = 1.0e99;

  for(i=1; pattern; i++) {
    if ((pattern & 0x1) && (rundb.B.scint.ToF[i]<min_offset)) {
      min_offset = rundb.B.scint.ToF[i];
      min_index = i;
    }
    pattern >>= 1;
  }

  return min_index;
}

int bclamScintIndex ( double x )
{
  double pad = 7.0 + ( x - 380.0 ) / 160.0;
  if ( (pad<1.0) || (15.0<=pad) ) pad = 1.0;
  return (int)pad;
}

int qsddcScintIndex(int pattern)
{
  int i, min_index = 0;
  double min_offset = 1.0e99;

  if (rundb.C.scint.use_dE==0) {
    for(i=0; pattern; i++) {
      if ((pattern & 0x1) && (rundb.C.scint.ToF[i]<min_offset)) {
	min_offset = rundb.C.scint.ToF[i];
	min_index = i;
      }
      pattern >>= 1;
    }
  }
  else {
    for(i=0; pattern; i++) {
      if ((pattern & 0x1) && (rundb.C.scint.dE[i]<min_offset)) {
	min_offset = rundb.C.scint.dE[i];
	min_index = i;
      }
      pattern >>= 1;
     }
  }
  return min_index;
}

// qsddcScintIndex has to be checked!
int qsddcScintIndex ( double x )
{
  if (rundb.C.scint.use_dE==0) {
//    double pad = 1.0 + ( x + 292.7 ) / 160.86;
    double pad = 2.0 + ( x + 134.2 ) / 159.65; // 30.09.99
    if ( (pad<0.0) || (15.0<=pad) ) pad = 0.0;
    return (int)pad;
  }
  //dE has to be checked
  else {
    double pad = 1.0 + ( x + 359.3 ) / 162.1;
    if ( (pad<0.0) || (15.0<=pad) ) pad = 0.0;
    return (int)pad;
  }
}



///////////////////////////////////////////////////////
// OLD VERSION:
//////////////////////////////////////////////////////

int scintillator_OLD(AquaTree  *atree,
		 AquaTree  *out,
		 struct Spectrometer &onl, 
                 struct SPECTROMETER &rund,
		 int num_dEpad, int num_ToFpad,
                 struct MpsPaddle *dEpad, 
                 struct MpsPaddle *ToFpad,
                 struct MpsPm *dE_B,
                 short *de_tof_time)
{
  //bssdel: std::cerr << "OLD SCINTraw scintillator routine" << std::endl;
  int pattern = 0, i;
  int counter = 0;

  onl.ToF.paddle      = onl.dE.paddle = -1;
  onl.ToF.scint       = onl.dE.scint  = 0;
  onl.ToF.Energy.left = onl.ToF.Energy.right = 0;
  onl.dE.Energy.left  = onl.dE.Energy.right = 0;

  double maxenergy = 0;
  if (!onl.trigger)  return 0;

  int number = 1;
  for (i=0; i<num_ToFpad; i++) {
    if (atree->itemOK(&ToFpad[i].left.energy)
	&& atree->itemOK(&ToFpad[i].right.energy)) {
      double thisenergy = ToFpad[i].left.energy * ToFpad[i].right.energy;
      if (thisenergy>maxenergy) {
	maxenergy = thisenergy;
	onl.ToF.Energy.right       = ToFpad[i].right.energy;
	onl.ToF.Energy.right_corr  = rund.scint.ToF_corr_right_offset[i] + 
	  ToFpad[i].right.energy * rund.scint.ToF_corr_right_scale[i];
	onl.ToF.Energy.left        = ToFpad[i].left.energy;
	onl.ToF.Energy.left_corr   = rund.scint.ToF_corr_left_offset[i] +
	  ToFpad[i].left.energy * rund.scint.ToF_corr_left_scale[i];
	onl.ToF.paddle = i + 1;
	onl.ToF.scint = rund.scint.ToF[onl.ToF.paddle - 1];
      }
      out->packEventData(&onl.ToF.Energy.left, 4);
      out->packEventData(&onl.ToF.paddle, 1);
      out->packEventData(&onl.ToF.scint, 1);
      counter++;
      pattern |= number;
    }
    number <<= 1;
  }
  
  onl.ToF.hits = counter;
  out->packEventData(&onl.ToF.hits, 1);
  onl.ToF.pattern = pattern;
  out->packEventData(&onl.ToF.pattern, 1);

  number = 1;
  maxenergy = 0;
  counter = 0;

  if (!dE_B) {
    for (i=0; i<num_dEpad; i++)  {
      if (atree->itemOK(&dEpad[i].left.energy) &&
	  atree->itemOK(&dEpad[i].right.energy)) {
	double thisenergy = dEpad[i].left.energy * dEpad[i].right.energy;
	if  (thisenergy>maxenergy) {
	  maxenergy = thisenergy;
	  onl.dE.Energy.right = dEpad[i].right.energy;
	  onl.dE.Energy.right_corr  = rund.scint.dE_corr_right_offset[i] + 
	    dEpad[i].right.energy * rund.scint.dE_corr_right_scale[i];
	  onl.dE.Energy.left = dEpad[i].left.energy;
	  onl.dE.Energy.left_corr = rund.scint.dE_corr_left_offset[i] +
	    dEpad[i].left.energy * rund.scint.dE_corr_left_scale[i];
	  onl.dE.paddle = i + 1;
	  onl.dE.scint = rund.scint.dE[onl.dE.paddle - 1];
	  out->packEventData(&onl.dE.Energy.left, 4);
	  out->packEventData(&onl.dE.paddle, 1);
	  out->packEventData(&onl.dE.scint, 1);
	}
	counter++;
	pattern |= number;
      }
      number <<= 1;
    }
  } else { // Special case for dE layer of Spectrometer B ! That's odd.
    for (i=0; i<num_dEpad; i++)  {
      if (atree->itemOK(&dE_B[i].energy))  {
	double thisenergy = dE_B[i].energy;
	if  (thisenergy>maxenergy) {
	  maxenergy = thisenergy;
	  onl.dE.Energy.right = dE_B[i].energy;
	  onl.dE.Energy.right_corr  = rund.scint.dE_corr_right_offset[i] + 
	    dE_B[i].energy * rund.scint.dE_corr_right_scale[i];
	  onl.dE.Energy.left = dE_B[i].energy;
	  onl.dE.Energy.left_corr = rund.scint.dE_corr_left_offset[i] +
	    dE_B[i].energy * rund.scint.dE_corr_left_scale[i];
	  onl.dE.paddle = i + 1;
	  onl.dE.scint = rund.scint.dE[onl.dE.paddle - 1];
	  out->packEventData(&onl.dE.Energy.left, 4);
	  out->packEventData(&onl.dE.paddle, 1);
	  out->packEventData(&onl.dE.scint, 1);
	}
	counter++;
	pattern |= number;
      }
      number <<= 1;
    }
  }
  
  onl.dE.hits = counter;
  out->packEventData(&onl.dE.hits, 1);
  onl.dE.pattern = pattern;
  out->packEventData(&onl.dE.pattern, 1);
 
  if ( atree->itemOK(de_tof_time) && 
       out->itemOK(&onl.dE.paddle) && 
       out->itemOK(&onl.ToF.paddle)){
    //onl.raw_dE_ToF = *de_tof_time*rundb.tdc_resolution + rund.scint.walk[0];
	onl.raw_dE_ToF = *de_tof_time*rund.de_tof_tdc_resolution + rund.scint.walk[0];
    //    cout << " resol = " << rund.de_tof_tdc_resolution << endl; 
    if (rund.scint.use_dE) onl.raw_dE_ToF *= -1;
    onl.raw_dE_ToF +=	rund.scint.ToF[onl.ToF.paddle - 1];
    onl.raw_dE_ToF -= rund.scint.dE [onl.dE.paddle  - 1];
    if (onl.ToF.Energy.right>0 && onl.dE.Energy.right>0) {
      onl.dE_ToF = onl.raw_dE_ToF-rund.scint.walk[1]/sqrt(onl.ToF.Energy.right)
	+ rund.scint.walk[2]/sqrt(onl.dE.Energy.right);
      out->packEventData(&onl.dE_ToF,     1);
    }
    out->packEventData(&onl.raw_dE_ToF, 1);
  }
  
  return pattern;
}
