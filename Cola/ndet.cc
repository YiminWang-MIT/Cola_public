
//								      -*-c++-*-
// AQUA: data AcQUisition for A1 experiments
//
// Copyright (c) 2003-2013
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: ndet.cc 2519 2013-09-05 02:11:12Z distler $
//

#include <math.h>
#include <stdio.h>
#include "ndet.h"
#include "rundatabase.h"
#include <iostream>
#include <unistd.h>

#include "ndet.inc.cc"  //bss stuff
#include "ndet.init.cc"  //bss constructors, destructors and init functions, outsourced for better overview

//#define ___slowly___
//#define bss___debug1___

long int G_TdcUpPattern;
long int G_TdcDnPattern;



int NDetector::gmeanOK(int LayerToCheck, int BarToCheck)
{return layer[LayerToCheck]->gmeanOK(BarToCheck);}

int NDetector::TDCsignalFlag(int LayerToCheck, int BarToCheck)
{return layer[LayerToCheck]->TDCsignalFlag(BarToCheck, bestPaddle);}

int NDetector::handle()
{

  if (mAND_TdcAN && mAND_TdcAN->ok() && mAND_ATimeAtTarget && mAND_ATimeAtTarget->ok())
    mANDo_TimeOfTrigger->handle((*mAND_TdcAN-410)*0.1+*mAND_ATimeAtTarget);
  else mANDo_TimeOfTrigger->handle(-1);

  if (mAND_TdcAN && mAND_TdcAN->ok())
    std::cerr << *mAND_TdcAN;
  if (mAND_ATimeAtTarget && mAND_ATimeAtTarget->ok())
    std::cerr << "\t" << *mAND_ATimeAtTarget;
  if (mANDo_TimeOfTrigger && mANDo_TimeOfTrigger->ok())
    std::cerr << "\t" << *mANDo_TimeOfTrigger;
  std::cerr << std::endl;




  G_TdcUpPattern = 0;
  G_TdcDnPattern = 0;
  okFlags = 0;
  mAN_BestTdcMulti1Distance->handle(1E6);
  bestTiming = 1E6; bestPaddle = -1;
  sum_gmean=0;
  highest_gmean=0;
  highest_26gmean=0;
  dE->handle();
  number_TDCs=0;
  //bss  std::cerr << "NEW EVENT\n" ;
  for (int i=0; i<NDETlayers; i++) {
    layer[i]->handle();
    number_TDCs+=layer[i]->number_TDCs;
    int layerOK = layer[i]->ok();
    if (fabs((*layer[i]->mAN_BestTdcMulti1Distance)())<fabs((*mAN_BestTdcMulti1Distance)())) {
      //bss      std::cerr << "\tlayer" << i+1 << ":\t" << "layerOK=" << layerOK << ",\t fabs((*layer[" << i << "]->mAN_BestTdcMulti1Distance)())="<< fabs((*layer[i]->mAN_BestTdcMulti1Distance)()) << std::endl;
      mAN_BestTdcMulti1Distance->handle((*layer[i]->mAN_BestTdcMulti1Distance)());
    } ////////////////////////arghhhhhh

    // simple charged particle test - hit in the first layer
    if (i==0) charged->handle((0 != layerOK));
    if (layerOK) {
      okFlags |= (layerOK << (i*NDETbars));
      if (layer[i]->timing() < bestTiming) {
	bestTiming = layer[i]->timing();
	bestPaddle = layer[i]->paddle();
      }
    }
    sum_gmean +=layer[i]->get_sum_gmean();
    if (layer[i]->get_highest_gmean()>highest_gmean) {
      highest_gmean = layer[i]->get_highest_gmean();
    }
    //    if (i>0 && layer[i]->get_highest_gmean()>highest_26gmean 
    //	&& layer[i]->get_highest_gmean()>layer[0]->get_highest_gmean()) {
    if (i>0 && layer[i]->get_highest_gmean()>highest_26gmean) {
      highest_26gmean = layer[i]->get_highest_gmean();
    }

  }
    
  //  std::cout << (*mAN_BestTdcMulti1Distance)() << "    \tBEST IN NDET\n";
  //  std::cout << "##########################################################################\n\n\n";
  highest_ndet_gmean->handle(highest_gmean);
  highest_ndet_26gmean->handle(highest_26gmean);
  sum_ndet_gmean->handle(sum_gmean);
    
  particleType=-1;

  //'Nifty Parallel Count' of number of ones in an unsigned integer.
  unsigned int cnt = okFlags;
  cnt = (cnt & 0x55555555) + (cnt>>1 & 0x55555555);
  cnt = (cnt & 0x33333333) + (cnt>>2 & 0x33333333);
  cnt = (cnt & 0x0f0f0f0f) + (cnt>>4 & 0x0f0f0f0f);
  cnt = (cnt & 0x00ff00ff) + (cnt>>8 & 0x00ff00ff);
  cnt = (cnt & 0x0000ffff) + (cnt>>16& 0x0000ffff);
  multi->handle(cnt);

  //test OK:
  //unsigned int test = okFlags;
  //int mul=0;
  //for (int t=0; t<30; t++) if (test>>t & 0x1) mul++;
  //std::cout << cnt << ":" << mul << std::endl;
  
  
  //   std::cerr << cnt << "cnt\n";
  pattern->handle(okFlags);
  paddle->handle(bestPaddle);
  timeAtTarget->handle(bestPaddle<0 ? 0.0 : //bss: why this??
		       layer[bestPaddle/NDETlayers]->timeAtTarget());

  //fprintf(stderr, "NDET: multi %2d pattern 0x%08X 0x%08X\n",
  //        cnt, okFlags,(*pattern)());

  bsstools=0;
  if (getenv("BSSTOOLS") != NULL) bsstools=atoi(getenv("BSSTOOLS"));

  if (
      (getenv("BSSTOOLS") != NULL)  //useflag
      //      && sync_info_ndet 
      //      && sync_info_ndet->ok()        //ndet OK
      //      && !(*sync_info_ndet & 0x2000) //ndet singles
      ) 
    particleType=identify(cnt);

  //  particleType=identify(cnt);




#ifdef bss___debug1___
  if (number_TDCs == 0) {
    //bss temp_check->handle(0);
    std::cerr << "no TDCs";
    for (int i=0; i<NDETlayers; i++) 
      ;//layer[i]->cerr_adcs();
    //    usleep(3000000);
  } else ;//bss temp_check->handle(1);
  
  //std::cerr << "\n";
#endif
#ifdef ___slowly___
  //  usleep(200000);
#endif


  //PrintTdcPattern(G_TdcUpPattern, G_TdcDnPattern);


  return 0;
}


