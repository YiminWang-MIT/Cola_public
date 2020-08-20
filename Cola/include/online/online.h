/*
 * $Id: online.h 2683 2016-12-09 14:00:15Z aesser $
 */

#ifndef __ONLINE_H__
#define __ONLINE_H__

/** @name First level reconstructed data.
 * 
 *  The data structure "online" is used to store all reconstructed
 *  first level data. It can be accessed in a "*.col" file 
 *  via 
 * 
 * \TEX{\tt Scalar variable = @online.xxxxx} 
 */ 
/*@{*/
/** Reconstructed coordinates at target point*/
struct TargetCoord { 
  /** Momentum [percent of reference momentum]  */ double dp;
  /** \TEX{$\theta_0$} (positive down) [mrad]   */ double th;
  /** \TEX{$y_0$} [cm]                          */ double y0;
  /** \TEX{$\phi_0$} [mrad]                     */ double ph;
  /** pathlength in spectrometer [cm]           */ double len;
};

/** Reconstructed Vertex position */
struct vertex {
  /** x (direction spec. C) [mm]         */ double x; 
  /** y (down) [mm]                      */ double y;
  /** z (beam direction) [mm]            */ double z;             
};

/** Cerenkov data */
struct cerenkov {
  /** number of hits                     */ short hit;
  /** Energy sum                         */ double sum;
  /** Hit pattern                        */ short pattern;
  /**                                    */ float FloatedPed[12];
}; 

/** Scintillator data */
struct scinti       { 
  /** Energy left/right multiplier       */ struct {double left,right,
						      left_corr,
						      right_corr;} Energy;
  /** Hit pattern                        */ short pattern; 
  /** Number of paddles hit              */ int hits;
  /** Number of paddles dummy hit (E>0)  */ int dummy_hits;
  /** Number of paddle used for timing   */ short paddle;
  /** Delay of paddle used for timing    */ double scint;
  /** bss 2013-12-30 software ped subtr  */ double AdcPedCorr_left[15];
  /** bss 2013-12-30 software ped subtr  */ double AdcPedCorr_right[15];
  /** bss 2013-12-30 finally scaled      */ double AdcScaled_left[15];
  /** bss 2013-12-30 finally scaled      */ double AdcScaled_right[15];

};

/** Vertical drift chamber */
struct vdc          { 
  /** Raw information of first 8 wires with signal */ int raw_time[8];
  /** Raw information of first 8 wires with signal */ int raw_wire[8]; 
  /** Raw information of first 8 wires with signal */ int time; 
  /** Raw information of first 8 wires with signal */ int wire; 
  /** Multiplicity                       */ int multi;
  /** Error code, see chamber library    */ int error;
  /** Wire pattern, see chamber library  */ int pattern; 
  /** reconstructed position             */ double x; 
  /** reconstructed slope                */ double slope;
  /** \TEX{$\chi^2$} of straight line fit*/ double chi_sqr; 
  /** minimal drift time */ double maxdt; 
  /** maximal drift time */ double mindt; 
  /** number of good wires*/ double nw; 
};

/** Focal plane coordinates for a wire chamber packet */
struct FocalCoord { 
  /** x (dispersive) [mm]                */ double x;
  /** \TEX{$\theta$} (dispersive) [mrad] */ double th;
  /** y [mm]                             */ double y;
  /** \TEX{$\phi$} [mrad]                */ double ph;
};

/** SIMULATED focal plane coordinates for a wire chamber packet */
struct simFocalCoord { 
  /** x (dispersive) [mm]                */ double x;
  /** \TEX{$\theta$} (dispersive) [mrad] */ double th;
  /** y [mm]                             */ double y;
  /** \TEX{$\phi$} [mrad]                */ double ph;
  /** x (dispersive) [mm]                */ double x_decay;
  /** \TEX{$\theta$} (dispersive) [mrad] */ double th_decay;
  /** y [mm]                             */ double y_decay;
  /** \TEX{$\phi$} [mrad]                */ double ph_decay;
};

