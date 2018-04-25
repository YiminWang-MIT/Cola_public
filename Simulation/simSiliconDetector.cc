//                                                                    -*-c++-*-
// $Id: simSiliconDetector.cc 2588 2014-10-30 10:31:33Z kohly $
//

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <iostream>
#include <fstream>
#include <iosfwd>
#include <cstdio>
#include "Aqua/AquaTree.h"
#include "Cola/Material.h"
#include "Cola/QuasiRandom.h" 
#include "Simulation/simSiliconDetector.h"

#include "Cola/Targetlength.h"  

using namespace std;
extern modeltype ModelType;

//Simulate ~50 % more acceptance (factor 1.5)
simSiliconDetector::simSiliconDetector(class AquaTree *outtree,Particle *p, double angle_, double oopang, \
				       double distance_, double centmom_, double simResolution_[8], double simAcceptScale_)
  : simDetectorBase(outtree, p, angle_, oopang, centmom_,		\
		    simAcceptScale_<=0 ? M_PI/2      : atan2(BB2_width/2,distance_)*simAcceptScale_, \
		    simAcceptScale_<=0 ? M_PI        : atan2(BB2_width/2,distance_)*simAcceptScale_, \
		    simAcceptScale_<=0 ? centmom_*0.98   : 0.5*Si_MomentumAcceptance*simAcceptScale_)
{
  distance = distance_;   //cm (distance from target)
  angle = angle_;
  Absorber_distance  = distance-2.0; 
  ResSci = simResolution_[0]/2.35;
  ResBB2 = simResolution_[1]/2.35;
  for (int i=0; i<5; i++) ResMSX[i] = simResolution_[i+2]/2.35;
  ResVeto = simResolution_[7]/2.35;

  AquaNode 
    **ea = reinterpret_cast<AquaNode**>(E_A), 
    **eb = reinterpret_cast<AquaNode**>(E_B);
  outtree->multifind(ea, 0, 23,              "online.si.E_A[%d]");
  outtree->multifind(eb, 0, 23,              "online.si.E_B[%d]");
  
  AquaNode **ms = reinterpret_cast<AquaNode**>(E_MSX);
  outtree->multifind(ms, 0, 4, "online.si.E_MSX[%d]");
  
  E_scint    = (AquaNodeDouble *)outtree->find("online.si.E_scint");  
  E_bb2    = (AquaNodeDouble *)outtree->find("online.si.E_bb2a");  
  E_VETO   = (AquaNodeDouble *)outtree->find("online.si.E_VETO");
  
  simtheta = (AquaNodeDouble *)outtree->find("online.si.simtheta");
  simphi = (AquaNodeDouble *)outtree->find("online.si.simphi");
  simenergy = (AquaNodeDouble *)outtree->find("online.si.simenergy");
  simmomentum = (AquaNodeDouble *)outtree->find("online.si.simmomentum");
  
  EnergyParticle = (AquaNodeDouble *)outtree->find("online.si.EnergyParticle");
  MomentumParticle = (AquaNodeDouble *)outtree->find("online.si.MomentumParticle");
  ThetaParticle = (AquaNodeDouble *)outtree->find("online.si.ThetaParticle");
  PhiParticle = (AquaNodeDouble *)outtree->find("online.si.PhiParticle");

  si_theta = (AquaNodeDouble *)outtree->find("online.si.si_theta");
  si_phi = (AquaNodeDouble *)outtree->find("online.si.si_phi");

  OK = (AquaNodeUShort *)outtree->find("online.si.OK");
  Eloss_corr = (AquaNodeDouble *)outtree->find("online.si.Eloss_corr");
  
  absorber = new Absorber(Point(0,0,-2.0)); 
  scint = new Scint(Point(0,0,-1.5));
  bb2 = new BB2(Point(0,0,0));
  for (int i=0;i<5;i++){
    msx[i] = new MSX(Point(0, 0, 0.20+i*0.32));
  }
  veto = new VETO(Point(0,0,0.20+2.50+0.10)); 
  
  move(Point(0, 0, distance),0);
  move(Point(0, 0, 0), angle);
  
  if (debugSI) siplot=new siPlot("siplot.ps");
}

simSiliconDetector::~simSiliconDetector() 
{
  delete scint;
  delete absorber;
  delete bb2;
  for (int i=0;i<5;i++) delete msx[i];
  delete veto;
  if (debugSI) siplot->close();
  if (debugSI) delete siplot;
}


bool simSiliconDetector::accept(Particle vf){
    double mom = this->getMomentum();
    double Dmom = this->getDmomentum();
    double theta = this->getAngle();
    //double phi = this->getOop();
    double Dtheta = this->getDtheta();
    double Dphi = this->getDphi();

    return (bool)vf.specCheck(mom-Dmom, mom+Dmom, theta, Dphi, Dtheta);
    
    //bool accept_p = (vf.momentum() > mom - Dmom) && (vf.momentum() < mom + Dmom);
    //bool accept_theta = (vf.theta() > theta - Dtheta) && (vf.theta() < theta + Dtheta);
    //bool accept_phi = (vf.phi() > phi - Dphi) && (vf.phi() < phi + Dphi);
    //return accept_p && accept_theta && accept_phi;
}

Point simSiliconDetector::getAngles(double XZ, double Y){
    double factor = -1/tan(M_PI-angle-atan2(XZ,distance));
    double pos[3];
    pos[0] = sqrt((distance*distance)/(1+factor*factor));
    pos[1] = Y;
    pos[2] = factor*pos[0];
    Point pp(pos[0],pos[1],pos[2]);
    return pp;
}