int NDetLayer::gmeanOK(int BarToCheck)
{
  return bar[BarToCheck]->gmeanOK();
}

int NDetLayer::TDCsignalFlag(int BarToCheck, int BestPaddle)
{
  return bar[BarToCheck]->TDCsignalFlag(BestPaddle, bestPaddle);
}

int NDetLayer::handle()//bss tempdouble TimeOfTrigger=-1)
{
  okFlags = 0;
  mAN_BestTdcMulti1Distance->handle(1E6);
  bestTiming = 1E6; bestPaddle = -1;
  highest_gmean=0; sum_gmean=0;
  number_TDCs = 0;
  //  for (int i=0; i<NDETbars; i++) {
  for (int i=NDETbars-1; i>=0; i--) {  //change order for simple plot output .xX...
    bar[i]->handle();//bss tempTimeOfTrigger);
    if (bar[i]->get_gmean() > highest_gmean) {
      highest_gmean = bar[i]->get_gmean();
    }
    sum_gmean+=bar[i]->get_gmean();
    if (bar[i]->ok()) {
      okFlags |= (0x1 << i);
      if (bar[i]->timing() < bestTiming) {
	bestTiming = bar[i]->timing();
	bestPaddle = bar[i]->id();
      }
      if (
	  fabs((*bar[i]->mAN_TdcMulti1Distance)())<fabs((*mAN_BestTdcMulti1Distance)())) {
	mAN_BestTdcMulti1Distance->handle((*bar[i]->mAN_TdcMulti1Distance)());
      }
    }
  }
  
  //  std::cout << (*mAN_BestTdcMulti1Distance)() << "    \tBEST IN LAYER\n\n";

  highest_layer_gmean->handle(highest_gmean);
  sum_layer_gmean->handle(sum_gmean);
  return 0;
}
  