/** Coordinates of the point of scattering */
struct ScatCoord  { 
  /** x position of point of scattering */          double x;
  /** y position of point of scattering */          double y;
  /** z position of point of scattering */          double z;
  /** min. distance from VDC trace to HDC trace */  double dmin; 
  /** azimuthal angle of scattering */              double th; 
  /** polar angle of scattering */                  double ph; 
};

/** HDC plane information */
struct hdc_plane    { 
  /** drift time used for position calculation */ int pos_time;
  /** shortest drift time */                      int low_time;
  /** number of postime wire */                   int wire; 
  /** multiplicity */                             int multi;
  /** odd-even information */                     int oe; 
  /** ok value of HDC plane */                    int ok;
  /** x position [mm] */                          double x;
  /** distance from signal wire */                double xcell; 
};

/** Focal plane polarisation information */
struct fpp          { 
  /** horizontal driftchamber planes */        struct hdc_plane   u1,v1,u2,v2;
  /** coordinates in HDC system */             struct FocalCoord  hdc;
  /** HDC ok value */                          int                hdcOK;
  /** VDC track transformed into HDC system */ struct FocalCoord  vth;
  /** difference between VTH and HDC coords */ struct FocalCoord  dvh;
  /** coordinates of point of scattering */    struct ScatCoord   scat;
  /** particle's path from VDC to HDC */       double r;
  /** analyzing power */                       double anapow;
  /** kin. energy in the middle of the carbon*/double T_cc;
  /** return value of the ConeTest */          int ConeTest;
                    };

/** Spectrometer data */
struct Spectrometer { 
  /** Time of Flight scintillator layer */ struct scinti ToF;
  /** dE/dx scintillator layer          */ struct scinti dE;
  /** reconstructed coordinates         */ struct TargetCoord target;
  /** vertical driftchamber x1          */ struct vdc x1;
  /** vertical driftchamber s1          */ struct vdc s1;
  /** vertical driftchamber x2          */ struct vdc x2;
  /** vertical driftchamber s2          */ struct vdc s2;
  /** Flag for reconstruction status    */ int vdcOK;
  /** VDC status                        */ short vdcStatus;
  /** Cerenkov detektor                 */ struct cerenkov    Cerenkov;
  /** vertex reconstructed by THIS 
    spectrometer relativ to hall system */ struct vertex vertex;
  /** trigger information != 0 for hits */ int trigger;
  /** momentum 4 vector reconstructed   */ int p4_recon;
  /** Target coordinate reasonable      */ int target_coor_ok;
  /** Hit position x in scintillator    */ double x_scint;
  /** Hit position y in scintillator    */ double y_scint;
  /** Number of paddle from pattern     */ int Index_pattern;
  /** Number of paddle from x           */ int Index_x;
  /** Time between ToF/dE layer [ns]    */ double dE_ToF;
  /** Time between left/right side [ns] */ double Time_Left;
  /** Raw time left                     */ double raw_Time_Left;
  /** raw time right                    */ double raw_dE_ToF;
  /** Time with all corrections         */ double TimeAtTarget;
  /** Time with scint corrections only  */ double ScintCorr;
  /** Time shift due to extended target */ double Extended;
  /** Time between FP and trigger       */ double ScintDist; 
  /** Rawtime btw. Single + bunch [ns]  */ double single_tdc;
  /** Corrtime btw. Single + bunch [ns] */ double singlecorr;
  /** x after focus correction          */ double x;
  /** y after focus correction          */ double y;
  /** theta after focus correction      */ double theta;
  /** phi after focus correction        */ double phi;
  /** length after focus correction     */ double len;
  /** Intersection point of fit though the
   *  upper/lower chamber packet        */ double z_fp;
  /** Error of reconstructed x          */ double dx; 
  /** Error of reconstructed y          */ double dy; 
  /** Error of reconstructed th         */ double dtheta; 
  /** Error of reconstructed ph         */ double dphi;
  /** applied energy loss correction [GeV]  */ double Eloss_corr;  
  /** simulated energy loss [GeV]           */ double Eloss_sim;
  /** simulated focal-plane coordinates */ struct simFocalCoord simfp;
};

