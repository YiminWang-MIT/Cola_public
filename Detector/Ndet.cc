//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 2004
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//

#include "Ndet.h"

Ndet2016::Ndet2016(AquaTree *aquatree, AquaTree *outtree, AquaTRBsubevent **trbSubevent)
  : detectorBase(aquatree, outtree)
{
  aquaTrbSubevent = *trbSubevent;
}

Ndet2016::~Ndet2016()
{
  // Delete crap ...
}

int Ndet2016::init(char *mappingFile, char *calibrationFile)
{
  // make mapping and shit ...
  
  // if calibfile exists do calib, else
  setStandardCalibrationData();
  
  return 0;
}



int Ndet2016::handle()
{
  std::vector <channelHit> allHits;  
    
  trbTDCdata *dataPtr = NULL;
  int count = (aquaTrbSubevent)->getTrbTdcData(&dataPtr);
  
  int currentTdc = -1;
  int hitsPerChannel[NDET_CHANNELS_PER_TDC];
  for (int chn = 0; chn < NDET_CHANNELS_PER_TDC; chn++) {hitsPerChannel[chn] = 0;}
  int numberHitsCurrentTdc = 0;
  
  for (int i = 0; i < count; i++)
  {
    if (currentTdc == -1) {currentTdc = (int) dataPtr[i].trb * 4 + (int) dataPtr[i].tdc;}
    if (currentTdc !=  (int) dataPtr[i].trb * 4 + (int) dataPtr[i].tdc) // TODO letztes ereignis verarbeiten;
    { // treat TDC data
      for (int channel = 0; channel < 32; channel++)
      {
        // find first leading edge
       // double smallest_time = /*something small*/ ;
        int firstHit;
        for (int hit = 0; hit < numberHitsCurrentTdc; hit++)
        {
         // if ()
        }
      }
      
      // next tdc;
      numberHitsCurrentTdc = 0;
      currentTdc = (int) dataPtr[i].trb * 4 + (int) dataPtr[i].tdc;
      for (int chn = 0; chn < NDET_CHANNELS_PER_TDC; chn++) {hitsPerChannel[chn] = 0;}
    }
    
    if ((dataPtr[i].channel >= 0) && (dataPtr[i].channel < NDET_CHANNELS_PER_TDC)) {hitsPerChannel[dataPtr[i].channel]++;}
    
    allHits.push_back(tdc2pmtMapping((int) dataPtr[i].trb, (int) dataPtr[i].tdc,  dataPtr[i].channel));  
    
    
    
   /* std::cout << (unsigned long long)(dataPtr+i) << std::endl;
    std::cout << "  " << (int)dataPtr[i].trb
              << "\t" << (int)dataPtr[i].tdc
	      << "\t" << dataPtr[i].channel  << "\t" << dataPtr[i].epoch
	      << "\t" << dataPtr[i].coarse   << "\t" << dataPtr[i].fine << std::endl;*/

    /** Do analysis stuff here! */
    
  }
  delete dataPtr;
  return 0;
}



channelHit Ndet2016::tdc2pmtMapping(int trb, int tdc, int channel)
{
  int bar = -1;
  int pmt = -1;
  int edge = -1;
  
  if (channel % 2 == 0) {edge = trailing;}  
  else {edge = leading;}
  
  // if (trb = ???) {
    if (tdc == 0) {
      if ((channel == 49) || (channel == 50)) {bar =  0; pmt = top;}
      if ((channel == 51) || (channel == 52)) {bar =  2; pmt = top;}
      if ((channel == 53) || (channel == 54)) {bar =  4; pmt = top;}
      if ((channel == 55) || (channel == 56)) {bar =  6; pmt = top;}
      if ((channel == 57) || (channel == 58)) {bar =  8; pmt = top;}
      if ((channel == 59) || (channel == 60)) {bar = 10; pmt = top;}
      if ((channel == 61) || (channel == 62)) {bar = 12; pmt = top;}
      
      if ((channel == 17) || (channel == 18)) {bar =  0; pmt = bottom;}
      if ((channel == 19) || (channel == 20)) {bar =  2; pmt = bottom;}
      if ((channel == 21) || (channel == 22)) {bar =  4; pmt = bottom;}
      if ((channel == 23) || (channel == 24)) {bar =  6; pmt = bottom;}
      if ((channel == 25) || (channel == 26)) {bar =  8; pmt = bottom;}
      if ((channel == 27) || (channel == 28)) {bar = 10; pmt = bottom;}
      if ((channel == 29) || (channel == 30)) {bar = 12; pmt = bottom;}
    }
    
    if (tdc == 3) {
      if ((channel == 49) || (channel == 50)) {bar =  1; pmt = top;}
      if ((channel == 51) || (channel == 52)) {bar =  3; pmt = top;}
      if ((channel == 53) || (channel == 54)) {bar =  5; pmt = top;}
      if ((channel == 55) || (channel == 56)) {bar =  7; pmt = top;}
      if ((channel == 57) || (channel == 58)) {bar =  9; pmt = top;}
      if ((channel == 59) || (channel == 60)) {bar = 11; pmt = top;}
      if ((channel == 61) || (channel == 62)) {bar = 13; pmt = top;}
      
      if ((channel == 17) || (channel == 18)) {bar =  1; pmt = bottom;}
      if ((channel == 19) || (channel == 20)) {bar =  3; pmt = bottom;}
      if ((channel == 21) || (channel == 22)) {bar =  5; pmt = bottom;}
      if ((channel == 23) || (channel == 24)) {bar =  7; pmt = bottom;}
      if ((channel == 25) || (channel == 26)) {bar =  9; pmt = bottom;}
      if ((channel == 27) || (channel == 28)) {bar = 11; pmt = bottom;}
      if ((channel == 29) || (channel == 30)) {bar = 13; pmt = bottom;}
    }
    
    channelHit returnValue;
    returnValue.bar  =  bar;
    returnValue.pmt  =  pmt;
    returnValue.edge = edge;
    return returnValue;
}

int Ndet2016::loadCalibrationData()
{
  // TODO
  return 0;
}

int Ndet2016::setStandardCalibrationData()
{
  //timeCalibrationData.clear();
  
  double buffer[1024];
  for (int bin = 0; bin < 1024; bin++)
  {
    buffer[bin] = 5.0 * (bin / 1024.0);
  }
  
  for (int i = 0; i < NDET_NUMBER_TDC_CHANNELS; i++)
  {
    //timeCalibrationData.push_back(buffer);
  }
  return 0;
}

int Ndet2016::channelNumber(int trb, int tdc, int channel)
{
  int number = 0;
  number += channel;
  number += 65 * (tdc / 3); // WARNING temporary mapping for 2016 beamtime
  //number += trb * ??? // TODO
}