double NDetLayer::get_highest_gmean()
{
  return *highest_layer_gmean;
}

double NDetLayer::get_sum_gmean()
{
  return *sum_layer_gmean;
}


int NDetBar::TDCsignalFlag(int BestPaddle, int BestPaddleLayer)  //0: n/a 1:ok 2:best paddle ndet 4:best paddle of layer 8:overflow
{
  int RV=0;
  if ((mIn_TdcUp && mIn_TdcUp->ok()) && (mIn_TdcDown && mIn_TdcDown->ok()) 
      && (((*mIn_TdcUp)() < 4095) && ((*mIn_TdcDown)() < 4095))) 
    RV |= 0x1;
  else if ((mIn_TdcUp && mIn_TdcUp->ok()) && (mIn_TdcDown && mIn_TdcDown->ok()) 
	   && (((*mIn_TdcUp)() >= 4095) && ((*mIn_TdcDown)() >= 4095)))
    RV |= 0x8;
  if (index==BestPaddleLayer) {
    RV |=0x4;
    if (index==BestPaddle) RV |= 0x2;
  }
  return RV;
}

int NDetBar::handle()
{
  okFlag = 0;
  gmeanOKFlag=0;
  double walkUp = 0.0;
  double walkDn = 0.0;
  double adc_corr_up = 0.0;
  double adc_corr_down = 0.0;

  mAN_AdcParticlePeakUp->handle(AdcParticlePeakUp);
  mAN_AdcParticlePeakDown->handle(AdcParticlePeakDown);
  mAN_AdcPedestalUp->handle(pedUp);mAN_AdcPedestalDown->handle(pedDown);
  mAN_GmeanParticlePeak->handle(sqrt((AdcParticlePeakUp-pedUp)*(AdcParticlePeakDown-pedDown)));
  //  std::cerr << *mAN_GmeanParticlePeak << std::endl;


  if ((mAN_AdcUp && mAN_AdcUp->ok()) && (mAN_AdcDown && mAN_AdcDown->ok())) {

    //bsstmp 
    adc_corr_up  =   ((*mAN_AdcUp)()    -pedUp) *300./(AdcParticlePeakUp  -pedUp);
    //bsstmp 
    adc_corr_down  = ((*mAN_AdcDown)()-pedDown) *300./(AdcParticlePeakDown-pedDown);
    //flasher calib: adc_corr_up  =   ((*mAN_AdcUp)()    -pedUp)*300./(flasherUp  -pedUp);
    //flasher calib: adc_corr_down  = ((*mAN_AdcDown)()-pedDown)*300./(flasherDown-pedDown);
    
    if (adc_corr_up<0.0) adc_corr_up = 0.0;
    if (adc_corr_down<0.0) adc_corr_down = 0.0;

    //    walkUp = 0.05*sqrt(up);
    //    walkDn = 0.05*sqrt(down);
    //    mAN_EUp->handle(adc_corr_up); mAN_EDown->handle(adc_corr_down);
    mAN_AdcUpCorr->handle(adc_corr_up); mAN_AdcDownCorr->handle(adc_corr_down);
    double gmtemp = sqrt(adc_corr_up*adc_corr_down);
    mAN_Gmean->handle(gmtemp>0?gmtemp:-1);
    //mAN_Gmean->handle(sqrt(adc_corr_up*adc_corr_down));
    if (*mAN_Gmean>1.2 * 300)
	gmeanOKFlag=3;      
    else if (*mAN_Gmean>1.0 * 300)
	gmeanOKFlag=2;      
    else if (*mAN_Gmean>0.8 * 300)
	gmeanOKFlag=1;      
    //    gmeanOKFlag=3;

  }

  //global TDC-pattern:
  int tmpbin = 1 << index; 
  //  if ((mIn_TdcUp && mIn_TdcUp->ok()) && (mIn_TdcDown && mIn_TdcDown->ok())) 
  //    std::cerr << "L" << la_yer+1 << "B" << ba_r+1 << "\t" << (*mIn_TdcUp)() << "\t" << (*mIn_TdcDown)() << std::endl;
  if ((mIn_TdcUp && mIn_TdcUp->ok()) 
      && ((*mIn_TdcUp)() < 4095) 
      && ((*mIn_TdcUp)() > 0)
      ) G_TdcUpPattern |=tmpbin;
  if ((mIn_TdcDown && mIn_TdcDown->ok()) 
      && ((*mIn_TdcDown)() < 4095)
      && ((*mIn_TdcDown)() > 0)
      ) G_TdcDnPattern |=tmpbin;

  
  if ((mIn_TdcUp && mIn_TdcUp->ok()) && (mIn_TdcDown && mIn_TdcDown->ok()) 
      && (((*mIn_TdcUp)() < 4095) && ((*mIn_TdcDown)() < 4095))
      //&& (((*mIn_TdcUp)() > 0) && ((*mIn_TdcDown)() > 0))  //added bss 2009-02-27
      && (((*mIn_TdcUp)() > 1) && ((*mIn_TdcDown)() > 1))  //changed bss 2009-08-21: several events where TDC readout did not work properly, many TDC=1; best paddle will be then one with small banana parameters -> does not make any sense to claim them as best paddles; those events remain with a besttiming of 10^6 then, TDC flags remain as OK, but a implicit cut can be done on besttime<X, X<10^6
      ) {
    mAN_TdcMulti1Distance->handle(TdcDistanceToMulti1Gauge(TdcMulti1GaugeX0, TdcMulti1GaugeY0, TdcMulti1GaugeR, (*mIn_TdcUp)(), (*mIn_TdcDown)()));
    //bss    std::cerr << "\t\tL" << la_yer+1 << "B" << ba_r+1 << "dist=" <<(TdcDistanceToMulti1Gauge(TdcMulti1GaugeX0, TdcMulti1GaugeY0, TdcMulti1GaugeR, (*mIn_TdcUp)(), (*mIn_TdcDown)())) << std::endl;
    //bss    std::cerr << "\t\t\t" <<TdcMulti1GaugeX0 << "\t" << TdcMulti1GaugeY0 << "\t" << TdcMulti1GaugeR << "\t" << (*mIn_TdcUp)() << "\t" << (*mIn_TdcDown)() << std::endl;
    //    std::cout << "TdcUp = " << (*mIn_TdcUp)() << ",\tTdcDn = " << (*mIn_TdcDown)() <<std::endl;
    //    std::cout << "x0    = " << TdcMulti1GaugeX0 << "\ty0    = " << TdcMulti1GaugeY0 << "R    = " << TdcMulti1GaugeR << std::endl;
    //    std::cout << "Distance = " 
    //	      << (*mAN_TdcMulti1Distance)()
    //	      << std::endl << std::endl;
	
    //    std::cout << (*mAN_TdcMulti1Distance)() << "    \tin bar\n";

      double up = 1.0*((*mIn_TdcUp)()+walkUp);
      double down = 1.0*((*mIn_TdcDown)()+walkDn);
      mAN_tUp->handle(0.05*(up-offsUp));
      mAN_tDown->handle(0.05*(down-offsDown));
      mAN_Position->handle(0.0025*(up-down-offsPos));
      okFlag = 1; //mod style: ok if and only if tdc signal ok


  } else if ((mAN_AdcUp && mAN_AdcUp->ok()) && (mAN_AdcDown && mAN_AdcDown->ok())
	     && adc_corr_up > 150. && adc_corr_down > 150.) {
    ;
    /*    std::cerr << ++ADC_TDC_mismatch 
	      << " \t" << index 
	      << " \tEUp=" << adc_corr_up << " \tEDown=" << adc_corr_down 
	      << " \tmIn_TdcUp="   << (mIn_TdcUp->ok()?*mIn_TdcUp:-1)
	      << " \tmIn_TdcDown=" << (mIn_TdcDown->ok()?*mIn_TdcDown:-1)
	      << std::endl;
    */
  }
  return 0;
}