/** Scintillator data */
struct scintiD       { 
  /** Energy left/right multiplier       */ struct{double front, back;} energy;
  /** Energy left/right multiplier       */ struct{double front, back;} time;
};

/** Vertical drift chamber */
struct vdcD          { 
  /** Raw information of wires with signal */ int raw_time[16];
  /** Raw information of wires with signal */ int raw_wire[16]; 
  /** Time - placeholder                   */ int time; 
  /** Wire - placeholder                   */ int wire; 
  /** Multiplicity                       */ int multi;
  /** Error code, see chamber library    */ int error;
  /** Wire pattern, see chamber library  */ int pattern; 
  /** reconstructed position             */ double x; 
  /** reconstructed slope                */ double slope;
  /** \TEX{$\chi^2$} of straight line fit*/ double chi_sqr; 
  /** Length - placeholder                 */ double length; 
  /** Raw information of wires with signal */ double raw_length[16];
  /** Distance Wire-Reconstructed Track    */ double residuum;
  /** Like residuum but without Staggering */ double resid;
};

/** pion Spectrometer data */
struct SpectrometerD { 
  /** Scintillator for 5 layers         */ struct scintiD layer[5];
  /** reconstructed coordinates         */ struct TargetCoord target;
  /** vertical driftchamber x           */ struct vdcD planeX;
  /** vertical driftchamber y           */ struct vdcD planeY;
  /** Flag for reconstruction status    */ int pdcOK;
  /** vertex reconstructed by THIS 
    spectrometer relativ to hall system */ struct vertex vertex;
  /** trigger information != 0 for hits */ int trigger;
  /** Hit position x in scintillator    */ double x_scint;
  /** Hit position y in scintillator    */ double y_scint;
  /** Time with all corrections         */ double TimeAtTarget;
  /** x after focus correction          */ double x;
  /** y after focus correction          */ double y;
  /** theta after focus correction      */ double theta;
  /** phi after focus correction        */ double phi;
  /** length after focus correction     */ double len;
  /** Error of reconstructed x          */ double dx; 
  /** Error of reconstructed y          */ double dy; 
  /** Error of reconstructed th         */ double dtheta; 
  /** Error of reconstructed ph         */ double dphi;
  /** applied energy loss correction [GeV]  */ double Eloss_corr;  
  /** simulated energy loss [GeV]           */ double Eloss_sim;
};

/** Neutron detector */
struct NdetBar {
  double EUp;
  double EDown;
  double Gmean;
  double tUp;
  double tDown;
  double Position;
  double AdcUpCorr;
  double AdcDownCorr;
  double AdcParticlePeakUp;
  double AdcParticlePeakDown;
  double GmeanParticlePeak;
  double AdcPedestalUp;
  double AdcPedestalDown;
  int hit;
  double TdcMulti1Distance;
  double TdcUp;
  double TdcDown;
};

struct NdetdEBar {
  double time;
  int veto;
};


struct NdetdE { //bss
  //  double ECorr[9];
  //  double TCorr[9];
  //  int Veto[9];
  struct NdetdEBar bar[10];
};

struct NdetLayer {
  struct NdetBar bar[6];
  double highest_layer_gmean;
  double sum_layer_gmean;
  double BestTdcMulti1Distance;
};

struct Ndet { 
  /** Trigger in neutron detector       */ int    trigger;
  /** Time with all corrections         */ double TimeAtTarget;
  /** Number of paddle                  */ short  paddle;
  /** */                                   int multi;
  /** */                                   int pattern;
  /** */                                   int charged;
  /** */				   struct NdetLayer layer[7];
  /** reconstructed coordinates         */ struct TargetCoord target;
  /** simulated focal-plane coordinates */ struct simFocalCoord simfp;
  /** bss*/                                struct NdetdE dE;
  /** bss*/                                double highest_ndet_gmean;
  /** bss*/                                double sum_ndet_gmean;
  /** bss*/                                int temp_check;
  /** bss*/                                double highest_ndet_26gmean;
  /** bss*/                                double detected_energy;
  /** bss*/                                double BestTdcMulti1Distance;
  /** bss T_AN-A.TimeAtAtarget*/           double TimeOfTrigger;
};

