// "npol.h"
// This is the header file for "npol.cc" and "npolPrint.cc"

//$Id: npol.h 2216 2008-06-13 21:13:47Z distler $
// modified 2002/04/03 M. Seimetz

#include "Aqua_abcn.h"
#include "Aqua_online.h"
#include "QuasiRandom.h"   // -> random numbers for hit scaling
#include "rundatabase.h"   // "run.db" Parser
#include "Masses.h"        // -> Class Particle
#include <stdlib.h>
#include <string.h>

//Shortcut for rundb variables
#define rn rundb.npol

//default error value
#define errorVal -1717


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//:::Class for Config. files::::::::::::::::::::::::::::::::::::::::::::::::://
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
class CnpolConfig {
public:
  double col1[80],col2[80],col3[80],col4[80];
  
  CnpolConfig(char filename[256], short cols, int *verbose) {
    for (int i=0; i<80; i++) col1[i]=col2[i]=col3[i]=col4[i]=0;
    if (!this->readConfigFromFile(filename,cols,verbose)) exit(1);
  };

private:
  int readConfigFromFile(char filename[256],short anzahl,int *verbose);
};


class CnpolConfigFiles {
public:
  CnpolConfig *position;         // Position of paddle relativ to fixed origin
  CnpolConfig *pedestal;         // Pedestals for ADCs
  CnpolConfig *tdcCut;           // Limits for TDC cuts
  CnpolConfig *adcCut;           // Limits for ADC cuts
  CnpolConfig *timeCut;          // Limits for Time cuts
  CnpolConfig *pulshCut;         // Limits for Pulsheigh cuts
  CnpolConfig *tdcDiff;          // SoL in Paddle and Diff0 (Offset of TDC_diff)
  CnpolConfig *tdcSum;           // Offset of Time (TDC_Sum), for ToF calculation
  CnpolConfig *tdcWalk;          // Coefficients for walk correction
  CnpolConfig *adcRatio;         // Coefficients for "line" reconstruction
  CnpolConfigFiles(int *verbose) {
    position  = new CnpolConfig(rn.file_position,3,verbose);
    pedestal  = new CnpolConfig(rn.file_pedestal,4,verbose);
    tdcCut    = new CnpolConfig(rn.file_tdcCut,4,verbose);
    adcCut    = new CnpolConfig(rn.file_adcCut,4,verbose);
    timeCut   = new CnpolConfig(rn.file_timeCut,2,verbose);
    pulshCut  = new CnpolConfig(rn.file_pulshCut,2,verbose);
    tdcDiff   = new CnpolConfig(rn.file_tdcDiff,2,verbose);
    tdcSum    = new CnpolConfig(rn.file_tdcSum,1,verbose);
    tdcWalk   = new CnpolConfig(rn.file_tdcWalk,2,verbose);
    adcRatio  = new CnpolConfig(rn.file_adcRatio,2,verbose);
  };
};


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//:::Class for npol Magnet::::::::::::::::::::::::::::::::::::::::::::::::::://
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
const int npm_xdim=31;  // -(npm_xdim+0.5) <= x_magnet <= (npm_xdim+0.5)
const int npm_ydim=16;  // -(npm_ydim+0.5) <= y_magnet <= (npm_ydim+0.5)

class CnpolMagnet {
public:
  struct SnpolMagnet  *onl;                     // Pointer to online structure
  double integral[2*npm_xdim+1][2*npm_ydim+1];  // array of fieldintegrals
  double position[3];                           // origin of magnet (front) 
  double RundbCurrent;
  char   filename[512];                         // Current given in Run.db
  // double *MezzoCurrent;                      // Current from Mezzo data 

  CnpolMagnet(struct SnpolMagnet *online, int *verbose) {
    onl=online;
    for (int i=0; i<3; i++) position[i] = rn.magnet_pos[i];
    RundbCurrent=rn.magnet_I;
    //MezzoCurrent=NULL;
    
    if (RundbCurrent !=0) {
      getfilename(rn.magnet_path);
      if (!readMagnetDataFromFile(verbose)) exit(1);
    };
  };
 
  void getMagnetCoords(AquaTree *out, double firstWpos[3]);
  void getFieldIntegral(AquaTree *out);

private:  
  void getfilename(char path[256]);
  short readMagnetDataFromFile(int *verbose);
  int getXindex(double x0);
  int getYindex(double y0);
};


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//:::Class for single scintillator paddle:::::::::::::::::::::::::::::::::::://
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//Modeling the npol setup with c++ classes//
class CnpolPaddle {
  
public:
  static int               padnr;         
  static double            startTime;           // start time for ToF   
  static CnpolConfigFiles  *conf;               // Ptr. config data 
  static double            ADCjitter;           // Correction for adc jitter 
  static double            interruptrate;       // Interrupt rate  

