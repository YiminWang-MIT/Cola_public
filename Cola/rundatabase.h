//                                                                    -*-C++-*-
// $Id: rundatabase.h 2683 2016-12-09 14:00:15Z aesser $
//
// prescaler, TDC usw noch nicht auf D angepaßt!!!!

#ifndef __RUNDATABASE_H__
#define __RUNDATABASE_H__

/** @name Run database
 * 
 *  @doc The data structure "rundb" is used to store all setup information
 *  This file is read from run.db or a file given by the environment variable
 *  \TEX{RUN\_DB}
 *  It can be accessed in a "*.col" file via 
 *  \TEX{\tt Scalar variable = @NAME} 
 *  where NAME is the string given in run.db
 */ 
/*@{*/

/** Focal Plane polarimeter */ 
struct FPP {
  /**rel. theta-rot. of HDC to VDC (mrad, around 785) */ double hdc_theta_offs;
  /**rel. phi - rot. of HDC to VDC (mrad, around HDC x) */ double hdc_phi_offs;
  /**rel. psi - rot. of HDC to VDC (mrad, around HDC z) */ double hdc_psi_offs;
  /**x of HDC origin in the VDC-system (mm, around 650) */ double hdc_x_offs;
  /**y of HDC origin in the VDC-system (mm, around 0)   */ double hdc_y_offs;
  /**z of HDC origin in the VDC-system (mm, ar. 2500)   */ double hdc_z_offs;
  /**full thickness of carbon analyzer block (cm) */ double CarbonThickness;
};

/** Parameters for drifttime-space relation: */ 
struct DPARA {
  /**linear slope for negative driftimes */    double negdtslope;
  /**upper end (edge) of pos-time spectrum */  double bigdtlim;
  /**linear slope for driftimes greater than BIGDTLIM */ double bigdtslope;
  /**parameters for 8. order polynom */        double dtpara[9]; 
};

/** Horizontal drift chamber of the FPP*/
struct HDC {
  /** */ DPARA u1;
  /** */ DPARA v1;
  /** */ DPARA u2;
  /** */ DPARA v2;
  /** Configuration file */ char *configfile;
};

/** A layer of the Vertical drift chamber */ 
struct VDCLAYER {
  /** Array of diabled wires */
  double disabled[16];
  /** Array of drift time offsets for each paddle */
  double paddle[16];
};

/** Track Recognition Enhancement for VDCs */
struct VDC_IGNORE_CRITERIA { 
  /** mode: 0 not active, 1 active */ double mode;
  /** min/max drift time difference required between neighbored wires
      and walk of drift time difference (cf. angle th) with wire number */
  double cut[3];
};

/** Vertical drift chamber */ 
struct VDC { 
  /** Layers             */ VDCLAYER x1, s1, x2, s2;
  /** drift time offsets */ double   offset[4];
  /** drift velocity     */ double   driftvelocity;
  /** minimum drift path */ double   mindrift;
  /** drift vel. correction factor for s planes */  double corr_driftvel;
  /** drift time difference criterion x-layers*/ VDC_IGNORE_CRITERIA x;
  /** drift time difference criterion s-layers*/ VDC_IGNORE_CRITERIA s;
  /** Configuration      */ char     *initfile;
  /** Directory in which track ps-files are written */ char *trackps;
};

/** Pion Drift Chamber (SOS) */
struct PDC {
  /** Spline Nodes for X plane */ char *x_snofile;
  /** Spline Nodes for Y plane */ char *y_snofile;
};

/** Cerenkov detektor*/
struct CERENKOV { 
  /** Pedestals for cut */ double pedestal[12];
  /** linear Scale      */ double scale[12];
  /** linear offset     */ double offset[12];
  double FloatingPed; 
  double FloatingEvents;
  char *PedFile;
};

/** Scintillator layer */
struct SCINTILATOR { 
  /** Use dE Layer or ToF for timing? */    double use_dE;
  /** Paddle time offsets dE */             double dE[16];
  /** Paddle time offsets ToF*/             double ToF[16];
  /** Paddle time offsets dE(left tube)*/   double dE_Left[16];
  /** Paddle time offsets ToF(left Tube)*/  double ToF_Left[16];
  /** Pulseheight correction */             double walk[3];
  /** ADC offset for dE (left) */           double dE_corr_left_offset[16];
  /** ADC scale for dE (left) */            double dE_corr_left_scale[16];
  /** ADC offset for dE (right) */          double dE_corr_right_offset[16];
  /** ADC scale for dE (right) */           double dE_corr_right_scale[16];
  /** ADC offset for ToF (left) */          double ToF_corr_left_offset[16];
  /** ADC scale for ToF (left) */           double ToF_corr_left_scale[16];
  /** ADC offset for ToF (right) */         double ToF_corr_right_offset[16];
  /** ADC scale for ToF (right) */          double ToF_corr_right_scale[16];

  /** channels between 0 and this number = 0 */ double MinAdcOverThresholdValue;
  /** choose if you wanna use new routines*/ double UseNewHandle;

};

/** Target parameters */
struct TARGET { 
  struct OFFSET {double x,y,z;};
  /** Target position offsets in [mm] from reconstruction */ struct OFFSET offset;
  /** Target position offsets in [mm] for simulation */ struct OFFSET offset_sim;
  /** Target length in [mg/cm*2] (Solidstate/Waterfall) or [mm] (helium,cryo_ewald) */ 
  double length;
  /** extended target size **/ double totallength, totallength2; double lengthratio;
  /** High/Low flow for gas jet target **/ double flow;
  /** Target angle in [degrees] */       double angle;
  /** Target density in [g/cm**3] */     double density;
  /** Target wall thickness in [mm] */   double wallthickness;
  /** Target snow thickness in [mm] */   double snowthickness; 
  /** Target snow density in [g/cm**3] (Default: 0.4 g/cm**3) */ 
  double snowdensity;
  /** Target snow thickness parametrized? */
  double snowparam;
  /** Parameters for wall thickness **/
  double wallparam[3];
  char *wallfile;
  /** Vertex resolution in [mm] */       double vertexresolution;
  /** Vertex2 resolution in [mm] */         double vertexresolution2;  
  /** Vertex resolution ratio */            double vertexratio;
  /** External radiation switch (15=all) */ double externalRadiation;
  /** Internal radiation switch   (1=on) */ double internalRadiation;
  /** Target polarisation components     */ double Polarisation[3];
  /** Vertex distribution inside solidstate (1=yes) */ double vertexdistribution;
};

/** */ 
struct WOBBLER {
  /** */ double dx;
  /** */ double x0;
  /** */ double dy;
  /** */ double y0;
};

/** Beam parameters */
struct BEAM {  
  /** Beam position offsets in [mm] */ 
  struct OFFSET {
    /** Beam position offset in x-direction in [mm] (towards C) */ double x;
    /** Beam position offset in y-direction in [mm] (downwards) */ double y;
  };
  /** */ struct OFFSET offset;
  /** longitudinal Polarisation of electron beam */ double P_e;
};

/** */ 
struct TMA { 
  /** */ double x,y,theta,phi;
  /** */ char *filename;
  /** */ char *firstorder;
  /** */ double type;
};

/** Neutron detector */
struct NDET { 
  /** up/down pairs of TDC offsets   */ double offset[72];
  /** up/down pairs of ADC pedestals */ double pedestal[72];
  /** */ double position[36];
  /** */ double timeAtTarget[36];
  /** */ double angle;
  /** */ double oop_angle;
  /** distance colli-target */ double distance;
  /** height of the colli   */ double height;
  /** width of the colli    */ double width;
  /** callibration data     */ char *filename;
  /**bss:latest position of the (flasher pedestal) */ double flasherpedestal[72];
  /**bss:latest pos of the (flasher peak) */ double flasherpeak[72];
  /**bss:latest pos of the adc peak for particles*/ double AdcParticlePeak[72];
  /**bss:parametrization of multi1 gauge curves (x0, y0, R)*/ double TdcMulti1Gauge[90];
};

/** Neutron polarimeter (npol) */
struct NPOL {
//==> Limits for (timedependent) Cuts and configuration
  /** Maximum for Cut on Coincetime               */   double maxcoincT;
  /** Can be removed later ?!?                    */ 
  /** Lower limit for norad cut                   */   double noradlow;
  /** Upper limit for norad cut                   */   double noradup;
  /** amplitude of the adc jitter                 */   double maxjitter;
  /** Minimum accepted ADC values                 */   double minADC[5];
  /** Maximum accepted ADC values                 */   double maxADC[5];
  /** TDC Scale [ns/bin]                          */   double tdcScale[5];
  /** Attenuation factor for x dep. walk pars     */   double mu;
  /** Electron beam polarisation                  */   double beamPol;

//==> Position of the detectors  
  /** Angle between lab and npol z-axis           */   double alpha;
  /** Origin of 1st Wall in npol coord.           */   double W1_origin[3];
  /** Origin of 2nd Wall upper part. (npol coord) */   double Top_origin[3];
  /** Origin of 2nd Wall lower part. (npol coord) */   double Bottom_origin[3];
  /** Position of trigger detector 1 (npol coord) */   double trig1_pos[3];
  /** Position of trigger detector 2 (npol coord) */   double trig2_pos[3];
  /** Position of trigger detector 3 (npol coord) */   double trig3_pos[3];

//==>Configuration files
  /** Filename of position configuration          */   char   *file_position;             
  /** Filename of pedestals                       */   char   *file_pedestal;
  /** Filename of TDC correction and SoL config.  */   char   *file_tdcDiff;
  /** Filename for time (ToF) correction          */   char   *file_tdcSum;
  /** Filename for lambda and deltaQ              */   char   *file_adcRatio;
  /** Filename for walk correction                */   char   *file_tdcWalk;
  /** Filename of TDC cuts configuration          */   char   *file_tdcCut;
  /** Filename of ADC cuts configuration          */   char   *file_adcCut;
  /** Filename of Time cuts configuration         */   char   *file_timeCut;
  /** Filename of Pulsh cuts configuration        */   char   *file_pulshCut;

  
//==> Scaling of multi hits (FK 01/12/18: not used in Cola any more)
  /** Scaling of single hits in ndets 1st wall    */   double W1_single;     
  /** Scaling of double hits in ndets 1st wall    */   double W1_double[2];
  /** Scaling of single hits in ndets 2nd wall    */   double W2_single;
  /** Scaling of double hits in ndets 2nd wall    */   double W2_double[6];
  /** Scaling of triple hits in ndets 2nd wall    */   double W2_triple[3]; 

