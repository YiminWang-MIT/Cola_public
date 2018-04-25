//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 2004
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: KaosDetectors.cc 2608 2014-11-26 15:09:05Z patrick $
//

#include "Aqua/AquaTree.h"
#include "KaosDetectors.h"
#include <cmath>
#include <fstream>
#include <iostream>

#undef EXTRAPOLATE_MISSING_CHANNELS

#undef SIMPLE_TDC_EXTRAPOLATION

#undef INCREASE_DOUBLE_HIT_RECOGNITION

//#define DEBUG_KAOS_TOF_QUALITY
//#define DEBUG_KAOS_CALIB
//#define DEBUG_KAOS_TOF
//#define DEBUG_KAOS_TOF_HIT_ARRAY
//#define DEBUG_KAOS_TOF_CLUSTER
//#define DEBUG_KAOS_AEROGEL

KaosDetectors::KaosDetectors(AquaTree *aquatree, AquaTree *outtree)
  : detectorBase(aquatree, outtree)
{
  kaos_debug = 0;  // DEBUGGING OUTPUT {0|1|2|3}

  AquaNode **Ntdcft = (AquaNode **)tdcft;
  atree->multifind(Ntdcft, 0, 29, "abcn.kaos.tof.fdet[%d].top.time");
  AquaNode **Ntdcfb = (AquaNode **)tdcfb;
  atree->multifind(Ntdcfb, 0, 29, "abcn.kaos.tof.fdet[%d].bottom.time");
  AquaNode **Nadcft = (AquaNode **)adcft;
  atree->multifind(Nadcft, 0, 29, "abcn.kaos.tof.fdet[%d].top.charge");
  AquaNode **Nadcfb = (AquaNode **)adcfb;
  atree->multifind(Nadcfb, 0, 29, "abcn.kaos.tof.fdet[%d].bottom.charge");

  AquaNode **Ntdcgt = (AquaNode **)tdcgt;
  atree->multifind(Ntdcgt, 0, 29, "abcn.kaos.tof.gdet[%d].top.time");
  AquaNode **Ntdcgb = (AquaNode **)tdcgb;
  atree->multifind(Ntdcgb, 0, 29, "abcn.kaos.tof.gdet[%d].bottom.time");
  AquaNode **Nadcgt = (AquaNode **)adcgt;
  atree->multifind(Nadcgt, 0, 29, "abcn.kaos.tof.gdet[%d].top.charge");
  AquaNode **Nadcgb = (AquaNode **)adcgb;
  atree->multifind(Nadcgb, 0, 29, "abcn.kaos.tof.gdet[%d].bottom.charge");

  AquaNode **Nadcat = (AquaNode **)adcat;
  atree->multifind(Nadcat, 0, 11, "abcn.kaos.aerogel.segment[%d].top.charge");
  AquaNode **Nadcab = (AquaNode **)adcab;
  atree->multifind(Nadcab, 0, 11, "abcn.kaos.aerogel.segment[%d].bottom.charge");

  AquaNode **Ntdcx = (AquaNode **)tdcx;
  atree->multifind(Ntdcx, 0, 7, "abcn.kaos.tof.xtra[%d].time");

  AquaNode **Nadcx = (AquaNode **)adcx;
  atree->multifind(Nadcx, 0, 7, "abcn.kaos.tof.xtra[%d].charge");

  clusterSizeH  = (AquaNodeInt *) online->find("online.kaos.hdet.clusterSize");
  clusterSizeG  = (AquaNodeInt *) online->find("online.kaos.gdet.clusterSize");
  clusterSizeI  = (AquaNodeInt *) online->find("online.kaos.idet.clusterSize");

  xpositionH    = (AquaNodeDouble *) online->find("online.kaos.hdet.positionXZ");
  xpositionG    = (AquaNodeDouble *) online->find("online.kaos.gdet.positionXZ");
  xpositionI    = (AquaNodeDouble *) online->find("online.kaos.idet.positionXZ");

  paddleH       = (AquaNodeDouble *) online->find("online.kaos.hdet.paddle");
  paddleG       = (AquaNodeDouble *) online->find("online.kaos.gdet.paddle");
  paddleI       = (AquaNodeDouble *) online->find("online.kaos.idet.paddle");

  ypositionH    = (AquaNodeDouble *) online->find("online.kaos.hdet.positionY");
  ypositionG    = (AquaNodeDouble *) online->find("online.kaos.gdet.positionY");
  ypositionI    = (AquaNodeDouble *) online->find("online.kaos.idet.positionY");

  timeH    = (AquaNodeDouble *) online->find("online.kaos.hdet.time");
  timeG    = (AquaNodeDouble *) online->find("online.kaos.gdet.time");
  timeI    = (AquaNodeDouble *) online->find("online.kaos.idet.time");

  deltaTimeClusterH = (AquaNodeDouble *) online->find("online.kaos.hdet.deltaTimeCluster");
  deltaTimeClusterG = (AquaNodeDouble *) online->find("online.kaos.gdet.deltaTimeCluster");
  deltaTimeClusterI = (AquaNodeDouble *) online->find("online.kaos.idet.deltaTimeCluster");
  
  chargeH  = (AquaNodeDouble *) online->find("online.kaos.hdet.dE");
  chargeG  = (AquaNodeDouble *) online->find("online.kaos.gdet.dE");
  chargeI  = (AquaNodeDouble *) online->find("online.kaos.idet.dE");

  dEdxH    = (AquaNodeDouble *) online->find("online.kaos.hdet.dEdx");
  dEdxG    = (AquaNodeDouble *) online->find("online.kaos.gdet.dEdx");
  dEdxI    = (AquaNodeDouble *) online->find("online.kaos.idet.dEdx");

  clusterH = (AquaNodeInt *) online->find("online.kaos.hdet.cluster");
  clusterG = (AquaNodeInt *) online->find("online.kaos.gdet.cluster");
  clusterI = (AquaNodeInt *) online->find("online.kaos.idet.cluster");

  //  lengthH  = (AquaNodeDouble *) online->find("online.kaos.hdet.length");
  //  lengthG  = (AquaNodeDouble *) online->find("online.kaos.gdet.length");
  //  lengthI  = (AquaNodeDouble *) online->find("online.kaos.idet.length");

  theta_tof  = (AquaNodeDouble *) online->find("online.kaos.tof.theta");
  phi_tof    = (AquaNodeDouble *) online->find("online.kaos.tof.phi");
  tracks_tof = (AquaNodeShort *)  online->find("online.kaos.tof.tracks");
  trackID_tof= (AquaNodeShort *)  online->find("online.kaos.multitrack.trackID");
  tracklengthGH_tof = (AquaNodeDouble *) online->find("online.kaos.tof.tracklength");
  tracklengthIH_tof = (AquaNodeDouble *) online->find("online.kaos.tof.tracklengthIH");
  flighttimeGH_tof  = (AquaNodeDouble *) online->find("online.kaos.tof.flighttime");
  flighttimeIH_tof  = (AquaNodeDouble *) online->find("online.kaos.tof.flighttimeIH");

  AC1segment_tof= (AquaNodeDouble *) online->find("online.kaos.tof.AC1segment");
  AC1HitADC_tof = (AquaNodeDouble *) online->find("online.kaos.tof.AC1HitADC");
  AC2segment_tof= (AquaNodeDouble *) online->find("online.kaos.tof.AC2segment");
  AC2HitADC_tof = (AquaNodeDouble *) online->find("online.kaos.tof.AC2HitADC");
  AC1xposition_tof = (AquaNodeDouble *) online->find("online.kaos.tof.AC1positionXZ");
  AC1yposition_tof = (AquaNodeDouble *) online->find("online.kaos.tof.AC1positionY");
  AC2xposition_tof = (AquaNodeDouble *) online->find("online.kaos.tof.AC2positionXZ");
  AC2yposition_tof = (AquaNodeDouble *) online->find("online.kaos.tof.AC2positionY");

  tagger_tof   = (AquaNodeShort *)  online->find("online.kaos.tof.tagger");

  q_tagger = (AquaNodeDouble *) online->find("online.kaos.tof.q_tagger");
  q_aerogel= (AquaNodeDouble *) online->find("online.kaos.tof.q_aerogel");
  q_phi    = (AquaNodeDouble *) online->find("online.kaos.tof.q_phi");
  q_theta  = (AquaNodeDouble *) online->find("online.kaos.tof.q_theta");
  q_dEdx   = (AquaNodeDouble *) online->find("online.kaos.tof.q_dEdx");
  q_TOF    = (AquaNodeDouble *) online->find("online.kaos.tof.q_TOF");
  q_GIx    = (AquaNodeDouble *) online->find("online.kaos.tof.q_GIx");
  q_xHx    = (AquaNodeDouble *) online->find("online.kaos.tof.q_xHx");
  q_xGx    = (AquaNodeDouble *) online->find("online.kaos.tof.q_xGx");
  q_total  = (AquaNodeDouble *) online->find("online.kaos.tof.quality");

  // online spectra
  AquaNode **Nqft = (AquaNode **)qHt;
  online->multifind(Nqft, 0, 29, "online.kaos.hdet.qtop[%d]");
  AquaNode **Nqfb = (AquaNode **)qHb;
  online->multifind(Nqfb, 0, 29, "online.kaos.hdet.qbot[%d]");

  if (use_tof == 3) 
    {
      AquaNode **Nqit = (AquaNode **)qIt;
      online->multifind(Nqit, 0, 14, "online.kaos.idet.qtop[%d]");
      AquaNode **Nqib = (AquaNode **)qIb;
      online->multifind(Nqib, 0, 14, "online.kaos.idet.qbot[%d]");
      
      AquaNode **Nqgt = (AquaNode **)qGt;
      online->multifind(Nqgt, 0, 14, "online.kaos.gdet.qtop[%d]");
      AquaNode **Nqgb = (AquaNode **)qGb;
      online->multifind(Nqgb, 0, 14, "online.kaos.gdet.qbot[%d]");
    }
  else
    {
      AquaNode **Nqgt = (AquaNode **)qGt;
      online->multifind(Nqgt, 0, 29, "online.kaos.gdet.qtop[%d]");
      AquaNode **Nqgb = (AquaNode **)qGb;
      online->multifind(Nqgb, 0, 29, "online.kaos.gdet.qbot[%d]");
    }
  
  AquaNode **Ntft = (AquaNode **)tHt;
  online->multifind(Ntft, 0, 29, "online.kaos.hdet.ttop[%d]");
  AquaNode **Ntfb = (AquaNode **)tHb;
  online->multifind(Ntfb, 0, 29, "online.kaos.hdet.tbot[%d]");
  AquaNode **Ntgt = (AquaNode **)tGt;
  online->multifind(Ntgt, 0, 29, "online.kaos.gdet.ttop[%d]");
  AquaNode **Ntgb = (AquaNode **)tGb;
  online->multifind(Ntgb, 0, 29, "online.kaos.gdet.tbot[%d]");

  AquaNode **NqACt = (AquaNode **)qACt;
  online->multifind(NqACt, 0, 11, "online.kaos.aerogel.ACqtop[%d]");
  AquaNode **NqACb = (AquaNode **)qACb;
  online->multifind(NqACb, 0, 11, "online.kaos.aerogel.ACqbot[%d]");

  AquaNode **Ntimex = (AquaNode **)time_xtra;
  online->multifind(Ntimex, 0, 7, "online.kaos.xtra.time[%d]");

  AquaNode **Nchargex = (AquaNode **)charge_xtra;
  online->multifind(Nchargex, 0, 7, "online.kaos.xtra.charge[%d]");

  // trajectory coordinates
  theta  = (AquaNodeDouble *) online->find("online.kaos.mwpc.theta");
  phi    = (AquaNodeDouble *) online->find("online.kaos.mwpc.phi");
  hitLx  = (AquaNodeDouble *) online->find("online.kaos.mwpc.l.x.hit");
  hitLy  = (AquaNodeDouble *) online->find("online.kaos.mwpc.l.y.hit");
  quality= (AquaNodeDouble *) online->find("online.kaos.mwpc.quality");
}

KaosDetectors::~KaosDetectors()
{
  ;
}

