//								      -*-c++-*-
// COLA: Analysis and simulation for A1 experiments
//
// Copyright (c) 1999-2001
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//


#include "FourVector/FourVector.h"   // relativistic four vector arithmetics
#include "Colli.h"                   // Definition of Collimators
#include "Cola/QuasiRandom.h"             // Random Generator
#include "Cola/include/online/online.h"        // 1st level reconstructed data
#include "Cola/Targetlength.h"       // Targetcells and Energyloss
#include "Cola/Luminosity.h"
#include "TMA/transandlook.h"
#include "spinTransfer.h"            // spin transfer target -> focal plane
#include "simSpecwithTrace.h"



#define   L	 12.0
#define DIST_X_1_2 272.0
#define L_2	 (2 * L)
extern int ergloss;
modeltype ModelType;
	
simSpecwithTrace::simSpecwithTrace(AquaTree *outtree, 
				 double ang, double angRes, double angRes2,
				 double vertiAngRatio,
				 double mom, double momRes, double momRes2, 
				 int ndp, double *dpc, 
				 double *ara, double *mra,
				 double refmom, double akz, 
				 const char *colli, Collimator* ColliList[],
				 int vac, Particle *p,
				 double *Eloss_corr, double *Eloss_sim,
				 double Dtheta, double Dphi, double Dmomentum,
				 double oopang)
  : simDetectorBase(outtree, p, ang, oopang, mom, Dtheta, Dphi, Dmomentum)
{
  sin_x_s   = sin(40 * M_PI / 180.0);
  sec_x_s   = 1.0 / sin_x_s;
  cos_x_s   = cos(40 * M_PI / 180.0);
  csc_x_s   = 1.0 / cos_x_s;
  tan_x_s   = tan(40 * M_PI / 180.0);
  cot_x_s   = 1.0 / tan_x_s;


  // should be init'd from run.db;
  li[0].maxwire=416;
  li[1].maxwire=352;
  li[2].maxwire=416;
  li[3].maxwire=352;
  
 
  li[0].poswireone=1420.57;
  li[1].poswireone=1161.04;
  li[2].poswireone=1772.57;
  li[3].poswireone=1430.69;

  li[0].slope=-5;
  li[1].slope=-5;
  li[2].slope=-5;
  li[3].slope=-5;

  for (int i=0; i<li[0].maxwire;i++) li[0].probability[i]=0.95;
  for (int i=0; i<li[1].maxwire;i++) li[1].probability[i]=0.95;
  for (int i=0; i<li[2].maxwire;i++) li[2].probability[i]=0.95;
  for (int i=0; i<li[3].maxwire;i++) li[3].probability[i]=0.95;

  li[0].probability[31]=0;
  li[0].probability[45]=0;
  li[0].probability[192]=0;
  li[0].probability[222]=0;
  li[0].probability[388]=0;

  li[1].probability[149]=0;
  li[1].probability[215]=0;
  li[1].probability[240]=0;

  li[2].probability[128]=0;
  li[2].probability[257]=0;
  li[2].probability[266]=0;

  li[3].probability[139]=0;
  li[3].probability[283]=0;




  vacuum = vac; 
  onlElossCorr = Eloss_corr;
  onlElossSim = Eloss_sim;
  steps = 1;
  if (ergloss > 1) {
    steps = 5;
    if (particle == P_electron) steps=  1;
    if (particle == P_proton)   steps= 10;
    if (particle == P_deuteron) steps=100;
  }
  momentumRes = momRes; 
  refmomentum = refmom;
  angularRes = angRes;

  momentumRes2 = momRes2; 
  angularRes2  = angRes2;
  ndpcuts = ndp;
  if (ndp) {
    mratio = new double[ndp];
    aratio = new double[ndp]; 
    dpcuts = new double[ndp];
    for (int i=0; i<ndp; i++) 
      {mratio[i]=mra[i]; aratio[i]=ara[i]; dpcuts[i]=dpc[i];}
  } 
  else {mratio=NULL; aratio=NULL; dpcuts=NULL;}
  
  verti = vertiAngRatio;

  acceptance = akz; 

  if (!colli[0]) 
    collimator = ColliList[0]; 
  else 
    for(int i=0; ColliList[i]; i++) 
      if (!strcmp(colli, ColliList[i]->getName()))
	collimator = ColliList[i];
  if (!collimator) {
    collimator = ColliList[0]; 
    std::cout << "Collimator \"" << colli << "\" unknown, using \""
	      << collimator->getName() << "\"." << std::endl;
  }
  if (!strcmp(collimator->getName(), "4pi"))     acceptance = 1000.0;
  if (!strcmp(collimator->getName(), "inPlane")) acceptance = 1000.0;

  std::cerr <<"FATAL: make transform-data a run.db variable"<<std::endl;
  exit(-1234);
  // work in progress, 
  //  transform.LoadFirstOrder("/users/kpha1/bernauer/data/datasets/solution/notrans.firstorder");
  //  transform.LoadSolution("/users/kpha1/bernauer/data/datasets/solution/4/855A_all_rev.solution");

}




