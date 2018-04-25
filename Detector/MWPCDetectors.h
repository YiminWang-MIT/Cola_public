//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 2004
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
//
#include "HMBook/hmbook.h"
#include "detectorBase.h"
#include <list>
#include <vector>
#include <string>

#define KAOS_MWPC_ADCX 240     // number of ADC's in x chain
#define KAOS_MWPC_ADCY 70      // number of ADC's in y chain
#define KAOS_WALL_PADDLES 30   // number of paddles in scintillator wall
#define KAOS_WALLH_PADDLES 30  // number of paddles in scintillator wall
#define degree (180./M_PI)

class MWPCDetectors : public detectorBase {

private:
  static int const MWPC_ADCX = KAOS_MWPC_ADCX;
  static int const MWPC_ADCY = KAOS_MWPC_ADCY;
  static int const PADDLE_F  = 30;
  static int const PADDLE_G  = 30;
  
  // ====================================
  // Input data 
  // ====================================
  // MWPC data
  AquaNodeUShort *Lx[KAOS_MWPC_ADCX];
  AquaNodeUShort *Mx[KAOS_MWPC_ADCX];
  AquaNodeUShort *Ly[KAOS_MWPC_ADCY];
  AquaNodeUShort *My[KAOS_MWPC_ADCY];
  
  // TDC data from wall F
  AquaNodeUShort *tdcft[KAOS_WALL_PADDLES];
  AquaNodeUShort *tdcfb[KAOS_WALL_PADDLES];
  AquaNodeUShort *tdcgt[KAOS_WALL_PADDLES];
  AquaNodeUShort *tdcgb[KAOS_WALL_PADDLES];


  // ====================================
  // Output data
  // ====================================
  AquaNodeShort  *tracks;  // number of tracks found with quality >= quality_min
  
  // number of clusters found
  AquaNodeInt *clusterLx;
  AquaNodeInt *clusterLy;
  AquaNodeInt *clusterMx;
  AquaNodeInt *clusterMy;

  // return only one track (besty qualtiy)
  // hit position in lab coordinates
  AquaNodeDouble *hitLx;
  AquaNodeDouble *hitLy;
  AquaNodeDouble *hitMx;
  AquaNodeDouble *hitMy;
  
  // cluster (truncated) width and hit error in ADC channels
  AquaNodeDouble *hitwidthLx;
  AquaNodeDouble *hitwidthLy;
  AquaNodeDouble *hitwidthMx;
  AquaNodeDouble *hitwidthMy;
  AquaNodeDouble *hiterrLx;
  AquaNodeDouble *hiterrLy;
  AquaNodeDouble *hiterrMx;
  AquaNodeDouble *hiterrMy;

  AquaNodeDouble *sndquality; // total quality of 2nd best track
  AquaNodeDouble *quality;    // total quality of track
  AquaNodeDouble *qualcL;     // quality charge ratio L
  AquaNodeDouble *qualcM;     // quality charge ratio M
  AquaNodeDouble *qualtheta;  // quality of theta
  AquaNodeDouble *qualphi;    // quality of phi
  AquaNodeDouble *qualwallF;  // quality wall F
  AquaNodeDouble *qualwallG;  // quality wall F
  AquaNodeDouble *qualtimeF;  // quality time F
  AquaNodeDouble *qualtimeG;  // quality time F
  AquaNodeDouble *theta;      // angle theta
  AquaNodeDouble *phi;        // angle phi

  // truncated cluster charge
  AquaNodeDouble *chargeLx; 
  AquaNodeDouble *chargeLy;
  AquaNodeDouble *chargeMx;
  AquaNodeDouble *chargeMy;

  // number of ADC in (not truncated) cluster (== ADC multiplicity)
  AquaNodeInt *multiLx;
  AquaNodeInt *multiLy;
  AquaNodeInt *multiMx;
  AquaNodeInt *multiMy;

  // highest ADC value
  AquaNodeDouble *hitmaxvalueLx;
  AquaNodeDouble *hitmaxvalueLy;
  AquaNodeDouble *hitmaxvalueMx;
  AquaNodeDouble *hitmaxvalueMy;

  // online histograms
  HIST *ADCNrLx, *ADCNrLy, *ADCNrMx, *ADCNrMy;
  HIST *ADCChargeLx, *ADCChargeLy, *ADCChargeMx, *ADCChargeMy;
  HIST *RAWADCNrLx, *RAWADCNrLy, *RAWADCNrMx, *RAWADCNrMy;


  // ====================================
  // variables for values from run.db
  // ====================================
  // Kaos Detector Positions (det_pos[])
  double xLM;  // MWPC M
  double zLM;  // MWPC M
  double xLF;  // WALL F
  double zLF;  // WALL F
  double xLG;  // WALL G
  double zLG;  // WALL G
  double yL;   // MWPC L
  double yM;   // MWPC M
  double yF;   // WALL F
  double yG;   // WALL G
  double lF;   // PADDLE SIZE WALL F
  double lG;   // PADDLE SIZE WALL G
  double alphaF;//ANGLE WALL F
  double alphaG;//ANGLE WALL G 

  double use_trigger, use_tof, use_tagger, use_track;

  int trigger[30][30];

  double track; // selected track

  double time_offsetF[30];
  double time_slopeF[30];
  double time_offsetG[30];
  double time_slopeG[30];

  double TDC_correction[120];
  double TDC_offset[120];