int NDetBar::gmeanOK()
{
  return gmeanOKFlag;
}

double NDetBar::timing()
{
  //bss fabs makes no sense! Think of when another bar made the timing, the faster tdc can be zero then while the slower is positive
  //bss should be up>dn?up:dn
  //the more positive TDC minus offset (meaning the slower one) may have made the timing, not the faster one
  //if another bar made the timing, at least one of those numbers should be positive (the other one can in principle be negative)
  //if this bar made the timing, one of those numbers should be zero or both - sitting in the curve of the banana - 
  //should have small, negative numbers!
  return (fabs((*mAN_tUp)())<fabs((*mAN_tDown)()) ? (*mAN_tUp)() : (*mAN_tDown)());
}

double NDetBar::get_gmean()
{
  return *mAN_Gmean;
}

double NDetBar::timeAtTarget()
{
  return offsTime; // + (okFlag ? -2.5*fabs((*mAN_Position)()) : 0.0);
}

//bss start//////////////////////////////////////////////////


int NDetdE::Veto(int bar) 
{
  if (dEBar[bar]->mANIo_Veto && dEBar[bar]->mANIo_Veto->ok())
    return *dEBar[bar]->mANIo_Veto; else {std::cerr << "dE->Veto n/a\n"; exit(-69);}
}