int simSpecwithTrace::resolution(struct TargetCoord *tc)
{
  if (particle.getID() == 0) return -1;
  static PseudoRandom psrandom;
  static normal norm(&psrandom, &psrandom);

  double p  = particle.momentum(); 
  double E  = particle.energy(); 
  double theta = particle.theta();
  double phi   = particle.phi();
  double mRes = momentumRes;
  double aRes = angularRes;

  // dp-dependent resolutions
  if (mratio) {
    double dp = (p/(momentum/refmomentum)-1)*100.0;
    int i=0;
    while (dpcuts[i]<dp && i<ndpcuts-1) i++;
    if (psrandom()<mratio[i]) mRes = momentumRes2;
    if (psrandom()<aratio[i]) aRes = angularRes2;
  }
  double pn = p * (1 + norm() * mRes);

  double phires = (2 * psrandom() - 1) * M_PI;
  double thetares = norm() * aRes;

  // If vertical angle resolution is better than horizontal (Spec. B)
  if (verti < 1) {
    double sp = sin(phires)*verti;
    double cp = cos(phires);
    phires   = atan2(sp, cp);
    thetares *= sqrt(sp*sp+cp*cp);
  }  

  particle.initPolar(sqrt(E*E - p*p + pn*pn), pn, thetares, phires);
  particle.rot_theta(theta);
  particle.rot_phi(phi);

  tc->th = particle.specTheta(angle);
  tc->ph = -particle.specPhi(angle); // different sign in lib4vec!
  tc->dp = particle.specDelta(momentum/refmomentum);
  
  atree->packEventData(&(tc->th), 1);
  atree->packEventData(&(tc->ph), 1);
  atree->packEventData(&(tc->dp), 1);



  return 0;
}

int simSpecwithTrace::energyLossCorrection(target *target, double x[3]) 
{
  double EnergyBefore = 0;
  if (!particle.getCharge()) return -1;
  if (!ergloss) return -1;

  EnergyBefore = particle.energy();
  if (!vacuum) target->EnergyLossCorrChamber(particle);
  target->EnergyLossCorr(particle, x[0], x[1], x[2], steps);
  if (onlElossCorr) *onlElossCorr = (particle.energy() - EnergyBefore);

  return 0;
}
  
int  simSpecwithTrace::check(Particle vf, double x[3],
			   target *target, struct TargetCoord *tc)
{
  std::cerr<<"simSpecwithTrace::check is deprecated and non-functional. Please correct!"<<std::endl;
  return 0;

}






double simSpecwithTrace::checkwithtrace(Particle vf, double x[3],
					target *target, struct Spectrometer *onlS, modeltype ModelType)
{
  double EnergyBefore = 0;
  double probability=1;
  if (!collimator->accepted(x, vf, angle, oop, refmomentum)) return 0; 
  particle = vf;

  if (ergloss) {
    EnergyBefore = vf.energy();
    target->EnergyLossSim(particle, x[0], x[1], x[2], steps, ModelType);
    if (!vacuum) target->EnergyLossSimChamber(particle); 
    if (onlElossSim) *onlElossSim = (EnergyBefore - particle.energy());
  }
  if (fabs(particle.specDelta(momentum)) > acceptance) return 0;   

  onlS->target.th = particle.specTheta(angle);
  onlS->target.ph = -particle.specPhi(angle); // different sign in lib4vec!
  onlS->target.dp = particle.specDelta(momentum/refmomentum);
  onlS->target.y0 = (- x[0] * cos(angle) + x[2] * sin(angle))/10.0; 
  // this is only an approximation (i.e. ph==0) to initialize the variable
  // proper vertex coordinate y0 has to go here...

   double in[DEF_DIMIN];
  double out[DEF_DIMOUT];
  in[0]=onlS->target.dp;
  in[1]=onlS->target.th;
  in[2]=onlS->target.y0;
  in[3]=onlS->target.ph;

  transform.firstorderreconstruct(in);
  transform.lookup(in,out);
   
  onlS->x=out[0];
  onlS->theta=out[1];
  onlS->y=out[2];
  onlS->phi=out[3];
  onlS->len=out[4];

  // Do focalplane cuts


  probability*=SimulVDC(onlS);

  atree->packEventData(&(onlS->target.th), 1);
  atree->packEventData(&(onlS->target.ph), 1);
  atree->packEventData(&(onlS->target.dp), 1);
  atree->packEventData(&(onlS->target.y0), 1);

  atree->packEventData(&(onlS->x), 1);
  atree->packEventData(&(onlS->y), 1);
  atree->packEventData(&(onlS->theta), 1);
  atree->packEventData(&(onlS->phi), 1);
  atree->packEventData(&(onlS->len), 1);
  
  return probability;
}                                                                   


