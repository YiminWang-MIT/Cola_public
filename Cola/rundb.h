struct VDC
{ double 
    offset[4],
    driftvelocity;
};

struct CERENKOV
{ double pedestal[12];
  double scale[12];
  double offset[12];
};

struct SCINTILATOR
{ double use_dE;
  double dE[16];
  double ToF[16];
  double walk[3];
};

struct WOBBLER 
{ double dx, x0, dy, y0;
};

struct TMA
{ double x,y,theta,phi;
  char *filename;
  char *firstorder;
    int type;
};

struct SPECTROMETER
{ double 
    angle, left, angularResolution,
    momentum, momResolution, de_tof_tdc_resolution;
  struct SCINTILATOR scint; 
  struct CERENKOV cerenkov;
  struct TMA tma;
  struct WOBBLER wobbler;
  struct VDC vdc;
  char *collimator;
};

struct RUN
{ struct SPECTROMETER A, B, C;
  char   *setup;
  char   *reaction;
  char   *target;
  char   *use_vertex;
  char   *use_wobbler_adc;
  char   *wobbler_corr;
  char   *focus_corr;
  double targetlength, targetangle, vertexresolution;
  double TargetpositionIsKnown;
  double Ebeam;
  double q2, q2_range;
  double tdc_resolution;
  double wobx, woby;
  double use_ab_tdc;
  double use_ac_tdc;
  double use_bc_tdc;
  double use_a_tdc;
  double use_b_tdc;
  double use_c_tdc;
  double coincidencetime;
  double coincidencetime_AB;
  double coincidencetime_CB;
  double coincidencetime_CA;
  double singletime;
  double singletime_A;
  double singletime_B;
  double singletime_C;
  char *additional_title;
};











