#include "TriplePol.h"
#include "Simulation/Formfactor.h"
#include <fstream>

#include <cstdlib>

extern SobolSequence sobol;

namespace TriplePolNS
{
  ifstream infile;
  ofstream outfile;
  int mode=0;
  float buf[46];
  //  float plusmax, minusmax, unpolmax; 
}

// TriplePol modes:
// 0: normal phase space for experiment
// 1: phase space for golak code (theta_e and Eprime fixed)
// 2: classic PWIA model
// 3: generate events for golak code
// 4: read in output from golak code
// 5: read in kinematics output from golak code and return pwia XS
// 6: calculate proton polarisation with golak code

generateTriplePol::generateTriplePol(reaction *r, simDetectorBase *Sime, 
				     simDetectorBase *Sim1, SIMUL *rundb, modeltype ModelType)
{
  if (getenv("TriplePolMODE") == NULL) {cerr << "TriplePolMODE not set!\n"; exit(-130);}
  else TriplePolNS::mode=atoi(getenv("TriplePolMODE"));

  sime=Sime; sim1=Sim1; 
  Reaction   = r; 
  targetmass = Reaction->getTarget()->getMass();
  dcthe      = 1;                      // generation Range special for this generator
  dphie      = sime->getDphi();        //                  phi        electron
  dpe        = 1;                      //                  special for this generator
  dcth1      = sin(sim1->getDtheta()); //                  cos(theta) particle 1
  dphi1      = sim1->getDphi();        //                  phi        particle 1
  switch (TriplePolNS::mode) {
  case 0: case 1: //phase space modes
    Label = "d[W]!";;
    Unit  = "sr";
    if (TriplePolNS::mode == 0) {
      dcthe = sin(sime->getDtheta()); // generation Range cos(theta) electron
      dpe   = sime->getDmomentum();   //                  momentum   electron
    }
    break;

  case 2: case 3: case 4: case 6:// cross section modes
    Label = "d[s]";
    Unit  = "mbarn";
    if (TriplePolNS::mode == 2) {
      dcthe = sin(sime->getDtheta()); // generation Range cos(theta) electron
      dpe   = sime->getDmomentum();   //                  momentum   electron
    } else {
      if (getenv("TriplePolFILE") == NULL){cerr << "TriplePolFILE not set!\n"; exit(-131);}
      if (TriplePolNS::mode == 3) TriplePolNS::outfile.open(getenv("TriplePolFILE"));
      else {
	TriplePolNS::infile.open(getenv("TriplePolFILE"));
      }
    }
    break;

  case 5:
    Label = "d[s]";
    Unit  = "mbarn";
    break;
  }

  if ((TriplePolNS::mode == 1) || (TriplePolNS::mode == 3) || (TriplePolNS::mode == 5)) {
    if (getenv("EPRIME") == NULL) {cerr << "EPRIME not set! (in GeV)\n"; exit(-132);}
    else Eprime=atof(getenv("EPRIME"));

    if (getenv("THETAE") == NULL) {cerr << "THETAE not set! (in degree)\n"; exit(-133);}
    else thetae=atof(getenv("THETAE"))*rad;
  } 
  Target.initPolar(targetmass,0,0,0);
}

double generateTriplePol::integrationVolume()
{
  return 8 * dcthe * dphie * dpe * 
    4 * dcth1 * dphi1;
}

void generateTriplePol::Photon_rotation(FourVector Photon, FourVector* Proton)
{
  double Oopq = 0;
  if (Photon.phi() < 0) Oopq = Photon.phi() + 2 * M_PI; else Oopq = Photon.phi();
  Oopq -= M_PI;
  Proton->rot_phi(-Oopq); //rotate proton to inplane-photon
}