int simSpecwithTrace::pos2wire(int layer,double pos){
  int wire=(int)trunc((pos-li[layer].poswireone)/li[layer].slope+0.5);
  if (wire <0 ) return 0; 
  if (wire>=li[layer].maxwire) return li[layer].maxwire-1;
  return wire;
}




double  simSpecwithTrace::SimulVDC(struct Spectrometer *onlS){

  double prob=1;

  double slopex=-tan((onlS->theta/1E3)-M_PI_2);

  double slopes=sec_x_s/(cot_x_s/slopex-tan(onlS->phi/1e3)); 
  // hack for Spec A coming up!

  double posx1=onlS->x;
  double poss1= onlS->y/sec_x_s+onlS->x*cos_x_s+L_2/slopes;
  double posx2=posx1+DIST_X_1_2/slopex;
  double poss2=poss1+DIST_X_1_2/slopes;


  int wirex1=pos2wire(0,posx1);
  int wires1=pos2wire(1,poss1);
  int wirex2=pos2wire(2,posx2);
  int wires2=pos2wire(3,poss2);


  onlS->x1.wire=wirex1;
  onlS->s1.wire=wires1;
  onlS->x2.wire=wirex2;
  onlS->s2.wire=wires2;
  onlS->x1.x=posx1;
  onlS->s1.x=poss1;
  onlS->x2.x=posx2;
  onlS->s2.x=poss2;
  onlS->x1.slope=slopex;
  onlS->s1.slope=slopes;
  onlS->x2.slope=slopex;
  onlS->s2.slope=slopes;


  int rmultix1=0;
  int rmultis1=0;
  int rmultix2=0;
  int rmultis2=0;
  int i;
  int startwire,stopwire;

  // x1
  startwire=pos2wire(0,posx1+L/slopex);
  stopwire=pos2wire(0,posx1-L/slopex);
  if (startwire>stopwire) {int t=startwire;startwire=stopwire;stopwire=t;}
  for ( i=startwire;i<=stopwire;i++) if ((rand()*1.0/RAND_MAX)<li[0].probability[i]) rmultix1++;
  // s1
  startwire=pos2wire(1,poss1+L/slopes);
  stopwire=pos2wire(1,poss1-L/slopes);
  if (startwire>stopwire) {int t=startwire;startwire=stopwire;stopwire=t;}
  for ( i=startwire;i<=stopwire;i++) if ((rand()*1.0/RAND_MAX)<li[1].probability[i]) rmultis1++;
  // x2
  startwire=pos2wire(2,posx2+L/slopex);
  stopwire=pos2wire(2,posx2-L/slopex);
  if (startwire>stopwire) {int t=startwire;startwire=stopwire;stopwire=t;}
  for ( i=startwire;i<=stopwire;i++) if ((rand()*1.0/RAND_MAX)<li[2].probability[i]) rmultix2++;
  // s2
  startwire=pos2wire(3,posx1+L/slopex);
  stopwire=pos2wire(3,posx1-L/slopex);
  if (startwire>stopwire) {int t=startwire;startwire=stopwire;stopwire=t;}
  for ( i=startwire;i<=stopwire;i++) if ((rand()*1.0/RAND_MAX)<li[3].probability[i]) rmultis2++;


  //Globale totzeit?

  if ((rand()*1.0/RAND_MAX)<0.20) rmultix1=0;
  if ((rand()*1.0/RAND_MAX)<0.20) rmultis1=0;
  if ((rand()*1.0/RAND_MAX)<0.20) rmultix2=0;
  if ((rand()*1.0/RAND_MAX)<0.20) rmultis2=0;

  if (rmultix1+rmultix2<3) return 0;
  if (rmultis1+rmultis2<3) return 0;


  onlS->x1.multi=rmultix1;
  onlS->s1.multi=rmultis1;
  onlS->x2.multi=rmultix2;
  onlS->s2.multi=rmultis2;



  atree->packEventData(&(onlS->x1.wire), 1);
  atree->packEventData(&(onlS->s1.wire), 1);
  atree->packEventData(&(onlS->x2.wire), 1);
  atree->packEventData(&(onlS->s2.wire), 1);
  atree->packEventData(&(onlS->x1.x), 1);
  atree->packEventData(&(onlS->s1.x), 1);
  atree->packEventData(&(onlS->x2.x), 1);
  atree->packEventData(&(onlS->s2.x), 1);
  atree->packEventData(&(onlS->x1.slope), 1);
  atree->packEventData(&(onlS->s1.slope), 1);
  atree->packEventData(&(onlS->x2.slope), 1);
  atree->packEventData(&(onlS->s2.slope), 1);
  atree->packEventData(&(onlS->x1.multi), 1);
  atree->packEventData(&(onlS->s1.multi), 1);
  atree->packEventData(&(onlS->x2.multi), 1);
  atree->packEventData(&(onlS->s2.multi), 1);


  return prob;
}
