/*                                                                      -*-c-*-
 * the guy to blame:
 *
 * Michael O. Distler                        mailto:distler@mit.edu
 * MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
 * Cambridge, MA 02139                       fax    (617) 258-5440
 *
 * $Header: /tmp/cvsroot/Cola/ColaMIT/rundatabase.h,v 2.4 2005-05-17 10:21:24 distler Exp $
 *
 * run database  structure
 */

#ifndef __RUNDATABASE_H__
#define __RUNDATABASE_H__

struct TARGET { 
  struct OFFSET { double x, y, z; };
  /* Tg position offsets in [mm] for reconstruction */ struct OFFSET offset;
  /* Tg position offsets in [mm] for simulation */     struct OFFSET offset_sim;
  /* Tg length [g/cm**3] (Solidstate) */              double length;
  /* Tg angle [deg] */                                double angle;
  /* Tg density [g/cm**3] */                          double density;
  /* Tg wall thickness [mm] */                        double wallthickness;
  /* Tg snow thickness [mm] */                        double snowthickness; 
  /* Tg snow density [g/cm**3] (default 0.4) */       double snowdensity;
  /* Vertex resolution [mm] */                        double vertexresolution;
  /* External radiation switch (15=all) */            double externalRadiation;
  /* Internal radiation switch  (1=on) */             double internalRadiation;
};

struct HDC {
  struct {
    char * initfile;
  } x[4], y[4];
};

struct TMA {
  char * initfile;
};

struct TCC {
  char * initfile;
};

struct OOPS {
  struct HDC hdc;
  struct TMA tma;
  struct TCC tcc;
  double flipped;
  double angle;
  double oop_angle;
  double momentum;
  double drift_distance;
  double FPparam[6];
  double scint_thr[3];
  double bad_planes[6];
  /** Resolution for simulation               */ double angularResolution;
  /** Different error for vertical angle      */ double verticalAngleRatio; 
  /** Resolution for simulation               */ double momResolution;
  /** 2nd Resolution for simulation           */ double angularResolution2; 
  /** 2nd Resolution for simulation           */ double momResolution2;
  /** Number of dp-Intervals                  */ double ndpcuts; 
  /** dp-Intervals for larger errors          */ double dpcuts[20]; 
  /** Probability for larger error (angles)   */ double angularRatio[20]; 
  /** Probability for larger error (momentum) */ double momRatio[20]; 
  /** vaccum scattering chamber connected?    */ double vacuum;
  /** collimator name from Colli.cc */           char   *collimator;
};

struct OHIPS {
  struct TMA tma;
  struct TCC tcc;
  double angle;
  double momentum;
  double drift_distance;
  double FPparam[6];
  double VDCparam[4];
  double dtcorr1[4];
  double dtcorr2[4];
  double drift_velocity;
  double cerenkov_thr[3];
  double pbg_offset[18];
  double pbg_gmfactor[18];
  double pbg_tbfactor;
  double S1_tdc_offset[4];
  /** Resolution for simulation               */ double angularResolution;
  /** Different error for vertical angle      */ double verticalAngleRatio; 
  /** Resolution for simulation               */ double momResolution;
  /** 2nd Resolution for simulation           */ double angularResolution2; 
  /** 2nd Resolution for simulation           */ double momResolution2;
  /** Number of dp-Intervals                  */ double ndpcuts; 
  /** dp-Intervals for larger errors          */ double dpcuts[20]; 
  /** Probability for larger error (angles)   */ double angularRatio[20]; 
  /** Probability for larger error (momentum) */ double momRatio[20]; 
  /** vaccum scattering chamber connected?    */ double vacuum;
  /** collimator name from Colli.cc */           char   *collimator;
};

struct TOF {
  double min;
  double center;
  double max;
};

struct COINC {
  double O_tdc[2]; // OHIPS TDC offset and slope
  double A_tdc[2]; // OOPSA TDC offset and slope
  double B_tdc[2]; // OOPSB TDC offset and slope
  double C_tdc[2]; // OOPSC TDC offset and slope
  double D_tdc[2]; // OOPSD TDC offset and slope
};

struct Bpm {
  /** */ double x0;
  /** */ double dx;
  /** */ double y0;
  /** */ double dy;
};