  struct SnpolPaddle  *onl;                     // Pointer to online structure
  int                 nr;                       // Paddle # (0-79)
  unsigned short      *adc1,*adc2,*tdc1,*tdc2;  // Pointer to raw data 
  unsigned int        *scaler;                  // Scaler 
  double              *tdcScale;                // TDC Scaling(ns/bin)
  double              *minADC;                  // Minimum for accepted ADC's
  double              pedlow1,pedlow2;          // Lower Range Pedestals
  double              pedup1,pedup2;            // Upper Range Pedestals
  double              adc1L,adc1H,adc2L,adc2H;  // Limits for ADCcuts  
  double              tdc1L,tdc1H,tdc2L,tdc2H;  // Limits for TDCcuts 
  double              timeL,timeH;              // Limits for Timecuts
  double              pulshL,pulshH;            // Limits for Timecuts 
  double              x,y,z;                    // Position (absolute)
  double              SoL;                      // Speed of Light [c_vak]
  double              Time0, Diff0;             // Time and Diff offset 
  double              walk1,walk2;              // walk correction
  double              lambda,deltaQ;            // attenuate length for ADCs
                                                // and Delta Q0
  int                 detector;                 // = 1 for v_1st
                                                // = 2 for n_1st 
                                                // = 3 for v_2nd
                                                // = 4 for n_2nd
                                                // = 5 for trigger
  double              *mu; // attenuation factor for walk calib. pars



  CnpolPaddle(struct SnpolPaddle *online, struct RnpolPaddle *rawdata,
                     double origin[3], short det) {
    onl = online;
    /* first nr is 0 */
    nr = padnr++;

    scaler = &(rawdata->scaler);
    x = conf->position->col1[nr];
    y = conf->position->col2[nr];
    z = conf->position->col3[nr];
    x+= origin[0];
    y+= origin[1];
    z+= origin[2];

    detector=det; 
    mu = &rn.mu; 
    tdcScale = &rn.tdcScale[detector-1];
    minADC = &rn.minADC[detector-1];
 
    SoL  = conf->tdcDiff->col1[nr];
    Diff0 =  conf->tdcDiff->col2[nr];
    Time0 =  conf->tdcSum->col1[nr];

    adc1 = &(rawdata->adc1);
    adc2 = &(rawdata->adc2);
    pedlow1 = conf->pedestal->col1[nr];
    pedlow2 = conf->pedestal->col2[nr];
    pedup1 = conf->pedestal->col3[nr];
    pedup2 = conf->pedestal->col4[nr];
    adc1L = conf->adcCut->col1[nr];
    adc1H = conf->adcCut->col2[nr];
    adc2L = conf->adcCut->col3[nr];
    adc2H = conf->adcCut->col4[nr];
    pulshL = conf->pulshCut->col1[nr];
    pulshH = conf->pulshCut->col2[nr];
    lambda = conf->adcRatio->col1[nr];
    deltaQ = conf->adcRatio->col2[nr];    
    
    tdc1 = &(rawdata->tdc1);
    tdc2 = &(rawdata->tdc2);
    tdc1L = conf->tdcCut->col1[nr];
    tdc1H = conf->tdcCut->col2[nr];
    tdc2L = conf->tdcCut->col3[nr];
    tdc2H = conf->tdcCut->col4[nr];
    timeL = conf->timeCut->col1[nr];
    timeH = conf->timeCut->col2[nr];  
    walk1 = conf->tdcWalk->col1[nr];
    walk2  = conf->tdcWalk->col2[nr];
  }; 
  
  int reconstruct(AquaTree *atree, AquaTree *out);
  void printInfo();
  short TDCandADC(AquaTree *atree);
  short onlyADC(AquaTree *atree);
};


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//:::Class for subframes of 2nd wall::::::::::::::::::::::::::::::::::::::::://
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
class CnpolFrame {
  PseudoRandom ran;
public:
  struct SnpolFrame *onl;          // pointer to online structure
  CnpolPaddle       *veto[5];
  CnpolPaddle       *ndet[13];

/*double           *singleHit;     //F.K. 01/19/12 obsolete  
  double           *doubleHit[6];  // not used anymore in npol.cc
  double           *tripleHit[3];  */

  double            *vetoUse;
  double            *neighbourVetoUse;
  
  CnpolFrame(struct SnpolFrame *online, 
             struct RnpolFrame *rawdata, double origin[3])
   {
    onl = online;
/*  singleHit = &rn.W2_single; // obsolete
    for (int i=0; i<6; i++) doubleHit[i] = &rn.W2_double[i];
    for (int i=0; i<3; i++) tripleHit[i] = &rn.W2_triple[i]; */
    vetoUse = &rn.veto_use;
    neighbourVetoUse = &rn.n_veto_use;
    for (int i=1; i<5; i++)   // generate veto paddles
      veto[i] = new CnpolPaddle(&(onl->veto[i]),
                                &(rawdata->veto[i]),origin,3);
    for (int i=1; i<13; i++)  // generate ndet paddles
      ndet[i] = new CnpolPaddle(&(onl->ndet[i]),
                                &(rawdata->ndet[i]),origin,4);
  };
  int reconstruct(AquaTree *atree, AquaTree *out, int charged);

};


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//:::Class for 1st wall:::::::::::::::::::::::::::::::::::::::::::::::::::::://
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
class CnpolWall1 {
  PseudoRandom ran; 
public:
  struct SnpolWall1 *onl;
  CnpolPaddle       *veto[16];
  CnpolPaddle       *ndet[31];
 
/* double            *singleHit;   //F.K. 01/19/12 obsolete
   double            *doubleHit[2]; // not used anymore in npol.cc  */
  double            *vetoUse;
  double            *neighbourVetoUse;
  double            *maxjitter; 
  