int 
KaosDetectors::init(double * det_pos, double * track_tof, double * threshold_,
		    double * ADC_gain_, double * ADC_pedestal_,
		    double * ADC_correction_, 
		    double * TDC_offset_, double * TDC_correction_,
		    double * TDC_time_corr_,const char *timewalk_corr_filename,
                    double * ADC_absorption_, double * ADC_scale_,
		    double * Aerogel_gain_, double * Aerogel_pedestal_,
		    double * Aerogel_corr_, double * Aerogel_offset_,
		    double use_trigger_, double use_tof_, double use_timewalk_,
		    double use_tagger_, double use_mwpc_, double use_track_, 
		    double use_aerogel_, const char *filename,
                    double use_multitrack_)
{
  // Kaos detector usage
  use_trigger    = use_trigger_;
  use_tof        = use_tof_;
  use_timewalk   = use_timewalk_;
  use_mwpc       = use_mwpc_;
  use_track      = use_track_;
  use_tagger     = use_tagger_;
  use_aerogel    = use_aerogel_;
  use_multitrack = use_multitrack_;

  // Kaos detector positions
  if (use_track < 3) // old coordinate system
    {
      xHG = det_pos[4] - det_pos[2];
      zHG = det_pos[5] - det_pos[3];
      zLM = det_pos[1];
      xLH = det_pos[2];
      zLH = det_pos[3];
      xLG = det_pos[4];
      zLG = det_pos[5];
      
      alphaH = det_pos[6];
      alphaG = det_pos[7];

      yH  = det_pos[10];
      yG  = det_pos[11];
      lH  = det_pos[13];
      lG  = det_pos[14];

      xLAC1= det_pos[16];
      zLAC1= det_pos[17];

    }
  else // 2o12 coordinate system
    {
      xG  = det_pos[4];
      zG  = det_pos[5];
      zM  = det_pos[30];
      xH  = det_pos[2];
      zH  = det_pos[3];
 
      xHG = det_pos[4]  - det_pos[2];
      zHG = det_pos[5]  - det_pos[3];

      zLM = det_pos[30] - det_pos[1];
      xLH = det_pos[2]  - det_pos[15];
      zLH = det_pos[3]  - det_pos[30];
      xLG = det_pos[4]  - det_pos[15];
      zLG = det_pos[5]  - det_pos[30];
      
      alphaH = det_pos[6];
      alphaG = det_pos[7];

      yH  = det_pos[10];
      yG  = det_pos[11];

      lH  = det_pos[13];
      lG  = det_pos[14];

      if (use_tof == 3)
	{
	  alphaI = alphaG;
	  xI  = xG  + sqrt(2) * 120 * cos( (45 + alphaG) / degree);
	  zI  = zG  + sqrt(2) * 120 * sin( (45 + alphaG) / degree);
	  // xI = xG + 120 * cos(alphaG / degree) + 120 * sin(alphaG / degree);
	  // zI = zG - 120 * sin(alphaG / degree) + 120 * cos(alphaG / degree);
	  xLI = xLG + sqrt(2) * 120 * cos( (45 + alphaG) / degree);
          zLI = zLG + sqrt(2) * 120 * sin( (45 + alphaG) / degree);
	  // xLI = xLG + 120 * cos(alphaG / degree) + 120 * sin(alphaG / degree);
	  // zLI = zLG - 120 * sin(alphaG / degree) + 120 * cos(alphaG / degree);
          yI = yG;
	  lI = lG;
	}

      xAC1= det_pos[16];
      zAC1= det_pos[17];

      xLAC1= det_pos[16] - det_pos[15];
      zLAC1= det_pos[17] - det_pos[30];

      xAC2= det_pos[33];
      zAC2= det_pos[34];

      xLAC2= det_pos[33] - det_pos[15];
      zLAC2= det_pos[34] - det_pos[30];
    }

  for (int i=0;i<6;i++) {
    AC1min[i]= det_pos[18+2*i];
    AC1max[i]= det_pos[19+2*i];
  }

  for (int i=0;i<6;i++) {
    AC2min[i]= det_pos[37+2*i];
    AC2max[i]= det_pos[38+2*i];
  }

  xLTAG= xLAC1 + 104;
  zLTAG= zLAC1 + 220;

  yTAG    = 0;
  yTAGTOP = 0;
  yTAGBOT = 0;

  /* TOF reconstruction parameters */

  deltaTOF       = track_tof[0];
  plateau_theta  = track_tof[1];
  sigma_theta    = track_tof[2];
  TOF_slope      = track_tof[3];

  offset_phi     = track_tof[4];
  plateau_phi    = track_tof[5];
  sigma_phi      = track_tof[6];
  slope_phi      = track_tof[7]; 

  cluster_time   = track_tof[8];
  cluster_max    = track_tof[9];

  reference_dEdxG= track_tof[10];
  plateau_dEdxG  = track_tof[11];
  sigma_dEdxG    = track_tof[12];
  dEdxG_slope    = track_tof[13];

  reference_dEdxH= track_tof[14];
  plateau_dEdxH  = track_tof[15];
  sigma_dEdxH    = track_tof[16];
  dEdxH_slope    = track_tof[17];

  reference_tof  = track_tof[18];
  plateau_TOF    = track_tof[19];
  sigma_TOF      = track_tof[20];
  deltat_slope   = track_tof[21];
 
  // ADC thresholds
  wallH.ind_thr = threshold_[1];
  wallG.ind_thr = threshold_[2];
  ACThreshold   = threshold_[3];
  TagThreshold  = threshold_[4];

  // ADC scale conversion
  HscaleADC2MeV = ADC_scale_[0];
  GscaleADC2MeV = ADC_scale_[1];

  for (int i=0; i<30; i++) {
    ADC_gain_HTOP[i]= ADC_gain_[i];
    ADC_ped_HTOP[i] = ADC_pedestal_[i];
    ADC_cor_HTOP[i] = ADC_correction_[i];
    TDC_off_HTOP[i] = TDC_offset_[i];
    TDC_cor_HTOP[i] = TDC_correction_[i];

    ADC_gain_HBOT[i]= ADC_gain_[i+30];
    ADC_ped_HBOT[i] = ADC_pedestal_[i+30];
    ADC_cor_HBOT[i] = ADC_correction_[i+30];
    TDC_off_HBOT[i] = TDC_offset_[i+30];
    TDC_cor_HBOT[i] = TDC_correction_[i+30];

    ADC_gain_GTOP[i]= ADC_gain_[i+60];
    ADC_ped_GTOP[i] = ADC_pedestal_[i+60];
    ADC_cor_GTOP[i] = ADC_correction_[i+60];
    TDC_off_GTOP[i] = TDC_offset_[i+60];
    TDC_cor_GTOP[i] = TDC_correction_[i+60];

    ADC_gain_GBOT[i]= ADC_gain_[i+90];
    ADC_ped_GBOT[i] = ADC_pedestal_[i+90];
    ADC_cor_GBOT[i] = ADC_correction_[i+90];
    TDC_off_GBOT[i] = TDC_offset_[i+90];
    TDC_cor_GBOT[i] = TDC_correction_[i+90];

    time_offsetH[i] = TDC_time_corr_[i];
    time_slopeH[i]  = TDC_time_corr_[i+30];
    time_offsetG[i] = TDC_time_corr_[i+60];
    time_slopeG[i]  = TDC_time_corr_[i+90];

    absorption_HTOP[i]= ADC_absorption_[i*4+1];
    absorption_HBOT[i]= ADC_absorption_[i*4+3];
    absorption_GTOP[i]= ADC_absorption_[i*4+121];
    absorption_GBOT[i]= ADC_absorption_[i*4+123];
  }

  for (int i=0; i<8; i++) {
    TDC_cor_x[i]  = TDC_correction_[120+i];
    ADC_cor_x[i]  = ADC_correction_[120+i];
    ADC_ped_x[i]  = ADC_pedestal_[120+i];
  }

  for (int i=0; i<6; i++) {
    // for AC1
    Aerogel_gain_TOP[i]  = Aerogel_gain_[i];
    Aerogel_gain_BOT[i]  = Aerogel_gain_[i+6];
    Aerogel_ped_TOP[i]   = Aerogel_pedestal_[i];
    Aerogel_ped_BOT[i]   = Aerogel_pedestal_[i+6];
    Aerogel_cor_TOP[i]  = Aerogel_corr_[i];
    Aerogel_cor_BOT[i]  = Aerogel_corr_[i+6];
    Aerogel_off_TOP[i]  = Aerogel_offset_[i];
    Aerogel_off_BOT[i]  = Aerogel_offset_[i+6];
    // for AC2
    Aerogel_gain_TOP[i+6]= Aerogel_gain_[i+12];
    Aerogel_gain_BOT[i+6]= Aerogel_gain_[i+12+6];
    Aerogel_ped_TOP[i+6] = Aerogel_pedestal_[i+12];
    Aerogel_ped_BOT[i+6] = Aerogel_pedestal_[i+12+6];
    Aerogel_cor_TOP[i+6]  = Aerogel_corr_[i+12];
    Aerogel_cor_BOT[i+6]  = Aerogel_corr_[i+12+6];
    Aerogel_off_TOP[i+6]  = Aerogel_offset_[i+12];
    Aerogel_off_BOT[i+6]  = Aerogel_offset_[i+12+6];
  }
  
  if (use_timewalk)
  {
    std::ifstream timeWalkCorrectionFile(timewalk_corr_filename);
    for (int varDetector = 0; varDetector < 2; varDetector++)
    {
      for (int varPmt = 0; varPmt < 2; varPmt++) // TODO change to 3 to include top-bottom-difference-correction -> change timewalk files as well
      {
        for (int varSegment = 0; varSegment < 30; varSegment++)
        {
          for (int varCorrection = 0; varCorrection < 4; varCorrection++)
          {
            double detector;
            double topBottom;
            double segment;
            double correction;
            double par[10];
            timeWalkCorrectionFile >> detector >> topBottom >> segment >> correction >> par[0] >> par[1] >> par[2] >> par[3] >> par[4] >> par[5] >> par[6] >> par[7] >> par[8] >> par[9];
            if ((detector == varDetector) && (topBottom == varPmt) && (segment == varSegment) && (correction == varCorrection))
            {
              if ((detector == 0) && (topBottom == 0))
              {
                timewalk_cor_HTOP[varSegment][varCorrection][0] = par[0];
                timewalk_cor_HTOP[varSegment][varCorrection][1] = par[1];
                timewalk_cor_HTOP[varSegment][varCorrection][2] = par[2];
                timewalk_cor_HTOP[varSegment][varCorrection][3] = par[3];
                timewalk_cor_HTOP[varSegment][varCorrection][4] = par[4];
                timewalk_cor_HTOP[varSegment][varCorrection][5] = par[5];
                timewalk_cor_HTOP[varSegment][varCorrection][6] = par[6];
                timewalk_cor_HTOP[varSegment][varCorrection][7] = par[7];
                timewalk_cor_HTOP[varSegment][varCorrection][8] = par[8];
                timewalk_cor_HTOP[varSegment][varCorrection][9] = par[9];
              }
              if ((detector == 0) && (topBottom == 1))
              {
                timewalk_cor_HBOT[varSegment][varCorrection][0] = par[0];
                timewalk_cor_HBOT[varSegment][varCorrection][1] = par[1];
                timewalk_cor_HBOT[varSegment][varCorrection][2] = par[2];
                timewalk_cor_HBOT[varSegment][varCorrection][3] = par[3];
                timewalk_cor_HBOT[varSegment][varCorrection][4] = par[4];
                timewalk_cor_HBOT[varSegment][varCorrection][5] = par[5];
                timewalk_cor_HBOT[varSegment][varCorrection][6] = par[6];
                timewalk_cor_HBOT[varSegment][varCorrection][7] = par[7];
                timewalk_cor_HBOT[varSegment][varCorrection][8] = par[8];
                timewalk_cor_HBOT[varSegment][varCorrection][9] = par[9];
              }
              if ((detector == 0) && (topBottom == 2))
              {
                timewalk_cor_HSUB[varSegment][varCorrection][0] = par[0];
                timewalk_cor_HSUB[varSegment][varCorrection][1] = par[1];
                timewalk_cor_HSUB[varSegment][varCorrection][2] = par[2];
                timewalk_cor_HSUB[varSegment][varCorrection][3] = par[3];
                timewalk_cor_HSUB[varSegment][varCorrection][4] = par[4];
                timewalk_cor_HSUB[varSegment][varCorrection][5] = par[5];
                timewalk_cor_HSUB[varSegment][varCorrection][6] = par[6];
                timewalk_cor_HSUB[varSegment][varCorrection][7] = par[7];
                timewalk_cor_HSUB[varSegment][varCorrection][8] = par[8];
                timewalk_cor_HSUB[varSegment][varCorrection][9] = par[9];
              }
              if ((detector == 1) && (topBottom == 0))
              {
                timewalk_cor_GTOP[varSegment][varCorrection][0] = par[0];
                timewalk_cor_GTOP[varSegment][varCorrection][1] = par[1];
                timewalk_cor_GTOP[varSegment][varCorrection][2] = par[2];
                timewalk_cor_GTOP[varSegment][varCorrection][3] = par[3];
                timewalk_cor_GTOP[varSegment][varCorrection][4] = par[4];
                timewalk_cor_GTOP[varSegment][varCorrection][5] = par[5];
                timewalk_cor_GTOP[varSegment][varCorrection][6] = par[6];
                timewalk_cor_GTOP[varSegment][varCorrection][7] = par[7];
                timewalk_cor_GTOP[varSegment][varCorrection][8] = par[8];
                timewalk_cor_GTOP[varSegment][varCorrection][9] = par[9];
              }
              if ((detector == 1) && (topBottom == 1))
              {
                timewalk_cor_GBOT[varSegment][varCorrection][0] = par[0];
                timewalk_cor_GBOT[varSegment][varCorrection][1] = par[1];
                timewalk_cor_GBOT[varSegment][varCorrection][2] = par[2];
                timewalk_cor_GBOT[varSegment][varCorrection][3] = par[3];
                timewalk_cor_GBOT[varSegment][varCorrection][4] = par[4];
                timewalk_cor_GBOT[varSegment][varCorrection][5] = par[5];
                timewalk_cor_GBOT[varSegment][varCorrection][6] = par[6];
                timewalk_cor_GBOT[varSegment][varCorrection][7] = par[7];
                timewalk_cor_GBOT[varSegment][varCorrection][8] = par[8];
                timewalk_cor_GBOT[varSegment][varCorrection][9] = par[9];
              }
              if ((detector == 1) && (topBottom == 2))
              {
                timewalk_cor_GSUB[varSegment][varCorrection][0] = par[0];
                timewalk_cor_GSUB[varSegment][varCorrection][1] = par[1];
                timewalk_cor_GSUB[varSegment][varCorrection][2] = par[2];
                timewalk_cor_GSUB[varSegment][varCorrection][3] = par[3];
                timewalk_cor_GSUB[varSegment][varCorrection][4] = par[4];
                timewalk_cor_GSUB[varSegment][varCorrection][5] = par[5];
                timewalk_cor_GSUB[varSegment][varCorrection][6] = par[6];
                timewalk_cor_GSUB[varSegment][varCorrection][7] = par[7];
                timewalk_cor_GSUB[varSegment][varCorrection][8] = par[8];
                timewalk_cor_GSUB[varSegment][varCorrection][9] = par[9];
              }
            } else {
              std::cerr << "error parsing timewalk file line: " << ((varDetector * 2 + varPmt) * 30 + varSegment) * 4 + varCorrection << std::endl;
            }
          }
        }
      }
    }
  }
  
  if (use_trigger)
    {
      std::ifstream triggerfile(filename);
      int label;
      if (triggerfile)
	{
	  if (use_tof == 3)
	    {for (int G=0; G<15; G++)
		{for (int I=0; I<15; I++)
		    {for (int H=0; H<30; H++)
			{if (triggerfile  >> label >> label >> label >> triggerGIH[G][I][H]) 
			    {if (use_trigger == 2) std::cout << "G" << G 
							     << " I" << I 
							     << " H" << H 
							     << " "  << triggerGIH[G][I][H]
							     << std::endl;}
			  else
			    std::cerr << "Error reading G" 
				      << G << " I" << I 
				      << " H" << H << std::endl;
			} 
		    }
		}
	    }
	  else
	    {
	      for (int G=0; G<30; G++) triggerfile >> label; // reading G labels
	      for (int G=0; G<30; G++)
		{triggerfile >> label; // reading H label
		  for (int H=0; H<30; H++)
		    if (triggerfile >> triggerGH[G][H]) 
		      {if (use_trigger == 2) std::cout << triggerGH[G][H];}
		    else
		      std::cerr << "Error reading G" 
				<< G << " H" << H << std::endl;
		  if (use_trigger == 2) std::cout << std::endl;
		}
	    }
	  triggerfile.close();
	}
      else  std::cerr << "<KaosDetectors.cc> No trigger file found: "  
		      << filename << std::endl;
    }

  return 0;
}

