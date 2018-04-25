//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 2004
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
//

#ifndef __FIBRE_DETECTOR_H__
#define __FIBRE_DETECTOR_H__

#include "HMBook/hmbook.h"
#include "detectorBase.h"
#include "FibreEventDisplay.h"
#include <list>
#include <vector>
#ifdef CLUSTER_INFO
#include <iostream>
#include <fstream>
#endif

struct hit
{
  int channel;
  double time;
  double trailingtime;
  int type;
} ;

struct cluster
{
  int firstElement;       // first element of hits in cluster
  int lastElement;        // last element of hits in cluster
  int multiplicity;       // cluster multiplicity
  int leadingChannel;     // fibre with leading time
  double leadingTime;     // leading time of hit channel in ns
  double trailingTime;    // leading time of hit channel in ns
  int leadingTimeChannel; // channel with leading time = hit channel
  double centerTime;      // center time in ns
  double center;          // position of central channel
  double endTime;
  double RMSTime;         // RMS of cluster times in ns
  double meanTime;        // mean of cluster times in ns

  int width;              // width in channels
  double RMSlength;       // pulse length variation in ns
  double quality;         // quality without unit
  int lowestChannel;      // lowest channel of cluster
  int heighestChannel;    // heighest channel of cluster
} ;


class FibreDetector : public detectorBase {
protected:

// set-up of the detector
#define MAXHITS    300
#define MAX_PULSELENGTH 45

#ifndef FIBRECHNS
#define FIBRECHNS 4608
#endif

#define XCHNS     2304
#define THCHNS    2304

#define XPMTS     72
#define THPMTS    72
#define EXTCHNS    8

// pitch between fibre channels in mm
#define PITCH         0.8300 // nominal
#define XPITCH        0.8438 // as measured by Adrian Weber
#define THPITCH       0.8414 // as measured by Adrian Weber
// distance between X and Theta Layer in mm
#define LAYER_DIST     259.1
// parallel offset bewteen X and Theta layer in mm
#define X_TH_OFFSET    602.7

#define TDC_RESOLUTION 0.117712
#define TDC_ROLL       62054
#define TDC_ROLL_LIMIT 2000 // just a try... could be optimized 

// speed of light in mm / ns
#define LIGHTSPEED 299.792458

#define degree (180./M_PI)

  // raw data
  AquaNodeUShort *xtime[MAXHITS];
  AquaNodeUShort *xchn[MAXHITS];
  
  AquaNodeUShort *thtime[MAXHITS];
  AquaNodeUShort *thchn[MAXHITS];
  
  AquaNodeInt    *x_leadingChannel;
  AquaNodeDouble *x_leadingTime;
  AquaNodeDouble *x_trailingTime;
  AquaNodeInt    *x_minChannel; 
  AquaNodeInt    *x_maxChannel;
  AquaNodeInt    *x_hitMultiplicity;  
  AquaNodeInt    *x_totalMultiplicity;
  AquaNodeInt    *x_numberClusters;
  AquaNodeInt    *x_typeCluster;
  AquaNodeInt    *x_typeAfterpulse;
  AquaNodeInt    *x_typeSingles;
  AquaNodeInt    *x_channels;
  AquaNodeDouble *x_times;
  AquaNodeDouble *x_timeRMS;
  AquaNodeDouble *x_timeMean;
  AquaNodeDouble *x_lengthRMS;
  
  AquaNodeInt    *theta_leadingChannel;
  AquaNodeDouble *theta_leadingTime;
  AquaNodeDouble *theta_trailingTime;
  AquaNodeInt    *theta_minChannel; 
  AquaNodeInt    *theta_maxChannel;
  AquaNodeInt    *theta_hitMultiplicity;  
  AquaNodeInt    *theta_totalMultiplicity;
  AquaNodeInt    *theta_numberClusters;
  AquaNodeInt    *theta_typeCluster;
  AquaNodeInt    *theta_typeAfterpulse;
  AquaNodeInt    *theta_typeSingles;
  AquaNodeInt    *theta_channels;
  AquaNodeDouble *theta_times;
  AquaNodeDouble *theta_timeRMS;    
  AquaNodeDouble *theta_timeMean;    
  AquaNodeDouble *theta_lengthRMS;
  
  AquaNodeInt    *x_pmtSingles[XPMTS];
  AquaNodeInt    *x_pmtAfterpulses[XPMTS];
  AquaNodeInt    *x_pmtClusterHits[XPMTS];
  
  AquaNodeInt    *theta_pmtSingles[XPMTS];
  AquaNodeInt    *theta_pmtAfterpulses[XPMTS];
  AquaNodeInt    *theta_pmtClusterHits[XPMTS];
  
  AquaNodeInt    *track_number_tracks;
  AquaNodeDouble *track_angle;
  AquaNodeDouble *track_flightpath;
  AquaNodeDouble *track_quality;
    
  // trigger subtracted, cluster corrected and mapped time spectra
  AquaNodeDouble *x_TDC[XCHNS]; 
  AquaNodeDouble *th_TDC[THCHNS]; 
  AquaNodeDouble *ext_TDC[EXTCHNS]; 

  HIST * FibreNrX, * FibreTimeX;
  HIST * FibreNrTheta, * FibreTimeTheta;
  HIST * TrackFibreNrX, * TrackFibreTimeX;
  HIST * TrackFibreNrTheta, * TrackFibreTimeTheta;

  HIST * ThresholdChannelX, * TrackThresholdChannelX;;
  HIST * ThresholdChannelTheta, * TrackThresholdChannelTheta;

  // TDC calibration
  double TDC_offset[FIBRECHNS];
  int    TriggerChannel, HadronChannel, ElectronChannel;
  double TriggerOffset;

public:
  FibreDetector(AquaTree *aquatree, AquaTree *outtree);
  virtual ~FibreDetector();
  
  virtual int init(double * TDC_offset_,
		   double * trigger_,
		   double * analysisParamerters_,
		   double * eventDisplay_);
  virtual int handle();
  
private:
  
  FibreEventDisplay *EventDisplay;
  
  int fibre_debug;
  void clusterFind(std::list<hit> * hitList, std::list<hit> * hitListTh);
  int pmt2thresholdChannel(int pmtCh);
  int minClusterSize;
  int clusterSeparationChannels;
  double clusterSeparationTime;
  double minAngle;
  double maxAngle;
  int eventDisplayPages;
  double eventDisplayMinTime;
  double eventDisplayMaxTime;
  
};

#endif /* __FIBRE_DETECTOR_H__ */
