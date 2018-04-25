//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
// 
// Multitrack analysis for the Kaos Spectrometer
// activated by run.bd switch: kaos.use_multi_track = 1
//
// Copyright (c) 2004
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// 
//

#include "Aqua/AquaTree.h"
#include "KaosMultiTrack.h"
#include <cmath>



KaosMultiTrack::KaosMultiTrack(AquaTree *aquatree, AquaTree *outtree)
  : KaosDetectors(aquatree, outtree)
{
  kaos_debug = 0;  // DEBUGGING OUTPUT {0|1|2|3}

  //////////////////////////////////
  // ABCN
  //////////////////////////////////
  // input data from wall H
  AquaNode **Ntdcft = (AquaNode **)tdcft;
  atree->multifind(Ntdcft, 0, 29, "abcn.kaos.tof.fdet[%d].top.time");
  AquaNode **Ntdcfb = (AquaNode **)tdcfb;
  atree->multifind(Ntdcfb, 0, 29, "abcn.kaos.tof.fdet[%d].bottom.time");
  AquaNode **Nadcft = (AquaNode **)adcft;
  atree->multifind(Nadcft, 0, 29, "abcn.kaos.tof.fdet[%d].top.charge");
  AquaNode **Nadcfb = (AquaNode **)adcfb;
  atree->multifind(Nadcfb, 0, 29, "abcn.kaos.tof.fdet[%d].bottom.charge");

  // input data from wall G (and wall I in three wall setup)
  AquaNode **Ntdcgt = (AquaNode **)tdcgt;
  atree->multifind(Ntdcgt, 0, 29, "abcn.kaos.tof.gdet[%d].top.time");
  AquaNode **Ntdcgb = (AquaNode **)tdcgb;
  atree->multifind(Ntdcgb, 0, 29, "abcn.kaos.tof.gdet[%d].bottom.time");
  AquaNode **Nadcgt = (AquaNode **)adcgt;
  atree->multifind(Nadcgt, 0, 29, "abcn.kaos.tof.gdet[%d].top.charge");
  AquaNode **Nadcgb = (AquaNode **)adcgb;
  atree->multifind(Nadcgb, 0, 29, "abcn.kaos.tof.gdet[%d].bottom.charge");

  // input data from aerogel Cherenkov
  AquaNode **Nadcat = (AquaNode **)adcat;
  atree->multifind(Nadcat, 0, 11, "abcn.kaos.aerogel.segment[%d].top.charge");
  AquaNode **Nadcab = (AquaNode **)adcab;
  atree->multifind(Nadcab, 0, 11, "abcn.kaos.aerogel.segment[%d].bottom.charge");

  AquaNode **Ntdcx = (AquaNode **)tdcx;
  atree->multifind(Ntdcx, 0, 7, "abcn.kaos.tof.xtra[%d].time");
  AquaNode **Nadcx = (AquaNode **)adcx;
  atree->multifind(Nadcx, 0, 7, "abcn.kaos.tof.xtra[%d].charge");

  // input data from MWPC
  /*AquaNode **snodeLx = (AquaNode **) Lx;
  atree->multifind(snodeLx, 0, 239, "abcn.kaos.mwpc.l.x[%d]");
  AquaNode **snodeMx = (AquaNode **) Mx;
  atree->multifind(snodeMx, 0, 239, "abcn.kaos.mwpc.m.x[%d]");
  AquaNode **snodeLy = (AquaNode **) Ly;
  atree->multifind(snodeLy, 0, 69, "abcn.kaos.mwpc.l.y[%d]");
  AquaNode **snodeMy = (AquaNode **) My;
  atree->multifind(snodeMy, 0, 69, "abcn.kaos.mwpc.m.y[%d]");*/

  ///////////////////////////////////
  // ONLINE
  ///////////////////////////////////
  
  eventNumberTracks  = (AquaNodeInt    *) online->find("online.kaos.tof.tracks");
  eventMultiplicityH = (AquaNodeInt    *) online->find("online.kaos.hdet.clusterSize"); // TODO implement for paddles + clusters
  eventMultiplicityG = (AquaNodeInt    *) online->find("online.kaos.gdet.clusterSize"); 
  
  trackPaddleH       = (AquaNodeDouble *) online->find("online.kaos.hdet.paddle"); //TODO output correct position instead of first paddle in cluster
  trackPaddleG       = (AquaNodeDouble *) online->find("online.kaos.gdet.paddle");
  
  trackPositionHx    = (AquaNodeDouble *) online->find("online.kaos.hdet.positionXZ");
  trackPositionHy    = (AquaNodeDouble *) online->find("online.kaos.hdet.positionY");
  trackPositionGx    = (AquaNodeDouble *) online->find("online.kaos.gdet.positionXZ");
  trackPositionGy    = (AquaNodeDouble *) online->find("online.kaos.gdet.positionY");
 
  trackTheta         = (AquaNodeDouble *) online->find("online.kaos.tof.theta");
  trackPhi           = (AquaNodeDouble *) online->find("online.kaos.tof.phi");
  
  trackLength        = (AquaNodeDouble *) online->find("online.kaos.tof.tracklength");
  
  trackDedxH         = (AquaNodeDouble *) online->find("online.kaos.hdet.dEdx");
  trackDedxG         = (AquaNodeDouble *) online->find("online.kaos.gdet.dEdx");

  trackDeH           = (AquaNodeDouble *) online->find("online.kaos.hdet.dE");
  trackDeG           = (AquaNodeDouble *) online->find("online.kaos.gdet.dE");
  
  trackTimeH         = (AquaNodeDouble *) online->find("online.kaos.hdet.time");
  trackTimeG         = (AquaNodeDouble *) online->find("online.kaos.gdet.time");
  trackToF           = (AquaNodeDouble *) online->find("online.kaos.tof.flighttime");

  if (use_tof == 3)
  {
    trackTimeI        = (AquaNodeDouble *) online->find("online.kaos.idet.time");
    trackDeI          = (AquaNodeDouble *) online->find("online.kaos.idet.dE");
    trackDedxI        = (AquaNodeDouble *) online->find("online.kaos.idet.dEdx");
    trackPositionIx   = (AquaNodeDouble *) online->find("online.kaos.idet.positionXZ");
    trackPositionIy   = (AquaNodeDouble *) online->find("online.kaos.idet.positionY");
    trackPaddleI      = (AquaNodeDouble *) online->find("online.kaos.idet.paddle");
    eventMultiplicityI= (AquaNodeInt    *) online->find("online.kaos.idet.clusterSize"); 
    trackToF_IH       = (AquaNodeDouble *) online->find("online.kaos.tof.flighttimeIH");
  }

  // information about other detectors
  trackAC1Segment     = (AquaNodeInt    *) online->find("online.kaos.tof.AC1segment");
  trackAC2Segment     = (AquaNodeInt    *) online->find("online.kaos.tof.AC2segment");

  // qualities
  q_tagger = (AquaNodeDouble *) online->find("online.kaos.tof.q_tagger");
  q_aerogel= (AquaNodeDouble *) online->find("online.kaos.tof.q_aerogel");
  q_phi    = (AquaNodeDouble *) online->find("online.kaos.tof.q_phi");
  q_theta  = (AquaNodeDouble *) online->find("online.kaos.tof.q_theta");
  q_dEdx   = (AquaNodeDouble *) online->find("online.kaos.tof.q_dEdx");
  q_TOF    = (AquaNodeDouble *) online->find("online.kaos.tof.q_TOF");
  q_xHx    = (AquaNodeDouble *) online->find("online.kaos.tof.q_xHx");
  q_xGx    = (AquaNodeDouble *) online->find("online.kaos.tof.q_xGx");
  q_total  = (AquaNodeDouble *) online->find("online.kaos.tof.quality");

  trackID            = (AquaNodeInt    *) online->find("online.kaos.multitrack.trackID");

  // information for tracks with two paddle hits
  /*track2PaddleHitH   = (AquaNodeInt    *) online->find("online.kaos.multitrack.twoPaddleHitH");
  track2PaddleHitG   = (AquaNodeInt    *) online->find("online.kaos.multitrack.twoPaddleHitG");
  trackDeltaHy       = (AquaNodeDouble *) online->find("online.kaos.multitrack.deltaPositionHy");
  trackDeltaGy       = (AquaNodeDouble *) online->find("online.kaos.multitrack.deltaPositionGy");
  trackDeltaTimeH    = (AquaNodeDouble *) online->find("online.kaos.multitrack.deltaTimeH");
  trackDeltaTimeG    = (AquaNodeDouble *) online->find("online.kaos.multitrack.deltaTimeG");

  // infromation for tracks next to two paddle hits
  trackNextToTwoPaddleHitH = (AquaNodeInt    *) online->find("online.kaos.multitrack.nextTo2PaddleHitH");
  trackDeltaTLowerH        = (AquaNodeDouble *) online->find("online.kaos.multitrack.deltaTLowerH");
  trackDeltaTHigherH       = (AquaNodeDouble *) online->find("online.kaos.multitrack.deltaTHigherH");
  trackDeltaYLowerH        = (AquaNodeDouble *) online->find("online.kaos.multitrack.deltaYLowerH");
  trackDeltaYHigherH       = (AquaNodeDouble *) online->find("online.kaos.multitrack.deltaYHigherH");
    
  trackNextToTwoPaddleHitG = (AquaNodeInt    *) online->find("online.kaos.multitrack.nextTo2PaddleHitG");
  trackDeltaTLowerG        = (AquaNodeDouble *) online->find("online.kaos.multitrack.deltaTLowerG");
  trackDeltaTHigherG       = (AquaNodeDouble *) online->find("online.kaos.multitrack.deltaTHigherG");
  trackDeltaYLowerG        = (AquaNodeDouble *) online->find("online.kaos.multitrack.deltaYLowerG");
  trackDeltaYHigherG       = (AquaNodeDouble *) online->find("online.kaos.multitrack.deltaYHigherG");
*/
  trackVector = new std::vector<t_track>;
  
}