/** Neutron polarimeter (npol) */

struct SnpolPaddle {
  /** debug information (-> npol.cc)  */  int              allOk;
  /** cut on adc1                     */  short            adc1_cut;
  /** cut on adc2                     */  short            adc2_cut;
  /** cut on Pulsheigh                */  short            pulsh_cut; 
  /** cut on tdc1                     */  short            tdc1_cut;
  /** cut on tdc2                     */  short            tdc2_cut;
  /** cut on Time                     */  short            time_cut; 
  /** a1_cut*a2_cut*t1_cut*t2_cut     */  short            hit_cut;
  /** cut on hitpaddle                */  short            hitpaddle_cut;
  /** ADC1 with all corrections       */  double           adc1;
  /** ADC2 with all corrections       */  double           adc2;
  /** TDC1 with all corrections       */  double           tdc1;
  /** TDC2 with all corrections       */  double           tdc2;
  /** pulsheight == Sqrt(adc1*adc2)   */  double           pulsh;
  /** TDC difference == tdc1 - tdc2   */  double           diff;
  /** line ~ ln(ADC1/ADC2)            */  double           line;             
  /** TDC sum == (tdc1 + tdc2) / 2    */  double           time;
  /** Time of Flight                  */  double           tof;
  /** Scaler in Hz                    */  double           scaler;
  /** Hit position in npol coord.     */  double           npolCoord[3];
  /** Hit position in lab coord.      */  double           labCoord[3];
  /** beta == v/c                     */  double           beta;
  /** SoL in Scintillator [c_vak]     */  double           c_eff;
  /** Attenuation length [cm]         */  double           lambda;
};

/* All arrays of detectors have one detector more then the physical
   setup. This additional virtual detector has always the index 0 in
   the array (the real detectors starts with the index one !!!) and
   his entries (-> struct SnpolPaddle) are equal to the entries of
   the hitpaddle, which was selected by Cola. -> npol.cc

   short cut: hitpaddle(s) == the paddle(s) with positiv hit_cut

   The variables "vetohit" and "ndethit" contains the numbers of
   all hitpaddles. The vector field zero contains the paddle 
   number of the hitpaddle, which was selected by Cola.
*/

/** This structure is part of struct SnpolWall2  (see below) */
struct SnpolFrame {
  /** vetos                           */  struct SnpolPaddle   veto[5];
  /** ndets                           */  struct SnpolPaddle   ndet[13];
  /** hit ndets. glazier              */  struct SnpolPaddle   hitPad[8];
  /** numbers of the veto hitpaddles  */  int                  vetohit[5];
  /** numbers of the ndet hitpaddles  */  int                  ndethit[13];
  /** Multiplicity of vetos (Hit_cut) */  short                vetomulti;
  /** Multipl. of veto (data complete)*/  short                rawvetomulti;
  /** Multiplicity of ndets (Hit_cut) */  short                ndetmulti[3];  
  /** Multipl. of ndet (data complete)*/  short                rawndetmulti[3];
  /** Multipl. of "good hits"         */  short                triggermulti[3];
};

struct SnpolWall2 {
  /** upper part of 2nd wall          */  struct SnpolFrame    top;
  /** lower part of 2nd wall          */  struct SnpolFrame    bottom;
  /** veto hitpaddle                  */  struct SnpolPaddle   vhitpaddle;
  /** ndet hitpaddle                  */  struct SnpolPaddle   nhitpaddle;
  /** number of the veto hitpaddle    */  int                  vetohit;
  /** number of the ndet hitpaddle    */  int                  ndethit;
  /** "Software trigger" (-> npol.cc) */  short                eventpattern;
 };