  /** Use vetos info to determinate the ndet hit ?*/   double veto_use;
  /** Use also the neighbour vetos                */   double n_veto_use;
  /** Allow multiple hits                         */   double usemulti;

//==> Magnet
  /** Current of the Magnet [A]                   */   double magnet_I;
  /** Origin of magnet (front)                    */   double magnet_pos[3];
  /** Search path for fieldintegral files         */   char   *magnet_path;
  /** Lower limit for magnet field integral       */   double Bint_low;
  /** Upper limit for magnet field integral       */   double Bint_up;
};

/** Silicon detector */
struct SILICON {
  /** Configuration file                              */	char *configfile;
  /** Detector angle                                  */	double angle;
  /** Detector distance from target                   */        double distance;
  /** Central momentum for simulation                 */        double simCentMom;
  /** Detector resolution for simulation              */        double simResolution[8];
  /** Detector acceptance for simulation              */        double simAcceptScale;
};

/** Flash ADC */
struct FLASHADC {
  /** Use fADC (yes/no)                               */ 	double use_fadc;
  /** fADC M-constant [sampling periods]              */	double M;
  /** fADC Trapezoid rise time [sampling periods]     */	double trap_RT;
  /** fADC Trapezoid flat top min. [sampling periods] */	double trap_FT_min;
  /** fADC Trapezoid flat top max. [sampling periods] */	double trap_FT_max;
  /** fADC sample where signal actually starts        */	double signal_start;
  /** Use pile-up control/reject                      */	double pileup_control;
  /** Threshold for pile-up control/reject            */	double pileup_threshold;
};

/** Kaos detector */
struct KAOS {
  /** readout prescaler                     */ double Prescaler;
  /** readout scaler for singles            */ double ScalerSingle;
  /** readout scaler for doubles            */ double ScalerDouble;
  /** spectrometer angle [deg]              */ double angle;
  /** momentum [MeV]                        */ double momentum;
  /** resolution for simulation             */ double angularResolution;
  /** resolution for simulation                 */ double momResolution;
  /** 2nd resolution for simulation         */ double angularResolution2; 
  /** 2nd resolution for simulation         */ double momResolution2;
  /** probability for larger error (momentum) */ double momRatio[20]; 
  /**                                       */ struct TMA tma;
  /** collimator name              */   char   *collimator;
  /** acceptance definition file   */   char* acceptance;
  /** trigger definition file      */   char* trigger;

  /** TOF wall analysis (yes/no)   */   double use_tof;    // no TOF: 0, wall F: 1, wall H: 2
  /** MWPC analysis (yes/no)       */	double use_mwpc;   // handle MWPC data?
  /** track reconstruction         */	double use_track;  // MWPC: 1, TOF: 2, TOF 2012: 3, MWPC 2012: 4
  /** multi Track ananlysis        */   double use_multi_track; // handle multiple tracks?
  /** mininmum quality             */   double min_quality;
  /** tagger quality (yes/no)      */	double use_tagger; // use tagger in track selection?
  /** aerogel quality (yes/no)     */	double use_aerogel;// use aerogel in track selection?

  /** focal plane position         */	double fe_pos[5];

  /** detector positions and size  */	double det_pos[50];

  // old co-ordinate system (all values in millimeter/degree):
  //   x in direction of dispersive plane (from MWPC L ADC 0)
  //   y in non-dispersive plane (from mid-plane)
  //   z: perpendicular to MWPCs (from MWPC L pregap plane)
  // 0: MWPC M POSITION xLM
  // 1: MWPC M POSITION zLM
  // 2: WALL F/H POSITION xLF/H 
  // 3: WALL F/H POSITION zLF/H
  // 4: WALL G POSITION xLG
  // 5: WALL G POSITION zLG
  // 6: WALL F/H ANGLE
  // 7: WALL G ANGLE
  // 8: MWPC L POSITION yL
  // 9: MWPC M POSITION yM
  //10: WALL F/H POSITION yF/H
  //11: WALL G POSITION yG
  //12: AEROGEL POSITION yAC
  //13: PADDLE SIZE F/H lF/H
  //14: PADDLE SIZE G lG
  //15: MWPC L POSITION TO BASE POINT
  //16: AEROGEL POSITION xLAC
  //17: AEROGEL POSITION zLAC
  //18: AEROGEL SEGMENT 0 MIN
  //19: AEROGEL SEGMENT 0 MAX
  //20: AEROGEL SEGMENT 1 MIN
  //21: AEROGEL SEGMENT 1 MAX
  //22: AEROGEL SEGMENT 2 MIN
  //23: AEROGEL SEGMENT 2 MAX
  //24: AEROGEL SEGMENT 3 MIN
  //25: AEROGEL SEGMENT 3 MAX
  //26: AEROGEL SEGMENT 4 MIN
  //27: AEROGEL SEGMENT 4 MAX
  //28: AEROGEL SEGMENT 5 MIN
  //29: AEROGEL SEGMENT 5 MAX
  //30: OFFSET CHAMBER L TO MWPC BASELINE

  // 2o12 co-ordinate system (all values in millimeter/degree):
  //   x in direction of dispersive plane (from mark on platform)
  //   y in non-dispersive plane (from mid-plane)
  //   z: perpendicular to dispersive plane (from mark on platform)

  // 0: MWPC M POSITION xM
  // 1: MWPC M POSITION zM
  // 2: WALL H POSITION xH 
  // 3: WALL H POSITION zH
  // 4: WALL G POSITION xG
  // 5: WALL G POSITION zG
  // 6: WALL H ANGLE
  // 7: WALL G ANGLE
  // 8: MWPC L POSITION yL
  // 9: MWPC M POSITION yM
  //10: WALL H POSITION yH
  //11: WALL G POSITION yG
  //12: AC 1 POSITION yAC
  //13: PADDLE SIZE H lH
  //14: PADDLE SIZE G lG
  //15: MWPC L xL
  //16: AC 1 POSITION xAC
  //17: AC 1 POSITION zAC
  //18: AC 1 SEGMENT 0 MIN
  //19: AC 1 SEGMENT 0 MAX
  //20: AC 1 SEGMENT 1 MIN
  //21: AC 1 SEGMENT 1 MAX
  //22: AC 1 SEGMENT 2 MIN
  //23: AC 1 SEGMENT 2 MAX
  //24: AC 1 SEGMENT 3 MIN
  //25: AC 1 SEGMENT 3 MAX
  //26: AC 1 SEGMENT 4 MIN
  //27: AC 1 SEGMENT 4 MAX
  //28: AC 1 SEGMENT 5 MIN
  //29: AC 1 SEGMENT 5 MAX
  //30: MWPC POSITION zL
  //31: MWPC ANGLE
  //32: AC 1 ANGLE
  //33: AC 2 POSITION xAC
  //34: AC 2 POSITION zAC
  //35: AC 2 ANGLE
  //36: AC 2 POSITION yAC
  //37: AC 2 SEGMENT 0 MIN
  //38: AC 2 SEGMENT 0 MAX
  //39: AC 2 SEGMENT 1 MIN
  //40: AC 2 SEGMENT 1 MAX
  //41: AC 2 SEGMENT 2 MIN
  //42: AC 2 SEGMENT 2 MAX
  //43: AC 2 SEGMENT 3 MIN
  //44: AC 2 SEGMENT 3 MAX
  //45: AC 2 SEGMENT 4 MIN
  //46: AC 2 SEGMENT 4 MAX
  //47: AC 2 SEGMENT 5 MIN
  //48: AC 2 SEGMENT 5 MAX

  /* mwpc track reconstruction paramaters */	double track_mwpc[31]; 

  // 0: theta_min
  // 1: theta_max
  // 2: phi_min
  // 3: phi_max
  // 4: slope_max for theta
  // 5: minimum for finding a peak in a cluster
  // 6: sigma quality hit position scintillator walls
  // 7: slope quality Ly vs phi
  // 8: offset quality Ly vs phi
  // 9: sigma quality Ly vs phi
  // 10: plateau quality Ly vs phi
  // 11: slope quality MQx vs MQy
  // 12: offset quality MQx vs MQy
  // 13: sigma quality MQx vs MQy
  // 14: plateau quality MQx vs MQy
  // 15: slope quality LQx vs LQy
  // 16: offset quality LQx vs LQy
  // 17: sigma quality LQx vs LQy
  // 18: plateau quality LQx vs LQy
  // 19: cluster separation
  // 20: cluster minimum width
  // 21: maximum peak right side
  // 22: maximum peak left side
  // 23: --
  // 24: minimum MWPC ADC threshold
  // 25: sigma quality timing
  // 26: plateau quality timing
  // 27: x minimum charge ratio for satellites
  // 28: y minimum charge ratio for satellites
  // 29: minimum charge for increasing width
  // 30: separation threshold

  /* TOF reconstruction parameters */	double track_tof[25]; 

  /* general quality criteria  */

  //  1: gaussian width in H and G (paddles) [used only with MWPC]
  //  2: theta plateau   (°)
  //  3: theta sigma     (°)
  //  4: hit G vs H slope
  //  5: phi vs y offset (°)
  //  6: phi plateau     (°)
  //  7: phi sigma       (°)
  //  8: phi vs y slope  (°/mm)
  //  9: maximum time difference in cluster (ps)
  // 10: maximum size of cluster (paddles)

  /* particle specific quality criteria */

  // 11: dEdxG reference       (MeV/cm)
  // 12: dEdxG plateau         (MeV/cm)
  // 13: dEdxG sigma           (MeV/cm)
  // 14: dEdxG slope vs Hit G  (MeV/cm /paddle)
  // 15: dEdxH reference       (MeV/cm)
  // 16: dEdxH plateau         (MeV/cm)
  // 17: dEdxH sigma           (MeV/cm)
  // 18: dEdxH slope vs Hit H  (MeV/cm /paddle)

  // 19: time-of-flight reference (ns)
  // 20: time-of-flight plateau   (ns)
  // 21: time-of-flight sigma     (ns)
  // 22: time-of-flight slope     (ns/H)

  /** timing offsets               */	double offset[5];
  // 0: --
  // 1: offset of raw time Fastbus TDC H (ns)
  // 2: offset of raw time Fastbus TDC G (ns)
  // 3: offset of coinc time Fastbus TDC H (ns)
  // 4: offset of coinc time SpekB TDC (ns)

  /** ADC thresholds in anlysis    */	double threshold[5];
  // 0: --
  // 1: wall F/H ADC threshold
  // 2: wall G ADC threshold
  // 3: aerogel ADC threshold
  // 4: tagger ADC threshold

