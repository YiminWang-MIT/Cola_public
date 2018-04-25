//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 2004
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: MWPCDetectors.cc 2512 2013-07-26 14:45:23Z patrick $
//

#include "Aqua/AquaTree.h"
#include "MWPCDetectors.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string.h>

//#define DEBUG_MWPC_CLUSTER
//#define DEBUG_MWPC_CHARGE_QUALITY
//#define DEBUG_TOF_DATA
//#define DEBUG_MWPC_QUALITY

MWPCDetectors::MWPCDetectors(AquaTree *aquatree, AquaTree *outtree)
  : detectorBase(aquatree, outtree)
{
  // input data from MWPC
  AquaNode **snodeLx = (AquaNode **) Lx;
  atree->multifind(snodeLx, 0, 239, "abcn.kaos.mwpc.l.x[%d]");
  AquaNode **snodeMx = (AquaNode **) Mx;
  atree->multifind(snodeMx, 0, 239, "abcn.kaos.mwpc.m.x[%d]");
  AquaNode **snodeLy = (AquaNode **) Ly;
  atree->multifind(snodeLy, 0, 69, "abcn.kaos.mwpc.l.y[%d]");
  AquaNode **snodeMy = (AquaNode **) My;
  atree->multifind(snodeMy, 0, 69, "abcn.kaos.mwpc.m.y[%d]");
  
  // input TDC data from wall F
  AquaNode **Ntdcft = (AquaNode **)tdcft;
  atree->multifind(Ntdcft, 0, 29, "abcn.kaos.tof.fdet[%d].top.time");
  AquaNode **Ntdcfb = (AquaNode **)tdcfb;
  atree->multifind(Ntdcfb, 0, 29, "abcn.kaos.tof.fdet[%d].bottom.time");

  // input TDC data from wall G
  AquaNode **Ntdcgt = (AquaNode **)tdcgt;
  atree->multifind(Ntdcgt, 0, 29, "abcn.kaos.tof.gdet[%d].top.time");
  AquaNode **Ntdcgb = (AquaNode **)tdcgb;
  atree->multifind(Ntdcgb, 0, 29, "abcn.kaos.tof.gdet[%d].bottom.time");
  
  // output data
  multiLx   = (AquaNodeInt *) online->find("online.kaos.mwpc.l.x.multi");
  multiLy   = (AquaNodeInt *) online->find("online.kaos.mwpc.l.y.multi");
  multiMx   = (AquaNodeInt *) online->find("online.kaos.mwpc.m.x.multi");
  multiMy   = (AquaNodeInt *) online->find("online.kaos.mwpc.m.y.multi");

  hitLx     = (AquaNodeDouble *) online->find("online.kaos.mwpc.l.x.hit");
  hitLy     = (AquaNodeDouble *) online->find("online.kaos.mwpc.l.y.hit");
  hitMx     = (AquaNodeDouble *) online->find("online.kaos.mwpc.m.x.hit");
  hitMy     = (AquaNodeDouble *) online->find("online.kaos.mwpc.m.y.hit");

  hiterrLx  = (AquaNodeDouble *) online->find("online.kaos.mwpc.l.x.hiterr");
  hiterrLy  = (AquaNodeDouble *) online->find("online.kaos.mwpc.l.y.hiterr");
  hiterrMx  = (AquaNodeDouble *) online->find("online.kaos.mwpc.m.x.hiterr");
  hiterrMy  = (AquaNodeDouble *) online->find("online.kaos.mwpc.m.y.hiterr");
  
  hitwidthLx = (AquaNodeDouble *) online->find("online.kaos.mwpc.l.x.hitwidth");
  hitwidthLy = (AquaNodeDouble *) online->find("online.kaos.mwpc.l.y.hitwidth");
  hitwidthMx = (AquaNodeDouble *) online->find("online.kaos.mwpc.m.x.hitwidth");
  hitwidthMy = (AquaNodeDouble *) online->find("online.kaos.mwpc.m.y.hitwidth");
 
  hitmaxvalueLx = (AquaNodeDouble *) online->find("online.kaos.mwpc.l.x.hitmaxvalue");
  hitmaxvalueLy = (AquaNodeDouble *) online->find("online.kaos.mwpc.l.y.hitmaxvalue");
  hitmaxvalueMx = (AquaNodeDouble *) online->find("online.kaos.mwpc.m.x.hitmaxvalue");
  hitmaxvalueMy = (AquaNodeDouble *) online->find("online.kaos.mwpc.m.y.hitmaxvalue");

  tracks    = (AquaNodeShort  *) online->find("online.kaos.mwpc.tracks");
  quality   = (AquaNodeDouble *) online->find("online.kaos.mwpc.quality");
  qualcL  = (AquaNodeDouble *) online->find("online.kaos.mwpc.qualcL");
  qualcM  = (AquaNodeDouble *) online->find("online.kaos.mwpc.qualcM");
  qualtheta = (AquaNodeDouble *) online->find("online.kaos.mwpc.qualtheta");
  qualphi   = (AquaNodeDouble *) online->find("online.kaos.mwpc.qualphi");
  qualwallF = (AquaNodeDouble *) online->find("online.kaos.mwpc.qualwallF");
  qualwallG = (AquaNodeDouble *) online->find("online.kaos.mwpc.qualwallG");
  qualtimeF = (AquaNodeDouble *) online->find("online.kaos.mwpc.qualtimeF");
  qualtimeG = (AquaNodeDouble *) online->find("online.kaos.mwpc.qualtimeG");
  sndquality= (AquaNodeDouble *) online->find("online.kaos.mwpc.sndquality");
  theta     = (AquaNodeDouble *) online->find("online.kaos.mwpc.theta");
  phi       = (AquaNodeDouble *) online->find("online.kaos.mwpc.phi");

  chargeLx  = (AquaNodeDouble *) online->find("online.kaos.mwpc.l.x.charge");
  chargeLy  = (AquaNodeDouble *) online->find("online.kaos.mwpc.l.y.charge");
  chargeMx  = (AquaNodeDouble *) online->find("online.kaos.mwpc.m.x.charge");
  chargeMy  = (AquaNodeDouble *) online->find("online.kaos.mwpc.m.y.charge");

  clusterLx = (AquaNodeInt *) online->find("online.kaos.mwpc.l.x.cluster");
  clusterLy = (AquaNodeInt *) online->find("online.kaos.mwpc.l.y.cluster");
  clusterMx = (AquaNodeInt *) online->find("online.kaos.mwpc.m.x.cluster");
  clusterMy = (AquaNodeInt *) online->find("online.kaos.mwpc.m.y.cluster");

  // output online Histograms
  char title[64];
  sprintf(title, "KAOS/MWPC/L/Nr RAW ADC x");
  RAWADCNrLx = HMFind(title);
  sprintf(title, "KAOS/MWPC/M/Nr RAW ADC x");
  RAWADCNrMx = HMFind(title);
  sprintf(title, "KAOS/MWPC/L/Nr RAW ADC y");
  RAWADCNrLy = HMFind(title);
  sprintf(title, "KAOS/MWPC/M/Nr RAW ADC y");
  RAWADCNrMy = HMFind(title);

  sprintf(title, "KAOS/MWPC/L/Nr ADC x");
  ADCNrLx = HMFind(title);
  sprintf(title, "KAOS/MWPC/M/Nr ADC x");
  ADCNrMx = HMFind(title);
  sprintf(title, "KAOS/MWPC/L/Nr ADC y");
  ADCNrLy = HMFind(title);
  sprintf(title, "KAOS/MWPC/M/Nr ADC y");
  ADCNrMy = HMFind(title);

  sprintf(title, "KAOS/MWPC/L/Charge ADC x");
  ADCChargeLx = HMFind(title);
  sprintf(title, "KAOS/MWPC/M/Charge ADC x");
  ADCChargeMx = HMFind(title);
  sprintf(title, "KAOS/MWPC/L/Charge ADC y");
  ADCChargeLy = HMFind(title);
  sprintf(title, "KAOS/MWPC/M/Charge ADC y");
  ADCChargeMy = HMFind(title);
}

// ========================================================

MWPCDetectors::~MWPCDetectors()
{
  ;
}

// ========================================================

void
MWPCDetectors::check_input_value(std::string str, double value)
{
  if (value == 0)
    {
      std::cerr << "ERROR: " << str 
		<< " quality value set to 0 in run.db" << std::endl;
      exit(1);
    }
} 

int 
MWPCDetectors::init(double *det_pos, double *track_mwpc, double quality_min_,
		    double *gain, double * TDC_correction_,
		    double *TDC_time_corr_, double *TDC_offset_, 
		    double use_trigger_, double use_tof_, double use_tagger_,
		    double use_track_, const char *filename)
{

 // Kaos detector usage
  use_trigger  = use_trigger_;
  use_tof      = use_tof_;
  use_tagger   = use_tagger_;
  use_track    = use_track_;

  // Kaos detector positions
  if (use_track < 3) // old coordinate system
    {
      xLM  = det_pos[0];
      zLM  = det_pos[1];
      xLF  = det_pos[2];
      zLF  = det_pos[3];
      xLG  = det_pos[4];
      zLG  = det_pos[5];
      alphaF = det_pos[6];
      alphaG = det_pos[7];
      yL   = det_pos[8];
      yM   = det_pos[9];
      yF   = det_pos[10];
      yG   = det_pos[11];
      lF   = det_pos[13];
      lG   = det_pos[14];
    }
  else // 2o12 coordinate system
    {
      xLM = det_pos[15] - det_pos[0];
      zLM = det_pos[30] - det_pos[1];
      xLF = det_pos[2]  - det_pos[15];
      zLF = det_pos[3]  - det_pos[30];
      xLG = det_pos[4]  - det_pos[15];
      zLG = det_pos[5]  - det_pos[30];

      alphaF = det_pos[6];
      alphaG = det_pos[7];

      yL   = det_pos[8];
      yM   = det_pos[9];
      yF   = det_pos[10];
      yG   = det_pos[11];

      lF   = det_pos[13];
      lG   = det_pos[14];
     }
 
  // track reconstruction
  theta_min   = track_mwpc[0];
  theta_max   = track_mwpc[1];
  slope_max   = track_mwpc[4];

  quality_min = quality_min_;

  threshold_x  = track_mwpc[23];
  threshold_y  = track_mwpc[24];

  peak_min    = track_mwpc[5];   // minimum for finding a peak in a cluster
  qual_walls  = track_mwpc[6];   // quality hit position scintillator walls

  qual_gaussian_time  = track_mwpc[25];   // quality timing vs y
  qual_plateau_time   = track_mwpc[26];   // quality timing vs y

  // error center of charge
  adc_err_base = 1.0;  // constant error for ADC values
  adc_err_var = 2.0;   // error in percent of read ADC values

  // quality phi (phi vs HitLy)
  slope_phi   = track_mwpc[7];   // slope  Ly vs phi
  offset_phi  = track_mwpc[8];   // offset Ly vs phi
  qual_phi[0] = track_mwpc[9];   // quality Ly vs phi
  qual_phi[1] = track_mwpc[10];  // quality Ly vs phi plateau width

  // quality charge ratio My/Mx
  /*
  fit_c_M[0] = -9.4e-01; // fit p0
  fit_c_M[1] = 5.1e-01;  // fit p1
  fit_c_M[2] = 4.1e-04;  // fit p2
  fit_c_M[3] = -9.6e-08; // fit p3
  */
  fit_c_M[0] = 0.0;
  fit_c_M[1] = 0.9;
  fit_c_M[2] = 0.0;
  fit_c_M[3] = 0.0;

  qual_c_M[0] = track_mwpc[11];
  qual_c_M[1] = track_mwpc[12];
  qual_c_M[2] = track_mwpc[13];
  qual_c_M[3] = track_mwpc[14];
  
  // quality charge ratio Ly/Lx
  /*
  fit_c_L[0] = 1.9e+00;
  fit_c_L[1] = 6.2e-01;
  fit_c_L[2] = 2.7e-04;
  fit_c_L[3] = -3.2e-08;
  */
  fit_c_L[0] = -20.;
  fit_c_L[1] = 0.84;
  fit_c_L[2] = 0.0;
  fit_c_L[3] = 0.0;

  qual_c_L[0] = track_mwpc[15];
  qual_c_L[1] = track_mwpc[16];
  qual_c_L[2] = track_mwpc[17];
  qual_c_L[3] = track_mwpc[18];

  big_charge  = track_mwpc[29];
  sep_threshold = track_mwpc[30];

  // chain My
  event.chain[0].cnt = MWPC_ADCY;
  // number of ADCs to have value 0 for separating two clusters
  event.chain[0].sep = (int) track_mwpc[19];
  // delete all clusters with less or equal number of ADCs
  event.chain[0].del = (int) track_mwpc[20] - 1;
  event.chain[0].t_right = (int) track_mwpc[21];
  event.chain[0].t_left  = (int) track_mwpc[22];
  event.chain[0].sat_ratio = track_mwpc[28];

  // chain Mx
  event.chain[1].cnt = MWPC_ADCX;
  // number of ADCs to have value 0 for separating two clusters
  event.chain[1].sep = (int) track_mwpc[19];
  // delete all clusters with less or equal number of ADCs
  event.chain[1].del = (int) track_mwpc[20] - 1;
  event.chain[1].t_right = (int) track_mwpc[21];
  event.chain[1].t_left  = (int) track_mwpc[22];
  event.chain[1].sat_ratio = track_mwpc[27];

  // chain Ly
  event.chain[2].cnt = MWPC_ADCY;
  // number of ADCs to have value 0 for separating two clusters
  event.chain[2].sep = (int) track_mwpc[19];
  // delete all clusters with less or equal number of ADCs
  event.chain[2].del = (int) track_mwpc[20] - 1;
  event.chain[2].t_right = (int) track_mwpc[21];
  event.chain[2].t_left  = (int) track_mwpc[22];
  event.chain[2].sat_ratio = track_mwpc[28];

  // chain Lx
  event.chain[3].cnt = MWPC_ADCX;
  // number of ADCs to have value 0 for separating two clusters
  event.chain[3].sep = (int) track_mwpc[19];
  // delete all clusters with less or equal number of ADCs
  event.chain[3].del = (int) track_mwpc[20] - 1;
  event.chain[3].t_right = (int) track_mwpc[21];
  event.chain[3].t_left  = (int) track_mwpc[22];
  event.chain[3].sat_ratio = track_mwpc[2];

  for (int i=0; i<30; i++) {
    time_offsetF[i] = TDC_time_corr_[i];
    time_slopeF[i]  = TDC_time_corr_[i+30];
    time_offsetG[i] = TDC_time_corr_[i+60];
    time_slopeG[i]  = TDC_time_corr_[i+90];

    TDC_correction[i]    = TDC_correction_[i];
    TDC_correction[i+30] = TDC_correction_[i+30];
    TDC_correction[i+60] = TDC_correction_[i+60];
    TDC_correction[i+90] = TDC_correction_[i+90];

    TDC_offset[i]    = TDC_offset_[i];
    TDC_offset[i+30] = TDC_offset_[i+30];
    TDC_offset[i+60] = TDC_offset_[i+60];
    TDC_offset[i+90] = TDC_offset_[i+90];
  }

  int offset;
  struct t_inter inter;
  for (int c=0; c<4; c++)
  {
    event.chain[c].data_raw.resize(event.chain[c].cnt);
    event.chain[c].data_gain.resize(event.chain[c].cnt);
    event.chain[c].data_inter.resize(event.chain[c].cnt);
    event.chain[c].data_thresh.resize(event.chain[c].cnt);
    event.chain[c].data.resize(event.chain[c].cnt);
    event.chain[c].cluster.clear();
    event.chain[c].cluster_full.clear();
    event.chain[c].cluster_trunc.clear();

    // get gain from run.db
    // values for gain:  > 0
    //                   = 0 : disabled ADC
    //                   < 0 : interpolate ADC
    event.chain[c].gain.resize(event.chain[c].cnt);
    switch(c)
      {
      case 0:
	offset=550;
	break;
      case 1:
	offset=310;
	break;
      case 2:
	offset=240;
	break;
      case 3:
	offset=0;
	break;
      default:
	std::cout << "ERROR: MWPCDetector::init" << std::endl;
	exit(1);
	break;
      } // switch(c)
    for (int i=0; i<event.chain[c].cnt; i++)
      event.chain[c].gain[i]=gain[i+offset];
    
    // get threshold from run.db (one value for all ADCs, should be changed)
    event.chain[c].thresh.resize(event.chain[c].cnt);

    if (c == 0 || c == 2) 
      fill(event.chain[c].thresh.begin(), 
	   event.chain[c].thresh.end(), threshold_y);
    else if (c == 1 || c == 3) 
      fill(event.chain[c].thresh.begin(), 
	   event.chain[c].thresh.end(), threshold_x);

    // fill vector for interpolated ADCs
    event.chain[c].inter.clear();
    inter.length=0;
    for (int i=0; i<event.chain[c].cnt; i++)
      {
	if (event.chain[c].gain[i]<0)
	  {
	    if (inter.length==0)
	      {
		inter.adc=i;
		inter.length++;
	      }
	    else
	      inter.length++;
	  }
	else 
	  {
	    if (inter.length>0 && inter.adc!=0 && 
		inter.adc+inter.length!=event.chain[c].cnt)
	      {
		event.chain[c].inter.push_back(inter);
		inter.length=0;
	      }
	    else
	      inter.length=0;
	  }
      }
  } // for c

  event.track.clear();
  for (int t=0; t<4; t++)
    {
      event.tof[t].resize(PADDLE_F);
    }

  // check if quality != 0 from run.db
  check_input_value("quality scintillator walls",qual_walls);  
  check_input_value("quality phi",qual_phi[0]);  
  check_input_value("quality charge ratio M",qual_c_M[0]);  
  check_input_value("quality charge ratio L",qual_c_L[0]);  


  if (use_trigger)
    {
      std::ifstream triggerfile(filename);
      int label;
      if (triggerfile)
	{
	  for (int G=0; G<30; G++) triggerfile >> label; // reading G labels
	  for (int G=0; G<30; G++)
	    {triggerfile >> label; // reading H label
	      for (int H=0; H<30; H++)
		if (triggerfile >> trigger[G][H]) 
		  {if (use_trigger == 2) std::cout << trigger[G][H];}
		else
		  std::cerr << "Error reading bin G" 
			    << G << " H" << H << std::endl;
	      if (use_trigger == 2) std::cout << std::endl;
	    }
	  triggerfile.close();
	}
      else  std::cerr << "<<MWPCDetectors.cc> No trigger file found: "  
		      << filename << std::endl;
    }

  return 0;
}

// ========================================================

void MWPCDetectors::reset()
{
  for (int i=0; i<4; i++)
    {
      event.chain[i].cluster.clear();
      event.chain[i].cluster_full.clear();
      event.chain[i].cluster_trunc.clear();
    }
  event.track.clear();
}

// ========================================================

int MWPCDetectors::get_raw_data()
{
  AquaNodeUShort **chain_aqua;
  int cnt=0;
  for (int c=0; c<4; c++)
    {
      switch(c)
	{
	case 0:
	  chain_aqua=My;
	  break;
	case 1:
	  chain_aqua=Mx;
	  break;
	case 2:
	  chain_aqua=Ly;
	  break;
	case 3:
	  chain_aqua=Lx;
	  break;
	default:
	  std::cout << "ERROR: MWPCDetector::get_raw_data" << std::endl;
	  exit(1);
	  break;
	} // switch(c)
      for (int i=0; i<event.chain[c].cnt; i++)
	{
	  if (chain_aqua[i]->ok())
	    {
	      cnt++;
	      event.chain[c].data[i]=*chain_aqua[i];
	    }
	  else event.chain[c].data[i]=0;
	} // for i
    } // for c

  return cnt;
}

// ========================================================

void MWPCDetectors::get_data_gain()
{
  for (int c=0; c<4; c++)
    {
      for (int i=0; i<event.chain[c].cnt; i++)
	{
	  if (event.chain[c].gain[i]>=0)
	    {
	      if (event.chain[c].data[i]*event.chain[c].gain[i]<255)
		event.chain[c].data[i]= event.chain[c].data[i] 
		  * event.chain[c].gain[i];
	      else event.chain[c].data[i]=255.0;
	    }
	  else event.chain[c].data[i]=0;
	}
    }
}

// ========================================================