struct SnpolWall1 {
  /** vetos                           */  struct SnpolPaddle   veto[16];
  /** ndets                           */  struct SnpolPaddle   ndet[31];
  /** hit ndets.  glazier             */  struct SnpolPaddle   hitPad[10];
  /** numbers of the veto hitpaddles  */  int                  vetohit[16];
  /** numbers of the ndet hitpaddles  */  int                  ndethit[31];
  /** "Software trigger" (-> npol.cc) */  short                eventpattern;
  /** Multiplicity of vetos (Hit_cut) */  short                vetomulti;
  /** Multipl. of veto (data complete)*/  short                rawvetomulti;
  /** Multipl. (double hits in ndets) */  short                vetoPhits;
  /** Multiplicity of ndets (Hit_cut) */  short                ndetmulti[2];
  /** Multipl. of ndet (data complete)*/  short                rawndetmulti[2];
  /** Multipl. of "good hits"         */  short                triggermulti[2];
  /** Correction for pedestal oscill. */  double               ADCjitter; 
  /** integral B dx. glazier          */  double               fieldintegral[10];
};

struct SnpolMagnet {
  /** integral B dx                   */  double               fieldintegral;
  /** Reconst. Coordinates at the     */  double               frontCoord[2];
  /** front of the magnet             */
};

struct SnpolFSI {
  /** Polarisation components         */  float                Px_Born;
  /**                                 */  float                Py_Born;
  /**                                 */  float                Pz_Born;
  /**                                 */  float                Px_Full;
  /**                                 */  float                Py_Full;
  /**                                 */  float                Pz_Full;
};

struct Snpol {
  /** 1st wall                        */  struct SnpolWall1    wall1;
  /** 2nd wall                        */  struct SnpolWall2    wall2;
  /** trigger detectors               */  struct SnpolPaddle   trigger[4];
  /** Dipol magnet                    */  struct SnpolMagnet   magnet;     
  /** FSI corrections                 */  struct SnpolFSI      fsi;
  /** Interruptrate                  .*/  double               interruptrate;
  /** r,theta,phi in scatter coordsys.*/  double               scatterCoord[3];
  /** ToF between 1st and 2nd wall    */  double               tof12;
  /** beta (between 1st and 2nd wall) */  double               beta12; 
  /** "Software trigger" (-> npol.cc) */  short                eventpattern;
  /** Trigger detector TDCs           */  unsigned short       triggerTDC[9];
  /** Angles for FSI correction       */  double               Theta_np;
  /**                                 */  double               Phi_R;
  /**                                 */  double               Theta_nq;
};

struct SiliconOnline {
  /** silicon theta                   */  double si_theta;
  /** silicon phi                     */  double si_phi;  
  /** Scintillator energy             */  double E_scint;
  /** BB2 energy front                */  double E_bb2a;
  /** BB2 energy back                 */  double E_bb2b;
  /** MSX energies                    */  double E_MSX[5];
  /** fADC energies                   */  double E_fADC[6];
  /** VETO energy                     */  double E_VETO;
  /** BB2a strip energies             */  double E_A[24];
  /** BB2b strip energies             */  double E_B[24];
  /** Particle Energy                 */  double EnergyParticle; 
  /** Particle Mometum                */  double MomentumParticle; 
  /** Particle theta                  */  double ThetaParticle;
  /** Particle phi                    */  double PhiParticle;
  /** P_fadc Energy                   */  double EnergyP_fadc; 
  /** P_fadc Momentum                 */  double MomentumP_fadc;  
  /** Target Energy loss correction   */  double Eloss_corr;
  /** Particle OK                     */  unsigned short OK;
  /** Simulated theta                 */  double simtheta;
  /** Simulated phi                   */  double simphi;
  /** Simulated energy                */  double simenergy;
  /** Simulated momentum              */  double simmomentum;
  /** V1724 channels                  */  double v1724energy[6];
  /** Reconstructed energies          */  double E_rec[3];
};

///////////////////////////////////////////////////////////////////////////////////
// Kaos spectromter 
///////////////////////////////////////////////////////////////////////////////////

struct tofhits {
  /** paddle multiplicity in cluster  */ short  clusterSize;
  /** cluster multiplicity            */ short  cluster;
  /** hit paddle no.                  */ double paddle;
  /** xz hit position [mm] along plane*/ double positionXZ;
  /** y hit position [mm]             */ double positionY;
  /** total energy loss [MeV]         */ double dE;
  /** specific energy loss [MeV/cm]   */ double dEdx;
  /** raw time [ns]                   */ double time;
  /** delta t between pad. in cluster */ double deltaTimeCluster;
  /** online spectra: charges         */ double qtop[30];
  /** online spectra: charges         */ double qbot[30];
  /** online spectra: times           */ double ttop[30];
  /** online spectra: times           */ double tbot[30];
};