double generateTriplePol::generateEvent(double helicity)
{

  //Generate electron kinematics
  //case 0 and 2 generate a random electron
  //case 1, 3 and 5 generate only phi_e
  //case 4 and 6 read in electron from file 
  switch (TriplePolNS::mode) {
  case 0: case 2:
    Eprime = sime->getMomentum()- (2*sobol()-1)*dpe;
    generateLabAngles(&Reaction->electronOut, Eprime,
		      sime->getAngle(), sime->getOop(), dcthe, dphie);
    break;

  case 1: case 3: case 5:
    phie = (2 * sobol() - 1) * dphie;
    Reaction->electronOut.initPolar(Eprime, Eprime, thetae, phie); 
    break;

  case 4: case 6:
    Reaction->electronOut.initPolar(TriplePolNS::buf[3], TriplePolNS::buf[3], 
				    TriplePolNS::buf[4], TriplePolNS::buf[5]); 
    //maximum cross section for the different settings
    //needed for polarization calculation
    if (fabs(TriplePolNS::buf[3]-0.700)<0.01 && fabs((TriplePolNS::buf[4]/rad)-27.0)<0.3) XSmax=1.1e-6;
    if (fabs(TriplePolNS::buf[3]-0.700)<0.01 && fabs((TriplePolNS::buf[4]/rad)-27.8)<0.3) XSmax=1.3e-6;
    if (fabs(TriplePolNS::buf[3]-0.700)<0.01 && fabs((TriplePolNS::buf[4]/rad)-28.6)<0.3) XSmax=1.45e-6;

    if (fabs(TriplePolNS::buf[3]-0.723)<0.01 && fabs((TriplePolNS::buf[4]/rad)-27.0)<0.3) XSmax=3.75e-6;
    if (fabs(TriplePolNS::buf[3]-0.723)<0.01 && fabs((TriplePolNS::buf[4]/rad)-27.8)<0.3) XSmax=4.45e-6;
    if (fabs(TriplePolNS::buf[3]-0.723)<0.01 && fabs((TriplePolNS::buf[4]/rad)-28.6)<0.3) XSmax=5.3e-6;

    if (fabs(TriplePolNS::buf[3]-0.746)<0.01 && fabs((TriplePolNS::buf[4]/rad)-27.0)<0.3) XSmax=1.5e-5;
    if (fabs(TriplePolNS::buf[3]-0.746)<0.01 && fabs((TriplePolNS::buf[4]/rad)-27.8)<0.3) XSmax=1.75e-5;
    if (fabs(TriplePolNS::buf[3]-0.746)<0.01 && fabs((TriplePolNS::buf[4]/rad)-28.6)<0.3) XSmax=2.0e-5;
    break;
  }

  FourVector Photon = Reaction->electronIn-Reaction->electronOut;

  double weight = 1.0;


  if (TriplePolNS::mode < 4 || TriplePolNS::mode==5) {
    //generate proton
    generateLabAngles(&Reaction->Out1, 1, sim1->getAngle(), 
		      sim1->getOop(), dcth1, dphi1);
    //calculate Proton momentum
    FourVector Dummy;
    Dummy.initPolar(1,1,Reaction->Out1.theta(),Reaction->Out1.phi());
    double
      thpq = acos(Dummy.getP()*Photon.getP()/(Dummy.momentum()*Photon.momentum())),
      //angle between photon and proton
      Elab = Photon.energy() + targetmass, //total lab energy
      qp   = Photon.momentum() * cos(thpq), //component of q paralle to proton momentum
      ECM  = sqrt(sqr(Elab)-sqr(Photon.momentum())), //center of mass energy
      X    = sqr(Elab) - sqr(qp),
      Y    = -Elab*(sqr(ECM)+sqr(Reaction->getM1()->getMass())-sqr(Reaction->getM2()->getMass())),
      Z    = sqr(sqr(ECM)+sqr(Reaction->getM1()->getMass())-sqr(Reaction->getM2()->getMass()))/4.0
      +sqr(Reaction->getM1()->getMass()*qp);
    if (sqr(Y)-4*X*Z < 0) return 0;
    double
      Ep   = (-Y+sqrt(sqr(Y)-4*X*Z))/(2*X),
      pp   = sqrt(sqr(Ep)-sqr(Reaction->getM1()->getMass()));
    Reaction->Out1.initPolar(Ep, pp, Reaction->Out1.theta(), Reaction->Out1.phi());
    if (TriplePolNS::mode < 2) { 
      //Add photon flux to weight to get a two-fold differential crosssection
      //phase space only
      double
	E0      = Reaction->electronIn.energy(),
	the     = Reaction->electronOut.theta(),
	q2      = -4 * E0 * Eprime * pow(sin(the/2),2),
	s       = targetmass*targetmass + q2 + 2 * targetmass * (E0 - Eprime),
	epsilon = 1/(1 - 2*(pow(E0 - Eprime,2) - q2) / q2 * pow(tan(the/2),2)),
	kgamma  = (s - targetmass*targetmass) / 2 / targetmass,
	flux    = alpha/2/M_PI/M_PI * Eprime/E0 * kgamma / -q2 / (1-epsilon);
      weight  *= flux;
    }
    if (TriplePolNS::mode == 2 || TriplePolNS::mode == 5) {
      weight *= pwia(Reaction->electronIn, Reaction->electronOut, 
		     Reaction->Out1,Photon+Target-Reaction->Out1) * 1e-3;
    }
    if (TriplePolNS::mode == 3) {
      TriplePolNS::buf[3] = Reaction->electronOut.energy();
      TriplePolNS::buf[4] = Reaction->electronOut.theta();
      TriplePolNS::buf[5] = Reaction->electronOut.phi();
      TriplePolNS::buf[7] = Reaction->Out1.momentum();
      TriplePolNS::buf[8] = Reaction->Out1.theta();
      TriplePolNS::buf[9] = Reaction->Out1.phi();
    }
    if (TriplePolNS::mode==1) {
      Momentum Spin(0,0,0.7);
      Spin.rot_theta(Photon.theta());
      Spin.rot_phi(Photon.theta());
      int tmp = (sobol()>0.5?1:-1);
      Reaction->Out1.setSpin(Spin * helicity * tmp);
    }
  } else {
    double Ep=sqrt(sqr(TriplePolNS::buf[7]) + sqr(m_proton));
    Reaction->Out1.initPolar(Ep,TriplePolNS::buf[7], 
			     TriplePolNS::buf[8], TriplePolNS::buf[9]);
    if (TriplePolNS::mode==6) {
      //Change the electron a little bit
      //and recalculate the proton energy from this
      double the=Reaction->electronOut.theta()+(2*sobol()-1)*0.02;
      double Eout=Reaction->electronOut.energy()+(2*sobol()-1)*0.01;
      double phe=Reaction->electronOut.phi();
      Reaction->electronOut.initPolar(Eout,Eout,the,phe);
      Photon = Reaction->electronIn-Reaction->electronOut;
      FourVector Dummy;
      Dummy.initPolar(1,1,Reaction->Out1.theta(),Reaction->Out1.phi());
      double
	thpq = acos(Dummy.getP()*Photon.getP()/(Dummy.momentum()*Photon.momentum())),
	//angle between photon and proton
	Elab = Photon.energy() + targetmass, //total lab energy
	qp   = Photon.momentum() * cos(thpq), //component of q paralle to proton momentum
	ECM  = sqrt(sqr(Elab)-sqr(Photon.momentum())), //center of mass energy
	X    = sqr(Elab) - sqr(qp),
	Y    = -Elab*(sqr(ECM)+sqr(Reaction->getM1()->getMass())-sqr(Reaction->getM2()->getMass())),
	//      Y    = -Elab*(sqr(ECM)+sqr(m_proton)-sqr(m_H2)),
	Z    = sqr(sqr(ECM)+sqr(Reaction->getM1()->getMass())-sqr(Reaction->getM2()->getMass()))/4.0
	+sqr(Reaction->getM1()->getMass()*qp);
      //      Z    = sqr(sqr(ECM)+sqr(m_proton)-sqr(m_H2))/4.0+sqr(m_proton*qp);
      if (sqr(Y)-4*X*Z < 0) return 0;
      Ep   = (-Y+sqrt(sqr(Y)-4*X*Z))/(2*X);
      double
	pp   = sqrt(sqr(Ep)-sqr(Reaction->getM1()->getMass()));
      Reaction->Out1.initPolar(Ep, pp, Reaction->Out1.theta(), Reaction->Out1.phi());
    }
    float XS[6][9];
    XS[0][0]=TriplePolNS::buf[10]; //SpinDir 1, M1=-1/2, Sigma
    XS[0][1]=TriplePolNS::buf[11]; //SpinDir 1, M1=-1/2, A
    XS[0][2]=TriplePolNS::buf[12]; //SpinDir 1, M1=+1/2, Sigma
    XS[0][3]=TriplePolNS::buf[13]; //SpinDir 1, M1=+1/2, A
    XS[0][4]=TriplePolNS::buf[14]; //SpinDir 1, M1=0,    Sigma
    XS[0][5]=TriplePolNS::buf[15]; //SpinDir 1, M1=0,    A
    XS[0][6]=XS[0][0]+helicity*XS[0][1]*XS[0][0]/100.0; //SpinDir 1, M1=-1/2, sigma
    XS[0][7]=XS[0][2]+helicity*XS[0][3]*XS[0][2]/100.0; //SpinDir 1, M1=+1/2, sigma
    XS[0][8]=XS[0][4]+helicity*XS[0][5]*XS[0][4]/100.0; //SpinDir 1, M1=0   , sigma

    XS[1][0]=TriplePolNS::buf[16]; //SpinDir 2, M1=-1/2, Sigma
    XS[1][1]=TriplePolNS::buf[17]; //SpinDir 2, M1=-1/2, A
    XS[1][2]=TriplePolNS::buf[18]; //SpinDir 2, M1=+1/2, Sigma
    XS[1][3]=TriplePolNS::buf[19]; //SpinDir 2, M1=+1/2, A
    XS[1][4]=TriplePolNS::buf[20]; //SpinDir 2, M1=0,    Sigma
    XS[1][5]=TriplePolNS::buf[21]; //SpinDir 2, M1=0,    A
    XS[1][6]=XS[1][0]+helicity*XS[1][1]*XS[1][0]/100.0; //SpinDir 2, M1=-1/2, sigma
    XS[1][7]=XS[1][2]+helicity*XS[1][3]*XS[1][2]/100.0; //SpinDir 2, M1=+1/2, sigma
    XS[1][8]=XS[1][4]+helicity*XS[1][5]*XS[1][4]/100.0; //SpinDir 2, M1=0   , sigma

    // printf("%1.1f %5.4e %5.4e %5.4e %5.4e %5.4e %5.4e\n",helicity,
    // 	   XS[0][6],XS[0][0]-helicity*XS[0][1]*XS[0][0]/100.0,
    // 	   XS[0][7],XS[0][2]-helicity*XS[0][3]*XS[0][2]/100.0,
    // 	   (XS[0][6]-XS[0][7])/XS[0][8],
    // 	   (XS[0][0]-helicity*XS[0][1]*XS[0][0]/100.0-(XS[0][2]-helicity*XS[0][3]*XS[0][2]/100.0))/
    // 	   (XS[0][0]-helicity*XS[0][1]*XS[0][0]/100.0+XS[0][2]-helicity*XS[0][3]*XS[0][2]/100.0));

    // printf("%5.4e %5.4e %5.4e %5.4e %5.4e %5.4e\n",
    // 	   XS[1][6],XS[1][0]-helicity*XS[1][1]*XS[1][0]/100.0,
    // 	   XS[1][7],XS[1][2]-helicity*XS[1][3]*XS[1][2]/100.0,
    // 	   (XS[1][6]-XS[1][7])/XS[1][8],
    // 	   (XS[1][0]-helicity*XS[1][1]*XS[1][0]/100.0-(XS[1][2]-helicity*XS[1][3]*XS[1][2]/100.0))/
    // 	   (XS[1][0]-helicity*XS[1][1]*XS[1][0]/100.0+XS[1][2]-helicity*XS[1][3]*XS[1][2]/100.0));

    // cout << "-------------\n";

    XS[2][0]=TriplePolNS::buf[22]; //SpinDir 3, M1=-1/2, Sigma
    XS[2][1]=TriplePolNS::buf[23]; //SpinDir 3, M1=-1/2, A
    XS[2][2]=TriplePolNS::buf[24]; //SpinDir 3, M1=+1/2, Sigma
    XS[2][3]=TriplePolNS::buf[25]; //SpinDir 3, M1=+1/2, A
    XS[2][4]=TriplePolNS::buf[26]; //SpinDir 3, M1=0,    Sigma
    XS[2][5]=TriplePolNS::buf[27]; //SpinDir 3, M1=0,    A
    XS[2][6]=XS[2][0]+helicity*XS[2][1]*XS[2][0]/100.0; //SpinDir 3, M1=-1/2, sigma
    XS[2][7]=XS[2][2]+helicity*XS[2][3]*XS[2][2]/100.0; //SpinDir 3, M1=+1/2, sigma
    XS[2][8]=XS[2][4]+helicity*XS[2][5]*XS[2][4]/100.0; //SpinDir 3, M1=0   , sigma

    XS[3][0]=TriplePolNS::buf[28]; //SpinDir 4, M1=-1/2, Sigma
    XS[3][1]=TriplePolNS::buf[29]; //SpinDir 4, M1=-1/2, A
    XS[3][2]=TriplePolNS::buf[30]; //SpinDir 4, M1=+1/2, Sigma
    XS[3][3]=TriplePolNS::buf[31]; //SpinDir 4, M1=+1/2, A
    XS[3][4]=TriplePolNS::buf[32]; //SpinDir 4, M1=0,    Sigma
    XS[3][5]=TriplePolNS::buf[33]; //SpinDir 4, M1=0,    A
    XS[3][6]=XS[3][0]+helicity*XS[3][1]*XS[3][0]/100.0; //SpinDir 4, M1=-1/2, sigma
    XS[3][7]=XS[3][2]+helicity*XS[3][3]*XS[3][2]/100.0; //SpinDir 4, M1=+1/2, sigma
    XS[3][8]=XS[3][4]+helicity*XS[3][5]*XS[3][4]/100.0; //SpinDir 4, M1=0   , sigma

    XS[4][0]=TriplePolNS::buf[34]; //SpinDir 5, M1=-1/2, Sigma
    XS[4][1]=TriplePolNS::buf[35]; //SpinDir 5, M1=-1/2, A
    XS[4][2]=TriplePolNS::buf[36]; //SpinDir 5, M1=+1/2, Sigma
    XS[4][3]=TriplePolNS::buf[37]; //SpinDir 5, M1=+1/2, A
    XS[4][4]=TriplePolNS::buf[38]; //SpinDir 5, M1=0,    Sigma
    XS[4][5]=TriplePolNS::buf[39]; //SpinDir 5, M1=0,    A
    XS[4][6]=XS[4][0]+helicity*XS[4][1]*XS[4][0]/100.0; //SpinDir 5, M1=-1/2, sigma
    XS[4][7]=XS[4][2]+helicity*XS[4][3]*XS[4][2]/100.0; //SpinDir 5, M1=+1/2, sigma
    XS[4][8]=XS[4][4]+helicity*XS[4][5]*XS[4][4]/100.0; //SpinDir 5, M1=0   , sigma

    XS[5][0]=TriplePolNS::buf[40]; //SpinDir 6, M1=-1/2, Sigma
    XS[5][1]=TriplePolNS::buf[41]; //SpinDir 6, M1=-1/2, A
    XS[5][2]=TriplePolNS::buf[42]; //SpinDir 6, M1=+1/2, Sigma
    XS[5][3]=TriplePolNS::buf[43]; //SpinDir 6, M1=+1/2, A
    XS[5][4]=TriplePolNS::buf[44]; //SpinDir 6, M1=0,    Sigma
    XS[5][5]=TriplePolNS::buf[45]; //SpinDir 6, M1=0,    A
    XS[5][6]=XS[5][0]+helicity*XS[5][1]*XS[5][0]/100.0; //SpinDir 1, M1=-1/2, sigma
    XS[5][7]=XS[5][2]+helicity*XS[5][3]*XS[5][2]/100.0; //SpinDir 1, M1=+1/2, sigma
    XS[5][8]=XS[5][4]+helicity*XS[5][5]*XS[5][4]/100.0; //SpinDir 1, M1=0   , sigma

    // printf("%4.3e %4.3e %4.3e %4.3e %4.3e %4.3e\n",XS[0][0],XS[0][1],XS[0][2],XS[0][3],XS[0][4],XS[0][5]);
    // printf("%4.3e %4.3e %4.3e %4.3e %4.3e %4.3e\n",XS[1][0],XS[1][1],XS[1][2],XS[1][3],XS[1][4],XS[1][5]);
    // cout << "----------" << endl;

    //    FourVector pCMS=Reaction->Out1,
    //      CMS=Target+Photon;
    //    pq=pq.Lorentz(-CMS);
    FourVector Deuteron=Target+Photon-Reaction->Out1;
    FourVector dq=Deuteron;
    dq=dq.rotateTo(Photon);

    // double 
    //   A1=XS[0][0]/(XS[0][1]*XS[0][0]/100.0),
    //   A2=XS[0][2]/(XS[0][3]*XS[0][2]/100.0),
    //   A3=(XS[0][1]*XS[0][0]/100.0)/XS[0][0],
    //   A4=(XS[0][3]*XS[0][2]/100.0)/XS[0][2];

    // printf("%5.4f %5.4f %5.4f %5.4f\n",
    // 	   A1,A2,A3,A4);

    // double
    //   E0      = Reaction->electronIn.energy(),
    //   Eprime  = Reaction->electronOut.energy(),
    //   the     = Reaction->electronOut.theta(),
    //   q2      = -4 * E0 * Eprime * pow(sin(the/2),2),
    //   epsilon = 1/(1 - 2*(pow(Photon.momentum(),2) / q2 * pow(tan(the/2),2))),
    //   epsilonL=-q2/(Photon.energy()*Photon.energy())*epsilon;

    // FourVector
    //   CMS=Photon+Target;
    // Photon=Photon.Lorentz(-CMS);
    // FourVector
    // Proton=Reaction->Out1.Lorentz(-CMS);
    // pq=Proton.rotateTo(Photon);
    
    // cout << Reaction->electronOut.energy()*1e3 << "\t" //1
    // 	 << Reaction->electronOut.theta()/rad << "\t"  //2
    // 	 << Reaction->electronOut.phi() << "\t"    //3
    // 	 << helicity << "\t"                           //4
    // 	 << Reaction->Out1.momentum()*1e3 << "\t"      //5
    // 	 << Reaction->Out1.theta() << "\t"         //6
    // 	 << Reaction->Out1.phi() << "\t"           //7
    // 	 << pq.theta() << "\t"                     //8
    // 	 << pq.phi() << "\t"                       //9
    // 	 << Deuteron.momentum()*1e3 << "\t"            //10
    // 	 << Deuteron.theta() << "\t"               //11
    // 	 << Deuteron.phi() << "\t"                 //12
    // 	 << dq.theta() << "\t"                     //13
    // 	 << dq.phi() << "\t"                       //14
    // 	 << epsilon << "\t"                            //15
    // 	 << epsilonL << "\t"                           //16
    // 	 << sqrt(2*epsilonL*(1-epsilon)) << "\t"       //17
    // 	 << sqrt(1-epsilon*epsilon) << "\t"            //18
    // 	 << XS[0][4]*1e7 << "\t" << XS[0][5]/100.0*XS[0][4]*1e7 << "\t" //19+20
    // 	 << XS[1][4]*1e7 << "\t" << XS[1][5]/100.0*XS[1][4]*1e7 << "\t" //21+22
    // 	 << XS[2][4]*1e7 << "\t" << XS[2][5]/100.0*XS[2][4]*1e7 << "\t" //23+24
    // 	 << XS[3][4]*1e7 << "\t" << XS[3][5]/100.0*XS[3][4]*1e7 << "\t" //25+26
    // 	 << XS[4][4]*1e7 << "\t" << XS[4][5]/100.0*XS[4][4]*1e7 << "\t" //27+28
    // 	 << XS[5][4]*1e7 << "\t" << XS[5][5]/100.0*XS[5][4]*1e7 << "\t" //29+30
    // 	 << pwia(Reaction->electronIn, Reaction->electronOut, 
    // 		 Reaction->Out1,Photon+Target-Reaction->Out1) << endl;

    if (sobol()*XSmax <= XS[0][8]) {
      Momentum Spin;
      double spinabs=(XS[0][7]-XS[0][6])/(XS[0][6]+XS[0][7]);
      //      if (helicity>0) spinabs=(-XS[1][7]+XS[1][6])/XS[1][8];
      Spin.initPolar(spinabs,57.7*rad,-180*rad);
      Reaction->Out1.setSpin(Spin);
    }

    weight *= XS[0][8] * 1e4;
  }

  return weight;
}