int 
KaosDetectors::fill_hit_array(AquaNodeDouble **T_TOP,
			      AquaNodeDouble **T_BOT,
			      AquaNodeDouble **Q_TOP,
			      AquaNodeDouble **Q_BOT, 
			      double *time_offset, double *time_slope,
			      t_group &wall, int StartNoOfPaddle, int StopNoOfPaddle)
{
  unsigned short N_hit=0;
  
  for (int i=StartNoOfPaddle; i <= StopNoOfPaddle; i++)
    {
//       std::cout << i << " " << *Q_TOP[i] << " " << *Q_BOT[i] << " " << *T_TOP[i] << " " << *T_BOT[i] << std::endl;
      
      if ( *Q_TOP[i] > 0 && *Q_BOT[i] > 0 &&
	*T_TOP[i] > 0 && *T_BOT[i] > 0 )
	{
  	  wall.hits[N_hit].paddle = i - StartNoOfPaddle;

	  // ADC GMEAN
	  wall.hits[N_hit].adc_gmean = sqrt(*Q_TOP[i] * *Q_BOT[i]);
          
          // TDC MEAN
          wall.hits[N_hit].tdc_mean = (*T_TOP[i] + *T_BOT[i])/2 * 0.025; // in ns
          wall.hits[N_hit].tdc_delta= (time_offset[i] +                  // in mm
                                       (*T_TOP[i] - *T_BOT[i]) * time_slope[i]);
          
          #ifdef INCREASE_DOUBLE_HIT_RECOGNITION
          /* This piece of code should fix the problem,
           * that double hits are not recognised as such,
           * when one channel has no valid TDC information
           * for all single hits, the neighbouring channel
           * with the highest ADC is used as
           * the second hit of a multi paddle hit,
           * only if there is no neighbouring hit to the neighbour*/ 
          double meanAdcLower = 0;
          double meanAdcHigher = 0;
          if ((i == StopNoOfPaddle) || (*T_TOP[i + 1] <= 0 || *T_BOT[i + 1] <= 0 || *Q_TOP[i + 1] <= 0 || *Q_BOT[i + 1] <= 0 ))
          { // no hit in higher neighbour
            if (i - 1 == StartNoOfPaddle)
            { // treating hits on the lower side
              if (*Q_TOP[i - 1] > 0 && *Q_BOT[i - 1] > 0 && (*T_TOP[i - 1] <= 0 || *T_BOT[i - 1] <= 0)) meanAdcLower = sqrt(*Q_TOP[i - 1] * *Q_BOT[i - 1]);
            } else {
              if ((i - 1 > StartNoOfPaddle) && (*T_TOP[i - 2] <= 0 || *T_BOT[i - 2] <= 0 || *Q_TOP[i - 2] <= 0 || *Q_BOT[i - 2] <= 0 ))
                if (*Q_TOP[i - 1] > 0 && *Q_BOT[i - 1] > 0 && (*T_TOP[i - 1] <= 0 || *T_BOT[i - 1] <= 0)) meanAdcLower = sqrt(*Q_TOP[i - 1] * *Q_BOT[i - 1]);
            }
          }
          
          if ((i == StartNoOfPaddle) || (*T_TOP[i - 1] <= 0 || *T_BOT[i - 1] <= 0 || *Q_TOP[i - 1] <= 0 || *Q_BOT[i - 1] <= 0 ))
          { // no hit in lowher neighbour
            if (i + 1 == StopNoOfPaddle)
            { // treating hits on the higher side
              if (*Q_TOP[i + 1] > 0 && *Q_BOT[i + 1] > 0 && (*T_TOP[i + 1] <= 0 || *T_BOT[i + 1] <= 0)) meanAdcHigher = sqrt(*Q_TOP[i + 1] * *Q_BOT[i + 1]);
            } else {
              if ((i + 1 < StopNoOfPaddle) && (*T_TOP[i + 2] <= 0 || *T_BOT[i + 2] <= 0 || *Q_TOP[i + 2] <= 0 || *Q_BOT[i + 2] <= 0 ))
                if (*Q_TOP[i + 1] > 0 && *Q_BOT[i + 1] > 0 && (*T_TOP[i + 1] <= 0 || *T_BOT[i + 1] <= 0)) meanAdcHigher = sqrt(*Q_TOP[i + 1] * *Q_BOT[i + 1]);
            }
          }
          
          if (meanAdcLower > 0 || meanAdcHigher > 0)
          { //std::cout << meanAdcLower << " " << meanAdcHigher << std::endl;
            if (meanAdcLower > meanAdcHigher)
            { // add lower neighbour as hit
              wall.hits[N_hit + 1].paddle    = wall.hits[N_hit].paddle;
              // ADC GMEAN
              wall.hits[N_hit + 1].adc_gmean = wall.hits[N_hit].adc_gmean;
              // TDC MEAN
              wall.hits[N_hit + 1].tdc_mean  = wall.hits[N_hit].tdc_mean;
              wall.hits[N_hit + 1].tdc_delta = wall.hits[N_hit].tdc_delta;
              
              wall.hits[N_hit].paddle = i - StartNoOfPaddle - 1;
              // ADC GMEAN
              wall.hits[N_hit].adc_gmean = meanAdcLower;
              // TDC MEAN
              // wall.hits[N_hit].tdc_mean = wall.hits[N_hit].tdc_mean; // nothing to be done here
              // wall.hits[N_hit].tdc_delta = wall.hits[N_hit].tdc_delta // keep initiol parameters
              N_hit++; //std::cout << "lower" << std::endl;
            } else { // add higher neighbour as hit
              N_hit++; //std::cout << "higher" << std::endl;
              wall.hits[N_hit].paddle = i - StartNoOfPaddle + 1;
              // ADC GMEAN
              wall.hits[N_hit].adc_gmean = meanAdcHigher;
              // TDC MEAN
              wall.hits[N_hit].tdc_mean = wall.hits[N_hit - 1].tdc_mean; // take data from actual hit
              wall.hits[N_hit].tdc_delta= wall.hits[N_hit - 1].tdc_delta; // take data from actual hit
            }
          }
          
          #endif // INCREASE_DOUBLE_HIT_RECOGNITION
          


          #ifdef DEBUG_KAOS_TOF
	  std::cout << "paddle=" << i - StartNoOfPaddle << std::endl;
	  std::cout << "TOP TDC: " << *T_TOP[i] 
		    << "  BOT TDC: " << *T_BOT[i] << std::endl;
	  std::cout << "TOP ADC: " << *Q_TOP[i] \
		    << "  BOT ADC: " << *Q_BOT[i] << std::endl;
	  #endif

	  // increase hit counter
	  if (wall.hits[N_hit].tdc_mean != 0) 
	    {
	      N_hit++;
	      if (kaos_debug == 1) std::cout << "+";
	    }
	  else if (kaos_debug == 1) std::cout << "-";
	}
      else 
  	{if (kaos_debug == 1) std::cout << "-";}
    }

  /* // the following code defines hits by ADC 
     // (useful for debugging when TDC is missing)

  if (N_hit==0)
    {
      for (int i=0; i<NoOfPaddles; i++)
	{
	  if  (*Q_TOP[i] > 0 && *Q_BOT[i] > 0) 
	    {
	      wall.hits[N_hit].paddle = i;
	      
	      // ADC GMEAN
	      wall.hits[N_hit].adc_gmean = sqrt(*Q_TOP[i] * *Q_BOT[i]);

	      wall.hits[N_hit].tdc_mean = 0;
	      wall.hits[N_hit].cluster_id = -1;
	  
	      // increase hit counter
	      N_hit++;
	      wall.hits[N_hit].cluster_id = -1;
	    }
	}
    }
  */

  wall.N_hits = N_hit;
  if (kaos_debug == 1) std::cout << std::endl;
 
  return 0;
}