KaosMultiTrack::~KaosMultiTrack()
{
  delete trackVector;
};

/*int 
KaosMultiTrack::fill_hit_array(AquaNodeDouble **T_TOP,
                              AquaNodeDouble **T_BOT,
                              AquaNodeDouble **Q_TOP,
                              AquaNodeDouble **Q_BOT, 
                              double *time_offset, double *time_slope,
                              t_wall &wall, int NoOfPaddles)
{
  unsigned short N_hit=0;

  for (int i = 0; i < NoOfPaddles; i++)
  {
    
    if ((*Q_TOP[i] > 0) && (*Q_BOT[i] > 0) &&
        (*T_TOP[i] > 0) && (*T_BOT[i] > 0))
    {
      wall.hits[N_hit].paddleNo           = i;
      wall.hits[N_hit].twoPaddleHit       = 0;
      wall.hits[N_hit].ADCMean            = sqrt(*Q_TOP[i] * *Q_BOT[i]);
      wall.hits[N_hit].TDCMean            = (*T_TOP[i] + *T_BOT[i])/2;
      wall.hits[N_hit].TDCDelta           = time_offset[i] + (*T_TOP[i] - *T_BOT[i]) * time_slope[i];
      wall.hits[N_hit].ADCMeanAdditional  = 0;
      wall.hits[N_hit].TDCMeanAdditional  = 0;
      wall.hits[N_hit].TDCDeltaAdditional = 0;
      wall.hits[N_hit].ADCAsymmetry       = 0;
      
      // find hits next to double hits
      if ((i > 0) && (*Q_TOP[i - 1] > 0) && (*Q_BOT[i - 1] > 0) && (*T_TOP[i - 1] > 0) && (*T_BOT[i - 1] > 0))
      {
        wall.hits[N_hit].deltaTlower  = (*T_TOP[i - 1] + *T_BOT[i - 1])/2 - wall.hits[N_hit].TDCMean;
        wall.hits[N_hit].deltaYlower  = time_offset[i - 1] + (*T_TOP[i - 1] - *T_BOT[i - 1]) * time_slope[i - 1] - wall.hits[N_hit].TDCDelta;
        wall.hits[N_hit].twoPaddleHit += 16; // set bit 5
      }
      
      if ((i + 1 < NoOfPaddles) && (*Q_TOP[i + 1] > 0) && (*Q_BOT[i + 1] > 0) && (*T_TOP[i + 1] > 0) && (*T_BOT[i + 1] > 0))
      {
        wall.hits[N_hit].deltaThigher = wall.hits[N_hit].TDCMean - (*T_TOP[i + 1] + *T_BOT[i + 1])/2;
        wall.hits[N_hit].deltaYhigher = wall.hits[N_hit].TDCDelta - (time_offset[i + 1] + (*T_TOP[i + 1] - *T_BOT[i + 1]) * time_slope[i + 1]);
        wall.hits[N_hit].twoPaddleHit += 32; // set bit 6
      }
      
      if (wall.hits[N_hit].TDCMean != 0)
      {
        N_hit++;
      }
      
      // find double cluster
      if ((i + 1 < NoOfPaddles) &&
          (*Q_TOP[i + 1] > 0) && (*Q_BOT[i + 1] > 0) &&
          (*T_TOP[i + 1] > 0) && (*T_BOT[i + 1] > 0))
      {
        wall.hits[N_hit].paddleNo           = i;
        wall.hits[N_hit].twoPaddleHit       = 1;
        wall.hits[N_hit].ADCMean            = sqrt(*Q_TOP[i] * *Q_BOT[i]);
        wall.hits[N_hit].TDCMean            = (*T_TOP[i] + *T_BOT[i])/2;
        wall.hits[N_hit].TDCDelta           = time_offset[i] + (*T_TOP[i] - *T_BOT[i]) * time_slope[i];
        wall.hits[N_hit].ADCMeanAdditional  = sqrt(*Q_TOP[i + 1] * *Q_BOT[i + 1]);
        wall.hits[N_hit].TDCMeanAdditional  = (*T_TOP[i + 1] + *T_BOT[i + 1])/2;
        wall.hits[N_hit].TDCDeltaAdditional = time_offset[i + 1] + (*T_TOP[i + 1] - *T_BOT[i + 1]) * time_slope[i + 1];
        wall.hits[N_hit].ADCAsymmetry       = (wall.hits[N_hit].ADCMean - wall.hits[N_hit].ADCMeanAdditional) / 
                                              (wall.hits[N_hit].ADCMean + wall.hits[N_hit].ADCMeanAdditional);
        if (wall.hits[N_hit].TDCMean != 0)
        {
          N_hit++;
        }
      }
    
    }
      
      
// #ifdef DEBUG_KAOS_TOF
//       std::cout << "paddle=" << i << std::endl;
//       std::cout << "TOP TDC: " << *T_TOP[i] 
//           << "  BOT TDC: " << *T_BOT[i] << std::endl;
//       std::cout << "TOP ADC: " << *Q_TOP[i] \
//           << "  BOT ADC: " << *Q_BOT[i] << std::endl;
// #endif

      // increase hit counter
//       if (wall.hits[N_hit].tdc_mean != 0) 
//       {
//         N_hit++;
//         if (kaos_debug == 1) std::cout << "+";
//       }
//       else if (kaos_debug == 1) std::cout << "-";
//     }
//     else 
//     {if (kaos_debug == 1) std::cout << "-";}
   

  }

  wall.N_hits = N_hit;
  //if (kaos_debug == 1) std::cout << std::endl;
  return 0;
}*/