double generateTriplePol::pwia(FourVector Ein, FourVector Eout, FourVector Proton, FourVector Missing) 
{
  //See "Electromagnetic Response of Atomic Nuclei"
  //Chapter 7.2.4
  static const NucleonFormfactor *ff = new MergellFit();
  FourVector Photon = Ein - Eout;
  double 
    //    E        = Ein.energy(),
    pprime   = Proton.momentum(),
    Epprime  = Proton.energy(),
    thetae   = Eout.theta(),
    //    omega    = Photon.energy(),
    q        = Photon.momentum(),
    //    q2       = Photon * Photon,
    p        = Missing.momentum(), //initial proton momentum
    Ebar     = sqrt(sqr(p)+sqr(m_proton)),
    omegabar = Epprime - Ebar,
    Q2bar    = sqr(q) - sqr(omegabar),
    gamma    = Proton.rotate(Photon).theta(),
    //phi      = Proton.rotate(Photon).phi(),
    f1       = ff->F1(-Q2bar),
    f2       = ff->F2(-Q2bar),
    ff1      = f1 * f1 + Q2bar / 4 / sqr(m_proton) * f2 * f2,
    ff2      = sqr(f1 + f2),
    rho00    = sqr(Q2bar)/(sqr(q*q)),
    rho11    = Q2bar/(2*q*q)+sqr(tan(thetae/2)),
    rho01    = Q2bar/(q*q*sqrt(2))*sqrt(Q2bar/(q*q)+sqr(tan(thetae/2))),
    rho1m1   = -Q2bar/(2*q*q),
    sigmaM   = sqr(2*alpha/Q2bar*Eout.energy()*cos(thetae/2)),//sqr(alpha*cos(thetae/2)/(2*E*sqr(sin(thetae/2)))),
    g00      = 1/(4 * Ebar * Epprime) * (sqr(Ebar+Epprime)*ff1 - q*q*ff2),
    g11      = Q2bar/(2*Ebar*Epprime)*ff2 + sqr(pprime*sin(gamma))/(Ebar*Epprime)*ff1,
    g01      = -sqrt(2)*pprime*sin(gamma)/(Ebar*Epprime) * ff1,
    g1m1     = -sqr(pprime*sin(gamma))/(Ebar*Epprime) * ff1,
    sigmacc1 = sigmaM * (rho00*g00 + rho01*g01 + rho1m1*g1m1 + rho11*g11);

  return pprime * Epprime * sigmacc1
    * S(Missing.energy(),Missing.momentum())
    * 1/(1-Epprime/Missing.energy()*(Proton.getP()*Missing.getP()) 
	 / (Proton.getP() * Proton.getP())) * mubarn;

  //  return A/integrationVolume();
}