int 
KaosDetectors::cnt_cluster(t_group &wall)
{
  int cluster_id = 0;
  t_hit hit_old;

  if (wall.N_hits <= 0) 
  {
    wall.cnt_cluster = 0;
    return 0;
  }
  
  wall.hits[0].cluster_id = cluster_id;
  hit_old = wall.hits[0];

  for (int i=1; i<wall.N_hits; i++)
  {
    // separate clusters if maximum size is reached ||
    // separate clusters if time difference is too large
    if (fabs(wall.hits[i].tdc_mean - hit_old.tdc_mean) > cluster_time/1000 ||
	(wall.hits[i].paddle - hit_old.paddle > (cluster_max-1) ) || 
	(wall.hits[i].paddle - wall.hits[i-1].paddle > 1) )
    {
      cluster_id++;
      hit_old = wall.hits[i];
    }
    wall.hits[i].cluster_id = cluster_id;
  }
  wall.cnt_cluster = cluster_id + 1;

  return cluster_id + 1;
}

void 
KaosDetectors::get_cluster_data(t_group &wall,
				double threshold)
{

  if (wall.cnt_cluster <= 0) return;

  for (int cluster_id=0; cluster_id < wall.cnt_cluster; cluster_id++)
    {
      wall.cluster[cluster_id].cluster_id = cluster_id;
      wall.cluster[cluster_id].hit_cnt    = 0;
      wall.cluster[cluster_id].charge     = 0;
      wall.cluster[cluster_id].mean_time  = 0;
      wall.cluster[cluster_id].delta_time = 0;

      wall.cluster[cluster_id].center_of_charge = -1;
      wall.cluster[cluster_id].max_adc       = 32;
      wall.cluster[cluster_id].max_adc_value = 0;
      
      t_hit max;
      max.cluster_id = -1;
      max.paddle     = -1;
      max.adc_gmean  =  0;
      max.tdc_mean   =  0;
      max.tdc_delta  =  0;

      double charge  = 0; 
      double ADCgmean= 0; 
      double product = 0;
      
      double maxTimeInCluster = -999999999;
      double minTimeInCluster = 999999999;

      for (int i=0; i < wall.N_hits; i++)
	{
	  // find paddle in cluster with maximum ADC mean value 
	  // this is used to define timing
	  if (wall.hits[i].cluster_id == cluster_id) 
	    {
	      ADCgmean = wall.hits[i].adc_gmean;
	      
	      // initialize counters for new cluster
	      if (wall.hits[i].cluster_id != max.cluster_id)
		{
                  max = wall.hits[i];
		  charge  = 0;
		  product = 0;
		}
	      // check for maximum ADC mean value in cluster	  
	      else if (ADCgmean > max.adc_gmean) max = wall.hits[i]; 
	      
	      wall.cluster[cluster_id].hit_cnt++;
	      charge += ADCgmean;
	      product = product + ( (wall.hits[i].paddle + 1) * ADCgmean);
              if (wall.hits[i].tdc_mean > maxTimeInCluster) {maxTimeInCluster = wall.hits[i].tdc_mean;}
              if (wall.hits[i].tdc_mean < minTimeInCluster) {minTimeInCluster = wall.hits[i].tdc_mean;}
            }  // if (wall.hits[i].cluster_id == cluster_id
	} // for (int i=0; i < wall.N_hits; i++)
     
      // provide cluster information
      if (charge > threshold && product > 0)
	{ 
	  wall.cluster[cluster_id].charge        = charge;
	  wall.cluster[cluster_id].center_of_charge = (product/charge) - 1;
	  wall.cluster[cluster_id].max_adc       = max.paddle;
	  wall.cluster[cluster_id].max_adc_value = max.adc_gmean;
	  wall.cluster[cluster_id].mean_time     = max.tdc_mean;
	  wall.cluster[cluster_id].delta_time    = max.tdc_delta;
          wall.cluster[cluster_id].delta_time_cluster = maxTimeInCluster - minTimeInCluster;
	}
    }  
}

void 
KaosDetectors::calibrate_TDC(AquaNodeUShort **TDC_TOP,
			     AquaNodeUShort **TDC_BOT, 
			     AquaNodeDouble **T_TOP,
			     AquaNodeDouble **T_BOT,
			     double *TDC_off_TOP, double *TDC_off_BOT,
			     double *TDC_cor_TOP, double *TDC_cor_BOT,
			     double x, double y, double l,
			     double *time_offset, double *time_slope,
                             int wallID,
                             AquaNodeUShort **ADC_TOP,
                             AquaNodeUShort **ADC_BOT,
                             double *ADC_cor_TOP, double *ADC_cor_BOT)
{
  double RAW;
  
  for (int i=0; i<30; i++)
    {
      // TOP TDC
      if (TDC_TOP[i]->ok() && TDC_TOP[i]->scalar() > 0)
	{
	  if (*TDC_TOP[i] > 4095)  // TDC High Range correction
	    RAW = (*TDC_TOP[i]-4095)*8 + TDC_cor_TOP[i]-24;
	  else RAW = *TDC_TOP[i];
          #ifdef DEBUG_KAOS_CALIB
	  std::cout << i << " TOP TDC:  " << *TDC_TOP[i] << std::endl;
          #endif
	  *T_TOP[i] = RAW + 40*TDC_off_TOP[i];
          if (use_timewalk == 1)
          {
            double rawADC;
            if (*ADC_TOP[i] > 4095) {rawADC = 8*(*ADC_TOP[i]-4096) + ADC_cor_TOP[i]-24;}
            else {rawADC = *ADC_TOP[i];}
            
            if (wallID == 0)
            {
              for (int j = 0; j < 4; j++)
              {
                if ((rawADC > timewalk_cor_HTOP[i][j][8]) && ((rawADC < timewalk_cor_HTOP[i][j][9]))) 
                  *T_TOP[i] = *T_TOP[i] - (timewalk_cor_HTOP[i][j][0] + 
                                 rawADC * (timewalk_cor_HTOP[i][j][1] + 
                                 rawADC * (timewalk_cor_HTOP[i][j][2] + 
                                 rawADC * (timewalk_cor_HTOP[i][j][3] + 
                                 rawADC * (timewalk_cor_HTOP[i][j][4] + 
                                 rawADC * (timewalk_cor_HTOP[i][j][5] + 
                                 rawADC * (timewalk_cor_HTOP[i][j][6] + 
                                 rawADC * (timewalk_cor_HTOP[i][j][7]))))))));
              }
            }
            else
            {
              for (int j = 0; j < 4; j++)
              {
                if ((rawADC > timewalk_cor_GTOP[i][j][8]) && ((rawADC < timewalk_cor_GTOP[i][j][9]))) 
                  *T_TOP[i] = *T_TOP[i] - (timewalk_cor_GTOP[i][j][0] + 
                                 rawADC * (timewalk_cor_GTOP[i][j][1] + 
                                 rawADC * (timewalk_cor_GTOP[i][j][2] + 
                                 rawADC * (timewalk_cor_GTOP[i][j][3] + 
                                 rawADC * (timewalk_cor_GTOP[i][j][4] + 
                                 rawADC * (timewalk_cor_GTOP[i][j][5] + 
                                 rawADC * (timewalk_cor_GTOP[i][j][6] + 
                                 rawADC * (timewalk_cor_GTOP[i][j][7]))))))));
              }
            }
          }
	}
      else *T_TOP[i] = 0;
	  
      // BOTTOM TDC
      if (TDC_BOT[i]->ok() && TDC_BOT[i]->scalar() > 0) 
	{ 
	  if (*TDC_BOT[i] > 4095)  // TDC High Range correction
	    RAW = (*TDC_BOT[i]-4095)*8 + TDC_cor_BOT[i]-24;
	  else RAW = *TDC_BOT[i];
          #ifdef DEBUG_KAOS_CALIB
	  std::cout << i << " BOT TDC:  " << *TDC_BOT[i] << std::endl;
          #endif
	  *T_BOT[i]= RAW + 40.*TDC_off_BOT[i];
          if (use_timewalk == 1)
          {
            double rawADC;
            if (*ADC_BOT[i] > 4095) {rawADC = 8*(*ADC_BOT[i]-4096) + ADC_cor_BOT[i]-24;}
            else {rawADC = *ADC_BOT[i];}
            
            if (wallID == 0)
            {
              for (int j = 0; j < 4; j++)
              {
                if ((rawADC > timewalk_cor_HBOT[i][j][8]) && ((rawADC < timewalk_cor_HBOT[i][j][9]))) 
                  *T_BOT[i] = *T_BOT[i] - (timewalk_cor_HBOT[i][j][0] + 
                                 rawADC * (timewalk_cor_HBOT[i][j][1] + 
                                 rawADC * (timewalk_cor_HBOT[i][j][2] + 
                                 rawADC * (timewalk_cor_HBOT[i][j][3] + 
                                 rawADC * (timewalk_cor_HBOT[i][j][4] + 
                                 rawADC * (timewalk_cor_HBOT[i][j][5] + 
                                 rawADC * (timewalk_cor_HBOT[i][j][6] + 
                                 rawADC * (timewalk_cor_HBOT[i][j][7]))))))));
              }
            }
            else
            {
              for (int j = 0; j < 4; j++)
              {
                if ((rawADC > timewalk_cor_GBOT[i][j][8]) && ((rawADC < timewalk_cor_GBOT[i][j][9]))) 
                  *T_BOT[i] = *T_BOT[i] - (timewalk_cor_GBOT[i][j][0] + 
                                 rawADC * (timewalk_cor_GBOT[i][j][1] + 
                                 rawADC * (timewalk_cor_GBOT[i][j][2] + 
                                 rawADC * (timewalk_cor_GBOT[i][j][3] + 
                                 rawADC * (timewalk_cor_GBOT[i][j][4] + 
                                 rawADC * (timewalk_cor_GBOT[i][j][5] + 
                                 rawADC * (timewalk_cor_GBOT[i][j][6] + 
                                 rawADC * (timewalk_cor_GBOT[i][j][7]))))))));
              }
            }
          }
	}
      else *T_BOT[i] =  0;

#ifdef EXTRAPOLATE_MISSING_CHANNELS
      // MISSING BOTTOM TDC
      if (*T_BOT[i] == 0 && *T_TOP[i] > 0 && time_slope[i] != 0 &&
	  ( (x - (i+0.5)*l)/l <  1 + deltaTOF) && 
	  ( (x - (i+0.5)*l)/l > -1 + deltaTOF))
	{
	  *T_BOT[i] = *T_TOP[i] 
	    + 1/time_slope[i]*y - time_offset[i]/time_slope[i];

          #ifdef DEBUG_KAOS_TOF
	  std::cout << "BOT TDC MISSING IN PADDLE " << i << std::endl;
	  std::cout << "SLOPE:  "   << time_slope[i] 
		    << "  y:  "     << y 
		    << "  OFFSET: " << time_offset[i] << std::endl;
	  std::cout << "EXTRAPOLATED BOT TDC:  " << *T_BOT[i] << std::endl;
          #endif
	}

      // MISSING TOP TDC
      if (*T_TOP[i] == 0 && *T_BOT[i] > 0 && time_slope[i] != 0 && 
	  ( (x - (i+0.5)*l)/l <  1 + deltaTOF) && 
	  ( (x - (i+0.5)*l)/l > -1 + deltaTOF))
	{
	  *T_TOP[i] = *T_BOT[i] 
	    - (1/time_slope[i]*y - time_offset[i]/time_slope[i]) ;

          #ifdef DEBUG_KAOS_TOF
	  std::cout << "TOP TDC MISSING IN PADDLE " << i << std::endl;
	  std::cout << "SLOPE:  "   << time_slope[i] 
		    << "  y:  "     << y 
		    << "  OFFSET: " << time_offset[i] << std::endl;
	  std::cout << "EXTRAPOLATED TOP TDC:  " << *T_TOP[i] << std::endl;
          #endif
	}
#endif

#ifdef SIMPLE_TDC_EXTRAPOLATION
if (*T_BOT[i] == 0 && *T_TOP[i]  > 0) {*T_BOT[i] = *T_TOP[i];}
if (*T_BOT[i]  > 0 && *T_TOP[i] == 0) {*T_TOP[i] = *T_BOT[i];}
#endif

      if (use_timewalk == 1)
      { // top bottom TDC substraction ADC timewalk correction
        
      }
    }
  return;
}

