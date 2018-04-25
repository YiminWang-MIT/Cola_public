//
// $Id: Cerenkov.cc 2664 2016-02-01 09:39:51Z schlimme $
//

//
// $Id: Cerenkov.cc 2664 2016-02-01 09:39:51Z schlimme $
//

#include "rundatabase.h"       // Run Database
#include "Cerenkov.h"
#include <cstdlib>
#include <cmath>
#include <iostream>
void 
cerenkovAC::GetPedestalsMax()
{
  int PedestalsMax[12]; 
  float PedestalsMaxAverage[12];
  float PedSum, NormSum;

  for (int i=0; i<12; i++) PedestalsMax[i] = 1;    
  
  for (int j=0; j<12; j++) {
    for (int i=1; i<1000; i++) {
      if (CheckPedestalsAC[j][i]>CheckPedestalsAC[j][PedestalsMax[j]])    
	PedestalsMax[j] = i;
    }
    
    PedSum = NormSum = 0;

    for (int i=-2; i<3; i++) {
      PedSum += CheckPedestalsAC[j][PedestalsMax[j]-i] * (PedestalsMax[j]-i);
      NormSum += CheckPedestalsAC[j][PedestalsMax[j]-i];
    }
    if (NormSum!=0) PedestalsMaxAverage[j] = PedSum / NormSum;
    else PedestalsMaxAverage[j] =0;
  }
  

  if (out) {
    for (int i=0; i<12;i++) {
      fprintf(out,"%f ",PedestalsMaxAverage[i]); 
    }
    fprintf(out,"\n");
    fflush(out);
  }

  for (int j=0; j<12;j++) for (int i=1; i<1000;i++) CheckPedestalsAC[j][i] = 0;
}  
  
void 
cerenkovAC::LoadPedestals()
{
  char Input[20];
  
  if (in)
    for (int i=0; i<12; i++) {
      if (fscanf(in,"%s ", Input)); 
      if (atof(Input) != 0) Pedestals[i] = atof(Input);
    }
}

void 
cerenkovAC::rawdata(AquaTree *atree, vdcBase *vdc, int nr)
{
  onl->sum = onl->hit = 0;
  for (int i=0; i<12; i++)
    onl->FloatedPed[i] = 0;


  if (rundbcer->FloatingPed==1 && !(nr % (int)rundbcer->FloatingEvents)
      && nr>0) GetPedestalsMax();

  if (rundbcer->FloatingPed>=2 && rundbcer->FloatingPed!=17&& (!(nr % (int)rundbcer->FloatingEvents)
				   || nr==0)) LoadPedestals();
  
  for (int i=0; i<12; i++)
    if (atree->itemOK(&(cer->mirror[i].energy))) {
      double erg = cer->mirror[i].energy;
      
      if (rundbcer->FloatingPed==1 && erg<1000 && erg>0 && 
	  vdc->vdcPacket()->ok1()==PACKET_STATUS_BOTH) {
	
	CheckPedestalsAC[i][(int)erg]++;
      }
      
      double cor = 0;
     
      //New possible value for FloaingPed added by Y.Kohl and A.Weber in August 2013 during ISR Beamtime. This was necessary because the Hardware correction for the pedestals in A did not work and the pedestal correction needed to be done after the measurement. 
      //To use this you set the value for A.Cerenkov.FloatingPed = 17 in the run.db. The value by which you want to correct can be set with A.Cerenkov.pedestal. If you also need to scale, you can use A.Cerenkov.scale

      if (rundbcer->FloatingPed==17) {	
	//Calculate the corrected value:
	//OLD: cor = -rundbcer->pedestal[i] + rundbcer->scale[i] * erg;
	//bss intermediate: cor = (erg - rundbcer->pedestal[i]) * rundbcer->scale[i];
	cor = (erg - rundbcer->pedestal[i]); //pedestal shifted
	if(cor<=5) //The Hardware correction also sets everything to zero which is less than 5, so this is done here, too.
	  cor=0;
	else
	  cor *= rundbcer->scale[i];

	//The correction now contains values which are less than zero, to get rid of these, they are set to zero. 
	//To see the correction for each channel, the corrected value is filled into the already existing FloatedPed variable:
	onl->FloatedPed[i] = cor;	
      }
      else{
	if (rundbcer->FloatingPed==0) {
	  cor = rundbcer->offset[i] + rundbcer->scale[i] * erg;
	  onl->FloatedPed[i] = 0;
	}
	else {
	  if (rundbcer->FloatingPed==3) {
	    cor = rundbcer->scale[i] * erg / Pedestals[i];
	    onl->FloatedPed[i] = cor;
	  }
	  else {
	    cor = -(Pedestals[i]) + rundbcer->scale[i] * erg;
	    onl->FloatedPed[i] = cor;
	  }
	}
      }

      onl->sum += cor;
 
      //The hit variable is only filled, if the corrected value is bigger or equal to the set pedestal
      if (rundbcer->FloatingPed==17) { //bss: correct for pedestal and checking to be above is double bad; ->exception for FP==17
	if (cor>0) onl->hit++;

      } else if (cor >= rundbcer->pedestal[i]) onl->hit++; //bss this was before


      
    }
}