void MWPCDetectors::get_data_inter()
// interpolate missing adc's or adc groups
// missing ADCs are marked in gain as -1
// missing ADCs at begin or end are not interpolated
// linear interpolation: y = mx +b
{
  int gap_start, gap_end;
  double m, b;
  for (int c=0; c<4; c++)
    {
      for (unsigned int i=0; i<event.chain[c].inter.size(); i++)
	{
	  gap_start = event.chain[c].inter[i].adc - 1;
	  gap_end   = gap_start + event.chain[c].inter[i].length + 1;
	  m = ( event.chain[c].data[gap_end] - event.chain[c].data[gap_start])
	    / (gap_end - gap_start);
	  b = event.chain[c].data[gap_start] - m * gap_start;
	  for (int j=event.chain[c].inter[i].adc;
	       j<event.chain[c].inter[i].adc + event.chain[c].inter[i].length;
	       j++)
	    {
	      event.chain[c].data[j] = m * j + b;
	    } // for j
	} // for i
    } // for c
}


// ========================================================
void MWPCDetectors::remove_threshold()
{
  for (int c=0; c<4; c++)
    {
      for (unsigned int i=0; i<event.chain[c].data_thresh.size(); i++)
	{
	  if (event.chain[c].data[i] <= event.chain[c].thresh[i] && 
	      event.chain[c].data[i] > 0)
	    event.chain[c].data[i] = 0;
	} // for i
    } // for c
}

// ========================================================

bool pred_greater_zero(double element)
{
  if (element>0) return true;
  else return false;
}

void MWPCDetectors::build_cluster()
{
  struct t_cluster cluster;
  std::vector<double>::iterator it_start;
  std::vector<double>::iterator it_end;
  for (int c=0; c<4; c++)
    {
      it_start = event.chain[c].data.begin();
      while (it_start != event.chain[c].data.end())
	{
	  it_start = find_if (it_start, event.chain[c].data.end(), 
			      pred_greater_zero);
	  if (it_start!=event.chain[c].data.end())
	    {
	      it_end = search_n(it_start, event.chain[c].data.end(), 
				event.chain[c].sep, 0);
	      cluster.start = it_start - event.chain[c].data.begin();
	      cluster.end = it_end - event.chain[c].data.begin() - 1;
	      if (cluster.end - cluster.start+1 > event.chain[c].del)
		event.chain[c].cluster.push_back(cluster);
	      it_start = it_end;
	    }
	} // while
    } // for c
}

// ========================================================

void MWPCDetectors::build_cluster_trunc()
// Vorraussetztung: build_cluster() und get_cluster_data() wurde bereits
// durchgeführt (benötigt werden start, ende, max_adc und max_value)
// start und ende werden neu gesetzt, get_cluster_data() muss danach
// erneut aufgerufen werden
{
  #ifdef DEBUG_MWPC_CLUSTER
  std::cout << "MWPCDetectors::build_cluster_trunc" << std::endl;
  #endif

  int cnt_l, cnt_r, adc, left, right, tleft, tright, extra_width;
  int peak_r, peak_l;
  uint initial_size;
  double charge_left, charge, charge_right, charge_tleft, charge_tright;
  struct t_cluster start_cluster, end_cluster;

  for (int c=0; c<4; c++)
    {
      initial_size = event.chain[c].cluster.size();
      for (uint i=0; i < initial_size; i++)
	{
	  peak_r = 0; peak_l= 0;

          #ifdef DEBUG_MWPC_CLUSTER
	  std::cout << "--> analysing cluster " << i 
	            << " in MWPC chain " << c
		    << "  starting at " << event.chain[c].cluster[i].start
		    << "  ending at " << event.chain[c].cluster[i].end
		    << "  max at " << event.chain[c].cluster[i].max_adc
		    << std::endl;
          #endif

	  // find new start position left of truncated cluster

	  // start search at max_adc
	  adc = event.chain[c].cluster[i].max_adc;
	  left = event.chain[c].cluster[i].start;

	  if (event.chain[c].cluster[i].max_value > big_charge) extra_width = 1;
	  else extra_width = 0;

	  while (event.chain[c].data[adc] == 
		 event.chain[c].cluster[i].max_value && adc > left) adc--;

	  cnt_l = 1;
	  while (cnt_l < event.chain[c].t_left + extra_width && adc > left && 
		 event.chain[c].data[adc] > sep_threshold)
	    {adc--; cnt_l++;}
	  
	  if (event.chain[c].data[adc] < event.chain[c].cluster[i].max_value 
	      * peak_min || left == adc) {peak_l = 1; left = adc;}

	  // find new end position of truncated cluster
	  adc = event.chain[c].cluster[i].max_adc;
	  right = event.chain[c].cluster[i].end;

	  while (event.chain[c].data[adc] == 
		 event.chain[c].cluster[i].max_value && adc < right) adc++;

	  cnt_r = 1;
	  while (cnt_r < event.chain[c].t_right + extra_width && adc < right &&
		 event.chain[c].data[adc] > sep_threshold) {adc++; cnt_r++;}

	  if (event.chain[c].data[adc] < event.chain[c].cluster[i].max_value
	      * peak_min || right == adc) {peak_r = 1; right = adc;}

	  // checking for peak-like structures
	  // you shall never try to understand this, but it works...
	  if ((left && event.chain[c].data[left-1]  
	       > event.chain[c].cluster[i].max_value) ||
	      (right < event.chain[c].cnt && event.chain[c].data[right+1] 
	       > event.chain[c].cluster[i].max_value) ||
	      (peak_l == 0 && peak_r == 0)) // no peak
	    {
	      #ifdef DEBUG_MWPC_CLUSTER
	      std::cout << "--> cluster not peaked -> zeroed" << std::endl;
	      std::cout << "\tid=" << event.chain[c].cluster[i].id
			<< "\tstart=" << event.chain[c].cluster[i].start
			<< "\tend=" << event.chain[c].cluster[i].end
			<< "\tpeak ADC no.=" 
			<< event.chain[c].cluster[i].max_adc
			<< "\tpeak ADC value=" 
			<<  event.chain[c].cluster[i].max_value
			<< "\tcharge=" 
			<<  event.chain[c].cluster[i].charge
			<< "\tcenter-of-charge=" 
			<<  event.chain[c].cluster[i].cofc
			<< std::endl;
              #endif
	      event.chain[c].cluster[i].start = 0; 
	      event.chain[c].cluster[i].end = 0; 
	    } // no peak
	  else // peak
	    {
	      #ifdef DEBUG_MWPC_CLUSTER
	      std::cout << "--> cluster is peaked" << std::endl;
              #endif

	      // looking for additional peaks

	      // find charge ratios
	      charge_left = 0, charge = 0, charge_right = 0;
	      charge_tleft = 0, charge_tright = 0;
	      for (int j=event.chain[c].cluster[i].start; j<=left - 1; j++)
		{charge_left = charge_left + event.chain[c].data[j];}
	      for (int j=left; j<=right; j++)
		{charge = charge + event.chain[c].data[j];}
	      for (int j=right+1; j<=event.chain[c].cluster[i].end; j++)
		{charge_right = charge_right + event.chain[c].data[j];}
	      
	      if ((left - event.chain[c].del > event.chain[c].cluster[i].start)
		  && charge_left > charge * event.chain[c].sat_ratio)
		{
		  if (left-1 - event.chain[c].t_left - event.chain[c].del 
		      > event.chain[c].cluster[i].start )
		    {
		      tleft = left-1 - event.chain[c].t_left;
		      // check charge between left - 1 and tleft
		      for (int j= tleft; j<=left-1; j++)
			{charge_tleft = charge_tleft + event.chain[c].data[j];}
		    }
		  else {charge_tleft = charge_left; tleft = left;}
		  
	          #ifdef DEBUG_MWPC_CLUSTER
		  std::cout << "--> testing charges left to the cluster:  "
			    << "  charge left:  " << charge_left 
			    << "  left ADC:  " << left
			    << "  charge first 2 channels left:  " 
			    << charge_tleft 
			    << "  2 channels left:  " 
			    << tleft << std::endl;
	          #endif

		  if (charge_tleft > charge_left * peak_min)
		    // create new cluster between start and left - 1
		    start_cluster.end   = left - 1;
		  else
		    // create new cluster between start and tleft
		    start_cluster.end   = tleft;

		  start_cluster.start = event.chain[c].cluster[i].start;
		  if (start_cluster.start + event.chain[c].del 
		      < start_cluster.end)
		    {
		      event.chain[c].cluster.push_back(start_cluster);
  	              #ifdef DEBUG_MWPC_CLUSTER
		      std::cout << "--> created left cluster between " 
				<< start_cluster.start
				<< " and " << start_cluster.end
				<< " ; new cluster size is " 
				<< event.chain[c].cluster.size()
				<< std::endl;
	              #endif
		    }
		}

	      if ( (right + event.chain[c].del < event.chain[c].cluster[i].end) 
		   && charge_right >  charge * event.chain[c].sat_ratio)
		{
		  if (right+1 + event.chain[c].t_right + event.chain[c].del 
		      < event.chain[c].cluster[i].end)
		    {
		      tright = right+1 + event.chain[c].t_right;
		      // check charge between right + 1 and tright
		      for (int j= right+1; j<=tright; j++)
			{charge_tright = charge_tright+event.chain[c].data[j];}
		    }
		  else {charge_tright = charge_right; tright = right;}

	          #ifdef DEBUG_MWPC_CLUSTER
		  std::cout << "--> testing charges right to the cluster:  "
			    << "  charge right:  " << charge_right
			    << "  right ADC:  " << right
			    << "  charge first 2 channels right:  "
			    << charge_tright 
			    << "  2 channels right:  " << tright << std::endl;
	          #endif

		  if (charge_tright > charge_right * peak_min)
		    // create new cluster between right + 1 and end
		    end_cluster.start = right + 1;
		  else
		    // create new cluster between tright and end
		    end_cluster.start = tright;
		  end_cluster.end   = event.chain[c].cluster[i].end;
		  
		  if (end_cluster.start + event.chain[c].del < end_cluster.end)
		    {
		      event.chain[c].cluster.push_back(end_cluster);
    	              #ifdef DEBUG_MWPC_CLUSTER
		      std::cout << "--> created right cluster between " 
				<< end_cluster.start
				<< " and " << end_cluster.end
				<< " ; new cluster size is " 
				<< event.chain[c].cluster.size()
				<< std::endl;
	              #endif
		    }
		}

              #ifdef DEBUG_MWPC_CLUSTER
	      std::cout << "--> cluster " << i << " starts now at " 
			<< left
			<< " and ends at " << right << std::endl;
              #endif
	      event.chain[c].cluster[i].start = left;
	      event.chain[c].cluster[i].end   = right;

	    } // else peak
	} // for i
    } // for c
}

// ========================================================

double MWPCDetectors::get_cofc_lab(int chain, double cofc)
// calculate center of charge in lab system; unit: mm
{
  double cofc_lab;
  switch(chain)
    {
    case 0: // My
      cofc_lab = -5.0 * cofc + 172.5 - yM;
      break;
    case 1: // Mx
      cofc_lab = 5.0 * cofc - xLM;
      break;
    case 2: // Ly
      cofc_lab = -5.0 * cofc + 172.5 - yL;
      break;
    case 3: // Lx
      cofc_lab = 5.0 * cofc;
      break;
    default:
      cofc_lab = 0;
      std::cout << "ERROR: MWPCDetectors::get_cofc_lab" << std::endl;
      exit(1);
      break;
    } // switch
  return cofc_lab;
}

// ========================================================

bool pred_max(double i, double j)
{
  return i<=j;
}

void MWPCDetectors::get_cluster_data()
{
  int start, end;
  uint size;
  double charge;
  double hit_numerator;
  double sigma, adc_err, hit_err;
  std::vector<double>::iterator it;
  std::vector<t_cluster>::iterator it_cl;

  for (int c=0; c<4; c++)
    {
      it_cl = event.chain[c].cluster.begin();
      size = event.chain[c].cluster.size();
      for (unsigned int i = 0; i < size; i++)
	{
	  charge = 0.0;
	  hit_numerator = 0.0;
	  sigma = 0.0;
	  hit_err = 0.0;

	  while (i < size && event.chain[c].cluster[i].start == 
	      event.chain[c].cluster[i].end)
	    {
              #ifdef DEBUG_MWPC_CLUSTER
	      std::cout << "--> erased cluster " << i 
			<< " in MWPC chain " << c << std::endl;
	      #endif
	      event.chain[c].cluster.erase(it_cl + i);
	      size--;
	    }

	  if (i < size)
	    {
	      // === cluster_start ad cluster_end
	      start = event.chain[c].cluster[i].start;
	      end = event.chain[c].cluster[i].end;

	      // === cluster_id
	      event.chain[c].cluster[i].id = i;

	      // === adc multiplicity in cluster
	      event.chain[c].cluster[i].cnt = end - start + 1;

	      // === cluster charge and hit position
	      for (int j=start; j<=end; j++)
	    {
	      charge = charge + event.chain[c].data[j];
	      hit_numerator = hit_numerator + (event.chain[c].data[j] * j);
	    } // for j
	      event.chain[c].cluster[i].charge   = charge;
	      event.chain[c].cluster[i].cofc     = hit_numerator/charge;
	      event.chain[c].cluster[i].cofc_lab = 
		get_cofc_lab(c, event.chain[c].cluster[i].cofc);

	      // === ADC No. with highest value in cluster and highest value
	      it = max_element(event.chain[c].data.begin()+start, 
			       event.chain[c].data.begin()+end+1, pred_max);
	      event.chain[c].cluster[i].max_value = *it;
	      event.chain[c].cluster[i].max_adc   = it 
		- event.chain[c].data.begin();
	      
	      // === width of charge distribution and sys. error of hit position
	      for (int j=start; j<=end; j++)
		{
		  sigma = sigma + pow(j-event.chain[c].cluster[i].cofc,2)*
		    event.chain[c].data[j]/event.chain[c].cluster[i].charge;
		  adc_err = adc_err_base 
		    + adc_err_var/100.0*sqrt(event.chain[c].data[j]);
		  hit_err = hit_err
		    + pow((j-event.chain[c].cluster[i].cofc)/
			  event.chain[c].cluster[i].charge,2)
		    * pow(adc_err,2);
		}
	      event.chain[c].cluster[i].cofc_sig = sqrt(sigma);
	      event.chain[c].cluster[i].cofc_err = sqrt(hit_err);
	    } // i < size
	} // for i
    } // for c
  
  return;
}

// ========================================================

bool MWPCDetectors::pred_chain_max_value(struct t_cluster c1, struct t_cluster c2)
{
  return c1.max_value < c2.max_value;
}

//void MWPCDetectors::gen_cluster_quality()
/* calculate the quality of clusters in one chain:
   Find the highest ADC value from the chain and compare these value
   with the highest ADC value of every cluster.
   Build the ratio (highest value chain)/(highest value cluster)
   If (ratio < qual_cluster_min) set cluster quality to ratio
   else set cluster quality to 1.              */
/*
{
  std::vector<struct t_cluster>::iterator it;
  double q_max_value;
  for (int c=0; c<4; c++)
    {
      if (event.chain[c].cluster.size()>0)
	{
	  it = max_element(event.chain[c].cluster.begin(),
			   event.chain[c].cluster.end(),
			   pred_chain_max_value);
	  for (unsigned int i=0; i<event.chain[c].cluster.size(); i++)
	    {
	      q_max_value = event.chain[c].cluster[i].max_value * 1.0 / it->max_value;
	      if (q_max_value < qual_cluster_min)
		event.chain[c].cluster[i].q_max_value = q_max_value;
	      else event.chain[c].cluster[i].q_max_value = 1.0;
	    } // for i
	} // if
    } // for c
}
*/

// ========================================================

double MWPCDetectors::get_theta(double mx_cofc, double lx_cofc)
{
  return atan ( (lx_cofc-mx_cofc) / zLM) * degree;
}

double MWPCDetectors::get_phi(double my_cofc, double ly_cofc)
{
  return atan ( (ly_cofc-my_cofc) / zLM ) * degree; 
}

// ========================================================

void MWPCDetectors::gen_track_list()
{
  struct t_track element;
  event.track.clear();
  for (unsigned int mx=0; mx<event.chain[1].cluster.size(); mx++)
    {
      for (unsigned int my=0; my<event.chain[0].cluster.size(); my++)
	{
	  for (unsigned int lx=0; lx<event.chain[3].cluster.size(); lx++)
	    {
	      for (unsigned int ly=0; ly<event.chain[2].cluster.size(); ly++)
		{
		  element.id[0] = my;
		  element.id[1] = mx;
		  element.id[2] = ly;
		  element.id[3] = lx;

		  element.theta = get_theta(event.chain[1].cluster[mx].cofc_lab,
					    event.chain[3].cluster[lx].cofc_lab);
		  element.phi   = get_phi(event.chain[0].cluster[my].cofc_lab,
					  event.chain[2].cluster[ly].cofc_lab);

		  element.q_FGtrig   = 0;
		  element.q_c_L      = 0;
		  element.q_c_M      = 0;
		  element.q_theta    = 0;
		  element.q_phi      = 0;
		  element.q_wall_F   = 0;
		  element.q_wall_G   = 0;
		  element.q_time_F   = 0;
		  element.q_time_G   = 0;
		  element.quality    = 0;

		  event.track.push_back(element);
		} // for lx
	    } // for ly
	} // for mx
    } // for my
}


// ========================================================

void MWPCDetectors::get_q_theta()
{
  for (std::list<struct t_track>::iterator it=event.track.begin(); 
       it!=event.track.end(); ++it)
    {
      double hitLx;
      double delta;
      hitLx = event.chain[3].cluster[it->id[3]].cofc_lab;

      if ( (it->theta < theta_max + (hitLx-360)*(slope_max-0.001) ) && 
	   (it->theta > theta_min + (hitLx-50)*slope_max) )
	it->q_theta = 1.0;
      else if (it->theta < theta_min + (hitLx-50)*slope_max)
	{ 
	  delta = it->theta - (theta_min + (hitLx-50)*slope_max); 
	  // using the same sigma as for the vertical angle
	  it->q_theta = calc_quality(qual_phi[0], delta, 0);
	}
      else if (it->theta > (theta_max + (hitLx-360)*(slope_max-0.001)))
	{
	  delta = it->theta - (theta_max + (hitLx-360)*(slope_max-0.001));
	  // using the same sigma as for the vertical angle
	  it->q_theta = calc_quality(qual_phi[0], delta, 0);
	}
      // else it->q_theta = 0;
    }
}

// ========================================================


void MWPCDetectors::get_q_phi()
{
  double phi_exp, delta;
  for (std::list<struct t_track>::iterator it=event.track.begin(); 
       it!=event.track.end(); ++it)
    {
      phi_exp   = offset_phi + slope_phi * 
	event.chain[2].cluster[it->id[2]].cofc_lab;
      delta     = phi_exp - it->phi;
      it->q_phi = calc_quality(qual_phi[0], delta, qual_phi[1]);
    }
}

// ========================================================

void  MWPCDetectors::get_q_c()
{
  double QMy_exp, QLy_exp, delta;
  double QMy, QMx, QLy, QLx;
  double scaled_qsigma, scaled_qwidth;
  
  for (std::list<struct t_track>::iterator it=event.track.begin(); 
       it!=event.track.end(); ++it)
    {
      // chamber M
      QMy = event.chain[0].cluster[it->id[0]].charge;
      QMx = event.chain[1].cluster[it->id[1]].charge;
      QMy_exp = fit_c_M[0]
	+ fit_c_M[1] * QMx
	+ fit_c_M[2] * pow(QMx,2)
	+ fit_c_M[3] * pow(QMx,3);
      delta = QMy_exp - QMy;

      scaled_qsigma  = qual_c_M[0] + QMx*qual_c_M[2];
      scaled_qwidth  = qual_c_M[1] + QMx*qual_c_M[3];
      
      it->q_c_M = calc_quality(scaled_qsigma, delta, scaled_qwidth);

      #ifdef DEBUG_MWPC_CHARGE_QUALITY
            std::cout << "QMy " << QMy << "  QMx " << QMx;
            std::cout << "  quality " << it->q_c_M << std::endl;
      #endif

      // chamber L
      QLy = event.chain[2].cluster[it->id[2]].charge;
      QLx = event.chain[3].cluster[it->id[3]].charge;
      QLy_exp = fit_c_L[0]
	+ fit_c_L[1] * QLx
	+ fit_c_L[2] * pow(QLx,2)
	+ fit_c_L[3] * pow(QLx,3);
      delta = QLy_exp - QLy;

      scaled_qsigma  = qual_c_L[0] + QLx*qual_c_L[2];
      scaled_qwidth  = qual_c_L[1] + QLx*qual_c_L[3];

      it->q_c_L = calc_quality(scaled_qsigma, delta, scaled_qwidth);

      #ifdef DEBUG_MWPC_CHARGE_QUALITY
      std::cout << "QLy " << QLy << "  QLx " << QLx;
      std::cout << "  quality " << it->q_c_L << std::endl;
      #endif

    }
}