struct aerohits {
  /** online spectra: charges         */ double ACqtop[12];
  /** online spectra: charges         */ double ACqbot[12];
  /** online spectra: times           */ double ACttop[12];
  /** online spectra: times           */ double ACtbot[12];
};

struct toftracks {
  /** angle in dispersive plane [deg] */ double theta;
  /** angle in non-disp. plane [deg]  */ double phi;
  /** track multiplicity              */ short  tracks;
  /** aerogel hit segment no.         */ double AC1segment;
  /** aerogel ADC value in hit segment*/ double AC1HitADC;
  /** aerogel XZ position             */ double AC1positionXZ;
  /** aerogel Y position              */ double AC1positionY;
  /** aerogel hit segment no.         */ double AC2segment;
  /** aerogel ADC value in hit segment*/ double AC2HitADC;
  /** aerogel XZ position             */ double AC2positionXZ;
  /** aerogel Y position              */ double AC2positionY;
  /** tagger bar no.                  */ short  tagger;
  /** tracklength between walls [mm]  */ double tracklength;
  /** tracklength between wallsI&H[mm]*/ double tracklengthIH;
  /** flighttime between walls [ns]   */ double flighttime;
  /** flighttime between walls I&H[ns]*/ double flighttimeIH;
  /** quality tagger data             */ double q_tagger;
  /** quality aerogel data            */ double q_aerogel;
  /** quality phi                     */ double q_phi;
  /** quality theta                   */ double q_theta;
  /** quality dEdx                    */ double q_dEdx;
  /** quality TOF                     */ double q_TOF;
  /** quality track G and I           */ double q_GIx;
  /** quality MWPC track to wall H    */ double q_xHx;
  /** quality MWPC track to wall G    */ double q_xGx;
  /** quality                         */ double quality;
};

struct fibrehits {
  /** Channel of hit fibre            */ int    hitLeadingChannel;
  /** Time of hit in ns               */ double hitLeadingTime;
  /** Time of last hit in cluster     */ double hitTrailingTime;
  /** Lowest channel of best cluster  */ int    hitMinChannel;
  /** Heighest channel of best cluster*/ int    hitMaxChannel;
  /** Hit multiplicity of best cluster*/ int    hitMultiplicity;
  /** Total Multiplicity              */ int    Multiplicity;
  /** Number of clusters              */ int    numberClusters;
  /** Number of signals in clusters   */ int    typeCluster;
  /** Number of afterpulses           */ int    typeAfterpulse;
  /** Number of single signals        */ int    typeSingle;
  /** optional variable for channels  */ int    channels;
  /** optional variable for times     */ double times;
  /** Mean time in cluster            */ double timeMean;
  /** RMS time in cluster             */ double timeRMS;
  /** RMS length in cluster           */ double lengthRMS;
  /** Nr of raw fibre                 */ short  NrofFibre;
  /** Time of raw fibre               */ short  TimeofFibre;
  /** Nr of raw fibre in Cluster      */ short  ClusterNrofFibre;
  /** Time of raw fibre in CLuster    */ short  ClusterTimeofFibre;
  /** Online spectra: times           */ double time[2336];
  //  /** No of singles for every PMT     */ int    pmtSingles[72];
  //  /** No of afterpulses for every PMT */ int    pmtAfterpulses[72];
  //  /** No of clusterhits for every PMT */ int    pmtClusterHits[72];
};

struct external {
  /** time in external detectors      */ double time[8];
  /** charge in external detectors    */ double charge[8];
};

struct mwpchits {
  /** Nr of raw ADC                   */ short  NrofRAWADC;
  /** Nr of ADC after threshold       */ short  NrofADC;
  /** Charge of ADC after threshold   */ short  ChargeofADC;
  /** hit multiplicity                */ short  multi;
  /** cluster multiplicity            */ short  cluster;
  /** reconstructed hit position      */ double hit;
  /** error hit position              */ double hiterr;
  /** hit peak width                  */ double hitwidth;
  /** highest ADC value in hit peak   */ double hitmaxvalue;
  /** collected charge in cluster     */ double charge;
};