  CnpolWall1(struct SnpolWall1 *online, struct Rnpol1stwall *rawdata){
    onl = online;
/*  singleHit = &rn.W1_single; // obsolete
    for (int i=0; i<2; i++) doubleHit[i] = &rn.W1_double[i]; */
    maxjitter = &rn.maxjitter; 
    vetoUse = &rn.veto_use;
    neighbourVetoUse = &rn.n_veto_use;
    for (int i=1; i<16; i++)   // generate veto paddle of 1st wall
       veto[i] = new CnpolPaddle(&(onl->veto[i]),
                                 &(rawdata->veto[i]), rn.W1_origin, 1);
    for (int i=1; i<31; i++)   // generate ndet paddles of 1st wall
       ndet[i] = new CnpolPaddle(&(onl->ndet[i]),
                                 &(rawdata->ndet[i]), rn.W1_origin, 2);
  };

  int reconstruct(AquaTree *atree, AquaTree *out, int charged);
  double getADCjitter(AquaTree *atree);
};


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//:::Class for total 2nd wall:::::::::::::::::::::::::::::::::::::::::::::::://
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
class CnpolWall2 {
  PseudoRandom ran; 
public:
  struct SnpolWall2 *onl;
  CnpolFrame        *top;
  CnpolFrame        *bottom;

  CnpolWall2(struct SnpolWall2 *online, struct Rnpol2ndwall *rawdata){
    onl = online;
// generate upper part of 2nd wall
    top    = new CnpolFrame(&(onl->top), 
                            &(rawdata->top), rn.Top_origin);        
// generate lower part of 2nd wall
    bottom = new CnpolFrame(&(onl->bottom), 
                            &(rawdata->bottom), rn.Bottom_origin);
  };

  int reconstruct(AquaTree *atree, AquaTree *out, int charged);
};


//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
//:::Class for total polarimeter::::::::::::::::::::::::::::::::::::::::::::://
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::://
class Cnpol {
public:
  
  CnpolWall1   *wall1;        // 1st wall of npol
  CnpolWall2   *wall2;        // 2nd wall fo npol
  CnpolPaddle  *trigger[4];   // trigger detectores
  CnpolMagnet  *magnet;       // magnet of npol  
  
  struct Snpol *onl;
  short npolInReaction;
  long int   Interrupts;

  unsigned int   *TimeStamp;    // Used for interrupt rate, 
  unsigned int   OldTimeStamp; // timestamps comes from data

  time_t        lastTime;      // Used for calculation time estimation, 
  time_t        ColaStartTime; // system time is used

  Cnpol(struct Rnpolraw *raw, struct Snpol *online, int *verbose) {
    npolInReaction=(strstr(rundb.reaction,"NPOL") != NULL);    
//Check if NPOL is specified in the Run.db reaction. If not nothing is done.
    if (npolInReaction) {
      Interrupts = OldTimeStamp = lastTime = 0;
      if (*verbose) lastTime=ColaStartTime=time(NULL);
      else  lastTime=ColaStartTime=errorVal;
      TimeStamp = &raw->clockticks; 
      checkRunDB();      // check if all needed information is given in Run.db 
      onl = online;          
      CnpolPaddle::conf = new CnpolConfigFiles(verbose); 
      wall1 = new CnpolWall1(&(onl->wall1),&(raw->wall1)); // create 1st wall 
      wall2 = new CnpolWall2(&(onl->wall2),&(raw->wall2)); // create 2nd wall
      trigger[1] = new CnpolPaddle(&(onl->trigger[1]), 
                                   &(raw->trigger[1]), rn.trig1_pos, 5);       
      trigger[2] = new CnpolPaddle(&(onl->trigger[2]),
                                   &(raw->trigger[2]), rn.trig2_pos, 5);
      trigger[3] = new CnpolPaddle(&(onl->trigger[3]),
                                   &(raw->trigger[3]), rn.trig3_pos, 5);
      magnet = new CnpolMagnet(&(onl->magnet),verbose);   // create  magnet
      if (*verbose) printInfo();  // save (Run.db) infos in file
      delete CnpolPaddle::conf;              
    };
  };

 int reconstruct(AquaTree *atree, AquaTree *out, Particle &particle, 
                  double tStart); 
  
private:
   void printInfo();
   void checkRunDB();
};
///THE END/////////////////////////////////////////////////////////////////