// ========================================================

int MWPCDetectors::get_tof_data()
{
  AquaNodeUShort **tof_tdc;
  double RAW;
  int cnt=0;

  for (int t=0; t<4; t++)
    {
      switch(t)
		{
	case 0:
	  tof_tdc = tdcft;
	  break;
	case 1:
	  tof_tdc = tdcfb;
	  break;
	case 2:
	  tof_tdc = tdcgt;
	  break;
	case 3:
	  tof_tdc = tdcgb;
	  break;
	default:
	  std::cout << "ERROR: MWPCDetectors::get_tof_data" << std::endl;
	  exit(1);
	  break;
	} // switch
      for (unsigned int i=0; i<event.tof[t].size(); i++)
	{
	  if (tof_tdc[i]->ok() && tof_tdc[i]->scalar() > 0)
	    {
	      cnt++;
	      if (*tof_tdc[i] > 4095)  // TDC High Range correction
		RAW = (*tof_tdc[i]-4095)*8+TDC_correction[i+t*30]-24;
	      else RAW = *tof_tdc[i];
	      event.tof[t][i] = RAW + 40*TDC_offset[i+t*30];
	    }
	  else event.tof[t][i] = 0;
	} // for i
    } // for t
  return cnt;
}

// ========================================================

void MWPCDetectors::get_q_tof()
{
  double hitF_exp, paddleF_exp, yF_exp;
  double hitG_exp, paddleG_exp, yG_exp;
  double hitLx, hitLy;
  double deltaF, multiF, deltaG, multiG, quality;
  double deltatF, deltatG, deltaFy, deltaGy;
  double Fxprime, Gxprime, Fzprime, Fz, Gzprime, Gz;
  int paddleF, paddleG;
  // triggerG;

  #ifdef DEBUG_TOF_DATA
  std::cout << "====== DEBUG_TOF_DATA ======" << std::endl;
  #endif

  for (std::list<t_track>::iterator it=event.track.begin(); 
       it!=event.track.end(); ++it)
    {
      // chamber coordinates
      hitLx = event.chain[3].cluster[it->id[3]].cofc_lab;
      hitLy = event.chain[2].cluster[it->id[2]].cofc_lab;

      // calculate hit position in wall F coordinates (mm)
      Fxprime  = hitLx - xLF + tan(it->theta/degree)*zLF;
      hitF_exp = Fxprime * cos(it->theta/degree)/
	cos((it->theta+alphaF)/degree);

      Fzprime = zLF + tan(alphaF/degree)*(hitLx-xLF);
      Fz= Fzprime * cos(alphaF/degree)/cos((alphaF+it->theta)/degree);

      // calculate y position in wall F coordinates (mm)
      yF_exp = hitLy - yF + tan(it->phi/degree)*Fz;

       // calculate hit paddle in wall F
      paddleF_exp = hitF_exp/lF - 0.5;
      deltaF = PADDLE_F;
      deltatF= 0;
      multiF = 0;
      
      // calculate hit position in wall G coordinates (mm)
      Gxprime = hitLx - xLG + tan(it->theta/degree)*zLG;
      hitG_exp =  Gxprime * cos(it->theta/degree)
	/cos((it->theta+alphaG)/degree);

      Gzprime = zLG + tan(alphaG/degree)*(hitLx-xLG);
      Gz= Gzprime * cos(alphaG/degree)/cos((alphaG+it->theta)/degree);

       // calculate y position in wall G coordinates (mm)
      yG_exp = hitLy - yG + tan(it->phi/degree)*Gz;

      // calculate hit paddle in wall G
      paddleG_exp = hitG_exp/lG - 0.5;
      deltaG = PADDLE_G;
      deltatG= 0;
      multiG = 0;

      // -- calculate qualities --

      paddleF = -1;
      // loop over wall F
      for (int j=0; j<PADDLE_F; j++)
	{
	  // requiring TOP & BOT signals for quality check
	  if (event.tof[0][j] > 0 && event.tof[1][j] > 0) // wall F
	    {
	      multiF++;
	      if (fabs(paddleF_exp - j) < deltaF)
		{deltaF = fabs(paddleF_exp - j); paddleF = j;}
	    }
	} // for j

      if (multiF > 0)
	{
	  quality = calc_quality(qual_walls, deltaF, 0.5);
	  it->q_wall_F = quality;

	  if (time_slopeF[paddleF] != 0)
	    {
	      // time difference
	      deltatF = event.tof[1][paddleF] - event.tof[0][paddleF];

	      deltaFy = yF_exp - (time_offsetF[paddleF] + 
				  deltatF * time_slopeF[paddleF]);
	      quality = calc_quality(qual_plateau_time, deltaFy, 
				     qual_gaussian_time);
	    }
	  else quality = 1.; // no quality without parameters

	  it->q_time_F = quality;
	} // multi F
      else
	{
	  it->q_wall_F = 0; // no information from wall F
	  it->q_time_F = 0; // no information from wall F
	}

      paddleG = -1;
      // loop over wall G
      for (int j=0; j<PADDLE_G; j++)
	{
	  // requiring TOP & BOT signals for quality check
	  if (event.tof[2][j]>0 && event.tof[3][j]>0) // wall G
	    {
	      multiG++;
	      if (fabs(paddleG_exp - j) < deltaG)
		{deltaG = fabs(paddleG_exp - j); paddleG = j;}
	    }
	} // for j
      
      if (multiG > 0)
	{
	  quality = calc_quality(qual_walls, deltaG, 0.5);
	  it->q_wall_G = quality;

	  if (time_slopeG[paddleG] != 0)
	    {
	      // time difference
	      deltatG = event.tof[3][paddleG] - event.tof[2][paddleG];

	      deltaGy = yG_exp - (time_offsetG[paddleG] + 
	  			  deltatG * time_slopeG[paddleG]);

	      quality = calc_quality(qual_plateau_time, deltaGy, 
				     qual_gaussian_time);
	    }
	  else quality = 1.; // no quality without parameters

	  it->q_time_G = quality;
	} // multi G
      else
	{
	  it->q_wall_G = 0; // no information from wall G
	  it->q_time_G = 0; // no information from wall G
	}
      
      /*      triggerG = paddleG + 32;
      
      if (
	  (it->q_wall_G > quality_min) && (it->q_wall_F > quality_min) && (
	  (paddleF == (0) && (triggerG == (30) || triggerG == (31)|| 
			      triggerG == (32) || triggerG == (33)||
			      triggerG == (34) || triggerG == (35)||
			      triggerG == (36))) ||
	  (paddleF == (1) && (triggerG == (31) || triggerG == (32) || 
			      triggerG == (33) || triggerG == (34) ||
			      triggerG == (35) || triggerG == (36) || 
			      triggerG == (37))) ||
	  (paddleF == (2) && (triggerG == (32) || triggerG == (33) || 
			      triggerG == (34) || triggerG == (35) ||
			      triggerG == (36) || triggerG == (37) || 
			      triggerG == (38))) ||
	  (paddleF == (3) && (triggerG == (33) || triggerG == (34) || 
			      triggerG == (35) || triggerG == (36) ||
			      triggerG == (37) || triggerG == (38) || 
			      triggerG == (39))) ||
	  (paddleF == (4) && (triggerG == (34) || triggerG == (35) || 
			      triggerG == (36) || triggerG == (37) ||
			      triggerG == (38) || triggerG == (39) || 
			      triggerG == (40))) ||
	  (paddleF == (5) && (triggerG == (35) || triggerG == (36) || 
			      triggerG == (37) || triggerG == (38) ||
			      triggerG == (39) || triggerG == (40) || 
			      triggerG == (41))) ||
	  (paddleF == (6) && (triggerG == (36) || triggerG == (37) || 
			      triggerG == (38) || triggerG == (39) ||
			      triggerG == (40) || triggerG == (41) || 
			      triggerG == (42))) ||
	  (paddleF == (7) && (triggerG == (37) || triggerG == (38) || 
			      triggerG == (39) || triggerG == (40) ||
			      triggerG == (41) || triggerG == (42) || 
			      triggerG == (43))) ||
	  (paddleF == (8) && (triggerG == (38) || triggerG == (39) || 
			      triggerG == (40) || triggerG == (41) ||
			      triggerG == (42) || triggerG == (43) || 
			      triggerG == (44))) ||
	  (paddleF == (9) && (triggerG == (39) || triggerG == (40) || 
			      triggerG == (41) || triggerG == (42) ||
			      triggerG == (43) || triggerG == (44))) ||
	  (paddleF == (10) && (triggerG == (40) || triggerG == (41) || 
			       triggerG == (42) || triggerG == (43) || 
			       triggerG == (44) || triggerG == (45))) ||
	  (paddleF == (11) && (triggerG == (41) || triggerG == (42) || 
			       triggerG == (43) || triggerG == (44) || 
			       triggerG == (45))) ||
	  (paddleF == (12) && (triggerG == (42) || triggerG == (43) || 
			       triggerG == (44) || triggerG == (45) || 
			       triggerG == (46))) ||
	  (paddleF == (13) && (triggerG == (43) || triggerG == (44) || 
			       triggerG == (45) || triggerG == (46) || 
			       triggerG == (47))) ||
	  (paddleF == (14) && (triggerG == (44) || triggerG == (45) || 
			       triggerG == (46) || triggerG == (47) || 
			       triggerG == (48))) ||
	  (paddleF == (15) && (triggerG == (45) || triggerG == (46) || 
			       triggerG == (47) || triggerG == (48) || 
			       triggerG == (49))) ||
	  (paddleF == (16) && (triggerG == (46) || triggerG == (47) || 
			       triggerG == (48) || triggerG == (49) || 
			       triggerG == (50))) ||
	  (paddleF == (17) && (triggerG == (47) || triggerG == (48) || 
			       triggerG == (49) || triggerG == (50) || 
			       triggerG == (51))) ||
	  (paddleF == (18) && (triggerG == (48) || triggerG == (49) || 
			       triggerG == (50) || triggerG == (51) || 
			       triggerG == (51))) ||
	  (paddleF == (19) && (triggerG == (49) || triggerG == (50) || 
			       triggerG == (51) || triggerG == (52) || 
			       triggerG == (53))) ||
	  (paddleF == (20) && (triggerG == (50) || triggerG == (51) || 
			       triggerG == (52) || triggerG == (53) || 
 			       triggerG == (54))) ||
	  (paddleF == (21) && (triggerG == (51) || triggerG == (52) || 
			       triggerG == (53) || triggerG == (54) || 
 			       triggerG == (55))) ||
	  (paddleF == (22) && (triggerG == (52) || triggerG == (53) || 
			       triggerG == (54) || triggerG == (55) || 
 			       triggerG == (56))) ||
	  (paddleF == (23) && (triggerG == (53) || triggerG == (54) || 
			       triggerG == (55) || triggerG == (56) || 
			       triggerG == (57))) ||
	  (paddleF == (24) && (triggerG == (54) || triggerG == (55) || 
			       triggerG == (56) || triggerG == (57) || 
			       triggerG == (58))) ||
	  (paddleF == (25) && (triggerG == (55) || triggerG == (56) || 
			       triggerG == (57) || triggerG == (58) || 
			       triggerG == (59))) ||
	  (paddleF == (26) && (triggerG == (56) || triggerG == (57) || 
			       triggerG == (58) || triggerG == (59))) ||
	  (paddleF == (27) && (triggerG == (57) || triggerG == (58) || 
			       triggerG == (59))) ||
	  (paddleF == (28) && (triggerG == (58) || triggerG == (59))) ||
	  (paddleF == (29) && (triggerG == (59)))
									   )) 
	{it->q_FGtrig = 1.;}
      else if (use_trigger == 0) {it->q_FGtrig = 1.;}
      else {it->q_FGtrig = 0.;}
      */

      if (paddleF < 30 && paddleG < 30 && 
	  (use_trigger == 0 || trigger[paddleG][paddleF] == 1) ) 
	it->q_FGtrig = 1;
      else it->q_FGtrig = 0;

      #ifdef DEBUG_TOF_DATA
      std::cout << "id=" << it->id[0]
		<< "\tid=" << it->id[1]
		<< "\tid=" << it->id[2]
		<< "\tid=" << it->id[3]
		<< "\tF hit=" << it->q_wall_F
		<< "\tF time=" << it->q_time_F
		<< "\tG hit=" << it->q_wall_G
		<< "\tG time=" << it->q_time_G
		<< "\tFG trig=" << it->q_FGtrig
		<< std::endl;
      #endif

    } // for it
}


// ========================================================

void MWPCDetectors::calc_total_quality()
{
  for (std::list<t_track>::iterator it=event.track.begin(); 
       it!=event.track.end(); ++it)
  {
    it->quality = 
        it->q_c_M
      * it->q_c_L
      * it->q_theta
      * it->q_phi
      * it->q_wall_G
      * it->q_wall_F
      * it->q_time_G
      * it->q_time_F
      * it->q_FGtrig
      ;
  }
}

// ========================================================

bool MWPCDetectors::pred_sortby_quality(struct t_track first, 
					struct t_track second)
{
  if (first.quality > second.quality) return true;
  else return false;
}

// ========================================================