FourVector simSiliconDetector::initparticle(double m, double p, double th0, double th, double ph)
{
  double tan_th = tan(th);
  double tan_ph = tan(ph);
  double p_z = p / sqrt(1 + square(tan_th) + square(tan_ph));

  FourVector part(energy(p,m), p_z * tan_th, p_z * tan_ph, p_z);
  part.rot_theta(th0);

  return part;
}

void simSiliconDetector::setSteps(double E){
    //if (E<0.002) steps=500;
    //else if (E<0.015) steps=400;
    //else if (E<0.030) steps=300;
    //else steps=200;
    steps=600;
}


double simSiliconDetector::deadlayer(Particle P){
    Particle tmp=P;
    const double thick = 1.7e-4; //[cm] 
    if (tmp.energy()-tmp.getMass()>0.0002) tmp += Al->dEdx(tmp,thick); 
    else tmp.setE(tmp.getMass());//(200 keV particles are stopped in 1.7um Al)
    return (P.energy()-tmp.energy());
}

int simSiliconDetector::check(Particle vf, double xx[3],
			      target *target, struct TargetCoord *, struct simFocalCoord *fc, modeltype ModelType)
{
  double x[]={xx[0]/10, xx[1]/10, xx[2]/10}; //divide by 10 because wobbler target/wobbler units are mm

  Point position = Point(x);
  Point direction(vf[1], vf[2], vf[3]);

  OK->handle(0);
  
  //Check is the particle enters detector acceptance
  if (accept(vf)==false) return 0;
     
  const double low=0.00005; //50 keV lowest energy for Bethe-Bloch calculation
  const double thr=0.001; //1 MeV threshold to start Bethe-Bloch calculation (below this limit
                             //particles are absorbed and no Bethe-Bloch is necessary
  steps=200;
  double Ebb2=0.0;
  double Emsx[5];
  for (int i=0;i<5;i++) Emsx[i]=0;
  double Eveto=0;
  double Eabsorber=0;
  double Escint=0;
  double Estraggling=0;
  double TargetEloss=0;
  double sigma2=0;
  double beta=0;
  double BB2path=0.0;
  double Epart=0.0;
  double horY=0.0;
  double verXZ=0.0;
  int layer=0;
  double Edead = 0.0;
  double tt = 0.0;
 
  //Energy straggling in detectors
  const double sigma0_Al = 0.000451; //should be multiplied with sqrt(pathlength[cm]) to get straggling sigma
  const double sigma0_Si = 0.000427;
  const double sigma0_Sc = 0.000296;    
  //const double sigma0_CH2 = 0.000272;    
  const double sigma0_12C = 0.000365;    

  //generates random numbers according to Gauss distribution
  static PseudoRandom psrandom;
  static normal norm(&psrandom, &psrandom);

  //******************************
  // --- Target Energy loss ---
  //******************************
  
  Particle particle = vf;
  Particle tmp=particle;
  Particle old=tmp;

  Point inidir(particle[1], particle[2], particle[3]);

  //draw a blue line in the original particle direction
  
  if (debugSI){
      siplot->color(0,0,0);
      draw(*siplot);
      siplot->color(0,0,1);
      siplot->line(Point(x), Point(x) + inidir/sqrt(inidir*inidir)*12); //12 cm from the vertex
  }
  
  target->EnergyLossSim(tmp, x[0], x[1], x[2], steps, ModelType);

  //Energy straggling
  beta=sqrt(1-1/(particle.gamma()*particle.gamma()));
  //sigma2 = (1-0.5*beta*beta)/(1-beta*beta)*sigma0_CH2*sigma0_CH2; //for polyethylene target
  sigma2 = (1-0.5*beta*beta)/(1-beta*beta)*sigma0_12C*sigma0_12C; //for carbon target
  sigma2 *= target->getLength_in_Target(x[0],x[1],x[2],particle.theta(),particle.phi())/10;
  Estraggling = norm() * sqrt(sigma2);

  tmp -= Estraggling;
  TargetEloss=particle.energy()-tmp.energy();
  
  particle = tmp;

  position += inidir/sqrt(inidir*inidir)*target->getLength_in_Target(x[0],x[1],x[2],vf.theta(),vf.phi())/10;
  direction = Point(particle[1], particle[2], particle[3]);
  
  //******************************
  // --- Absorber Energy loss ---
  //******************************
  //Point p_abs_1 = absorber->check(Point(x), p_target, siplot); //check for events in absorber
  Point p_abs_1 = absorber->check(position, direction, siplot); //check for events in absorber
 
  //draw a red line in the scattered particle direction  
  if (debugSI){
      siplot->color(1,0,0);
      siplot->line(position, p_abs_1);
  }

  if (absorber->getPathlength()>0.0){
      tmp=particle;
      Epart=particle.energy()-particle.getMass();
      for (int i=0; i<steps; i++) {
	  if (Epart>thr && (tmp.energy()-tmp.getMass()>low)){
	      old = tmp;
	      tmp += Al->dEdx(tmp,absorber->getPathlength()/steps);
	      if (fabs(old.energy()-tmp.energy())<low/steps){
		  tmp.setE(tmp.getMass());
		  break;   
	      }
	      Al->MultiScat(tmp, old.momentum(),absorber->getPathlength()/steps);
	  }
	  else {
	      tmp.setE(tmp.getMass());
	      break;
	  }
      } 
      
      //Energy straggling
      if (tmp.energy()-tmp.getMass() > low){
	  beta=sqrt(1-1/(particle.gamma()*particle.gamma()));
	  sigma2 = (1-0.5*beta*beta)/(1-beta*beta)*sigma0_Al*sigma0_Al;
	  sigma2 *= absorber->getPathlength();
	  Estraggling = norm() * sqrt(sigma2);
	  tmp -= Estraggling;
      }
      Eabsorber = particle.energy()-tmp.energy(); 
    
      particle = tmp; 
      direction = Point(particle[1], particle[2], particle[3]); 
      position = p_abs_1 + direction/sqrt(direction*direction)*absorber->getPathlength();
  }
  if (Eabsorber <= thr){
      if (debugSI) siplot->page();
      return 1; //Exit if the particle did not cross absorber
  }
 
  if (particle.energy()-particle.getMass()>0){
 
      //**********************************
      // --- Scintillator Energy loss ---
      //**********************************
      Point p_abs_2 = direction;
      //Point p_sci_1 = scint->check(Point(x),p_abs_2, siplot); //check for events in scintillator
      Point p_sci_1 = scint->check(position, direction, siplot); //check for events in scintillator
      
      //Exit if we don't see event in scintillator, because scintillator gives the trigger in real experiment
      if (p_sci_1[0]==0 && p_sci_1[1]==0 && p_sci_1[2]==0){
	  if (debugSI) siplot->page();
	  return 1;
      }
      //draw a red line in the scattered particle direction  
      if (debugSI){
	  siplot->color(1,0,0);
	  if (p_sci_1[0]!=0 || p_sci_1[1]!=0 || p_sci_1[2]!=0)
	      siplot->line(position, p_sci_1);
      }
  
      if (scint->getPathlength() > 0.0){
	  tmp=particle;
	  Epart=particle.energy()-particle.getMass();
	  setSteps(Epart);
	  for (int i=0; i<steps; i++) {
	      if (Epart>thr && (tmp.energy()-tmp.getMass())>low){
		  old = tmp;
		  tmp += Scintillator->dEdx(tmp,scint->getPathlength()/steps);
		  if (fabs(old.energy()-tmp.energy())<low/steps){
		      tmp.setE(tmp.getMass());
		      break;   
		  }
		  Scintillator->MultiScat(tmp, old.momentum(),scint->getPathlength()/steps);
	      } 
	      else {
		  //tmp -= tmp.energy()-tmp.getMass();
		  tmp.setE(tmp.getMass());
		  break;
	      } 
	  } 
	  
	  if (tmp.energy()-tmp.getMass() > low){
	      //Energy straggling correction
	      beta=sqrt(1-1/(particle.gamma()*particle.gamma()));
	      sigma2 = (1-0.5*beta*beta)/(1-beta*beta)*sigma0_Sc*sigma0_Sc;
	      sigma2 *= scint->getPathlength();
	      Estraggling = norm() * sqrt(sigma2);
	      tmp -= Estraggling;
	  }
	  Escint = particle.energy()-tmp.energy();
	  particle = tmp;
	  
	  direction = Point(particle[1], particle[2], particle[3]); 
	  position = p_sci_1 + direction/sqrt(direction*direction)*scint->getPathlength();
	  
	  if (Escint < thr){
	      if (debugSI) siplot->page();
	      return 1; //Because we have a threshold on the scintillator anyway
	  }
      }
      
      if ((particle.energy()-particle.getMass())>0 && scint->getPathlength()>0.0){
      	  
	  //**********************************
	  // --- BB2 ---
	  //**********************************
	  Point p_sci_2 = direction;
	  //Point p_bb2_1 = bb2->check(Point(x), p_sci_2, siplot);
	  Point p_bb2_1 = bb2->check(position, direction, siplot);
	  
	  //draw a red line in the scattered part direction
	  if (debugSI){
	      siplot->color(1,0,0);
	      if (p_bb2_1[0]!=0 || p_bb2_1[1]!=0 || p_bb2_1[2]!=0){
		  siplot->line(position, p_bb2_1);
	      }
	  }
	  
	  //Point dir(part[1], part[2], part[3]);
	  //if (debugSI) draw(*siplot);
	  //if (debugSI) siplot->color(0,0,1);
	  //if (debugSI) siplot->line(Point(x), Point(x) + dir/sqrt(dir*dir)*10);
	  
	  //Point r = bb2->check(Point(x), dir, siplot);
	  //for(int i=0;i<5;i++) msx[i]->check(Point(x), dir, siplot);
	  //veto->check(Point(x), dir, siplot);
	  //if (debugSI) siplot->page();
	  
	  //if (r[0]==0 && r[1]==0 && r[2]==0) return 0;
	  //dir = r - Point(x);
	  //dir = dir / sqrt(dir * dir) * part.momentum();
	  //part = FourVector (part[0], dir[0], dir[1], dir[2]);
	  
	  //Eloss in BB2 
	  BB2path=0.0;
	  Particle tmp1=particle;
	  int cnt_h=0,cnt_v=0; 
	  int vert_no[2],hori_no[2];
	  
	  for (int i=0;i<24;i++){
	      if (bb2->vert[i]->getPathlength()>0.0){
		  vert_no[cnt_h] = i; cnt_h++;
		  BB2path += bb2->vert[i]->getPathlength();//zbrajam jer postoji mogucnost da prodje kroz 2 stripa
		  verXZ=(i+0.5-12.0)/10.0; //vertical coordinate in [cm]
		  for (int j=0; j<steps; j++){
		      if (tmp1.energy()-tmp1.getMass()>low) tmp1 += Si->dEdx(tmp1, bb2->vert[i]->getPathlength()/steps);
		      else break;  
		  }
		  E_A[i]->handle(particle.energy()-tmp1.energy());
	      }
	  }
	  //This is for handling to case when the particle crosses two strips
	  if (cnt_h==2){
	      double k = vert_no[0]*bb2->vert[vert_no[0]]->getPathlength()/BB2path + vert_no[1]*bb2->vert[vert_no[1]]->getPathlength()/BB2path;
	      verXZ=(k+0.5-12.0)/10.0; //vertical coordinate in [cm]
	  }


	  tmp = particle;
	  Epart=particle.energy()-particle.getMass();
	  setSteps(Epart);
	  for (int i=0;i<24;i++){
	      if (bb2->hori[i]->getPathlength()>0.0) {
		  hori_no[cnt_v] = i; cnt_v++;
		  horY=(i+0.5-12.0)/10.0; //horizontal coordinate in [cm]
		  for (int j=0; j<steps; j++){
		      if (Epart>thr && (tmp.energy()-tmp.getMass())>low){
			  old = tmp;
			  tmp += Si->dEdx(tmp,bb2->hori[i]->getPathlength()/steps);
			  if (fabs(old.energy()-tmp.energy())<low/steps){
			      tmp.setE(tmp.getMass());
			      break;   
			  }
			  Si->MultiScat(tmp, old.momentum(),bb2->hori[i]->getPathlength()/steps);
		      }
		      else {
			  //tmp -= tmp.energy()-tmp.getMass();
			  tmp.setE(tmp.getMass());
			  break;
		      }
		  }
		  E_B[i]->handle(particle.energy()-tmp.energy());
	      } 
	  }
	  //This is for handling to case when the particle crosses two strips
	  if (cnt_v==2){
	      double k = hori_no[0]*bb2->hori[hori_no[0]]->getPathlength()/BB2path + hori_no[1]*bb2->hori[hori_no[1]]->getPathlength()/BB2path;
	      horY=(k+0.5-12.0)/10.0; //horizontal coordinate in [cm]
	  }


	  if (BB2path >= BB2_thickness){ //Only those that cross total BB2

	   Particle ppp = initparticle(particle.getMass(),particle.momentum(),angle,atan2(verXZ,distance),atan2(horY,distance));
	   simtheta->handle(ppp.theta()); //trebalo bi pravilno handlati slucaj kad prodje kroz 2 stripa
	   simphi->handle(ppp.phi());  
	   si_theta->handle(atan2(verXZ,distance));
	   si_phi->handle(atan2(horY,distance));

	   /*
	     printf("\nRectangular: (%7.4f,%7.4f,%7.4f,%7.4f)\n",prot[0],prot[1],prot[2],prot[3]);
	     printf("Spherical  : (%7.4f,%7.4f,%7.4f,%7.4f)\n",prot.energy(),prot.momentum(),prot.theta()*180/M_PI,prot.phi()*180/M_PI);
	     printf("Strip(ver_xz,hor_y) : (%7.4f,%7.4f)\n",verXZ,horY);
	     printf("\nRectangular: (%7.4f,%7.4f,%7.4f)\n",pp[0],pp[1],pp[2]);
	     printf("Spherical  : (%7.4f,%7.4f,%7.4f,%7.4f)\n",ppp.energy(),ppp.momentum(),ppp.theta()*180/M_PI,ppp.phi()*180/M_PI);
	   */
	   
	   if (tmp.energy()-tmp.getMass() > low){
	       //Energy straggling
	       beta=sqrt(1-1/(particle.gamma()*particle.gamma()));
	       sigma2 = (1-0.5*beta*beta)/(1-beta*beta)*sigma0_Si*sigma0_Si;
	       sigma2 *= BB2path;
	       Estraggling = norm() * sqrt(sigma2);
	       tmp -= Estraggling;
	   }
	   Ebb2 = particle.energy()-tmp.energy();
	   if (Ebb2 <= 0) Ebb2=0.0;
	   particle = tmp;
	  }
  
	  if (particle.energy()-particle.getMass()>0 && scint->getPathlength()>0.0){
	  	      
	      //**********************************
	      // --- MSX1 ---
	      //**********************************
	      Point p_bb2_2(particle[1], particle[2], particle[3]);
	      Point p_msx1_1(0,0,0);
	      
	      //in case we have an event in BB2
	      if (p_bb2_1[0]!=0 || p_bb2_1[1]!=0 || p_bb2_1[2]!=0){
		  //p_msx1_1 = msx[0]->check(Point(x), p_bb2_2, siplot);
		  p_msx1_1 = msx[0]->check(p_bb2_1+p_bb2_2/sqrt(p_bb2_2*p_bb2_2)*BB2path, p_bb2_2, siplot);
		  
		  //draw a red line in the scattered particle direction
		  if (debugSI){
		      siplot->color(1,0,0);
		      if (p_msx1_1[0]!=0 || p_msx1_1[1]!=0 || p_msx1_1[2]!=0)
			  siplot->line(p_bb2_1+p_bb2_2/sqrt(p_bb2_2*p_bb2_2)*BB2path, p_msx1_1);
		  }
		  
	      }
	      else{ //in case we don't have an event in bb2, which can happen because of its smaller acceptance
		  //p_msx1_1 = msx[0]->check(Point(x), p_sci_2, siplot);
		  p_msx1_1 = msx[0]->check(p_sci_1+p_sci_2/sqrt(p_sci_2*p_sci_2)*scint->getPathlength(), p_sci_2, siplot);
		  
		  //draw a red line in the scattered particle direction
		  if (debugSI){
		      siplot->color(1,0,0);
		      if (p_msx1_1[0]!=0 || p_msx1_1[1]!=0 || p_msx1_1[2]!=0)
			  siplot->line(p_sci_1+p_sci_2/sqrt(p_sci_2*p_sci_2)*scint->getPathlength(), p_msx1_1);
		  }
	      }
	      //Point dir1(particle[1], particle[2], particle[3]);
	      //msx[0]->check(Point(x), dir1, siplot); //check for events in msx1
	      
	      if (msx[0]->getPathlength() > 0.0){
		  
		  tmp=particle;
		  Epart=particle.energy()-particle.getMass();
		  setSteps(Epart);
		  for (int j=0; j<steps; j++){
		      if (Epart>thr && (tmp.energy()-tmp.getMass())>low){
			  old = tmp;
			  tmp += Si->dEdx(tmp, msx[0]->getPathlength()/steps);
			  if (fabs(old.energy()-tmp.energy())<low/steps){
			      tmp.setE(tmp.getMass());
			      break;   
			  }
			  Si->MultiScat(tmp, old.momentum(),msx[0]->getPathlength()/steps);
		      }
		      else {
			  //tmp -= tmp.energy()-tmp.getMass();
			  tmp.setE(tmp.getMass());
			  break;
		      }	
		  }
		  
		  if (tmp.energy()-tmp.getMass() > low){
		      //Energy straggling
		      beta=sqrt(1-1/(particle.gamma()*particle.gamma()));
		      sigma2 = (1-0.5*beta*beta)/(1-beta*beta)*sigma0_Si*sigma0_Si;
		      sigma2 *= msx[0]->getPathlength();
		      Estraggling = norm() * sqrt(sigma2);
		      tmp -= Estraggling;
		  }
		  Emsx[0] = particle.energy()-tmp.energy();
		  if (Emsx[0] < 0) Emsx[0] = 0.0;
		  particle = tmp;
		  layer++;
	      }
	      
	      //Dead layer energy loss 
	      Edead += (tt = deadlayer(particle)); particle -= tt;

	      if (particle.energy()-particle.getMass()>0 && msx[0]->getPathlength()>0.0){
	      		  
		  //**********************************
		  // --- MSX2 ---
		  //**********************************
		  Point p_msx1_2(particle[1], particle[2], particle[3]);
		  //Point p_msx2_1 = msx[1]->check(Point(x), p_msx1_2, siplot);
		  Point p_msx2_1 = msx[1]->check(p_msx1_1+p_msx1_2/sqrt(p_msx1_2*p_msx1_2)*msx[0]->getPathlength(), p_msx1_2, siplot);
		  
		  //draw a red line in the scattered particle direction
		  if (debugSI){
		      siplot->color(1,0,0);
		      if (p_msx2_1[0]!=0 || p_msx2_1[1]!=0 || p_msx2_1[2]!=0)
			  siplot->line(p_msx1_1+p_msx1_2/sqrt(p_msx1_2*p_msx1_2)*msx[0]->getPathlength(), p_msx2_1);
		      //if (debugSI) siplot->line(p_msx1_1+Point(0.1,0,0), p_msx1_1+Point(-0.1,0,0));
		      //if (debugSI) siplot->line(p_msx1_1+Point(0,0,0.1), p_msx1_1+Point(0,0,-0.1));
		      //if (debugSI) siplot->line(p_msx2_1+Point(0.1,0,0), p_msx2_1+Point(-0.1,0,0));
		      //if (debugSI) siplot->line(p_msx2_1+Point(0,0,0.1), p_msx2_1+Point(0,0,-0.1));
		  }
		  
		  //Point dir2(part[1], part[2], part[3]);
		  //msx[1]->check(Point(x), dir2, siplot);
		  
		  if (msx[1]->getPathlength() > 0.0){ 	  
		      tmp=particle;
		      Epart=particle.energy()-particle.getMass();
		      setSteps(Epart);
		      for (int j=0; j<steps; j++){
			  if (Epart>thr && (tmp.energy()-tmp.getMass())>low){
			      old = tmp;
			      tmp += Si->dEdx(tmp, msx[1]->getPathlength()/steps);
			      if (fabs(old.energy()-tmp.energy())<low/steps){
				  tmp.setE(tmp.getMass());
				  break;   
			      }
			      Si->MultiScat(tmp, old.momentum(),msx[1]->getPathlength()/steps);
			  }
			  else {
			      //tmp -= (tmp.energy()-tmp.getMass());
			      tmp.setE(tmp.getMass());
			      break;
			  }
		      }
		      
		      if (tmp.energy()-tmp.getMass() > low){
			  //Energy straggling
			  beta=sqrt(1-1/(particle.gamma()*particle.gamma()));
			  sigma2 = (1-0.5*beta*beta)/(1-beta*beta)*sigma0_Si*sigma0_Si;
			  sigma2 *= msx[1]->getPathlength();
			  Estraggling = norm() * sqrt(sigma2);
			  tmp -= Estraggling;
		      }	    
		      Emsx[1] = particle.energy()-tmp.energy();
		      if (Emsx[1] < 0) Emsx[1] = 0.0;
		      particle = tmp;
		      layer++;
		  }
		 
		  //Dead layer energy loss 
		  Edead += (tt = deadlayer(particle)); particle -= tt;

		  if (particle.energy()-particle.getMass()>0 && msx[1]->getPathlength()>0.0){
		  	  	      
		      //**********************************
		      // --- MSX3 ---
		      //**********************************
		      Point p_msx2_2(particle[1], particle[2], particle[3]);
		      //Point p_msx3_1 = msx[2]->check(Point(x), p_msx2_2, siplot);
		      Point p_msx3_1 = msx[2]->check(p_msx2_1+p_msx2_2/sqrt(p_msx2_2*p_msx2_2)*msx[1]->getPathlength(), p_msx2_2, siplot);
		      
		      //draw a red line in the scattered particle direction
		      if (debugSI){
			  siplot->color(1,0,0);
			  if (p_msx3_1[0]!=0 || p_msx3_1[1]!=0 || p_msx3_1[2]!=0)
			      siplot->line(p_msx2_1+p_msx2_2/sqrt(p_msx2_2*p_msx2_2)*msx[1]->getPathlength(), p_msx3_1);
		      }
		      
		      //Point dir3(particle[1], particle[2], particle[3]);
		      //msx[2]->check(Point(x), dir3, siplot);
		      
		      if (msx[2]->getPathlength() > 0.0){ 	  
			  tmp=particle;
			  Epart=particle.energy()-particle.getMass();
			  setSteps(Epart);
			  for (int j=0; j<steps; j++){
			      if (Epart>thr && (tmp.energy()-tmp.getMass()>low)){
				  old = tmp;
				  tmp += Si->dEdx(tmp, msx[2]->getPathlength()/steps);
				  if (fabs(old.energy()-tmp.energy())<low/steps){
				      tmp.setE(tmp.getMass());
				      break;   
				  }
				  Si->MultiScat(tmp, old.momentum(),msx[2]->getPathlength()/steps);
			      }
			      else {
				  //tmp -= tmp.energy()-tmp.getMass();
				  tmp.setE(tmp.getMass());
				  break;
			      }
			  }
			  
			  if (tmp.energy()-tmp.getMass() > low){
			      //Energy straggling
			      beta=sqrt(1-1/(particle.gamma()*particle.gamma()));
			      sigma2 = (1-0.5*beta*beta)/(1-beta*beta)*sigma0_Si*sigma0_Si;
			      sigma2 *= msx[2]->getPathlength();
			      Estraggling = norm() * sqrt(sigma2);
			      tmp -= Estraggling;
			  }
			  Emsx[2] = particle.energy()-tmp.energy();
			  if (Emsx[2] < 0) Emsx[2] = 0.0;
			  particle = tmp;
			  layer++;
		      }
		   
		      //Dead layer energy loss 
		      Edead += (tt = deadlayer(particle)); particle -= tt;
		      
		      if (particle.energy()-particle.getMass()>0 && msx[2]->getPathlength()>0.0){
		      			  
			  //**********************************
			  // --- MSX4 ---
			  //**********************************
			  Point p_msx3_2(particle[1], particle[2], particle[3]);
			  //Point p_msx4_1 = msx[3]->check(Point(x), p_msx3_2, siplot);
			  Point p_msx4_1 = msx[3]->check(p_msx3_1+p_msx3_2/sqrt(p_msx3_2*p_msx3_2)*msx[2]->getPathlength(), p_msx3_2, siplot);
			  
			  //draw a red line in the scattered particle direction
			  if (debugSI){
			      siplot->color(1,0,0);
			      if (p_msx4_1[0]!=0 || p_msx4_1[1]!=0 || p_msx4_1[2]!=0)
				  siplot->line(p_msx3_1+p_msx3_2/sqrt(p_msx3_2*p_msx3_2)*msx[2]->getPathlength(), p_msx4_1);
			  }
			  
			  //Point dir4(particle[1], particle[2], particle[3]);
			  //msx[3]->check(Point(x), dir4, siplot);
			  
			  if (msx[3]->getPathlength() > 0.0){ 	
			      tmp=particle;
			      Epart=particle.energy()-particle.getMass();
			      setSteps(Epart);
			      for (int j=0; j<steps; j++){
				  if (Epart>thr && (tmp.energy()-tmp.getMass())>low){
				      old = tmp;
				      tmp += Si->dEdx(tmp, msx[3]->getPathlength()/steps);
				      if (fabs(old.energy()-tmp.energy())<low/steps){
					  tmp.setE(tmp.getMass());
					  break;   
				      }
				      Si->MultiScat(tmp, old.momentum(),msx[3]->getPathlength()/steps);
				  }
				  else {
				      //tmp -= tmp.energy()-tmp.getMass();
				      tmp.setE(tmp.getMass());
				      break;
				  }
			      }
			      
			      if (tmp.energy()-tmp.getMass() > low){
				  //Energy straggling
				  beta=sqrt(1-1/(particle.gamma()*particle.gamma()));
				  sigma2 = (1-0.5*beta*beta)/(1-beta*beta)*sigma0_Si*sigma0_Si;
				  sigma2 *= msx[3]->getPathlength();
				  Estraggling = norm() * sqrt(sigma2);
				  tmp -= Estraggling;
			      }
			      Emsx[3] = particle.energy()-tmp.energy();
			      if (Emsx[3] < 0) Emsx[3] = 0.0;
			      particle = tmp;
			      layer++;
			  }

			  //Dead layer energy loss 
			  Edead += (tt = deadlayer(particle)); particle -= tt;
 
			  if (particle.energy()-particle.getMass()>0 && msx[3]->getPathlength()>0.0){
			  			      
			      //**********************************
			      // --- MSX5 ---
			      //**********************************
			      Point p_msx4_2(particle[1], particle[2], particle[3]);
			      //Point p_msx5_1 = msx[4]->check(Point(x), p_msx4_2, siplot);
			      Point p_msx5_1 = msx[4]->check(p_msx4_1+p_msx4_2/sqrt(p_msx4_2*p_msx4_2)*msx[3]->getPathlength(), p_msx4_2, siplot);
			      
			      //draw a red line in the scattered particleicle direction
			      if (debugSI){
				  siplot->color(1,0,0);
				  if (p_msx5_1[0]!=0 || p_msx5_1[1]!=0 || p_msx5_1[2]!=0)
				      siplot->line(p_msx4_1+p_msx4_2/sqrt(p_msx4_2*p_msx4_2)*msx[3]->getPathlength(), p_msx5_1);
			      }
			      
			      //Point dir5(particle[1], particle[2], particle[3]);
			      //msx[4]->check(Point(x), dir5, siplot);
			      
			      if (msx[4]->getPathlength() > 0.0){ 	
				  tmp=particle;
				  Epart=particle.energy()-particle.getMass();
				  setSteps(Epart);
				  for (int j=0; j<steps; j++){
				      if (Epart>thr && (tmp.energy()-tmp.getMass())>low){
					  old = tmp;
					  tmp += Si->dEdx(tmp, msx[4]->getPathlength()/steps);
					  if (fabs(old.energy()-tmp.energy())<low/steps){
					      tmp.setE(tmp.getMass());
					      break;   
					  }
					  Si->MultiScat(tmp, old.momentum(),msx[4]->getPathlength()/steps);
				      }
				      else {
					  //tmp -= tmp.energy()-tmp.getMass();
					  tmp.setE(tmp.getMass());
					  break;
				      }
				  }
				  
				  if (tmp.energy()-tmp.getMass() > low){
				      //Energy straggling
				      beta=sqrt(1-1/(particle.gamma()*particle.gamma()));
				      sigma2 = (1-0.5*beta*beta)/(1-beta*beta)*sigma0_Si*sigma0_Si;
				      sigma2 *= msx[4]->getPathlength();
				      Estraggling = norm() * sqrt(sigma2);
				      tmp -= Estraggling;
				  }			  
				  Emsx[4] = particle.energy()-tmp.energy();
				  if (Emsx[4] < 0) Emsx[4] = 0.0;
				  particle = tmp;
				  layer++;
			      }
			    
			      //Dead layer energy loss 
			      Edead += (tt = deadlayer(particle)); particle -= tt;

			      if (particle.energy()-particle.getMass()>0){
				  
				  //**********************************
				  // --- VETO ---
				  //**********************************
				  Point p_msx5_2(particle[1], particle[2], particle[3]);
				  //Point p_veto_1 = veto->check(Point(x), p_msx5_2, siplot);
				  
				  //in case we have an event in MSX5
				  if (msx[4]->getPathlength()>0){
				      Point p_veto_1 = veto->check(p_msx5_1+p_msx5_2/sqrt(p_msx5_2*p_msx5_2)*msx[4]->getPathlength(), p_msx5_2, siplot);
				      if (debugSI) siplot->color(1,0,0);
				      if (debugSI) siplot->line(p_msx5_1+p_msx5_2/sqrt(p_msx5_2*p_msx5_2)*msx[4]->getPathlength(), p_veto_1);
				  }
				  else if (msx[3]->getPathlength()>0){
				      Point p_veto_1 = veto->check(p_msx4_1+p_msx4_2/sqrt(p_msx4_2*p_msx4_2)*msx[3]->getPathlength(), p_msx4_2, siplot);
				      if (debugSI) siplot->color(1,0,0);
				      if (debugSI) siplot->line(p_msx4_1+p_msx4_2/sqrt(p_msx4_2*p_msx4_2)*msx[3]->getPathlength(), p_veto_1);
				  } 
				  else if (msx[2]->getPathlength()>0){
				      Point p_veto_1 = veto->check(p_msx3_1+p_msx3_2/sqrt(p_msx3_2*p_msx3_2)*msx[2]->getPathlength(), p_msx3_2, siplot);
				      if (debugSI) siplot->color(1,0,0);
				      if (debugSI) siplot->line(p_msx3_1+p_msx3_2/sqrt(p_msx3_2*p_msx3_2)*msx[2]->getPathlength(), p_veto_1);
				  } 
				  else if (msx[1]->getPathlength()>0){
				      Point p_veto_1 = veto->check(p_msx2_1+p_msx2_2/sqrt(p_msx2_2*p_msx2_2)*msx[1]->getPathlength(), p_msx2_2, siplot);
				      if (debugSI) siplot->color(1,0,0);
				      if (debugSI) siplot->line(p_msx2_1+p_msx2_2/sqrt(p_msx2_2*p_msx2_2)*msx[1]->getPathlength(), p_veto_1);
				  } 
				  else if (msx[0]->getPathlength()>0){
				      Point p_veto_1 = veto->check(p_msx1_1+p_msx1_2/sqrt(p_msx1_2*p_msx1_2)*msx[0]->getPathlength(), p_msx1_2, siplot);
				      if (debugSI) siplot->color(1,0,0);
				      if (debugSI) siplot->line(p_msx1_1+p_msx1_2/sqrt(p_msx1_2*p_msx1_2)*msx[0]->getPathlength(), p_veto_1);
				  } 
				  else if (BB2path>0){
				      Point p_veto_1 = veto->check(p_bb2_1+p_bb2_2/sqrt(p_bb2_2*p_bb2_2)*BB2path, p_bb2_2, siplot);
				      if (debugSI) siplot->color(1,0,0);
				      if (debugSI) siplot->line(p_bb2_1+p_bb2_2/sqrt(p_bb2_2*p_bb2_2)*BB2path, p_veto_1);
				  } 
				  else if (scint->getPathlength()>0){
				      Point p_veto_1 = veto->check(p_sci_1+p_sci_2/sqrt(p_sci_2*p_sci_2)*scint->getPathlength(), p_sci_2, siplot);
				      if (debugSI) siplot->color(1,0,0);
				      if (debugSI) siplot->line(p_sci_1+p_sci_2/sqrt(p_sci_2*p_sci_2)*scint->getPathlength(), p_veto_1);
				  } 
				  else if (absorber->getPathlength()>0){
				      Point p_veto_1 = veto->check(p_abs_1+p_abs_2/sqrt(p_abs_2*p_abs_2)*absorber->getPathlength(), p_sci_2, siplot);
				      if (debugSI) siplot->color(1,0,0);
				      if (debugSI) siplot->line(p_abs_1+p_abs_2/sqrt(p_abs_2*p_abs_2)*absorber->getPathlength(), p_veto_1);
				  } 
				  else{
				      Point p_veto_1 = veto->check(Point(x),inidir, siplot);
				      if (debugSI) siplot->color(1,0,0);
				      if (debugSI) siplot->line(Point(x), p_veto_1);
				  }
				  //draw a red line in the scattered particle direction
				  //if (debugSI){
				  //   siplot->color(1,0,0);
				  //  if (p_veto_1[0]!=0 || p_veto_1[1]!=0 || p_veto_1[2]!=0)
				  //      siplot->line(p_msx5_1+p_msx5_2/sqrt(p_msx5_2*p_msx5_2)*msx[4]->getPathlength(), p_veto_1);
				  //}
				  
				  //Point dir6(particle[1], particle[2], particle[3]);
				  //veto->check(Point(x), dir6, siplot);
				  
				  if (veto->getPathlength() > 0.0){ 
				      tmp=particle;
				      Epart=particle.energy()-particle.getMass();
				      setSteps(Epart);
				      for (int j=0; j<steps; j++){
					  if (Epart>thr && (tmp.energy()-tmp.getMass()>low)){
					      old = tmp;
					      tmp += Si->dEdx(tmp, veto->getPathlength()/steps);
					      if (fabs(old.energy()-tmp.energy())<low/steps){
						  tmp.setE(tmp.getMass());
						  break;   
					      }
					      Si->MultiScat(tmp, old.momentum(),veto->getPathlength()/steps);
					  }
					  else {
					      //tmp -= tmp.energy()-tmp.getMass();
					      tmp.setE(tmp.getMass());
					      break;
					  }
				      }
				      
				      if (tmp.energy()-tmp.getMass() > low){
					  //Energy straggling
					  beta=sqrt(1-1/(particle.gamma()*particle.gamma()));
					  sigma2 = (1-0.5*beta*beta)/(1-beta*beta)*sigma0_Si*sigma0_Si;
					  sigma2 *= veto->getPathlength();
					  Estraggling = norm() * sqrt(sigma2);
					  tmp -= Estraggling;
				      }
				      Eveto = particle.energy()-tmp.energy();
				      if (Eveto < 0) Eveto = 0.0;
				      particle = tmp;
				      layer++;
				  }
			      }
			  }
		      }
		  }
	      }
	  }
      }
  }
  if (debugSI) siplot->page();
  
  //In analysis these energies are used for energy reconstruction not the smeared ones 
  double Escint0 = Escint;
  double Ebb20 = Ebb2;

  //Take into account detector resolution
  Escint += norm()*ResSci;
  Ebb2 += norm()*ResBB2;
  for (int i=0; i<5; i++) Emsx[i] += norm()*ResMSX[i];
  Eveto += norm()*ResVeto;
  
  //Summing only energies of layers that fired + 1 (as in Cola)
  double SiE=0;
  SiE += TargetEloss + Eabsorber + Escint0 + Ebb20;
  for (int i=0; i<layer; i++) SiE+=Emsx[i];
  if (layer>0 && layer<5) SiE+=Emsx[layer];
  else if (layer>=5) SiE+=Eveto;
  
  //This energy is summed at the end as it is reconstructed in analysis
  SiE += Edead;

  SiE +=  particle.getMass();

  simenergy->handle(SiE);
  simmomentum->handle(sqrt(SiE*SiE-pow(particle.getMass(),2)));
  Eloss_corr->handle(TargetEloss);
  
  E_scint->handle(Escint);
  E_bb2->handle(Ebb2);
  for (int i=0;i<5;i++) E_MSX[i]->handle(Emsx[i]);
  E_VETO->handle(Eveto);
  
  return 1; 
}
