//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 2004
//
// Institut f?r Kernphysik, Universit?t Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
//

#ifndef KAOSDETECTORS_H
#define KAOSDETECTORS_H

#include "detectorBase.h"
#include <list>

#define degree (180./M_PI)

class KaosDetectors : public detectorBase {

protected:
  AquaNodeUShort *tdcft[30];
  AquaNodeUShort *tdcfb[30];
  AquaNodeUShort *adcft[30];
  AquaNodeUShort *adcfb[30];

  AquaNodeUShort *tdcgt[30];
  AquaNodeUShort *tdcgb[30];
  AquaNodeUShort *adcgt[30];
  AquaNodeUShort *adcgb[30];

  AquaNodeUShort *adcat[12];
  AquaNodeUShort *adcab[12];

  AquaNodeUShort *tdcx[8];
  AquaNodeUShort *adcx[8];

  AquaNodeDouble *time_xtra[8];
  AquaNodeDouble *charge_xtra[8];

  AquaNodeDouble *paddleH, *paddleG, *paddleI;
  AquaNodeDouble *xpositionH, *xpositionG, *xpositionI;
  AquaNodeDouble *ypositionH, *ypositionG, *ypositionI;
  AquaNodeDouble *timeH, *timeG, *timeI;
  AquaNodeDouble *chargeH, *chargeG, *chargeI;
  AquaNodeDouble *dEdxH, *dEdxG, *dEdxI;
  AquaNodeDouble *deltaTimeClusterH, *deltaTimeClusterG, *deltaTimeClusterI;

  AquaNodeDouble *theta_tof;
  AquaNodeDouble *phi_tof;
  AquaNodeShort  *tracks_tof;
  AquaNodeShort  *trackID_tof;
  AquaNodeDouble *AC1segment_tof;
  AquaNodeDouble *AC1xposition_tof;
  AquaNodeDouble *AC1yposition_tof;
  AquaNodeDouble *AC1HitADC_tof;
  AquaNodeDouble *AC2segment_tof;
  AquaNodeDouble *AC2xposition_tof;
  AquaNodeDouble *AC2yposition_tof;
  AquaNodeDouble *AC2HitADC_tof;
  AquaNodeShort  *tagger_tof;
  AquaNodeDouble *tracklengthGH_tof;
  AquaNodeDouble *tracklengthIH_tof;
  AquaNodeDouble *flighttimeGH_tof;
  AquaNodeDouble *flighttimeIH_tof;

  AquaNodeDouble *q_trigger;
  AquaNodeDouble *q_tagger;
  AquaNodeDouble *q_aerogel;
  AquaNodeDouble *q_phi;
  AquaNodeDouble *q_theta;
  AquaNodeDouble *q_dEdx;
  AquaNodeDouble *q_TOF;
  AquaNodeDouble *q_xHx;
  AquaNodeDouble *q_xGx;
  AquaNodeDouble *q_GIx;
  AquaNodeDouble *q_total;

  AquaNodeDouble *qHt[30];
  AquaNodeDouble *qHb[30];
  AquaNodeDouble *qGt[30];
  AquaNodeDouble *qGb[30];
  AquaNodeDouble *qIt[30];
  AquaNodeDouble *qIb[30];

  AquaNodeDouble *tHt[30];
  AquaNodeDouble *tHb[30];
  AquaNodeDouble *tGt[30];
  AquaNodeDouble *tGb[30];
  AquaNodeDouble *tIt[30];
  AquaNodeDouble *tIb[30];

  AquaNodeDouble *qACt[12];
  AquaNodeDouble *qACb[12];

  AquaNodeInt *clusterSizeH, *clusterSizeG, *clusterSizeI;
  AquaNodeInt *clusterH, *clusterG, *clusterI;

  //  AquaNodeDouble *lengthH;
  //  AquaNodeDouble *lengthG;
  //  AquaNodeDouble *lengthI;

  double ADC_gain_HTOP[30];
  double ADC_gain_HBOT[30];
  double ADC_gain_GTOP[30];
  double ADC_gain_GBOT[30];

  double ADC_ped_HTOP[30];
  double ADC_ped_HBOT[30];
  double ADC_ped_GTOP[30];
  double ADC_ped_GBOT[30];

  double Aerogel_gain_TOP[12];
  double Aerogel_gain_BOT[12];

  double Aerogel_ped_TOP[12];
  double Aerogel_ped_BOT[12];

  double Aerogel_cor_TOP[12];
  double Aerogel_cor_BOT[12];

  double Aerogel_off_TOP[12];
  double Aerogel_off_BOT[12];

  double ADC_ped_x[30];

  double ADC_cor_HTOP[30];
  double ADC_cor_HBOT[30];
  double ADC_cor_GTOP[30];
  double ADC_cor_GBOT[30];

  double TDC_off_HTOP[30];
  double TDC_off_HBOT[30];
  double TDC_off_GTOP[30];
  double TDC_off_GBOT[30];