void 
KaosDetectors::calibrate_ADC(AquaNodeUShort **ADC_TOP,
			     AquaNodeUShort **ADC_BOT, 
			     AquaNodeDouble **Q_TOP,
			     AquaNodeDouble **Q_BOT,
			     double *ped_TOP, double *ped_BOT, 
			     double *gain_TOP, double *gain_BOT,
			     double *ADC_cor_TOP, double *ADC_cor_BOT,
			     double *abs_len_TOP, double *abs_len_BOT,
			     double x, double y, double l)
{  
  double RAW;

  for (int i=0; i<30; i++)
    {
      // TOP ADC
      if (ADC_TOP[i]->ok() && (*ADC_TOP[i]-ped_TOP[i]) > 0)
	{
	  if (*ADC_TOP[i] > 4095)  // ADC High Range correction
	    {RAW = 8*(*ADC_TOP[i]-4096) + ADC_cor_TOP[i]-24;
             #ifdef DEBUG_KAOS_CALIB
	      std::cout << i << "ADC: " << *ADC_TOP[i] << std::endl;
	      std::cout << i << "off: " << ADC_cor_TOP[i] << std::endl;
	      std::cout << i << "RAW: " << RAW << std::endl;
	     #endif
	    }
	  else RAW = *ADC_TOP[i];
	  //	  *Q_TOP[i] = (RAW-ped_TOP[i]);

	  // ---------------------------------------------
	  *Q_TOP[i] = (RAW-ped_TOP[i])*gain_TOP[i];
	  // ---------------------------------------------

          #ifdef DEBUG_KAOS_CALIB
	  std::cout << "prop. length: " << y 
		    << "  Q: " << (RAW-ped_TOP[i])*gain_TOP[i]
		    << " dE: " << *Q_TOP[i] << std::endl;
          #endif
	}
      else *Q_TOP[i] = 0;
      
      // BOTTOM ADC
      if (ADC_BOT[i]->ok() && (*ADC_BOT[i]-ped_BOT[i]) > 0)
	{
	  if (*ADC_BOT[i] > 4095)  // ADC High Range correction
	    RAW = 8*(*ADC_BOT[i]-4096) + ADC_cor_BOT[i]-24;
	  else RAW = *ADC_BOT[i];

	  //*Q_BOT[i] = (RAW-ped_BOT[i]); // calibration

	  // ---------------------------------------------
	  *Q_BOT[i] = (RAW-ped_BOT[i])*gain_BOT[i];
	  // ---------------------------------------------

          #ifdef DEBUG_KAOS_CALIB
	  std::cout << "prop. length: " << y 
		    << "  Q: " << (RAW-ped_BOT[i])*gain_BOT[i]
		    << " dE: " << *Q_BOT[i] << std::endl;
          #endif
	}
      else *Q_BOT[i] = 0;

#ifdef EXTRAPOLATE_MISSING_CHANNELS
      // MISSING TOP ADC
      if (*Q_TOP[i] == 0 && *Q_BOT[i] > 0 &&
	  ( (x - (i+0.5)*l)/l <  1 + deltaTOF) &&
	  ( (x - (i+0.5)*l)/l > -1 + deltaTOF))
	{
	  *Q_TOP[i] = *Q_BOT[i] * exp(+2*y/abs_len_BOT[i]);

          #ifdef DEBUG_KAOS_TOF
	  std::cout << "TOP ADC MISSING IN PADDLE " << i << std::endl;
          std::cout << "EXTRAPOLATED TOP ADC: " << *Q_TOP[i] << std::endl;
          #endif
	}
      
      // MISSING BOTTOM ADC
      if (*Q_BOT[i] == 0 && *Q_TOP[i] > 0 &&
	  ( (x - (i+0.5)*l)/l <  1 + deltaTOF) && 
	  ( (x - (i+0.5)*l)/l > -1 + deltaTOF))
	{
	  *Q_BOT[i] = *Q_TOP[i] * exp(-2*y/abs_len_TOP[i]); 

          #ifdef DEBUG_KAOS_TOF
	  std::cout << "BOT ADC MISSING IN PADDLE " << i << std::endl;
          std::cout << "EXTRAPOLATED BOT ADC: " << *Q_BOT[i] << std::endl;
          #endif
	}
#endif

    } // end of for loop

  return;
}

void 
KaosDetectors::calibrate_Aerogel(AquaNodeUShort **AC_ADC_TOP,
				 AquaNodeUShort **AC_ADC_BOT, 
				 AquaNodeDouble **AC_Q_TOP,
				 AquaNodeDouble **AC_Q_BOT, 
				 double *AC_ped_TOP, double *AC_ped_BOT, 
				 double *AC_gain_TOP, double *AC_gain_BOT,
				 double *AC_cor_TOP, double *AC_cor_BOT)
{
  double RAW;
  for (int i=0; i<6; i++)
    {
      // TOP ADC
      // for AC1
      if (AC_ADC_TOP[i]->ok() && (*AC_ADC_TOP[i]-AC_ped_TOP[i]) > 0)
	{
	  if (*AC_ADC_TOP[i] > 4095)  // ADC High Range correction
	    RAW = 8*(*AC_ADC_TOP[i]-4096) + AC_cor_TOP[i]-24;
	  else RAW = *AC_ADC_TOP[i];
	  *AC_Q_TOP[i] = (RAW-AC_ped_TOP[i])*AC_gain_TOP[i];
	} 
      else *AC_Q_TOP[i] = 0;

      // for AC2
      if (AC_ADC_TOP[i+6]->ok() && (*AC_ADC_TOP[i+6]-AC_ped_TOP[i+6]) > 0)
	{
	  if (*AC_ADC_TOP[i+6] > 4095)  // ADC High Range correction
	    RAW = 8*(*AC_ADC_TOP[i+6]-4096) + AC_cor_TOP[i+6]-24;
	  else RAW = *AC_ADC_TOP[i+6];
	  *AC_Q_TOP[i+6] = (RAW-AC_ped_TOP[i+6])*AC_gain_TOP[i+6];
	}
      else *AC_Q_TOP[i+6] = 0;

      // BOTTOM ADC
      // for AC1
      if (AC_ADC_BOT[i]->ok() && (*AC_ADC_BOT[i]-AC_ped_BOT[i]) > 0)
	{
	  if (*AC_ADC_BOT[i] > 4095)  // ADC High Range correction
	    RAW = 8*(*AC_ADC_BOT[i]-4096) + AC_cor_BOT[i]-24; 
	  else RAW = *AC_ADC_BOT[i];
	  *AC_Q_BOT[i] = (RAW-AC_ped_BOT[i])*AC_gain_BOT[i];
	}
      else *AC_Q_BOT[i] = 0;

      // for AC2
      if (AC_ADC_BOT[i+6]->ok() && (*AC_ADC_BOT[i+6]-AC_ped_BOT[i+6]) > 0)
	{
	  if (*AC_ADC_BOT[i+6] > 4095)  // ADC High Range correction
	    RAW = 8*(*AC_ADC_BOT[i+6]-4096) + AC_cor_BOT[i+6]-24;
	  else RAW = *AC_ADC_BOT[i+6];
	  *AC_Q_BOT[i+6] = (RAW-AC_ped_BOT[i+6])*AC_gain_BOT[i+6];
	}
      else *AC_Q_BOT[i+6] = 0;

#ifdef DEBUG_KAOS_AEROGEL
      if (i==0) std::cout << "AC High Range correction" << std::endl;
      std::cout << "AC1T" << i << "  : " << AC_cor_TOP[i]
                << ",  AC1B" << i << "  : " << AC_cor_BOT[i]
                << ",  AC2T" << i << "  : " << AC_cor_TOP[i+6]
                << ",  AC2B" << i << "  : " << AC_cor_BOT[i+6] << std::endl;
#endif
    }

  return;
}

/*
void calibrate_AerogelTDC(AquaNodeUShort **TDC_TOP,
			  AquaNodeUShort **TDC_BOT, 
			  AquaNodeDouble **T_TOP,
			  AquaNodeDouble **T_BOT, 
			  double *TDC_off_TOP, double *TDC_off_BOT)
{
  return; 
}
*/

double KaosDetectors::calc_quality(double sigma, double delta, 
				   double delta_max)
{
  if (fabs(delta) <= fabs(delta_max)) return 1.0;
    else
    {
      delta = fabs(delta)-fabs(delta_max);
      double width = sigma*1.0/sqrt(2); 
      // quality == exp(-1) for delta = sigma
      double exponent = - pow(delta,2) / (2 * pow(width,2));
      if (exponent < -50) return 0;
      else return exp(exponent);
    }
}

// ========================================================

int
KaosDetectors::check_trigger(unsigned short paddleH, unsigned short paddleG)
{
  if (paddleH >= 0 && paddleH < 30 && paddleG >= 0 && paddleG < 30 && 
      (use_trigger == 0 || triggerGH[paddleG][paddleH] == 1) ) return 1;
  else return 0;
}

int
KaosDetectors::check_trigger(unsigned short paddleG, unsigned short paddleI, 
			     unsigned short paddleH)
{
  if (paddleH >= 0 && paddleH < 30 && 
      paddleG >= 0 && paddleG < 15 && 
      paddleI >= 0 && paddleI < 15 && 
      (use_trigger == 0 || triggerGIH[paddleG][paddleI][paddleH] == 1) ) 
    return 1;
  else return 0;
}

double KaosDetectors::get_theta(double positionxH, double positionxG)
{
  double theta = atan( ( positionxH * cos(alphaH/degree) + (xLH-xLG)
			     - positionxG * cos(alphaG/degree) )
			   /( positionxH * sin(alphaH/degree) + (zLH-zLG) - 
			      positionxG * sin(alphaG/degree) ) ) *degree;
  return theta;
}

double KaosDetectors::get_phi(double positionyH, double positionyG, 
			      double positionxH, double positionxG)
{
  double phi_TOF = atan( (positionyH - positionyG)/sqrt(pow(positionxH * cos(alphaH/degree)+
			 (xLH-xLG) - positionxG * cos(alphaG/degree),2)
			 +pow(positionxH * sin(alphaH/degree) + (zLH-zLG) -
			      positionxG * sin(alphaG/degree),2 ) ) )*degree;
  return phi_TOF;
}

double KaosDetectors::get_tracklength_GH(double positionxH, double positionxG, double phi)
{  
  double tracklength = sqrt( pow(positionxH * cos(alphaH/degree) + (xLH-xLG) -
				  positionxG * cos(alphaG/degree),2 )
			      +pow(positionxH * sin(alphaH/degree) + (zLH-zLG) -
				   positionxG * sin(alphaG/degree),2 ) )
    / cos(phi/degree);

  return tracklength;
}

double KaosDetectors::get_tracklength_IH(double positionxH, double positionxI, double phi)
{  
  double tracklength = sqrt( pow(positionxH * cos(alphaH/degree) + (xLH-xLI) -
				  positionxI * cos(alphaI/degree),2 )
			      +pow(positionxH * sin(alphaH/degree) + (zLH-zLI) -
				   positionxI * sin(alphaI/degree),2 ) )
    / cos(phi/degree);

  return tracklength;
}

