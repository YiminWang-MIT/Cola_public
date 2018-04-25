//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 2008
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: FlashADC.cc 2354 2009-11-13 13:58:04Z merkel $
//

#include "Aqua/AquaTree.h"
#include "FlashADC.h"
#include <cmath>
#include <cstdio>
#include <iostream>

FlashADC::FlashADC(AquaTree *aquatree, AquaTree *outtree)
  : detectorBase(aquatree, outtree)
{
  AquaNode **smp = (AquaNode **)samples_ch[0];
  aquatree->multifind(smp, 0, fADC_samples-1, ".si.V1724.chan[0].sample[%d]"); 
  
  smp = (AquaNode **)samples_ch[1];
  aquatree->multifind(smp, 0, fADC_samples-1, ".si.V1724.chan[1].sample[%d]"); 
  
  smp = (AquaNode **)samples_ch[2];
  aquatree->multifind(smp, 0, fADC_samples-1, ".si.V1724.chan[2].sample[%d]"); 
  
  smp = (AquaNode **)samples_ch[3];
  aquatree->multifind(smp, 0, fADC_samples-1, ".si.V1724.chan[3].sample[%d]"); 
  
  smp = (AquaNode **)samples_ch[4];
  aquatree->multifind(smp, 0, fADC_samples-1, ".si.V1724.chan[4].sample[%d]"); 
  
  smp = (AquaNode **)samples_ch[5];
  aquatree->multifind(smp, 0, fADC_samples-1, ".si.V1724.chan[5].sample[%d]"); 
 
  AquaNode **mmm = (AquaNode **)maximum;
  aquatree->multifind(mmm, 0, fADC_channels-1, ".si.V1724.chan[%d].maximum"); 

  AquaNode **eng = (AquaNode **)E_ch;
  online->multifind(eng, 0, 5, "online.si.v1724energy[%d]");

  init();
}

FlashADC::~FlashADC()
{
  ;
}

int FlashADC::handle(){

    Lmin = RT + FT_min; //minimum width of the shaping window [samples]
    Lmax = RT + FT_max; //maximum width of the shaping window [samples]
    
    Smin = 0; //signal minimum for sliding window determination
    Smax = 3000; //signal maximum for sliding window determination

    Estart = 30; //first sample after Sstart for energy integration
    Estop = 0; //last sample for energy calculation before the end of the flat top


    for (int k=0; k<fADC_channels; k++){
	// --- Reset variables ---
	baseline_s[k] = 0; baseline_t[k] = 0;
	d[k] = 0; s[k] = 0; pprime[k] = 0;
	Ltmp[k] = 0; L[k] = 0; Epts[k] = 0;
	Etmp[k] = 0.0;
	threshold[k] = false;
	trigger_mark[k] = Sstart;
    
	// --- Calculate signal base line ---
	for (int n=0; n<4; n++) baseline_s[k] += *samples_ch[k][n];
	baseline_s[k] = baseline_s[k]/4;

	// --- Sliding window determination ---
	Ltmp[k]=(double)(Lmax-Lmin)/(Smax-Smin) * (fabs(*maximum[k])-Smin) + Lmin; //window width for MA calculation 
	if (Ltmp[k]<Lmin) Ltmp[k]=Lmin;
	else if (Ltmp[k]>Lmax) Ltmp[k]=Lmax;
	L[k]=(int)(Ltmp[k]+0.5); //0.5 is for the correct round off
	Epts[k] = L[k] - Estart - Estop; //number of samples on flat top for energy calculation
    }
    
    //Oscillogram loop
    for (int n=0; n<fADC_samples*2; n++){  
	
	for (int k=0; k<fADC_channels; k++){
	 
	    //Fill in the dummy samples for trapezoid calculation
	    if (n<fADC_samples) smpl[k][n] = baseline_s[k];
 	    
	    //Get the real samples
	    else smpl[k][n] = *samples_ch[k][n-fADC_samples];
	    
	    // --- Trapezoid calculation ---
	    if (n-L[k]-RT<0) d[k] = 0;
	    else d[k] = smpl[k][n] - smpl[k][n-RT] - smpl[k][n-L[k]] + smpl[k][n-L[k]-RT];
	    pprime[k] += d[k];
	    s[k] += pprime[k] + d[k]*M;
	    
	    // --- Trapezoid signal ---
	    trapezoid[k][n]=s[k];
	  
	    // --- Pile-up control or reject ---
	    if (pileup_ctrl != 0){
		
		if (n >= Sstart+Estart+4 && n < Sstart+Estart+Lmax){
		    threshold[k] = (trapezoid[k][n]-trapezoid[k][n-2]) < pileup_thr; //Set the threshold for each channel
		    
		    if (pileup_ctrl == 1){
			//Wait at least Estart samples before next trigger
			if (threshold[k] == true && n > trigger_mark[k] + Estart + 4){
			    trigger_mark[k]=n; //Mark the trigger sample
			    //Plot the trigger point (for debugging)
			    //if (trigger_mark[k] < Sstart+Estart+Epts[k]) trapezoid[k][trigger_mark[k]]+=1e8;
			    // --- Pile-up control ---
			    if (trigger_mark[k] < Sstart+Estart+Epts[k]) Epts[k] -= (Sstart+Estart+Epts[k]-trigger_mark[k]);
			}
		    }
		    if (pileup_ctrl == 2 && k==0){
			//Wait at least Estart samples before next trigger
			if (threshold[0] == true && n > trigger_mark[0] + Estart + 4){
			    trigger_mark[0]=n; //Mark the trigger sample
			    //Plot the trigger point (for debuging)
			    //if (trigger_mark[0] < Sstart+Estart+Epts[0]) trapezoid[k][trigger_mark[0]]+=1e8;
			    // --- Pile-up reject ---			    
			    if (trigger_mark[0] < Sstart+Estart+Epts[0]) return 0;

			}	
		    }
		}
	    }

	    // --- Trapezoid base line calculation ---
	    if (n >= Sstart-BL_position-BL_points && n < Sstart-BL_position) baseline_t[k] += trapezoid[k][n];
	   
	    // --- Trapezoid energy calculation ---
	    if (n >= Sstart+Estart && n < Sstart+Estart+Epts[k]) Etmp[k] += trapezoid[k][n];
	    if (n == (Sstart+Lmax)){
	        Etmp[k] *= 1.0/Epts[k];
		Etmp[k] -= 1.0*baseline_t[k]/(BL_points);

		//Handle the calculated energy
		E_ch[k]->handle(Etmp[k]*16383/M/RT/1e4);
	    }
	}
    }
   
    //DEBUG OUTPUT//////////////
    if (fADC_osc){
	printSamples();
    }
    ////////////////////////////
    
    return 0;
    
}

int FlashADC::init(double M_, double RT_, double FT_min_, double FT_max_, \
		   double Sstart_, double pileup_ctrl_, double pileup_thr_)
{
  M=M_; RT=RT_; FT_min=FT_min_; FT_max=FT_max_; Sstart=Sstart_;
  pileup_ctrl = pileup_ctrl_; pileup_thr = pileup_thr_; 
  Sstart += 400; //add 400 dummy samples before actual samples 
  return 0;
}

int FlashADC::printSamples()
{
    for (int i=0; i<fADC_samples*2; i++){
	for (int j=0; j<6;j++){
	    printf("%d\t %d\t",smpl[j][i], trapezoid[j][i]);
	}
	printf("\n");
    }
    return 0;
}
