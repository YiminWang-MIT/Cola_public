//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 2004
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
//

#include "detectorBase.h"
#include "KaosDetectors.h"
//#include <list>
#include <vector>

#define degree (180./M_PI)
#define KAOS_HADRON_TDC_RESOLUTION 0.025

class KaosMultiTrack : public KaosDetectors {

private:
  
  struct t_paddleHit
  {
    int paddleNo;
    double ADCMean;
    double TDCMean;
    double TDCDelta;
    // info for 2 paddle hits
    int twoPaddleHit;
    double ADCMeanAdditional;
    double TDCMeanAdditional;
    double TDCDeltaAdditional;
    double ADCAsymmetry;
    // info for next to 2 paddle hits
    double deltaTlower;
    double deltaThigher;
    double deltaYlower;
    double deltaYhigher;
  };
  
  /*struct t_wall
  {
    t_paddleHit hits[60];
    double      ind_thr;           // individual threshold
    int         N_hits;            // number of entries in array t_hit
    int         cnt_cluster;       // count clusters
  };*/
  
  struct t_track
  {
    t_paddleHit paddleHitH;
    t_paddleHit paddleHitG;
    t_paddleHit paddleHitI;
  };
  
//   t_wall wallH;
//   t_wall wallG;
//   t_wall wallI;
  
  t_group wallH;
  t_group wallG;
  t_group wallI;
  
  AquaNodeInt    *eventNumberTracks;
  AquaNodeInt    *eventMultiplicityH;
  AquaNodeInt    *eventMultiplicityG;
  
  AquaNodeInt    *trackID;
  AquaNodeDouble *trackPaddleH;
  AquaNodeDouble *trackPaddleG;
  AquaNodeInt    *track2PaddleHitH;
  AquaNodeInt    *track2PaddleHitG;
  
  AquaNodeDouble *trackPositionHx;
  AquaNodeDouble *trackPositionHy;
  AquaNodeDouble *trackPositionGx;
  AquaNodeDouble *trackPositionGy;

  AquaNodeDouble *trackDeltaHy;
  AquaNodeDouble *trackDeltaGy;
  
  AquaNodeDouble *trackTheta;
  AquaNodeDouble *trackPhi;
  
  AquaNodeDouble *trackLength;
  
  AquaNodeDouble *trackDedxH;
  AquaNodeDouble *trackDedxG;
  AquaNodeDouble *trackDeH;
  AquaNodeDouble *trackDeG;

  AquaNodeDouble *trackTimeH;
  AquaNodeDouble *trackTimeG;
  AquaNodeDouble *trackDeltaTimeH;
  AquaNodeDouble *trackDeltaTimeG;
  AquaNodeDouble *trackToF;
  
  AquaNodeInt    *trackAC1Segment;
  AquaNodeInt    *trackAC2Segment;
  
  AquaNodeInt    *trackNextToTwoPaddleHitH;
  AquaNodeInt    *trackNextToTwoPaddleHitG;
  AquaNodeDouble *trackDeltaTLowerH;
  AquaNodeDouble *trackDeltaTHigherH;
  AquaNodeDouble *trackDeltaYLowerH;
  AquaNodeDouble *trackDeltaYHigherH;
    
  AquaNodeDouble *trackDeltaTLowerG;
  AquaNodeDouble *trackDeltaTHigherG;
  AquaNodeDouble *trackDeltaYLowerG;
  AquaNodeDouble *trackDeltaYHigherG;

  AquaNodeInt    *eventMultiplicityI;
  AquaNodeDouble *trackPaddleI;
  AquaNodeInt    *track2PaddleHitI;
  AquaNodeDouble *trackPositionIx;
  AquaNodeDouble *trackPositionIy;
  AquaNodeDouble *trackDeltaIy;      
  AquaNodeDouble *trackDedxI;
  AquaNodeDouble *trackDeI;
  AquaNodeDouble *trackTimeI;
  AquaNodeDouble *trackDeltaTimeI;
  AquaNodeDouble *trackToF_IH;


  std::vector<t_track> *trackVector;
  int numberTracks;
  
public:
  int kaos_debug;

  KaosMultiTrack(AquaTree *aquatree, AquaTree *outtree);
  virtual ~KaosMultiTrack();
  virtual int handle();
 /* virtual int init(double *det_pos, double *track_tof, double * threshold_,
                   double *ADC_gain_, double *ADC_pedestal_,
                   double *ADC_correction_, 
                   double *TDC_offset_, double *TDC_correction_,
                   double *TDC_time_corr_, double *ADC_absorption_,
                   double *ADC_scale,
                   double *Aerogel_gain_, double *Aerogel_pedestal_,
                   double use_trigger_, double use_tof_, 
                   double use_tagger_, double use_mwpc_, double use_track_, 
                   double use_aerogel_, const char *filename);*/

 /* int fill_hit_array(AquaNodeDouble **T_TOP,
                     AquaNodeDouble **T_BOT, 
                     AquaNodeDouble **Q_TOP,
                     AquaNodeDouble **Q_BOT, 
                     double *time_offset, double *time_slope,
                     t_wall &wall, int NoOfPaddles);*/

//  int cnt_cluster(t_group &wall);

//  void get_cluster_data(t_group &wall, double threshold);


  //int check_trigger_three_walls(int minPaddleH, int maxPaddleH, int minPaddleG, int maxPaddleG, int minPaddleI, int maxPaddleI);
  //int check_trigger_three_walls(unsigned short paddleH, unsigned short paddleG);


  //int check_trigger(unsigned short paddleH, unsigned short paddleG);
} ;