double MWPCDetectors::calc_quality(double sigma, double delta, 
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
MWPCDetectors::handle()
{
  reset();

  get_raw_data();  // get raw data

  for (int c=0; c<4; c++) // save data in data_raw
      copy(event.chain[c].data.begin(),
	   event.chain[c].data.end(),
	   event.chain[c].data_raw.begin());

  get_data_gain(); 
  // multiply data with gain, set interpolated and disabled ADCs to 0

  for (int c=0; c<4; c++) // save data in data_gain
      copy(event.chain[c].data.begin(),
	   event.chain[c].data.end(),
	   event.chain[c].data_gain.begin());

  get_data_inter(); // interpolate ADCs

  for (int c=0; c<4; c++) // save data in data_inter
      copy(event.chain[c].data.begin(),
	   event.chain[c].data.end(),
	   event.chain[c].data_inter.begin());

  remove_threshold(); // remove threshold

  for (int c=0; c<4; c++) // save data in data_thresh
      copy(event.chain[c].data.begin(),
	   event.chain[c].data.end(),
	   event.chain[c].data_thresh.begin());

  build_cluster();
  get_cluster_data();
  for (int c=0; c<4; c++) // save cluster in cluster_full
    event.chain[c].cluster_full = event.chain[c].cluster;

  #ifdef DEBUG_MWPC_CLUSTER
  std::cout << "====== DEBUG_MWPC_CLUSTER ======" << std::endl;
  std::cout << "====== INITIAL CLUSTERS   ======" << std::endl;
  for (int j=0; j<4; j++)
    {
      std::cout << "cnt=" << event.chain[j].cluster_full.size() << std::endl;
      for (uint i=0; i<event.chain[j].cluster_full.size(); i++)
	{
	  std::cout << "\tid=" << event.chain[j].cluster_full[i].id
		    << "\tstart=" << event.chain[j].cluster_full[i].start
		    << "\tend=" << event.chain[j].cluster_full[i].end
		    << "\tpeak ADC no.=" 
		    << event.chain[j].cluster_full[i].max_adc
		    << "\tpeak ADC value=" 
		    <<  event.chain[j].cluster_full[i].max_value
		    << "\tcharge=" <<  event.chain[j].cluster_full[i].charge
		    << "\tcenter-of-charge=" 
		    <<  event.chain[j].cluster_full[i].cofc
		    << std::endl;
	}
    }
  #endif

  build_cluster_trunc(); // for initial clusters
  get_cluster_data();

  #ifdef DEBUG_MWPC_CLUSTER
  std::cout << "====== DEBUG_MWPC_CLUSTER ======" << std::endl;
  std::cout << "====== TRUNCATED CLUSTERS ======" << std::endl;
  for (int j=0; j<4; j++)
    {
      std::cout << "cnt=" << event.chain[j].cluster.size() << std::endl;
      for (uint i=0; i<event.chain[j].cluster.size(); i++)
	{
	  std::cout << "\tid=" << event.chain[j].cluster[i].id
		    << "\tstart=" << event.chain[j].cluster[i].start
		    << "\tend=" << event.chain[j].cluster[i].end
		    << "\tpeak ADC no.=" << event.chain[j].cluster[i].max_adc
		    << "\tpeak ADC value=" 
		    <<  event.chain[j].cluster[i].max_value
		    << "\tcharge=" <<  event.chain[j].cluster[i].charge
		    << "\tcenter-of-charge=" <<  event.chain[j].cluster[i].cofc
		    << std::endl;
	}
    }
  #endif

  build_cluster_trunc(); // for extra clusters
  get_cluster_data();

  #ifdef DEBUG_MWPC_CLUSTER
  std::cout << "====== DEBUG_MWPC_CLUSTER ======" << std::endl;
  std::cout << "====== FINAL CLUSTERS     ======" << std::endl;
  for (int j=0; j<4; j++)
    {
      std::cout << "cnt=" << event.chain[j].cluster.size() << std::endl;
      for (uint i=0; i<event.chain[j].cluster.size(); i++)
	{
	  std::cout << "\tid=" << event.chain[j].cluster[i].id
		    << "\tstart=" << event.chain[j].cluster[i].start
		    << "\tend=" << event.chain[j].cluster[i].end
		    << "\tpeak ADC no.=" << event.chain[j].cluster[i].max_adc
		    << "\tpeak ADC value=" 
		    <<  event.chain[j].cluster[i].max_value
		    << "\tcharge=" <<  event.chain[j].cluster[i].charge
		    << "\tcenter-of-charge=" <<  event.chain[j].cluster[i].cofc
		    << std::endl;
	}
    }
  #endif

  get_tof_data();

  gen_track_list();
  
  if (event.track.size() > 0)
    {
      get_q_theta();
      get_q_phi();
      get_q_c();
      get_q_tof();
      calc_total_quality();
      event.track.sort(pred_sortby_quality);
    }

  std::list<t_track>::iterator it;

  #ifdef DEBUG_MWPC_QUALITY
  std::cout << "====== DEBUG_MWPC_QUALITY ======" << std::endl;
  for (it=event.track.begin(); it!=event.track.end(); ++it)
    {
      std::cout << "\tid=" << it->id[0] << it->id[1] << it->id[2] << it->id[3] 
		<< "\tq_c_L="    <<  it->q_c_L
		<< "\tq_c_M="    <<  it->q_c_M
		<< "\tq_theta="  <<  it->q_theta
		<< "\tq_phi="    <<  it->q_phi
		<< "\tq_wall_F=" <<  it->q_wall_F
		<< "\tq_wall_G=" <<  it->q_wall_G
		<< "\tq_time_F=" <<  it->q_time_F
		<< "\tq_time_G=" <<  it->q_time_G
		<< "\tq_FGtrig=" <<  it->q_FGtrig
		<< "\tquality="  <<  it->quality
		<< std::endl;
    }
  #endif

  // FILL ONLINE HISTOGRAMS
  // Lx
  for (int i=0; i<event.chain[3].cnt; i++)
    {
      if (event.chain[3].data_raw[i]>0 && RAWADCNrLx) HMFill(RAWADCNrLx, i, 0, 1); // RAW
      if (event.chain[3].data_thresh[i]>0 && ADCNrLx && ADCChargeLx)
	{
	  HMFill(ADCNrLx, i, 0, 1);
	  HMFill(ADCChargeLx, i, 0, event.chain[3].data_thresh[i]);
	  HMSetDivide(ADCChargeLx, ADCNrLx);
	}
    }
  
  // Ly
  for (int i=0; i<event.chain[2].cnt; i++)
    {
      if (event.chain[2].data_raw[i]>0 && RAWADCNrLy) HMFill(RAWADCNrLy, i, 0, 1); // RAW
      if (event.chain[2].data_thresh[i]>0 && ADCNrLy && ADCChargeLy)
	{
	  HMFill(ADCNrLy, i, 0, 1);
	  HMFill(ADCChargeLy, i, 0, event.chain[2].data_thresh[i]);
	  HMSetDivide(ADCChargeLy, ADCNrLy);
	}
    }

  // Mx
  for (int i=0; i<event.chain[1].cnt; i++)
    {
      if (event.chain[1].data_raw[i]>0 && RAWADCNrMx) HMFill(RAWADCNrMx, i, 0, 1); // RAW
      if (event.chain[1].data_thresh[i]>0 && ADCNrMx && ADCChargeMx)
	{
	  HMFill(ADCNrMx, i, 0, 1);
	  HMFill(ADCChargeMx, i, 0, event.chain[1].data_thresh[i]);
	  HMSetDivide(ADCChargeMx, ADCNrMx);
	}
    }
  
  // My
  for (int i=0; i<event.chain[0].cnt; i++)
    {
      if (event.chain[0].data_raw[i]>0 && RAWADCNrMy) HMFill(RAWADCNrMy, i, 0, 1); // RAW
      if (event.chain[0].data_thresh[i]>0 && ADCNrMy && ADCChargeMy){
	HMFill(ADCNrMy, i, 0, 1);
	HMFill(ADCChargeMy, i, 0, event.chain[0].data_thresh[i]);
	HMSetDivide(ADCChargeMy, ADCNrMy);
      }
    }


  if (event.track.size() > 0)
    {
      // NOW CHOOSE THE TRACK
      int id_My, id_Mx, id_Ly, id_Lx;
      // number of tracks with quality > quality_min
      int cnt_good_track = 0;

      for (it=event.track.begin(); it!=event.track.end(); ++it)
	{
	  if (it == ++event.track.begin() && it->quality >= quality_min) 
	    sndquality -> handle(it->quality);
	  // return quality for 2nd best track
	  if (it->quality >= quality_min) cnt_good_track++;
	}

      //return data from best track
      it=event.track.begin();

      id_My = it->id[0];
      id_Mx = it->id[1];
      id_Ly = it->id[2];
      id_Lx = it->id[3];

      theta        -> handle(it->theta);
      phi          -> handle(it->phi); 
      quality      -> handle(it->quality);
      qualcL       -> handle(it->q_c_L);
      qualcM       -> handle(it->q_c_M);
      qualtheta    -> handle(it->q_theta);
      qualphi      -> handle(it->q_phi);
      qualwallF    -> handle(it->q_wall_F);
      qualwallG    -> handle(it->q_wall_G);
      qualtimeF    -> handle(it->q_time_F);
      qualtimeG    -> handle(it->q_time_G);

      tracks       -> handle(cnt_good_track);

      clusterLx    ->handle(event.chain[3].cluster.size());          
      // number of cluster in chain
      multiLx      ->handle(event.chain[3].cluster_full[id_Lx].cnt); 
      // number of ADCs in cluster (not truncated)
      chargeLx     ->handle(event.chain[3].cluster[id_Lx].charge);   
      // charge of cluster (truncated)
      hitLx        ->handle(event.chain[3].cluster[id_Lx].cofc_lab); 
      // center of charge in lab-coordinates (truncated)
      hiterrLx     ->handle(event.chain[3].cluster[id_Lx].cofc_err); 
      // center of charge error in ADC-coordinates (truncated)
      hitwidthLx   ->handle(event.chain[3].cluster[id_Lx].cofc_sig); 
      // center of charge sigma in ADC-coordinates (truncated)
      hitmaxvalueLx->handle(event.chain[3].cluster[id_Lx].max_value);
      // highest value in cluster
    
      clusterLy    ->handle(event.chain[2].cluster.size());          
      // number of cluster in chain
      multiLy      ->handle(event.chain[2].cluster_full[id_Ly].cnt); 
      // number of ADCs in cluster (not truncated)
      chargeLy     ->handle(event.chain[2].cluster[id_Ly].charge);   
      // charge of cluster (truncated)
      hitLy        ->handle(event.chain[2].cluster[id_Ly].cofc_lab); 
      // center of charge in lab-coordinates (truncated)
      hiterrLy     ->handle(event.chain[2].cluster[id_Ly].cofc_err); 
      // center of charge error in ADC-coordinates (truncated)
      hitwidthLy   ->handle(event.chain[2].cluster[id_Ly].cofc_sig); 
      // center of charge sigma in ADC-coordinates (truncated)
      hitmaxvalueLy->handle(event.chain[2].cluster[id_Ly].max_value);
      // highest value in cluster

      clusterMx    ->handle(event.chain[1].cluster.size());          
      // number of cluster in chain
      multiMx      ->handle(event.chain[1].cluster_full[id_Mx].cnt); 
      // number of ADCs in cluster (not truncated)
      chargeMx     ->handle(event.chain[1].cluster[id_Mx].charge);   
      // charge of cluster (truncated)
      hitMx        ->handle(event.chain[1].cluster[id_Mx].cofc_lab); 
      // center of charge in lab-coordinates (truncated)
      hiterrMx     ->handle(event.chain[1].cluster[id_Mx].cofc_err); 
      // center of charge error in ADC-coordinates (truncated)
      hitwidthMx   ->handle(event.chain[1].cluster[id_Mx].cofc_sig); 
      // center of charge sigma in ADC-coordinates (truncated)
      hitmaxvalueMx->handle(event.chain[1].cluster[id_Mx].max_value);
      // highest value in cluster

      clusterMy    ->handle(event.chain[0].cluster.size());          
      // number of cluster in chain
      multiMy      ->handle(event.chain[0].cluster_full[id_My].cnt); 
      // number of ADCs in cluster (not truncated)
      chargeMy     ->handle(event.chain[0].cluster[id_My].charge);   
      // charge of cluster (truncated)
      hitMy        ->handle(event.chain[0].cluster[id_My].cofc_lab); 
      // center of charge in lab-coordinates (truncated)
      hiterrMy     ->handle(event.chain[0].cluster[id_My].cofc_err); 
      // center of charge error in ADC-coordinates (truncated)
      hitwidthMy   ->handle(event.chain[0].cluster[id_My].cofc_sig); 
      // center of charge sigma in ADC-coordinates (truncated)
      hitmaxvalueMy->handle(event.chain[0].cluster[id_My].max_value);
      // highest value in cluster

    } // if (event.track.size()>0)
  
  else // return cluster nos. only
    {
      quality   -> handle(0);

      tracks    -> handle(event.track.size());
      clusterLx -> handle(event.chain[3].cluster.size());
      clusterLy -> handle(event.chain[2].cluster.size());
      clusterMx -> handle(event.chain[1].cluster.size());
      clusterMy -> handle(event.chain[0].cluster.size());
    }

  return 0;
}


// =======================================================================
// =======================================================================
// =======================================================================
// create 3 postscript files:
// track_???.ps : show signals and tracks
// signal_??.ps : show signals only
// chamber_??.ps: show MWPC M, L

// DIN-A4: 210 x 297 Millimeter
// Postscript Point 0:0 (x:y) is at corner left:bottom 
// Postscript: 1 Point = 1/72 inch,   1 inch = 25.4 mm
// Postscript DIN-A4: 595 x 842 Points


double ps(double x)
// x: position in mm on paper A4
// return: position in Points on paper A4
{
  double fak = 72/25.4;   // Points pro Millimeter
  return x * fak;
}


double labps(double x)
// x: position in mm in lab-system
// return: position in Points on paper A4
{
  double scale = 0.1;  // Massstab Labor:A4 1:10
  return ps(x)*scale;
}


// draw signals of chains for signal.ps
void draw_chain_signal(FILE *out, double *buf, int buf_len, char *label, 
		       double signal_scale, double tick_label_size)
{
  double tick_len =        ps(1.0);     // tick length on paper: 1 mm
  double tick_width =      ps(0.1);     // tick width on paper:  0.1 mm
  double signal_width =    labps(5.0);  // ADC signal width (lab): 5 mm

  fprintf(out, "black\n");
  fprintf(out, "/Helvetica findfont %f scalefont setfont\n", tick_label_size);
  for (int i=0; i<buf_len; i++)
  {
    // draw signals
    if (buf[i]>0)
      fprintf(out, "%f %f m %f %f l %f lw s\n",
                   i*signal_width, 0.0,
                   i*signal_width, buf[i]*signal_scale,
		   signal_width);
    // ticks and tick-labels (ADC)
    if (buf_len==KAOS_MWPC_ADCX) // x-chains
    {
      if ((i%20==0) || (i==239))
      {
        fprintf(out, "%f %f m %f %f l %f lw s  %%tick\n",
	            i*signal_width, 0.0, i*signal_width, -tick_len, tick_width);
        fprintf(out, "%f %f (%i) center  %%ticklabel\n",
	           i*signal_width, -tick_len-tick_label_size/1.3, i);
      }
    }
    else // y-chains
    {
      if ((i%10==0) || (i==69))
      {
        fprintf(out, "%f %f m %f %f l %f lw s  %%tick\n",
	            i*signal_width, 0.0, i*signal_width, -tick_len, tick_width);
        fprintf(out, "%f %f (%i) center  %%ticklabel\n",
	           i*signal_width, -tick_len-tick_label_size/1.3, i);
      }

    }
  }
  // ticks and tick-label (signal)
  for (int i=0; i<256; i++)
  {
    if (((i%50==0) || (i==255)) && (i!=250))
    {
      fprintf(out, "%f %f m %f %f l %f lw s  %%tick\n",
	           -signal_width/2, i*signal_scale,
		   -signal_width/2-tick_len, i*signal_scale,
		   tick_width);
      fprintf(out, "%f (%i) sw pop add neg %f m (%i) show\n",
                   signal_width/2+tick_len+tick_len/3, i, i*signal_scale-tick_label_size/3, i);

    }
  }
  // frame
  fprintf(out, "n %f %f m %f %f l %f %f l %f %f l c %f lw s\n",
               -signal_width/2, 0.0,
	        (buf_len-1)*signal_width+signal_width/2, 0.0,
	        (buf_len-1)*signal_width+signal_width/2, 255*signal_scale,
	        -signal_width/2, 255*signal_scale,
	        tick_width);
  // frame label
  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", tick_label_size);
  fprintf(out, "%f (%s) sw pop add neg %f (%s) sw pop sub m +90 rotate (%s) show -90 rotate\n",
	  signal_width/2+tick_len+tick_label_size/2, "255", 255*signal_scale, "ADC Counts", "ADC Counts");
  fprintf(out, "%f %f (%s) center\n",
               (buf_len-1)*signal_width/2, 255*signal_scale+tick_label_size/2, label);
  fprintf(out, "%f (%s) sw pop sub %f m (%s) show\n",
                (buf_len-1)*signal_width+signal_width/2, "ADC No",
		-tick_len-tick_label_size-tick_len-tick_label_size/1.3, "ADC No");
}

// draw signals of chains for chamber.ps
void draw_chain_chamber(FILE *out, double *buf, int buf_len, char *label, 
			double signal_scale, double tick_label_size)
{
  double tick_len =        ps(1.0);     // tick length on paper: 1 mm
  double tick_width =      ps(0.1);     // tick width on paper:  0.1 mm
  double signal_width =    labps(5.0);  // ADC signal width (lab): 5 mm

  fprintf(out, "black\n");
  fprintf(out, "/Helvetica findfont %f scalefont setfont\n", tick_label_size);
  for (int i=0; i<buf_len; i++)
  {
    // draw signals
    if (buf[i]>0)
      fprintf(out, "%f %f m %f %f l %f lw s\n",
                   i*signal_width, 0.0,
                   i*signal_width, buf[i]*signal_scale,
		   signal_width);
    // ticks and tick-labels (ADC)
    if (buf_len==KAOS_MWPC_ADCX) // x-chains
    {
      if ((i%20==0) || (i==239))
      {
        fprintf(out, "%f %f m %f %f l %f lw s  %%tick\n",
		i*signal_width, 255*signal_scale, i*signal_width, 255*signal_scale+tick_len, tick_width);
        fprintf(out, "%f %f (%i) center  %%ticklabel\n",
		i*signal_width, 255*signal_scale+tick_len+tick_label_size/3, i);
      }
      if ((i%10==0) && (i!=0))
      {
        fprintf(out, "%f %f m %f %f l %f lw s  %%tick\n",
		i*signal_width, 255*signal_scale, i*signal_width, 255*signal_scale+tick_len*0.7, tick_width);
      }
    }
    else // y-chains
    {
      if ((i%10==0) || (i==KAOS_MWPC_ADCY-1))
      {
        fprintf(out, "%f %f m %f %f l %f lw s  %%tick\n",
		i*signal_width, 255*signal_scale, i*signal_width, 255*signal_scale+tick_len, tick_width);
        fprintf(out, "%f %f (%i) center  %%ticklabel\n",
		i*signal_width, 255*signal_scale+tick_len+tick_label_size/3, i);
      }

    }
  }
  // ticks and tick-label (signal)
  for (int i=0; i<256; i++)
  {
    if (( (i%50==0) || (i==255)) && (i!=250) && (i!=0))
    {
      fprintf(out, "%f %f m %f %f l %f lw s  %%tick\n",
	           -signal_width/2, i*signal_scale,
		   -signal_width/2-tick_len, i*signal_scale,
		   tick_width);
      fprintf(out, "%f (%i) sw pop add neg %f m (%i) show\n",
                   signal_width/2+tick_len+tick_len/3, i, i*signal_scale-tick_label_size/3, i);

    }
  }
  // frame
  fprintf(out, "n %f %f m %f %f l %f %f l %f %f l c %f lw s\n",
               -signal_width/2, 0.0,
	        (buf_len-1)*signal_width+signal_width/2, 0.0,
	        (buf_len-1)*signal_width+signal_width/2, 255*signal_scale,
	        -signal_width/2, 255*signal_scale,
	        tick_width);
  // frame label
  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", tick_label_size);
  fprintf(out, "%f (%s) sw pop add neg %f (%s) sw pop sub m +90 rotate (%s) show -90 rotate\n",
	  signal_width/2+tick_len+tick_label_size/2, "255", 255*signal_scale, "ADC Counts", "ADC Counts");
  fprintf(out, "%f %f (%s) center\n",
	  (buf_len-1)*signal_width/2, 255*signal_scale+tick_len+tick_label_size*1.5, label);
}

// draw signals of chains for track.ps
void draw_chain(FILE *out, double *buf, int buf_len, char *ylabel, 
		double signal_scale, double tick_label_size)
{
  double tick_len =        ps(1.0);     // tick length on paper: 1 mm
  double tick_width =      ps(0.1);     // tick width on paper:  0.1 mm
  double signal_width =    labps(5.0);  // ADC signal width (lab): 5 mm

  fprintf(out, "black\n");
  fprintf(out, "/Helvetica findfont %f scalefont setfont\n", tick_label_size);
  // different handling for x- and y-chains
  if (buf_len==KAOS_MWPC_ADCX)  // draw x-chain
  {
    for (int i=0; i<buf_len; i++)
    {
      // draw signals
      if (buf[i]>0)
        fprintf(out, "%f %f m %f %f l %f lw s\n",
	             0.0, -i*signal_width,
		     -buf[i]*signal_scale, -i*signal_width,
		     signal_width);
      // ticks and tick-labels (ADC)
      if ((i%20==0) || (i==239))
      {
        fprintf(out, "%f %f m %f %f l %f lw s  %%tick\n",
	             0.0, -i*signal_width,
		     tick_len, -i*signal_width,
		     tick_width);
	fprintf(out, "%f %f m (%i) show  %%ticklabel\n",
	             tick_len, -i*signal_width-tick_label_size/3, i);
      }
    }
    // ticks and tick-label (signal)
    for (int i=0; i<256; i++)
    {
      if (((i%50==0) || (i==255)) && (i!=250))
      {
        fprintf(out, "%f %f m %f %f l %f lw s  %%tick\n",
	             -i*signal_scale, signal_width/2,
		     -i*signal_scale, signal_width/2+tick_len,
		     tick_width);
	fprintf(out, "%f %f m +90 rotate (%i) show -90 rotate\n",
	             -i*signal_scale+tick_label_size/3, signal_width/2+tick_len, i);
      }
    }
    // frame
    fprintf(out, "n %f %f m %f %f l %f %f l %f %f l c %f lw s\n",
                 0.0, signal_width/2,
		 -255*signal_scale, signal_width/2,
		 -255*signal_scale, -239*signal_width-signal_width/2,
		 0.0, -239*signal_width-signal_width/2,
		 tick_width);
    // frame label
    fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", tick_label_size);
    fprintf(out, "%f (%s) sw pop %f add m (%s) show\n", 
	    -255*signal_scale, "255" ,signal_width/2+tick_len+tick_label_size/5, "ADC Counts");
    fprintf(out, "%f %f m (%s) show\n", -255*signal_scale, 
	    -239*signal_width-signal_width/2-tick_label_size, ylabel);
    fprintf(out, "%f (%s) sw pop add (%s) sw pop neg %f add m +90 rotate (%s) show -90 rotate\n",
	    tick_len+tick_label_size, "20", "ADC No", signal_width/2 ,"ADC No");
  } // end x-chain
  
  else // draw y-chain
  {
    for (int i=0; i<buf_len; i++)
    {
      // draw signals
      if (buf[i]>0)
        fprintf(out, "%f %f m %f %f l %f lw s\n",
                     i*signal_width, 0.0,
	  	     i*signal_width, buf[i]*signal_scale,
		     signal_width);
      // ticks and tick-labels (ADC)
      if ((i%10==0) || (i==69))
      {
        fprintf(out, "%f %f m %f %f l %f lw s  %%tick\n",
                     i*signal_width, 0.0,
		     i*signal_width, -tick_len,
		     tick_width);
        fprintf(out, "%f %f (%i) center  %%ticklabel\n",
                     i*signal_width, -tick_len-tick_label_size/1.3, i);
      }
    }
    // ticks and tick-label (signal)
    for (int i=0; i<256; i++)
    {
      if (((i%50==0) || (i==255)) && (i!=250))
      {
        fprintf(out, "%f %f m %f %f l %f lw s  %%tick\n",
	             69*signal_width+signal_width/2, i*signal_scale,
		     69*signal_width+signal_width/2+tick_len, i*signal_scale,
		     tick_width);
        fprintf(out, "%f %f m (%i) show\n",
	             69*signal_width+signal_width/2+tick_len, i*signal_scale-tick_label_size/3, i);
      }
    }
    // frame
    fprintf(out, "n %f %f m %f %f l %f %f l %f %f l c %f lw s\n",
                 -signal_width/2, 0.0,
		 69*signal_width+signal_width/2, 0.0,
		 69*signal_width+signal_width/2, 255*signal_scale,
		 -signal_width/2, 255*signal_scale,
		 tick_width);
    // frame label
    fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", tick_label_size);
    fprintf(out, "%f (%s) sw pop add %f (%s) sw pop sub m +90 rotate (%s) show -90 rotate\n",
                  69*signal_width+signal_width/2+tick_len+tick_label_size, "255", 255*signal_scale, "ADC Counts", "ADC Counts");
    fprintf(out, "%f %f (%s) sw pop sub m +90 rotate (%s) show -90 rotate\n",
                 -tick_label_size/2, 255*signal_scale, ylabel, ylabel);
    fprintf(out, "%f (%s) sw pop add %f m (%s) show\n",
                  69*signal_width+signal_width/2, "69", -tick_len-tick_label_size/1.3, "ADC No");
  } // end y-chain

  fprintf(out, "/Helvetica findfont %f scalefont setfont\n", tick_label_size);
}


// color text in track-list for file tracks.ps
void ps_draw_qual(FILE *out, double x, double y, double q)
{
  if ((q>0) && q<1)
  {
    if (q>0.0001) fprintf(out, "%f %f (%.4f) center\n", x, y, q);
      else      fprintf(out, "%f %f (%.0E) center\n", x, y, q);
  }
  if (q==0)         fprintf(out, "gray5 %f %f (%i) center black\n", x, y, 0);
  if (q==1)         fprintf(out, "%f %f (%i) center\n", x, y, 1);
}


void ps_header(FILE *out, long event)
{
  fprintf(out, "%%!PS-Adobe-2.0\n");
  fprintf(out, "%%%%Creator: MWPCDetectors, by Patrick Achenbach (2010)\n");
  fprintf(out, "%%%% original version by Michael Boesz\n");
  fprintf(out, "%%%%Title: Event No. %li\n", event);
  fprintf(out, "%%%%Pages: 1\n");
  fprintf(out, "%%%%DocumentFonts: Helvetica, Helvetica-Bold\n");
  fprintf(out, "%%%%EndComments\n");
  fprintf(out, "/m     {moveto}      def       /l      {lineto}      def\n");
  fprintf(out, "/r     {rlineto}     def       /rm     {rmoveto}     def\n");
  fprintf(out, "/s     {stroke}      def       /c      {closepath}   def\n");
  fprintf(out, "/n     {newpath}     def       /gs     {gsave}       def\n");
  fprintf(out, "/gr    {grestore}    def       /sw     {stringwidth} def\n");
  fprintf(out, "/lw    {setlinewidth} def\n");
  fprintf(out, "/center {dup sw pop 2.0 div 3 index exch sub 2 index m show pop pop} def\n");
  fprintf(out, "/black   {0.0 0.0 0.0 setrgbcolor} def\n");
  fprintf(out, "/gray5   {0.5 0.5 0.5 setrgbcolor} def\n");
  fprintf(out, "/gray7   {0.7 0.7 0.7 setrgbcolor} def\n");
  fprintf(out, "/gray8   {0.8 0.8 0.8 setrgbcolor} def\n");
  fprintf(out, "/white   {1.0 1.0 1.0 setrgbcolor} def\n");
  fprintf(out, "/red     {1.0 0.0 0.0 setrgbcolor} def\n");
  fprintf(out, "/green   {0.0 1.0 0.0 setrgbcolor} def\n");
  fprintf(out, "/green2  {0.25 0.75 0.25 setrgbcolor} def\n");
  fprintf(out, "/blue    {0.0 0.0 1.0 setrgbcolor} def\n");
  fprintf(out, "/yellow  {0.9 1.0 0.0 setrgbcolor} def\n");
  fprintf(out, "/yellow2 {1.0 0.67 0.0 setrgbcolor} def\n");
  fprintf(out, "%%%%EndProlog\n");
}


void
MWPCDetectors::draw(char *trackDir, long eventno)
{
  // Paper A4 - do not edit
  double max_y=ps(297.0);          // paper height 297 mm
//  double max_x=ps(210.0);          // paper width 210 mm
  
  // these values can be edited
  double b_left = ps(10.0);                // border left 10 mm
  double b_top  = max_y - ps(10.0);        // border top 10 mm
  double dist_F_ychain = +ps(30.0);        // distance between Wall F and y-chains
  double arrow_len   = ps(3.0);            // arrow length
  double arrow_width = ps(2.0);            // arrow width
  double hit_width   = ps(0.2);            // line width for hit position 
  double track_width = ps(0.2);            // line width for tracks
  double track0_width= ps(0.4);            // line width for track 0
  double font1_size  = ps(2.0);            // font size  2 mm
  double font2_size  = ps(2.0);            // font size
  double font3_size  = ps(3.0);            // font size
  double ytrack_topoffet  = +ps(0.0);      // top offset for y-Tracks
  double ytrack_botoffset = -ps(10.0);     // bottom offset for y-Tacks
  double xtrack_leftoffset  = -ps(0.0);    // lest offset for x-Tracks
  double xtrack_rightoffset = +labps(lF/2)+arrow_len/2; // right offset for x-Tracks
  double signal_scale=ps(0.13);           // scale ADC signals

  // do not edit following values
  double baseMx_x= b_left + 255*signal_scale;
  double baseLx_x= baseMx_x + labps(zLM);
  double baseMx_y= b_top;
  double baseLx_y= baseMx_y - labps(xLM);
  double baseF_x = baseLx_x + labps(zLF);
  double baseF_y = baseLx_y - labps(xLF) - labps(lF/2);
  double baseG_x = baseLx_x + labps(zLG);
  double baseG_y = baseLx_y - labps(xLG) - labps(lG/2);
  double base0_x = (baseG_x + baseF_x)/2 + dist_F_ychain;
  double baseLy_x = base0_x + labps(yL);
  double baseMy_x = base0_x + labps(yM);
  double baseMy_y = baseMx_y - 255*signal_scale;
  double baseLy_y = baseMy_y - labps(zLM);

  double buffer[KAOS_MWPC_ADCX];

  // Filename for track_??.ps, signal_??.ps and chamber_??.ps
  char name_track[40];
  char name_sig[40];
  char name_cham[40];
  char fname_track[120];
  char fname_sig[120];
  char fname_cham[120];
  sprintf(name_track, "/track_%li.ps",  eventno);
  sprintf(name_sig,   "/signal_%li.ps", eventno);
  sprintf(name_cham,  "/chamber_%li.ps", eventno);
  strcpy(fname_track, trackDir);
  strcat(fname_track, name_track);
  strcpy(fname_sig, trackDir);
  strcat(fname_sig, name_sig);
  strcpy(fname_cham, trackDir);
  strcat(fname_cham, name_cham);
 
#ifdef DRAW_CHAMBER
  {
  // =============================================================
  // create ps-File chamber_??.ps 
  // =============================================================
  FILE *out_cham=fopen(fname_cham,"w");
  double signal_scale = ps(0.15);
  double ps_framesize_x= labps(25); // size of stealit-frame border 25 mm ??
  double ps_framesize_y = labps(25); // size of stealit-frame border 25 mm ??
  double ps_frame_M_x = ps(20);
  double ps_frame_M_y = max_y - ps(30) - 255*signal_scale;
  double ps_signal_Mx_x = ps_frame_M_x + ps_framesize_x;
  double ps_signal_Mx_y = ps_frame_M_y;
  double ps_signal_My_x = ps_signal_Mx_x + (KAOS_MWPC_ADCX-1)*labps(5.0) + ps_framesize_x;
  double ps_signal_My_y = ps_signal_Mx_y - ps_framesize_y;

  double ps_M_L_y = 255*signal_scale + 2*ps_framesize_y + (KAOS_MWPC_ADCY-1)*labps(5.0) + ps(20);
  double ps_frame_L_x = ps_frame_M_x; 
  double ps_frame_L_y = ps_frame_M_y - ps_M_L_y;
  double ps_signal_Lx_x = ps_signal_Mx_x;
  double ps_signal_Lx_y = ps_signal_Mx_y - ps_M_L_y;
  double ps_signal_Ly_x = ps_signal_My_x;
  double ps_signal_Ly_y = ps_signal_My_y - ps_M_L_y;

  int anode_wire = 30;

  ps_header(out_cham, eventno);
  //  fprintf(out_cham, "%%%%Page %i %i\n", 1, 1);
  fprintf(out_cham, "%%%%BoundingBox: 0 0 595 842\n");
  fprintf(out_cham, "black\n");
  fprintf(out_cham, "/Helvetica findfont %f scalefont setfont\n", font1_size);

  // ========================================
  // MWPC M
  // ========================================
  // === Data Mx
  fprintf(out_cham, "%% Data Mx\n");
  fprintf(out_cham, "gs\n");
  fprintf(out_cham, "%f %f translate\n", ps_signal_Mx_x, ps_signal_Mx_y);
  for (int i=0; i<KAOS_MWPC_ADCX; i++)
  {
    if (Mx[i]->ok())  buffer[i]=Mx[i]->scalar();
      else            buffer[i]=0.0;
  }
  draw_chain_chamber(out_cham, buffer, KAOS_MWPC_ADCX, "MWPC Mx - ADC Nr.", signal_scale, font1_size);
  fprintf(out_cham, "gr\n");
  // === Data My 
  fprintf(out_cham, "%% Data My\n");
  fprintf(out_cham, "gs\n");
  fprintf(out_cham, "%f %f translate\n", ps_signal_My_x, ps_signal_My_y);
  fprintf(out_cham, "-90 rotate\n");
  for (int i=0; i<KAOS_MWPC_ADCY; i++)
  {
    if (My[i]->ok())  buffer[i]=My[i]->scalar();
      else            buffer[i]=0.0;
  }
  draw_chain_chamber(out_cham, buffer, KAOS_MWPC_ADCY, "MWPC My - ADC Nr.", signal_scale, font1_size);
  fprintf(out_cham, "+90 rotate\n");
  fprintf(out_cham, "gr\n");

  // MWPC window M
  fprintf(out_cham, "gs\n");
  fprintf(out_cham, "%f %f translate gray8\n", ps_signal_Mx_x, ps_signal_My_y);
  fprintf(out_cham, "n 0 0 m %f %f l %f %f l %f %f l c fill s\n",
                     (KAOS_MWPC_ADCX-1)*labps(5.0), 0.0,
		     (KAOS_MWPC_ADCX-1)*labps(5.0), -(KAOS_MWPC_ADCY-1)*labps(5.0),
		     0.0, -(KAOS_MWPC_ADCY-1)*labps(5.0));
  fprintf(out_cham, "black gr\n");

  // Stealite frame with some anode wires
  fprintf(out_cham, "%%Stealite frame\n");
  fprintf(out_cham, "gs\n");
  fprintf(out_cham, "%f %f translate\n", ps_frame_M_x, ps_frame_M_y);
  fprintf(out_cham, "gs green2 0.1 lw\n");
  fprintf(out_cham, "n 0 0 m %f %f l %f %f l %f %f l c\n",
	  (KAOS_MWPC_ADCX-1)*labps(5.0)+2*ps_framesize_x, 0.0,
	  (KAOS_MWPC_ADCX-1)*labps(5.0)+2*ps_framesize_x, 
	  -(KAOS_MWPC_ADCY-1)*labps(5.0)-2*ps_framesize_y,
	  0.0, -(KAOS_MWPC_ADCY-1)*labps(5.0)-2*ps_framesize_y);

  fprintf(out_cham, "gs s gr clip\n");
  // Anode wires
  for (int i=0; i<anode_wire; i++)
  {
    fprintf(out_cham, "%f %f m -150 -150 r s %%Anode wire\n",
	    (((KAOS_MWPC_ADCX-1)*labps(5.0)+2*ps_framesize_x)+(KAOS_MWPC_ADCY-1)*labps(5.0)+2*ps_framesize_y)*i
		      /(anode_wire-1), 0.0 );
  }
  fprintf(out_cham, "gr\n");
  fprintf(out_cham, "gr\n");

  // === Hitposition Mx 
  fprintf(out_cham, "%% Hitposition Mx\n");
  fprintf(out_cham, "gs\n");
  fprintf(out_cham, "%f %f translate\n", ps_signal_Mx_x, ps_signal_Mx_y);
  fprintf(out_cham, "blue\n");

  for (unsigned int i=0; i<event.chain[1].cluster.size(); i++)
  {
    double hitMx = labps(event.chain[1].cluster[i].cofc * 5);
    double y = -(KAOS_MWPC_ADCY-1)*labps(5.0)-2*ps_framesize_y-ps(2.0);
    fprintf(out_cham, "%f %f m %f %f l %f setlinewidth s\n",
                 hitMx, 255*signal_scale,
		 hitMx, y,
		 hit_width);
    fprintf(out_cham, "%f %f %f 0 360 arc fill s\n", hitMx, y, ps(1.8));
    fprintf(out_cham, "white\n");
    fprintf(out_cham, "%f %f (%i) center\n", hitMx, y-font1_size/3, i);
    fprintf(out_cham, "blue\n");
  }
  fprintf(out_cham, "gr\n");

  // === Hitposition My 
  fprintf(out_cham, "%% Hitposition My\n");
  fprintf(out_cham, "gs\n");
  fprintf(out_cham, "%f %f translate\n", ps_signal_My_x, ps_signal_My_y);
  fprintf(out_cham, "blue\n");

  for (unsigned int i=0; i<event.chain[0].cluster.size(); i++)
  {
    double hitMy = labps(event.chain[0].cluster[i].cofc * 5);
    double x = -(KAOS_MWPC_ADCX-1)*labps(5.0)-2*ps_framesize_y-ps(2.0);
    fprintf(out_cham, "%f %f m %f %f l %f setlinewidth s\n",
                 255*signal_scale, -hitMy,
		 x, -hitMy,
		 hit_width);
    fprintf(out_cham, "%f %f %f 0 360 arc fill s\n", x, -hitMy, ps(1.8));
    fprintf(out_cham, "white\n");
    fprintf(out_cham, "%f %f (%i) center\n", x, -hitMy-font1_size/3, i);
    fprintf(out_cham, "blue\n");
  }
  fprintf(out_cham, "gr\n");


  // ========================================
  // MWPC L 
  // ========================================
  // === Data Lx
  fprintf(out_cham, "%% Data Lx\n");
  fprintf(out_cham, "gs\n");
  fprintf(out_cham, "%f %f translate\n", ps_signal_Lx_x, ps_signal_Lx_y);
  for (int i=0; i<KAOS_MWPC_ADCX; i++)
  {
    if (Lx[i]->ok())  buffer[i]=Lx[i]->scalar();
      else            buffer[i]=0.0;
  }
  draw_chain_chamber(out_cham, buffer, KAOS_MWPC_ADCX, "MWPC Lx - ADC Nr.", signal_scale, font1_size);
  fprintf(out_cham, "gr\n");
  // === Data Ly 
  fprintf(out_cham, "%% Data Ly\n");
  fprintf(out_cham, "gs\n");
  fprintf(out_cham, "%f %f translate\n", ps_signal_Ly_x, ps_signal_Ly_y);
  fprintf(out_cham, "-90 rotate\n");
  for (int i=0; i<KAOS_MWPC_ADCY; i++)
  {
    if (Ly[i]->ok())  buffer[i]=Ly[i]->scalar();
      else            buffer[i]=0.0;
  }
  draw_chain_chamber(out_cham, buffer, KAOS_MWPC_ADCY, "MWPC Ly - ADC Nr.", signal_scale, font1_size);
  fprintf(out_cham, "+90 rotate\n");
  fprintf(out_cham, "gr\n");

  // MWPC window L 
  fprintf(out_cham, "gs\n");
  fprintf(out_cham, "%f %f translate gray8\n", ps_signal_Lx_x, ps_signal_Ly_y);
  fprintf(out_cham, "n 0 0 m %f %f l %f %f l %f %f l c fill s\n",
                     (KAOS_MWPC_ADCX-1)*labps(5.0), 0.0,
		     (KAOS_MWPC_ADCX-1)*labps(5.0), 
	  -(KAOS_MWPC_ADCY-1)*labps(5.0), 0.0, -(KAOS_MWPC_ADCY-1)*labps(5.0));
  fprintf(out_cham, "black gr\n");

  // Stealite frame with some anode wires
  fprintf(out_cham, "%%Stealite frame\n");
  fprintf(out_cham, "gs\n");
  fprintf(out_cham, "%f %f translate\n", ps_frame_L_x, ps_frame_L_y);
  fprintf(out_cham, "gs green2 0.1 lw\n");
  fprintf(out_cham, "n 0 0 m %f %f l %f %f l %f %f l c\n",
	  (KAOS_MWPC_ADCX-1)*labps(5.0)+2*ps_framesize_x, 0.0,
	  (KAOS_MWPC_ADCX-1)*labps(5.0)+2*ps_framesize_x, 
	  -(KAOS_MWPC_ADCY-1)*labps(5.0)-2*ps_framesize_y,
	  0.0, -(KAOS_MWPC_ADCY-1)*labps(5.0)-2*ps_framesize_y);

  fprintf(out_cham, "gs s gr clip\n");
  // Anode wires
  for (int i=0; i<anode_wire; i++)
  {
    fprintf(out_cham, "%f %f m -150 -150 r s %%Anode wire\n",
                      (((KAOS_MWPC_ADCX-1)*labps(5.0)+2*ps_framesize_x)+(KAOS_MWPC_ADCY-1)*labps(5.0)+2*ps_framesize_y)*i
		      /(anode_wire-1), 0.0 );
  }
  fprintf(out_cham, "gr\n");
  fprintf(out_cham, "gr\n");

  // === Hitposition Lx 
  fprintf(out_cham, "%% Hitposition Lx\n");
  fprintf(out_cham, "gs\n");
  fprintf(out_cham, "%f %f translate\n", ps_signal_Lx_x, ps_signal_Lx_y);
  fprintf(out_cham, "blue\n");
  for (unsigned int i=0; i<event.chain[3].cluster.size(); i++)
  {
    double hitLx = labps(event.chain[3].cluster[i].cofc * 5);
    double y = -(KAOS_MWPC_ADCY-1)*labps(5.0)-2*ps_framesize_y-ps(2.0);
    fprintf(out_cham, "%f %f m %f %f l %f setlinewidth s\n",
                 hitLx, 255*signal_scale,
		 hitLx, y,
		 hit_width);
    fprintf(out_cham, "%f %f %f 0 360 arc fill s\n", hitLx, y, ps(1.8));
    fprintf(out_cham, "white\n");
    fprintf(out_cham, "%f %f (%i) center\n", hitLx, y-font1_size/3, i);
    fprintf(out_cham, "blue\n");
  }
  fprintf(out_cham, "gr\n");

  // === Hitposition Ly 
  fprintf(out_cham, "%% Hitposition Ly\n");
  fprintf(out_cham, "gs\n");
  fprintf(out_cham, "%f %f translate\n", ps_signal_Ly_x, ps_signal_Ly_y);
  fprintf(out_cham, "blue\n");
  for (unsigned int i=0; i<event.chain[2].cluster.size(); i++)
  {
    double hitLy = labps(event.chain[2].cluster[i].cofc * 5);
    double x = -(KAOS_MWPC_ADCX-1)*labps(5.0)-2*ps_framesize_y-ps(2.0);
    fprintf(out_cham, "%f %f m %f %f l %f setlinewidth s\n",
                 255*signal_scale, -hitLy,
		 x, -hitLy,
		 hit_width);
    fprintf(out_cham, "%f %f %f 0 360 arc fill s\n", x, -hitLy, ps(1.8));
    fprintf(out_cham, "white\n");
    fprintf(out_cham, "%f %f (%i) center\n", x, -hitLy-font1_size/3, i);
    fprintf(out_cham, "blue\n");
  }
  fprintf(out_cham, "gr\n");


  fprintf(out_cham, "gr\n");
  fprintf(out_cham, "showpage\n");
  fprintf(out_cham, "%%%%Trailer\n");
  fprintf(out_cham, "%%%%EOF\n");

  fclose(out_cham);
  }
#endif

#ifdef DRAW_SIGNALS
  {
  // =============================================================
  // create ps-file for signals only
  // =============================================================
  FILE *out_sig=fopen(fname_sig,"w");
  double signal_scale = ps(0.15);
  double ps_signal_Mx_x = ps(10);
  double ps_signal_My_x = ps_signal_Mx_x+255*labps(5.0)+ps(10);
  double ps_signal_Lx_x = ps_signal_Mx_x;
  double ps_signal_Ly_x = ps_signal_My_x;
  double ps_signal_M_y  = max_y-ps(30)-255*signal_scale;
  double ps_signal_L_y  = ps_signal_M_y - 255*signal_scale-ps(10);
  
  ps_header(out_sig, eventno);
  
  // === Data Mx 
  fprintf(out_sig, "%% Data Mx\n");
  fprintf(out_sig, "gs\n");
  fprintf(out_sig, "%f %f translate\n", ps_signal_Mx_x, ps_signal_M_y);
  for (int i=0; i<KAOS_MWPC_ADCX; i++)
  {
    if (Mx[i]->ok())  buffer[i]=Mx[i]->scalar();
      else            buffer[i]=0.0;
  }
  draw_chain_signal(out_sig, buffer, KAOS_MWPC_ADCX, "MWPC Mx", 
		    signal_scale, font1_size);
  fprintf(out_sig, "gr\n");
  // === Data My 
  fprintf(out_sig, "%% Data My\n");
  fprintf(out_sig, "gs\n");
  fprintf(out_sig, "%f %f translate\n", ps_signal_My_x, ps_signal_M_y);
  for (int i=0; i<KAOS_MWPC_ADCY; i++)
  {
    if (My[i]->ok())  buffer[i]=My[i]->scalar();
      else            buffer[i]=0.0;
  }
  draw_chain_signal(out_sig, buffer, KAOS_MWPC_ADCY, "MWPC My", signal_scale, font1_size);
  fprintf(out_sig, "gr\n");


  // === Data Lx 
  fprintf(out_sig, "%% Data Lx\n");
  fprintf(out_sig, "gs\n");
  fprintf(out_sig, "%f %f translate\n", ps_signal_Lx_x, ps_signal_L_y);
  for (int i=0; i<KAOS_MWPC_ADCX; i++)
  {
    if (Lx[i]->ok())  buffer[i]=Lx[i]->scalar();
      else            buffer[i]=0.0;
  }
  draw_chain_signal(out_sig, buffer, KAOS_MWPC_ADCX, "MWPC Lx", signal_scale, font1_size);
  fprintf(out_sig, "gr\n");
  // === Data Ly 
  fprintf(out_sig, "%% Data Ly\n");
  fprintf(out_sig, "gs\n");
  fprintf(out_sig, "%f %f translate\n", ps_signal_Ly_x, ps_signal_L_y);
  for (int i=0; i<KAOS_MWPC_ADCY; i++)
  {
    if (Ly[i]->ok())  buffer[i]=Ly[i]->scalar();
      else            buffer[i]=0.0;
  }
  draw_chain_signal(out_sig, buffer, KAOS_MWPC_ADCY, "MWPC Ly", signal_scale, font1_size);
  fprintf(out_sig, "gr\n");
  
  fprintf(out_sig, "showpage\n");
  fprintf(out_sig, "%%%%Trailer\n");
  fclose(out_sig);
  }
#endif

  if (use_tof == 1) // WALL F
{
  // =============================================================
  // create ps-File for tracks
  // =============================================================
  FILE *out=fopen(fname_track,"w");
  ps_header(out, eventno);
  fprintf(out, "%%%%Page %i %i\n", 1, 1);
  fprintf(out, "white\n");
  fprintf(out, "n 0 0 m 595 0 l 595 842 l 0 842 l c fill\n");

  fprintf(out, "black\n");
  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", ps(6.0));
  fprintf(out, "%f %f m (Kaos Event Display) show\n", 180., b_top);
  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", ps(4.0));
  fprintf(out, "%f %f m (MWPCs and TOF Walls) show\n", 200., b_top-20);

  fprintf(out, "/Helvetica findfont %f scalefont setfont\n", font1_size);

  // =====================================================
  // x-chains
  // =====================================================

  // === Data Lx 
  fprintf(out, "%% Data Lx\n");
  fprintf(out, "gs\n");
  fprintf(out, "%f %f translate\n", baseLx_x, baseLx_y);
  for (int i=0; i<KAOS_MWPC_ADCX; i++)
    {
      if (Lx[i]->ok())  buffer[i]=Lx[i]->scalar();
      else            buffer[i]=0.0;
    }
  draw_chain(out, buffer, KAOS_MWPC_ADCX, "MWPC Lx", signal_scale, font1_size);
  fprintf(out, "gr\n");

  // === Data Mx
  fprintf(out, "%% Data Mx\n");
  fprintf(out, "gs\n");
  fprintf(out, "%f %f translate\n", baseMx_x, baseMx_y);
  for (int i=0; i<KAOS_MWPC_ADCX; i++)
    {
      if (Mx[i]->ok())  buffer[i]=Mx[i]->scalar();
      else            buffer[i]=0.0;
    }
  draw_chain(out, buffer, KAOS_MWPC_ADCX, "MWPC Mx", signal_scale, font1_size);
  fprintf(out, "gr\n");

  // === Data Wall F
  fprintf(out, "%%Data Wall F\n");
  fprintf(out, "gs\n");
  fprintf(out, "%f %f translate\n", baseF_x, baseF_y);
  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", font1_size);
  fprintf(out, "%f %f (%s) center\n", 0.0, 0.0+labps(lF/2)+font1_size/4, 
	  "TOF Wall F");
  fprintf(out, "/Helvetica findfont %f scalefont setfont\n", font1_size);
  fprintf(out, "gray5\n");
  // draw line through the center of Wall F
  fprintf(out, "%f %f m %f %f l %f lw s\n", 0.0, labps(lF/2), 
	  0.0, -labps(KAOS_WALL_PADDLES*lF-lF/2), ps(0.1));
  
  fprintf(out, "black\n");
  for (int i=0; i<KAOS_WALL_PADDLES; i++)
    {
      double alpha = -37.0;     // paddle angle
      double b = labps(20.0);   // paddle width (lab): 20 mm
      double l = labps(37.0);   // paddle length (lab): 37 mm
      // lF: distance between two paddles
      double x0 = -b/2;
      double y0 = +l/2;
      double x1 = +b/2;
      double y1 = +l/2;

      double x2 = -x0;
      double y2 = -y0;
      double x3 = -x1;
      double y3 = -y1;
      fprintf(out, "gray5\n");
      fprintf(out, "n %f %f %f 0 360 arc s c\n", 0.0, 0.0, labps(lF/2));
      fprintf(out, "black\n");
      if (event.tof[0][i]==0 || event.tof[1][i]==0)
	fprintf(out, "%f rotate n %f %f m %f %f l %f %f l %f %f l c %f lw s %f rotate\n",
		alpha, x0, y0, x1, y1, x2, y2, x3, y3, ps(0.1), -alpha);
      else
	fprintf(out, "%f rotate n %f %f m %f %f l %f %f l %f %f l c fill %f lw s %f rotate\n",
		alpha, x0, y0, x1, y1, x2, y2, x3, y3, ps(0.1), -alpha);
      if ((i%5==0) || (i==29))
	fprintf(out, "%f rotate %f %f (%i) center %f rotate\n",
		(alpha+90), 0.0, (b-font1_size)/2-b/2+font1_size/7, i, -(alpha+90));
      fprintf(out, "%f %f translate\n", 0.0, -labps(lF));
    }
  fprintf(out, "gr\n");

  // === Data Wall G
  fprintf(out, "%%Data Wall G\n");
  fprintf(out, "gs\n");
  fprintf(out, "%f %f translate\n", baseG_x, baseG_y);
  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", font1_size);
  fprintf(out, "%f %f (%s) center\n", 0.0, 0.0+labps(lG/2)+font1_size/4, 
	  "TOF Wall G");
  fprintf(out, "/Helvetica findfont %f scalefont setfont\n", font1_size);
  fprintf(out, "gray5\n");
  // draw line through the center of Wall G
  fprintf(out, "%f %f m %f %f l %f lw s\n", 0.0, labps(lG/2), 
	  0.0, -labps(KAOS_WALL_PADDLES*lG-lG/2), ps(0.1));
  
  fprintf(out, "black\n");
  for (int i=0; i<KAOS_WALL_PADDLES; i++)
    {
      double alpha = 0.0;       // paddle angle
      double b = labps(20.0);   // paddle width (lab): 20 mm
      double l = labps(lG);     // distance between two paddles
      double x0 = -b/2;
      double y0 = +l/2;
      double x1 = +b/2;
      double y1 = +l/2;

      double x2 = -x0;
      double y2 = -y0;
      double x3 = -x1;
      double y3 = -y1;
      //    fprintf(out, "gray5\n");
      //    fprintf(out, "n %f %f %f 0 360 arc s c\n", 0.0, 0.0, labps(lG/2));
      fprintf(out, "black\n");
      if (event.tof[2][i]==0 || event.tof[3][i]==0)
	fprintf(out, "%f rotate n %f %f m %f %f l %f %f l %f %f l c %f lw s %f rotate\n",
		alpha, x0, y0, x1, y1, x2, y2, x3, y3, ps(0.1), -alpha);
      else
	fprintf(out, "%f rotate n %f %f m %f %f l %f %f l %f %f l c fill %f lw s %f rotate\n",
		alpha, x0, y0, x1, y1, x2, y2, x3, y3, ps(0.1), -alpha);
      if ((i%5==0) || (i==29))
	fprintf(out, "%f rotate %f %f (%i) center %f rotate\n",
		(alpha+90), 0.0, (b-font1_size)/2-b/2+font1_size/7, i, -(alpha+90));
      fprintf(out, "%f %f translate\n", 0.0, -labps(lG));
    }
  fprintf(out, "gr\n");


  // === Tracks x
  fprintf(out, "%% Tracks x\n");
  fprintf(out, "gs\n");
  fprintf(out, "%f %f translate\n", baseLx_x, baseLx_y);
  for (unsigned int i=0; i<event.chain[1].cluster.size(); i++)
    {
      for (unsigned int j=0; j<event.chain[3].cluster.size(); j++)
	{
	  int cnt_track=0;
	  int found_track0 = 0;
	  double qual = 0;
	  std::stringstream str;
	  for (std::list<t_track>::iterator it=event.track.begin(); it!=event.track.end(); ++it)
	    {
	      if ((int (i)==it->id[1]) && (int (j)==it->id[3]))
		{
		  str << cnt_track << ",";
		  if (cnt_track==0) found_track0=1;
		  qual = qual + it->quality;
		}
	      cnt_track++;
	    }
	  if (str.str().length()>0)
	    {
	      double theta = get_theta(event.chain[1].cluster[i].cofc_lab,
				       event.chain[3].cluster[j].cofc_lab);
	      double hitLx = labps(event.chain[3].cluster[j].cofc_lab);
	      double hitMx = labps(event.chain[1].cluster[i].cofc_lab);

	      // Gerade durch hit-Positionen bestimmen y=mx+b
	      double m = (hitLx - hitMx) / (0.0 - labps(zLM));
	      double b = -hitLx - m * 0.0;
	      double x0 = -labps(zLM)+xtrack_leftoffset;
	      double y0 = m * x0 + b;
	      double x1 = +labps(zLG)+xtrack_rightoffset;
	      double y1 = m * x1 + b;
	
	      if (qual>0)
		{
		  if (found_track0==1) fprintf(out, "black %f lw\n", track0_width);
		  else fprintf(out, "yellow2 %f lw\n", track_width);
		}
	      else fprintf(out, "gray7 %f lw\n", track_width);

	      fprintf(out, "%f %f m %f %f l s\n", x0, y0, x1, y1);
	      // arrow
	      fprintf(out, "n %f %f m %f rotate %f %f r %f %f r %f %f r %f rotate c fill s\n",
		      x1, y1, -theta, -arrow_len, +arrow_width/2, 0.0, 
		      -arrow_width, +arrow_len, +arrow_width/2, + theta);
	      // tracknumbers
	      fprintf(out, "black\n");
	      fprintf(out, "%f %f m (%s) show\n", 
		      x1+ps(0.5), y1-font1_size/3, str.str().erase(str.str().size()-1).c_str());
	    }
	}
    }


  // === Hitposition Lx 
  fprintf(out, "%% Hitposition Lx\n");
  fprintf(out, "blue\n");
  for (unsigned int i=0; i<event.chain[3].cluster.size(); i++)
    {
      double hitLx = labps(event.chain[3].cluster[i].cofc_lab);
      fprintf(out, "%f %f m %f %f l %f setlinewidth s\n",
	      +ps(2.0-1.8), -hitLx,
	      -255*signal_scale, -hitLx,
	      hit_width);
      fprintf(out, "%f %f %f 0 360 arc fill s\n", ps(2.0), -hitLx, ps(1.8));
      fprintf(out, "white\n");
      fprintf(out, "%f %f m (%i) show\n", ps(1.5), -hitLx-font1_size/3, i);
      fprintf(out, "blue\n");
    }
  // === Hitpostion Mx
  fprintf(out, "%% Hitposition Mx\n");
  for (unsigned int i=0; i<event.chain[1].cluster.size(); i++)
    {
      double hitMx = labps(event.chain[1].cluster[i].cofc_lab);
      fprintf(out, "%f %f m %f %f l %f setlinewidth s\n",
	      -labps(zLM)+ps(2.0-1.8), -hitMx,
	      -labps(zLM)-255*signal_scale, -hitMx,
	      hit_width);
      fprintf(out, "%f %f %f 0 360 arc fill s\n", -labps(zLM)+ps(2.0), -hitMx, ps(1.8));
      fprintf(out, "white\n");
      fprintf(out, "%f %f m (%i) show\n", -labps(zLM)+ps(1.5), -hitMx-font1_size/3, i);
      fprintf(out, "blue\n");
    }
  fprintf(out, "gr\n");


  // =====================================================
  // y-chains
  // =====================================================

  // === Data My
  fprintf(out, "%% Data My\n");
  fprintf(out, "gs\n");
  fprintf(out, "%f %f translate\n", baseMy_x, baseMy_y);
  for (int i=0; i<KAOS_MWPC_ADCY; i++)
    {
      if (My[i]->ok())  buffer[i]=My[i]->scalar();
      else            buffer[i]=0.0;
    }
  draw_chain(out, buffer, KAOS_MWPC_ADCY, "MWPC My", signal_scale, font1_size);
  fprintf(out, "gr\n");

  // Data Ly
  fprintf(out, "%% Data Ly\n");
  fprintf(out, "gs\n");
  fprintf(out, "%f %f translate\n", baseLy_x, baseLy_y);
  for (int i=0; i<KAOS_MWPC_ADCY; i++)
    {
      if (Ly[i]->ok())  buffer[i]=Ly[i]->scalar();
      else            buffer[i]=0.0;
    }
  draw_chain(out, buffer, KAOS_MWPC_ADCY, "MWPC Ly", signal_scale, font1_size);
  fprintf(out, "gr\n");

  // === Tracks y
  fprintf(out, "%% Tracks y\n");
  fprintf(out, "gs\n");
  fprintf(out, "%f %f translate\n", base0_x+labps(34.5*5), baseLy_y);
  for (unsigned int i=0; i<event.chain[0].cluster.size(); i++)
    {
      for (unsigned int j=0; j<event.chain[2].cluster.size(); j++)
	{
	  int cnt_track=0;
	  int found_track0 = 0;
	  double qual = 0;
	  std::stringstream str;
	  for (std::list<t_track>::iterator it=event.track.begin(); it!=event.track.end(); ++it)
	    {
	      if ((int (i)==it->id[0]) && (int (j)==it->id[2]))
		{
		  str << cnt_track << ",";
		  if (cnt_track==0) found_track0=1;
		  qual = qual + it->quality;
		}
	      cnt_track++;
	    }
	  if (str.str().length()>0)
	    {
	      // Gerade durch hit-Positionen bestimmen y=mx+b
	      double phi = get_phi(event.chain[0].cluster[i].cofc_lab,
				   event.chain[2].cluster[j].cofc_lab);
	
	      double hitMy = event.chain[0].cluster[i].cofc_lab;
	      double hitLy = event.chain[2].cluster[j].cofc_lab;
	      double m = (0.0-labps(zLM)) / (-labps(hitLy) - (-labps(hitMy)));
	      double b = 0.0 - m * (-labps(hitLy));
	      double y0 = labps(zLM)+ytrack_topoffet;      // Anfangspunkt y0 festlegen
	      double x0 = (y0-b)/m;                        // Berechnung x0 zu y0
	      double y1 = ytrack_botoffset;                // Endpunkt y1 festlegen
	      double x1 = (y1-b)/m;                        // Berechnung x1 zu y1
	
	      if (qual>0)
		{
		  if (found_track0==1) fprintf(out, "black %f lw\n", track0_width);
		  else fprintf(out, "yellow2 %f lw\n", track_width);
		}
	      else fprintf(out, "gray7 %f\n", track_width);
	      fprintf(out, "%f %f m %f %f l s\n", x0, y0, x1, y1);
	      // arrow
	      fprintf(out, "n %f %f m %f rotate %f %f r %f %f r %f %f r %f rotate c fill s\n",
		      x1, y1, -phi, +arrow_width/2, +arrow_len, -arrow_width, 0.0, +arrow_width/2, -arrow_len, phi);
	      // print track numbers
	      fprintf(out, "black\n");
	      fprintf(out, "%f %f m -90 rotate (%s) show +90 rotate\n",
		      x1-font1_size/3, y1-ps(0.5), str.str().erase(str.str().size()-1).c_str());
	    }
	}
    }

  // === Hitpostition My
  fprintf(out, "%% Hitposition My\n");
  fprintf(out, "blue\n");
  for (unsigned int i=0; i<event.chain[0].cluster.size(); i++)
    {
      double hitMy = labps(event.chain[0].cluster[i].cofc_lab);
      fprintf(out, "%f %f m %f %f l %f setlinewidth s\n",
	      -hitMy, -ps(2.0-1.8)+labps(zLM),
	      -hitMy, +255*signal_scale+labps(zLM),
	      hit_width);
      fprintf(out, "%f %f %f 0 360 arc fill s\n", -hitMy, -ps(2.0)+labps(zLM), ps(1.8));
      fprintf(out, "white\n");
      fprintf(out, "%f %f m (%i) show\n", 
	      -hitMy-font1_size/4 , labps(zLM)-ps(1.5)-font1_size/1.7, i);
      fprintf(out, "blue\n");
    }
  // === Hitpostition Ly
  fprintf(out, "%% Hitposition Ly\n");
  fprintf(out, "blue\n");
  for (unsigned int i=0; i<event.chain[2].cluster.size(); i++)
    {
      double hitLy = labps(event.chain[2].cluster[i].cofc_lab);
      fprintf(out, "%f %f m %f %f l %f setlinewidth s\n",
	      -hitLy, -ps(2.0-1.8),
	      -hitLy, +255*signal_scale,
	      hit_width);
      fprintf(out, "%f %f %f 0 360 arc fill s\n", -hitLy, -ps(2.0), ps(1.8));
      fprintf(out, "white\n");
      fprintf(out, "%f %f m (%i) show\n", -hitLy-font1_size/4 , -ps(1.5)-font1_size/1.7, i);
      fprintf(out, "blue\n");
    }
  fprintf(out, "gr\n");


  // Text ...
  double col_left, row_top, col, row, x0, y0, x1, y1, x2;
  double colmul = 3.1;
  // Detector Position
  col = ps(15.0);
  if (baseLy_x > baseMy_x) col_left=baseLy_x + col;
  else col_left=baseMy_x + col;
  row_top = baseLy_y -ps(60.0);
  row = font2_size*1.1;
  x0 = col_left;
  y0 = row_top;
  x1 = col_left + col/1.5;
  x2 = col_left + 2*col/2;
  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", font2_size);
  fprintf(out, "%f %f m (Detector positions) show\n", x0, y0);
  fprintf(out, "/Helvetica findfont %f scalefont setfont\n", font2_size);
  fprintf(out, "%f %f m %f %f l %f lw s\n", 
	  col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));
  y0 = y0 - row; fprintf(out, "%f %f m (MWPC M:) show %f %f m (xLM) show %f %f m (%.1f) show\n", 
			 x0, y0, x1, y0, x2, y0, xLM);
  y0 = y0 - row; fprintf(out, "%f %f m (MWPC M:) show %f %f m (zLM) show %f %f m (%.1f) show\n", 
			 x0, y0, x1, y0, x2, y0, zLM);
  y0 = y0 - row; fprintf(out, "%f %f m (MWPC M:) show %f %f m (yM)  show %f %f m (%.1f) show\n", 
			 x0, y0, x1, y0, x2, y0, yM);
  y0 = y0 - row; fprintf(out, "%f %f m (MWPC L:) show %f %f m (yL)  show %f %f m (%.1f) show\n", 
			 x0, y0, x1, y0, x2, y0, yL);
  y0 = y0 - row; fprintf(out, "%f %f m (WALL F:) show %f %f m (xLF) show %f %f m (%.1f) show\n", 
			 x0, y0, x1, y0, x2, y0, xLF);
  y0 = y0 - row; fprintf(out, "%f %f m (WALL F:) show %f %f m (zLF) show %f %f m (%.1f) show\n", 
			 x0, y0, x1, y0, x2, y0, zLF);
  y0 = y0 - row; fprintf(out, "%f %f m (WALL G:) show %f %f m (xLG) show %f %f m (%.1f) show\n", 
			 x0, y0, x1, y0, x2, y0, xLG);
  y0 = y0 - row; fprintf(out, "%f %f m (WALL G:) show %f %f m (zLG) show %f %f m (%.1f) show\n", 
			 x0, y0, x1, y0, x2, y0, zLG);
  
  // Analyse parameter: cluster detection
  col = ps(15.0);
  x0 = col_left;
  x1 = x0 + col*2;
  x2 = x1 + col*3;
  y0 = y0 - 2*row;
  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", font2_size);
  fprintf(out, "%f %f m (Analyse parameters: cluster detection) show\n", x0, y0);
  fprintf(out, "/Helvetica findfont %f scalefont setfont\n", font2_size);
  fprintf(out, "%f %f m %f %f l %f lw s\n", 
	  col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));
  y0 = y0 - row; fprintf(out, "%f %f m (ADC threshold) show %f %f m (%.0f) show\n", 
			 x0, y0, x1, y0, event.chain[0].thresh[0]);
  y0 = y0 - row; fprintf(out, "%f %f m (cluster separation Mx) show %f %f m (%i) show\n", 
			 x0, y0, x1, y0, event.chain[1].sep);
  y0 = y0 - row; fprintf(out, "%f %f m (cluster separation My) show %f %f m (%i) show\n", 
			 x0, y0, x1, y0, event.chain[0].sep);
  y0 = y0 - row; fprintf(out, "%f %f m (cluster separation Lx) show %f %f m (%i) show\n", 
			 x0, y0, x1, y0, event.chain[3].sep);
  y0 = y0 - row; fprintf(out, "%f %f m (cluster separation Ly) show %f %f m (%i) show\n", 
			 x0, y0, x1, y0, event.chain[2].sep);
  y0 = y0 - row; fprintf(out, "%f %f m (cluster delete Mx) show %f %f m (%i) show\n", 
			 x0, y0, x1, y0, event.chain[1].del);
  y0 = y0 - row; fprintf(out, "%f %f m (cluster delete My) show %f %f m (%i) show\n", 
			 x0, y0, x1, y0, event.chain[0].del);
  y0 = y0 - row; fprintf(out, "%f %f m (cluster delete Lx) show %f %f m (%i) show\n", 
			 x0, y0, x1, y0, event.chain[3].del);
  y0 = y0 - row; fprintf(out, "%f %f m (cluster delete Ly) show %f %f m (%i) show\n", 
			 x0, y0, x1, y0, event.chain[2].del);
  y0 = y0 - row; fprintf(out, "%f %f m (trunc ADC left) show %f %f m (%i) show\n", 
			 x0, y0, x1, y0, event.chain[0].t_left);
  y0 = y0 - row; fprintf(out, "%f %f m (trunc ADC right) show %f %f m (%i) show\n", 
			 x0, y0, x1, y0, event.chain[0].t_right);

  // Analyse parameter: quality parameter
  col = ps(12.0);
  x0 = col_left;
  x1 = x0 + col;
  x2 = x1 + col;
  y0 = y0 - 2*row;

  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", font2_size);
  fprintf(out, "%f %f m (Analyse parameters: qualities) show\n", x0, y0);
  fprintf(out, "/Helvetica findfont %f scalefont setfont\n", font2_size);
  fprintf(out, "%f %f m %f %f l %f lw s\n", 
	  col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));
  y0 = y0 - row; fprintf(out, "%f %f m (peak min.:) show %f %f m (%.1f) show\n", 
			 x0, y0, x1, y0, peak_min);
  fprintf(out, "gray5 %f %f m %f %f l %f lw s black\n", 
	  col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));
  y0 = y0 - row; fprintf(out, "%f %f m (theta) show %f %f m (min=%.1f)show %f %f m (max=%.1f) show\n",
			 x0, y0, x1, y0, theta_min, x1+col, y0, theta_max);
  
  fprintf(out, "gray5 %f %f m %f %f l %f lw s black\n", col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));
  y0 = y0 - row; fprintf(out, "%f %f m (q ratio M:) show %f %f m (QMy = p0 + p1*QMx + p2*QMx^2 + p3*QMx^3) show\n", 
			 x0, y0, x1, y0);
  y0 = y0 - row; fprintf(out, "%f %f m (p0=%.2f) show %f %f m (p1=%.2f) show\n", 
			 x1, y0, fit_c_M[0], x2, y0, fit_c_M[1]);
  y0 = y0 - row; fprintf(out, "%f %f m (p2=%.2f) show %f %f m (p3=%.2f) show\n", 
			 x1, y0, fit_c_M[2], x2, y0, fit_c_M[3]);
  y0 = y0 - row; fprintf(out, "%f %f m (width=%.0f) show %f %f m (scale=%.3f) show\n", 
			 x1, y0, qual_c_M[0], x2, y0, qual_c_M[2]);
  y0 = y0 - row; fprintf(out, "%f %f m (plateau=%.0f) show %f %f m (scale=%.3f) show\n", 
			 x1, y0, qual_c_M[1], x2, y0, qual_c_M[3]);

  fprintf(out, "gray5 %f %f m %f %f l %f lw s black\n", col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));
  y0 = y0 - row; fprintf(out, "%f %f m (q ratio L:) show %f %f m (QLy = p0 + p1*QLx + p2*QLx^2 + p3*QLx^3) show\n", 
			 x0, y0, x1, y0);
  y0 = y0 - row; fprintf(out, "%f %f m (p0=%.2f) show %f %f m (p1=%.2f) show\n", 
			 x1, y0, fit_c_L[0], x2, y0, fit_c_L[1]);
  y0 = y0 - row; fprintf(out, "%f %f m (p2=%.2f) show %f %f m (p3=%.2f) show\n", 
			 x1, y0, fit_c_L[2], x2, y0, fit_c_L[3]);
  y0 = y0 - row; fprintf(out, "%f %f m (width=%.0f) show %f %f m (scale=%.3f) show\n", 
			 x1, y0, qual_c_L[0], x2, y0, qual_c_L[2]);
  y0 = y0 - row; fprintf(out, "%f %f m (plateau=%.0f) show %f %f m (scale=%.3f) show\n", 
			 x1, y0, qual_c_L[1], x2, y0, qual_c_L[3]);

  fprintf(out, "gray5 %f %f m %f %f l %f lw s black\n", col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));
  y0 = y0 - row; fprintf(out, "%f %f m (phi) show %f %f m (phi=p0 + p1*Ly) show\n", x0, y0, x1, y0);
  y0 = y0 - row; fprintf(out, "%f %f m (p0=%.3f) show %f %f m (p1=%.3f) show\n", x1, y0, offset_phi, x2, y0, slope_phi);
  y0 = y0 - row; fprintf(out, "%f %f m (width=%.1f) show %f %f m (plateau=%.1f) show\n", 
			 x1, y0, qual_phi[0], x2, y0, qual_phi[1]);

  fprintf(out, "gray5 %f %f m %f %f l %f lw s black\n", col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));
  y0 = y0 - row; fprintf(out, "%f %f m (TOF hits) show %f %f m (width=%.1f)show %f %f m (plateau=%.1f) show\n", 
			 x0, y0, x1, y0, qual_walls, x2, y0, 1.0);

  fprintf(out, "gray5 %f %f m %f %f l %f lw s black\n", col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));
  y0 = y0 - row; fprintf(out, "%f %f m (TOF times) show %f %f m (width=%.1f)show %f %f m (plateau=%.1f) show\n", 
			 x0, y0, x1, y0, qual_gaussian_time, x2, y0, qual_plateau_time);

  fprintf(out, "gray5 %f %f m %f %f l %f lw s black\n", col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));
  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", font3_size);
  y0 = y0 - 3*row; fprintf(out, "%f %f m (event no.: %li) show\n", x0, y0, eventno);
  fprintf(out, "gray5 %f %f m %f %f l %f lw s black\n", col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));

  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", font3_size);
  y0 = y0 - 3*row; fprintf(out, "%f %f m (quality: ) show\n", x0, y0); 
  ps_draw_qual(out, x0+50, y0, quality->scalar());
  fprintf(out, "gray5 %f %f m %f %f l %f lw s black\n", col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));

  // Text ...
  col_left = b_left;
  row_top  = baseG_y - labps(30*lG);
  //  col = ps(12.0);
  col = ps(12.0);
  row = font2_size*1.1;

  x0 = col_left+col/2;
  y0 = row_top;
  y1 = row_top-row;

  fprintf(out, "%% Table Quality\n");
  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", font3_size);
  fprintf(out, "%f %f m (List of Tracks within trigger acceptance:) show\n", x0, y0 + 3*row); 

  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", font2_size);
  fprintf(out, "%f %f (Track) center\n", x0, y1);
  x0 = x0 + col;
  fprintf(out, "%f %f (Cluster) center\n", x0, y0);
  fprintf(out, "%f %f (Mx) center\n", x0, y1);
  x0 = x0 + col;
  fprintf(out, "%f %f (Cluster) center\n", x0, y0);
  fprintf(out, "%f %f (My) center\n", x0, y1);
  x0 = x0 + col;
  fprintf(out, "%f %f (Cluster) center\n", x0, y0);
  fprintf(out, "%f %f (Lx) center\n", x0, y1);
  x0 = x0 + col;
  fprintf(out, "%f %f (Cluster) center\n", x0, y0);
  fprintf(out, "%f %f (Ly) center\n", x0, y1);
  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (total) center\n", x0, y1);

  /*  x0 = x0 + col;
      fprintf(out, "%f %f (Quality) center\n", x0, y0);
      fprintf(out, "%f %f (Mx) center\n", x0, y1);
      x0 = x0 + col;
      fprintf(out, "%f %f (Quality) center\n", x0, y0);
      fprintf(out, "%f %f (My) center\n", x0, y1);
      x0 = x0 + col;
      fprintf(out, "%f %f (Quality) center\n", x0, y0);
      fprintf(out, "%f %f (Lx) center\n", x0, y1);
      x0 = x0 + col;
      fprintf(out, "%f %f (Quality) center\n", x0, y0);
      fprintf(out, "%f %f (Ly) center\n", x0, y1);
  */

  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (Ratio Mx/My) center\n", x0, y1);
  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (Ratio Lx/Ly) center\n", x0, y1);

  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (theta) center\n", x0, y1);
  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (phi) center\n", x0, y1);

  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (Wall F) center\n", x0, y1);
  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (Wall G) center\n", x0, y1);

  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (Time F) center\n", x0, y1);

  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (Time G) center\n", x0, y1);

  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (trigger) center\n", x0, y1);
 
  fprintf(out, "%f %f m %f %f l %f lw s\n", col_left, y1-row/4, 
	  x0+col/2, y1-row/4, ps(0.1));
  fprintf(out, "/Helvetica findfont %f scalefont setfont\n", font2_size);

  // Values
  int cnt_track = -1;
  int cnt_list = -1;
  y0 = row_top - row;
  for (std::list<t_track>::iterator it=event.track.begin(); 
       it!=event.track.end(); ++it)
    {
      cnt_track++;
      if (it->q_FGtrig && (cnt_list < 10))
	{
	  x0 = col_left+col/2;
	  y0 = y0 - row;
	  cnt_list++;
	  fprintf(out, "%f %f (%i) center\n", x0, y0, cnt_track);
	
	  fprintf(out, "blue\n");
	  x0 = x0 + col;
	  fprintf(out, "%f %f (%i) center\n", x0, y0, it->id[1]);
	  x0 = x0 + col;
	  fprintf(out, "%f %f (%i) center\n", x0, y0, it->id[0]);
	  x0 = x0 + col;
	  fprintf(out, "%f %f (%i) center\n", x0, y0, it->id[3]);
	  x0 = x0 + col;
	  fprintf(out, "%f %f (%i) center\n", x0, y0, it->id[2]);
    
	  fprintf(out, "black\n");
	  x0 = x0 + col; ps_draw_qual(out, x0, y0, it->quality);

	  /*
	    x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_max_value[1]);
	    x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_max_value[0]);
	    x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_max_value[3]);
	    x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_max_value[2]);
	  */
	
	  x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_c_M);
	  x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_c_L);

	  x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_theta);
	  x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_phi);
	
	  x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_wall_F);
	  x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_wall_G);
	
	  x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_time_F);
	  x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_time_G);
	
	  x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_FGtrig);
	}
    }
  if (uint (cnt_track) < event.track.size()) fprintf(out, "%f %f (and more tracks) center\n", x0 + col, y0);

  fprintf(out, "showpage\n");
  fprintf(out, "%%%%Trailer\n");
  fprintf(out, "%%%%EOF\n");
  fclose(out);

  } //WALLF

  if (use_tof == 2) // WALL H
{  
  // =============================================================
  // create ps-File for tracks
  // =============================================================
  FILE *out=fopen(fname_track,"w");
  ps_header(out, eventno);
  fprintf(out, "%%%%Page %i %i\n", 1, 1);
  fprintf(out, "white\n");
  fprintf(out, "n 0 0 m 595 0 l 595 842 l 0 842 l c fill\n");

  fprintf(out, "black\n");
  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", ps(6.0));
  fprintf(out, "%f %f m (Kaos Event Display) show\n", 180., b_top);
  //  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", ps(4.0));
  //  fprintf(out, "%f %f m (MWPCs and TOF Walls) show\n", 200., b_top-20);

  fprintf(out, "/Helvetica findfont %f scalefont setfont\n", font1_size);

  // =====================================================
  // x-chains
  // =====================================================

  // === Data Lx 
  fprintf(out, "%% Data Lx\n");
  fprintf(out, "gs\n");
  fprintf(out, "%f %f translate\n", baseLx_x, baseLx_y);
  for (int i=0; i<KAOS_MWPC_ADCX; i++)
  {
    if (Lx[i]->ok())  buffer[i]=Lx[i]->scalar();
      else            buffer[i]=0.0;
  }
  draw_chain(out, buffer, KAOS_MWPC_ADCX, "MWPC Lx", signal_scale, font1_size);
  fprintf(out, "gr\n");

  // === Data Mx
  fprintf(out, "%% Data Mx\n");
  fprintf(out, "gs\n");
  fprintf(out, "%f %f translate\n", baseMx_x, baseMx_y);
  for (int i=0; i<KAOS_MWPC_ADCX; i++)
  {
    if (Mx[i]->ok())  buffer[i]=Mx[i]->scalar();
      else            buffer[i]=0.0;
  }
  draw_chain(out, buffer, KAOS_MWPC_ADCX, "MWPC Mx", signal_scale, font1_size);
  fprintf(out, "gr\n");

  // === Data Wall H


  double alpha  =  0.0;     // paddle inclination angle
  double b = labps(20.0); // paddle width (lab): 20 mm
    
  fprintf(out, "%%Data Wall H\n");
  fprintf(out, "gs\n");
  fprintf(out, "%f %f translate\n", baseF_x, baseF_y);
  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", font1_size);
  fprintf(out, "%f %f (%s) center\n", 0.0, 0.0+labps(lF/2)+font1_size/4, 
	  "TOF Wall H");
  fprintf(out, "/Helvetica findfont %f scalefont setfont\n", font1_size);
  fprintf(out, "gray5\n");

  // draw line through the center of Wall H
  fprintf(out, "%f %f m %f %f l %f lw s\n", 0.0, labps(lF/2), 
	  -labps( (KAOS_WALLH_PADDLES*lF-lF/2)*sin(-alphaF/degree)), 
	  -labps( (KAOS_WALLH_PADDLES*lF-lF/2)*cos(-alphaF/degree)),
	  ps(0.1));
  
  fprintf(out, "black\n");
  for (int i=0; i<KAOS_WALLH_PADDLES; i++)
  {
   double l = labps(lF);   // lF: distance between two paddles
                            
    double x0 = -b/2;
    double y0 = +l/2;
    double x1 = +b/2;
    double y1 = +l/2;

    double x2 = -x0;
    double y2 = -y0;
    double x3 = -x1;
    double y3 = -y1;

    fprintf(out, "black\n");
    if (event.tof[0][i]==0 || event.tof[1][i]==0)
      fprintf(out, 
      "%f rotate n %f %f m %f %f l %f %f l %f %f l c %f lw s %f rotate\n",
	      alphaF, x0, y0, x1, y1, x2, y2, x3, y3, ps(0.1), -alphaF);
    else
      fprintf(out, 
      "%f rotate n %f %f m %f %f l %f %f l %f %f l c fill %f lw s %f rotate\n",
	      alphaF, x0, y0, x1, y1, x2, y2, x3, y3, ps(0.1), -alphaF);
    if ((i%5==0) || (i==29))
      fprintf(out, "%f rotate %f %f (%i) center %f rotate\n",
      (alpha+90), 0.0, (b-font1_size)/2-b/2+font1_size/7, i, -(alpha+90));
    fprintf(out, "%f %f translate\n", 
	    -labps(lF*sin(-alphaF/degree)),
	    -labps(lF*cos(-alphaF/degree)) );
  }
  fprintf(out, "gr\n");

  // === Data Wall G
  fprintf(out, "%%Data Wall G\n");
  fprintf(out, "gs\n");
  fprintf(out, "%f %f translate\n", baseG_x, baseG_y);
  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", font1_size);
  fprintf(out, "%f %f (%s) center\n", 0.0, 0.0+labps(lG/2)+font1_size/4, 
	  "TOF Wall G");
  fprintf(out, "/Helvetica findfont %f scalefont setfont\n", font1_size);
  fprintf(out, "gray5\n");

  // draw line through the center of Wall G
  fprintf(out, "%f %f m %f %f l %f lw s\n", 0.0, labps(lG/2), 
	  -labps( (KAOS_WALL_PADDLES*lG-lG/2)*sin(-alphaG/degree)), 
	  -labps( (KAOS_WALL_PADDLES*lG-lG/2)*cos(-alphaG/degree)),
	  ps(0.1));
  
  fprintf(out, "black\n");
  for (int i=0; i<KAOS_WALL_PADDLES; i++)
  {
    double alpha = 0.0;       // paddle angle
    double b = labps(20.0);   // paddle width (lab): 20 mm
    double l = labps(lG);     // distance between two paddles
    double x0 = -b/2;
    double y0 = +l/2;
    double x1 = +b/2;
    double y1 = +l/2;

    double x2 = -x0;
    double y2 = -y0;
    double x3 = -x1;
    double y3 = -y1;

    fprintf(out, "black\n");
    if (event.tof[2][i]==0 || event.tof[3][i]==0)
      fprintf(out, 
      "%f rotate n %f %f m %f %f l %f %f l %f %f l c %f lw s %f rotate\n",
	      alphaG, x0, y0, x1, y1, x2, y2, x3, y3, ps(0.1), -alphaG);
    else
      fprintf(out, 
      "%f rotate n %f %f m %f %f l %f %f l %f %f l c fill %f lw s %f rotate\n",
	      alphaG, x0, y0, x1, y1, x2, y2, x3, y3, ps(0.1), -alphaG);
    if ((i%5==0) || (i==29))
      fprintf(out, "%f rotate %f %f (%i) center %f rotate\n",
      (alpha+90), 0.0, (b-font1_size)/2-b/2+font1_size/7, i, -(alpha+90));
    fprintf(out, "%f %f translate\n", 
	    -labps(lG*sin(-alphaG/degree)),
	    -labps(lG*cos(-alphaG/degree)) );
  }
  fprintf(out, "gr\n");


  // === Tracks x
  fprintf(out, "%% Tracks x\n");
  fprintf(out, "gs\n");
  fprintf(out, "%f %f translate\n", baseLx_x, baseLx_y);
  for (unsigned int i=0; i<event.chain[1].cluster.size(); i++)
  {
    for (unsigned int j=0; j<event.chain[3].cluster.size(); j++)
    {
      int cnt_track=0;
      int found_track0 = 0;
      double qual = 0;
      std::stringstream str;
      for (std::list<t_track>::iterator it=event.track.begin(); it!=event.track.end(); ++it)
      {
        if ((int (i)==it->id[1]) && (int (j)==it->id[3]))
	{
	  str << cnt_track << ",";
	  if (cnt_track==0) found_track0=1;
	  qual = qual + it->quality;
	}
        cnt_track++;
      }
      if (str.str().length()>0)
      {
	double theta = get_theta(event.chain[1].cluster[i].cofc_lab,
				 event.chain[3].cluster[j].cofc_lab);
	double hitLx = labps(event.chain[3].cluster[j].cofc_lab);
	double hitMx = labps(event.chain[1].cluster[i].cofc_lab);

	// Gerade durch hit-Positionen bestimmen y=mx+b
	double m = (hitLx - hitMx) / (0.0 - labps(zLM));
	double b = -hitLx - m * 0.0;
	double x0 = -labps(zLM)+xtrack_leftoffset;
	double y0 = m * x0 + b;
	double x1 = +labps(zLF*1.8)+xtrack_rightoffset; ////// !
	double y1 = m * x1 + b;

	if (qual>0)
	{
          if (found_track0==1) fprintf(out, "black %f lw\n", track0_width);
	    else fprintf(out, "yellow2 %f lw\n", track_width);
	}
	else fprintf(out, "gray7 %f lw\n", track_width);

        fprintf(out, "%f %f m %f %f l s\n", x0, y0, x1, y1);
	// arrow
	fprintf(out, "n %f %f m %f rotate %f %f r %f %f r %f %f r %f rotate c fill s\n",
		x1, y1, -theta, -arrow_len, +arrow_width/2, 0.0, 
		-arrow_width, +arrow_len, +arrow_width/2, + theta);
	// track numbers
        fprintf(out, "black\n");
	fprintf(out, "%f %f m (%s) show\n", 
		x1+ps(0.5), y1-font1_size/3, str.str().erase(str.str().size()-1).c_str());
      }
    }
  }


  // === Hit position Lx 
  fprintf(out, "%% Hitposition Lx\n");
  fprintf(out, "blue\n");
  for (unsigned int i=0; i<event.chain[3].cluster.size(); i++)
  {
    double hitLx = labps(event.chain[3].cluster[i].cofc_lab);
    fprintf(out, "%f %f m %f %f l %f setlinewidth s\n",
                 +ps(2.0-1.8), -hitLx,
		 -255*signal_scale, -hitLx,
		 hit_width);
    fprintf(out, "%f %f %f 0 360 arc fill s\n", ps(2.0), -hitLx, ps(1.8));
    fprintf(out, "white\n");
    fprintf(out, "%f %f m (%i) show\n", ps(1.5), -hitLx-font1_size/3, i);
    fprintf(out, "blue\n");
  }
  // === Hit postion Mx
  fprintf(out, "%% Hitposition Mx\n");
  for (unsigned int i=0; i<event.chain[1].cluster.size(); i++)
  {
    double hitMx = labps(event.chain[1].cluster[i].cofc_lab);
    fprintf(out, "%f %f m %f %f l %f setlinewidth s\n",
                 -labps(zLM)+ps(2.0-1.8), -hitMx,
		 -labps(zLM)-255*signal_scale, -hitMx,
		 hit_width);
    fprintf(out, "%f %f %f 0 360 arc fill s\n", -labps(zLM)+ps(2.0), -hitMx, ps(1.8));
    fprintf(out, "white\n");
    fprintf(out, "%f %f m (%i) show\n", -labps(zLM)+ps(1.5), -hitMx-font1_size/3, i);
    fprintf(out, "blue\n");
  }
  fprintf(out, "gr\n");


  // =====================================================
  // y-chains
  // =====================================================

  // === Data My
  fprintf(out, "%% Data My\n");
  fprintf(out, "gs\n");
  fprintf(out, "%f %f translate\n", baseMy_x, baseMy_y);
  for (int i=0; i<KAOS_MWPC_ADCY; i++)
  {
    if (My[i]->ok())  buffer[i]=My[i]->scalar();
      else            buffer[i]=0.0;
  }
  draw_chain(out, buffer, KAOS_MWPC_ADCY, "MWPC My", signal_scale, font1_size);
  fprintf(out, "gr\n");

  // Data Ly
  fprintf(out, "%% Data Ly\n");
  fprintf(out, "gs\n");
  fprintf(out, "%f %f translate\n", baseLy_x, baseLy_y);
  for (int i=0; i<KAOS_MWPC_ADCY; i++)
  {
    if (Ly[i]->ok())  buffer[i]=Ly[i]->scalar();
      else            buffer[i]=0.0;
  }
  draw_chain(out, buffer, KAOS_MWPC_ADCY, "MWPC Ly", signal_scale, font1_size);
  fprintf(out, "gr\n");

  // === Tracks y
  fprintf(out, "%% Tracks y\n");
  fprintf(out, "gs\n");
  fprintf(out, "%f %f translate\n", base0_x+labps(34.5*5), baseLy_y);
  for (unsigned int i=0; i<event.chain[0].cluster.size(); i++)
  {
    for (unsigned int j=0; j<event.chain[2].cluster.size(); j++)
    {
      int cnt_track=0;
      int found_track0 = 0;
      double qual = 0;
      std::stringstream str;
      for (std::list<t_track>::iterator it=event.track.begin(); it!=event.track.end(); ++it)
      {
        if ((int (i)==it->id[0]) && (int (j)==it->id[2]))
	{
	  str << cnt_track << ",";
	  if (cnt_track==0) found_track0=1;
	  qual = qual + it->quality;
	}
        cnt_track++;
      }
      if (str.str().length()>0)
      {
	// Gerade durch hit-Positionen bestimmen y=mx+b
	double phi = get_phi(event.chain[0].cluster[i].cofc_lab,
			     event.chain[2].cluster[j].cofc_lab);
	
	double hitMy = event.chain[0].cluster[i].cofc_lab;
	double hitLy = event.chain[2].cluster[j].cofc_lab;
	double m = (0.0-labps(zLM)) / (-labps(hitLy) - (-labps(hitMy)));
	double b = 0.0 - m * (-labps(hitLy));
	double y0 = labps(zLM)+ytrack_topoffet;      // Anfangspunkt y0 festlegen
	double x0 = (y0-b)/m;                        // Berechnung x0 zu y0
	double y1 = ytrack_botoffset;                // Endpunkt y1 festlegen
	double x1 = (y1-b)/m;                        // Berechnung x1 zu y1
	
	if (qual>0)
	{
          if (found_track0==1) fprintf(out, "black %f lw\n", track0_width);
	    else fprintf(out, "yellow2 %f lw\n", track_width);
	}
	else fprintf(out, "gray7 %f\n", track_width);
	fprintf(out, "%f %f m %f %f l s\n", x0, y0, x1, y1);
	// arrow
	fprintf(out, "n %f %f m %f rotate %f %f r %f %f r %f %f r %f rotate c fill s\n",
		x1, y1, -phi, +arrow_width/2, +arrow_len, -arrow_width, 0.0, +arrow_width/2, -arrow_len, phi);
        // print track numbers
        fprintf(out, "black\n");
	fprintf(out, "%f %f m -90 rotate (%s) show +90 rotate\n",
		x1-font1_size/3, y1-ps(0.5), str.str().erase(str.str().size()-1).c_str());
      }
    }
  }

  // === Hit postition My
  fprintf(out, "%% Hitposition My\n");
  fprintf(out, "blue\n");
  for (unsigned int i=0; i<event.chain[0].cluster.size(); i++)
  {
    double hitMy = labps(event.chain[0].cluster[i].cofc_lab);
    fprintf(out, "%f %f m %f %f l %f setlinewidth s\n",
                 -hitMy, -ps(2.0-1.8)+labps(zLM),
		 -hitMy, +255*signal_scale+labps(zLM),
		 hit_width);
    fprintf(out, "%f %f %f 0 360 arc fill s\n", -hitMy, -ps(2.0)+labps(zLM), ps(1.8));
    fprintf(out, "white\n");
    fprintf(out, "%f %f m (%i) show\n", 
	    -hitMy-font1_size/4 , labps(zLM)-ps(1.5)-font1_size/1.7, i);
    fprintf(out, "blue\n");
  }
  // === Hit postition Ly
  fprintf(out, "%% Hitposition Ly\n");
  fprintf(out, "blue\n");
  for (unsigned int i=0; i<event.chain[2].cluster.size(); i++)
  {
    double hitLy = labps(event.chain[2].cluster[i].cofc_lab);
    fprintf(out, "%f %f m %f %f l %f setlinewidth s\n",
                 -hitLy, -ps(2.0-1.8),
		 -hitLy, +255*signal_scale,
		 hit_width);
    fprintf(out, "%f %f %f 0 360 arc fill s\n", -hitLy, -ps(2.0), ps(1.8));
    fprintf(out, "white\n");
    fprintf(out, "%f %f m (%i) show\n", -hitLy-font1_size/4 , -ps(1.5)-font1_size/1.7, i);
    fprintf(out, "blue\n");
  }
  fprintf(out, "gr\n");

  // Text ...
  double col_left, row_top, col, row, x0, y0, x1, y1, x2;

  // Detector Position
  col = ps(15.0);
  if (baseLy_x > baseMy_x) col_left=baseLy_x + col;
  else col_left=baseMy_x + col;
  row_top = baseLy_y -ps(60.0);
  row = font2_size*1.1;

  x0 = col_left;
  y0 = row_top;
  x1 = col_left + col/1.5;
  x2 = col_left + 2*col/2;

if (use_tof == 1) // WALL F
{
  double colmul = 3.1;

  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", font2_size);
  fprintf(out, "%f %f m (Detector positions) show\n", x0, y0);
  fprintf(out, "/Helvetica findfont %f scalefont setfont\n", font2_size);
  fprintf(out, "%f %f m %f %f l %f lw s\n", 
	  col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));
  y0 = y0 - row; fprintf(out, "%f %f m (MWPC M:) show %f %f m (xLM) show %f %f m (%.1f) show\n", 
			 x0, y0, x1, y0, x2, y0, xLM);
  y0 = y0 - row; fprintf(out, "%f %f m (MWPC M:) show %f %f m (zLM) show %f %f m (%.1f) show\n", 
			 x0, y0, x1, y0, x2, y0, zLM);
  y0 = y0 - row; fprintf(out, "%f %f m (MWPC M:) show %f %f m (yM)  show %f %f m (%.1f) show\n", 
			 x0, y0, x1, y0, x2, y0, yM);
  y0 = y0 - row; fprintf(out, "%f %f m (MWPC L:) show %f %f m (yL)  show %f %f m (%.1f) show\n", 
			 x0, y0, x1, y0, x2, y0, yL);
  y0 = y0 - row; fprintf(out, "%f %f m (WALL H:) show %f %f m (xLH) show %f %f m (%.1f) show\n", 
			 x0, y0, x1, y0, x2, y0, xLF);
  y0 = y0 - row; fprintf(out, "%f %f m (WALL H:) show %f %f m (zLH) show %f %f m (%.1f) show\n", 
			 x0, y0, x1, y0, x2, y0, zLF);
  y0 = y0 - row; fprintf(out, "%f %f m (WALL G:) show %f %f m (xLG) show %f %f m (%.1f) show\n", 
			 x0, y0, x1, y0, x2, y0, xLG);
  y0 = y0 - row; fprintf(out, "%f %f m (WALL G:) show %f %f m (zLG) show %f %f m (%.1f) show\n", 
			 x0, y0, x1, y0, x2, y0, zLG);
  
  // Analyse parameter: cluster detection
  col = ps(15.0);
  x0 = col_left;
  x1 = x0 + col*2;
  x2 = x1 + col*3;
  y0 = y0 - 2*row;
  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", font2_size);
  fprintf(out, "%f %f m (Analyse parameters: cluster detection) show\n", x0, y0);
  fprintf(out, "/Helvetica findfont %f scalefont setfont\n", font2_size);
  fprintf(out, "%f %f m %f %f l %f lw s\n", 
	  col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));
  y0 = y0 - row; fprintf(out, "%f %f m (ADC threshold) show %f %f m (%.0f) show\n", 
			 x0, y0, x1, y0, event.chain[0].thresh[0]);
  y0 = y0 - row; fprintf(out, "%f %f m (cluster separation Mx) show %f %f m (%i) show\n", 
			 x0, y0, x1, y0, event.chain[1].sep);
  y0 = y0 - row; fprintf(out, "%f %f m (cluster separation My) show %f %f m (%i) show\n", 
			 x0, y0, x1, y0, event.chain[0].sep);
  y0 = y0 - row; fprintf(out, "%f %f m (cluster separation Lx) show %f %f m (%i) show\n", 
			 x0, y0, x1, y0, event.chain[3].sep);
  y0 = y0 - row; fprintf(out, "%f %f m (cluster separation Ly) show %f %f m (%i) show\n", 
			 x0, y0, x1, y0, event.chain[2].sep);
  y0 = y0 - row; fprintf(out, "%f %f m (cluster delete Mx) show %f %f m (%i) show\n", 
			 x0, y0, x1, y0, event.chain[1].del);
  y0 = y0 - row; fprintf(out, "%f %f m (cluster delete My) show %f %f m (%i) show\n", 
			 x0, y0, x1, y0, event.chain[0].del);
  y0 = y0 - row; fprintf(out, "%f %f m (cluster delete Lx) show %f %f m (%i) show\n", 
			 x0, y0, x1, y0, event.chain[3].del);
  y0 = y0 - row; fprintf(out, "%f %f m (cluster delete Ly) show %f %f m (%i) show\n", 
			 x0, y0, x1, y0, event.chain[2].del);
  y0 = y0 - row; fprintf(out, "%f %f m (trunc ADC left) show %f %f m (%i) show\n", 
			 x0, y0, x1, y0, event.chain[0].t_left);
  y0 = y0 - row; fprintf(out, "%f %f m (trunc ADC right) show %f %f m (%i) show\n", 
			 x0, y0, x1, y0, event.chain[0].t_right);

  // Analyse parameter: quality parameter
  col = ps(12.0);
  x0 = col_left;
  x1 = x0 + col;
  x2 = x1 + col;
  y0 = y0 - 2*row;

  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", font2_size);
  fprintf(out, "%f %f m (Analyse parameters: qualities) show\n", x0, y0);
  fprintf(out, "/Helvetica findfont %f scalefont setfont\n", font2_size);
  fprintf(out, "%f %f m %f %f l %f lw s\n", 
	  col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));
  y0 = y0 - row; fprintf(out, "%f %f m (peak min.:) show %f %f m (%.1f) show\n", 
			 x0, y0, x1, y0, peak_min);
  fprintf(out, "gray5 %f %f m %f %f l %f lw s black\n", 
	  col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));
  y0 = y0 - row; fprintf(out, "%f %f m (theta) show %f %f m (min=%.1f)show %f %f m (max=%.1f) show\n",
                              x0, y0, x1, y0, theta_min, x1+col, y0, theta_max);
  
  fprintf(out, "gray5 %f %f m %f %f l %f lw s black\n", col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));
  y0 = y0 - row; fprintf(out, "%f %f m (q ratio M:) show %f %f m (QMy = p0 + p1*QMx + p2*QMx^2 + p3*QMx^3) show\n", 
			 x0, y0, x1, y0);
  y0 = y0 - row; fprintf(out, "%f %f m (p0=%.2f) show %f %f m (p1=%.2f) show\n", 
			 x1, y0, fit_c_M[0], x2, y0, fit_c_M[1]);
  y0 = y0 - row; fprintf(out, "%f %f m (p2=%.2f) show %f %f m (p3=%.2f) show\n", 
			 x1, y0, fit_c_M[2], x2, y0, fit_c_M[3]);
  y0 = y0 - row; fprintf(out, "%f %f m (width=%.0f) show %f %f m (scale=%.3f) show\n", 
			 x1, y0, qual_c_M[0], x2, y0, qual_c_M[2]);
  y0 = y0 - row; fprintf(out, "%f %f m (plateau=%.0f) show %f %f m (scale=%.3f) show\n", 
			 x1, y0, qual_c_M[1], x2, y0, qual_c_M[3]);

  fprintf(out, "gray5 %f %f m %f %f l %f lw s black\n", col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));
  y0 = y0 - row; fprintf(out, "%f %f m (q ratio L:) show %f %f m (QLy = p0 + p1*QLx + p2*QLx^2 + p3*QLx^3) show\n", 
			 x0, y0, x1, y0);
  y0 = y0 - row; fprintf(out, "%f %f m (p0=%.2f) show %f %f m (p1=%.2f) show\n", 
			 x1, y0, fit_c_L[0], x2, y0, fit_c_L[1]);
  y0 = y0 - row; fprintf(out, "%f %f m (p2=%.2f) show %f %f m (p3=%.2f) show\n", 
			 x1, y0, fit_c_L[2], x2, y0, fit_c_L[3]);
  y0 = y0 - row; fprintf(out, "%f %f m (width=%.0f) show %f %f m (scale=%.3f) show\n", 
			 x1, y0, qual_c_L[0], x2, y0, qual_c_L[2]);
  y0 = y0 - row; fprintf(out, "%f %f m (plateau=%.0f) show %f %f m (scale=%.3f) show\n", 
			 x1, y0, qual_c_L[1], x2, y0, qual_c_L[3]);

  fprintf(out, "gray5 %f %f m %f %f l %f lw s black\n", col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));
  y0 = y0 - row; fprintf(out, "%f %f m (phi) show %f %f m (phi=p0 + p1*Ly) show\n", x0, y0, x1, y0);
  y0 = y0 - row; fprintf(out, "%f %f m (p0=%.3f) show %f %f m (p1=%.3f) show\n", x1, y0, offset_phi, x2, y0, slope_phi);
  y0 = y0 - row; fprintf(out, "%f %f m (width=%.1f) show %f %f m (plateau=%.1f) show\n", 
			 x1, y0, qual_phi[0], x2, y0, qual_phi[1]);

  fprintf(out, "gray5 %f %f m %f %f l %f lw s black\n", col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));
  y0 = y0 - row; fprintf(out, "%f %f m (TOF hits) show %f %f m (width=%.1f)show %f %f m (plateau=%.1f) show\n", 
			 x0, y0, x1, y0, qual_walls, x2, y0, 1.0);

  fprintf(out, "gray5 %f %f m %f %f l %f lw s black\n", col_left, y0-row/4, col_left+colmul*col, y0-row/4, ps(0.1));
  y0 = y0 - row; fprintf(out, "%f %f m (TOF times) show %f %f m (width=%.1f)show %f %f m (plateau=%.1f) show\n", 
			 x0, y0, x1, y0, qual_gaussian_time, x2, y0, qual_plateau_time);

 } // WALL H

  col_left = b_left;
  row_top  = baseG_y - labps(27.5*lG);

  col = ps(12.0);
  row = font2_size*1.1;

  x0 = col_left+col/2;
  y0 = row_top;
  y1 = row_top-row;

  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", font3_size);
  y0 = y0 - 3*row; fprintf(out, "%f %f m (Event no.: %li) show\n", x0, y0, eventno);

  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", font3_size);
  y0 = y0 - 3*row; fprintf(out, "%f %f m (Quality: ) show\n", x0, y0); 

  ps_draw_qual(out, x0+46, y0, quality->scalar());

  // Text ...
  col_left = b_left;
  row_top  = baseG_y - labps(31*lG);
  col = ps(12.0);
  row = font2_size*1.1;

  x0 = col_left+col/2;
  y0 = row_top;
  y1 = row_top-row;

  fprintf(out, "%% Table Quality\n");
  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", font3_size);
  fprintf(out, "%f %f m (List of Tracks:) show\n", x0, y0 + 3*row); 

  fprintf(out, "/Helvetica-Bold findfont %f scalefont setfont\n", font2_size);
  fprintf(out, "%f %f (Track) center\n", x0, y1);
  x0 = x0 + col;
  fprintf(out, "%f %f (Cluster) center\n", x0, y0);
  fprintf(out, "%f %f (Mx) center\n", x0, y1);
  x0 = x0 + col;
  fprintf(out, "%f %f (Cluster) center\n", x0, y0);
  fprintf(out, "%f %f (My) center\n", x0, y1);
  x0 = x0 + col;
  fprintf(out, "%f %f (Cluster) center\n", x0, y0);
  fprintf(out, "%f %f (Lx) center\n", x0, y1);
  x0 = x0 + col;
  fprintf(out, "%f %f (Cluster) center\n", x0, y0);
  fprintf(out, "%f %f (Ly) center\n", x0, y1);
  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (total) center\n", x0, y1);

  /*  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (Mx) center\n", x0, y1);
  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (My) center\n", x0, y1);
  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (Lx) center\n", x0, y1);
  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (Ly) center\n", x0, y1);
  */

  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (Ratio Mx/My) center\n", x0, y1);
  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (Ratio Lx/Ly) center\n", x0, y1);

  //  x0 = x0 + col;
  //  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  //  fprintf(out, "%f %f (theta) center\n", x0, y1);
  //  x0 = x0 + col;
  //  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  //  fprintf(out, "%f %f (phi) center\n", x0, y1);

  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (Wall H) center\n", x0, y1);
  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (Wall G) center\n", x0, y1);

  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (Time H) center\n", x0, y1);

  x0 = x0 + col;
  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  fprintf(out, "%f %f (Time G) center\n", x0, y1);

  //  x0 = x0 + col;
  //  fprintf(out, "%f %f (Quality) center\n", x0, y0);
  //  fprintf(out, "%f %f (trigger) center\n", x0, y1);
 
  fprintf(out, "%f %f m %f %f l %f lw s\n", col_left, y1-row/4, 
	  x0+col/2, y1-row/4, ps(0.1));
  fprintf(out, "/Helvetica findfont %f scalefont setfont\n", font2_size);

  // Values
  int cnt_track = -1;
  int cnt_list = -1;
  y0 = row_top - row;
  for (std::list<t_track>::iterator it=event.track.begin(); 
       it!=event.track.end(); ++it)
  {
    cnt_track++;
    if (cnt_list < 10)
      {
	x0 = col_left+col/2;
	y0 = y0 - row;
	cnt_list++;
	fprintf(out, "%f %f (%i) center\n", x0, y0, cnt_track);
	
	fprintf(out, "blue\n");
	x0 = x0 + col;
	fprintf(out, "%f %f (%i) center\n", x0, y0, it->id[1]);
	x0 = x0 + col;
	fprintf(out, "%f %f (%i) center\n", x0, y0, it->id[0]);
	x0 = x0 + col;
	fprintf(out, "%f %f (%i) center\n", x0, y0, it->id[3]);
	x0 = x0 + col;
	fprintf(out, "%f %f (%i) center\n", x0, y0, it->id[2]);
    
	fprintf(out, "black\n");
	x0 = x0 + col; ps_draw_qual(out, x0, y0, it->quality);

	/*
	  x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_max_value[1]);
	  x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_max_value[0]);
	  x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_max_value[3]);
	  x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_max_value[2]);
	*/
	
	x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_c_M);
	x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_c_L);

	//	x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_theta);
	//	x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_phi);
	
	x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_wall_F);
	x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_wall_G);
	
	x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_time_F);
	x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_time_G);
	
	//	x0 = x0 + col; ps_draw_qual(out, x0, y0, it->q_FGtrig);
      }
  }
  if (cnt_track > cnt_list) 
    fprintf(out, "%f %f (and more tracks) center\n", x0 + col, y0);

  fprintf(out, "showpage\n");
  fprintf(out, "%%%%Trailer\n");
  fprintf(out, "%%%%EOF\n");
  fclose(out);

} // WALL F

}