/*int KaosMultiTrack::check_trigger_three_walls(int minPaddleH, int maxPaddleH, int minPaddleG, int maxPaddleG, int minPaddleI, int maxPaddleI)
{
  double posxzMinH = minPaddleH * lH;
  double posxzMinG = minPaddleG * lG;
  double posxzMinI = minPaddleI * lG; // not an error, same size paddles
  double posxzMaxH = (maxPaddleH + 1) * lH;
  double posxzMaxG = (maxPaddleG + 1) * lG;
  double posxzMaxI = (maxPaddleI + 1) * lG; // not an error, same size paddles

  double posxMinH = xH + cos(alphaH / degree) * posxzMinH;
  double poszMinH = zH + sin(alphaH / degree) * posxzMinH;
  double posxMaxH = xH + cos(alphaH / degree) * posxzMaxH;
  double poszMaxH = zH + sin(alphaH / degree) * posxzMaxH;
  
  double posxMinG = xG + cos(alphaG / degree) * posxzMinG;
  double poszMinG = zG + sin(alphaG / degree) * posxzMinG;
  double posxMaxG = xG + cos(alphaG / degree) * posxzMaxG;
  double poszMaxG = zG + sin(alphaG / degree) * posxzMaxG;
  
  double thetaMin = atan((posxMinH - posxMinG) / (poszMinH - poszMinG)) * degree;
  double thetaMax = atan((posxMaxH - posxMaxG) / (poszMaxH - poszMaxG)) * degree;
  
//   double offsetMin = posxMinG - poszMinG * (posxMinH - posxMinG) / (poszMinH -poszMinG);
//   double offsetMax = posxMaxG - poszMaxG * (posxMaxH - posxMaxG) / (poszMaxH -poszMaxG);
  
  double cosAlphaI = cos(alphaI / degree); // to increase calculation (a little bit (maybe))
  double sinAlphaI = sin(alphaI / degree);
  double cosThetaMin = cos(thetaMin / degree);
  double sinThetaMin = sin(thetaMin / degree);
  double cosThetaMax = cos(thetaMax / degree);
  double sinThetaMax = sin(thetaMax / degree);
  
  double allowedPosxzMinI = ((posxMinG - xI) / cosAlphaI + sinThetaMin / (cosThetaMin * cosAlphaI)) / (1 - sinAlphaI * sinThetaMin / (cosThetaMin * cosAlphaI));
  double allowedPosxzMaxI = ((posxMaxG - xI) / cosAlphaI + sinThetaMax / (cosThetaMax * cosAlphaI)) / (1 - sinAlphaI * sinThetaMax / (cosThetaMax * cosAlphaI));
    
  if ((posxzMinI < allowedPosxzMaxI) && (posxzMaxI > allowedPosxzMinI)) return 1;
  return 0;
}
*/