  /** ADC scale                    */	double ADC_scale[2];
  /** ADC gain                     */	double ADC_gain[120];
  /** ADC pedestal                 */	double ADC_pedestal[128];
  /** ADC high range correction    */	double ADC_correction[128];
  /** ADC high range correction    */	double ADC_absorption[240];
  /** TDC offset                   */	double TDC_offset[120];
  /** TDC high range correction    */	double TDC_correction[128];
  /** TOF wall time difference     */	double TDC_time_corr[120];
  /** TOF timewalk correction file */   char*  timewalk_corr;
  /** usage of timewalk correction */   double use_timewalk_corr;
  /** MWPC ADC gain                */	double MWPC_gain[620];
  /** TOF trigger check            */	double use_trigger;
  /** Event display files directory*/   char*  MWPC_trackDir;
  /** AC ADC gain                  */   double Aerogel_gain[24];
  /** AC ADC pedestal              */   double Aerogel_pedestal[24]; 
  /** AC ADC high range correction */	double Aerogel_correction[24];
  /** AC TDC offset                */   double Aerogel_offset[24];

  /** vaccum scattering chamber connected?  */ double vacuum;

};

/** fibre detector */
#define FIBRECHNS  4608

struct FIBRE {
  /** fibre sync mapped (1= B, 2=C)*/	double use_fibre;
  /** CATCH TDC offset             */	double TDC_offset[FIBRECHNS];
  /** TDC trigger (channel/offset) */	double trigger[4]; 
      /* trigger chn, offset,
         hadron chn, electron chn  */
  /** parameters for analysis      */	double analysisParameters[5];
  // cluster min size, 
  // cluster separation channels, 
  // cluster separation time, 
  // min angle, 
  // max angle */
  /** Fibre Event Display          */	double eventDisplay[3]; 
  // pages, min time, max time
  /** momentum [MeV]               */ double momentum;
  /**                              */ struct TMA tma;
};

struct NCUBE {
  /** TRB Calibration file         */ char *calibration;
  /** Mapping file                 */ char *mapping;
};

struct NDET2016 {
  /** Enable Ndet analysis         */   double use_ndet;
  /** TDC calibration file name    */   char  *calibrationFile;
  /** Mapping file                 */   char  *mappingFile;
};

/** Spectrometer */ 
struct SPECTROMETER {
  /** Readout Prescaler                     */ double Prescaler;
  /** Readout scaler for singles            */ double ScalerSingle;
  /** Readout scaler for doubles            */ double ScalerDouble;
  /** Readout scaler for strobe             */ double PreStrobe;
  /** PLU Deadtime per event (90ns, formerly 400ns) */ double StrobeDead;
  /** spectrometer angle [deg]              */ double angle;
  /** Is spec on the left side?             */ double left;
  /** momentum [MeV]                        */ double momentum;
  /** Resolution for simulation             */ double angularResolution;
  /** Smaller error for vertical angle (Spec B) */ double verticalAngleRatio; 
  /** Resolution for simulation                 */ double momResolution;
  /** 2nd Resolution for simulation         */ double angularResolution2; 
  /** 2nd Resolution for simulation         */ double momResolution2;
  /** Number of dp-Intervals                */ double ndpcuts; 
  /** dp-Intervals for larger errors        */ double dpcuts[20]; 
  /** Probability for larger error (angles)   */ double angularRatio[20]; 
  /** Probability for larger error (momentum) */ double momRatio[20]; 
  /** Resolution for de_tof TDC             */ double de_tof_tdc_resolution;
  /** vaccum scattering chamber connected?  */ double vacuum;
  /**                                       */ struct SCINTILATOR scint; 
  /**                                       */ struct CERENKOV cerenkov;
  /**                                       */ struct TMA tma;
  /**                                       */ struct WOBBLER wobbler;
  /**                                       */ struct VDC vdc;
  /** collimator name, see Colli.cc for valid colli's */ char   *collimator;
  /** collimator acceptance */ double   collimatoracceptance;
  /** out-of-plane-angle, default = 0       */ double oopangle;
  /** chamber acceptance definition file    */ char* simChamber;
};

/** Simulation parameters */
struct SIMUL 
{
  /** */ double CMSTheta[2];
  /** */ double massA;
  /** */ double CMSPhi[2];
  /** */ double DecayTheta[2];
  /** */ double DecayPhi[2];
  /** */ double DecayMass[2];
  /** Simulated q2 of the reaction in [GeV²\c²]*/      double q2;
  /** Simulated q2 range in [GeV²\c²] */               double q2_range;
  /** Wobbler amplitude x (horizontally) in [mm] */    double wobx;
  /** Wobbler amplitude y (vertically) in [mm] */      double woby;
  /** */ double excitation;
  /** Cut off for generating 1/k Brems-spectrum [GeV]*/ double RadiationCutOff;
  /** Minimum photon lab momentum for VCS            */ double BHmin;
  /** Maximum photon lab momentum for VCS            */ double BHmax;
  /** Maximum weight for Neumann algorithm           */ double maxWeight;
  /** Target polarisation components                 */ double TargetPol[3];
  /** multi purpose generator flag */                   double GeneratorFlag;
};  

/** Complete tree of rundb-entries */
struct RUN
{ /** Spectrometer     */ struct SPECTROMETER A, B, C, D;
  /** neutron detector */ struct NDET N, N2;
  /** neutron detector */ struct NCUBE ncube;
  /** neutron polarimeter     */ struct NPOL npol;
  /** neutron detector 2916   */ struct NDET2016 ndet;
  /** Focal Plane Polarimeter */ struct FPP ppolA;
  /** Horizontal drift chamber */ struct HDC hdcA;
  /** Pion drift chamber       */ struct PDC pdc;
  /** Silicon detector         */ struct SILICON si;
  /** Flash ADC                */ struct FLASHADC fadc;
  /** Fibre detector         */ struct FIBRE SciFi;
  /** Kaos detectors         */ struct KAOS kaos;
  /** Name of setup   */ char   *setup;
  /** Reaction      */ char   *reaction;
  /** Target cell   */ char   *target;
  /** Which spec. determines vertex? */ char   *use_vertex;
  /**            */ char   *use_wobbler_adc;
  /** Use wobbler correction */ char   *wobbler_corr;
  /** Use focus correction */ char   *focus_corr;
  /** Vertex position      */ struct TARGET Target;
  /** */ double TargetpositionIsKnown;
  /** Beam Energy */ double Ebeam;
  /** */ struct BEAM beam;
  /** Target kinetic energy in A*(MeV/u) for colliding beams */ double Etarget;
  /** Number of turns in MAMI B */ double turns;
  /** ns/bin for Coincidence TDC */ double tdc_resolution;
  /** */ double ab_tdc_resolution;
  /** */ double ac_tdc_resolution;
  /** */ double bc_tdc_resolution;
  /** */ double a_tdc_resolution;
  /** */ double b_tdc_resolution;
  /** */ double c_tdc_resolution;
  /** */ double use_ab_tdc;
  /** */ double use_ac_tdc;
  /** */ double use_bc_tdc;
  /** */ double use_a_tdc;
  /** */ double use_b_tdc;
  /** */ double use_c_tdc;
  /** */ double coincidencetime;
  /** */ double coincidencetime_AB;
  /** */ double coincidencetime_CB;
  /** */ double coincidencetime_CA;
  /** */ double coincidencetime_HB;
  /** */ double singletime;
  /** */ double singletime_A;
  /** */ double singletime_B;
  /** */ double singletime_C;
  /** */ char *additional_title;
  /** Joker to pass through the run.db */ double param[40];
  /** Cut on q-omega plot */ double qw_cut[8];
  /** Simulation */ struct SIMUL sim;
  /** parameters not used anymore */ double wastebasket;
};

/*@}*/
typedef struct RUN rundatabase;