struct KFocalCoord { 
  /** x (dispersive) [mm]             */ double x;
  /** theta (dispersive) [mrad]       */ double th;
  /** y [mm]                          */ double y;
  /** phi [mrad]                      */ double ph;
  /** x_i (intersection point) [mm]   */ double x_i;
  /** z_i (intersection point) [mm]   */ double z_i;
};

struct layers {
  /** layers x,y                      */ struct mwpchits x, y;
};

struct MWPC {
  /** two wire chambers               */ struct layers l, m;
  /** angle in dispersive plane (deg) */ double theta;
  /** angle in non-disp. plane (deg)  */ double phi;
  /** track multiplicity              */ short  tracks;
  /** track quality                   */ double quality;
  /** 2nd best track quality          */ double sndquality;
  //  /** quality charge Lx               */ double qualchargeLx;
  //  /** quality charge Ly               */ double qualchargeLy;
  //  /** quality charge Mx               */ double qualchargeMx;
  //  /** quality charge My               */ double qualchargeMy;
  /** quality charge ratio L          */ double qualcL;
  /** quality charge ratio M          */ double qualcM;
  /** quality theta                   */ double qualtheta;
  /** quality phi                     */ double qualphi;
  /** quality wall F                  */ double qualwallF;
  /** quality wall G                  */ double qualwallG;
  /** quality time F                  */ double qualtimeF;
  /** quality time G                  */ double qualtimeG;

};

struct multihits {
  /** ID of current track (1..n)      */ int    trackID;
  // entries for tracks containing a two paddle hit
  /** additional paddle hit in H      */ int    twoPaddleHitH;
  /** additional paddle hit in G      */ int    twoPaddleHitG;
  /** ..                              */ double deltaPositionHy;
  /** ..                              */ double deltaPositionGy;
  /** ..                              */ double deltaTimeH;
  /** ..                              */ double deltaTimeG;
  // entries for tracks next to a two pladdle hit
  /** 1 for lower end, 2 for higher   */ int    nextTo2PaddleHitH;
  /** 1 for lower end, 2 for higher   */ int    nextTo2PaddleHitG;
  /**                                 */ double deltaTLowerH;
  /**                                 */ double deltaTHigherH;
  /**                                 */ double deltaTLowerG;
  /**                                 */ double deltaTHigherG;
  /**                                 */ double deltaYLowerH;
  /**                                 */ double deltaYHigherH;
  /**                                 */ double deltaYLowerG;
  /**                                 */ double deltaYHigherG;
};

struct fibres {
  /** trigger information != 0 for hits */ int trigger;
  /** two fibre planes                  */ struct fibrehits x, theta;
  /**                                   */    int numberTracks;
  /**                                   */    double angle;
  /**                                   */    double flightpath;
  /**                                   */    double quality;
  /** focal plane coordinates           */ struct KFocalCoord coord;
  /** reconstructed coordinates         */ struct TargetCoord target;
  /** external channels                 */ struct external ext;
};

struct SpectrometerK {
  /** trigger information != 0 for hits */ int trigger;
  /** three time-of-flight walls        */ struct tofhits hdet, gdet, idet;
  /** combined TOF system               */ struct toftracks tof;
  /** wire chambers                     */ struct MWPC mwpc;
  /** aerogel Cerenkov                  */ struct aerohits aerogel;
  /** multitrack analysis               */ struct multihits multitrack;
  /** fibre detector                    */ struct fibres scifi;
  /** focal plane coordinates           */ struct KFocalCoord coord;
  /** reconstructed coordinates         */ struct TargetCoord target;
  /** external channels                 */ struct external xtra;
  /** time with corrections             */ double TimeAtTarget;
  /** coincidence time with corrections */ double CoincTimeAtTarget;
  /** applied energy loss correction [GeV]  */ double Eloss_corr;  
  /** simulated energy loss [GeV]           */ double Eloss_sim;
};