int NDetdE::handle()
{
  for (int i=0; i<9; i++) {
    std::cerr << *mANS_Adc[i] << "\t" ;
    dEBar[i]->handle();
  }
  std::cerr << std::endl;
      
  int dETdcRange[9][2];
  dETdcRange[0][0]=1060; dETdcRange[0][1]=1300; //Bar1
  dETdcRange[1][0]=1080; dETdcRange[1][1]=1350; //Bar2
  dETdcRange[2][0]=1000; dETdcRange[2][1]=1260; //Bar3
  dETdcRange[3][0]=1040; dETdcRange[3][1]=1260; //Bar4
  dETdcRange[4][0]=1040; dETdcRange[4][1]=1300; //Bar5
  dETdcRange[5][0]=1060; dETdcRange[5][1]=1350; //Bar6
  dETdcRange[6][0]=1070; dETdcRange[6][1]=1320; //Bar7
  dETdcRange[7][0]=1000; dETdcRange[7][1]=1180; //Bar8
  dETdcRange[8][0]=1050; dETdcRange[8][1]=1330; //Bar9



  /*  
  for (int index = 8; index>=0; index--){
    //veto flags adc signal
    if (dEThres <*mAND_ECorr[index] )
      mANI_Veto[index]->handle(0x4);
    else if (dEThres <*mAND_ECorr[index] +100)
      mANI_Veto[index]->handle(0x2);
    else if (dEThres <*mAND_ECorr[index] +200)
      mANI_Veto[index]->handle(0x1);
    else
      mANI_Veto[index]->handle(0);
    
    //veto flags tdc signal
    if (mANS_Tdc[index] && mANS_Tdc[index]->ok()) {
      if (*mANS_Tdc[index]>dETdcRange[index][0] && *mANS_Tdc[index]<dETdcRange[index][1]) {
	mANI_Veto[index]->handle(*mANI_Veto[index]+0x20);
      }
      else
	mANI_Veto[index]->handle(*mANI_Veto[index]+0x10);
    }
  }     

  */



  return 0;
}


int NDetdEBar::handle()
{
  mANIo_Veto->handle(0);
  return 0;
}

//bss stop/////////////////////////////////////