#ifdef __RUNPARSER__
//char *TZ_ENV = "TZ=MET-1METDST";
char *TZ_ENV = "TZ=Europe/Berlin";
rundatabase rundb;
struct rundatabasetranslationlist {
  int type; 
  char *Name; /** Name in run.db       */
  char *sql;  /** Name in SQL database */
  void *addr;
} lname[] = { 
  { 1, "Ebeam",	       		"Setup.Ebeam",	        &rundb.Ebeam},
  { 1, "Turns",	       		"Setup.Turns",          &rundb.turns},
  {40, "Param",	      		"Setup.Parameter",  	&rundb.param},
  {8, "qw_cut",	      	        "Setup.qw_cut",  	&rundb.qw_cut},
  {-1, "Setup",	 		"Setup.Setup",	 	&rundb.setup},
  {-1, "Reaction",	 	"Setup.Reaction", 	&rundb.reaction},
  { 1, "TDC_Resolution",	"Setup.TDC_Resolution",	&rundb.tdc_resolution},
  { 1, "AB_TDC_Resolution","Setup.AB_TDC_Resolution",&rundb.ab_tdc_resolution},
  { 1, "BC_TDC_Resolution","Setup.BC_TDC_Resolution",&rundb.bc_tdc_resolution},
  { 1, "AC_TDC_Resolution","Setup.AC_TDC_Resolution",&rundb.ac_tdc_resolution},
  { 1, "A_TDC_Resolution", "Setup.A_TDC_Resolution", &rundb.a_tdc_resolution},
  { 1, "B_TDC_Resolution", "Setup.B_TDC_Resolution", &rundb.b_tdc_resolution},
  { 1, "C_TDC_Resolution", "Setup.C_TDC_Resolution", &rundb.c_tdc_resolution},
  { 1, "use_AB_TDC",	 	"Setup.use_AB_TDC",	 &rundb.use_ab_tdc},
  { 1, "use_AC_TDC",	 	"Setup.use_AC_TDC",	 &rundb.use_ac_tdc},
  { 1, "use_BC_TDC",	       	"Setup.use_BC_TDC",	 &rundb.use_bc_tdc},
  { 1, "use_A_TDC",	 	"Setup.use_A_TDC",	 &rundb.use_a_tdc},
  { 1, "use_B_TDC",	 	"Setup.use_B_TDC",	 &rundb.use_b_tdc},
  { 1, "use_C_TDC",	       	"Setup.use_C_TDC",	 &rundb.use_c_tdc},
  { 20,"N.ADC",                 "Setup.N.ADC",           rundb.N.pedestal},
  { 20,"N.TDC",                 "Setup.N.TDC",           rundb.N.offset},
  { 20,"N2.ADC",                "Setup.N2.ADC",          rundb.N2.pedestal},
  { 20,"N2.TDC",                "Setup.N2.TDC",          rundb.N2.offset},

  { 1, "CoincidenceTime", 	"Coincidence_Time", &rundb.coincidencetime},
  { 1, "ABTime",		"AB_Time",	    &rundb.coincidencetime_AB},
  { 1, "BCTime",		"BC_Time",	    &rundb.coincidencetime_CB},
  { 1, "ACTime",		"AC_Time",	    &rundb.coincidencetime_CA},
  { 1, "HBTime",		"HB_Time",	    &rundb.coincidencetime_HB},
  
  { 1, "SingleTime", 	        "Single_Time",      &rundb.singletime},
  { 1, "ATime",		        "A_Time",	    &rundb.singletime_A},
  { 1, "BTime",		        "B_Time",	    &rundb.singletime_B},
  { 1, "CTime",	                "C_Time",	    &rundb.singletime_C},
  
  { 1, "Beam.Offset.x",       	"Beam.Offset.x",    &rundb.beam.offset.x},
  { 1, "Beam.Offset.y",       	"Beam.Offset.y",    &rundb.beam.offset.y},
  { 1, "Beam.Polarisation",    	"Beam.Polarisation",&rundb.beam.P_e},
  {-1, "use_vertex",            "Setup.use_vertex",  &rundb.use_vertex},
  {-1, "use_wobbler_adc",       "Setup.use_wobbler_adc",  &rundb.use_wobbler_adc},
  {-1, "wobbler_corr",          "Setup.wobbler_corr",  &rundb.wobbler_corr},
  {-1, "focus_corr",            "Setup.focus_corr",  &rundb.focus_corr},
  { 1, "TargetpositionIsKnown", "Setup.TargetpositionIsKnown", &rundb.TargetpositionIsKnown},

  { 1, "Simul.mass.A",	       	"Simul.mass.A",	     &rundb.sim.massA},
  { 1, "Simul.q2",	       	"Simul.q2",	     &rundb.sim.q2},
  { 1, "Simul.q2.Range",	"Simul.q2.Range",    &rundb.sim.q2_range},
  { 1, "Simul.Excitation",	"Simul.Excitation",  &rundb.sim.excitation},
  { 2, "Simul.CMS.Theta",    	"Simul.CMS.Theta",   &rundb.sim.CMSTheta},
  { 2, "Simul.CMS.Phi",       	"Simul.CMS.Phi",     &rundb.sim.CMSPhi},
  { 2, "Simul.Decay.Theta",   	"Simul.Decay.Theta", &rundb.sim.DecayTheta},
  { 2, "Simul.Decay.Phi",     	"Simul.Decay.Phi",   &rundb.sim.DecayPhi},
  { 2, "Simul.Decay.Mass",    	"Simul.Decay.Mass",  &rundb.sim.DecayMass},
  { 1, "Simul.Wobbler.x",	"Simul.Wobbler.x",   &rundb.sim.wobx},
  { 1, "Simul.Wobbler.y",	"Simul.Wobbler.y",   &rundb.sim.woby},
  { 1, "Simul.Radiation.Cutoff","Simul.Radiation.Cutoff", 
    &rundb.sim.RadiationCutOff},
  { 1, "Simul.BH.min",          "Simul.BH.min",      &rundb.sim.BHmin},
  { 1, "Simul.BH.max",          "Simul.BH.max",      &rundb.sim.BHmax},
  { 1, "Simul.maxWeight",       "Simul.maxWeight",   &rundb.sim.maxWeight},
  { 3, "Simul.TargetPol",       "Simul.TargetPol",   &rundb.sim.TargetPol},
  { 1, "Simul.GeneratorFlag",   "Simul.GeneratorFlag",   &rundb.sim.GeneratorFlag},

  {-1, "Additional_Title",      "Additional_Title",  &rundb.additional_title},
  { 1, "Etarget",               "Etarget",           &rundb.Etarget},
  {-1, "Target",	 	"Target.Name",	     &rundb.target},
  { 1, "Target.Offset.x",     	"Target.Offset.x",   &rundb.Target.offset.x},
  { 1, "Target.Offset.y",     	"Target.Offset.y",   &rundb.Target.offset.y},
  { 1, "Target.Offset.z",     	"Target.Offset.z",   &rundb.Target.offset.z},
  { 1, "Target.SimOffset.x",  	"Target.SimOffset.x",
    &rundb.Target.offset_sim.x},
  { 1, "Target.SimOffset.y",  	"Target.SimOffset.y",
    &rundb.Target.offset_sim.y},
  { 1, "Target.SimOffset.z",  	"Target.SimOffset.z",
    &rundb.Target.offset_sim.z},
  { 1, "Target.VertexResolution","Target.VertexResolution",
    &rundb.Target.vertexresolution},
  { 1, "Target.VertexResolution2","Target.VertexResolution2",
    &rundb.Target.vertexresolution2},  
  { 1, "Target.vertexRatio","Target.vertexRatio",
    &rundb.Target.vertexratio}, 
  { 1, "Target.externalRadiation", "Target.externalRadiation", 
    &rundb.Target.externalRadiation},
  { 1, "Target.internalRadiation", "Target.internalRadiation", 
    &rundb.Target.internalRadiation},
  { 1, "Target.Length", 	"Target.Length", 	&rundb.Target.length},
  { 1, "Target.Angle",		"Target.Angle",		&rundb.Target.angle},
  { 1, "Target.Density", 	"Target.Density", 	&rundb.Target.density},

  { 1, "Target.totallength", 	"Target.totallength", 	&rundb.Target.totallength},
  { 1, "Target.totallength2", 	"Target.totallength2", 	&rundb.Target.totallength2},
  { 1, "Target.lengthratio", 	"Target.lengthratio", 	&rundb.Target.lengthratio},
  { 1, "Target.flow", 	"Target.flow", 	&rundb.Target.flow},

  { 1, "Target.WallThickness",	"Target.WallThickness", &rundb.Target.wallthickness},
  { 1, "Target.WallThickness",	"Target.WallThickness", &rundb.Target.wallthickness},
  { 1, "Target.SnowThickness",	"Target.SnowThickness", &rundb.Target.snowthickness},
  { 1, "Target.SnowDensity",  	"Target.SnowDensity",
    &rundb.Target.snowdensity},
  { 1, "Target.SnowParam"    ,"Target.SnowParam"   ,&rundb.Target.snowparam},
  { 4, "Target.WallParam"    ,"Target.WallParam"   ,&rundb.Target.wallparam},
  {-1, "Target.WallFile"     ,"Target.WallFile"    ,&rundb.Target.wallfile},
  { 3, "Target.Polarisation" ,"Target.Polarisation",&rundb.Target.Polarisation},
  { 1, "Target.VertexDistribution" ,"Target.VertexDistribution",&rundb.Target.vertexdistribution},
  {-1, "HDCA.config"         ,"HDCA.config"        ,&rundb.hdcA.configfile},
  { 9, "HDCA.U1.dtpara"      ,"HDCA.U1.dtpara"     ,&rundb.hdcA.u1.dtpara},
  { 1, "HDCA.U1.bigdtslope"  ,"HDCA.U1.bigdtslope" ,&rundb.hdcA.u1.bigdtslope},
  { 1, "HDCA.U1.bigdtlim"    ,"HDCA.U1.bigdtlim"   ,&rundb.hdcA.u1.bigdtlim},
  { 1, "HDCA.U1.negdtslope"  ,"HDCA.U1.negdtslope" ,&rundb.hdcA.u1.negdtslope},
  { 9, "HDCA.V1.dtpara"      ,"HDCA.V1.dtpara"     ,&rundb.hdcA.v1.dtpara},
  { 1, "HDCA.V1.bigdtslope"  ,"HDCA.V1.bigdtslope" ,&rundb.hdcA.v1.bigdtslope},
  { 1, "HDCA.V1.bigdtlim"    ,"HDCA.V1.bigdtlim"   ,&rundb.hdcA.v1.bigdtlim},
  { 1, "HDCA.V1.negdtslope"  ,"HDCA.V1.negdtslope" ,&rundb.hdcA.v1.negdtslope},
  { 9, "HDCA.U2.dtpara"      ,"HDCA.U2.dtpara"     ,&rundb.hdcA.u2.dtpara},
  { 1, "HDCA.U2.bigdtslope"  ,"HDCA.U2.bigdtslope" ,&rundb.hdcA.u2.bigdtslope},
  { 1, "HDCA.U2.bigdtlim"    ,"HDCA.U2.bigdtlim"   ,&rundb.hdcA.u2.bigdtlim},
  { 1, "HDCA.U2.negdtslope"  ,"HDCA.U2.negdtslope" ,&rundb.hdcA.u2.negdtslope},
  { 9, "HDCA.V2.dtpara"      ,"HDCA.V2.dtpara"     ,&rundb.hdcA.v2.dtpara},
  { 1, "HDCA.V2.bigdtslope"  ,"HDCA.V2.bigdtslope" ,&rundb.hdcA.v2.bigdtslope},
  { 1, "HDCA.V2.bigdtlim"    ,"HDCA.V2.bigdtlim"   ,&rundb.hdcA.v2.bigdtlim},
  { 1, "HDCA.V2.negdtslope"  ,"HDCA.V2.negdtslope" ,&rundb.hdcA.v2.negdtslope},
  
  { 1, "PPOLA.HDC_theta_offs","HDCA.HDC_theta_offs",&rundb.ppolA.hdc_theta_offs},
  { 1, "PPOLA.HDC_phi_offs"  ,"HDCA.HDC_phi_offs"  ,&rundb.ppolA.hdc_phi_offs},
  { 1, "PPOLA.HDC_psi_offs"  ,"HDCA.HDC_psi_offs"  ,&rundb.ppolA.hdc_psi_offs},
  { 1, "PPOLA.HDC_x_offs"    ,"HDCA.HDC_x_offs"    ,&rundb.ppolA.hdc_x_offs},
  { 1, "PPOLA.HDC_y_offs"    ,"HDCA.HDC_y_offs"    ,&rundb.ppolA.hdc_y_offs},
  { 1, "PPOLA.HDC_z_offs"    ,"HDCA.HDC_z_offs"    ,&rundb.ppolA.hdc_z_offs},
  { 1, "PPOLA.CarbonThickness","HDCA.CarbonThickness",
    &rundb.ppolA.CarbonThickness},
  
  {-1, "A.tma.file",	 	"A.tma.file",	 	&rundb.A.tma.filename},
  {-1, "A.tma.firstorder",	 	"A.tma.firstorder",	 	&rundb.A.tma.firstorder},
  {1,"A.tma.type","A.tma.type",&rundb.A.tma.type},
  {-1, "A.collimator",		"A.collimator",		&rundb.A.collimator},
  { 1, "A.Vacuum",	 	"A.Vacuum_Connection",&rundb.A.vacuum},
  {-1, "A.VDC.config",          "A.VDC.config",       &rundb.A.vdc.initfile},
  {16, "A.VDC.x1.Disabled",     "A.VDC.x1_Disabled",  rundb.A.vdc.x1.disabled},
  {16, "A.VDC.s1.Disabled",     "A.VDC.s1_Disabled",  rundb.A.vdc.s1.disabled},
  {16, "A.VDC.x2.Disabled",     "A.VDC.x2_Disabled",  rundb.A.vdc.x2.disabled},
  {16, "A.VDC.s2.Disabled",     "A.VDC.s2_Disabled",  rundb.A.vdc.s2.disabled},
  {16, "A.VDC.x1.paddle",       "A.VDC.x1_paddle",  rundb.A.vdc.x1.paddle},
  {16, "A.VDC.s1.paddle",       "A.VDC.s1_paddle",  rundb.A.vdc.s1.paddle},
  {16, "A.VDC.x2.paddle",       "A.VDC.x2_paddle",  rundb.A.vdc.x2.paddle},
  {16, "A.VDC.s2.paddle",       "A.VDC.s2_paddle",  rundb.A.vdc.s2.paddle},
  { 4, "A.VDC.TDC",	        "A.VDC.TDC",	 	rundb.A.vdc.offset},

  { 1, "A.VDC.Driftvelocity",   "A.VDC.Driftvelocity",	
    &rundb.A.vdc.driftvelocity},
  { 1, "A.VDC.MinimumDrift",   "A.VDC.MinimumDrift", &rundb.A.vdc.mindrift},
  { 1, "A.VDC.S_corrDriftvelocity", "A.VDC.S_corrDriftvelocity",	
    &rundb.A.vdc.corr_driftvel},
  { 1, "A.VDC.x.mode",          "A.VDC.x.mode",         &rundb.A.vdc.x.mode},
  { 3, "A.VDC.x.cut",           "A.VDC.x.cut",           rundb.A.vdc.x.cut},
  { 1, "A.VDC.s.mode",          "A.VDC.s.mode",         &rundb.A.vdc.s.mode},
  { 3, "A.VDC.s.cut",           "A.VDC.s.cut",           rundb.A.vdc.s.cut},
  {-1, "A.VDC.trackDir", 	"A.VDC.trackDir", 	&rundb.A.vdc.trackps},
  { 1, "A.Scaler.Prescaler",  	"A.Scaler.Prescaler",  	&rundb.A.Prescaler},
  { 1, "A.Scaler.Single",     	"A.Scaler.Single",     	&rundb.A.ScalerSingle},
  { 1, "A.Scaler.Double",     	"A.Scaler.Double",     	&rundb.A.ScalerDouble},
  { 1, "A.Scaler.Strobe",     	"A.Scaler.Strobe",     	&rundb.A.PreStrobe},
  { 1, "A.StrobeDead",     	"A.StrobeDead",     	&rundb.A.StrobeDead},
  { 1, "A.Wobbler.dx", 		"A.Wobbler.dx", 	&rundb.A.wobbler.dx},
  { 1, "A.Wobbler.x0",       	"A.Wobbler.x0",       	&rundb.A.wobbler.x0},
  { 1, "A.Wobbler.dy", 		"A.Wobbler.dy", 	&rundb.A.wobbler.dy},
  { 1, "A.Wobbler.y0", 		"A.Wobbler.y0", 	&rundb.A.wobbler.y0},
  { 1, "A.angle",		"A.angle",		&rundb.A.angle},
  { 1, "A.oopangle",		"A.oopangle",		&rundb.A.oopangle},
  { 1, "A.momentum",		"A.momentum",		&rundb.A.momentum},
  { 1, "A.momResolution",	"A.momResolution",   &rundb.A.momResolution},
  { 1, "A.momResolution2",	"A.momResolution2",  &rundb.A.momResolution2},
  { 1, "A.angularResolution",   "A.angularResolution",
    &rundb.A.angularResolution},
  { 1, "A.angularResolution2",  "A.angularResolution2",
    &rundb.A.angularResolution2},
  {20, "A.dpcuts",	        "A.dpcuts",             rundb.A.dpcuts},
  {20, "A.momRatio",	        "A.momRatio",           rundb.A.momRatio},
  {20, "A.angularRatio",	"A.angularRatio",	rundb.A.angularRatio},
  { 1, "A.verticalAngleRatio",	"A.verticalAngleRatio",	
    &rundb.A.verticalAngleRatio},
  { 1, "A.dEToF_TDC_Resolution","A.dEToF_TDC_Resolution", 
    &rundb.A.de_tof_tdc_resolution},
  { 1, "A.tma.x",		"A.tma.x",		&rundb.A.tma.x},
  { 1, "A.tma.y",		"A.tma.y",		&rundb.A.tma.y},
  { 1, "A.tma.theta",		"A.tma.theta",		&rundb.A.tma.theta},
  { 1, "A.tma.phi",		"A.tma.phi",		&rundb.A.tma.phi},
  { 3, "A.scint.walk", 		"A.scint.walk", 	rundb.A.scint.walk},
  {15, "A.scint.dE.corr.left.offset",  "A.scint.dE.corr.left.offset", 	rundb.A.scint.dE_corr_left_offset},
  {15, "A.scint.dE.corr.left.scale",   "A.scint.dE.corr.left.scale", 	rundb.A.scint.dE_corr_left_scale},
  {15, "A.scint.dE.corr.right.offset", "A.scint.dE.corr.right.offset", 	rundb.A.scint.dE_corr_right_offset},
  {15, "A.scint.dE.corr.right.scale",  "A.scint.dE.corr.right.scale", 	rundb.A.scint.dE_corr_right_scale},
  {15, "A.scint.ToF.corr.left.offset", "A.scint.ToF.corr.left.offset", 	rundb.A.scint.ToF_corr_left_offset},
  {15, "A.scint.ToF.corr.left.scale",  "A.scint.ToF.corr.left.scale", 	rundb.A.scint.ToF_corr_left_scale},
  {15, "A.scint.ToF.corr.right.offset","A.scint.ToF.corr.right.offset", rundb.A.scint.ToF_corr_right_offset},
  {15, "A.scint.ToF.corr.right.scale", "A.scint.ToF.corr.right.scale", 	rundb.A.scint.ToF_corr_right_scale},
  { 1, "A.scint.MinAdcOverThresholdValue", "A.scint.MinAdcOverThresholdValue", &rundb.A.scint.MinAdcOverThresholdValue},
  { 1, "A.scint.UseNewHandle", "A.scint.UseNewHandle", &rundb.A.scint.UseNewHandle},
  {15, "A.scint.dE",		"A.scint.dE",		rundb.A.scint.dE},
  {15, "A.scint.ToF.Left",  	"A.scint.ToF.Left",  	rundb.A.scint.ToF_Left},
  {15, "A.scint.dE.Left",   	"A.scint.dE.Left",   	rundb.A.scint.dE_Left},
  {15, "A.scint.ToF",		"A.scint.ToF",		rundb.A.scint.ToF},
  { 1, "A.scint.use_dE", 	"A.scint.use_dE", 	&rundb.A.scint.use_dE},
  {12, "A.Cerenkov.pedestal", "A.Cerenkov.pedestal", 	rundb.A.cerenkov.pedestal},
  {12, "A.Cerenkov.scale",   	"A.Cerenkov.scale",     rundb.A.cerenkov.scale},
  {12, "A.Cerenkov.offset",  	"A.Cerenkov.offset",    rundb.A.cerenkov.offset},
  { 1, "A.Cerenkov.FloatingPed","A.Cerenkov.FloatingPed",    
    &rundb.A.cerenkov.FloatingPed},
  { 1, "A.Cerenkov.FloatingEvents", 	"A.Cerenkov.FloatingEvents", 
    &rundb.A.cerenkov.FloatingEvents},
  {-1, "A.Cerenkov.PedFile",        	"A.Cerenkov.PedFile",        
   &rundb.A.cerenkov.PedFile},
   {-1, "A.simChamber","A.simChamber",&rundb.A.simChamber},
 
  {-1, "B.tma.file",	 	"B.tma.file",	 	&rundb.B.tma.filename},
  {-1, "B.tma.firstorder",	 	"B.tma.firstorder",	 	&rundb.B.tma.firstorder},
  {1,  "B.tma.type",            "B.tma.type",           &rundb.B.tma.type},
  {-1, "B.collimator",		"B.collimator",		&rundb.B.collimator},
  {1, "B.collimator.acceptance",		"B.collimator.acceptance",		&rundb.B.collimatoracceptance},
  { 1, "B.Vacuum",	 	"B.Vacuum_Connection",	&rundb.B.vacuum},
  {-1, "B.VDC.config",          "B.VDC.config",       &rundb.B.vdc.initfile},
  {16, "B.VDC.x1.Disabled",     "B.VDC.x1_Disabled",  rundb.B.vdc.x1.disabled},
  {16, "B.VDC.s1.Disabled",     "B.VDC.s1_Disabled",  rundb.B.vdc.s1.disabled},
  {16, "B.VDC.x2.Disabled",     "B.VDC.x2_Disabled",  rundb.B.vdc.x2.disabled},
  {16, "B.VDC.s2.Disabled",     "B.VDC.s2_Disabled",  rundb.B.vdc.s2.disabled},
  {16, "B.VDC.x1.paddle",       "B.VDC.x1_paddle",  rundb.B.vdc.x1.paddle},
  {16, "B.VDC.s1.paddle",       "B.VDC.s1_paddle",  rundb.B.vdc.s1.paddle},
  {16, "B.VDC.x2.paddle",       "B.VDC.x2_paddle",  rundb.B.vdc.x2.paddle},
  {16, "B.VDC.s2.paddle",       "B.VDC.s2_paddle",  rundb.B.vdc.s2.paddle},
  { 4, "B.VDC.TDC",	        "B.VDC.TDC",	 	rundb.B.vdc.offset},
  { 1, "B.VDC.Driftvelocity",   "B.VDC.Driftvelocity", 
    &rundb.B.vdc.driftvelocity},
  { 1, "B.VDC.MinimumDrift",   "B.VDC.MinimumDrift", &rundb.B.vdc.mindrift},
  { 1, "B.VDC.S_corrDriftvelocity", "B.VDC.S_corrDriftvelocity",	
    &rundb.B.vdc.corr_driftvel},
    { 1, "B.VDC.x.mode","B.VDC.x.mode",  &rundb.B.vdc.x.mode},
    { 3, "B.VDC.x.cut", "B.VDC.x.cut",    rundb.B.vdc.x.cut},
    { 1, "B.VDC.s.mode","B.VDC.s.mode",  &rundb.B.vdc.s.mode},
    { 3, "B.VDC.s.cut", "B.VDC.s.cut",    rundb.B.vdc.s.cut},
  {-1, "B.VDC.trackDir", 	"B.VDC.trackDir", 	&rundb.B.vdc.trackps},
  { 1, "B.Scaler.Prescaler",  	"B.Scaler.Prescaler",  	&rundb.B.Prescaler},
  { 1, "B.Scaler.Single",     	"B.Scaler.Single",     	&rundb.B.ScalerSingle},
  { 1, "B.Scaler.Double",     	"B.Scaler.Double",     	&rundb.B.ScalerDouble},
  { 1, "B.Scaler.Strobe",     	"B.Scaler.Strobe",     	&rundb.B.PreStrobe},
  { 1, "B.StrobeDead",     	"B.StrobeDead",     	&rundb.B.StrobeDead},
  { 1, "B.Wobbler.dx", 		"B.Wobbler.dx", 	&rundb.B.wobbler.dx},
  { 1, "B.Wobbler.x0",       	"B.Wobbler.x0",       	&rundb.B.wobbler.x0},
  { 1, "B.Wobbler.dy", 		"B.Wobbler.dy", 	&rundb.B.wobbler.dy},
  { 1, "B.Wobbler.y0", 		"B.Wobbler.y0", 	&rundb.B.wobbler.y0},
  { 1, "B.angle",		"B.angle",		&rundb.B.angle},
  { 1, "B.left",                "B.side.left",          &rundb.B.left},
  { 1, "B.oopangle",		"B.oopangle",		&rundb.B.oopangle},
  { 1, "B.momentum",		"B.momentum",		&rundb.B.momentum},
  { 1, "B.momResolution",	"B.momResolution",   &rundb.B.momResolution},
  { 1, "B.momResolution2",	"B.momResolution2",  &rundb.B.momResolution2},
  { 1, "B.angularResolution",   "B.angularResolution",
    &rundb.B.angularResolution},
  { 1, "B.angularResolution2",  "B.angularResolution2",
    &rundb.B.angularResolution2},
  {20, "B.dpcuts",	        "B.dpcuts",             rundb.B.dpcuts},
  {20, "B.momRatio",	        "B.momRatio",           rundb.B.momRatio},
  {20, "B.angularRatio",	"B.angularRatio",	rundb.B.angularRatio},
  { 1, "B.verticalAngleRatio",	"B.verticalAngleRatio",	
    &rundb.B.verticalAngleRatio},
  { 1, "B.dEToF_TDC_Resolution","B.dEToF_TDC_Resolution", &rundb.B.de_tof_tdc_resolution},
  { 1, "B.tma.x",		"B.tma.x",		&rundb.B.tma.x},
  { 1, "B.tma.y",		"B.tma.y",		&rundb.B.tma.y},
  { 1, "B.tma.theta",		"B.tma.theta",		&rundb.B.tma.theta},
  { 1, "B.tma.phi",		"B.tma.phi",		&rundb.B.tma.phi},
  { 3, "B.scint.walk", 		"B.scint.walk", 	rundb.B.scint.walk},
  {14, "B.scint.dE.corr.left.offset",  "B.scint.dE.corr.left.offset", 	rundb.B.scint.dE_corr_left_offset},
  {14, "B.scint.dE.corr.left.scale",   "B.scint.dE.corr.left.scale", 	rundb.B.scint.dE_corr_left_scale},
  {14, "B.scint.dE.corr.right.offset", "B.scint.dE.corr.right.offset", 	rundb.B.scint.dE_corr_right_offset},
  {14, "B.scint.dE.corr.right.scale",  "B.scint.dE.corr.right.scale", 	rundb.B.scint.dE_corr_right_scale},
  {14, "B.scint.ToF.corr.left.offset", "B.scint.ToF.corr.left.offset", 	rundb.B.scint.ToF_corr_left_offset},
  {14, "B.scint.ToF.corr.left.scale",  "B.scint.ToF.corr.left.scale", 	rundb.B.scint.ToF_corr_left_scale},
  {14, "B.scint.ToF.corr.right.offset","B.scint.ToF.corr.right.offset", rundb.B.scint.ToF_corr_right_offset},
  {14, "B.scint.ToF.corr.right.scale", "B.scint.ToF.corr.right.scale", 	rundb.B.scint.ToF_corr_right_scale},
  { 1, "B.scint.MinAdcOverThresholdValue", "B.scint.MinAdcOverThresholdValue", &rundb.B.scint.MinAdcOverThresholdValue},
  { 1, "B.scint.UseNewHandle", "B.scint.UseNewHandle", &rundb.B.scint.UseNewHandle},
  {14, "B.scint.dE",		"B.scint.dE",		rundb.B.scint.dE},
  {14, "B.scint.ToF.Left",  	"B.scint.ToF.Left",  	rundb.B.scint.ToF_Left},
  {14, "B.scint.dE.Left",   	"B.scint.dE.Left",   	rundb.B.scint.dE_Left},
  {14, "B.scint.ToF",		"B.scint.ToF",		rundb.B.scint.ToF},
  { 1, "B.scint.use_dE", 	"B.scint.use_dE", 	&rundb.B.scint.use_dE},
{12, "B.Cerenkov.pedestal", "B.Cerenkov.pedestal", 	rundb.B.cerenkov.pedestal},
  {12, "B.Cerenkov.scale",   	"B.Cerenkov.scale",     rundb.B.cerenkov.scale},
  {12, "B.Cerenkov.offset",  	"B.Cerenkov.offset",    rundb.B.cerenkov.offset},
  { 1, "B.Cerenkov.FloatingPed","B.Cerenkov.FloatingPed",    
    &rundb.B.cerenkov.FloatingPed},
  { 1, "B.Cerenkov.FloatingEvents", 	"B.Cerenkov.FloatingEvents", 
    &rundb.B.cerenkov.FloatingEvents},
  {-1, "B.Cerenkov.PedFile",        	"B.Cerenkov.PedFile",        
   &rundb.B.cerenkov.PedFile},
  {-1, "B.simChamber","B.simChamber",&rundb.B.simChamber},
  
  {-1, "C.tma.file",	 	"C.tma.file",	 	&rundb.C.tma.filename},
  {-1, "C.tma.firstorder",	 	"C.tma.firstorder",	 	&rundb.C.tma.firstorder},
  {1,"C.tma.type","C.tma.type",&rundb.C.tma.type},
  {-1, "C.collimator",		"C.collimator",		&rundb.C.collimator},
  { 1, "C.Vacuum",	 	"C.Vacuum_Connection",	 	&rundb.C.vacuum},
  {-1, "C.VDC.config",          "C.VDC.config",       &rundb.C.vdc.initfile},
  {16, "C.VDC.x1.Disabled",     "C.VDC.x1_Disabled",  rundb.C.vdc.x1.disabled},
  {16, "C.VDC.s1.Disabled",     "C.VDC.s1_Disabled",  rundb.C.vdc.s1.disabled},
  {16, "C.VDC.x2.Disabled",     "C.VDC.x2_Disabled",  rundb.C.vdc.x2.disabled},
  {16, "C.VDC.s2.Disabled",     "C.VDC.s2_Disabled",  rundb.C.vdc.s2.disabled},
  {16, "C.VDC.x1.paddle",       "C.VDC.x1_paddle",  rundb.C.vdc.x1.paddle},
  {16, "C.VDC.s1.paddle",       "C.VDC.s1_paddle",  rundb.C.vdc.s1.paddle},
  {16, "C.VDC.x2.paddle",       "C.VDC.x2_paddle",  rundb.C.vdc.x2.paddle},
  {16, "C.VDC.s2.paddle",       "C.VDC.s2_paddle",  rundb.C.vdc.s2.paddle},
  { 4, "C.VDC.TDC",	        "C.VDC.TDC",	 	rundb.C.vdc.offset},
  { 1, "C.VDC.Driftvelocity",   "C.VDC.Driftvelocity",	
    &rundb.C.vdc.driftvelocity},
  { 1, "C.VDC.MinimumDrift",   "C.VDC.MinimumDrift", &rundb.C.vdc.mindrift},
  { 1, "C.VDC.S_corrDriftvelocity", "C.VDC.S_corrDriftvelocity",	
    &rundb.C.vdc.corr_driftvel},
  { 1, "C.VDC.x.mode",          "C.VDC.x.mode",         &rundb.C.vdc.x.mode},
  { 3, "C.VDC.x.cut",           "C.VDC.x.cut",           rundb.C.vdc.x.cut},
  { 1, "C.VDC.s.mode",          "C.VDC.s.mode",         &rundb.C.vdc.s.mode},
  { 3, "C.VDC.s.cut",           "C.VDC.s.cut",           rundb.C.vdc.s.cut},
  {-1, "C.VDC.trackDir", 	"C.VDC.trackDir", 	&rundb.C.vdc.trackps},
  { 1, "C.Scaler.Prescaler",  	"C.Scaler.Prescaler",  	&rundb.C.Prescaler},
  { 1, "C.Scaler.Single",     	"C.Scaler.Single",     	&rundb.C.ScalerSingle},
  { 1, "C.Scaler.Double",     	"C.Scaler.Double",     	&rundb.C.ScalerDouble},
  { 1, "C.Scaler.Strobe",     	"C.Scaler.Strobe",     	&rundb.C.PreStrobe},
  { 1, "C.StrobeDead",     	"C.StrobeDead",     	&rundb.C.StrobeDead},
  { 1, "C.Wobbler.dx", 		"C.Wobbler.dx", 	&rundb.C.wobbler.dx},
  { 1, "C.Wobbler.x0",       	"C.Wobbler.x0",       	&rundb.C.wobbler.x0},
  { 1, "C.Wobbler.dy", 		"C.Wobbler.dy", 	&rundb.C.wobbler.dy},
  { 1, "C.Wobbler.y0", 		"C.Wobbler.y0", 	&rundb.C.wobbler.y0},
  { 1, "C.angle",		"C.angle",		&rundb.C.angle},
  { 1, "C.oopangle",		"C.oopangle",		&rundb.C.oopangle},
  { 1, "C.momentum",		"C.momentum",		&rundb.C.momentum},
  { 1, "C.momResolution",	"C.momResolution",   &rundb.C.momResolution},
  { 1, "C.momResolution2",	"C.momResolution2",  &rundb.C.momResolution2},
  { 1, "C.angularResolution",   "C.angularResolution",
    &rundb.C.angularResolution},
  { 1, "C.angularResolution2",  "C.angularResolution2",
    &rundb.C.angularResolution2},
  {20, "C.dpcuts",	        "C.dpcuts",             rundb.C.dpcuts},
  {20, "C.momRatio",	        "C.momRatio",           rundb.C.momRatio},
  {20, "C.angularRatio",	"C.angularRatio",	rundb.C.angularRatio},
  { 1, "C.verticalAngleRatio",	"C.verticalAngleRatio",	
    &rundb.C.verticalAngleRatio},
  { 1, "C.dEToF_TDC_Resolution","C.dEToF_TDC_Resolution", 
    &rundb.C.de_tof_tdc_resolution},
  { 1, "C.tma.x",		"C.tma.x",		&rundb.C.tma.x},
  { 1, "C.tma.y",		"C.tma.y",		&rundb.C.tma.y},
  { 1, "C.tma.theta",		"C.tma.theta",		&rundb.C.tma.theta},
  { 1, "C.tma.phi",		"C.tma.phi",		&rundb.C.tma.phi},
  { 3, "C.scint.walk", 		"C.scint.walk", 	rundb.C.scint.walk},
  {15, "C.scint.dE.corr.left.offset",  "C.scint.dE.corr.left.offset", 	rundb.C.scint.dE_corr_left_offset},
  {15, "C.scint.dE.corr.left.scale",   "C.scint.dE.corr.left.scale", 	rundb.C.scint.dE_corr_left_scale},
  {15, "C.scint.dE.corr.right.offset", "C.scint.dE.corr.right.offset", 	rundb.C.scint.dE_corr_right_offset},
  {15, "C.scint.dE.corr.right.scale",  "C.scint.dE.corr.right.scale", 	rundb.C.scint.dE_corr_right_scale},
  {15, "C.scint.ToF.corr.left.offset", "C.scint.ToF.corr.left.offset", 	rundb.C.scint.ToF_corr_left_offset},
  {15, "C.scint.ToF.corr.left.scale",  "C.scint.ToF.corr.left.scale", 	rundb.C.scint.ToF_corr_left_scale},
  {15, "C.scint.ToF.corr.right.offset","C.scint.ToF.corr.right.offset", rundb.C.scint.ToF_corr_right_offset},
  {15, "C.scint.ToF.corr.right.scale", "C.scint.ToF.corr.right.scale", 	rundb.C.scint.ToF_corr_right_scale},
  { 1, "C.scint.MinAdcOverThresholdValue", "C.scint.MinAdcOverThresholdValue", &rundb.C.scint.MinAdcOverThresholdValue},
  { 1, "C.scint.UseNewHandle", "C.scint.UseNewHandle", &rundb.C.scint.UseNewHandle},
  {15, "C.scint.dE",		"C.scint.dE",		rundb.C.scint.dE},
  {15, "C.scint.ToF.Left",  	"C.scint.ToF.Left",  	rundb.C.scint.ToF_Left},
  {15, "C.scint.dE.Left",   	"C.scint.dE.Left",   	rundb.C.scint.dE_Left},
  {15, "C.scint.ToF",		"C.scint.ToF",		rundb.C.scint.ToF},
  { 1, "C.scint.use_dE", 	"C.scint.use_dE", 	&rundb.C.scint.use_dE},
  {12, "C.Cerenkov.pedestal", "C.Cerenkov.pedestal", 	rundb.C.cerenkov.pedestal},
  {12, "C.Cerenkov.scale",   	"C.Cerenkov.scale",     rundb.C.cerenkov.scale},
  {12, "C.Cerenkov.offset",  	"C.Cerenkov.offset",    rundb.C.cerenkov.offset},
  { 1, "C.Cerenkov.FloatingPed","C.Cerenkov.FloatingPed",    
    &rundb.C.cerenkov.FloatingPed},
  { 1, "C.Cerenkov.FloatingEvents", 	"C.Cerenkov.FloatingEvents", 
    &rundb.C.cerenkov.FloatingEvents},
  {-1, "C.Cerenkov.PedFile",        	"C.Cerenkov.PedFile",        
   &rundb.C.cerenkov.PedFile},
  {-1, "C.simChamber","C.simChamber",&rundb.C.simChamber},
  
  {-1, "D.tma.file",	      "D.tma.file",	     &rundb.D.tma.filename},
  {-1, "D.tma.firstorder",	      "D.tma.firstorder",	     &rundb.D.tma.firstorder},
  {1,"D.tma.type","D.tma.type",&rundb.D.tma.type},
  {-1, "D.collimator",	      "D.collimator",	     &rundb.D.collimator},
  { 1, "D.Vacuum",	      "D.Vacuum_Connection", &rundb.D.vacuum},
  { 1, "D.Wobbler.dx", 	      "D.Wobbler.dx", 	     &rundb.D.wobbler.dx},
  { 1, "D.Wobbler.x0",        "D.Wobbler.x0",        &rundb.D.wobbler.x0},
  { 1, "D.Wobbler.dy", 	      "D.Wobbler.dy", 	     &rundb.D.wobbler.dy},
  { 1, "D.Wobbler.y0", 	      "D.Wobbler.y0", 	     &rundb.D.wobbler.y0},
  { 1, "D.angle",	      "D.angle",	     &rundb.D.angle},
  { 1, "D.oopangle",	      "D.oopangle",	     &rundb.D.oopangle},
  { 1, "D.momentum",	      "D.momentum",	     &rundb.D.momentum},
  { 1, "D.momResolution",	"D.momResolution",   &rundb.D.momResolution},
  { 1, "D.momResolution2",	"D.momResolution2",  &rundb.D.momResolution2},
  { 1, "D.angularResolution",   "D.angularResolution",
    &rundb.D.angularResolution},
  { 1, "D.angularResolution2",  "D.angularResolution2",
    &rundb.D.angularResolution2},
  {20, "D.dpcuts",	        "D.dpcuts",             rundb.D.dpcuts},
  {20, "D.momRatio",	        "D.momRatio",           rundb.D.momRatio},
  {20, "D.angularRatio",	"D.angularRatio",	rundb.D.angularRatio},
  { 1, "D.momResolution",     "D.momResolution",     &rundb.D.momResolution},
  { 1, "D.angularResolution", "D.angularResolution", 
    &rundb.D.angularResolution},
  { 1, "D.verticalAngleRatio",	"D.verticalAngleRatio",	
    &rundb.D.verticalAngleRatio},
  { 1, "D.tma.x",	      "D.tma.x",	     &rundb.D.tma.x},
  { 1, "D.tma.y",	      "D.tma.y",	     &rundb.D.tma.y},
  { 1, "D.tma.theta",	      "D.tma.theta",	     &rundb.D.tma.theta},
  { 1, "D.tma.phi",	      "D.tma.phi",	     &rundb.D.tma.phi},
  { 3, "D.scint.walk", 	      "D.scint.walk", 	     rundb.D.scint.walk},
  {15, "D.scint.dE",	      "D.scint.dE",	     rundb.D.scint.dE},
  {15, "D.scint.ToF.Left",    "D.scint.ToF.Left",    rundb.D.scint.ToF_Left},
  {15, "D.scint.dE.Left",     "D.scint.dE.Left",     rundb.D.scint.dE_Left},
  {15, "D.scint.ToF",	      "D.scint.ToF",	     rundb.D.scint.ToF},
  { 1, "D.scint.use_dE",      "D.scint.use_dE",      &rundb.D.scint.use_dE},
  { -1,"D.SNO.X",             "D.SNO.X",             &rundb.pdc.x_snofile},
  { -1,"D.SNO.Y",             "D.SNO.Y",             &rundb.pdc.y_snofile},

  {72, "N.offset",	 "N.offset",       rundb.N.offset},
  {72, "N.pedestal",	 "N.pedestal",     rundb.N.pedestal},
  {36, "N.position",	 "N.position",     rundb.N.position},
  {36, "N.timeAtTarget", "N.timeAtTarget", rundb.N.timeAtTarget},
  { 1, "N.angle",	 "N.angle",       &rundb.N.angle},
  { 1, "N.oop_angle",    "N.oop_angle",   &rundb.N.oop_angle},
  { 1, "N.distance",	 "N.distance",    &rundb.N.distance},
  { 1, "N.height",	 "N.height",      &rundb.N.height},
  { 1, "N.width",	 "N.width",       &rundb.N.width},
  {-1, "N.file",	 "N.file",        &rundb.N.filename},
  {72, "N.flasherpedestal", "N.flasherpedestal", rundb.N.flasherpedestal},
  {72, "N.flasherpeak",	    "N.flasherpeak",     rundb.N.flasherpeak},
  {72, "N.AdcParticlePeak", "N.AdcParticlePeak", rundb.N.AdcParticlePeak},
  {90, "N.TdcMulti1Gauge",  "N.TdcMulti1Gauge",  rundb.N.TdcMulti1Gauge},

  { 5,  "nPol.tdcScale",      "nPol.tdcScale",      rundb.npol.tdcScale},
  { 1,  "nPol.maxCoincTime",  "nPol.maxCoincTime",  &rundb.npol.maxcoincT},
  { 1,  "nPol.noradlow",      "nPol.noradlow",      &rundb.npol.noradlow},
  { 1,  "nPol.noradup",       "nPol.noradup",       &rundb.npol.noradup},
  { 1,  "nPol.adc.maxjitter", "nPol.maxjitter",     &rundb.npol.maxjitter},
  { 1,  "nPol.Wall2.mu",      "nPol.mu",            &rundb.npol.mu},
  { 1,  "nPol.beamPol",       "nPol.beamPol",       &rundb.npol.beamPol},
  { 5,  "nPol.minADC",        "nPol.minADC",        rundb.npol.minADC},
  { 5,  "nPol.maxADC",        "nPol.maxADC",        rundb.npol.maxADC},
  { 1,  "nPol.alpha",         "nPol.alpha",         &rundb.npol.alpha},
  { 3,  "nPol.Wall1.origin",  "nPol.Wall1.origin",  rundb.npol.W1_origin},
  { 3,  "nPol.Top.origin",    "nPol.Top.origin",    rundb.npol.Top_origin},
  { 3,  "nPol.Bottom.origin", "nPol.Bottom.origin", rundb.npol.Bottom_origin},
  { 3,  "nPol.trigger1.pos",  "nPol.trigger1.pos",  rundb.npol.trig1_pos},
  { 3,  "nPol.trigger2.pos",  "nPol.trigger2.pos",  rundb.npol.trig2_pos},
  { 3,  "nPol.trigger3.pos",  "nPol.trigger3.pos",  rundb.npol.trig3_pos}, 
  {-1,  "nPol.file.position", "nPol.file.position", &rundb.npol.file_position},
  {-1,  "nPol.file.pedestal", "nPol.file.pedestal", &rundb.npol.file_pedestal},
  {-1,  "nPol.file.tdcDiff",  "nPol.file.tdcDiff",  &rundb.npol.file_tdcDiff},
  {-1,  "nPol.file.tdcSum",   "nPol.file.tdcSum",   &rundb.npol.file_tdcSum},
  {-1,  "nPol.file.tdcWalk",  "nPol.file.tdcWalk",  &rundb.npol.file_tdcWalk},
  {-1,  "nPol.file.tdcCut",   "nPol.file.tdcCut",   &rundb.npol.file_tdcCut},
  {-1,  "nPol.file.adcCut",   "nPol.file.adcCut",   &rundb.npol.file_adcCut},
  {-1,  "nPol.file.timeCut",  "nPol.file.timeCut",  &rundb.npol.file_timeCut},
  {-1,  "nPol.file.pulshCut", "nPol.file.pulshCut", &rundb.npol.file_pulshCut},
  {-1,  "nPol.file.adcRatio", "nPol.file.adcRatio", &rundb.npol.file_adcRatio},
  { 1,  "nPol.Wall1.single",  "nPol.Wall1.single",  &rundb.npol.W1_single},
  { 2,  "nPol.Wall1.double",  "nPol.Wall1.double",  rundb.npol.W1_double},
  { 1,  "nPol.Wall2.single",  "nPol.Wall2.single",  &rundb.npol.W2_single},
  { 2,  "nPol.Wall2.double",  "nPol.Wall2.double",  rundb.npol.W2_double},
  { 3,  "nPol.Wall2.triple",  "nPol.Wall2.triple",  rundb.npol.W2_triple},
  { 1,  "nPol.UseVetos",      "nPol.UseVetos",      &rundb.npol.veto_use},
  { 1,  "nPol.UseNVetos",     "nPol.UseNVetos",     &rundb.npol.n_veto_use},
  { 1,  "nPol.UseMulti",      "nPol.UseMulti",      &rundb.npol.usemulti},
  { 1,  "nPol.magnet.I",      "nPol.magnet.I",      &rundb.npol.magnet_I},
  { 3,  "nPol.magnet.pos",    "nPol.magnet.pos",    rundb.npol.magnet_pos},
  {-1,  "nPol.magnet.path",   "nPol.magnet.path",   &rundb.npol.magnet_path},
  { 1,  "nPol.Bint.low",      "nPol.Bint.low",      &rundb.npol.Bint_low},
  { 1,  "nPol.Bint.up",       "nPol.Bint.up",       &rundb.npol.Bint_up},
  {-1,  "Si.config",          "Si.config",          &rundb.si.configfile},
  { 1,  "Si.angle",           "Si.angle",           &rundb.si.angle},
  { 1,  "Si.distance",        "Si.distance",        &rundb.si.distance},
  { 1,  "Si.simCentMom",      "Si.simCentMom",      &rundb.si.simCentMom},
  { 8,  "Si.simResolution",   "Si.simResolution",   &rundb.si.simResolution},
  { 1,  "Si.simAcceptScale",  "Si.simAcceptScale",  &rundb.si.simAcceptScale},
  { 1,  "use_fADC",           "use_fADC",           &rundb.fadc.use_fadc},
  { 1,  "fADC.M",             "fADC.M",             &rundb.fadc.M},
  { 1,  "fADC.RT",            "fADC.RT",            &rundb.fadc.trap_RT},
  { 1,  "fADC.FTmin",         "fADC.FTmin",         &rundb.fadc.trap_FT_min},
  { 1,  "fADC.FTmax",         "fADC.FTmax",         &rundb.fadc.trap_FT_max},
  { 1,  "fADC.Sstart",        "fADC.Sstart",        &rundb.fadc.signal_start},
  { 1,  "fADC.pileup.ctrl",   "fADC.pileup.ctrl",   &rundb.fadc.pileup_control},
  { 1,  "fADC.pileup.thr",    "fADC.pileup.thr",    &rundb.fadc.pileup_threshold},
  { 1,  "SciFi.use_fibre",    "SciFi.use_fibre",    &rundb.SciFi.use_fibre},
  {FIBRECHNS, "SciFi.TDC_offset",   "SciFi.TDC_offset",   &rundb.SciFi.TDC_offset},
  { 4,  "SciFi.trigger",      "SciFi.trigger",      &rundb.SciFi.trigger},
  { 5,  "SciFi.analysisParameters", "SciFi.analysisParameters",	&rundb.SciFi.analysisParameters},
  { 3,  "SciFi.eventDisplay", "SciFi.eventDisplay", &rundb.SciFi.eventDisplay},
  {-1,  "SciFi.tma.file",     "SciFi.tma.file",	    &rundb.SciFi.tma.filename},
  { 1,  "SciFi.momentum",     "SciFi.momentum",	    &rundb.SciFi.momentum},
  { 1,  "kaos.use_tof",       "kaos.use_tof",       &rundb.kaos.use_tof},
  { 1,  "kaos.use_mwpc",      "kaos.use_mwpc",      &rundb.kaos.use_mwpc},
  { 1,  "kaos.use_track",     "kaos.use_track",     &rundb.kaos.use_track},
  { 1,  "kaos.use_multi_track", "kaos.use_multi_track", &rundb.kaos.use_multi_track},
  { 1,  "kaos.use_tagger",    "kaos.use_tagger",    &rundb.kaos.use_tagger},
  { 1,  "kaos.use_aerogel",   "kaos.use_aerogel",   &rundb.kaos.use_aerogel},
  { 1,  "kaos.use_trigger",   "kaos.use_trigger",   &rundb.kaos.use_trigger},
  { 1,  "kaos.use_timewalk_corr",   "kaos.use_timewalk_corr",   &rundb.kaos.use_timewalk_corr},
  { 5,  "kaos.fe_pos",        "kaos.fe_pos",        &rundb.kaos.fe_pos},
  { 1,  "kaos.min_quality",   "kaos.min_quality",   &rundb.kaos.min_quality},
  {50,  "kaos.det_pos",       "kaos.det_pos",       &rundb.kaos.det_pos},
  {31,  "kaos.track_mwpc",    "kaos.track_mwpc",    &rundb.kaos.track_mwpc},
  {25,  "kaos.track_tof",     "kaos.track_tof",     &rundb.kaos.track_tof},
  { 2,  "kaos.ADC_scale",     "kaos.ADC_scale",     &rundb.kaos.ADC_scale},
  { 5,  "kaos.threshold",     "kaos.threshold",     &rundb.kaos.threshold},
  {120, "kaos.ADC_gain",      "kaos.ADC_gain",      &rundb.kaos.ADC_gain},
  {128, "kaos.ADC_pedestal",  "kaos.ADC_pedestal",  &rundb.kaos.ADC_pedestal},
  {128, "kaos.ADC_correction","kaos.ADC_correction",&rundb.kaos.ADC_correction},
  {240, "kaos.ADC_absorption","kaos.ADC_absorpion", &rundb.kaos.ADC_absorption},
  { 5,  "kaos.offset",        "kaos.offset",        &rundb.kaos.offset},
  {120, "kaos.TDC_offset",    "kaos.TDC_offset",    &rundb.kaos.TDC_offset},
  {128, "kaos.TDC_correction","kaos.TDC_correction",&rundb.kaos.TDC_correction},
  {120, "kaos.TDC_time_corr", "kaos.TDC_time_corr", &rundb.kaos.TDC_time_corr},
  { 1,  "kaos.timewalk_corr", "kaos.timewalk_corr", &rundb.kaos.timewalk_corr},
  {24,  "kaos.Aerogel_gain",       "kaos.Aerogel_gain",       &rundb.kaos.Aerogel_gain},
  {24,  "kaos.Aerogel_pedestal",   "kaos.Aerogel_pedestal",   &rundb.kaos.Aerogel_pedestal},
  {24,  "kaos.Aerogel_correction", "kaos.Aerogel_correction", &rundb.kaos.Aerogel_correction},
  {24,  "kaos.Aerogel_offset",     "kaos.Aerogel_offset",     &rundb.kaos.Aerogel_offset},
  {620, "kaos.MWPC_gain",     "kaos.MWPC_gain",     &rundb.kaos.MWPC_gain},
  {-1,  "kaos.MWPC_trackDir", "kaos.MWPC_trackDir", &rundb.kaos.MWPC_trackDir},
  {-1,  "kaos.tma.file",      "kaos.tma.file",	    &rundb.kaos.tma.filename},
  { 1,  "kaos.tma.type",      "kaos.tma.type",      &rundb.kaos.tma.type},
  { 1,  "kaos.angle",	      "kaos.angle",	    &rundb.kaos.angle},
  { 1,  "kaos.momentum",	 "kaos.momentum",	  &rundb.kaos.momentum},
  { 1,  "kaos.Scaler.Prescaler", "kaos.Scaler.Prescaler", &rundb.kaos.Prescaler},
  { 1,  "kaos.Scaler.Single",    "kaos.Scaler.Single",    &rundb.kaos.ScalerSingle},
  { 1,  "kaos.Scaler.Double",    "kaos.Scaler.Double",    &rundb.kaos.ScalerDouble},
  {-1,  "kaos.collimator",	 "kaos.collimator",	  &rundb.kaos.collimator},
  {-1,  "kaos.acceptance",       "kaos.acceptance",       &rundb.kaos.acceptance},
  {-1,  "kaos.trigger",          "kaos.trigger",          &rundb.kaos.trigger},
  { 1,  "kaos.momResolution",    "kaos.momResolution",    &rundb.kaos.momResolution},
  { 1,  "kaos.angularResolution","kaos.angularResolution",&rundb.kaos.angularResolution},
  { 1,  "kaos.angularResolution2","kaos.angularResolution2",&rundb.kaos.angularResolution2},
  { 1,  "kaos.vacuum",           "kaos.Vacuum_Connection",&rundb.kaos.vacuum},
  { -1, "ncube.calibration",     "ncube.calibration",     &rundb.ncube.calibration},
  { -1, "ncube.mapping",         "ncube.mapping",         &rundb.ncube.mapping},
  { -1, "ndet.calibrationFile",  "ndet.calibrationFile",  &rundb.ndet.calibrationFile},
  { -1, "ndet.mappingFile",      "ndet.mappingFile",      &rundb.ndet.mappingFile},
  {  1, "ndet.use_ndet",         "ndet.use_ndet",         &rundb.ndet.use_ndet},
  {4711, "q2",		"", &rundb.wastebasket}, // removed 1.12.98
  {4711, "q2.Range",	"", &rundb.wastebasket}, // removed 1.12.98
  {4711, "Wobbler.x",	"", &rundb.wastebasket}, // removed 1.12.98
  {4711, "Wobbler.y",	"", &rundb.wastebasket}, // removed 1.12.98
 {0, NULL, 0}
};
#else /* __RUNPARSER__ */
extern rundatabase rundb;
extern struct longname { int type; 
                char *Name; 
                char *sql;
                void *addr;
} lname[];
#endif /* __RUNPARSER__ */

#include <ctime>
void readRunDatabase(const char *name, const char *runname, 
		     const char *setup, time_t time);
void readSQLdatabase(const char *runname, const char *setup,
		     const char *host, const char *dbname);

#endif /* __RUNDATABASE_H__ */