void KaosDetectors::gen_track_list(t_group wallH, t_group wallG, t_group wallI)
{
  struct t_track element;
  double Hx, Hy, Gx, Gy, scintpathH, scintpathG;
  double dEdxH, dEdxG, TimeH, TimeG, TimeI;
  unsigned short paddleH, paddleG, paddleI;
  double TAGy=0;
  double AC1x=0, LenGAC1z=0, AC1y=0, AC1Hit=0, AC1seg=0;
  double AC2x=0, LenGAC2z=0, AC2y=0, AC2Hit=0, AC2seg=0;
  int AC1segment, AC2segment;

  event.track.clear();

  // INITIALIZE QUALITIES
	  
  element.q_tagger   = 0;
  element.q_aerogel  = 0;
  element.q_theta    = 0;
  element.q_phi      = 0;
  element.q_dEdxG    = 0;
  element.q_dEdxI    = 0;
  element.q_dEdxH    = 0;
  element.q_TOF      = 0;
  element.q_TOFI     = 0;
  element.q_GIx      = 0;
  element.q_xHx      = 0;
  element.q_xGx      = 0;
  element.quality    = 0;

  for (int h=0; h< wallH.cnt_cluster; h++) // WALL H
    {
      element.id[0] = h;

      Hx = (wallH.cluster[h].center_of_charge + 0.5) * lH;
      Hy = wallH.cluster[h].delta_time;

      element.xH = Hx;
      element.yH = Hy;

      TimeH = wallH.cluster[h].mean_time;

      paddleH = wallH.cluster[h].max_adc;
	  
      for (int g=0; g < wallG.cnt_cluster; g++) // WALL G
	{
	  element.id[1] = g;
	 
	  Gx = (wallG.cluster[g].center_of_charge + 0.5) * lG;
	  Gy = wallG.cluster[g].delta_time;

	  element.xG = Gx;
	  element.yG = Gy;

	  element.theta     = get_theta(Hx, Gx);
	  element.phi       = get_phi(Hy, Gy, Hx, Gx);
	  element.length_GH = get_tracklength_GH(Hx, Gx, element.phi);

	  scintpathH = sqrt( 1 + pow(tan((element.theta + alphaH)/degree),2) 
			     + pow(tan(element.phi/degree),2) ) * 2;
	  
	  scintpathG = sqrt( 1 + pow(tan((element.theta + alphaG)/degree),2) 
			     + pow(tan(element.phi/degree),2) ) * 2;

	  dEdxH = wallH.cluster[h].charge/scintpathH*HscaleADC2MeV;
	  element.dEdxH = dEdxH;

	  dEdxG = wallG.cluster[g].charge/scintpathG*GscaleADC2MeV;
	  element.dEdxG = dEdxG;

	  TimeG = wallG.cluster[g].mean_time;

	  if (TimeG > 0 && TimeH > 0) 
	    element.TOF_GH = (TimeG-TimeH)/element.length_GH*1000;
	  else element.TOF_GH = 0;

	  paddleG = wallG.cluster[g].max_adc;

	  // AEROGEL AC1 (SLOWENIAN)

	  if (use_track == 1 || use_track == 4) // use MWPC tracking
	    {	   
	      AC1x = (hitLx->scalar() - xLAC1 + tan(theta->scalar()/degree)*zLAC1 )* 
		cos(theta->scalar()/degree) / 
		cos( (theta->scalar() + alphaG)/degree);

	      // the following lines need to be adapted to MWPC tracking...
	      LenGAC1z = sqrt(pow( AC1x + (xLAC1-xLG) - Gx * cos(alphaG/degree), 2)
			     + pow( AC1x + (zLAC1-zLG) - Gx * sin(alphaG/degree), 2));
	      AC1y = Gy + yG + tan(phi->scalar()/degree)*LenGAC1z;
	  }
	  else if (use_track == 2 || use_track == 3) // use TOF tracking
	    {
	      AC1x = (Gx - (xAC1-xG) + tan(element.theta/degree) * (zAC1-zG)) * 
		cos(element.theta/degree) / cos( (element.theta + alphaG)/degree);
	     
	      LenGAC1z = sqrt(pow( AC1x + (xAC1-xG) - Gx * cos(alphaG/degree), 2)
			     + pow( AC1x + (zAC1-zG) - Gx * sin(alphaG/degree), 2));
	      AC1y = Gy + yG + tan(element.phi/degree)*LenGAC1z;
	    }

	  AC1segment = 0; 
	  if ( (AC1x > AC1min[0] && AC1x < AC1max[0]) ) AC1segment += 1;
	  if ( (AC1x > AC1min[1] && AC1x < AC1max[1]) ) AC1segment += 2;
	  if ( (AC1x > AC1min[2] && AC1x < AC1max[2]) ) AC1segment += 4;
	  if ( (AC1x > AC1min[3] && AC1x < AC1max[3]) ) AC1segment += 8;
	  if ( (AC1x > AC1min[4] && AC1x < AC1max[4]) ) AC1segment += 16;
	  if ( (AC1x > AC1min[5] && AC1x < AC1max[5]) ) AC1segment += 32;

	  AC1Hit = 0; 
	  for (int iseg=0; iseg<6; iseg++)
	    {
	      if ( AC1segment & (int) pow(2,iseg) )
		{
		  if (AC1y < -100) AC1seg = qACb[iseg]->scalar();  
		  else if (AC1y > 100) AC1seg = qACt[iseg]->scalar();  
		  else if (AC1y <= 100 && AC1y >= -100) 
		    AC1seg = qACb[iseg]->scalar() + qACt[iseg]->scalar();  
		}
	      if (AC1seg >= AC1Hit) AC1Hit = AC1seg; 
	    }

	  if (AC1segment > 0) element.AC1segment = log2(AC1segment); 
	  else element.AC1segment = -1;

	  element.AC1HitADC = AC1Hit;
	  element.AC1x = AC1x;
	  element.AC1y = AC1y;

	  // AEROGEL AC2 (JAPANESE)

	  if (use_track == 1 || use_track == 4) // use MWPC tracking
	    {	   
	      AC2x = (hitLx->scalar() - xLAC2 + tan(theta->scalar()/degree)*zLAC2 )* 
		cos(theta->scalar()/degree) / 
		cos( (theta->scalar() + alphaG)/degree);
	      
	      // the following lines need to be adapted to MWPC tracking...
	      LenGAC2z = sqrt(pow( AC2x + (xLAC2-xLG) - Gx * cos(alphaG/degree), 2)
			      + pow( AC2x + (zLAC2-zLG) - Gx * sin(alphaG/degree), 2));
	      AC2y = Gy + yG + tan(phi->scalar()/degree)*LenGAC2z;
	    }
	  else if (use_track == 2 || use_track == 3) // use TOF tracking
	    {
	      AC2x = (Gx - (xAC2-xG) + tan(element.theta/degree) * (zAC2-zG)) * 
		cos(element.theta/degree) / cos( (element.theta + alphaG)/degree);
	      
	      LenGAC2z = sqrt(pow( AC2x + (xAC2-xG) - Gx * cos(alphaG/degree), 2)
			      + pow( AC2x + (zAC2-zG) - Gx * sin(alphaG/degree), 2));
	      AC2y = Gy + yG + tan(element.phi/degree)*LenGAC2z;
		}
	  
	  AC2segment = 0; 
	  if ( (AC2x > AC2min[0] && AC2x < AC2max[0]) ) AC2segment += 1;
	  if ( (AC2x > AC2min[1] && AC2x < AC2max[1]) ) AC2segment += 2;
	  if ( (AC2x > AC2min[2] && AC2x < AC2max[2]) ) AC2segment += 4;
	  if ( (AC2x > AC2min[3] && AC2x < AC2max[3]) ) AC2segment += 8;
	  if ( (AC2x > AC2min[4] && AC2x < AC2max[4]) ) AC2segment += 16;
	  if ( (AC2x > AC2min[5] && AC2x < AC2max[5]) ) AC2segment += 32;
	  
	  AC2Hit = 0; 
	  for (int iseg=0; iseg<6; iseg++)
	    {
	      if ( AC2segment & (int) pow(2,iseg) )
		{
		  AC2seg = qACb[iseg+6]->scalar() + qACt[iseg+6]->scalar();  
		}
	      if (AC2seg >= AC2Hit) AC2Hit = AC2seg; 
	    }
	  
	  if (AC2segment > 0) element.AC2segment = log2(AC2segment); 
	  else element.AC2segment = -1;

	  element.AC2HitADC = AC2Hit;
	  element.AC2x = AC2x;
	  element.AC2y = AC2y;

	  // TAGGER

	  element.tagger = 0;
	  if (use_track == 1 || use_track == 4) // use MWPC tracking
	    TAGy= (hitLy->scalar() + yTAG + tan(phi->scalar()/degree)*zLTAG);
	  else if (use_track == 2 || use_track == 3) // use TOF tracking
	    {TAGy = wallG.cluster[g].delta_time - yG + yTAG +
		tan(element.phi/degree) * (zLTAG-zLG);
	    }

	  if (TAGy > yTAGTOP) element.tagger = 1;
	  else if (TAGy < yTAGBOT) element.tagger = 2;

	      
	  if (use_tof == 3)
	    for (int i=0; i < wallI.cnt_cluster; i++) // WALL I
	    {
	      element.id[2] = i;

	      element.xI = (wallI.cluster[i].center_of_charge + 0.5) * lI;
	      element.yI = wallI.cluster[i].delta_time;
	      element.dEdxI = wallI.cluster[i].charge/scintpathG*GscaleADC2MeV;
	      paddleI = wallI.cluster[i].max_adc;
	      element.length_IH = get_tracklength_IH(Hx, element.xI, element.phi);

	      TimeI = wallI.cluster[i].mean_time;
	      
	      if (TimeI > 0 && TimeH > 0) 
		element.TOF_IH = (TimeI-TimeH)/element.length_IH*1000;
	      else element.TOF_IH = 0;

	      // CHECK TRIGGER CONDITION

	      if ( check_trigger(paddleG, paddleI, paddleH) 
                // widened trigger condition to account for double hits
                || check_trigger(paddleG, paddleI - 1, paddleH) 
                || check_trigger(paddleG, paddleI + 1, paddleH) 
                // 
              )
		    event.track.push_back(element);
	    }
	  else if ( check_trigger(paddleH, paddleG) )
	    event.track.push_back(element);
	}
    }
}

void KaosDetectors::get_q_theta()
{
  double G,H, delta;

  for (std::list<struct t_track>::iterator it=event.track.begin(); 
       it!=event.track.end(); ++it)
    {
      G = wallG.cluster[it->id[1]].center_of_charge;
      H = wallH.cluster[it->id[0]].center_of_charge;
      delta = G - H * TOF_slope;
      
      it->q_theta = calc_quality(sigma_theta, delta, plateau_theta);      
    }
}
  
void KaosDetectors::get_q_phi()
{
  double yH, phi_exp, delta;

  for (std::list<struct t_track>::iterator it=event.track.begin(); 
       it!=event.track.end(); ++it)
    {
      yH        = wallH.cluster[it->id[0]].delta_time;
      phi_exp   = offset_phi + slope_phi * yH;
      delta     = phi_exp - it->phi;

      it->q_phi = calc_quality(sigma_phi, delta, plateau_phi);      
    }
}

void KaosDetectors::get_q_dEdxG()
{
  double G,delta;

  for (std::list<struct t_track>::iterator it=event.track.begin(); 
       it!=event.track.end(); ++it)
    {
      G = wallG.cluster[it->id[1]].center_of_charge;

      delta = it->dEdxG - (reference_dEdxG - G * dEdxG_slope);
      it->q_dEdxG = calc_quality(sigma_dEdxG, delta, plateau_dEdxG);      
    }
}

void KaosDetectors::get_q_dEdxI()
{
  double I,delta;

  for (std::list<struct t_track>::iterator it=event.track.begin(); 
       it!=event.track.end(); ++it)
    {
      I = wallI.cluster[it->id[2]].center_of_charge;

      delta = it->dEdxI - (reference_dEdxG - I * dEdxG_slope);
      it->q_dEdxI = calc_quality(sigma_dEdxG, delta, plateau_dEdxG);      
    }
}