void 
cerenkovB::GetPedestalsMax()
{
  int PedestalsMax[5]; 
  float PedestalsMaxAverage[5];
  float PedSum, NormSum;

  for (int i=0; i<5; i++) PedestalsMax[i] = 1;    
  
  for (int j=0; j<5; j++) {
    for (int i=1; i<1000; i++) {
      if (CheckPedestalsB[j][i]>CheckPedestalsB[j][PedestalsMax[j]])         
	PedestalsMax[j] = i;
    }
    
    PedSum = NormSum = 0;

    for (int i=-2; i<3; i++) {
      PedSum += CheckPedestalsB[j][PedestalsMax[j]-i] * (PedestalsMax[j]-i);
      NormSum += CheckPedestalsB[j][PedestalsMax[j]-i];
    }
    if (NormSum!=0) PedestalsMaxAverage[j] = PedSum / NormSum;
    else PedestalsMaxAverage[j] = 0;
  }
  

  if (out) {
    for (int i=0; i<5;i++) {
      fprintf(out,"%f ",PedestalsMaxAverage[i]); 
    }
    fprintf(out,"\n");
    fflush(out);
  }

  for (int j=0; j<5;j++) for (int i=1; i<1000;i++) CheckPedestalsB[j][i] = 0;
}  
  
void 
cerenkovB::LoadPedestals()
{
  char Input[20];

  if (in)
    for (int i=0; i<5;i++) {
      if (fscanf(in,"%s ", Input)); 
      if (atof(Input) != 0) Pedestals[i] = atof(Input);
    }
}

void 
cerenkovB::rawdata(AquaTree *atree, vdcBase *vdc, int nr)
{
  onl->sum = onl->hit = 0;
  for (int i=0; i<5; i++)
    onl->FloatedPed[i] = 0;


  if (rundbcer->FloatingPed==1 && !(nr % (int)rundbcer->FloatingEvents) 
      && (nr>0)) GetPedestalsMax();

  if (rundbcer->FloatingPed>=2 && rundbcer->FloatingPed!=17 && (!(nr % (int)rundbcer->FloatingEvents)
				   || nr==0)) LoadPedestals();
  
  for (int i=0; i<5; i++)
    if (atree->itemOK(&(cer->mirror[i].energy))) {
      double erg = cer->mirror[i].energy;
      
      if (rundbcer->FloatingPed==1 && erg<1000 && erg>0 && 
	  vdc->vdcPacket()->ok1()==PACKET_STATUS_BOTH) {
	CheckPedestalsB[i][(int)erg]++;
      }
      double cor = 0;
      if (rundbcer->FloatingPed==0) {
	cor = rundbcer->offset[i] + rundbcer->scale[i] * erg;
	onl->FloatedPed[i] = 0;
      }
      else {
	if (rundbcer->FloatingPed==3) {
	  cor = rundbcer->scale[i] * erg / Pedestals[i];
	  onl->FloatedPed[i] = cor;
	}
        if (rundbcer->FloatingPed==17) {  // same as for FloatingPed == 0, but values that are after the offset correction smaller than 0 are set to 0 so that they do not contribute to the energysum, because mirror values smaller than 0 do not contribute without the offset correction
	  //OLD: cor = -rundbcer->pedestal[i] + rundbcer->scale[i] * erg;
	  //bss intermediate: cor = (erg - rundbcer->pedestal[i]) * rundbcer->scale[i];
	  cor = (erg - rundbcer->pedestal[i]); //pedestal shifted
	  if ( cor <= 5)  //The Hardware correction also sets everything to zero which is less than 5, so this is done here, too.
	    cor = 0;
	  else cor *=  rundbcer->scale[i];
	  onl->FloatedPed[i] = cor;
        }    
	else {
	  cor = -(Pedestals[i]) + rundbcer->scale[i] * erg;
	  onl->FloatedPed[i] = cor;
	}
      }
      
      onl->sum += cor;
      //The hit variable is only filled, if the corrected value is bigger or equal to the set pedestal
      if (rundbcer->FloatingPed==17) { //bss: correct for pedestal and checking to be above is double bad; ->exception for FP==17
	if (cor>0) onl->hit++;

      } else if (cor >= rundbcer->pedestal[i]) onl->hit++; //bss this was before


    }
}