  double TDC_cor_HTOP[30];
  double TDC_cor_HBOT[30];
  double TDC_cor_GTOP[30];
  double TDC_cor_GBOT[30];

  double TDC_cor_x[8];
  double TDC_off_x[8];

  double ADC_cor_x[8];

  double time_offsetH[30];
  double time_slopeH[30];
  double time_offsetG[30];
  double time_slopeG[30];

  double absorption_HTOP[30];
  double absorption_HBOT[30];
  double absorption_GTOP[30];
  double absorption_GBOT[30];
  
  double timewalk_cor_HTOP[30][4][10];
  double timewalk_cor_HBOT[30][4][10];
  double timewalk_cor_HSUB[30][4][10];
  double timewalk_cor_GTOP[30][4][10];
  double timewalk_cor_GBOT[30][4][10];
  double timewalk_cor_GSUB[30][4][10];

  AquaNodeDouble *theta;
  AquaNodeDouble *phi;
  AquaNodeDouble *hitLx;
  AquaNodeDouble *hitLy;
  AquaNodeDouble *quality;
  AquaNodeDouble *dp;

  double xHG, zHG;
  double zM, zLM;
  double xH, zH, xLH, zLH;
  double xG, zG, xLG, zLG;
  double yH, yG;
  double alphaH, alphaG;
  double lH, lG;
  double xI, zI, xLI, zLI, yI, alphaI, lI;
  double xAC1, zAC1, xLAC1, zLAC1;
  double xAC2, zAC2, xLAC2, zLAC2;
  double AC1min[6], AC1max[6];
  double AC2min[6], AC2max[6];
  double yTAG, xLTAG, zLTAG, yTAGTOP, yTAGBOT;

  double sigma_theta, plateau_theta, TOF_slope;
  double dEdxG_slope, dEdxH_slope, deltat_slope; 
  double offset_phi, slope_phi, sigma_phi, plateau_phi;
  double sigma_dEdxG, plateau_dEdxG, sigma_TOF, plateau_TOF;
  double sigma_dEdxH, plateau_dEdxH;
  double deltaTOF, cluster_time, cluster_max;
  double HscaleADC2MeV, GscaleADC2MeV;
  double ACThreshold, TagThreshold;
  double reference_dEdxG, reference_dEdxH, reference_tof;

  double use_trigger, use_tof, use_tagger, use_mwpc, use_aerogel, use_track, use_timewalk, use_multitrack;

  int tracksInEvent;
  
  int triggerGH[30][30]; int triggerGIH[15][15][30];

 // structure for filtered hits from raw data for one group
  struct t_hit 
  {
    unsigned short paddle;             // PADDLE No.
    double         adc_gmean;          // ADC geometric mean
    double         tdc_mean;           // TDC mean
    double         tdc_delta;          // TDC delta
    int            cluster_id;         // cluster ID
  };

  // structure for one cluster of hits
  struct t_cluster
  {
    unsigned short cluster_id;  // cluster ID
    unsigned short hit_cnt;     // count hits in the cluster
    unsigned short max_adc;     // ADC chn with highest ADC value
    double max_adc_value;       // ADC highest value
    double mean_time;           // TDC mean time of ADC chn with highest value
    double delta_time;          // TDC delta time of ADC chn with highest value
    double delta_time_cluster;  // TDC delta time of paddles in cluster
    double charge;              // summed ADC values of cluster
    double center_of_charge;    // center-of-charge
  };

  // structure for one group of sub-detectors
  struct t_group
  {
    t_hit      hits[30];
    t_cluster  cluster[30];
    double     ind_thr;         // individual threshold
    int        N_hits;          // number of entries in array t_hit
    int        cnt_cluster;     // count clusters
  };

  t_group wallH;
  t_group wallG;
  t_group wallI;

  struct t_track
  {
    int    id[3];           // cluster id: 0=H, 1=G, 2=I
    double theta;           // angle theta
    double phi;             // angle phi
    double xH;              // x position in wall H
    double xG;              // x position in wall G
    double xI;              // x position in wall I
    double yH;              // y position in wall H
    double yG;              // y position in wall G
    double yI;              // y position in wall I
    double dEdxH;           // energy loss wall H
    double dEdxG;           // energy loss wall G
    double dEdxI;           // energy loss wall I
    double length_GH;       // flight length
    double length_IH;       // flight length
    double TOF_GH;          // time-of-flight
    double TOF_IH;          // time-of-flight
    int AC1segment;         // aerogel segment pattern
    double AC1HitADC;       // aerogel ADC value in hit segment
    double AC1x;            // aerogel x position
    double AC1y;            // aerogel y position
    double AC2x;            // aerogel x position
    double AC2y;            // aerogel y position
    int AC2segment;         // aerogel segment pattern
    double AC2HitADC;       // aerogel ADC value in hit segment
    int tagger;             // tagger bar no. (top=1, bot=2)
    double q_tagger;        // quality of tagger data
    double q_aerogel;       // quality of aerogel data
    double q_theta;         // quality of theta
    double q_phi;           // quality of phi
    double q_dEdxG;         // quality of dEdx comparison
    double q_dEdxI;         // quality of dEdx comparison
    double q_dEdxH;         // quality of dEdx comparison
    double q_TOF;           // quality of TOF
    double q_TOFI;          // quality of TOF
    double q_GIx;           // quality of track between G and I
    double q_xHx;           // quality of MWPC track extrapolation to wall H
    double q_xGx;           // quality of MWPC track extrapolation to wall G
                            //
    double quality;         // total quality
  };