void KaosDetectors::get_q_dEdxH()
{
  double H,delta;

  for (std::list<struct t_track>::iterator it=event.track.begin(); 
       it!=event.track.end(); ++it)
    {
      H = wallH.cluster[it->id[0]].center_of_charge;

      delta = it->dEdxH - (reference_dEdxH - H * dEdxH_slope);
      it->q_dEdxH = calc_quality(sigma_dEdxH, delta, plateau_dEdxH);      
    }
}
   
void KaosDetectors::get_q_TOF()
{
  double H, delta;

  for (std::list<struct t_track>::iterator it=event.track.begin(); 
       it!=event.track.end(); ++it)
    {
      // G = wallG.cluster[it->id[1]].center_of_charge;
      H = wallH.cluster[it->id[0]].center_of_charge;
      delta = it->TOF_GH - (reference_tof - H * deltat_slope);

      if (it->TOF_GH != 0)
	it->q_TOF = calc_quality(sigma_TOF, delta, plateau_TOF);      
    }
}

void KaosDetectors::get_q_TOFI()
{
  double H, delta;

  for (std::list<struct t_track>::iterator it=event.track.begin(); 
       it!=event.track.end(); ++it)
    {
      H = wallH.cluster[it->id[0]].center_of_charge;
      delta = it->TOF_IH - (reference_tof - H * deltat_slope);

      if (it->TOF_IH != 0)
	it->q_TOFI = calc_quality(sigma_TOF, delta, plateau_TOF);      
    }
}

void KaosDetectors::get_q_tagger()
{
  double TaggerTopIn, TaggerTopOut, TaggerBotIn, TaggerBotOut;
  double TaggerTop, TaggerBot;

  if (use_tagger)
    {
      TaggerTopIn  = qGb[29]->scalar();
      TaggerTopOut = qGt[29]->scalar();
      TaggerBotIn  = qGb[28]->scalar();
      TaggerBotOut = qGt[28]->scalar();

      TaggerTop = sqrt(TaggerTopIn*TaggerTopOut);
      TaggerBot = sqrt(TaggerBotIn*TaggerBotOut);

      for (std::list<struct t_track>::iterator it=event.track.begin(); 
	   it!=event.track.end(); ++it)
	{
	  if ( (it->tagger == 1 && TaggerTop > TagThreshold) ||
	       (it->tagger == 2 && TaggerBot > TagThreshold) )
	    it->q_tagger = 1; 
	}
    }
}

void KaosDetectors::get_q_aerogel()
{
  for (std::list<struct t_track>::iterator it=event.track.begin(); 
       it!=event.track.end(); ++it)
    {
      it->q_aerogel = calc_quality(ACThreshold, it->AC1HitADC, ACThreshold);
      if (use_aerogel == 2)
	{
	  it->q_aerogel *= calc_quality(ACThreshold, it->AC2HitADC, ACThreshold);
	}
    }
}

void KaosDetectors::get_q_GIx()
{
  double xG, xI;

  for (std::list<struct t_track>::iterator it=event.track.begin(); 
       it!=event.track.end(); ++it)
    {
      xG = wallG.cluster[it->id[1]].center_of_charge;
      xI = wallI.cluster[it->id[2]].center_of_charge;
      
      it->q_GIx = calc_quality(deltaTOF, xG - xI, 1);      
    }
}

void KaosDetectors::get_q_MWPC(double paddleH_exp, double paddleG_exp)
{
  double xH, xG;

  for (std::list<struct t_track>::iterator it=event.track.begin(); 
       it!=event.track.end(); ++it)
    {
      if (use_mwpc) 
	{
	  xH = wallH.cluster[it->id[0]].center_of_charge;
	  xG = wallG.cluster[it->id[1]].center_of_charge;
	  
	  it->q_xHx = calc_quality(deltaTOF, xH - paddleH_exp, 1);      
	  it->q_xGx = calc_quality(deltaTOF, xG - paddleG_exp, 1);      
	}
      else
	{
	  it->q_xHx = 0;
	  it->q_xGx = 0;
	}
    }
}

void KaosDetectors::calc_total_quality()
{
  for (std::list<t_track>::iterator it=event.track.begin(); 
       it!=event.track.end(); ++it)
  {
    it->quality = 
      it->q_theta
      * it->q_phi
      * it->q_dEdxG
      * it->q_dEdxH
      * it->q_TOF
      ;
    if (use_tof == 3)
      it->quality *= it->q_TOFI * it->q_dEdxI * it->q_GIx;

    if (use_track == 1 || use_track == 4) // use MWPC tracking
      {
      it->quality
 	 = it->q_xHx
	 * it->q_xGx
	;
      }

    if (use_tagger) it->quality *= it->q_tagger;
    if (use_aerogel) it->quality *= it->q_aerogel;
  }
}

bool KaosDetectors::sortby_quality(struct t_track first, 
					struct t_track second)
{
  if (first.quality > second.quality) return true;
  else return false;
}