double generateTriplePol::S(double E, double p) {
   const double A = 14025.5;
   const double B = 0.304644;
   const double C = 3.18892;
   const double D = 41.6073;
   return A*(C*p+B)*(C*p+B)/(1+exp(D*p+B));

  // const double A = 924.606;  
  // const double B = 33.1758;
  // const double C = -0.685343;

  // const double A = 14326/(4*M_PI);
  // const double B = 30.38;
  // const double C = 0.192;
   //  return A/(1+exp(B*p+C));
}

void generateTriplePol::Output(FourVector Ein, FourVector Eout, FourVector Proton)
{
  FourVector Target(targetmass,0,0,0);
  FourVector Photon = Ein-Eout,
    Deuteron = Target + Photon - Proton;
  //  double q2=Photon*Photon;
  //  double W=(Target+Photon).mass();
  cout << "Eprime=" << Eout.energy() << "\ttheta_e=" << Eout.theta()/rad << "\tphi_e=" << Eout.phi()/rad 
       << "\nomega=" << Photon.energy() << "\tq=" << Photon.momentum() << "\ttheta_q=" << Photon.theta()/rad << "\tphi_q=" << Photon.phi()/rad
       << "\nEp=" << Proton.energy() << "\tpp=" << Proton.momentum() << "\ttheta_p=" << Proton.theta()/rad << "\tphi_p=" << Proton.phi()/rad
       << "\nEd=" << Deuteron.energy() << "\tpd=" << Deuteron.momentum() << "\ttheta_d=" << Deuteron.theta()/rad << "\tphi_d=" << Deuteron.phi()/rad
       << "\nmd=" << Deuteron.mass()
       << "\n-1/2 : sigma=" << TriplePolNS::buf[10] << "\tA=" << TriplePolNS::buf[11]
       << "\n+1/2 : sigma=" << TriplePolNS::buf[12] << "\tA=" << TriplePolNS::buf[13]
       << "\nunpol: sigma=" << TriplePolNS::buf[14] << "\tA=" << TriplePolNS::buf[15]
       << "\n---------------------------------------\n"; 
}