struct BEAM {  
  /** Beam position offsets in [mm] */ 
  struct OFFSET {
    /** Beam position offset in x-direction in [mm] (beam right) */ double x;
    /** Beam position offset in y-direction in [mm] (beam left)  */ double y;
  };
  /** */ struct OFFSET offset;
};

struct SIMUL {
  double CMSTheta[2];
  double CMSPhi[2];
  double DecayTheta[2];
  double DecayPhi[2];
  double DecayMass[2];
  /** Simulated q2 of the reaction in [GeV²\c²]*/ double q2;
  /** Simulated q2 range in [GeV²\c²] */          double q2_range;
  /** Wobbler amplitude x (horizontally) in [mm] */     double wobx;
  /** Wobbler amplitude y (vertically) in [mm] */       double woby;
  double excitation;
  /** Cut off for generating 1/k Brems-spectrum [GeV]*/ double RadiationCutOff;
  /** Minimum photon lab momentum for VCS            */ double BHmin;
  /** Maximum photon lab momentum for VCS            */ double BHmax;
  /** Maximum weight for Neumann algorithm           */ double maxWeight;
};

typedef struct RUN {
  char *setup;
  char *reaction;
  char *target;
  double Ebeam;
  struct BEAM beam;
  double Etarget;
  struct TARGET Target;
  double helicity_flipped;
  struct OOPS  oopsA, oopsB, oopsC, oopsD;
  struct OHIPS ohips;
  struct TOF tof;
  struct COINC Coinc;
  struct SIMUL sim;
  double BPM_BIC_G_scale, FC_BIC_G_scale, FC_BIC_UG_scale;
  double BPM_BIC_G_ped, FC_BIC_G_ped, FC_BIC_UG_ped;
  /**                                */ double TargetpositionIsKnown;
  /** Which spec. determines vertex? */ char   *use_vertex;
  struct Bpm bpm[2];
} rundatabase;

#ifdef __RUNPARSER__

char *TZ_ENV = "TZ=EST5EDT";

rundatabase rundb;

