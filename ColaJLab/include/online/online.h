/*
 * Michael O. Distler
 * $Header: /tmp/cvsroot/Cola/ColaJLab/include/online/online.h,v 1.1.1.1 1999-01-24 21:44:07 distler Exp $
 */

#ifndef __ONLINE_H__
#define __ONLINE_H__

struct FocalCoord {
    float x;
    float th;
    float y;
    float ph;
};

struct TargetCoord {
    float dp;
    float th;
    float y0;
    float ph;
    float len;
};

struct Vdc {
    int time, wire, multi, error, pattern;
    float x, slope, chi_sqr;
};

struct HDCchamber {
  int error, wire, goodness, OEbit;
  float difftime, diffwire, drifttime;
  float coord, driftdist, rawcoord;
};

struct Hdc {
  struct HDCchamber chamber[4];
  int multi, pattern, OEpattern;
  float x, slope, chi_sqr;
};

struct ParticleCoord {
  int id;
  float mass;
  float Ekin;
  float momentum;
  float fourvec[4];
};

struct SpectrometerOHIPS {
  struct FocalCoord  chamber, chamberError;
  struct FocalCoord  focal,   focalError;
  struct TargetCoord target;
  struct Vdc u1, v1, u2, v2;
  struct ParticleCoord particle;
};

struct SpectrometerOOPS {
  struct FocalCoord  chamber, chamberError;
  struct FocalCoord  focal,   focalError;
  struct TargetCoord target;
  struct Hdc x, y;
  struct ParticleCoord particle;
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
};

struct online {
    int nr;
    float IntegratedLuminosity;
    float RunTime;
    int datataking;
    struct SpectrometerOHIPS ohips;
    struct SpectrometerOOPS  oopsA, oopsB, oopsC, oopsD;
    struct BeamInfo beam;
};

extern struct online online;

#endif /* __ONLINE_H__ */