  struct t_event
  {
    std::list <struct t_track> track;
  };
  
  struct t_event event;

  void gen_track_list(t_group wallH, t_group wallG, t_group wallI);

  double get_theta(double positionxH, double positionxG);
  double get_phi(double positionyH, double positionyG, 
		 double positionxH, double positionxG);
  double get_tracklength_GH(double positionxH, 
			  double positionxG, double phi);
  double get_tracklength_IH(double positionxH, 
			  double positionxI, double phi);

  void get_q_theta();
  void get_q_phi();
  void get_q_dEdxG();
  void get_q_dEdxI();
  void get_q_dEdxH();
  void get_q_TOF();
  void get_q_TOFI();
  void get_q_GIx();
  void get_q_tagger();
  void get_q_aerogel();
  void get_q_MWPC(double paddleH_exp, double paddleG_exp);

  double calc_quality(double sigma, double delta, double delta_max);
  void calc_total_quality();
  static bool sortby_quality(struct t_track first, struct t_track second);

public:
  int kaos_debug;

  KaosDetectors(AquaTree *aquatree, AquaTree *outtree);
  virtual ~KaosDetectors();
  virtual int handle();
  virtual int init(double *det_pos, double *track_tof, double * threshold_,
		   double *ADC_gain_, double *ADC_pedestal_,
		   double *ADC_correction_, 
		   double *TDC_offset_, double *TDC_correction_,
		   double *TDC_time_corr_,  const char *timewalk_corr_filename,
                   double *ADC_absorption_, double *ADC_scale,
		   double *Aerogel_gain_, double *Aerogel_pedestal_,
		   double *Aerogel_corr_, double *Aerogel_offset_,
		   double use_trigger_, double use_tof_, double use_timewalk_,
		   double use_tagger_, double use_mwpc_, double use_track_, 
		   double use_aerogel_, const char *filename,
                   double use_multitrack_);
  
  void calibrate_ADC(AquaNodeUShort **ADC_TOP,
		     AquaNodeUShort **ADC_BOT, 
		     AquaNodeDouble **Q_TOP,
		     AquaNodeDouble **Q_BOT, 
		     double *ped_TOP, double *ped_BOT, 
		     double *gain_TOP, double *gain_BOT,
		     double *ADC_cor_TOP, double *ADC_cor_BOT,
		     double *absorption_TOP, double *absorption_BOT, 
		     double x, double y, double l);
 
  void calibrate_Aerogel(AquaNodeUShort **ADC_TOP,
			 AquaNodeUShort **ADC_BOT, 
			 AquaNodeDouble **Q_TOP,
			 AquaNodeDouble **Q_BOT, 
			 double *ped_TOP, double *ped_BOT, 
			 double *gain_TOP, double *gain_BOT,
			 double *cor_TOP, double *cor_BOT);

  void calibrate_TDC(AquaNodeUShort **TDC_TOP,
		     AquaNodeUShort **TDC_BOT, 
		     AquaNodeDouble **T_TOP,
		     AquaNodeDouble **T_BOT, 
		     double *TDC_off_TOP, double *TDC_off_BOT,
		     double *TDC_cor_TOP, double *TDC_cor_BOT,
		     double x, double y, double l,
		     double *time_offset, double *time_slope,
                     // needed for timewalk correction
                     int wallID,
                     AquaNodeUShort **ADC_TOP,
                     AquaNodeUShort **ADC_BOT,
                     double *ADC_cor_TOP, double *ADC_cor_BOT);

  /*
  void calibrate_AerogelTDC(AquaNodeUShort **TDC_TOP,
		     AquaNodeUShort **TDC_BOT, 
		     AquaNodeDouble **T_TOP,
		     AquaNodeDouble **T_BOT, 
		     double *TDC_off_TOP, double *TDC_off_BOT);
  */

   int fill_hit_array(AquaNodeDouble **T_TOP,
		     AquaNodeDouble **T_BOT, 
		     AquaNodeDouble **Q_TOP,
		     AquaNodeDouble **Q_BOT, 
		     double *time_offset, double *time_slope,
		     t_group &wall, int StartNoOfPaddle,  int StopNoOfPaddle);

  int cnt_cluster(t_group &wall);

  void get_cluster_data(t_group &wall, double threshold);

  int check_trigger(unsigned short paddleH, unsigned short paddleG);
  int check_trigger(unsigned short paddleG, unsigned short paddleI, unsigned short paddleH);
};

#endif // KAOSDETECTORS_H