struct {
    int type; 
    char *Name; 
    char *sql;
    void *addr;
} lname[] = {
    { -1, "Setup",              "", &rundb.setup                   },
    { -1, "Reaction",           "", &rundb.reaction                },
    {  1, "Ebeam",              "", &rundb.Ebeam                   },
    {  1, "Simul.q2",           "", &rundb.sim.q2                  },
    {  1, "Simul.q2.Range",     "", &rundb.sim.q2_range            },
    {  1, "Simul.Excitation",   "", &rundb.sim.excitation          },
    {  2, "Simul.CMS.Theta",    "", &rundb.sim.CMSTheta            },
    {  2, "Simul.CMS.Phi",      "", &rundb.sim.CMSPhi              },
    {  2, "Simul.Decay.Theta",  "", &rundb.sim.DecayTheta          },
    {  2, "Simul.Decay.Phi",    "", &rundb.sim.DecayPhi            },
    {  2, "Simul.Decay.Mass",   "", &rundb.sim.DecayMass           },
    {  1, "Simul.Wobbler.x",    "", &rundb.sim.wobx                },
    {  1, "Simul.Wobbler.y",    "", &rundb.sim.woby                },
    {  1, "Simul.Radiation.Cutoff", "", &rundb.sim.RadiationCutOff },
    {  1, "Simul.BH.min",       "", &rundb.sim.BHmin               },
    {  1, "Simul.BH.max",       "", &rundb.sim.BHmax               },
    {  1, "Simul.maxWeight",    "", &rundb.sim.maxWeight           },
    {  1, "Etarget",            "", &rundb.Etarget                 },
    { -1, "Target",	 	"", &rundb.target                  },
    {  1, "Target.Offset.x",    "", &rundb.Target.offset.x         },
    {  1, "Target.Offset.y",    "", &rundb.Target.offset.y         },
    {  1, "Target.Offset.z",    "", &rundb.Target.offset.z         },
    {  1, "Target.SimOffset.x", "", &rundb.Target.offset_sim.x     },
    {  1, "Target.SimOffset.y", "", &rundb.Target.offset_sim.y     },
    {  1, "Target.SimOffset.z", "", &rundb.Target.offset_sim.z     },
    {  1, "Target.VertexRes",   "", &rundb.Target.vertexresolution },
    {  1, "Target.InternRad",   "", &rundb.Target.internalRadiation},
    {  1, "Target.ExternRad",   "", &rundb.Target.externalRadiation},
    {  1, "Target.Length", 	"", &rundb.Target.length           },
    {  1, "Target.Angle", 	"", &rundb.Target.angle            },
    {  1, "Target.Density", 	"", &rundb.Target.density          },
    {  1, "Target.WallThickness","", &rundb.Target.wallthickness   },
    {  1, "Target.SnowThickness","", &rundb.Target.snowthickness   },
    {  1, "Target.SnowDensity", "", &rundb.Target.snowdensity      },
    /* {  1, "Target",             "", &rundb.Target                  }, */
    {  1, "Helicity",           "", &rundb.helicity_flipped        },
    { -1, "OOPSA.collimator",   "", &rundb.oopsA.collimator        },
    { -1, "OOPSA.hdc.1.x.ini",  "", &rundb.oopsA.hdc.x[1].initfile }, 
    { -1, "OOPSA.hdc.1.y.ini",  "", &rundb.oopsA.hdc.y[1].initfile }, 
    { -1, "OOPSA.hdc.2.x.ini",  "", &rundb.oopsA.hdc.x[2].initfile }, 
    { -1, "OOPSA.hdc.2.y.ini",  "", &rundb.oopsA.hdc.y[2].initfile }, 
    { -1, "OOPSA.hdc.3.x.ini",  "", &rundb.oopsA.hdc.x[3].initfile }, 
    { -1, "OOPSA.hdc.3.y.ini",  "", &rundb.oopsA.hdc.y[3].initfile }, 
    { -1, "OOPSA.tma.initfile", "", &rundb.oopsA.tma.initfile      }, 
    { -1, "OOPSA.tcc.initfile", "", &rundb.oopsA.tcc.initfile      }, 
    {  1, "OOPSA.angle",        "", &rundb.oopsA.angle             }, 
    {  1, "OOPSA.oop.angle",    "", &rundb.oopsA.oop_angle         }, 
    {  1, "OOPSA.momentum",     "", &rundb.oopsA.momentum          }, 
    {  1, "OOPSA.drift.dist",   "", &rundb.oopsA.drift_distance    }, 
    {  1, "OOPSA.drift.dist",   "", &rundb.oopsA.drift_distance    }, 
    {  1, "OOPSA.flipped",      "", &rundb.oopsA.flipped           }, 
    {  6, "OOPSA.fp.param",     "", &rundb.oopsA.FPparam           }, 
    {  3, "OOPSA.proton.thr",   "", &rundb.oopsA.scint_thr         }, 
    {  6, "OOPSA.bad_planes",   "", &rundb.oopsA.bad_planes        }, 
    {  1, "OOPSA.momResolution",  "", &rundb.oopsA.momResolution   },
    {  1, "OOPSA.momResolution2", "", &rundb.oopsA.momResolution2  },
    {  1, "OOPSA.angResolution",  "", &rundb.oopsA.angularResolution },
    {  1, "OOPSA.angResolution2", "", &rundb.oopsA.angularResolution2 },
    { 20, "OOPSA.dpcuts",         "", &rundb.oopsA.dpcuts          },
    { 20, "OOPSA.momRatio",       "", &rundb.oopsA.momRatio        },
    { 20, "OOPSA.angRatio",       "", &rundb.oopsA.angularRatio    },
    {  1, "OOPSA.vertAngleRatio", "", &rundb.oopsA.verticalAngleRatio },
    { -1, "OOPSB.collimator",   "", &rundb.oopsB.collimator        },
    { -1, "OOPSB.hdc.1.x.ini",  "", &rundb.oopsB.hdc.x[1].initfile }, 
    { -1, "OOPSB.hdc.1.y.ini",  "", &rundb.oopsB.hdc.y[1].initfile }, 
    { -1, "OOPSB.hdc.2.x.ini",  "", &rundb.oopsB.hdc.x[2].initfile }, 
    { -1, "OOPSB.hdc.2.y.ini",  "", &rundb.oopsB.hdc.y[2].initfile }, 
    { -1, "OOPSB.hdc.3.x.ini",  "", &rundb.oopsB.hdc.x[3].initfile }, 
    { -1, "OOPSB.hdc.3.y.ini",  "", &rundb.oopsB.hdc.y[3].initfile }, 
    { -1, "OOPSB.tma.initfile", "", &rundb.oopsB.tma.initfile      }, 
    { -1, "OOPSB.tcc.initfile", "", &rundb.oopsB.tcc.initfile      }, 
    {  1, "OOPSB.angle",        "", &rundb.oopsB.angle             }, 
    {  1, "OOPSB.oop.angle",    "", &rundb.oopsB.oop_angle         }, 
    {  1, "OOPSB.momentum",     "", &rundb.oopsB.momentum          }, 
    {  1, "OOPSB.drift.dist",   "", &rundb.oopsB.drift_distance    }, 
    {  1, "OOPSB.flipped",      "", &rundb.oopsB.flipped           }, 
    {  6, "OOPSB.fp.param",     "", &rundb.oopsB.FPparam           }, 
    {  3, "OOPSB.proton.thr",   "", &rundb.oopsB.scint_thr         }, 
    {  6, "OOPSB.bad_planes",   "", &rundb.oopsB.bad_planes        }, 
    {  1, "OOPSB.momResolution",  "", &rundb.oopsB.momResolution   },
    {  1, "OOPSB.momResolution2", "", &rundb.oopsB.momResolution2  },
    {  1, "OOPSB.angResolution",  "", &rundb.oopsB.angularResolution },
    {  1, "OOPSB.angResolution2", "", &rundb.oopsB.angularResolution2 },
    { 20, "OOPSB.dpcuts",         "", &rundb.oopsB.dpcuts          },
    { 20, "OOPSB.momRatio",       "", &rundb.oopsB.momRatio        },
    { 20, "OOPSB.angRatio",       "", &rundb.oopsB.angularRatio    },
    {  1, "OOPSB.vertAngleRatio", "", &rundb.oopsB.verticalAngleRatio },
    { -1, "OOPSC.collimator",   "", &rundb.oopsC.collimator        },
    { -1, "OOPSC.hdc.1.x.ini",  "", &rundb.oopsC.hdc.x[1].initfile }, 
    { -1, "OOPSC.hdc.1.y.ini",  "", &rundb.oopsC.hdc.y[1].initfile }, 
    { -1, "OOPSC.hdc.2.x.ini",  "", &rundb.oopsC.hdc.x[2].initfile }, 
    { -1, "OOPSC.hdc.2.y.ini",  "", &rundb.oopsC.hdc.y[2].initfile }, 
    { -1, "OOPSC.hdc.3.x.ini",  "", &rundb.oopsC.hdc.x[3].initfile }, 
    { -1, "OOPSC.hdc.3.y.ini",  "", &rundb.oopsC.hdc.y[3].initfile }, 
    { -1, "OOPSC.tma.initfile", "", &rundb.oopsC.tma.initfile      }, 
    { -1, "OOPSC.tcc.initfile", "", &rundb.oopsC.tcc.initfile      }, 
    {  1, "OOPSC.angle",        "", &rundb.oopsC.angle             }, 
    {  1, "OOPSC.oop.angle",    "", &rundb.oopsC.oop_angle         }, 
    {  1, "OOPSC.momentum",     "", &rundb.oopsC.momentum          }, 
    {  1, "OOPSC.drift.dist",   "", &rundb.oopsC.drift_distance    }, 
    {  1, "OOPSC.flipped",      "", &rundb.oopsC.flipped           }, 
    {  6, "OOPSC.fp.param",     "", &rundb.oopsC.FPparam           }, 
    {  3, "OOPSC.proton.thr",   "", &rundb.oopsC.scint_thr         }, 
    {  6, "OOPSC.bad_planes",   "", &rundb.oopsC.bad_planes        }, 
    {  1, "OOPSC.momResolution",  "", &rundb.oopsC.momResolution   },
    {  1, "OOPSC.momResolution2", "", &rundb.oopsC.momResolution2  },
    {  1, "OOPSC.angResolution",  "", &rundb.oopsC.angularResolution },
    {  1, "OOPSC.angResolution2", "", &rundb.oopsC.angularResolution2 },
    { 20, "OOPSC.dpcuts",         "", &rundb.oopsC.dpcuts          },
    { 20, "OOPSC.momRatio",       "", &rundb.oopsC.momRatio        },
    { 20, "OOPSC.angRatio",       "", &rundb.oopsC.angularRatio    },
    {  1, "OOPSC.vertAngleRatio", "", &rundb.oopsC.verticalAngleRatio },
    { -1, "OOPSD.collimator",   "", &rundb.oopsD.collimator        },
    { -1, "OOPSD.hdc.1.x.ini",  "", &rundb.oopsD.hdc.x[1].initfile }, 
    { -1, "OOPSD.hdc.1.y.ini",  "", &rundb.oopsD.hdc.y[1].initfile }, 
    { -1, "OOPSD.hdc.2.x.ini",  "", &rundb.oopsD.hdc.x[2].initfile }, 
    { -1, "OOPSD.hdc.2.y.ini",  "", &rundb.oopsD.hdc.y[2].initfile }, 
    { -1, "OOPSD.hdc.3.x.ini",  "", &rundb.oopsD.hdc.x[3].initfile }, 
    { -1, "OOPSD.hdc.3.y.ini",  "", &rundb.oopsD.hdc.y[3].initfile }, 
    { -1, "OOPSD.tma.initfile", "", &rundb.oopsD.tma.initfile      }, 
    { -1, "OOPSD.tcc.initfile", "", &rundb.oopsD.tcc.initfile      }, 
    {  1, "OOPSD.angle",        "", &rundb.oopsD.angle             }, 
    {  1, "OOPSD.oop.angle",    "", &rundb.oopsD.oop_angle         }, 
    {  1, "OOPSD.momentum",     "", &rundb.oopsD.momentum          }, 
    {  1, "OOPSD.drift.dist",   "", &rundb.oopsD.drift_distance    }, 
    {  1, "OOPSD.flipped",      "", &rundb.oopsD.flipped           }, 
    {  6, "OOPSD.fp.param",     "", &rundb.oopsD.FPparam           }, 
    {  3, "OOPSD.proton.thr",   "", &rundb.oopsD.scint_thr         }, 
    {  6, "OOPSD.bad_planes",   "", &rundb.oopsD.bad_planes        }, 
    {  1, "OOPSD.momResolution",  "", &rundb.oopsD.momResolution   },
    {  1, "OOPSD.momResolution2", "", &rundb.oopsD.momResolution2  },
    {  1, "OOPSD.angResolution",  "", &rundb.oopsD.angularResolution },
    {  1, "OOPSD.angResolution2", "", &rundb.oopsD.angularResolution2 },
    { 20, "OOPSD.dpcuts",         "", &rundb.oopsD.dpcuts          },
    { 20, "OOPSD.momRatio",       "", &rundb.oopsD.momRatio        },
    { 20, "OOPSD.angRatio",       "", &rundb.oopsD.angularRatio    },
    {  1, "OOPSD.vertAngleRatio", "", &rundb.oopsD.verticalAngleRatio },
    { -1, "OHIPS.collimator",   "", &rundb.ohips.collimator        },
    { -1, "OHIPS.tma.initfile", "", &rundb.ohips.tma.initfile      }, 
    { -1, "OHIPS.tcc.initfile", "", &rundb.ohips.tcc.initfile      }, 
    {  1, "OHIPS.angle",        "", &rundb.ohips.angle             }, 
    {  1, "OHIPS.momentum",     "", &rundb.ohips.momentum          }, 
    {  1, "OHIPS.drift.dist",   "", &rundb.ohips.drift_distance    }, 
    {  4, "OHIPS.vdc.dtcorr1",  "", &rundb.ohips.dtcorr1           },
    {  4, "OHIPS.vdc.dtcorr2",  "", &rundb.ohips.dtcorr2           },
    {  6, "OHIPS.fp.param",     "", &rundb.ohips.FPparam           }, 
    {  4, "OHIPS.vdc.param",    "", &rundb.ohips.VDCparam          }, 
    {  1, "OHIPS.vdc.velocity", "", &rundb.ohips.drift_velocity    }, 
    {  3, "OHIPS.electron.thr", "", &rundb.ohips.cerenkov_thr      }, 
    {  1, "OHIPS.momResolution",  "", &rundb.ohips.momResolution   },
    {  1, "OHIPS.momResolution2", "", &rundb.ohips.momResolution2  },
    {  1, "OHIPS.angResolution",  "", &rundb.ohips.angularResolution },
    {  1, "OHIPS.angResolution2", "", &rundb.ohips.angularResolution2 },
    { 20, "OHIPS.dpcuts",         "", &rundb.ohips.dpcuts          },
    { 20, "OHIPS.momRatio",       "", &rundb.ohips.momRatio        },
    { 20, "OHIPS.angRatio",       "", &rundb.ohips.angularRatio    },
    {  1, "OHIPS.vertAngleRatio", "", &rundb.ohips.verticalAngleRatio },
    {  4, "OHIPS.S1_tdc_offset", "", &rundb.ohips.S1_tdc_offset },
    {  1, "TOF.cut.min",        "", &rundb.tof.min                 }, 
    {  1, "TOF.center",         "", &rundb.tof.center              }, 
    {  1, "TOF.cut.max",        "", &rundb.tof.max                 }, 
    {  2, "COINC.O_tdc",        "", &rundb.Coinc.O_tdc             },
    {  2, "COINC.A_tdc",        "", &rundb.Coinc.A_tdc             },
    {  2, "COINC.B_tdc",        "", &rundb.Coinc.B_tdc             },
    {  2, "COINC.C_tdc",        "", &rundb.Coinc.C_tdc             },
    {  2, "COINC.D_tdc",        "", &rundb.Coinc.D_tdc             },
    {  1, "FC_BIC_G_scale",     "", &rundb.FC_BIC_G_scale          },
    {  1, "FC_BIC_UG_scale",    "", &rundb.FC_BIC_UG_scale         },
    {  1, "BPM_BIC_G_scale",    "", &rundb.BPM_BIC_G_scale         },
    {  1, "FC_BIC_G_ped",       "", &rundb.FC_BIC_G_ped            },
    {  1, "FC_BIC_UG_ped",      "", &rundb.FC_BIC_UG_ped           },
    {  1, "BPM_BIC_G_ped",      "", &rundb.BPM_BIC_G_ped           },
    {  1, "Beam.Offset.x",      "", &rundb.beam.offset.x           },
    {  1, "Beam.Offset.y",      "", &rundb.beam.offset.y           },
    { -1, "use_vertex",         "", &rundb.use_vertex              },
    {  1, "BPM0_x_offset",      "", &rundb.bpm[0].x0               },
    {  1, "BPM0_x_slope",       "", &rundb.bpm[0].dx               },
    {  1, "BPM0_y_offset",      "", &rundb.bpm[0].y0               },
    {  1, "BPM0_y_slope",       "", &rundb.bpm[0].dy               },
    {  1, "BPM1_x_offset",      "", &rundb.bpm[1].x0               },
    {  1, "BPM1_x_slope",       "", &rundb.bpm[1].dx               },
    {  1, "BPM1_y_offset",      "", &rundb.bpm[1].y0               },
    {  1, "BPM1_y_slope",       "", &rundb.bpm[1].dy               },
    { 18, "OHIPS.pbg.offset"  , "", &rundb.ohips.pbg_offset        },
    { 18, "OHIPS.pbg.gmfactor", "", &rundb.ohips.pbg_gmfactor      },
    {  1, "OHIPS.pbg.tbfactor", "", &rundb.ohips.pbg_tbfactor      },
    {  0, NULL, NULL, 0 }
};

#else /* __RUNPARSER__ */

extern rundatabase rundb;

extern struct {
    int type;
    char *Name;
    char *sql;
    void *addr;
} lname[];

#endif /* __RUNPARSER__ */

#include <time.h>
void readSQLdatabase(const char *runname, const char *setup,
		     const char *host, const char *dbname);
void readRunDatabase(const char *name, const char *runname, 
		     const char *setup, time_t time);

#endif /* __RUNDATABASE_H__ */