  // acceptance cuts (acceptance[])
  double theta_min;
  double theta_max;
  double slope_max;
  //double phi_min;  // not used
  //double phi_max;  // not used
  double quality_min;

  double threshold_x, threshold_y;
  double sep_threshold;

  double peak_min;
  double big_charge;

  // quality for projection to scintillator walls 
  double qual_walls;  
  double qual_plateau_time;
  double qual_gaussian_time;

  // quality phi vs HitLy
  double offset_phi, slope_phi;
  double qual_phi[2];

  // quality charge ratio Mx/My
  double fit_c_M[4];
  double qual_c_M[4];

  // quality charge ratio Lx/Ly
  double fit_c_L[4];
  double qual_c_L[4];

  // error center of charge
  double adc_err_base;
  double adc_err_var;

  // ====================================
  // data analysis 
  // ====================================

  struct t_cluster
  {
    int id;             // cluster id
    int cnt;            // number of ADCs in cluster
    int start;          // lowest  ADC number (start of cluster)
    int end;            // highest ADC number (end of cluster)
    int max_adc;        // ADC number with highest value
    double max_value;   // highest value
    //    double q_max_value; // quality of highest value
    double charge;      // charge of cluster
    double cofc;        // center of charge in adc-coordinates
    double cofc_err;    // center of charge error in ADC-coordinates
    double cofc_sig;    // center of charge sigma in ADC-coordinates
    double cofc_lab;    // center of charge in lab-coordinates
  };

  struct t_inter
  {
    int adc;     // first ADC in group to interpolate
    int length;  // number of ADCs in group to interpolate
  };
 
  struct t_chain
  {
    int cnt;
    int sep;     // number of ADCs to have value 0 for separating two clusters
    int del;     // delete all clusters with less or equal than <del> ADCs
    int t_left;  // truncated cluster: number of ADCs taken left side from highest ADC(s)
    int t_right; // truncated cluster: number of ADCs taken right side from highest ADC(s)
    double sat_ratio;
    std::vector<struct t_inter> inter;
    std::vector<double> thresh;         // threshold for each ADC
    std::vector<double> gain;           // gain or disabled for each ADC
    std::vector<double> data_raw;       // raw data
    std::vector<double> data_gain;      // data multiplied with gain
    std::vector<double> data_inter;     // data with interpolated ADCs
    std::vector<double> data_thresh;    // data with removed threshold
    std::vector<double> data;           // working data
    std::vector<struct t_cluster> cluster_full;   // cluster from full data
    std::vector<struct t_cluster> cluster_trunc;  // cluster from truncated data
    std::vector<struct t_cluster> cluster;        // working cluster
  };

  struct t_tof_tdc
  {
    std::vector<double> toff_top;       // data from TOF F top
    std::vector<double> toff_bot;       // data from TOF F bottom
    std::vector<double> tofg_top;       // data from TOF G top
    std::vector<double> tofg_bot;       // data from TOF G bottom
  };

  struct t_track
  {
    int    id[4];           // cluster id: 0=my, 1=mx, 2=ly, 3=lx
    double theta;           // angle theta
    double phi;             // angle phi
    //    double q_max_value[4];  // quality max_value
    double q_c_L;           // quality charge ratio L
    double q_c_M;           // quality charge ratio M
    double q_theta;         // quality of theta (only chambers)
    double q_phi;           // quality of phi (only chambers)
    double q_wall_F;        // quality of theta (with wall F)
    double q_wall_G;        // quality of theta (with wall G)
    double q_time_F;        // quality of y (with wall F)
    double q_time_G;        // quality of y (with wall G)
    double q_FGtrig;        // quality of trigger condition
    double quality;         // total quality
 };

  struct t_event
  {
    struct t_chain chain[4];        // 0: My, 1:Mx, 2:Ly, 3:Lx
    std::vector<double> tof[4];     // 0: TOF F top; 1: TOF F bottom; 
                                    // 2: TOF G top; 3: TOF F bottom; 
    std::list<struct t_track> track;
  };

  struct t_event event;

  void check_input_value(std::string str, double value);
  double calc_quality(double sigma, double delta, double delta_max);
  void reset();
  int  get_raw_data();
  void get_data_gain();
  void get_data_inter();
  void remove_threshold();
  void build_cluster();
  void get_cluster_data();
  void build_cluster_trunc();
  static bool pred_chain_max_value(struct t_cluster c1, struct t_cluster c2);
  //  void gen_cluster_quality();
  void gen_track_list();
  double get_cofc_lab(int chain, double cofc);
  double get_theta(double mx_cofc_lab, double lx_cofc_lab);
  double get_phi(double my_cofc_lab, double ly_cofc_lab);
  void get_q_theta();
  void get_q_phi();
  void get_q_c();
  int get_tof_data();
  void get_q_tof();
  void calc_total_quality();
  static bool pred_sortby_quality(struct t_track first, struct t_track second);

public:
  MWPCDetectors(AquaTree *aquatree, AquaTree *outtree);
  virtual ~MWPCDetectors();
  virtual int handle();
  virtual int init(double *det_pos, double *track_mwpc, double quality_min, 
		   double *gain, double *TDC_correction_, 
		   double *TDC_time_corr_, double *TDC_offset_, 
		   double use_trigger_, double use_tof_, 
		   double use_tagger_, double use_track_, const char *filename);
  virtual void draw(char *trackDir, long eventno);
};