int 
KaosDetectors::handle()
{
  if ((use_multitrack != 1) || (event.track.size() == 0))
  {
    #ifdef DEBUG_KAOS_TOF
    std::cout << "###################### NEXT EVENT ######################" 
              << std::endl;
    #endif

    // init values
    double Hy= -1, Hz, Hzprime, Hx= 0, Hxprime;
    double Gy= -1, Gz, Gzprime, Gx= 0, Gxprime;
    double paddleH_exp= -100, paddleG_exp= -100;

    if (use_mwpc) 
      {
        // calculate hit position in wall H coordinates (mm)
        Hxprime = (hitLx->scalar() - xLH + tan(theta->scalar()/degree)*zLH);
        Hx = Hxprime * cos(theta->scalar()/degree)/
          cos((theta->scalar()+alphaH)/degree);

        Hzprime = zLH + tan(alphaH/degree)*(hitLx->scalar()-xLH);
        Hz= Hzprime * cos(alphaH/degree)/cos((alphaH+theta->scalar())/degree);

        // calculate y position in wall H coordinates (mm)
        Hy = hitLy->scalar() - yH + tan(phi->scalar()/degree)*Hz;

        // calculate hit paddle in wall H
        paddleH_exp = Hx/lH - 0.5;

        // calculate hit position in wall G coordinates (mm)
        Gxprime = hitLx->scalar() - xLG + tan(theta->scalar()/degree)*zLG;
        Gx =  Gxprime * cos(theta->scalar()/degree)
          /cos((theta->scalar()+alphaG)/degree);

        Gzprime = zLG + tan(alphaG/degree)*(hitLx->scalar()-xLG);
        Gz= Gzprime * cos(alphaG/degree)/cos((alphaG+theta->scalar())/degree);

        // calculate y position in wall G coordinates (mm)
        Gy = hitLy->scalar() - yG + tan(phi->scalar()/degree)*Gz;

        // calculate hit paddle in wall G
        paddleG_exp = Gx/lG - 0.5;
      }
    else 
      {
        Hx = -1000; Gx = -1000; Hy = -1000; Gy = -1000;
      }

    // calibrat ADC for pedestals, HighRange, absorption and gain
    calibrate_ADC(adcft, adcfb, qHt, qHb,       
                  ADC_ped_HTOP, ADC_ped_HBOT, ADC_gain_HTOP, ADC_gain_HBOT, 
                  ADC_cor_HTOP, ADC_cor_HBOT, absorption_HTOP, absorption_HBOT, 
                  Hx, Hy, lH);
    calibrate_ADC(adcgt, adcgb, qGt, qGb, 
                  ADC_ped_GTOP, ADC_ped_GBOT, ADC_gain_GTOP, ADC_gain_GBOT, 
                  ADC_cor_GTOP, ADC_cor_GBOT, absorption_GTOP, absorption_GBOT,
                  Gx, Gy, lG);  

    // calibrate TDC for HighRange, offsets
    calibrate_TDC(tdcft, tdcfb, tHt, tHb, TDC_off_HTOP, TDC_off_HBOT,
                  TDC_cor_HTOP, TDC_cor_HBOT, 
                  Hx, Hy, lH, time_offsetH, time_slopeH, 0, adcft, adcfb, ADC_cor_HTOP, ADC_cor_HBOT);
    calibrate_TDC(tdcgt, tdcgb, tGt, tGb, TDC_off_GTOP, TDC_off_GBOT,
                  TDC_cor_GTOP, TDC_cor_GBOT, 
                  Gx, Gy, lG, time_offsetG, time_slopeG, 1, adcgt, adcgb, ADC_cor_GTOP, ADC_cor_GBOT);

    // calibrate Aerogel for pedestals and gain
    calibrate_Aerogel(adcat, adcab, qACt, qACb,
                      Aerogel_ped_TOP, Aerogel_ped_BOT,
                      Aerogel_gain_TOP, Aerogel_gain_BOT,
                      Aerogel_cor_TOP, Aerogel_cor_BOT);
//tmptmp
//    calibrate_AerogelTDC(tdcft, tdcfb, tHt, tHb, TDC_off_HTOP, TDC_off_HBOT,
//                  TDC_cor_HTOP, TDC_cor_HBOT, 
//                  Hx, Hy, lH, time_offsetH, time_slopeH, 0, adcft, adcfb, ADC_cor_HTOP, ADC_cor_HBOT);

    // xtra (trigger) channels in TDC
    double RAW;
    for (int i=0; i<8; i++)
      {
        if (tdcx[i]->ok() && *tdcx[i] > 0)
          {
            if (*tdcx[i] > 4095)  // TDC High Range correction
              RAW = (*tdcx[i]-4095)*8+TDC_cor_x[i]-24;
            else RAW = *tdcx[i];
            time_xtra[i]->handle(RAW);
          }
        else time_xtra[i]->handle(0);
      }

    // xtra channels in ADC (efficiency counters and others)
    for (int i=0; i<8; i++)
      {
        if (adcx[i]->ok() && *adcx[i] > 0)
          {
            if (*adcx[i] > 4095)  // ADC High Range correction
              RAW = 8*(*adcx[i]-4096) + ADC_cor_x[i]-24 - ADC_ped_x[i];
            else RAW = *adcx[i] - ADC_ped_x[i];
            charge_xtra[i]->handle(RAW);
          }
      }

    // fill hit_array and get number of entries
    #ifdef DEBUG_KAOS_TOF
    std::cout << "wallH:" << std::endl;
    #endif
    fill_hit_array(tHt, tHb, qHt, qHb, time_offsetH, time_slopeH, wallH, 0, 29);
    #ifdef DEBUG_KAOS_TOF
    std::cout << "wallG:" << std::endl;
    #endif
    if (use_tagger)
      fill_hit_array(tGt, tGb, qGt, qGb, time_offsetG, time_slopeG, wallG, 0, 27);
    else if (use_tof == 3) // wall I
      {
        fill_hit_array(tGt, tGb, qGt, qGb, time_offsetG, time_slopeG, wallG, 0, 14);
        #ifdef DEBUG_KAOS_TOF
        std::cout << "wallI:" << std::endl;
        #endif
        fill_hit_array(tGt, tGb, qGt, qGb, time_offsetG, time_slopeG, wallI, 15, 29);
      }
    else
      fill_hit_array(tGt, tGb, qGt, qGb, time_offsetG, time_slopeG, wallG, 0, 29);

    // count TDC cluster
    cnt_cluster(wallH);
    cnt_cluster(wallG);
    cnt_cluster(wallI);

    // analyse cluster
    get_cluster_data(wallH, wallH.ind_thr);
    get_cluster_data(wallG, wallG.ind_thr);
    get_cluster_data(wallI, wallG.ind_thr);

    #ifdef DEBUG_KAOS_TOF_HIT_ARRAY
    std::cout << "====== DEBUG_KAOS_TOF_HIT_ARRAY" << std::endl;
    std::cout << "wallH:  N_hits=" << wallH.N_hits 
              << "\tcnt_cluster=" << wallH.cnt_cluster 
              << std::endl;
    for (int i=0; i<wallH.N_hits; i++)
    {
      std::cout << "\tpaddle=" << wallH.hits[i].paddle
                << "\ttdc mean=" << wallH.hits[i].tdc_mean
                << "\ttdc delta=" << wallH.hits[i].tdc_delta
                << "\tadc gmean=" << wallH.hits[i].adc_gmean
                << "\tcluster_id=" << wallH.hits[i].cluster_id
                << std::endl;
    }
    std::cout << "wallG:  N_hits=" << wallG.N_hits 
              << "\tcnt_cluster=" << wallG.cnt_cluster 
              << std::endl;
    for (int i=0; i<wallG.N_hits; i++)
    {
      std::cout << "\tpaddle=" << wallG.hits[i].paddle
                << "\ttdc mean=" << wallG.hits[i].tdc_mean
                << "\ttdc delta=" << wallG.hits[i].tdc_delta
                << "\tadc gmean=" << wallG.hits[i].adc_gmean
                << "\tcluster_id=" << wallG.hits[i].cluster_id
                << std::endl;
    }
    if (use_tof == 3)
      {
        std::cout << "wallI:  N_hits=" << wallI.N_hits 
                  << "\tcnt_cluster=" << wallI.cnt_cluster 
                  << std::endl;
        for (int i=0; i<wallI.N_hits; i++)
          {
            std::cout << "\tpaddle=" << wallI.hits[i].paddle
                      << "\ttdc mean=" << wallI.hits[i].tdc_mean
                      << "\ttdc delta=" << wallI.hits[i].tdc_delta
                      << "\tadc gmean=" << wallI.hits[i].adc_gmean
                      << "\tcluster_id=" << wallI.hits[i].cluster_id
                      << std::endl;
          }
      }
    #endif

    #ifdef DEBUG_KAOS_TOF_CLUSTER
    std::cout << "====== DEBUG_KAOS_TOF_CLUSTER" << std::endl;
    std::cout << "wallH:  cnt_cluster=" << wallH.cnt_cluster << std::endl;
    for (int i=0; i<wallH.cnt_cluster; i++)
    {
      std::cout << "\tcluster_id=" << wallH.cluster[i].cluster_id
                << "\thit_cnt=" << wallH.cluster[i].hit_cnt
                << "\tmax_adc=" << wallH.cluster[i].max_adc
                << "\tmean_time=" << wallH.cluster[i].mean_time
                << "\tcharge=" << wallH.cluster[i].charge
                << "\tcenter_of_charge=" << wallH.cluster[i].center_of_charge
                << std::endl;
    }
    std::cout << "wallG:  cnt_cluster=" << wallG.cnt_cluster << std::endl;
    for (int i=0; i<wallG.cnt_cluster; i++)
    {
      std::cout << "\tcluster_id=" << wallG.cluster[i].cluster_id
                << "\thit_cnt=" << wallG.cluster[i].hit_cnt
                << "\tmax_adc=" << wallG.cluster[i].max_adc
                << "\tmean_time=" << wallG.cluster[i].mean_time
                << "\tcharge=" << wallG.cluster[i].charge
                << "\tcenter_of_charge=" << wallG.cluster[i].center_of_charge
                << std::endl;
    }
    if (use_tof == 3)
      {
        std::cout << "wallI:  cnt_cluster=" << wallI.cnt_cluster << std::endl;
        for (int i=0; i<wallI.cnt_cluster; i++)
          {
            std::cout << "\tcluster_id=" << wallI.cluster[i].cluster_id
                      << "\thit_cnt=" << wallI.cluster[i].hit_cnt
                      << "\tmax_adc=" << wallI.cluster[i].max_adc
                      << "\tmean_time=" << wallI.cluster[i].mean_time
                      << "\tcharge=" << wallI.cluster[i].charge
                      << "\tcenter_of_charge=" << wallI.cluster[i].center_of_charge
                      << std::endl;
          }
      }
    #endif

    // select cluster
    gen_track_list(wallH, wallG, wallI);
  
      
    if (event.track.size() > 0)
    {
      get_q_theta();
      get_q_phi();
      get_q_dEdxG();
      get_q_dEdxI();
      get_q_dEdxH();
      get_q_TOF();
      get_q_TOFI();
      get_q_GIx();
      get_q_tagger();
      get_q_aerogel();
      get_q_MWPC(paddleH_exp, paddleG_exp);
      calc_total_quality();
      event.track.sort(sortby_quality);
    }
    tracksInEvent = event.track.size();
  }
  
  if (event.track.size() > 0)
  {
    std::list<t_track>::iterator it;
    
    #ifdef DEBUG_KAOS_TOF_QUALITY
    std::cout << "====== DEBUG_KAOS_TOF_QUALITY ======" << std::endl;
    for (it=event.track.begin(); it!=event.track.end(); ++it)
      {
        std::cout << "\tid=" << it->id[0] << it->id[1] 
                  << "\ttheta=" <<  it->theta
          //		    << "\ttheta MWPC=" <<  theta->scalar()
                  << "\tq_theta=" <<  it->q_theta
                  << "\tphi=" <<  it->phi
          //		    << "\tphi MWPC=" <<  phi->scalar()
                  << "\tq_phi=" <<  it->q_phi
                  << "\tdEdxH=" <<  it->dEdxH
                  << "\tdEdxG=" <<  it->dEdxG
                  << "\tq_dEdxG=" <<  it->q_dEdxG
                  << "\tq_dEdxH=" <<  it->q_dEdxH
                  << "\tq_dEdxI=" <<  it->q_dEdxI
                  << "\tTOF=" <<  it->TOF_GH
                  << "\tq_TOF=" <<  it->q_TOF
                  << "\tq_aero=" <<  it->q_aerogel
                  << "\tq_tagger=" <<  it->q_tagger
                  << "\tquality=" <<  it->quality
                  << std::endl;
      }
    #endif

    // choose the best track
    it      = event.track.begin();
    int idH = it->id[0];
    int idG = it->id[1];
    int idI = it->id[2];

    #ifdef DEBUG_KAOS_TOF_QUALITY
    std::cout << "====== DEBUG_KAOS_TOF_QUALITY" << std::endl;
    std::cout << "\twallH id= " << idH
              << "wallG id= " << idG
              << std::endl;
    #endif

    // return cluster size
    clusterSizeH->handle(wallH.cluster[idH].hit_cnt);
    clusterSizeG->handle(wallG.cluster[idG].hit_cnt);
    if (use_tof == 3) clusterSizeI->handle(wallI.cluster[idI].hit_cnt);

    // return cluster information
    paddleH->handle(wallH.cluster[idH].center_of_charge);
    paddleG->handle(wallG.cluster[idG].center_of_charge);
    if (use_tof == 3) paddleI->handle(wallI.cluster[idI].center_of_charge);

    timeH->handle(wallH.cluster[idH].mean_time);
    timeG->handle(wallG.cluster[idG].mean_time);
    if (use_tof == 3) timeI->handle(wallI.cluster[idI].mean_time);

    ypositionH->handle(wallH.cluster[idH].delta_time);
    ypositionG->handle(wallG.cluster[idG].delta_time);
    if (use_tof == 3) ypositionI->handle(wallI.cluster[idI].delta_time);

    chargeH->handle(wallH.cluster[idH].charge*HscaleADC2MeV);
    chargeG->handle(wallG.cluster[idG].charge*GscaleADC2MeV);
    if (use_tof == 3) chargeI->handle(wallI.cluster[idI].charge*GscaleADC2MeV);

    xpositionH->handle(it->xH);
    xpositionG->handle(it->xG);
    if (use_tof == 3) xpositionI->handle(it->xI);

    dEdxH->handle(it->dEdxH);
    dEdxG->handle(it->dEdxG);
    if (use_tof == 3) dEdxI->handle(it->dEdxI);

    deltaTimeClusterH->handle(wallH.cluster[idH].delta_time_cluster);
    deltaTimeClusterG->handle(wallG.cluster[idG].delta_time_cluster);
    if (use_tof == 3) deltaTimeClusterI->handle(wallH.cluster[idI].delta_time_cluster);
    
    
    // return track information
    theta_tof->handle(it->theta);
    phi_tof->handle(it->phi);

    tracklengthGH_tof->handle(it->length_GH);
    tracklengthIH_tof->handle(it->length_IH);
    flighttimeGH_tof->handle(it->TOF_GH);
    flighttimeIH_tof->handle(it->TOF_IH);

//       if (fabs(it->dEdxH - it->dEdxG) < 1 &&
// 	  it->dEdxH > 0 && it->dEdxG > 0)
// 	dEdx_tof->handle(it->dEdxG);
    AC1segment_tof->handle(it->AC1segment);
    AC1HitADC_tof->handle(it->AC1HitADC);
    AC1xposition_tof->handle(it->AC1x);
    AC1yposition_tof->handle(it->AC1y);

    AC2segment_tof->handle(it->AC2segment);
    AC2HitADC_tof->handle(it->AC2HitADC);
    AC2xposition_tof->handle(it->AC2x);
    AC2yposition_tof->handle(it->AC2y);

    tagger_tof->handle(it->tagger);

    // return quality assignments
    q_tagger   -> handle(it->q_tagger);
    q_aerogel  -> handle(it->q_aerogel);
    q_theta    -> handle(it->q_theta);
    q_phi      -> handle(it->q_phi);
    if (use_tof == 3) q_dEdx     -> handle(it->q_dEdxG * it->q_dEdxH * it->q_dEdxI);
    else q_dEdx     -> handle(it->q_dEdxG * it->q_dEdxH);
    if (use_tof == 3) q_TOF      -> handle(it->q_TOF * it->q_TOFI);
    else  q_TOF      -> handle(it->q_TOF);
    q_GIx      -> handle(it->q_GIx);
    q_xHx      -> handle(it->q_xHx);
    q_xGx      -> handle(it->q_xGx);
    q_total    -> handle(it->quality);
    
  } // cluster > 0

  // return minimum information
  tracks_tof->handle(tracksInEvent);
  trackID_tof->handle(event.track.size() - 1);
  
  clusterH->handle(wallH.cnt_cluster);
  clusterG->handle(wallG.cnt_cluster);
  if (use_tof == 3) clusterI->handle(wallI.cnt_cluster);
  
  // full debug information
  if (kaos_debug == 2) {
    std::cout << "--- DEBUG BEGIN ---"       << std::endl;
    std::cout << "H-> N (&&): " << *clusterSizeH << std::endl;
    std::cout << "H-> N_cluster" << *clusterH << std::endl;
    std::cout << "H-> cntr of charge: " << *xpositionH << std::endl;

    std::cout << "G-> N (&&): "  << *clusterSizeG << std::endl;
    std::cout << "G-> N_cluster" << *clusterG << std::endl;
    std::cout << "G-> cntr of charge: " << *xpositionG << std::endl;

    if (use_tof == 3) {
      std::cout << "I-> N (&&): "  << *clusterSizeI << std::endl;
      std::cout << "I-> N_cluster" << *clusterI << std::endl;
      std::cout << "I-> cntr of charge: " << *xpositionI << std::endl;
    }

    for (int i=0;i<30;i++) {
      std::cout << "------- " << i << " -------" << std::endl;
      std::cout << "-------------------"       << std::endl;
      std::cout << "H TDC TOP:  " << *tdcft[i] << std::endl;
      std::cout << "H TDC BOT:  " << *tdcfb[i] << std::endl;
      std::cout << "H ADC TOP:  " << *adcft[i] << std::endl;
      std::cout << "H ADC BOT:  " << *adcfb[i] << std::endl;
      std::cout << "-------------------"       << std::endl;
      std::cout << "G TDC TOP:  " << *tdcgt[i] << std::endl;
      std::cout << "G TDC BOT:  " << *tdcgb[i] << std::endl;
      std::cout << "G ADC TOP:  " << *adcgt[i] << std::endl;
      std::cout << "G ADC BOT:  " << *adcgb[i] << std::endl;
      std::cout << "-------------------"       << std::endl;
    }
    std::cout << "--- DEBUG END -----"       << std::endl;
  }

  if (use_multitrack == 1)
  {
    if (event.track.size() > 0) 
      event.track.pop_front();
    //std::cout << event.track.size() << std::endl;
    return event.track.size();
  }
  return 0;
}