int 
KaosMultiTrack::handle()
{
  /*#ifdef DEBUG_KAOS_TOF
  std::cout << "###################### NEXT TRACK ######################" 
	    << std::endl;
  #endif */

  // init values
/*  double Hy= -1, Hz, Hzprime, Hx= 0, Hxprime;
  double Gy= -1, Gz, Gzprime, Gx= 0, Gxprime;
  double paddleH_exp= -100, paddleG_exp= -100;*/

  double Hx = 0, Hy = -1, Gx = 0, Gy = -1;

  
  /*if (use_mwpc) 
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
  else */
    /*{
      Hx = 0; Gx = 0;
    }*/
  if (trackVector -> size() == 0)
  {
    // do calculations for new event
    
    // calibrate ADC for pedestals, HighRange, absorption and gain
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
                      Aerogel_gain_TOP, Aerogel_gain_BOT); 
  
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
    //fill_hit_array(tHt, tHb, qHt, qHb, time_offsetH, time_slopeH, wallH, 30);
    #ifdef DEBUG_KAOS_TOF
    std::cout << "wallG:" << std::endl;
    #endif
/*    if (use_tagger)
     fill_hit_array(tGt, tGb, qGt, qGb, time_offsetG, time_slopeG, wallG, 28);
    else fill_hit_array(tGt, tGb, qGt, qGb, time_offsetG, time_slopeG, wallG, 30);*/
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
    
    // find tracks
    gen_track_list(wallH, wallG, wallI);
  
    
    
    delete trackVector;
    trackVector = new std::vector<t_track>;
    numberTracks = 0;

    if (use_tof == 3)
    {
      int N_hitsI = 0;
      int N_hitsG = wallG.N_hits;
      for (int hitGIndex = 0; hitGIndex < wallG.N_hits; hitGIndex++)
      {
        if (wallG.hits[hitGIndex].paddleNo > 14)
        {
          if (N_hitsI == 0) N_hitsG = hitGIndex;
          wallI.hits[N_hitsI] = wallG.hits[hitGIndex];
          wallI.hits[N_hitsI].paddleNo -= 15;
          N_hitsI++;
        } //TODO catch 2 paddle hits G14, I0 here
      }
      wallI.N_hits = N_hitsI;
      wallG.N_hits = N_hitsG;
      //    std::cout << wallI.N_hit << " " << wallG.N_hits << endl;
      for (int hitHIndex = 0; hitHIndex < wallH.N_hits; hitHIndex++)
      {
        for (int hitGIndex = 0; hitGIndex < wallG.N_hits; hitGIndex++)
        {
          for (int hitIIndex = 0; hitIIndex < wallI.N_hits; hitIIndex++)
          {
            if (check_trigger(wallG.hits[hitGIndex].paddleNo,
                              wallI.hits[hitIIndex].paddleNo,
                              wallH.hits[hitHIndex].paddleNo)
              /*check_trigger_three_walls(wallH.hits[hitHIndex].paddleNo, wallH.hits[hitHIndex].paddleNo + (wallH.hits[hitHIndex].twoPaddleHit > 0),
                                          wallG.hits[hitGIndex].paddleNo, wallG.hits[hitGIndex].paddleNo + (wallG.hits[hitGIndex].twoPaddleHit > 0),
                                          wallI.hits[hitIIndex].paddleNo, wallI.hits[hitIIndex].paddleNo + (wallI.hits[hitIIndex].twoPaddleHit > 0)) == 1*/)
            {	  
              t_track trackBuffer;
              trackBuffer.paddleHitH = wallH.hits[hitHIndex];
              trackBuffer.paddleHitG = wallG.hits[hitGIndex];
              trackBuffer.paddleHitI = wallI.hits[hitIIndex];
              trackVector -> push_back(trackBuffer);
              numberTracks++;
            }
          }
        }
      }
    }
    else
    {
      for (int hitHIndex = 0; hitHIndex < wallH.N_hits; hitHIndex++)
      {
        for (int hitGIndex = 0; hitGIndex < wallG.N_hits; hitGIndex++)
        {
          // check trigger acceptance
          int triggerAcceptance = 1;
          triggerAcceptance = triggerAcceptance * KaosDetectors::check_trigger(wallH.hits[hitHIndex].paddleNo, wallG.hits[hitGIndex].paddleNo);
          // for two paddle hits check if neighbouring paddle is also in the trigger acceptance
          if (wallH.hits[hitHIndex].twoPaddleHit)
          {
            triggerAcceptance = triggerAcceptance * KaosDetectors::check_trigger(wallH.hits[hitHIndex].paddleNo + 1, wallG.hits[hitGIndex].paddleNo);
          }
          if (wallG.hits[hitGIndex].twoPaddleHit)
          {
            triggerAcceptance = triggerAcceptance * KaosDetectors::check_trigger(wallH.hits[hitHIndex].paddleNo, wallG.hits[hitGIndex].paddleNo + 1);
          }
          if ((wallH.hits[hitHIndex].twoPaddleHit) && (wallG.hits[hitGIndex].twoPaddleHit))
          {
            triggerAcceptance = triggerAcceptance * KaosDetectors::check_trigger(wallH.hits[hitHIndex].paddleNo + 1, wallG.hits[hitGIndex].paddleNo + 1);
          }

          // do additional exclutions for unwanted tracks here

          if (triggerAcceptance != 0)
          {
            t_track trackBuffer;
            trackBuffer.paddleHitH = wallH.hits[hitHIndex];
            trackBuffer.paddleHitG = wallG.hits[hitGIndex];
            trackVector -> push_back(trackBuffer);
            numberTracks++;
          }
        }
      }
    }
  }
  if (trackVector -> size() > 0)
  {
    t_track trackBuffer = trackVector -> back();

    // calculate qulaities
    /*double this_q_tagger   = 1;
    double this_q_aerogel  = calc_quality(ACThreshold, it->ACHitADC, ACThreshold);
    double this_q_theta    = calc_quality(sigma_theta, delta, plateau_theta);
    double this_q_phi      = calc_quality(sigma_phi, delta, plateau_phi);
    double this_q_dEdxG    = calc_quality(sigma_dEdxG, dEdxG - (reference_dEdxG -  * dEdxG_slope), plateau_dEdxG);
    double this_q_dEdxH    = calc_quality(sigma_dEdxH, dEdxH - (reference_dEdxH -  * dEdxG_slope), plateau_dEdxH);
    double this_q_TOF      = calc_quality(sigma_TOF, delta, plateau_TOF);
    double this_q_xHx      = 1;
    double this_q_xGx      = 1;
    double this_quality    = this_q_theta * this_q_phi * this_q_dEdxG * this_q_dEdxH * this_q_TOF;
    if (use_tagger)  this_quality *= this_q_tagger;
    if (use_aerogel) this_quality *= this_q_aerogel;
    
    q_tagger   -> handle(this_q_tagger);
    q_aerogel  -> handle(this_q_aerogel);
    q_theta    -> handle(this_q_theta);
    q_phi      -> handle(this_q_phi);
    q_dEdx     -> handle(this_q_dEdxG * this_q_dEdxH);
    q_TOF      -> handle(this_q_TOF);
    q_xHx      -> handle(this_q_xHx);
    q_xGx      -> handle(this_q_xGx);
    q_total    -> handle(this_quality);*/

    /* TODO make nice */
    double lI = lG;
    double alphaI = alphaG;
    double IscaleADC2MeV = GscaleADC2MeV;

    
    const double thicknessH = 20;
    const double thicknessG = 20;
    const double thicknessI = 20;
    
    // hit positions
    double xH = (trackBuffer.paddleHitH.paddleNo + 0.5) * lH;
    if (trackBuffer.paddleHitH.twoPaddleHit == 1) {xH += 0.5 * lH;}
    double yH = trackBuffer.paddleHitH.TDCDelta;
    if (trackBuffer.paddleHitH.twoPaddleHit == 1) {
	yH = (trackBuffer.paddleHitH.TDCDelta + trackBuffer.paddleHitH.TDCDeltaAdditional) / 2;}

    double xG = (trackBuffer.paddleHitG.paddleNo + 0.5) * lG;
    if (trackBuffer.paddleHitG.twoPaddleHit == 1) {xG += 0.5 * lG;}
    double yG = trackBuffer.paddleHitG.TDCDelta;
    if (trackBuffer.paddleHitG.twoPaddleHit == 1) {
	yH = (trackBuffer.paddleHitG.TDCDelta + trackBuffer.paddleHitG.TDCDeltaAdditional) / 2; }

    double xI = (trackBuffer.paddleHitI.paddleNo + 0.5) * lI;
    if (trackBuffer.paddleHitG.twoPaddleHit == 1) {xI += 0.5 * lI;}
    double yI = trackBuffer.paddleHitI.TDCDelta;
    if (trackBuffer.paddleHitI.twoPaddleHit == 1) {
	yI = (trackBuffer.paddleHitI.TDCDelta + trackBuffer.paddleHitI.TDCDeltaAdditional) / 2; }

    // angles + flightlength
    double theta = KaosDetectors::get_theta(xH, xG);
    double phi = KaosDetectors::get_phi(yH, yG, xH, xG);
    double tracklength = KaosDetectors::get_tracklength_GH(xH, xG, phi);
    double tracklengthIH = KaosDetectors::get_tracklength_IH(xH, xI, phi);
    
    // correction for hit position of two paddle hits
    if (trackBuffer.paddleHitH.twoPaddleHit == 1) {xH -= (thicknessH / 2) *  trackBuffer.paddleHitH.ADCAsymmetry * tan(theta / degree);}
    if (trackBuffer.paddleHitG.twoPaddleHit == 1) {xG -= (thicknessG / 2) *  trackBuffer.paddleHitG.ADCAsymmetry * tan(theta / degree);}
    if (trackBuffer.paddleHitI.twoPaddleHit == 1) {xI -= (thicknessI / 2) *  trackBuffer.paddleHitI.ADCAsymmetry * tan(theta / degree);}
    
    trackPositionHx -> handle(xH);
    trackPositionHy -> handle(yH);
    trackPositionGx -> handle(xG);
    trackPositionGy -> handle(yG);
    trackPositionIx -> handle(xI);
    trackPositionIy -> handle(yI);

//     trackDeltaHy -> handle(trackBuffer.paddleHitH.TDCDelta - trackBuffer.paddleHitH.TDCDeltaAdditional);
//     trackDeltaGy -> handle(trackBuffer.paddleHitG.TDCDelta - trackBuffer.paddleHitG.TDCDeltaAdditional);
    
    trackTheta -> handle(theta);
    trackPhi -> handle(phi);
    trackLength -> handle(tracklength);

    // energyloss
    double scintpathH = sqrt( 1 + pow(tan((theta + alphaH)/degree),2) 
        + pow(tan(phi/degree),2) ) * 2;
	  
    double scintpathG = sqrt( 1 + pow(tan((theta + alphaG)/degree),2) 
        + pow(tan(phi/degree),2) ) * 2;

    double scintpathI = sqrt( 1 + pow(tan((theta + alphaI)/degree),2) 
        + pow(tan(phi/degree),2) ) * 2;

    double dEdxH = trackBuffer.paddleHitH.ADCMean / scintpathH * HscaleADC2MeV;
    if (trackBuffer.paddleHitH.twoPaddleHit == 1) {
      dEdxH += trackBuffer.paddleHitH.ADCMeanAdditional / scintpathH * HscaleADC2MeV;}
    double dEdxG = trackBuffer.paddleHitG.ADCMean / scintpathG * GscaleADC2MeV;
    if (trackBuffer.paddleHitG.twoPaddleHit == 1) {
      dEdxG += trackBuffer.paddleHitG.ADCMeanAdditional / scintpathG * GscaleADC2MeV;}
    double dEdxI = trackBuffer.paddleHitI.ADCMean / scintpathI * IscaleADC2MeV;
    if (trackBuffer.paddleHitI.twoPaddleHit == 1) {
      dEdxI += trackBuffer.paddleHitI.ADCMeanAdditional / scintpathI * IscaleADC2MeV;}

    trackDedxH -> handle(dEdxH);
    trackDedxG -> handle(dEdxG);
    trackDedxI -> handle(dEdxI);

    double dEH = dEdxH * scintpathH;
    double dEG = dEdxG * scintpathG;
    double dEI = dEdxI * scintpathI;
    
    trackDeH -> handle(dEH);
    trackDeG -> handle(dEG);
    trackDeI -> handle(dEI);
    
    // timing
    double timeH = trackBuffer.paddleHitH.TDCMean * KAOS_HADRON_TDC_RESOLUTION;
    if (trackBuffer.paddleHitH.twoPaddleHit == 1) {
      timeH += trackBuffer.paddleHitH.TDCMeanAdditional * KAOS_HADRON_TDC_RESOLUTION;
      timeH = timeH / 2;}
    double timeG = trackBuffer.paddleHitG.TDCMean * KAOS_HADRON_TDC_RESOLUTION;
    if (trackBuffer.paddleHitG.twoPaddleHit == 1) {
      timeG += trackBuffer.paddleHitG.TDCMeanAdditional * KAOS_HADRON_TDC_RESOLUTION;
      timeG = timeG / 2;}
    double timeI = trackBuffer.paddleHitI.TDCMean * KAOS_HADRON_TDC_RESOLUTION;
    if (trackBuffer.paddleHitI.twoPaddleHit == 1) {
      timeI += trackBuffer.paddleHitI.TDCMeanAdditional * KAOS_HADRON_TDC_RESOLUTION;
      timeI = timeI / 2;}

//     trackTimeH -> handle(timeH);
//     trackTimeG -> handle(timeG);

   /* trackDeltaTimeH -> handle((trackBuffer.paddleHitH.TDCMean - 
			       trackBuffer.paddleHitH.TDCMeanAdditional) * 
			      KAOS_HADRON_TDC_RESOLUTION);
    trackDeltaTimeG -> handle((trackBuffer.paddleHitG.TDCMean - 
			       trackBuffer.paddleHitG.TDCMeanAdditional) * 
			      KAOS_HADRON_TDC_RESOLUTION);*/

    if (timeG > 0 && timeH > 0) 
      trackToF -> handle((timeG - timeH)/tracklength*1000); // was *100 P.A.
    else
      trackToF -> handle(0);

    if (use_tof == 3)
    {
      if (timeI > 0 && timeH > 0) 
        trackToF_IH -> handle((timeI - timeH)/tracklengthIH*1000); // was *100 P.A.
      else
        trackToF_IH -> handle(0);
    }
    /* MWPC track
    if (use_track == 1)
      ACx= (hitLx->scalar() - xLAC + tan(theta->scalar()/degree)*zLAC ) * 
      cos(theta->scalar()/degree) / cos( (theta->scalar() + alphaG)/degree);*/
    //    else if (use_track == 2)

    trackID -> handle(trackVector -> size());
    trackPaddleH -> handle(trackBuffer.paddleHitH.paddleNo);
    trackPaddleG -> handle(trackBuffer.paddleHitG.paddleNo);
    trackPaddleI -> handle(trackBuffer.paddleHitI.paddleNo);
//     track2PaddleHitH -> handle(trackBuffer.paddleHitH.twoPaddleHit & 0x01);
//     track2PaddleHitG -> handle(trackBuffer.paddleHitG.twoPaddleHit & 0x01);
    eventNumberTracks -> handle(numberTracks);
    
    // handle informatiopon for tr5acks next to two paddle hits
    double deltaT2P = 0.6;
    double deltaY2P = 70;
    bool discardTrack = false;
    if ((trackBuffer.paddleHitH.twoPaddleHit & 0x10) == 1) // paddle with higher number of tph in H
    {

      if ((trackBuffer.paddleHitH.deltaTlower < deltaT2P) && (trackBuffer.paddleHitH.deltaYlower < deltaY2P)) {discardTrack = true;}
     /* trackDeltaTLowerH  -> handle(trackBuffer.paddleHitH.deltaTlower);
      trackDeltaYLowerH  -> handle(trackBuffer.paddleHitH.deltaYlower);*/
    }

    if ((trackBuffer.paddleHitH.twoPaddleHit & 0x20) == 1) // paddle with lower number of tph in H
    {

      if ((trackBuffer.paddleHitH.deltaThigher < deltaT2P) && (trackBuffer.paddleHitH.deltaYhigher < deltaY2P)) {discardTrack = true;}
//       trackDeltaTHigherH -> handle(trackBuffer.paddleHitH.deltaThigher);
//       trackDeltaYHigherH -> handle(trackBuffer.paddleHitH.deltaYhigher);
    }

//     trackNextToTwoPaddleHitH -> handle(next22PaddleHitH);


    if ((trackBuffer.paddleHitG.twoPaddleHit & 0x10) == 1)  // paddle with higher number of tph in G
    {

      if ((trackBuffer.paddleHitG.deltaTlower < deltaT2P) && (trackBuffer.paddleHitG.deltaYlower  < deltaY2P)) {discardTrack = true;}
//       trackDeltaTLowerG  -> handle(trackBuffer.paddleHitG.deltaTlower);
//       trackDeltaYLowerG  -> handle(trackBuffer.paddleHitG.deltaYlower);
    }

    if ((trackBuffer.paddleHitG.twoPaddleHit & 0x20) == 1) // paddle with lower number of tph in G
    {

      if ((trackBuffer.paddleHitG.deltaThigher < deltaT2P) && (trackBuffer.paddleHitG.deltaYhigher < deltaY2P)) {discardTrack = true;}
//       trackDeltaTHigherG -> handle(trackBuffer.paddleHitG.deltaThigher);
//       trackDeltaYHigherG -> handle(trackBuffer.paddleHitG.deltaYhigher);
    }

    if ((trackBuffer.paddleHitG.twoPaddleHit & 0x01) == 1) // tph in G
    {
      if ((trackBuffer.paddleHitG.deltaThigher > deltaT2P) ||(trackBuffer.paddleHitG.deltaYhigher > deltaY2P)) {discardTrack = true;}
    }
    //trackNextToTwoPaddleHitG -> handle(next22PaddleHitG);
     
   /* if (discardTrack)
    {
      trackTimeH -> handle(timeH + 100);
      trackTimeG -> handle(timeG - 100);      
    }
    else
    {
*/      trackTimeH -> handle(timeH);
      trackTimeG -> handle(timeG);      
      trackTimeI -> handle(timeI);      
  /*  }
*/
    trackVector -> pop_back();
  }

  return trackVector -> size();
}