struct Beam {
  /** Beam helicity [+1 or -1]             */ int    helicity;
  /** applied beam energy loss correction  */ double Eloss_corr;
  /** simulated beam energy loss           */ double Eloss_sim;
};  

struct simulation {
  /** Virtual photon flux                  */ double Gamma;
  /** Vertex Q2                            */ double VertexQ2; 
  /** Integration Volume                   */ double Volume;
  /** Total Weight for this event          */ double weight;
  /** Focal Plane polarization x           */ double fppx;
  /** Focal Plane polarization y           */ double fppy;
  /** Focal Plane polarization z           */ double fppz;
  /** Center of Mass Spin x                */ double cmpx;
  /** Center of Mass Spin y                */ double cmpy;
  /** Center of Mass Spin z                */ double cmpz;
  /** Lab System Polarization x            */ double labpx;
  /** Lab System Polarization y            */ double labpy;
  /** Lab System Polarization z            */ double labpz;
  /** Scaling factor for this event        */ double scale;
  /** Polar angle of a Hard photon         */ double ThetaGammaHardCMS;   
  /** Azimuthal angle of a Hard photon     */ double PhiGammaHardCMS;     
  /** value of norm()                      */ double norm;
  /** value of psrandom()                  */ double normunity;
};


struct NCubeHit {
  /** */ double time;
  /** */ double width;
};

struct NCubeBar { 
  /** */ struct NCubeHit left;
  /** */ struct NCubeHit right;
};

struct NCubeLayer {
  /** */ struct NCubeBar bar[64];
};

struct NCubeMHit {
  /** */ int    layer;
  /** */ int    bar;
  /** */ int    side;
  /** */ double time;
  /** */ double width;
};

struct NCube {
  /** */ struct NCubeLayer layer[2];
  /** */ int               multiplicity;
  /** */ struct NCubeMHit  hit[256];
  /** */ double            fingerTime;
  /** */ double            fingerWidth;
};

struct NeutronDetector2016 {
  int trigger;
};

/** Complete tree of reconstructed data */
struct online       { 
  /** Event number                      */ int nr;
  /** Raw time between A, B [ns]        */ double ab_tdc;
  /** Raw time between B, C [ns]        */ double bc_tdc;
  /** Raw time between C, A [ns]        */ double ca_tdc;
  /** Corrected time between A, B [ns]  */ double abcorr;
  /** Corrected time between B, C [ns]  */ double bccorr;
  /** Corrected time between C, A [ns]  */ double cacorr;
  /** Unix time of the event            */ double datataking;
  /** Unix time of the event since start*/ double datataking2;
  /** Integrated Luminosity             */ double IntegratedLuminosity;
  /** Dead time fraction for singles A  */ double Dead_A;
  /** Dead time for singles in B        */ double Dead_B;
  /** Dead time for singles in C        */ double Dead_C;
  /** Dead time for doubles in AB       */ double Dead_AB;
  /** Dead time for doubles in AC       */ double Dead_AC;
  /** Dead time for doubles in BC       */ double Dead_BC;
  /** Dead time for triples in ABC      */ double Dead_ABC;
  /** Total run time [s]                */ double RunTime;
  /** Vertex pos. rel. to target system */ struct vertex Vertex;
  /** Spectrometer data in A            */ struct Spectrometer A; 
  /** Spectrometer data in B            */ struct Spectrometer B; 
  /** Spectrometer data in C            */ struct Spectrometer C;
  /** Spectrometer data in D            */ struct SpectrometerD D;
  /** Neutron detector                  */ struct Ndet N, N2;
  /** neutron polarimeter               */ struct Snpol npol;
  /** Beam parameters                   */ struct Beam Beam;
  /** Focal plane polarimeter in A      */ struct fpp ppolA;
  /** Simulation variables              */ struct simulation sim;
  /** Silicon detector                  */ struct SiliconOnline si;
  /** Kaos detectors                    */ struct SpectrometerK kaos;
  /** Neutron Detector                  */ struct NCube ncube;
  /** Neutron detector 2016             */ struct NeutronDetector2016 ndet;
};
/*@}*/
extern struct online online;

#endif
