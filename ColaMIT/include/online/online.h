/*
 * Michael O. Distler
 * $Header: /tmp/cvsroot/Cola/ColaMIT/include/online/online.h,v 2.2 2002-03-20 21:02:18 cola Exp $
 */

#ifndef __ONLINE_H__
#define __ONLINE_H__

struct FocalCoord {
    double x;
    double th;
    double y;
    double ph;
};

struct TargetCoord {
    double dp;
    double th;
    double y0;
    double ph;
    double len;
};

struct Vdc {
    int time, wire, multi, error, pattern;
    double x, slope, chi_sqr;
};

struct HDCchamber {
  int error, wire, goodness, OEbit;
  double difftime, diffwire, drifttime;
  double coord, driftdist, rawcoord;
};

struct Hdc {
  struct HDCchamber chamber[4];
  int multi, pattern, OEpattern, FitPattern;
  double x, slope, chi_sqr;
};

struct vertex {
  double x; 
  double y;
  double z;             
};

struct LeadGlass {
  double top; 
  double btm; 
  double sum; 
};

struct ParticleCoord {
  int id;
  double mass;
  double Ekin;
  double momentum;
  double fourvec[4];
};

struct SpectrometerOHIPS {
  struct FocalCoord  chamber, chamberError;
  struct FocalCoord  focal,   focalError;
  struct TargetCoord target;
  struct Vdc u1, v1, u2, v2;
  struct ParticleCoord particle;
  struct vertex vertex;
  /** applied energy loss correction */ double Eloss_corr;
  /** simulated energy loss */          double Eloss_sim;
  int trigger;
  struct LeadGlass leadglas;
};

struct SpectrometerOOPS {
  struct FocalCoord  chamber, chamberError;
  struct FocalCoord  focal,   focalError;
  struct TargetCoord target;
  struct Hdc x, y;
  struct ParticleCoord particle;
  struct vertex vertex;
  /** applied energy loss correction */ double Eloss_corr;
  /** simulated energy loss */          double Eloss_sim;
  int trigger;
};

struct BeamInfo {
  short helicity;
  unsigned short gunFlag;
  unsigned short hel_pattern;
  unsigned short timeslot;
  struct {
    unsigned short bt1;
    unsigned short bt2;
  } toroid;
  struct {
    unsigned short molx;
    unsigned short moly;
    unsigned short tgtx;
    unsigned short tgty;
  } position;
  struct {
    unsigned short halo1;
    unsigned short halo2;
  } halo;
  double Eloss_corr;
  double Eloss_sim;
};

struct simulation {
  /** Virtual photon flux                  */ double Gamma;
  /** Integration Volume                   */ double Volume;
  /** Total Weight for this event          */ double weight;
};

struct online {
  int nr;
  /** Raw OHIPS-OOPSA coinc time [ns] */       double oa_tc;
  /** Raw OHIPS-OOPSB coinc time [ns] */       double ob_tc;
  /** Raw OHIPS-OOPSC coinc time [ns] */       double oc_tc;
  /** Raw OHIPS-OOPSD coinc time [ns] */       double od_tc;
  /** Corrected OHIPS-OOPSA coinc time [ns] */ double oa_tccorr;
  /** Corrected OHIPS-OOPSB coinc time [ns] */ double ob_tccorr;
  /** Corrected OHIPS-OOPSC coinc time [ns] */ double oc_tccorr;
  /** Corrected OHIPS-OOPSD coinc time [ns] */ double od_tccorr;
  /** OHIPS-OOPSA coinc time correction type */ int oa_tccorr_type;
  /** OHIPS-OOPSB coinc time correction type */ int ob_tccorr_type;
  /** OHIPS-OOPSC coinc time correction type */ int oc_tccorr_type;
  /** OHIPS-OOPSD coinc time correction type */ int od_tccorr_type;
  double IntegratedLuminosity;
  /** Total run time [s]                    */ double RunTime;
  /** Vertex pos. rel. to target system     */ struct vertex Vertex;
  int datataking;
  struct SpectrometerOHIPS ohips;
  struct SpectrometerOOPS  oopsA, oopsB, oopsC, oopsD;
  struct BeamInfo beam;
  /** Simulation variables                  */ struct simulation sim;
};

extern struct online online;

#endif /* __ONLINE_H__ */
