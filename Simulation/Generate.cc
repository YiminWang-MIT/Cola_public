//                                                                    -*-C++-*-
// $Id: Generate.cc 2689 2017-05-31 10:21:51Z schlimme $
 
#include "Generate.h"
#include "QED.h"
#include "DM_QED.h"
#include "spinTransfer.h"            // spin transfer target -> focal plane
#include "Formfactor.h"
#include "Cola/Momentumdistribution.h"
#include <stdlib.h>
#include "FourVector/FourVector.h"
#include "Model/ChPT_VCS.h"
#include "Cola/include/online/online.h"
#include <fstream>
#include "Cola/Masses.h"
#include "float.h"

//test
#include "HMBook/hmbook.h"

#define OLDGEN 0

extern SobolSequence sobol;
double T1nrsimul;
double T1nrmaxsimul;

namespace He3HACK
{
  ifstream csfile;
  ofstream listfile;
  int mode=0;
  int num=0;
  float buf[16];

}
static double ct2;
int invbh=0;
const double llimit_k= 1e-10;

using namespace std;

// ----------------------------------------------------------------------------

double Spence(double x);

int find_c(const double&, const double&,
	   const double&, const double&, const double&,
	   double&, double&, double&,
	   double&, double&, double&, double&);

int zcomp(const double&, const double&);

const int protonContrib = getenv("PROTONCONTRIBUTION") != NULL;

// ============================================================================

void 
eventGenerator::generateLabAngles (Particle *P, double p, double theta0, 
				   double phi0, double dcostheta, double dphi)
{
  double costheta = (2*sobol()-1) * dcostheta;
  double phi      = (2*sobol()-1) * dphi;
  P->initPolar(energy(P->getMass(),p),  p,   acos(costheta),  phi);
  if (phi0!=0) P->rot_phi(phi0);
  P->rot_theta(theta0 - M_PI/2);
}  

double 
eventGenerator::calc_elasticEprime(double E0, double theta, double M) //E0 beam energy [GeV], theta scattering angle[rad], M target mass[GeV/c2]
{//more precise than approx with m_e<<1: Eprime = E0*M/(M+E0*(1.0-cos(theta)))
// see Friedrich phd 2000, A.11 [Merkel], with some sign swaps (compensates) and sin^2(x) = 1/2-cos(2x)/2
  return  ((-E0-M)*(m_e_sqr+E0*M) + (m_e_sqr-E0*E0)*cos(theta)*sqrt(M*M-m_e_sqr/2+m_e_sqr*cos(2*theta)/2))
    / ((E0*E0-m_e_sqr)*pow(cos(theta),2)-pow(E0+M,2));
}

double 
eventGenerator::eout_W_Q2_Phi(double setq2, double dW, double dq2) 
{
  double 
    E0  = Reaction->electronIn.energy(),
    W   = Reaction->threshold + sobol() * dW,
    q2  = setq2 + dq2 * (sobol() - 0.5),
    Ep  = E0 - (W * W - targetmass*targetmass - q2) / 2 / targetmass,
    arg = -q2 / 4 / E0 / Ep;
  if (arg<0 || arg>1) return 0; // possible due to radiation corrections
  double theta = 2 * asin(sqrt(arg));
  double phi   = dphie * (sobol() * 2 - 1);
  Reaction->electronOut.initPolar(energy(m_electron, Ep), Ep, theta, phi);
  //  if (sime->getOop()!=0) Reaction->electronOut.rot_phi(sime->getOop());
  return W / (2*E0*Ep*targetmass); // Jacobian dW,Dq2 -> dE,dcos(theta)
}

double 
eventGenerator::eout_p_Q2_Phi(double setq2, double dpcms, double dq2) 
{
  double 
    q2   = setq2 + dq2 * (sobol() - 0.5),
    pcms = dpcms *sqrt(sobol()),
    phi  = dphie * (sobol() * 2 - 1),
    E0   = Reaction->electronIn.energy(),
    m1   = Reaction->getM1()->getMass(),
    m2   = Reaction->getM2()->getMass(),
    E1   = sqrt(pcms * pcms + m1 * m1),
    E2   = sqrt(pcms * pcms + m2 * m2), 
    s    = pow(E1 + E2,2),
    W    = sqrt(s),
    Ep  = E0 - (W * W - targetmass*targetmass - q2) / 2 / targetmass,
    arg  = -q2 / 4 / E0 / Ep;
  if (arg<0 || arg>1) return 0; // possible due to radiation corrections
  double theta = 2 * asin(sqrt(arg));
  
  Reaction->electronOut.initPolar(energy(m_electron, Ep), Ep, theta, phi);
  //  if (sime->getOop()!=0) Reaction->electronOut.rot_phi(sime->getOop());
  return (dpcms/2) * (2 + E2/E1 + E1/E2) / (2 * E0 * Ep * targetmass);
}

double 
eventGenerator::eout_Q2_Theta_Phi(double setq2, double dq2) 
{   
  generateLabAngles(&Reaction->electronOut, 1, 
		    sime->getAngle(), sime->getOop(), dcte, sime->getDphi());
  
  double 
    E0    = Reaction->electronIn.energy(),
    theta = Reaction->electronOut.theta(),
    phi   = Reaction->electronOut.phi(), 
    q2    = setq2 + dq2 * (sobol() - 0.5),
    Ep    = q2 / (- 4 * E0 * pow(sin(theta/2),2));
  if (Ep > E0 || Ep < 0) return 0; // generated energyloss to large?
  
  Reaction->electronOut.initPolar(energy(m_electron, Ep), Ep, theta, phi);
  return Ep / -q2; // Jacobian dq2 -> dE
}

double 
eventGenerator::eoutIntegrationVolume(double dW, double dq2, double dpcms)
{
  this->dpcms = dpcms;
  if (dW    != 0) return dW    * dq2 * dphie * 2;
  if (dpcms != 0) return dpcms * dq2 * dphie * 2;
  return                 4 * sime->getDphi() * dcte * dq2;
}


// integral of 1/q^4 sin(theta) dtheta from 0 to theta, 
// needed for event generator (m_electron!=0 !!!)
inline double peE(double E) {return sqrt(E*E - m_electron*m_electron);}

inline double intq2(double E, double Ep, double theta) {
  double  p0 = peE(E), pp = peE(Ep);
  return -1/(2*p0*pp*(p0*p0+pp*pp-pow(E-Ep,2)-2*p0*pp*cos(theta)));
}

inline double intsf(double E, double Ep, double y) {
  double  p0 = peE(E), pp = peE(Ep);
  return acos((1/y+2*p0*pp*(-pow((E-Ep),2)+p0*p0+pp*pp))/(4*p0*p0*pp*pp));
}

// integral of 1/q^2 sin(theta) dtheta from 0 to theta, 
// needed for event generator (m_electron!=0 !!!)

inline double int2q2(double E, double Ep, double theta) {
  double  p0 = peE(E), pp = peE(Ep);
  return log(-2*p0*pp*cos(theta) - pow(E - Ep,2) + p0*p0 + pp*pp)/p0/pp/2;
}

inline double int2sf(double E, double Ep, double y) {
  double  p0 = peE(E), pp = peE(Ep);
  return acos((-exp(2*p0*pp*y) - pow(E-Ep,2) + p0*p0 + pp*pp)/2/p0/pp);
}

int testit() {
  double E  = 0.240;
  double Ep = 0.010;
  int n=180;
 
  HIST *id2 = HMBook1("Random",   "", "", "","",n,0.0,180.0);
  HIST *id2b = HMBook1("Random orig",   "", "", "","",n,0.0,180.0);
  
  double min = int2q2(E, Ep, 0);
  double max = int2q2(E, Ep, M_PI);
  double p0 = sqrt(E*E-m_electron*m_electron);
  double pp = sqrt(Ep*Ep-m_electron*m_electron); 
  FourVector ein(E, 0, 0, p0);

  int N=10000000;
  for (int i=0;i<N;i++) {
    double invert = int2sf(E, Ep,  min+drand48()*(max-min));
    double theta=acos(2*rndm()-1);
    FourVector eout(Ep, 0, pp *sin(theta), pp * cos(theta));
    FourVector photon = ein-eout;

    //    cout <<invert<<endl;
    HMFill(id2,  invert/M_PI*180, 0, (max-min)/N);
    HMFill(id2b, theta /M_PI*180, 0, pow(photon.square(),-1)*2/N);
  }
  HMOperation(id2,'/',id2b,"Ratio","","");
  HMWriteAll("Generatortest.his");
  exit(0);
  return 4711;
}
//static int test = testit();
///////////////////////////////////////////////////////////////////////////////
//
// - mA, mass of lepton pair generated from [m_A*0.8, m_A*1.15]
// - E,  energy of leptonpair generated from [Emin, Ebeam]
//

double generateDMQEDBackground::generateEvent(double) 
{
  double mt = Reaction->target.getMass();
  double E0 = Reaction->electronIn.energy();
  double p0 = Reaction->electronIn.momentum();
  FourVector CMS(E0+mt, 0, 0, p0);
  double s = CMS*CMS;
  mA = m_A;
  if (m_A<0) mA = -m_A*0.8 - m_A*0.35*sobol();  
  if (s< (mt+mA)*(mt+mA)) return 0;

  double pAcms = p_cms(s, mA, mt);
  FourVector AmaxCms(sqrt(mA*mA+pAcms*pAcms),0,0,pAcms);
  Emax = AmaxCms.Lorentz(CMS).energy();

  double E = Emin + (rundb.Ebeam/1000 - Emin) * sobol(); 
           // dE' = (rundb.Ebeam/1000 - Emin) 
  if (E>Emax || E<mA) return 0;
  
  double x=E/E0;
  if (x>0.9999) return 0;
  double p = sqrt(E*E-mA*mA);

  double b = mA/E0/x*sqrt(1-x); //generate theta with 1/(theta^2+b^2) distribution)
  double theta = b * tan(sobol()*atan(M_PI/b));
  double phi = sobol()*2*M_PI;
  jacobian = sin(theta) * (theta*theta+b*b)/b*atan(M_PI/b);
  //               sin(theta) from dcostheta = sin(theta)dtheta

  A.initPolar(E, p, theta, phi);                   //dOmega_A'! = 4 * M_PI
  
  FourVector epsystem = CMS - A;

  double s_ep = epsystem * epsystem;
  if (s_ep < pow(m_electron+mt, 2)) return 0;
  double p_ep = sqrt((s_ep - pow(m_electron + mt,2)) 
                   * (s_ep - pow(m_electron - mt,2))/4/s_ep); 

  static int graphs = getenv("TRIDENT") ? atoi(getenv("TRIDENT")) : 1;

  FourVector eep, einep = Reaction->electronIn.Lorentz(-epsystem);
  double Ep = sqrt(p_ep*p_ep + m_electron * m_electron);
  double Ebeam_ep = einep.energy();

  if (graphs==2 || graphs==1) {
    // Generate theta' (lab scattered e angle) with 1/q4*sin(theta) distribution
    // for trident graphs
    double cmin = intq2(Ebeam_ep, Ep, 0);
    double cmax = intq2(Ebeam_ep, Ep, M_PI);
    double etheta = intsf(Ebeam_ep, Ep, cmin + sobol()*(cmax - cmin));
    eep = Polar(Ep, p_ep, etheta, sobol()*2*M_PI);
    jacobian *= (cmax-cmin)*pow((einep-eep).square(),2)/2;
  } else if (graphs==3) {
    // Generate theta' (lab scattered e angle) with 1/q2*sin(theta) distribution
    // for intereference term
    double cmin = int2q2(Ebeam_ep, Ep, 0);
    double cmax = int2q2(Ebeam_ep, Ep, M_PI);
    double etheta = int2sf(Ebeam_ep, Ep, cmin + sobol()*(cmax - cmin));
    eep = Polar(Ep, p_ep, etheta, sobol()*2*M_PI);
    jacobian *= (cmax-cmin)*-(einep-eep).square()/2;  
  } else // Isotropic
    eep = Polar(Ep, p_ep, acos(sobol()*2-1), sobol()*2*M_PI);

  Reaction->electronOut = eep.rotateTo(einep).Lorentz(epsystem);
  
  // dOmegaE' = 4 * M_PI
  // Isotropic decay

  double thetaDecay  = acos(ctmin + (ctmax - ctmin) * sobol());
  double phiDecay = phmin+(phmax-phmin)*sobol(); //dOmegaDecay = (ctmax-xtmin)
                                                 //             *(phmax-phmix)
  double pdecay      = sqrt(mA*mA - 4*pow(m_electron,2))/2;
  Reaction->Decay1 = P_electron;
  Reaction->Decay2 = P_positron;
  Reaction->Decay1 = Polar(energy(m_electron, pdecay), pdecay, 
			   thetaDecay, phiDecay).Lorentz(A);
  Reaction->Decay2 = Polar(energy(m_electron, pdecay), pdecay, 
			   M_PI-thetaDecay, phiDecay+M_PI).Lorentz(A);

  mtheta = Reaction->Decay1.Lorentz(-A).rotate(A).theta();
  mphi   = Reaction->Decay1.Lorentz(-A).rotate(A).phi();
  
  return 1;
}

double generateDMBoson::generateEvent(double) 
{
  double mt = Reaction->target.getMass();
  double E0 = Reaction->electronIn.energy();
  double p0 = Reaction->electronIn.momentum();
  FourVector A, CMS(E0+mt, 0, 0, p0);
  double s = CMS*CMS;
  double mA = m_A;

  if (m_A<0) mA = -m_A - 0.050 + 0.100*sobol(); 

  if (s< (mt+mA)*(mt+mA)) return 0;
  double pAcms = p_cms(s,mA,mt);
  FourVector AmaxCms(sqrt(mA*mA+pAcms*pAcms),0,0,pAcms);
  Emax = AmaxCms.Lorentz(CMS).energy();
  double E = Emin + (rundb.Ebeam/1000 - Emin) * sobol();
  if (E>Emax || E<mA) return 0;
  double x=E/E0;
  if (x>0.9999) return 0;
  double p = sqrt(E*E-mA*mA);
  double b = mA/E0/x*sqrt(1-x); //generate theta with 1/(theta^2+b^2) distribution)
  double theta = b * tan(sobol()*atan(M_PI/b));
  double phi = sobol()*2*M_PI;
  double jacobian = sin(theta) * (theta*theta+b*b)/b * atan(M_PI/b);
   //               sin(theta) from dcostheta = sin(theta)dtheta
  A.initPolar(E, p, theta, phi);

  FourVector epsystem = CMS - A;
  double s_ep = epsystem * epsystem;
  if (s_ep < pow(m_electron+mt, 2)) return 0;
  double p_ep = sqrt((s_ep - pow(m_electron + mt,2)) 
		  * (s_ep - pow(m_electron - mt,2))/4/s_ep); 
  Reaction->electronOut = Polar(energy(m_electron,p_ep), p_ep, 
				acos(rndm()*2-1), rndm()*2*M_PI).Lorentz(epsystem);
  // Isotropic decay
  double costhetaDecay  = ctmin + (ctmax - ctmin) * sobol();
  double thetaDecay  = acos(costhetaDecay);
  double phiDecay    = phmin + (phmax-phmin) * sobol();
  double pdecay      = p_cms(mA*mA,m_electron, m_electron);
  Particle emcms = P_electron;
  Particle epcms = P_positron;
  emcms.initPolar(sqrt(m_electron*m_electron+pdecay*pdecay),pdecay, thetaDecay,phiDecay);
  epcms.initPolar(sqrt(m_electron*m_electron+pdecay*pdecay),pdecay, 
		  M_PI-thetaDecay, phiDecay+M_PI);
  Reaction->Decay1=emcms.Lorentz(A);
  Reaction->Decay2=epcms.Lorentz(A);
  if (m_A<0) jacobian *= 2*alpha/3/M_PI/mA*0.100;
  //  cout <<crosssection(E0, E, theta, mA, 1)/((3*M_PI/2/alpha)*(mA/0.1))
  //  /BHBackground(E0, E, theta,  mA, 0.1, costhetaDecay)<<" "<<costhetaDecay
  //							<<endl;
  
    return DMHeavyCS(Reaction->electronIn, Reaction->electronOut, A, mA)* jacobian;
    //		       
  //  return (heavycrosssection(E0, E, theta, mA, 1)/((3*M_PI/2/alpha)*(mA/0.1))
  //                                                       ) * jacobian;
  // + BHBackground(E0, E, theta, mA, 0.1, costhetaDecay)) * jacobian;
}

double generateElastic::generateEvent(double helicity)
{
  //std::cerr << "generateElastic" << std::endl;
  double E0 = Reaction->electronIn.energy();
  generateLabAngles(&Reaction->electronOut,1,sime->getAngle(), sime->getOop(), 
		    dcte,sime->getDphi());
  
  double theta = Reaction->electronOut.theta();
  double phi   = Reaction->electronOut.phi();
  double Ep    = (E0-Ex-Ex*Ex/2/targetmass)/(1+E0/targetmass*(1-cos(theta)));

  Reaction->electronOut.initPolar(energy(m_electron, Ep), Ep, theta, phi);   
  Reaction->Out1 = Reaction->electronIn - Reaction->electronOut 
    + *Reaction->getTarget();
  
  return 1;
}

double generateElasticRadiative::generateEvent(double helicity)
{
  //std::cerr << "generateElastic" << std::endl;
  double E0 = Reaction->electronIn.energy();
  generateLabAngles(&Reaction->electronOut,1,sime->getAngle(), sime->getOop(), 
		    dcte,sime->getDphi());
  
  double theta = Reaction->electronOut.theta();
  double phi   = Reaction->electronOut.phi();
  //enough calculation at this moment, need to hand the information to cooker
  //generator
  
  //set up basic information
  gen->setElectronTheta(theta);
  gen->setElectronPhi(phi);
  //std::cerr << "sime" <<  sime->getAngle() << std::endl;
  //for normalization
  //gen->setThetaRange(sime->getAngle()/M_PI*180.0, (sime->getAngle()+dcte)/M_PI*180.0);
  //gen->setPhiRange(sime->getOop()/M_PI*180.0, (sime->getOop()+sime->getDphi())/M_PI*180.0);
  gen->setBeamEnergy(E0*1000);//convert to MeV

  ge->lepton_prescatter.particle="e-";
  ge->lepton_prescatter.momentum = TLorentzVector(Reaction->electronIn[1], Reaction->electronIn[2], Reaction->electronIn[3], Reaction->electronIn[0]); 
  
  //simulate this event
  gen->generate(ge);

  double weight=0;

  //pull back information from cooker generator
  if (getenv("SPLINEFIT") && atoi(getenv("SPLINEFIT"))==1){
    std::cout<<"Olympis generator: Using Bernauer spline fit!"<<endl;
    weight=ge->weight.get_default();
  } else{
    weight=ge->weight.get_extra("method1_dipole");
  }
  //std::cout << ge->particles[0].particle << std::endl;
  //std::cout << ge->particles[0].momentum.E() << std::endl;
  Reaction->electronOut.initPolar(ge->particles[0].momentum.E(), ge->particles[0].momentum.P(), ge->particles[0].momentum.Theta(), ge->particles[0].momentum.Phi());
  Reaction->Out1.initPolar(ge->particles[1].momentum.E(), ge->particles[1].momentum.P(), ge->particles[1].momentum.Theta(), ge->particles[1].momentum.Phi());

  /*
  std::cout << Reaction->electronOut.energy() << std::endl;
  std::cout << Reaction->electronOut.theta() << std::endl;
  std::cout << weight << std::endl;
  */
  return weight;
}

//needed for Bremsstrahlung
inline double securePow(double x, double y) {
  if (x==0) return 0;
  double e=y*log(fabs(x));
  return e<-500 ? 0 : e>709 ? DBL_MAX : exp(e);
}

//#############################################################################

#if OLDGEN 
double 
generateBremsstrahlung::generateEvent(double helicity)
{
  double E0 = Reaction->electronIn.energy();
  FourVector In = FourVector(E0,0,0,Reaction->electronIn.momentum());
  // Set the electronOut FourVector with momentum = 1
  generateLabAngles(&Reaction->electronOut,1,sime->getAngle(), sime->getOop(), 
		    dcte,sime->getDphi());
  
  double theta = Reaction->electronOut.theta();
  double phi   = Reaction->electronOut.phi();


  double ct    = cos(theta), st = sin(theta);
  // energy of the outgoing electron in elastic scattering
  
  double Ep    = (-(E0 + m_proton)*(m_e_sqr+E0*m_proton)
		  + (m_e_sqr-E0*E0)*ct*sqrt(m_proton*m_proton-m_e_sqr*st*st))/
    ((E0*E0-m_e_sqr)*ct*ct-(E0+m_proton)*(E0+m_proton));

  // Set the electronOut FourVector
  Reaction->electronOut.initPolar(Ep, sqrt(Ep*Ep-m_e_sqr), theta, phi);
  Reaction->Out1 = In - Reaction->electronOut + *Reaction->getTarget();
  double q2 = (In-Reaction->electronOut).square();

  // correction to elastic electron scattering at the proton side as in Physical Review C, Volume 62, 025501, QED radiative corrections to virtual Compton scattering (M. Vanderhaeghen); p. 36 Eq. A74-A76. The energy loss is generated like on p. 19 only the term with Delta Es is used, the rest is assumed to be constant in the regime of interest. For Eq. A74 it is used: exp(deltaR)=(2*DeltaEs/Q/sqrt(x))**(4*alpha/pi*log(eta)); it is DeltaEs1=Q*sqrt(x)/2*random**(1/(4*alpha/pi*log(eta))); for Eq. A76 it is used: exp(deltaR)=(2*DeltaEs/MN)**(2*alpha/pi*(ENprime/pNprime*log(x)-1)); it is DeltaEs=MN/2*random**(1/(2*alpha/pi*(ENprime/pNprime*log(x)-1))); DeltaEs=DeltaEs+DeltaEs1; Eprime=-DeltaEs/eta+Ep (Eq. A48)

  double random = halton[4]();
  double eta = E0/Ep;
  double rho = sqrt(fabs(q2) + 4*m_proton*m_proton);
  double x = pow(sqrt(fabs(q2)) + rho,2)/(4*pow(m_proton,2));
  double t = 4*alpha/M_PI*log(eta);
  double DeltaEs = pow(fabs(q2)*x,0.5)/2*securePow(random,1./t);
  t = 2*alpha/M_PI*(Reaction->Out1.energy()/Reaction->Out1.momentum()*log(x)-1);
  DeltaEs+=m_proton/2*securePow(random,1./t);
  double Eprime = -DeltaEs/eta+Ep;
  if(Eprime <= m_electron) // make sure that there is any energy for the electron left
    {
      Eprime = m_electron+1e-10;
    }

  // Out1 is the excited proton
  Reaction->electronOut.initPolar(Eprime, sqrt(Eprime*Eprime-m_e_sqr), theta, phi);

  /*double Eproton = Reaction->Out1.energy();  
  double beta4 = Reaction->Out1.momentum()/Eproton;
  double tproton = 4 * log(E0/Ep) + log((1+beta4) / (1-beta4)) / beta4 - 2;*/
  // Internal radiative corrections for outgoing electron
  
  t =alpha/M_PI*(2*(log(fabs(q2)/m_e_sqr)-1));// + (protonContrib?0*tproton:0)); this part does not make any difference for 1M events, but it is not sure, if we can use the generation for the energy loss the same way, if we include the proton part.
  if ( t <= 0 ) return 0.0; // if t<=0 ,then q2 < m_e_sqr and then the formula is not valid any more
  //  cout <<"protonContrib:"<<protonContrib<<" "<<tproton<<" "<<log(fabs(q2)/m_e_sqr)-1<<endl;
  class Momentum P_Spin;
  class FourVector photon;
  
  double EBH,k;
  double dPhotonNorm;

  FourVector eoutsave = Reaction->electronOut;

  Reaction->electronOut=eoutsave;

  random = halton[2]();

  dPhotonNorm = 1.0;
  if ((BHmin != 0) && (BHmax != 0)) {
    double min =  pow(BHmin/Ep,t);
    double max =  pow(BHmax/Ep,t);
    random = min + random*(max - min);
    dPhotonNorm = (max - min);
  }
  double cutoff = securePow(RadCutOff,t);
  if (random<cutoff) return 0;

  // Energy loss because internal radiative corrections
  k = (Ep-m_electron)*securePow(random, 1.0/t);// the maximum energy the electron can lose is Ep-m_electron, if it would be more, than the mass of the electron would be smaller than m_electron
  double kk = k;  // store the real value of k
  if (k<1e-10)  k=1e-10; // k is the energy loss we put in the Bethe-Heitler calculation
  if ( kk == 0 ) return 0; // the weight would be 0 anway but some calculation does not work when kk == 0

  Reaction->electronOut -= k;


  k = Ep - Reaction->electronOut.energy(); // to minimize roundoff error

  double weight = 1.0;

  theta= Reaction->electronOut.getP()* Reaction->electronIn.getP();
  theta/=Reaction->electronOut.getP().abs()* Reaction->electronIn.getP().abs();
  theta=acos(theta);


  generateBetheHeitlerPeak BH(In, Reaction->electronOut);

  P_Spin=Momentum();

  bool lowerlimit;
  EBH=k;
  photon = BH.generate(weight,&lowerlimit,&EBH,&helicity,&P_Spin);
  double w2 = k/t/EBH;//  kk/t*securePow(kk/(Ep-m_electron),-t)/EBH: this is the correct factor, but k/t/EBH seems to work better;  kk/t*securePow(kk/(Ep-m_electron),-t) is correcting for the distribution with which the radiative tail is generated; /EBH is giving back the missing photon momentum
  if (weight<0){
    invbh++;
    if (invbh %100 ==0)   printf("Warning, Invalid BH.generate %i\n",invbh);
    return 0; // invalid event, very rare case 
  }

  // Out1 is now the final proton
  Reaction->Out1 = In + *Reaction->getTarget()-Reaction->electronOut - photon;
  
  Reaction->Out1 = Reaction->Out1.setSpin(P_Spin);


  /*
  
 //--- Now we need to consider, that the second order correction to the BH
  // and not to elastic are needed. Since Generator a priori considers the
  // elastic correction, we need to correct for the ratio:
  
  double deltaR = RealBHCorrection(Reaction->electronIn, Reaction->electronOut, Reaction->target, Reaction->Out1, photon2);
  RadiativeCorrection = exp(deltaR); // -- this is correction factor
  //cout<<"Correction: "<< RadiativeCorrection<< " Photon2 " << photon2.energy() << " real " << exp(deltaReal) << " Eprime " << Reaction->electronOut.energy() << " Photon " << photon.energy() << endl;
  //cout << Reaction->electronOut.energy() << " " << RadiativeCorrection << endl;
  weight *= RadiativeCorrection;
*/
  
  //--- For the complete higher order corrections some terms need to be added to 
  // the weight. With approx. random generator, we consider only part of the 
  // higher order correction. To get the complete exp(deltaR) correction, we 
  // need additional eta^(t/2) term. Of course, the VertexCorrection method still 
  // needs to be considered, to account for the part of the correction, that
  // does not depend on energy of emitted photon.
  /*
  if (protonContrib) {    
    //--- Missing terms for the whole Vanderhaeghen type higher order proton correction
    // are calculated using this function!
    double protonCorr = protonVertexCorrectionVanderhaeghen(Reaction->electronIn, Reaction->electronOut,  Reaction->Out1);
    //cout<<"Proton Correction: "<<Reaction->electronOut.energy()<<",  "<<protonCorr<<endl;  
    if (protonCorr>1.1) cout <<"pVC: "<<protonCorr<<endl;
    weight *= protonCorr;    
  }
  */
  
  
  // correction to Bethe-Heitler Diagrams as in Physical Review C, Volume 62, 025501, QED radiative corrections to virtual Compton scattering (M. Vanderhaeghen); p. 14 Eq. 58 and following. The energy loss is generated like on p. 19 only the term with Delta Es is used, the rest is assumed to be constant in the regime of interest.
  random = halton[3]();
  q2 = (In-Reaction->electronOut).square();
  double ktimesqprime = In * photon;
  double kprimetimesqprime = Reaction->electronOut * photon;
  double EtildeEtildeprimesqrt = securePow(4*(In.energy()-fabs(q2)/2/m_proton-ktimesqprime/m_proton)*(Reaction->electronOut.energy()+fabs(q2)/2/m_proton-kprimetimesqprime/m_proton),0.5); // = ( 4* Etilde * Etildeprime * Mm1**2 / MN**2)**0.5
  t =2*alpha*alpha/M_PI*(log(fabs(q2)/m_e_sqr)-1);  //  alpha*alpha because it is second order correction
  if ( t <= 0 ) return 0.0; // if t<=0 ,then q2 < m_e_sqr and then the formula is not valid any more
  k = EtildeEtildeprimesqrt * securePow(random, 1.0/t); // = (Mm1**2-MN**2)/MN; assume softphoton = l *(Reaction->electronOut.momentum.abs(),Reaction->electronOut.momentum()); it is Mm1**2-MN**2 = 2*Reaction->Out1 * softphoton => l = (Mm1**2-MN**2) / (2*Reaction->Out1*(Reaction->electronOut.momentum.abs(),Reaction->electronOut.momentum())) = k * MN / (2*Reaction->Out1*(Reaction->electronOut.momentum.abs(),Reaction->electronOut.momentum())), therefore the new electron out is: Reaction->electronOut-= l*Reaction->electronOut.momentum.abs()
  
  FourVector photon2 = FourVector(Reaction->electronOut.momentum(),Reaction->electronOut.getP());// this is just used for the calculation and not the soft photon
  if( (k * m_proton / 2 / (Reaction->Out1*photon2)*(Reaction->electronOut.momentum())) < Reaction->electronOut.energy() - m_electron - 1e-10) // make sure that there is still energy for the electron left
    {
      Reaction->electronOut-=(k * m_proton / 2 / (Reaction->Out1*photon2)*(Reaction->electronOut.momentum()));
      }
      //cout << k/2. << " " << Reaction->Out1*photon2 << " " << (Reaction->Out1.energy())*(photon2.energy()) - (Reaction->Out1.momentum())*(photon2.momentum()) << " " << EtildeEtildeprimesqrt << endl;
      

  double we=weight*w2;
  if (!(we>=0))
    {
      we=0;
      //cout <<"Error:"<< weight<<" "<<w2<<endl;//" "<<w3<<endl;
    }
  return we*dPhotonNorm;
}
#endif

////////////
//Jan's old generator
////////////
double protonVertexCorrection(double Ein, double Eout, double Eproton,double pproton,double x,double rho2)
{
  // JCB: this is the maximon tjon version. marc says: it is better!
   double (*s)(double) = Spence;  


   double eta=Ein/Eout;
   double lx=log(x);
   double delta1=s(1-eta/x)-s(1-1.0/(eta*x));
   double delta2=1+Eproton/pproton*(-0.5*lx*lx-lx*log(rho2/(m_proton*m_proton))+lx-s(1-1.0/(x*x))+2*s(-1.0/x)+M_PI*M_PI/6);

   return alpha/M_PI*(2*delta1+delta2);//missing the first term


  // mo tsai code
//   // 33musec on a Pentium III with 500MHz for this function!

//   const double Z = 1;
//   double (*s)(double) = Spence;  
//   double e1=Ein/m_proton, e3= Eout/m_proton, e4=Eproton/m_proton, 
//     c1=2*e3*e4-e1, c2=2*e1*e4-e3, eta = e1/e3;
//   double beta4 = sqrt(1 - 1/e4/e4);
//   double bb = sqrt((1  + beta4) / (1  - beta4));
//   double ee = sqrt((e4 -     1) / (e4 +     1));

//   double vertexZ =
//     -6*log(eta)*log(eta) +
//     (s((e3-1)/e1) - s((1-e3)/c1) + s(2*e3*(1-e3)/c1) 
//      - log(fabs(c1/(e1*(1-2*e3))))*log(2*e3) ) -
//     (s(1-e4/e3) - s((e4-e3)/c2) + s(2*e1*(e4-e3)/c2) 
//      - log(fabs(c2/(e3*(1-2*e1))))*log(2*e1) ) -
//     (s(1-1/e1)-s(1/e1-1)+s(2-2*e1)+log(fabs(2*e1-2))*log(2*e1))+
//     (s(1-1/e3)-s(1/e3-1)+s(2-2*e3)+log(fabs(2*e3-2))*log(2*e3));
  
//   double vertexZ2 =
//     - log(e4) - log(eta/e1)*(2*log(bb)/beta4-2)
//     + (log(bb)*log((e4+1)/2)-s(-ee*bb)+ s(ee/bb)-s(ee)+s(-ee))/beta4;

// //   double internZ  = -4*log(eta)                         * log(k/E1);
// //   double internZ2 = -(log((1+beta4)/(1-beta4))/beta4-2) * log(k/E1);

// //   cout.form("Vertex Z  = %.10g\n", -alpha/M_PI * vertexZ);
// //   cout.form("       Z² = %.10g\n", -alpha/M_PI * vertexZ2);
// //   cout.form("Intern Z  = %.10g\n", -alpha/M_PI * internZ);
// //   cout.form("       Z² = %.10g\n", -alpha/M_PI * internZ2);
// //   cout.form("Sum    Z  = %.10g\n", -alpha/M_PI * (internZ  + vertexZ));
// //   cout.form("       Z² = %.10g\n", -alpha/M_PI * (internZ2 + vertexZ2));

//   cout <<  - alpha / M_PI * (Z * vertexZ + Z * Z * vertexZ2)<<" \t"<< - alpha / M_PI * Z * vertexZ<<"\t"<< - alpha / M_PI * Z *Z* vertexZ2<<endl;
 
//  return - alpha / M_PI * (Z * vertexZ + Z * Z * vertexZ2);
}


int dummda=0;
double 
generateBremsstrahlung::generateEvent(double helicity)
{
  double E0 = Reaction->electronIn.energy();
  FourVector In = FourVector(E0,0,0,Reaction->electronIn.momentum());
  // Set the electronOut FourVector with momentum = 1
  generateLabAngles(&Reaction->electronOut,1,sime->getAngle(), sime->getOop(), 
		    dcte,sime->getDphi());
  
  double theta = Reaction->electronOut.theta();
  double phi   = Reaction->electronOut.phi();


  double ct    = cos(theta), st = sin(theta);
  // energy of the outgoing electron in elastic scattering
  
  double Ep    = (-(E0 + m_proton)*(m_e_sqr+E0*m_proton)
		  + (m_e_sqr-E0*E0)*ct*sqrt(m_proton*m_proton-m_e_sqr*st*st))/
    ((E0*E0-m_e_sqr)*ct*ct-(E0+m_proton)*(E0+m_proton));
  
  // Set the electronOut FourVector
  Reaction->electronOut.initPolar(Ep, sqrt(Ep*Ep-m_e_sqr), theta, phi);
  // Out1 is the excited proton
  Reaction->Out1 = In - Reaction->electronOut 
    + *Reaction->getTarget();
  
  double Eproton = Reaction->Out1.energy();  
  double q2 = (In-Reaction->electronOut).square();
  //  double beta4 = Reaction->Out1.momentum()/Eproton;

  double eta=E0/Ep;

  double Epp= E0+m_proton-Ep;
  double pp=sqrt(Epp*Epp-m_proton*m_proton);

  double rho=sqrt(4*m_proton*m_proton-q2);
  double x=(rho+sqrt(-q2))/(2*m_proton);  x=x*x;

  double tproton1 = alpha/M_PI*4*log(eta); // eqn 5.14

  double tproton2 = alpha/M_PI*2*(Epp/pp*log(x)-1); // eqn 5.15
  
  double telectron=alpha/M_PI*(2*(log(fabs(q2)/m_e_sqr)-1)); //eqn 5.9
  double t =telectron+ (protonContrib?tproton1+tproton2:0);

  //cout <<E0<<"\t"<<Ep<<"\t"<<t<<"\t"<<telectron<<"\t"<<tproton1<<"\t"<<tproton2<<endl;
//   cout <<"Q2, m_proton, x: "<<-q2<<" "<<m_proton<<" "<<x<<endl;
  class Momentum P_Spin;
  class FourVector photon;
  
  double EBH,k;
  double dPhotonNorm;

  FourVector eoutsave = Reaction->electronOut;

//   HIST *ida,*idb,*idc,*idd;

//   ida=HMBook1("Ohne weight dE","dE",""    ,"GeV","",2000,-1,1);
//   idb=HMBook1("Ohne weight dEs","dEs",""   ,"GeV","",2000,-1,1);
//   idc=HMBook1("Mit weight dE","",""   ,"Gev","",2000,-1,1);
//   idd=HMBook1("mit weight dEs","","","GeV","",2000,-1,1);


//   cout <<"E:"<<E0<<" E':"<<Ep<<" t:"<<t<<endl;

//   dummda++;
//   if (dummda!=1) return 0;

//   #define iterations 100000
//   for (int i=0;i<iterations;i++)
//     {
//       if ((i % (iterations/100))==0) std::cout <<i<<" "<<i*100.0/iterations<<"%"<<endl;


  //generating events with good distribution
  double random = halton[2]();

  dPhotonNorm = 1.0;
  if ((BHmin != 0) && (BHmax != 0)) {
    double min =  pow(BHmin/Ep,t);
    double max =  pow(BHmax/Ep,t);
    random = min + random*(max - min);
    dPhotonNorm = (max - min);
  }
  double cutoff = securePow(RadCutOff,t);
  if (random<cutoff) return 0;
  

  // Energy loss because internal radiative corrections
  // always soft corrections, ignore e mass?
  k = Ep *securePow(random, 1/t);
  //std::cout << Ep << '\t' << random << '\t' << 1/t << '\t' << k << std::endl;
  //  k=0.0405;

  double rk=k;
  if (k<1e-10)  k=1e-10;
  
  //  k=1e-10;
  Reaction->electronOut -= k;
 double k2 = Ep - Reaction->electronOut.energy(); // to minimize roundoff error
 
 if( k2==0 && k2<k) return 0;
 // if( k2==0) continue;

  k=k2;
  double weight = 1;

  theta= Reaction->electronOut.getP()* Reaction->electronIn.getP();
  theta/=Reaction->electronOut.getP().abs()* Reaction->electronIn.getP().abs();
  theta=acos(theta);

   FourVector q = In - Reaction->electronOut;
  
   NucleonFormfactor *FF = new DipoleFit();  

   generateBetheHeitlerPeak BH(In, Reaction->electronOut);

   P_Spin=Momentum();

   bool lowerlimit;
   EBH=k;

   photon = BH.generate(weight,&lowerlimit,&EBH,&helicity,&P_Spin);


//   double w2=1/(t+t2)*k/EBH;//*securePow(k,-2*t2);
//   cout <<k<<"\t"<<EBH<<"\t"<<k/EBH<<endl;
//   FourVector ph_ein=photon.rotate(In);
//   double v=rk;

//   //  double w3=exp(alpha/M_PI*(-0.5*pow(log(E0/Ep),2)+0.5*log(fabs(q*q)/m_e_sqr)-M_PI*M_PI/3.0+Spence(pow(cos(theta/2),2))));


  
//  double k= atan2(photon[1],photon[3])*180/M_PI;


//  double v=k*eta;

  //  weight*=Ep/FF->CrossSection(E0, theta); // um es eins zu machen!

  
   //dsigma/domega

   // 1/eta : m2, k/EBH: m, rest ohne
   double korr=k/EBH;//k/EBH;//(k/EBH)*(eta*k/EBH);//1/eta;//k/EBH;//(k/EBH)*(eta*k/EBH);
   //0.97ish
   
   //   cout<<"kEBH "<<k<<" "<<eta<<" "<<k/EBH<<" "<<korr<<endl;
   //std::cout << "korr = k/EBH = " << korr << std::endl; 
   //
   //
   //   weight=FF->CrossSection(E0,theta);
   //   No need for FF->CrossSection for this version
   //   Is is included in previous step BH.generate->ElasticCrossSection->gNN
   //
   weight*=pow(eta,telectron/2)/t*korr; // /
   
  //  weight=pow(eta,telectron/2)*FF->CrossSection(E0, theta); //t*korr; // /

   if (protonContrib) 
     weight*=pow(4*E0*E0/(-q2*x),tproton1/2)*pow(2*E0/m_proton,tproton2);

//    HMFill(ida,k,0,weight/eta);
//    HMFill(idb,k,0,weight*k/EBH);

//    HMFill(idc,k,0,weight*photon.energy()/EBH*t);
//   HMFill(idd,k,0,1 );
    
//  }
//   HMWriteAll("test.his");
//   exit(-1);
//   return -1;

  if (weight<0){
    invbh++;
    if (invbh %100 ==0)   printf("Warning, Invalid BH.generate %i\n",invbh);
  return 0; // invalid event, very rare case 
  }
  

  // Out1 is now the final proton
  Reaction->Out1 = In + *Reaction->getTarget()-Reaction->electronOut - photon;
  // Set the proton Spin
  //  P_Spin.rot_theta(  Reaction->Out1.theta());
  //  P_Spin.rot_phi(    Reaction->Out1.phi());
  
  Reaction->Out1 = Reaction->Out1.setSpin(P_Spin);



  if (protonContrib) {
    double pVC=protonVertexCorrection(E0,Ep,Eproton,pp,x,rho*rho);
//        std::cout <<E0<<"\t"<<Ep<<"\t"<<Eproton<<"\t"<<pVC<<"\n";
//        if (exp(pVC)>1.1) cout <<"pVC: "<<pVC<<endl;
    //std::cout << pVC << '\t' << exp(pVC) << std::endl;
    weight *= exp(pVC);
  }
  //  double we=weight;
  if (!(weight>=0))
    {
      weight=0;
      cout <<"Error:"<< weight<<endl;//" "<<w2<<endl;//" "<<w3<<endl;
    }
  //return FF->CrossSection(E0, theta);
  //  Reaction->electronOut=eoutsave;

  return weight*dPhotonNorm;
}

///////////////////////////////////////////////////////////////////////////////

double 
generateElasticCollider::generateEvent(double helicity)
{
  generateLabAngles(&Reaction->electronOut,1,sime->getAngle(), sime->getOop(), 
		    dcte,sime->getDphi());
  
  double E0    = Reaction->electronIn.energy();
  double theta = Reaction->electronOut.theta();
  double phi   = Reaction->electronOut.phi();
  FourVector svec = Reaction->electronIn + Reaction->target;
  double Ep = 0.5 * (svec.square() - pow(targetmass,2)) /
    (Reaction->target.energy() + E0
     + cos(theta) * (Reaction->target.momentum()-E0));

  Reaction->electronOut.initPolar(energy(m_electron, Ep), Ep, theta, phi);   
  Reaction->Out1 = Reaction->electronIn - Reaction->electronOut 
    + Reaction->target;

  return 1;
}

///////////////////////////////////////////////////////////////////////////////

double  generateTwoBodyPWIA::generateEvent(double helicity) {
  double 
    dcthe  = sin(sime->getDtheta()), // generation Range cos(theta) electron
    dphie  = sime->getDphi(),        //                  phi        electron
    dpe    = sime->getDmomentum(),   //                  momentum   electron
    dcth1  = sin(sim1->getDtheta()), //                  cos(theta) particle 1
    dphi1  = sim1->getDphi();        //                  phi        particle 1
  
  // generate electron in LAB frame
  double Eprime = sime->getMomentum()- (2*sobol()-1)*dpe;
  generateLabAngles(&Reaction->electronOut, Eprime,
		    sime->getAngle(), sime->getOop(), dcthe, dphie);
  FourVector photon = Reaction->electronIn-Reaction->electronOut;


  // generate particle 1 angles in LAB frame and calculate momentum
  generateLabAngles(&Reaction->Out1, 1, sim1->getAngle(), sim1->getOop(), 
		    dcth1, dphi1);
  double 
    ct = cos((Reaction->Out1.rotate(photon)).theta()),
    D  = targetmass,
    m2 = sqr(Reaction->getM1()->getMass()),
    M2 = sqr(Reaction->getM2()->getMass()),
    w  = photon.energy(),
    q  = photon.momentum(),
    det = (D*D*D*D+m2*m2+4*D*D*D*w-2*D*D*(m2+M2+q*q-3*w*w)+sqr(M2+q*q-w*w)
	   -4*D*w*(m2+M2+ q*q-w*w)-2*m2*(M2+q*q-2*ct*ct*q*q+w*w));

  if (det<0) return 0; // photon energy to low
  double pp, sign = 1;
  // Warning: there should be:   sign = sobol()>0.5 ? -1 : 1;
  // I only know experiments, where the high momentum part is accepted,
  // so I'm saving half of the computation time by ignoring this...
  //
  pp=(ct*q*(m2-M2-q*q+sqr(D+w))+sign*(D+w)*sqrt(det))/(2*(sqr(D+w)-sqr(ct*q)));
  Reaction->Out1.initPolar(sqrt(m2+pp*pp),pp,
			   Reaction->Out1.theta(), Reaction->Out1.phi());
  Reaction->Out2 = photon + *Reaction->getTarget() - Reaction->Out1;

  // Check e.g. with:
  //  cerr<<sqrt(Reaction->Out2*Reaction->Out2)<<endl;
 
  return
    4 * dcth1 * dphi1 *     // generation volume particle 1
    8 * dcthe * dphie * dpe // generation volume electron
    * (pwia ? pwia->CrossSection(Reaction->electronIn,
				 Reaction->electronOut,
				 FourVector(targetmass),
				 Reaction->Out1):1);
}



///////////////////////////////////////////////////////////////////////////////

// ramdom isotropic in six dimesions:
// - electron momentum and solid angle
// - proton momentum and solid angle.

double
generateIsotropic6D::generateEvent(double helicity) {
  double 
    dcthe  = sin(sime->getDtheta()),  // generation Range cos(theta) electron
    dphie  = sime->getDphi(),         //                  phi        electron
    dpe    = sime->getDmomentum(),    //                  momentum   electron
    dcth1  = sin(sim1->getDtheta()),  //                  cos(theta) particle 1
    dphi1  = sim1->getDphi(),         //                  phi        particle 1
    dpp    = sim1->getDmomentum();    //                  momentum   particle 1

  // generate scattered electron momentum and angles in lab frame
  generateLabAngles(&Reaction->electronOut,
		    sime->getMomentum() - (2*sobol() - 1)*dpe,
		    sime->getAngle(),
		    sime->getOop(),
		    dcthe, dphie);

  // generate proton momentum and angles in lab frame.
  generateLabAngles(&Reaction->Out1,
		    sim1->getMomentum() - (2*sobol() - 1)*dpp,
		    sim1->getAngle(),
		    sim1->getOop(),
		    dcth1, dphi1);

  // return
  //     8.0 * dcth1 * dphi1 * dpp    // generation volume particle 1
  //   * 8.0 * dcthe * dphie * dpe    // generation volume electron
  //   * 1.0;                         // cross section

  return 1.0;
}

///////////////////////////////////////////////////////////////////////////////

// 'generateDeuteronBreakup::generateEvent'
// - random generation of electron momentum and solid angle and proton
//   solid angle.
// - Calculation of proton energy and momentum, with possible ambiguity
//   treatment.
// - Calculation of Arenhoevel-D(e,e'p)n cross section

double
generateDeuteronBreakup::generateEvent(double helicity) {
  double 
    dcthe  = sin(sime->getDtheta()),  // generation Range cos(theta) electron
    dphie  = sime->getDphi(),         //                  phi        electron
    dpe    = sime->getDmomentum(),    //                  momentum   electron
    dcth1  = sin(sim1->getDtheta()),  //                  cos(theta) particle 1
    dphi1  = sim1->getDphi();         //                  phi        particle 1


  // Remarks about the ambiguity treatment:
  // Here in 'generateEvent', we generate random k_f, cos(th_e), ph_e,
  // cos(th_p), ph_p.  From these, the proton energy is calculated (since
  // the residuals mass is known). Here it's possible to get two proton
  // energies (quadratic equation in e_p). Both solutions are valid and
  // contribute if they do pass the cuts.
  // If we encounter a kinematical setting in k_f, cos(th_e), ph_e, cos(th_p),
  // ph_p that yields two e_p solutions, we pass this setting with e_p_1. In
  // the next call to 'generateEvent' we pass the same setting with e_p_2.
  //
  // For a basic ambiguity example, see '~/e-doc/ambiguity.tex'.

  // ============== ?????????????!!!!!!!!!!!!!!!!! ===============
  // Remaining problem: The simulated event counter *must not* be increased
  // for such an ambiguous event! ---> Harald.
  // ============== ?????????????!!!!!!!!!!!!!!!!! ===============


  // data to remember for the next call
  static int ep_resultcode = 0;                   // ep-calculation resultcode
  static reaction previous_reaction(*Reaction);   // reaction

  if( ep_resultcode != 3 ) {
    // The previous call of 'generateEvent' generated an event with only one
    // e_p solution. Therefore we can generate a new event now.

    // generate scattered electron momentum and angles in lab frame
    double k_f = sime->getMomentum() - (2*sobol() - 1)*dpe;
    generateLabAngles(&Reaction->electronOut, k_f,
		      sime->getAngle(), sime->getOop(), dcthe, dphie);

    // generate proton angles in lab frame. The proton momentum is set to 1
    // now. Its correct value is calculated later, with possible ambiguity.
    generateLabAngles(&Reaction->Out1, 1.0,
		      sim1->getAngle(), sim1->getOop(), dcth1, dphi1);
  }
  else
    // The previous call of 'generateEvent' generated an event with two e_p
    // solutions, so we take that old event again and complete it with the
    // second e_p solution (later in the code)
    *Reaction = previous_reaction;


  // Calculate various kinematical quantities. Beware: All those quantities
  // refer to the actual scattering process and are in general slightly
  // different from the measured quantities due to energy losses and small
  // angle straggling! Especially, the incident electron in
  // 'Reaction->electronIn' has already been subjected to those and therefore
  //   Reaction->electronIn != (e_beam, 0, 0, e_beam).
  // The variable names of said quantities don't indicate that small
  // difference right now, e.g.
  //   cos_th_e != cos(Reaction->electronOut.theta())!
  // ... to be named better, sometimes...

  FourVector photon = Reaction->electronIn - Reaction->electronOut;

  double 
    m_a = targetmass,
    m_p = Reaction->getM1()->getMass(),
    m_r = Reaction->getM2()->getMass(),

    k_i   =  Reaction->electronIn.energy(),
    omega =  photon.energy(),
    qnu2  = -photon.square(),      // 'Q^2'
    q2    =  qnu2 + omega*omega,   // 3-momentum transfer, squared
    q     =  sqrt(q2);             // 3-momentum transfer


  // ------- ???????????!!!!!!!!!!! ----------
  // The goal from here on are the angles th_e, th_pq and ph_pq.
  // Haralds suggestions for the latter two:
  //   double th_pq = Reaction->Out1.rotate(photon)).theta();
  //   double ph_pq = Reaction->Out1.rotate(photon).phi();
  // th_pq looks ok, but ph_pq doesn't! See also ~/Fourvector-test/
  // Therefore, we do it the hard way ...

  Momentum
    K_i = Reaction->electronIn.getP(),
    K_f = Reaction->electronOut.getP(),
    Q   = photon.getP(),
    P_p = Reaction->Out1.getP();

  // calculate scattering angle and angle between photon and proton
  double
    cos_th_e  = (K_i * K_f) / (K_i.abs() * K_f.abs()),
    cos_th_pq = (Q * P_p) / (Q.abs() * P_p.abs());

  // get a vector perpendicular to the scatter plane (fix mult)
  Momentum N_scat( Q[1]*K_i[2] - Q[2]*K_i[1],
		   Q[2]*K_i[0] - Q[0]*K_i[2],
		   Q[0]*K_i[1] - Q[1]*K_i[0] );

  // Get orthonormal base vectors with z-axis parallel to q, y axis
  // perpedicular to scatter plane, x-axis in scatter plane
  Momentum
    Ez( Q / Q.abs() ),
    Ey( N_scat / N_scat.abs() ),
    Ex( Ey[1]*Ez[2] - Ey[2]*Ez[1],
	Ey[2]*Ez[0] - Ey[0]*Ez[2],
	Ey[0]*Ez[1] - Ey[1]*Ez[0] );

  // get proton momentum components with respect to above coordinate system
  double p_x = P_p*Ex, p_y = P_p*Ey;      // p_z = P_p*Ez, for completeness

  // angle between scatter plane and reaction plane
  double ph_pq = atan2(p_y, p_x);


  // Now find proton energy/momentum. Depending on the e_p-solution(s) of the
  // previous call to generateEvent, either calculate them or reuse old ones
  double e_lab = omega + m_a;
  double e_p, p_p;
  static double e_cm, e_p_cm, p_p_cm, e_p_2, p_p_2;     // data to remember

  if( ep_resultcode != 3 ) {
    // The previous call of 'generateEvent' generated an event with only one
    // e_p solution, therefore we generated a new event and now have to
    // calculate its proton energy/momentum.
    ep_resultcode = find_c(e_lab, q, cos_th_pq, m_p, m_r,
			   e_cm, e_p_cm, p_p_cm,
			   e_p, p_p, e_p_2, p_p_2);

    if( ep_resultcode < 0 )   // no solution, reaction kinematically impossible
      return(0);

    if( ep_resultcode == 2 ) { // one solution, but values in 'e_p_2', 'p_p_2'
      e_p = e_p_2;
      p_p = p_p_2;
    }

    if( ep_resultcode == 3 )   // Two Solutions! Store Reaction for next call
      previous_reaction = *Reaction;
  }
  else {
    // The previous call of 'generateEvent' generated an event with two e_p
    // solutions, therefore we took the old values in k_f, th_e, ph_e, th_p,
    // ph_p and complete that event now with the second e_p solution.

    e_p = e_p_2;
    p_p = p_p_2;

    ep_resultcode = 0;
  }

  // proton kinematically fixed from here on, now complete the reaction
  Reaction->Out1.initPolar(e_p, p_p,
			   Reaction->Out1.theta(), Reaction->Out1.phi());
  Reaction->Out2 = photon + *Reaction->getTarget() - Reaction->Out1;


  // total kinetic energy and momentum transfer in cm frame
  double t_cm = e_cm - m_p - m_r;
  double q_cm = m_a * q / e_cm;

  // lorentz parameters for transformation from lab- to cm-frame
  double beta  = -q / e_lab;
  double gamma = e_lab / e_cm;

  // momentum of residual ('missing momentum')
  //  double p_r = sqrt(q2 + p_p*p_p - 2.0*q*p_p*cos_th_pq);

  // proton momentum components parallel and perpendicular to q
  double sin_th_pq = sqrt(1 - cos_th_pq*cos_th_pq);
  double p_p_long  = p_p * cos_th_pq;
  double p_p_trans = p_p * sin_th_pq;

  // proton momentum component parallel to q in cm-frame
  double p_p_cm_long = beta*gamma*e_p + gamma*p_p_long;

  // angle between p and q in cm-frame
  double th_pq_cm = atan2(p_p_trans, p_p_cm_long);

  // jacobian lab <-> cm:
  // jac_lab_cm = d(cos_th_pq) / d(cos_th_pq_cm) = dOmega / dOmega_cm
  double jac_lab_cm = fabs(  pow(p_p_cm/p_p, 3) * gamma
			     * (1.0 - beta*cos(th_pq_cm)*e_p_cm/p_p_cm) );

  // calculate mott cross section and D(e,e'p)n structure functions.
  // ('sigaren' wants MeV instead of GeV, therefore these factors 1e3, 1e6).
  static int initflag = 1;
  double sig_mott, v_l, v_t, v_lt, v_tt, w_l, w_t, w_lt, w_tt;
  sigaren(initflag, k_i*1.e3, omega*1.e3, cos_th_e,
	  q2*1.e6, qnu2*1.e6, 1.0/jac_lab_cm,
	  th_pq_cm, ph_pq, t_cm*1.e3,
	  q_cm*q_cm*1.e6, 0, 'p',
	  sig_mott, v_l, v_t, v_lt, v_tt,
	  w_l, w_t, w_lt, w_tt);

  // calculate D(e,e'p)n cross section
  double sig_deep = sig_mott * (  v_l*w_l + v_t*w_t + v_lt*w_lt*cos(ph_pq)
				  + v_tt*w_tt*cos(2*ph_pq) );

#ifdef NEVER
  const double deg2rad = M_PI/180.0;
  cout << "RB: th_e = " << acos(cos_th_e)/deg2rad
       << ", th_pq = " << acos(cos_th_pq)/deg2rad
       << ", ph_pq = " << ph_pq/deg2rad
       << ",\nep_resultcode = " << ep_resultcode
       << ", e_p = " << e_p
       << ", p_p = " << p_p
       << ", p_r = " << p_r
       << "\nsig_deep = " << sig_deep*1.e12
       << "\n";
#endif

  // now we need Harald again: what do we return? what do we do with it?
  // temporarily, we do as in 'generateTwoBodyPWIA'
  return
    4.0 * dcth1 * dphi1 *          // generation volume particle 1
    8.0 * dcthe * dphie * dpe      // generation volume electron
    * sig_deep;                    // cross section
}


///////////////////////////////////////////////////////////////////////////////

double generateThreeBodyPWIA::generateEvent(double helicity) {
  double 
    dcthe  = sin(sime->getDtheta()), // generation Range cos(theta) electron
    dphie  = sime->getDphi(),        //                  phi        electron
    dpe    = sime->getDmomentum(),   //                  momentum   electron
    dcth1  = sin(sim1->getDtheta()), //                  cos(theta) particle 1
    dphi1  = sim1->getDphi(),        //                  phi        particle 1
    dp1    = sim1->getDmomentum(),   //                  momentum   particle 
    E0 = Reaction->electronIn.energy(),
    m  = Reaction->getM1()->getMass(),
    m2 = sqr(Reaction->getM1()->getMass()),
    maxE1  = sqrt(m2 + sqr(sim1->getMomentum() + dp1)),
    minE1  = sqrt(m2 + sqr(sim1->getMomentum() - dp1));
    
  // generate electron in LAB frame
  double Eprime = sime->getMomentum()- (2*sobol()-1)*dpe;
  generateLabAngles(&Reaction->electronOut, Eprime,
		    sime->getAngle(), sime->getOop(), dcthe, dphie);

  // generate particle 1 in LAB frame
  if (maxE1 > E0 - Eprime + m) maxE1 = E0 - Eprime + m;
  if (maxE1 < minE1) return 0;
  double E1 = minE1 + sobol()*(maxE1 - minE1);
  generateLabAngles(&Reaction->Out1, sqrt(E1*E1 - m2), 
		    sim1->getAngle(), sim1->getOop(), dcth1, dphi1);
  return
    4 * dcth1 * dphi1 * (maxE1-minE1) // generation volume particle 1
    * 8 * dcthe * dphie * dpe        // generation volume electron
    * (pwia ? pwia->CrossSection(Reaction->electronIn, Reaction->electronOut,
				 FourVector(targetmass), Reaction->Out1):1);
}
///////////////////////////////////////////////////////////////////////////////
double VertexQ2=0;
double ThetaGammaHardCMS=0;
double PhiGammaHardCMS=0;
double RadiativeCorrection=0;
 
Tensor gNN(const FourVector& q, int mu) 
{
  //static const NucleonFormfactor *ff= new DipoleFit();
 
  static NucleonFormfactor *ff = NULL;
  if (ff==NULL){
    if (getenv("SPLINEFIT") && atoi(getenv("SPLINEFIT"))==1){
      std::cout<<"Using Bernauer spline fit!"<<endl;
      ff= new BernauerSplineFit();   
    }
    else{
      ff= new DipoleFit();
    }
  }
  
  double Q2 = -q.square();
  double v  = Q2 / (4*m_p_sqr);
  double GE = ff->G_E(-Q2);
  double GM = ff->G_M(-Q2);
  double F1_p = (v*GM + GE) / (1+v);
  Complex iF2_p(0, (GM - GE) / (1+v) / 2/m_proton);
  
  Tensor pauli= sigma[mu][0] * (iF2_p*  q[0]);
  pauli += sigma[mu][1]      * (iF2_p* -q[1]);
  pauli += sigma[mu][2]      * (iF2_p* -q[2]);
  pauli += sigma[mu][3]      * (iF2_p* -q[3]);
  pauli += F1_p * gam[mu];
  return   pauli;
}

FourVector phcm;
double ElasticCrossSection(const FourVector &e_in, 
			   const FourVector &e_out,
			   const FourVector &q_outus,
			   const double helicity,
			   const unsigned short a,
			   const unsigned short b,
			   class Momentum *P_Spin,
			   class Momentum *P_Spin_CM,
			   double *Msqr,
			   double *k,
			   double kp)
{
  FourVector q_out=(*k>-1)?q_outus * *k:q_outus;

  if (q_out.energy() == 0) {
    cout << "WARNING: Cross section for q_out=0 is infinity"<< endl;
    return 0.0;
  }

  FourVector p_in    = FourVector(m_proton, 0, 0, 0);  
  FourVector CM      = p_in + e_in - e_out;
  FourVector e_inCM  = e_in.Lorentz(-CM).rotate(CM);
  FourVector e_outCM = e_out.Lorentz(-CM).rotate(CM);
  FourVector q_outCM = q_out.Lorentz(-CM).rotate(CM);
  kp=*k;
  *k=q_outCM.momentum();
  FourVector q_outCMus = q_outCM/ *k;
  FourVector p_inCM  = p_in.Lorentz(-CM).rotate(CM);

  FourVector p_outCM = e_inCM - e_outCM + p_inCM - q_outCM;
  FourVector q_p     = e_inCM - e_outCM;
  FourVector q       = p_outCM - p_inCM;
  
  phcm=CM;
  const double eps=1e-7;
  
  FourVector qs=q_out/ q_out.momentum();
  FourVector qh1,qh2;

  qh1.initPolar(1,1,eps,0);
  qh2.initPolar(1,1,eps,M_PI/2);

  qh1=qh1.rotateTo(qs);
  qh2=qh2.rotateTo(qs);

  VertexQ2 = -q.square();
  ThetaGammaHardCMS=q_outCM.theta();
  PhiGammaHardCMS=q_outCM.phi();  

  FourVector qh1cm = qh1.Lorentz(-CM).rotate(CM);
  FourVector qh2cm = qh2.Lorentz(-CM).rotate(CM);
  FourVector qsCM = qs.Lorentz(-CM).rotate(CM);

  Momentum dummy;
  qh1cm/=qh1cm.momentum();
  qh2cm/=qh2cm.momentum();
  qsCM/=qsCM.momentum();
  qh1/=qh1.momentum();
  qh2/=qh2.momentum();
  qs/=qs.momentum();

  Momentum da=(Momentum)(qh1cm - qsCM).getP();
  Momentum db=(Momentum)( qh2cm - qsCM).getP();
  Momentum dc=(Momentum)(qh1 - qs).getP();
  Momentum dd=(Momentum)( qh2 - qs).getP();
  double jacobian = (dummy.mult(da,db).abs()/dummy.mult(dc,dd).abs());///qsCM.momentum();//  *pow(q_out.momentum()/q_outCM.momentum(),1);
  const double c = mubarn/64./pow(2*M_PI, 5)/m_proton; 
  double q2    = q.square();           // virtual photon q^2 
  double q2_p  = q_p.square(); 
  Complex i_e3_q2   =  i_e3 / q2;      // electron charge -
  Complex i_e3_q2_p = -i_e3 / q2_p;    // proton charge   +

  double s = CM.square();
  double recoil = e_out.momentum()/e_in.momentum();
  double sred=(s-m_proton*m_proton)/ *k; // give one of the missing photon momenta back *k
  double mubarn_GeV_srsqr = c*sred/s*recoil;

  FourVector ne1 = e_inCM  - q_outCM;                 // internal electron line
  FourVector ne2 = e_outCM + q_outCM;                 // internal electron line
//   Tensor t1 = (dag(ne1) + eID) / (ne1*ne1 - m_e_sqr); // Propagator e\g___g*/e'
//   Tensor t2 = (dag(ne2) + eID) / (ne2*ne2 - m_e_sqr); // Propagator e\g*___g/e'
  Tensor t1 = (dag(ne1) + eID) / (-2*e_inCM*q_outCMus); // Propagator e\g___g*/e'
  Tensor t2 = (dag(ne2) + eID) / (+2*e_outCM*q_outCMus); // Propagator e\g*___g/e'
  // hier wird ein k rausgezogen!
  

  FourVector np1 = p_inCM  - q_outCM;                 // internal proton line
  FourVector np2 = p_outCM + q_outCM;                 // internal proton line
//   Tensor t1p = (dag(np1) + pID) / (np1*np1 - m_p_sqr);// Propagator p\g___g*/p'
//   Tensor t2p = (dag(np2) + pID) / (np2*np2 - m_p_sqr);// Propagator p\g*___g/p'
  Tensor t1p = (dag(np1) + pID) / (-2*p_inCM*q_outCMus);// Propagator p\g___g*/p'
  Tensor t2p = (dag(np2) + pID) / (+2*p_outCM*q_outCMus);// Propagator p\g*___g/p'
  // hier wird auch ein k rausgezogen!

  //  cout << np1*np1-m_p_sqr<<" "<<-2*p_inCM*q_outCM<<endl;

  double   M_square = 0, Mn=0;
  double M_square_e=0, M_square_p=0;
  Complex  M_mu_ud[4][2];
  Momentum Spup(0,0,0), Spdown(0,0,0);

  double thSp = atan2(e_inCM[1], e_inCM[3]); // wigner rotation to e direction
  Spinor e_in_z_u(e_inCM,  0.5),   e_in_z_d(e_inCM, -0.5);

  Spinor ei[2] = { e_in_z_u * cos(thSp/2) + e_in_z_d * sin(thSp/2),
		   e_in_z_u *-sin(thSp/2) + e_in_z_d * cos(thSp/2)};

  Spinor eo[2] = {Spinor(e_outCM, 0.5), Spinor(e_outCM, -0.5)};
  Spinor po[2] = {Spinor(p_outCM, 0.5), Spinor(p_outCM, -0.5)};
  Spinor pi[2] = {Spinor(p_inCM,  0.5), Spinor(p_inCM,  -0.5)};
  
  int spin_e_in = (helicity > 0 ? 0 : 1);

  Tensor ee2gmunu[16];
  Tensor gNNnu[4];
  Tensor NN2gmunu[16];


  for(int mu=0; mu<4; mu++) {
    gNNnu[mu]=gNN(q,mu);
    for(int nu=0; nu<4; nu++) {
      ee2gmunu[nu+(mu<<2)]=gam[nu]*t1*gam[mu] + gam[mu]*t2*gam[nu];
      Tensor gNNnu = gNN(     q_p  , nu);
      Tensor gNNmu = gNN((-1)*q_outCM, mu); 
      NN2gmunu[nu+(mu<<2)] = gNNnu * t1p * gNNmu + gNNmu * t2p * gNNnu;
    }
 }

  for(int spin_p_in  = 0; spin_p_in  <= 1; spin_p_in++ )
    for(int spin_e_out = 0; spin_e_out <= 1; spin_e_out++) {
      for(int ix=0; ix<4; ix++) M_mu_ud[ix][0] = M_mu_ud[ix][1] = 0;
      for(int spin_p_out = 0; spin_p_out <= 1; spin_p_out++) {
	for(int mu=0; mu<4; mu++) {
	  Complex M_if = 0;
	  Complex M_if_p = 0;  // matrix element for radiation from proton 
	  
	  for (int nu=0; nu<4; nu++) {
	    Tensor ee2g = ee2gmunu[nu+(mu<<2)];
	    Complex el_part = !eo[spin_e_out] * ee2g      * ei[spin_e_in];
	    Complex nu_part = !po[spin_p_out] * gNNnu[nu] * pi[spin_p_in];
	    M_if += el_part * gmn[nu] * nu_part;   // Electron current
	    
	    if (a==0) { //Proton Contribution
	      nu_part = !po[spin_p_out] * NN2gmunu[nu+(mu<<2)]  * pi[spin_p_in];
	      el_part = !eo[spin_e_out] * gam[nu] * ei[spin_e_in];
	      M_if_p += el_part * gmn[nu] * nu_part;
	    }
	  }
	  
	  M_if  *=  i_e3_q2;    
	  M_if_p *=  i_e3_q2_p;
	  M_mu_ud[mu][spin_p_out] =  M_if + M_if_p;  

	  M_square += -norm(M_if + M_if_p) * re_gmn[mu];
	  M_square_e += -norm(M_if)*re_gmn[mu];
	  M_square_p+= -norm(M_if_p)*re_gmn[mu];
	}
      }
      
      for (int mu = 0; mu<4; mu++){
	Spup   += Momentum(norm(M_mu_ud[mu][0] -     M_mu_ud[mu][1])/2,
			   norm(M_mu_ud[mu][0] + i * M_mu_ud[mu][1])/2,
			   norm(M_mu_ud[mu][0])) * re_gmn[mu]/4;
	Spdown += Momentum(norm(M_mu_ud[mu][0] +     M_mu_ud[mu][1])/2,
			   norm(M_mu_ud[mu][0] - i * M_mu_ud[mu][1])/2,
			   norm(M_mu_ud[mu][1])) * re_gmn[mu]/4; 
	Mn += (norm(M_mu_ud[mu][0]) + norm(M_mu_ud[mu][1]))* re_gmn[mu]/4;
      }     
    }
    
  if (Msqr)  *Msqr = M_square;
  
  if (b==0) *P_Spin = (Spup-Spdown)/Mn; //BH+B Polarizations


  else                                  //VCS Polarizations
    {
      ChPT_VCS VCS;
 
      double pol = epsilon(e_inCM,e_outCM);  //Polarization

      double thgg = atan2(-q_outCM[1], q_outCM[3]); //Theta gammagamma
      double phigg = asin( q_outCM[2] / q_outCM.momentum() );//Phi gammagamma

      double M0 = VCS.M0(q_outCM,p_inCM,e_inCM,e_outCM,q_p,thgg,phigg,pol);
      double Mx = VCS.Mx(q_outCM,p_inCM,e_inCM,e_outCM,q_p,thgg,phigg,pol,helicity);
      double My = VCS.My(q_outCM,p_inCM,e_inCM,e_outCM,q_p,thgg,phigg,pol,helicity);
      double Mz = VCS.Mz(q_outCM,p_inCM,e_inCM,e_outCM,q_p,thgg,phigg,pol,helicity);

      //cout<< M0 <<" "<< Mx <<" "<< My <<" "<< Mz << " " << thgg*180/M_PI << endl;

      //FourVector qp(0,0,0,0.6);
      //cout << "Momentum q = " << qp.momentum()*1e3 << endl;
      //cout << "Structure Functions:" << endl;
      //cout << "P LL   = " << VCS.P_LL(qp) << endl;
      //cout << "P TT   = " << VCS.P_TT(qp) <<endl;
      //cout << "P LT   = " << VCS.P_LT(qp) <<endl;
      //cout << "P LTz  = " << VCS.P_LTz(qp) <<endl;
      //cout << "P LTz1 = " << VCS.P_LTz1(qp) <<endl;
      //cout << "P LTo  = " << VCS.P_LTo(qp) << endl;
      //cout << "P TTo  = " << VCS.P_TTo(qp) << endl;
      //cout << "P TTo1  = " << VCS.P_TTo1(qp) << endl;
      //cout << "P LTo1  = " << VCS.P_LTo1(qp) << endl;
      //cout << endl;
      //cout << "Generalized Polarizabilities:" << endl;
      //cout << "P(01,01)0=" << VCS.P01010(qp)*1e3
      //	   << "  P(11,11)0=" << VCS.P11110(qp)*1e3
      // 	   << "  P(11,11)1=" << VCS.P11111(qp)*1e3
      //	   << "  P(01,01)1=" << VCS.P01011(qp)*1e3
      //	   << "  P(01,12)1=" << VCS.P01121(qp)*1e3 
      //	   << "  P(11,02)1=" << VCS.P11021(qp)*1e3 << endl;

      //VCS Effect on the Polarizations
      double Px = ( ((Spup-Spdown)[0]/Mn)*M_square + Mx ) / ( M_square + 2*M0);
      double Py = ( ((Spup-Spdown)[1]/Mn)*M_square + My ) / ( M_square + 2*M0); 
      double Pz = ( ((Spup-Spdown)[2]/Mn)*M_square + Mz ) / ( M_square + 2*M0);
      
      //VCS.Coefficients(q_outCM,p_inCM,e_inCM,e_outCM,q_p,thgg,0,pol); 
      //VCS.Coefficients(q_outCM,p_inCM,e_inCM,e_outCM,q_p,thgg,20*M_PI/180.,pol); 
      //VCS.Coefficients(q_outCM,p_inCM,e_inCM,e_outCM,q_p,thgg,-20*M_PI/180.,pol); 

      *P_Spin=Momentum(Px,Py,Pz);
    }
 
  //Test
  // *P_Spin = Momentum(-0.5,0,0.5)*helicity;

  // Sign convention:
  // e_y =  e_in x e_out  / | e_in x e_out| 
  // e_z = (e_in - e_out) / | e_in - e_out|
  // e_x defined by e_x x e_y = e_z 
  // this leads to ex = (-1, 0, 0) ey = (0, -1, 0) for B on the right side!
   
  if (e_inCM[3] * e_outCM[1] - e_inCM[1] * e_outCM[3] < 0) 
    *P_Spin = Momentum(-(*P_Spin)[0], -(*P_Spin)[1], (*P_Spin)[2]);

  if (P_Spin_CM) *P_Spin_CM = *P_Spin;

  
  //cout << Px << " " << Py << " " <<  Pz << endl; 
  
  //This is for simulation only (Polarizations in LAB frame)
  //---------------------------------------
  Particle proton = P_proton;
  proton = p_outCM; 
  proton.setSpin(*P_Spin);
  proton = proton.rotateTo(CM);
  Particle lab = proton.Lorentz(CM);
  *P_Spin = lab.getSpin();
  //----------------------------------------
  

  //  double cs=mubarn_GeV_srsqr*M_square/2*jacobian;

  //  cout <<mubarn_GeV_srsqr*M_square/2*jacobian<<"\t"<<mubarn_GeV_srsqr*M_square_e/2*jacobian<<"\t"<<mubarn_GeV_srsqr*M_square_p/2*jacobian<<"\t"<<M_square/M_square_e<<endl;

  //BH+B CrossSection
  if (b==0) return mubarn_GeV_srsqr * M_square/2*jacobian;

  //GP Effects on CrossSection
  else{
    ChPT_VCS VCS;
    double eps = epsilon(e_inCM,e_outCM); 
    double thetagg = atan2(-q_outCM[1], q_outCM[3]);
    double phigg = asin( q_outCM[2] / q_outCM.momentum() );
    double Psi = VCS.M0(q_outCM,p_inCM,e_inCM,e_outCM,q,thetagg,phigg,eps);
    return mubarn_GeV_srsqr * (M_square / 2 /  *k + Psi);
  }
}

double DMBosonCrossSection(const FourVector &e_in,  const FourVector &e_out,
			   const FourVector &q_out, const double mA)
{
  FourVector p_in    = FourVector(m_proton, 0, 0, 0);  
  FourVector CM      = p_in + e_in - e_out;
  FourVector e_inCM  = e_in.Lorentz(-CM).rotate(CM);
  FourVector e_outCM = e_out.Lorentz(-CM).rotate(CM);
  FourVector q_outCM = q_out.Lorentz(-CM).rotate(CM);
  FourVector p_inCM  = p_in.Lorentz(-CM).rotate(CM);
  FourVector p_outCM = e_inCM - e_outCM + p_inCM - q_outCM;
  //FourVector q_p     = e_inCM - e_outCM;
  FourVector q       = p_outCM - p_inCM;
  FourVector ne1     = e_inCM  - q_outCM;             // internal electron line
  FourVector ne2     = e_outCM + q_outCM;             // internal electron line

  double q2          = q.square();                    // virtual photon q^2 
  Complex i_e3_q2    =  i_e3 / q2;                    // electron charge -
  double M_square = 0;

  Spinor ei[2] = {Spinor(e_inCM,  0.5), Spinor(e_inCM,  -0.5)};
  Spinor eo[2] = {Spinor(e_outCM, 0.5), Spinor(e_outCM, -0.5)};
  Spinor po[2] = {Spinor(p_outCM, 0.5), Spinor(p_outCM, -0.5)};
  Spinor pi[2] = {Spinor(p_inCM,  0.5), Spinor(p_inCM,  -0.5)};
 
  Tensor t1 = (dag(ne1) + eID) / (ne1*ne1 - m_electron*m_electron);
  Tensor t2 = (dag(ne2) + eID) / (ne2*ne2 - m_electron*m_electron);
  Tensor ee2gmunu[16], gNNnu[4];

  for(int mu=0; mu<4; mu++) {
    gNNnu[mu]=gNN(q,mu);              // Proton Vertex Gamma_mu
    for(int nu=0; nu<4; nu++)
      ee2gmunu[nu+(mu<<2)]=gam[nu]*t1*gam[mu] + gam[mu]*t2*gam[nu];
  }

  for(int spin_e_in  = 0; spin_e_in  <= 1; spin_e_in++ )
    for(int spin_p_in  = 0; spin_p_in  <= 1; spin_p_in++ )
      for(int spin_e_out = 0; spin_e_out <= 1; spin_e_out++) {
	for(int spin_p_out = 0; spin_p_out <= 1; spin_p_out++) {
	  Complex M_if[4] = {0,0,0,0};
	  for(int mu=0; mu<4; mu++) 
	    for (int nu=0; nu<4; nu++) {
	      Tensor ee2g = ee2gmunu[nu+(mu<<2)];
	      Complex el_part = !eo[spin_e_out] * ee2g      * ei[spin_e_in];
	      Complex nu_part = !po[spin_p_out] * gNNnu[nu] * pi[spin_p_in];
	      M_if[mu] += i_e3_q2 * gmn[nu] * el_part * nu_part;
	    }
	  
	  if (mA==0)
	    for (int mu=0;mu<4;mu++) 
	      M_square += norm(M_if[mu])*(-re_gmn[mu]);
	  else
	    for(int mu=0; mu<4; mu++) 
	      for (int nu=0; nu<4; nu++) 
		M_square += real(conj(M_if[mu])*M_if[nu]
				 *(-g_mu_nu[mu][nu]+gmn[mu]*gmn[nu]*q_outCM[mu]
				   *q_outCM[nu]/mA/mA));
	  
	}    
      }
  
  const double c = mubarn/64/pow(2*M_PI, 5)/m_proton; 
  double s = CM.square();
  return c*(s-m_proton*m_proton)/s*e_out.momentum()/e_in.momentum()*M_square/4;
}

double generateBetheHeitlerPeak::BetheHeitlerCrossSection(FourVector in,
 							  FourVector out,
  	 						  FourVector k) {
  Momentum k_mom = (Momentum) k;
  double ct1 = ((Momentum) in * k_mom)/ in.momentum()/k.momentum();
  double ct2 = ((Momentum)out * k_mom)/out.momentum()/k.momentum();
  double a1 =  in.energy()/ in.momentum();
  double a2 = out.energy()/out.momentum();
  double norm1 = -4-2*a1*log((a1-1)/(a1+1));
  double norm2 = -4-2*a2*log((a2-1)/(a2+1));
  double cs1 = (1-ct1*ct1)/(a1-ct1)/(a1-ct1)/norm1;
  double cs2 = (1-ct2*ct2)/(a2-ct2)/(a2-ct2)/norm2;
  return (0.5*cs1+0.5*cs2)/2/M_PI; 
}

generateBetheHeitlerPeak::generateBetheHeitlerPeak(FourVector ein,
						   FourVector eout) {
  in = ein;
  out = eout;
  a1 = ein .energy()/ein .momentum(); //y+1
  a2 = eout.energy()/eout.momentum(); //y+1
  norm1 = -4-2*a1*log((a1-1)/(a1+1)); // -4 -2a log(2/y+1)
  norm2 = -4-2*a2*log((a2-1)/(a2+1));
}

class FourVector 
generateBetheHeitlerPeak::generate(double &weight,bool *lowerlimit, double *EBH,double *helicity, 
				   class Momentum *P_Spin) 
{
  double kp=*EBH;
  FourVector photon = generate();
  *P_Spin = Momentum();;
  FourVector q = in - out;
  double qmom = q.momentum();
  double skp = (Momentum) photon * (Momentum) q;
  
  // photon momentum calculation to obtain the final proton mass 
  double k= (-2*m_proton*q[0]+qmom*qmom-q[0]*q[0])/2/(-m_proton-q[0]+skp);
  if (k<0) 
      k=1e-15;
  *EBH=k;
  *lowerlimit=false;

  weight = ElasticCrossSection(in, out, photon ,*helicity, protonContrib? 0:1, 0, P_Spin,NULL,NULL,EBH,kp);

  // 0 means to add proton contributions

  double fweight=0; 
   a = a1; Norm = norm1;
   fweight += f((Momentum)in  * (Momentum) photon/in.momentum())/2;
   a = a2; Norm = norm2;
   fweight += f((Momentum)out * (Momentum) photon/out.momentum())/2;
   weight *= 2 * M_PI / fweight; //=4pi/ (f(in,k)+f(out,k)) is equivalent to kweight in Olympis
   return photon*k; //photon is normed to 1; the multiplication with k gives it the wanted value
}

FourVector generateBetheHeitlerPeak::generate() {
  FourVector photon;
  if (prndm()>0.5) {
    a = a1;
    Norm = norm1;
    photon.initPolar(1, 1, acos(ct = transform(prndm())), prndm()*2*M_PI);
    ct2=ct;
    return rotateTo(photon, in);
  }
  a = a2;
  Norm = norm2;
  photon.initPolar(1, 1, acos(ct = transform(prndm())), prndm()*2*M_PI);
  ct2=ct;
  return rotateTo(photon, out);
}


double protonVertexCorrection(double Ein, double Eout, double Eproton)
{
  // 33musec on a Pentium III with 500MHz for this function!

  const double Z = 1;
  double (*s)(double) = Spence;  
  double e1=Ein/m_proton, e3= Eout/m_proton, e4=Eproton/m_proton, 
    c1=2*e3*e4-e1, c2=2*e1*e4-e3, eta = e1/e3;
  double beta4 = sqrt(1 - 1/e4/e4);
  double bb = sqrt((1  + beta4) / (1  - beta4));
  double ee = sqrt((e4 -     1) / (e4 +     1));

  double vertexZ =
    -6*log(eta)*log(eta) +
    (s((e3-1)/e1) - s((1-e3)/c1) + s(2*e3*(1-e3)/c1) 
     - log(fabs(c1/(e1*(1-2*e3))))*log(2*e3) ) -
    (s(1-e4/e3) - s((e4-e3)/c2) + s(2*e1*(e4-e3)/c2) 
     - log(fabs(c2/(e3*(1-2*e1))))*log(2*e1) ) -
    (s(1-1/e1)-s(1/e1-1)+s(2-2*e1)+log(fabs(2*e1-2))*log(2*e1))+
    (s(1-1/e3)-s(1/e3-1)+s(2-2*e3)+log(fabs(2*e3-2))*log(2*e3));
  
  double vertexZ2 =
    - log(e4) - log(eta/e1)*(2*log(bb)/beta4-2)
    + (log(bb)*log((e4+1)/2)-s(-ee*bb)+ s(ee/bb)-s(ee)+s(-ee))/beta4;

  // double internZ  = -4*log(eta)                         * log(k/E1);
  // double internZ2 = -(log((1+beta4)/(1-beta4))/beta4-2) * log(k/E1);

  // cout.form("Vertex Z  = %.10g\n", -alpha/M_PI * vertexZ);
  // cout.form("       Z² = %.10g\n", -alpha/M_PI * vertexZ2);
  // cout.form("Intern Z  = %.10g\n", -alpha/M_PI * internZ);
  // cout.form("       Z² = %.10g\n", -alpha/M_PI * internZ2);
  // cout.form("Sum    Z  = %.10g\n", -alpha/M_PI * (internZ  + vertexZ));
  // cout.form("       Z² = %.10g\n", -alpha/M_PI * (internZ2 + vertexZ2));
  return - alpha / M_PI * (Z * vertexZ + Z * Z * vertexZ2);
}

///////////////////////////////////////////////////////////////////////////////

//#############################################################################
//
// Elastic (real) and (virtual) corrections for elastic proton
// scattering
//
//#############################################################################


double RealBHCorrection(FourVector e_in, FourVector e_out, FourVector p_in, FourVector p_out, FourVector photon)
{

  double E0 = e_in.energy();
  double Eprime = e_out.energy();

  //cout<<"______________________________________________________"<<endl;
 
  //--- By definition (see Vanderhaeghen) the q is difference between
  // incoming and outgoing electron
  FourVector q = e_in - e_out;
  double Q2 = -(q*q); // this is the definition for Q2
  double ThetaE = 2*asin(pow(-(q*q)/4.0/E0/Eprime,0.5)); // e_in.theta() is not 0, with this the angle can be derived from the formula for Q2
  FourVector pm1 = p_out + photon;
  //pm1.print("pm1");
  double deltaR = 0;
  if (sqrt(pm1*pm1) > m_proton){
    double Mm1 = sqrt(pm1*pm1);  
    
    double E0t = m_proton/Mm1*(E0 - Q2/2.0/m_proton - e_in*photon/m_proton);
    double Eprimet = m_proton/Mm1*(Eprime + Q2/2.0/m_proton - e_out*photon/m_proton);
    
    double sin2thetaet = E0*Eprime/E0t/Eprimet*pow(sin(ThetaE/2.0),2.0);
    double cos2thetaet = 1.0 - sin2thetaet;
    
    double DeltaEs = (Mm1*Mm1 - m_proton*m_proton)/2.0/Mm1;

    //cout << E0t << " " << Eprimet << " " << cos2thetaet << " " << DeltaEs << " " << Q2 << endl;
    //cout<<"Ep: "<<Eprime<<"  DeltaEs: "<<DeltaEs<<endl;  
    deltaR = alpha/M_PI*(log(DeltaEs*DeltaEs/E0t/Eprimet)*(log(Q2/pow(m_electron,2))-1.0) -
          0.5*pow(log(E0t/Eprimet),2) + 0.5*pow(log(Q2/pow(m_electron,2)),2) -M_PI*M_PI/3.0 + Spence(cos2thetaet) );
    //cout<<"Ep: "<<Eprime<<"  deltaR: "<<deltaR<<endl;
  if (deltaR < -0.1 || deltaR > 0.095)
    {
      cout << Q2 << " " << Mm1 - m_proton << " " << DeltaEs << " " << E0t << " " << Eprimet << " " << pow(cos(ThetaE/2.),2.) << " " << cos2thetaet << endl;
    } 
  }

  return deltaR;

}

//--- Vanderhaeghen/Maximon-Tjon correction for the proton side. This function 
// contains the part of the exp(delta1 + delta2) correction that does not depend 
// on the energy of the emitted photon. The part, that depends on the energy of 
// the emitted photon is inherently considered in the generator via the 
// random generator of the energy of the emitted photon. 

double protonVertexCorrectionVanderhaeghen( FourVector e_in, FourVector e_out, FourVector p_out)
{
  double E0 = e_in.energy();
  double Eprime = e_out.energy();
  FourVector q = e_in - e_out;
  double Q2 = -(q*q); // this is the definition for Q2
  double ThetaE = 2*asin(pow(-(q*q)/4.0/E0/Eprime,0.5)); // e_in.theta() is not 0, with this the angle can be derived from the formula for Q2
  
  double EprimeElastic = E0/(1.0 + E0/m_proton*(1.0 - cos(ThetaE)));
    
  double EN = p_out.energy();
  double pN = p_out.momentum();
  
  double eta = E0/EprimeElastic;
  //bss unused: double DeltaEs = eta*(EprimeElastic - Eprime);
  
  double rho = sqrt(Q2 + 4*m_proton*m_proton);
  double x = pow(sqrt(Q2) + rho,2)/(4*pow(m_proton,2));
  
  double Z = 1;
  //--- only parts that do not depend on the energy of the emitted photon.
  double delta1 = Z*2*alpha/M_PI*( Spence(1-eta/x) - Spence(1-1/(eta*x)) );
  double delta2 = Z*Z*alpha/M_PI*( 1 + EN/pN*(-0.5*pow(log(x),2) - log(x)*log(pow(rho/m_proton,2)) 
                  + log(x) - Spence(1-1/pow(x,2)) +2*Spence(-1/x) + pow(M_PI,2)/6));	 
  
  double t1 = Z*4*alpha/M_PI*log(eta);
  double t2 = Z*Z*2*alpha/M_PI*(EN/pN*log(x)-1);
  
  double t1Chi1 = securePow(4*E0*E0/x/Q2, t1/2.);
  double t2Chi2 = securePow(4*E0*E0/m_proton/m_proton, t2/2.);  
  
	   
  return exp(delta1 + delta2)*t1Chi1*t2Chi2;
}


double 
generateElasticProton::generateEvent(double helicity)
{
  double E0 = Reaction->electronIn.energy();
  generateLabAngles(&Reaction->electronOut,1,sime->getAngle(), sime->getOop(), 
		    dcte,sime->getDphi());
  
  double theta = Reaction->electronOut.theta();
  double phi   = Reaction->electronOut.phi();
  //Final Electron Energy
  double Ep = ((-E0-m_proton)*(m_e_sqr+E0*m_proton) // see Friedrich phd 2000, A.11 [Merkel], with some sign swaps (compensates) and sin^2(x) = 1/2-cos(2x)/2
	       +(m_e_sqr-E0*E0)*cos(theta)*
	       sqrt(m_proton*m_proton-m_e_sqr/2+m_e_sqr*cos(2*theta)/2))/
    ((E0*E0-m_e_sqr)*pow(cos(theta),2)-pow(E0+m_proton,2));
  
  Reaction->electronOut.initPolar(Ep, sqrt(Ep*Ep-m_e_sqr), theta, phi);
  FourVector Photon = Reaction->electronIn - Reaction->electronOut;
  Reaction->Out1 = Photon + *Reaction->getTarget();
  
  //static const NucleonFormfactor *FF = new MergellFit();
  static const NucleonFormfactor *FF = new DipoleFit();
  double q2 = Photon * Photon;
  double GE = FF->G_E(q2);
  double GM = FF->G_M(q2);
  double t =  -q2 / (4 * m_proton * m_proton);
  double a = -2 * sqrt(t * (1 + t))* tan(theta/2);
  double b =  2 * t * sqrt(1 + t) 
    * sqrt(1 + t * pow(sin(theta/2),2)) 
    * tan(theta/2)/cos(theta/2);
  double c = t*(1+2*(1+t)*pow(tan(theta/2),2));
  Momentum pol(a * GE * GM / (GE * GE + c * GM * GM),
	       0,
	       b * GM * GM / (GE * GE + c * GM * GM));
  //  cout<<pol[0]<<" "<<pol[1]<<" "<<pol[2]<<endl;
  pol.rot_theta(Photon.theta());
  pol.rot_phi(Photon.phi());    
  Reaction->Out1.setSpin(pol * helicity);

  //  pol = Reaction->Out1.getSpin();
  //cout<<pol[0]<<endl;

  return FF->CrossSection(E0, theta);
}

double 
generateElasticPolElectronPolProton::generateEvent(double helicity)
{
  double E0 = Reaction->electronIn.energy();
  generateLabAngles(&Reaction->electronOut,1,sime->getAngle(), sime->getOop(), 
		    dcte,sime->getDphi());
  
  double theta = Reaction->electronOut.theta();
  double phi   = Reaction->electronOut.phi();
  //Final Electron Energy
  double Ep = ((-E0-m_proton)*(m_e_sqr+E0*m_proton) // see Friedrich phd 2000, A.11 [Merkel], with some sign swaps (compensates) and sin^2(x) = 1/2-cos(2x)/2
	       +(m_e_sqr-E0*E0)*cos(theta)*
	       sqrt(m_proton*m_proton-m_e_sqr/2+m_e_sqr*cos(2*theta)/2))/
    ((E0*E0-m_e_sqr)*pow(cos(theta),2)-pow(E0+m_proton,2));
  
  Reaction->electronOut.initPolar(Ep, sqrt(Ep*Ep-m_e_sqr), theta, phi);
  FourVector Photon = Reaction->electronIn - Reaction->electronOut;
  Reaction->Out1 = Photon + *Reaction->getTarget();



  ////////////////////////////////////////////////////////////////////
  // AT VERTEX :
  double q2     = Photon * Photon;
  double tau    =  -q2 / (4 * m_proton * m_proton);
  double tanTh2     = tan(theta/2);


  double a      = - 2 * sqrt(tau * (1 + tau)) * tanTh2;
  double b      = - 2 * tau * tanTh2 * sqrt(1 + tau + pow((1+tau)*tanTh2,2));
  double c      = 1;
  double d      = tau * (1 + 2 * (1 + tau) * tanTh2 * tanTh2);



  //dipole form factors:
  double GE = pow(1 - q2/0.71,-2);
  double GM = pow(1 - q2/0.71,-2) * 2.79284739;

  double A_I    = a * GE * GM / (c * GE * GE + d * GM * GM);
  double A_S    = b * GM * GM / (c * GE * GE + d * GM * GM);

  //determine angles between momentum transfer and target polarization vector
  Momentum photon             = Photon.getP();
  Momentum EIN                = Reaction->electronIn.getP();
  Momentum EOUT               = Reaction->electronOut.getP();
  Momentum scatteringplane    = photon.mult(EIN, EOUT);  //vector cross product
  Momentum targetpol          = Momentum(rundb.sim.TargetPol[0], rundb.sim.TargetPol[1], rundb.sim.TargetPol[2]);
  Momentum polarization_plane = photon.mult(photon, targetpol);

  double tv = angle(photon, targetpol);
  double pv = angle(scatteringplane, polarization_plane);
  //cout << "photon " << photon[0] << "\t"<< photon[1] << "\t"<< photon[2] << "\n";
  //cout << "tv, pv=\t" << tv/rad << "\t" << pv/rad << "\n";
  

  double sinTh2   = square(sin(0.5*theta));
  double mott     = mubarn *square(alpha/(2*E0*sinTh2))*(1-sinTh2)/(1+2*E0/m_proton *sinTh2); // Mott cross section in mu barn
  double XS_unpol = mott * ((GE * GE + tau * GM * GM) / (1 + tau) + 2 * tau * GM * GM * tanTh2 * tanTh2);
  double PolBeam  = rundb.beam.P_e / 100.;
  double PolTarget= targetpol.abs();
  //cout << "heli: " << helicity << "\n"; //for double-check; it should be +1 or -1, beam pol is in PolPeam!
  //cout << "beam pol: " << PolBeam << "\n";
  //cout << "targ pol: " << PolTarget << "\n";
  
  double XS       = XS_unpol * (1 +  helicity * PolBeam * PolTarget * (A_I * sin(tv) * cos(pv) + A_S * cos(tv)));
  //double XS       = mott * (1 +  helicity * PolBeam * PolTarget * (A_I * sin(tv) * cos(pv) + A_S * cos(tv))); //bss fixme
  
  // <- AT VERTEX
  ////////////////////////////////////////////////////////////////////


  //return 1; //bss fixme temp for test of vertex position distribution (decoupled from angular acceptance) only
  return XS;
}

///////////////////////////////////////////////////////////////////////////////
////////generateElasticNucleus von Jan////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

double 
generateElasticNucleus::generateEvent(double helicity)
{




  generateLabAngles(&Reaction->electronOut,1,sime->getAngle(), sime->getOop(), 
		    dcte,sime->getDphi());

  double ntheta = Reaction->electronOut.theta();
  double phi   = Reaction->electronOut.phi();  
  //jcb: Calculate the REAL theta (Multiscattering of incident beam!)
  double theta= Reaction->electronOut.getP()* Reaction->electronIn.getP();
  
  theta/=Reaction->electronOut.getP().abs()* Reaction->electronIn.getP().abs();
  theta=acos(theta);

  
  //std::cout << "Elastic Nucleus"<< std::endl;

  // which element did we hit (havar!)

//double elem=prndm()*(428+200+176+130+28+20+16);
  double elem=prndm()*(848+449+368+259+18+24+35);

  double tmass,Z; //mass and Charge 
  double R,a; //radius and skin  from Friedrich/Vögler Salient features of ...

  double tmassp;
  //double r;
  switch (Form)
    {
    case 3: case 5:case 6: case 7: case 8:
      tmass=m_O16;
      Z=8;
      R=2.777;
      a=0.839; 
      tmassp=tmass;
      
      if( Form==5) tmassp+=0.007117;//tmassp+=0.0069171;
      if (Form==6) tmassp+=0.00613; // tmassp+=0.006049;
      if( Form==7) tmassp+=0.0069171;
      if (Form==8) tmassp+=0.006049;
      
      break;
    case 4:
      std::cout << "N12"<< std::endl;
      printf("N12");
      tmass=m_N14;
      tmassp=tmass;
      Z=7;
      R=2.56;
      a=0.82;
      //	R=2.436; // C12
      //	a=0.810;
      break;
      
    default:
	//std::cout << "Havar"<< std::endl;
	//printf("Havar");
			if (elem<848) // Co
    {
      tmass=m_Co59;
      Z=27;
      R=(4.356+4.396)/2; //from Ni58 nad Ni60
      a=0.915;
      //std::cout << "Co"<< std::endl;
      }
									 else
	
							     if (elem<848+449)//Cr
      {
	//std::cout << "Cr"<< std::endl;
	tmass=m_Cr52;
	Z=24;	 
	R=4.173;
	a=0.924;
      }
      else
      if(elem<848+449+368) //Fe
	{
	  tmass=m_Fe56;
	  Z=26;
	  R=4.286;
	  a=0.936;
	}
      else
	if(elem<848+449+368+259) //Ni
	  {
	    tmass=m_Ni58;
	    Z=28;
	    R=4.356;
	    a=0.922;
	  }
	else
	  if(elem<848+449+368+259+18) //W
	    {
	      tmass=m_W184;
	      Z=74;
	      R=6.5;  //estimated
	      a=1;
	     
	    }
	    else
	   if(elem<848+449+368+259+18+24) //Mo  
	     {
	   	tmass=m_Mo98;
	   	Z=42;
	   	R=5.190;
	   	a=1.007;
	     }
	      else //Mn
	      {
		tmass=m_Mn55;
		Z=25;
		R=4.26; //estimated
		a=0.91;
		}
	    

  tmassp=tmass;
  //std::cout << "tmassp="<< tmassp<< std::endl;
  break;
  }

//Final Electron Energy
//   double Ep = (-(E0+tmass)*(m_e_sqr+E0*tmass)
// 	       +(m_e_sqr-E0*E0)*cos(theta)*
// 	       sqrt(tmass*tmass-m_e_sqr/2+m_e_sqr*cos(2*theta)/2))/
//     ((E0*E0-m_e_sqr)*pow(cos(theta),2)-pow(E0+tmass,2));

  double E0 = Reaction->electronIn.energy();

  double p_sqr=E0*E0-m_e_sqr;
  double cts=cos(theta)*cos(theta);
  double tmass_diff=tmass*tmass-tmassp*tmassp;
  double E0tmass=E0+tmass;
   
  //For tmassp=tmass, this gives the same results as formula above!



  double Ep=((E0*m_e_sqr + (E0*E0tmass + m_e_sqr)*tmass) + 0.5*E0tmass*tmass_diff +
	      sqrt( p_sqr*cts* ( m_e_sqr*(m_e_sqr-tmassp*tmassp+p_sqr*cts) + 0.25*tmass_diff*tmass_diff
		   - E0*(E0*(m_e_sqr - tmass*tmass)  - tmass*tmass_diff)))) / (E0tmass*E0tmass - p_sqr*cts);

  Reaction->electronOut.initPolar(Ep, sqrt(Ep*Ep-m_e_sqr), ntheta, phi); 


  // let's do energy loss all the time, like b4.
  double q2 = (Reaction->electronIn-Reaction->electronOut).square();
  double t =alpha/M_PI*((log(fabs(q2)/m_e_sqr)-1));
  //  std::cout <<1/t<<std::endl;

  Reaction->electronIn-= E0 * securePow(sobol(), 1/t);

  // recalculate kinematics
  E0 = Reaction->electronIn.energy();

  
  p_sqr=E0*E0-m_e_sqr;
  cts=cos(theta)*cos(theta);
  tmass_diff=tmass*tmass-tmassp*tmassp;
  E0tmass=E0+tmass;

  Ep=((E0*m_e_sqr + (E0*E0tmass + m_e_sqr)*tmass) + 0.5*E0tmass*tmass_diff +
	      sqrt( p_sqr*cts* ( m_e_sqr*(m_e_sqr-tmassp*tmassp+p_sqr*cts) + 0.25*tmass_diff*tmass_diff
		   - E0*(E0*(m_e_sqr - tmass*tmass)  - tmass*tmass_diff)))) / (E0tmass*E0tmass - p_sqr*cts);

  Reaction->electronOut.initPolar(Ep, sqrt(Ep*Ep-m_e_sqr), ntheta, phi); 




//        (2*E*m**2 + 2*E**2*mt + 2*m**2*mt + 3*E*mt**2 +
//       -         mt**3 - E*mtp**2 - mt*mtp**2 +
//       -         Sqrt(p**2*Cos(theta)**2*
//       -           (4*m**4 + mt**4 - 4*E**2*(m**2 - mt**2) - 4*m**2*mtp**2 -
//       -             2*mt**2*mtp**2 + mtp**4 + 4*E*(mt**3 - mt*mtp**2) +
//       -             4*m**2*p**2*Cos(theta)**2)))/
//       -       (2.*((E + mt)**2 - p**2*Cos(theta)**2))))



  FourVector Photon = Reaction->electronIn - Reaction->electronOut;
  Reaction->Out1 = Photon + *Reaction->getTarget();
  
  double sinTh2 = pow(sin(theta/2), 2);
  double mott= pow(Z*alpha/2/E0/sinTh2,2)*(1-sinTh2)/(1+2*E0/tmass*sinTh2)*mubarn*(848+449+368+259+18+24+35);

  Momentum mq=Reaction->electronIn.getP()-Reaction->electronOut.getP();
  R/=0.197327; //to get 1/GeV instead of fm
  a/=0.197327; 
  double q=sqrt(mq*mq);

  double alp=q*R;



  double F1=exp(-q*q/2*a*a);
  double F2=(3.0/alp/alp/alp*(sin(alp)-alp*cos(alpha)));
  double F=F1*F2;
  //  cout <<alp<<" "<<F1<<" "<<F2<<" "<<F<<endl;
  q2 = (Reaction->electronIn-Reaction->electronOut).square();
  t =alpha/M_PI*((log(fabs(q2)/m_e_sqr)-1));
  Reaction->electronOut-= Reaction->electronOut.energy() * securePow(sobol(), 1/t);


  switch (Form)
    {
    case 1:
      return mott*F1*F1;
    case 2:  case 3: case 4:
      return mott*F*F;
    default:
      return mott;
    }
}


///////////////////////////////////////////////////////////////////////////////
/////////generateElasticNucleus von Jan Ende///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/////////generateQuasiElasticNucleus von Jan///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

double 
generateQuasiElasticNucleus::generateEvent(double helicity)
{


  // which element did we hit (havar!)
  double elem=prndm()*(848+449+368+259+18+24+35);

  double tmass,Z,A; //mass and Charge and Atomic Number
  double pmass; //mass of product after proton/neutron knock out
  
  double pf; //Fermi impulse

  int pknock=(sobol()>0.5);


  if (elem<848) // Co
    {
      tmass=m_Co59;
      pmass=pknock?(m_Fe58):(m_Co58);
      Z=27;
      A=59;
      //std::cout << "Co QEL"<< std::endl;
      pf=0.257;
    }
  else
    if (elem<848+449)//Cr
      {
	tmass=m_Cr52;
	pmass=pknock?(m_V51):(m_Cr51);
	Z=24;	 
	A=52;
	pf=0.256;
      }
    else
      if(elem<848+449+368) //Fe
	{
	  tmass=m_Fe56;
	  pmass=pknock?(m_Mn55):(m_Fe55);
	  Z=26;
	  A=56;
	  pf=0.2565;
	}
      else
	if(elem<848+449+368+259) //Ni
	  {
	    tmass=m_Ni58;
	    pmass=pknock?(m_Co57):(m_Ni57);
	    Z=28;
	    A=58;
	    pf=0.257;
	  }
	else
	  if(elem<848+449+368+259+18) //W
	    {
	      tmass=m_W184;
	      pmass=pknock?(m_Ta183):(m_W183);
	      Z=74;
              A=184;
	      pf=0.264;
	     
	    }
	  else
	    if(elem<848+449+368+259+18+24) //Mo
	      {
		tmass=m_Mo98;
		pmass=pknock?(m_Nb97):(m_Mo97);
		Z=42;
		A=98;
		pf=0.259;
	      }
	    else //Mn
	      {
		tmass=m_Mn55;
		pmass=pknock?(m_Cr54):(m_Mn54);
		Z=25;
		A=55;
		pf=0.2565;
	      }

  double weight=(848+449+368+259+18+24+35);
  double dpe    = sime->getMomentum()*sime->getDmomentum();

  double E0 = Reaction->electronIn.energy();
  double Eprime = sime->getMomentum()- (2*sobol()-1)*dpe;

  generateLabAngles(&Reaction->electronOut,Eprime,sime->getAngle(), sime->getOop(), 
		    dcte,sime->getDphi());


  // let's do energy loss all the time, like b4.
  double q2 = (Reaction->electronIn-Reaction->electronOut).square();
  double t =alpha/M_PI*((log(fabs(q2)/m_e_sqr)-1));
  //  std::cout <<1/t<<std::endl;

  Reaction->electronIn-= E0 * securePow(sobol(), 1/t);
  E0 = Reaction->electronIn.energy();
  
  FourVector photon = Reaction->electronIn-Reaction->electronOut;


  Particle nucleonout,productout;
  // generate proton angles in LAB frame and calculate momentum

  double maxangle=M_PI/2;
  if (photon.momentum()>0.3)
    maxangle=asin(0.3/photon.momentum()); //0.3 is max pf

     generateLabAngles(&nucleonout, 1, 0, 0, sin(maxangle), maxangle*2);
  //    generateLabAngles(&nucleonout, 1, 0,0, 0,0);


  nucleonout=nucleonout.rotateTo(photon);

  
  double 
    ct = cos((nucleonout.rotate(photon)).theta()),
    D  = tmass,
    m2 = sqr(pknock?(m_proton):(m_neutron)),
    M2 = sqr(pmass),
    w  = photon.energy(),
    q  = photon.momentum(),
    det = (D*D*D*D+m2*m2+4*D*D*D*w-2*D*D*(m2+M2+q*q-3*w*w)+sqr(M2+q*q-w*w)
	   -4*D*w*(m2+M2+ q*q-w*w)-2*m2*(M2+q*q-2*ct*ct*q*q+w*w));


  if (det<0) return 0; // photon energy to low
  double pp, sign = 1;//sobol()>0.5 ? -1:1;

  pp=(ct*q*(m2-M2-q*q+sqr(D+w))+sign*(D+w)*sqrt(det))/(2*(sqr(D+w)-sqr(ct*q)));
  nucleonout.initPolar(sqrt(m2+pp*pp),pp,
			   nucleonout.theta(), nucleonout.phi());
  productout = photon + FourVector(tmass) - nucleonout;



  static int rej=0;
  if  (productout.momentum()>pf) 
    {
      rej++;
      //      std::cout<<"Rej:" <<pf<<" "<<sign<<" "<<productout.momentum()<<" "<<rej<<" Ct:"<<ct<<" "<<maxangle<<" "<<sign*pp<<std::endl;
      return 0; //Fermi function
    }
  //  std::cout<<"Acc:" <<pf<<" "<<sign<<" "<<productout.momentum()<<" "<<rej<<" Ct:"<<ct<<" "<<maxangle<<" "<<sign*pp<<std::endl;
  weight*=
    8* sin(maxangle)*maxangle*     // generation volume proton 
    8 * dcte * sime->getDphi()* dpe; // generation volume electron

  weight*=(pknock?(Z):(A-Z))/pf;// Norm of spectral function
  // now, calculate deforest 
  
   weight*=nucleonout.momentum()*nucleonout.energy();

    if (pknock)
      weight*=pwia->deForestCC1(Reaction->electronIn,Reaction->electronOut, nucleonout); 
    else
      weight*=pwia->deForestCC1_neutron(Reaction->electronIn,Reaction->electronOut, nucleonout); 


  Reaction->electronOut-= Reaction->electronOut.energy() * securePow(sobol(), 1/t);
   
  return weight;
}



///////////////////////////////////////////////////////////////////////////////
/////////generateQuasiElasticNucleus von Jan Ende//////////////////////////////
///////////////////////////////////////////////////////////////////////////////



//################################################################################
//##########     beginning elastic deuteron   ####################################
//################################################################################


const double AbbottFit3::SoGR[24] = {
  0.0, 0.4, 0.8, 1.2, 1.6, 2.0, 2.4, 2.8, 3.2, 3.6, 4.0, 4.4, 
  4.8, 5.2, 5.6, 6.0, 6.5, 7.0, 7.5, 8.0, 8.5, 9.0, 9.5, 10.0
};
const double AbbottFit3::SoGC[24] = {
  0.001586, 0.153416, 0.177804, 0.198969, 0.145431, 0.097231, 
  0.067972, 0.050103, 0.025602, 0.025764, 0.017142, 0.012376, 
  0.008021, 0.006033, 0.003642, 0.003163, 0.002188, 0.001342, 
  0.000851, 0.000538, 0.000333, 0.000217, 0.000126, 0.000100
};
const double AbbottFit3::SoGQ[24] = {
  0.046513, 0.138087, 0.181425, 0.174011, 0.139929, 0.091150, 
  0.070100, 0.047832, 0.031397, 0.025241, 0.016786, 0.012042, 
  0.007777, 0.005819, 0.003497, 0.003023, 0.002082, 0.001270, 
  0.000802, 0.000503, 0.000311, 0.000200, 0.000116, 0.000090
};
const double AbbottFit3::SoGM[24] = {
  0.043842, 0.159966, 0.182563, 0.177163, 0.120109, 0.103085, 
  0.067460, 0.038359, 0.031553, 0.023661, 0.015861, 0.011489, 
  0.007484, 0.005640, 0.003418, 0.002971, 0.002063, 0.001265, 
  0.000805, 0.000508, 0.000316, 0.000204, 0.000121, 0.000093
};
const double AbbottFit3::gamma = 0.6 * sqrt(2.0/3.0);

// Valid for: D, He3, He4, C12, N14, O16 (not tested, yet) 06/26/2014 M.Distler
generateElasticDeuteron::generateElasticDeuteron(reaction *r,
						 simDetectorBase *Sime, 
						 simDetectorBase *Sim1,
						 SIMUL *rundb)
{ 
  Label      = "d[s]";
  Unit       = "[m]b";
  Reaction   = r; sime=Sime; sim1=Sim1;
  targetmass = Reaction->getTarget()->getMass();
  dcte       = sin(sime->getDtheta());
  dphie      = calc_dphie();
  Gamma      = 1;
  if (!strcmp(Reaction->getTarget()->getName(), "He4")) {
    FF = new OttermannHe4();
  } else if (!strcmp(Reaction->getTarget()->getName(), "He3")) {
    FF = new OttermannHe3();
  } else {
    FF = new AbbottFit3();
  }
  //  std::cout << "Using generateElasticDeuteron with Simul.GeneratorFlag = " << rundb.sim.GeneratorFlag << "\n";

}

generateElasticDeuteron::~generateElasticDeuteron()
{
  delete FF;
}

double 
generateElasticDeuteron::generateEvent(double)
{
  //  std::cerr << "selecting generator, rundb.sim.GeneratorFlag=" << rundb.sim.GeneratorFlag << std::endl;
  if (rundb.sim.GeneratorFlag==-1)
    return generateEvent_VMOD(); //bss: I judge this to be wrong, but leave it in here since it is not used for deuteron FF experiment analysis alone (but also He3, He4 inclusive)
  if (rundb.sim.GeneratorFlag==1)
    return generateEvent_V1();
  if (rundb.sim.GeneratorFlag==2)
    return generateEvent_V2();
  if (rundb.sim.GeneratorFlag==3)
    return generateEvent_V3();
  std::cerr << "\nplease use valid run.db value for: run.db.sim.GeneratorFlag\n";
  std::cerr << " Simul.GeneratorFlag = 1  //(ed scattering, Cola standard radiation corrections)\n";
  std::cerr << " Simul.GeneratorFlag = 2  //(ed scattering, VdH internal bremsstrahlung style)\n";
  std::cerr << " Simul.GeneratorFlag = 3  //(ed scattering, VdH internal bremsstrahlung style + hadron contributions)\n";
  std::cerr << " Simul.GeneratorFlag = -1 //(MOD style, used for He-3, He-4)\n";
  exit(69);
}

double generateElasticDeuteron::generateEvent_V1() 
{ //this one uses the default internal bremsstrahlung generation, done in Radiation.cc, as well as vertexCorrection(..)
  //XS-walk is also inclusive, because internalBremsstralhung is used twice: before and after *this
  double E0 = Reaction->electronIn.energy();
  double M  = targetmass; 

  generateLabAngles(&Reaction->electronOut,1,sime->getAngle(), sime->getOop(), dcte,sime->getDphi());
  double ntheta = Reaction->electronOut.theta(); //angle in lab != scattering angle (multi scattering)
  double theta  = acos((Reaction->electronOut.getP()       * Reaction->electronIn.getP())
		       /(Reaction->electronOut.getP().abs()* Reaction->electronIn.getP().abs()));
  //using theta!=ntheta significantly broadens the Delta E' spectrum!
  //TEST:
  //theta = ntheta; //bss FIXME
  double phi    = Reaction->electronOut.phi();
  double Ep     = calc_elasticEprime(E0, theta, M); //outgoing electron energy, exact
  //approx would be: Ep = E0*M/(M+E0*(1.0-cos(theta)));
  Reaction->electronOut.initPolar(Ep, sqrt(Ep*Ep-m_e_sqr), ntheta, phi);
  FourVector Photon = Reaction->electronIn - Reaction->electronOut;
  Reaction->Out1    = Photon + *Reaction->getTarget();
  return FF->CrossSection(E0, theta);
}

//internal bremsstrahlung generation and radiation corrections (electron side+vacuum) are implemented here now.
//StandardCola correction is disabled via the GeneratorFlag variable.
double generateElasticDeuteron::generateEvent_V2()
{
  //ELASTIC ELECTRON SCATTERING KINEMATICS:
  double E0 = Reaction->electronIn.energy();
  double M = targetmass; 

  generateLabAngles(&Reaction->electronOut, 1, sime->getAngle(), sime->getOop(), dcte, sime->getDphi());
  double ntheta = Reaction->electronOut.theta(); //angle in lab != scattering angle (multi scattering)
  double theta  = acos((Reaction->electronOut.getP()       * Reaction->electronIn.getP())
		       /(Reaction->electronOut.getP().abs()* Reaction->electronIn.getP().abs()));
  double phi   = Reaction->electronOut.phi();
  //calculate preliminary kinematics (needed for radiator thickness calculation): elastic scattering
  double Ep    = calc_elasticEprime(E0, theta, M); //outgoing electron energy, exact
  Reaction->electronOut.initPolar(Ep, sqrt(Ep*Ep-m_e_sqr), ntheta, phi); 
  FourVector photon = Reaction->electronIn - Reaction->electronOut;
  double     Q2     = -photon.square();

  //RADIATION ELOSS ~ APPROXIMATED FROM ELASTIC ELECTRON SCATTERING KINEMATICS (with a very good accuracy -- Vanderhaeghen PRC 62 025501 p. 20)
  double a = alpha/M_PI*((log(Q2/m_e_sqr)-1)); //EQUIVALENT RADIATOR THICKNESS; vdH equ. 81 and IV B (i), jcb 5.9; 
  //ISR:
  Reaction->electronIn -= E0 * securePow(sobol(), 1/a);

  // recalculate kinematics -> this is used for cross section calculation and Delta E' independant radiative corrections
  E0 = Reaction->electronIn.energy();
  Ep = calc_elasticEprime(E0, theta, M);
  Reaction->electronOut.initPolar(Ep, sqrt(Ep*Ep-m_e_sqr), ntheta, phi);
  photon = Reaction->electronIn - Reaction->electronOut;
  Q2     = -photon.square();
  double XS = FF->CrossSection(E0, theta); //corrected for previous radiation loss of incoming electron
  //further radiative corrections:
  XS *= Radiation_LeptonAndVacuumCorrection(Q2, E0, Ep, theta);

  //FSR, after recalculating 'a' 
  a = alpha/M_PI*((log(Q2/m_e_sqr)-1));
  Reaction->electronOut -= Reaction->electronOut.energy() * securePow(sobol(), 1/a);
  return XS;
}

//INCLUDE RADIATIVE EFFECTS HADRON SIDE:
double generateElasticDeuteron::generateEvent_V3()
{
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //ELASTIC ELECTRON SCATTERING KINEMATICS:
  double E0 = Reaction->electronIn.energy();
  double M = targetmass; 
  double Z = Reaction->getTarget()->getCharge(); //needed for radiation correction on hadron side

  generateLabAngles(&Reaction->electronOut, 1, sime->getAngle(), sime->getOop(), dcte, sime->getDphi());
  double ntheta = Reaction->electronOut.theta(); //angle in lab != scattering angle (multi scattering)
  double theta  = acos((Reaction->electronOut.getP()       * Reaction->electronIn.getP())
		       /(Reaction->electronOut.getP().abs()* Reaction->electronIn.getP().abs()));
  double phi   = Reaction->electronOut.phi();
  //calculate preliminary kinematics (needed for radiator thickness calculation): elastic scattering
  double Ep    = calc_elasticEprime(E0, theta, M);
  Reaction->electronOut.initPolar(Ep, sqrt(Ep*Ep-m_e_sqr), ntheta, phi); // outgoing electron energy
  FourVector photon = Reaction->electronIn - Reaction->electronOut;
  double Q2    = -photon.square();

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //ISR:
  //quantities used for radiation tail generation ~ approximated from elastic electron scattering kinematics
  //(with a very good accuracy -- Vanderhaeghen PRC 62 025501 p. 20)
  //hadron side: 
  double eta = E0/Ep; //recoil factor
  FourVector hadron = Reaction->Out1 = photon + *Reaction->getTarget();
  double rho = sqrt(Q2+4*M*M);                                //MTj p.2, II first paragraph; JCB 2.20
  double x = pow(0.5*(sqrt(Q2)+rho)/M,2);                     //MTj p.2, II first paragraph; JCB 2.20
  double b = 2*alpha*Z/M_PI*log(eta);
  double c = alpha*Z*Z/M_PI*(hadron.energy()/hadron.momentum()*log(x)-1);
  //electron side: 
  double a = alpha/M_PI*((log(Q2/m_e_sqr)-1)); //EQUIVALENT RADIATOR THICKNESS; jcb 5.9, vdH equ. 81 and IV B (i); 
  //combined value for radiation tail generation:
  double t = a + b + c;
  Reaction->electronIn -= E0 * securePow(sobol(), 1/t);

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
  // RECALCULATE KINEMATICS:
  // same formulas as before, but with reduced incoming electron energy
  // this is used for cross section calculation, second tail generation part and Delta E' independant radiative corrections
  E0     = Reaction->electronIn.energy();
  Ep     = calc_elasticEprime(E0, theta, M);
  Reaction->electronOut.initPolar(Ep, sqrt(Ep*Ep-m_e_sqr), ntheta, phi);
  photon = Reaction->electronIn - Reaction->electronOut;
  Q2     = -photon.square();
  ///////////////////////////////////////////
  eta = E0/Ep; //recoil factor
  hadron = Reaction->Out1 = photon + *Reaction->getTarget(); //(neglect that previous ISR photon(s) can be radiated off hadron, changing its incoming kinematics slightly)
  rho = sqrt(Q2+4*M*M);                                //MTj p.2, II first paragraph; JCB 2.20
  x = pow(0.5*(sqrt(Q2)+rho)/M,2);                     //MTj p.2, II first paragraph; JCB 2.20
  b = 2*alpha*Z/M_PI*log(eta);
  c = alpha*Z*Z/M_PI*(hadron.energy()/hadron.momentum()*log(x)-1);
  a = alpha/M_PI*((log(Q2/m_e_sqr)-1));
  t = a + b + c;
  
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // XS CALCULATION AND RADIATIVE CORRECTION:
  // calculate elastic cross section for that kinematics (after ISR, before FSR):
  double XS = FF->CrossSection(E0, theta); //corrected for previous radiation loss of incoming electron
  //radiative corrections (after ISR, before FSR)::
  XS *= Radiation_LeptonAndVacuumCorrection(Q2, E0, Ep, theta);
  XS *= Radiation_HadronCorrection(Z, M, hadron.energy(), hadron.momentum(), rho, x, eta);
  XS *= securePow(4*E0*Ep/Q2/x, b) * securePow(4*E0*Ep/M/M, c); //throwing eloss once: then Ep->E0 in this line; doesnt make any significant difference anyhow (b,c<<1)
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  //FSR
  t = a + b + c;
  Reaction->electronOut -= Reaction->electronOut.energy() * securePow(sobol(), 1/t);

  return XS; 
}

double generateElasticDeuteron::generateEvent_VMOD()
{
  double E0 = Reaction->electronIn.energy();
  generateLabAngles(&Reaction->electronOut, 1,
		    sime->getAngle(), sime->getOop(), dcte, sime->getDphi());
  
  double theta = Reaction->electronOut.theta();
  double phi   = Reaction->electronOut.phi();

  // outgoing electron energy
  double Ep = ((-E0-targetmass)*(m_e_sqr+E0*targetmass)
	       +(m_e_sqr-E0*E0)*cos(theta)*
	       sqrt(targetmass*targetmass-m_e_sqr/2+m_e_sqr*cos(2*theta)/2))/
    ((E0*E0-m_e_sqr)*pow(cos(theta),2)-pow(E0+targetmass,2));
  Reaction->electronOut.initPolar(Ep, sqrt(Ep*Ep-m_e_sqr), theta, phi);

  // radiation loss: incoming electron
  FourVector photon = Reaction->electronIn - Reaction->electronOut;
  double q2 = photon.square();
  double a = alpha/M_PI*((log(fabs(q2)/m_e_sqr)-1)); //jcb 5.9, vdH IV b (i)
  double t = a;
  Reaction->electronIn -= E0 * securePow(sobol(), 1/t);

  // recalculate kinematics
  E0 = Reaction->electronIn.energy();
  Ep = ((-E0-targetmass)*(m_e_sqr+E0*targetmass)
	+(m_e_sqr-E0*E0)*cos(theta)*
	sqrt(targetmass*targetmass-m_e_sqr/2+m_e_sqr*cos(2*theta)/2))/
    ((E0*E0-m_e_sqr)*pow(cos(theta),2)-pow(E0+targetmass,2));
  Reaction->electronOut.initPolar(Ep, sqrt(Ep*Ep-m_e_sqr), theta, phi);

  // radiation loss: outgoing electron
  photon = Reaction->electronIn - Reaction->electronOut;
  FourVector nucleus = Reaction->Out1 = photon + *Reaction->getTarget();
  q2 = photon.square();
  a = alpha/M_PI*((log(fabs(q2)/m_e_sqr)-1));
  double eta = E0/Ep;
  double b = 2*alpha/M_PI*log(eta);
  double rho = sqrt(fabs(q2)+4*pow(targetmass,2));
  double x = pow(0.5*(sqrt(fabs(q2))+rho)/targetmass,2);
  double c = alpha/M_PI*(nucleus.energy()/nucleus.momentum()*log(x)-1);
  t = 2*a+2*b+2*c;
  Reaction->electronOut -= Reaction->electronOut.energy() * securePow(sobol(), 1/t);

  return securePow(eta, a) * securePow(pow(2*E0,2)/fabs(q2)/x, b) *
    securePow(pow(2*E0/targetmass,2), c) * FF->CrossSection(E0, theta);
    }





//################################################################################
//##########     end elastic deuteron   ##########################################
//################################################################################



//new class for nitrogen 
double 
generateElasticNitrogen::generateEvent(double) //bss FIXME: two times sampling, once with t=a, once with t=2a, weight with eta^a, which is not consistent all in all
{
  std::cout << "THIS CLASS IS COMPLETELY SCREWED!! e.g. deuteron mass in Eprime calculation, or internalBremsstrahlung once with 'a' (ISR), then with '2a' (FSR)\n";
  std::cerr << "THIS CLASS IS COMPLETELY SCREWED!! e.g. deuteron mass in Eprime calculation, or internalBremsstrahlung once with 'a' (ISR), then with '2a' (FSR)\n";
  double E0 = Reaction->electronIn.energy();
  generateLabAngles(&Reaction->electronOut,1,sime->getAngle(), sime->getOop(), 
		    dcte,sime->getDphi());
  
  double theta = Reaction->electronOut.theta();
  double phi   = Reaction->electronOut.phi();
  //Final Electron Energy
  double Ep = ((-E0-m_N14)*(m_e_sqr+E0*m_N14)
	       +(m_e_sqr-E0*E0)*cos(theta)*
	       sqrt(m_N14*m_N14-m_e_sqr/2+m_e_sqr*cos(2*theta)/2))/
    ((E0*E0-m_e_sqr)*pow(cos(theta),2)-pow(E0+m_N14,2));
  
  Reaction->electronOut.initPolar(Ep, sqrt(Ep*Ep-m_e_sqr), theta, phi);

  // radiation loss
  FourVector photon = Reaction->electronIn - Reaction->electronOut;
  double q2 = photon.square();
  double a = alpha/M_PI*((log(fabs(q2)/m_e_sqr)-1));
  double t = a;

  Reaction->electronIn -= E0 * securePow(sobol(), 1/t);

  // recalculate kinematics
  E0 = Reaction->electronIn.energy();
  Ep = ((-E0-m_deuteron)*(m_e_sqr+E0*m_deuteron)
	+(m_e_sqr-E0*E0)*cos(theta)*
	sqrt(m_deuteron*m_deuteron-m_e_sqr/2+m_e_sqr*cos(2*theta)/2))/
    ((E0*E0-m_e_sqr)*pow(cos(theta),2)-pow(E0+m_deuteron,2));
  Reaction->electronOut.initPolar(Ep, sqrt(Ep*Ep-m_e_sqr), theta, phi);

  photon = Reaction->electronIn - Reaction->electronOut;
  Reaction->Out1 = photon + *Reaction->getTarget();
  q2 = photon.square();
  a = alpha/M_PI*((log(fabs(q2)/m_e_sqr)-1));
  t = 2*a;
  Reaction->electronOut -= Reaction->electronOut.energy() * securePow(sobol(), 1/t);


  static const NitrogenFormfactor *FF = new DallyFit();
  return securePow(E0/Ep, a) * FF->CrossSection(E0, theta);
}

///////////////////////////////////////////////////////////////////////////////
double 
generateMott::generateEvent(double)
{
  std::cout << "THIS CLASS IS HARD CODED FOR DEUTERON TARGET\n";
  std::cerr << "THIS CLASS IS HARD CODED FOR DEUTERON TARGET\n";
  double E0 = Reaction->electronIn.energy();
  generateLabAngles(&Reaction->electronOut,1,sime->getAngle(), sime->getOop(), 
		    dcte,sime->getDphi());
  
  double theta = Reaction->electronOut.theta();
  double phi   = Reaction->electronOut.phi();

  double Ep = ((-E0-m_deuteron)*(m_e_sqr+E0*m_deuteron)
	       +(m_e_sqr-E0*E0)*cos(theta)*
	       sqrt(m_deuteron*m_deuteron-m_e_sqr/2+m_e_sqr*cos(2*theta)/2))/
    ((E0*E0-m_e_sqr)*pow(cos(theta),2)-pow(E0+m_deuteron,2));

  Reaction->electronOut.initPolar(Ep, sqrt(Ep*Ep-m_e_sqr), theta, phi);
  FourVector Photon = Reaction->electronIn - Reaction->electronOut;
  Reaction->Out1 = Photon + *Reaction->getTarget();

  double sinTh2 = square(sin(0.5*theta));
  return mubarn *square(alpha/(2*E0*sinTh2))*(1-sinTh2)/(1+2*E0/m_deuteron*sinTh2);
}

//////////////////////////////////////////////////////////////////////////////
double 
generateTwoBody::generateEvent(double helicity)
{ 
  Particle photon = P_gamma;
  photon          = Reaction->electronIn - Reaction->electronOut;
  FourVector CMS  = photon + *Reaction->getTarget();
  
  double 
    E0      = Reaction->electronIn.energy(),
    theta   = Reaction->electronOut.theta(),
    Ep      = Reaction->electronOut.energy(),
    s       = CMS.square(),
    q2      = photon.square(),
    epsilon = 1/(1-2*(pow(photon.energy(),2)-q2)/q2*pow(tan(theta/2),2)),
    kgamma  = (s - targetmass*targetmass) / 2 / targetmass;
  Gamma  = alpha/2/M_PI/M_PI * Ep/E0 * kgamma / -q2 / (1-epsilon);

  weight = Gamma;

  if (s < Reaction->threshold*Reaction->threshold) return 0; // below threshold

  double m1       = Reaction->getM1()->getMass();
  double m2       = Reaction->getM2()->getMass();
  double pcms     = sqrt((s - (m1+m2)*(m1+m2))*(s - (m1-m2)*(m1-m2))/4/s);
  double thetacms = acos(coscmsmin + sobol() * (coscmsmax-coscmsmin));
  double phicms   =      phicmsmin + sobol() * (phicmsmax-phicmsmin);

  // run.db CMS angles are for the first (detected) particle
  FourVector Out2CMS = Polar(energy(m2,pcms), pcms,
			     M_PI-thetacms, M_PI+phicms);
  Out2CMS.rot_theta(photon.theta());
  Out2CMS.rot_phi(  photon.phi()  );

  Reaction->Out2 = Lorentz(Out2CMS, CMS);
  Reaction->Out1 = CMS - Reaction->Out2;

  //  cout << Ep << " " << kgamma << endl;
  if (m) {
    // cout << "Model used:" << m->name << endl;

    // complementary angle for non-pi0 channels (MAID uses theta_piq only)
    if (!strcmp(m->name,"Maid") && *Reaction->getM2()!=P_pi0) {
      thetacms = M_PI-thetacms; phicms += M_PI;
    }
    if (!strcmp(m->name,"kMaid") && *Reaction->getM1()==P_kplus &&
	(*Reaction->getM2()==P_Lambda || *Reaction->getM2()==P_Sigma0) ) 
      {
      /*
        double thetadeg = thetacms/3.14*180;
        double phideg = phicms/3.14*180;
        cout << "theta: " << thetadeg << "phi: " << phideg << endl;
      */

      double E_o = rundb.Ebeam/1000;
      double EP_o = rundb.B.momentum/1000;
      double q_o = E_o - EP_o;
      double theta_o = rundb.B.angle/180*M_PI;
      double thetacms_o = acos(0.35);
      double phicms_o = 0;

      q2_o = -4 * E_o * EP_o * pow(sin(theta_o/2),2);
      s_o  = targetmass*targetmass + q2_o + 2 * targetmass * q_o;
      eps_o = 1/(1 - 2*(q_o*q_o - q2_o) / q2_o * 
			  pow(tan(theta_o/2),2));

      double xsec= m->CrossSection(sqrt(s), -q2, epsilon,
      				   thetacms, phicms, helicity);
 
      //      cout << "xsec: " << xsec << endl;
      
      /* cout << "Q2/Q2_o: " << q2/q2_o << "  W/W_o: " << sqrt(s)/sqrt(s_o) 
	      << "  eps/eps_o: " << epsilon/eps_o << endl;
      */

      // scales the cross-section in W, Q2, and cos theta to
      // project acceptance onto a single kinematic variable
      // 1: for theta dependence
      // 2: for Q2 dependence
      // 3: for W dependence
      if (scale == 1)
	{scalefactor = xsec/m->CrossSection(sqrt(s_o), -q2_o, eps_o,
				  thetacms, phicms_o, 0);
	  weight *= scalefactor;
	}
      else if (scale == 2)
	{scalefactor = xsec/m->CrossSection(sqrt(s_o), -q2, eps_o,
				  thetacms_o, phicms_o, 0);
	  weight *= scalefactor;
	}
      else if (scale == 3)
	{scalefactor = xsec/m->CrossSection(sqrt(s), -q2_o, eps_o,
				  thetacms_o, phicms_o, 0);
	  weight *= scalefactor;
	}
      else if (scale == 0) weight *= xsec;

      /*      cout << "Q2_o: " << -q2_o << " W_o: " << sqrt(s_o) 
	   << " eps_o: " << eps_o
	   << " xsec/xsec_o: " << scalefactor << endl;
      */
      //      cout << "weight (sigma): " << xsec << endl;
    }
    else {
      thetacms = M_PI-thetacms;
      weight *= m->CrossSection(sqrt(s) - Reaction->threshold, q2, epsilon,
				thetacms, phicms, helicity);
    }
    double P[3]={0, 0, 0};
    m->P_CMS(P, sqrt(s) - Reaction->threshold, q2, epsilon, 
	     thetacms, phicms, helicity);
    P[0] = -P[0];
    P[1] = -P[1];
    Reaction->CMSPolarization.set(P[0], P[1], P[2]);
    
    Momentum Spin(P[0], P[1], P[2]);

    Particle out1cm = P_proton;
    out1cm = Reaction->Out1.Lorentz(-CMS).rotate(photon);
    out1cm = out1cm.setSpin(Spin);
    Reaction->Out1 = out1cm.rotateTo(photon).Lorentz(CMS);
  }

  if (dpcms!=0) { //Correction for bin middle dW=1MeV
    weight *= pcms / kgamma * sqrt(s) / targetmass;
    int  dW = (int) ((sqrt(s) - m1 - m2)*1000);
    s = pow((dW + 0.5)/1e3 + m1 + m2,2);
    pcms = sqrt((s - (m1+m2)*(m1+m2))*(s - (m1-m2)*(m1-m2))/4/s);
    kgamma = (s - targetmass*targetmass) / 2 / targetmass;
    weight /= pcms / kgamma * sqrt(s) / targetmass;
  }

  //--- Check if weight NaN --- Mihas fix on 29.5.2015 until we do not solve problem
  // with Maid. This happens 5 times every 5M events for pi0 simulation. This is the case at about 5MeV above the threshold for pi0 production, there the pi+ production is possible and this seems to cause numerical problems
  if (weight != weight){  //cout<<"Model name Name: "<<m->name<<" weight: "<<weight<<" E0: "<<E0<<"  Ep:  "<<Ep
    //<<"  theta: "<<theta<<"  Gamma: "<<Gamma<<"  q2: "<<q2<<" energy: "<<sqrt(s) - Reaction->threshold
    //			    <<" epsilon: "<<epsilon<<"  thetacms: "<<thetacms<<"  phicms: "<<phicms<<"  helicity: "<<helicity
    //			    <<" CS: "<<m->CrossSection(sqrt(s) - Reaction->threshold, q2, epsilon, thetacms, phicms, helicity)<<endl;
    //			    cout<<"... Returning zero and continuing to next event!"<<endl;
  			    return 0.0;
  }

  return weight;
}

///////////////////////////////////////////////////////////////////////////////

double 
generateTripleCMS::generateEvent(double helicity)
{ 
  Particle photon = P_gamma;
  photon          = Reaction->electronIn - Reaction->electronOut;
  FourVector CMS  = photon + *Reaction->getTarget();
  
  double 
    E0            = Reaction->electronIn.energy(),
    theta         = Reaction->electronOut.theta(),
    Ep            = Reaction->electronOut.energy(),
    s             = CMS.square(),
    q2            = photon.square(),
    epsilon       = 1/(1-2*(pow(photon.energy(),2)-q2)/q2*pow(tan(theta/2),2)),
    kgamma        = (s - targetmass*targetmass) / 2 / targetmass,
    solidangleCMS = (coscmsmax-coscmsmin) * (phicmsmax-phicmsmin);
  Gamma  = alpha/2/M_PI/M_PI * Ep/E0 * kgamma / -q2 / (1-epsilon);
  weight = Gamma;
  
  if (s < Reaction->threshold*Reaction->threshold) return 0; // below threshold
  
  double 
    W          = sqrt(s),
    m1         = Reaction->getM1()->getMass(),
    massdecay1 = Reaction->getM2()->getMass(),
    massdecay2 = Reaction->getM3()->getMass(),
    m_min      = massdecay1 + massdecay2,
    m_max      = W - m1;
  if (m_min < decay_m_min) m_min = decay_m_min;
  if (m_max > decay_m_max) m_max = decay_m_max;
  double 
    m2          = m_min + sobol() * (m_max - m_min),
    pcms        = p_cms(s, m1, m2),
    p_decay     = p_cms(m2*m2, massdecay1, massdecay2),
    thetacms    = acos(coscmsmin + sobol() * (coscmsmax-coscmsmin)),
    phicms      =      phicmsmin + sobol() * (phicmsmax-phicmsmin),
    thetaD      = acos(cosDmin + sobol() * (cosDmax-cosDmin)),
    phiD        = phiDmin + sobol() * (phiDmax-phiDmin),
    solidangleD = (cosDmax-cosDmin) * (phiDmax-phiDmin);
  
  FourVector Out2CMS = Polar(energy(m2,pcms), pcms, thetacms, M_PI+phicms);
  FourVector D1CMS   = Polar(energy(massdecay1, p_decay), p_decay,thetaD,phiD);

  Out2CMS.rot_theta(photon.theta());  
  Out2CMS.rot_phi(  photon.phi()  );
  Reaction->Out2 = Lorentz(Out2CMS, CMS);
  Reaction->Out1 = CMS - Reaction->Out2;
  D1CMS.rot_theta(Reaction->Out2.theta());
  D1CMS.rot_phi(Reaction->Out2.phi());
  Reaction->Decay1 = Lorentz(D1CMS, Reaction->Out2);
  Reaction->Decay2 = Reaction->Out2 - Reaction->Decay1;
    
  weight *= solidangleCMS
    * solidangleD       // Solidangle Decaying subsystem
    * (m_max - m_min)         // Mass range for Decaying subsystem
    * 1/(32*pow(2*M_PI,5) * targetmass * photon.momentum())  //phasespace
    * pcms * p_decay; 
  return weight;
}

///////////////////////////////////////////////////////////////////////////////

double 
generateResonance::generateEvent(double helicity)
{ 
  static double width = Reaction->getM2()->getWidth();
  static double mass = Reaction->getM2()->getMass();

  Particle photon = P_gamma;
  photon          = Reaction->electronIn - Reaction->electronOut;
  FourVector CMS  = photon + *Reaction->getTarget();
  
  double 
    E0            = Reaction->electronIn.energy(),
    theta         = Reaction->electronOut.theta(),
    Ep            = Reaction->electronOut.energy(),
    s             = CMS.square(),
    q2            = photon.square(),
    epsilon       = 1/(1-2*(pow(photon.energy(),2)-q2)/q2*pow(tan(theta/2),2)),
    kgamma        = (s - targetmass*targetmass) / 2 / targetmass,
    m1            = Reaction->getM1()->getMass(),
    m2            = 0,
    massdecay1    = Reaction->getM3()->getMass(),
    massdecay2    = Reaction->getM4()->getMass(),
    m_min         = massdecay1 + massdecay2,
    m_max         = sqrt(s) - m1;
  Gamma  = alpha/2/M_PI/M_PI * Ep/E0 * kgamma / -q2 / (1-epsilon);
  weight = Gamma;
  
  if (m_min < decay_m_min) m_min = decay_m_min;
  if (m_max > decay_m_max) m_max = decay_m_max;


  if (ModelType == PWIA || ModelType == BoundDelta) { 
    double fermiP     = pwave12C.transform(sobol()) / 1000;;
    double fermiTheta = acos(2*sobol() - 1);
    double fermiPhi   = - M_PI + 2*M_PI*sobol();
    Reaction->Out1.initPolar(energy(m1, fermiP), fermiP, fermiTheta, fermiPhi);
    if (ModelType==BoundDelta) Reaction->Out1 = Lorentz(Reaction->Out1, CMS);
    Reaction->Out2 = CMS - Reaction->Out1;
    m2   = sqrt(Reaction->Out2.square());
  } else if (Reaction->getM2()->getWidth()!=0) {  // Resonance Production
    m2 = m_min + sobol() * (m_max - m_min);
    Reaction->threshold = m1 + m2;
    double S = m2*m2;
    weight *= S*width*width/((S-mass*mass)*(S-mass*mass)+s*s*width*width);
  }
  
  if (s < Reaction->threshold*Reaction->threshold) return 0; // below threshold

  if (m2 < massdecay1+massdecay2) return 0; //below Proton + Pion mass threshold

  double
    pcms        = p_cms(s, m1, m2),
    p_decay     = p_cms(m2*m2, massdecay1, massdecay2),
    thetacms    = acos(coscmsmin + sobol() * (coscmsmax-coscmsmin)),
    phicms      =      phicmsmin + sobol() * (phicmsmax-phicmsmin),
    ct          = cosDmin + sobol() * (cosDmax-cosDmin),
    thetaD      = acos(ct),
    phiD        = phiDmin + sobol() * (phiDmax-phiDmin);
   
  if (ModelType != PWIA && ModelType != BoundDelta) { 
    FourVector Out2CMS = Polar(energy(m2,pcms), pcms, thetacms, M_PI+phicms);
    Out2CMS.rot_theta(photon.theta());  
    Out2CMS.rot_phi(  photon.phi()  );
    Reaction->Out2 = Lorentz(Out2CMS, CMS);
    Reaction->Out1 = CMS - Reaction->Out2;
  }
  
  FourVector D1CMS   = Polar(energy(massdecay1, p_decay), p_decay,thetaD,phiD);
  D1CMS.rotateTo(Reaction->Out2); //rotate D1CMS to Delta system
  Reaction->Decay1 = Lorentz(D1CMS, Reaction->Out2); //boost D1CMS to system of Delta
  Reaction->Decay2 = Reaction->Out2 - Reaction->Decay1;
  
  if (*Reaction->getM2() == P_Delta0) // Delta Decay Distribution
    weight *= (5 - 3*ct*ct - epsilon * 3 * (1 - ct*ct) * cos(2*phiD))/8
      * p_decay;       // decay phase space
  
  Reaction->getA()->setSpin(Momentum(0,0,0));
  return weight;
}


///////////////////////////////////////////////////////////////////////////////
//
//
//void transform(reaction *r, double in[8], double out[8])
//{ 
//  double 
//   E0 = r->electronIn.energy(),
//    m0 = r->getTarget()->getMass(),
//    mm = r->Out1.getMass(),
//    m1 = r->Decay1.getMass(),
//    m2 = r->Decay2.getMass(),
//    Eout = Eprime(E0, m0, mm, in[0], in[1], m1, in[2], in[3], in[4], 
//		  m2, in[5], in[6], in[7]);
//
//  FourVector 
//    Target(m0),
//    ein(energy(m_electron,E0), 0, 0, E0),
//    eout  = Polar(energy(m_electron, Eout ), Eout , in[0], in[1]),
//    out1  = Polar(energy(m1,         in[2]), in[2], in[3], in[4]),
//    out2  = Polar(energy(m2,         in[5]), in[5], in[6], in[7]),
//    CMS   = eout - ein + Target,
//    out12 = out1 + out2,
//    out12CMS = rotate(Lorentz(out12,  -CMS), -CMS),
//    out2CMS  = rotate(Lorentz(out2, -out12), -CMS);
//  out[0] = Eout;                  // E'
//  out[1] = in[0];                 // cos(theta_e)
//  out[2] = in[1];                 // phi_e
//  out[3] = out12*out12;           // dm_12 ^2
//  out[4] = cos(out12CMS.theta()); // CMS Production angles cos(theta_CMS)
//  out[5] = out12CMS.phi();        //                       phi_CMS
//  out[6] = cos(out2CMS.theta());  // 1-2-Decay angles      cos(theta_DECAY)
//  out[7] = out2CMS.phi();         //                       phi_DECAY
//}
//
//double Jacobi(reaction *r, double in[8]) 
//{
//  const double epsilon = 1e-8;
//  double m[8][8], x[8], y[8];
//  for (int i=0; i<8; i++) x[i]=in[i];
//  for(int i=0; i<8; i++) {
//    x[i] = in[i] + epsilon;
//    transform(r, x, y);
//    for (int j=0;j<8;j++) m[i][j] = y[i];
//    x[i] = in[i] - epsilon;
//    transform(r, x, y);
//    for (int j=0;j<8;j++) m[i][j] = (m[i][j] - y[i])/(2*epsilon);
//  }
//
//  double mm[8][8], ret=1;
//  for (int i=0; i<8; i++) for(int j=0;j<8;j++) mm[i][j] = m[i][j];
//  for (int s=0; s<8-1; s++) {
//    if (mm[s][s]==0) { cerr<< "Can't calculate Determinant" <<endl; exit(-1);}
//    for (int z=s+1; z<8; z++) {
//      double faktor = mm[z][s]/mm[s][s];
//      for(int i=0; i<8; i++) mm[z][i] -= mm[s][i] * faktor;
//    }
//  }
//  for (int s=0;s<8;s++) ret *= mm[s+8*s];
//  return ret;
//}
//
//
///////////////////////////////////////////////////////////////////////////////
He3eepnBase::He3eepnBase()
{
  ps=(getenv("GRID") == NULL);
  t1mode=(getenv("T1MODE") ==NULL)?0:atoi(getenv("T1MODE"));
}

void He3eepnBase::Photon_rotation(FourVector* Photon, FourVector* Proton,FourVector* Neutron)
{
  double Oopq = 0;
  if (Photon->phi() < 0) Oopq = Photon->phi() + 2 * M_PI; else Oopq = Photon->phi();
  Oopq -= M_PI;
  Photon->rot_phi(-Oopq); 
  Proton->rot_phi(-Oopq); //rotate proton & neutron to inplane-photon
  Neutron->rot_phi(-Oopq);
}

double 
He3eepnBase::T1max_nr(double q, double omega_, double thg,
		      double th1, double ph1, double th2, double ph2)
{
  // units are GeV and radian
  const double m     = 0.93904;
  const double Ebind = 0.007718;

  double omega = omega_ - Ebind;
  double trg1 = cos(th2)*cos(thg) + cos(ph2)*sin(th2)*sin(thg);
  double trg2 = cos(th1)*cos(thg) + cos(ph1)*sin(th1)*sin(thg);
  double trg3 = cos(th1)*cos(th2) + cos(ph1-ph2)*sin(th1)*sin(th2);
  double p2 = (-(q*(8*trg1 - 4*trg2*trg3))
               - sqrt(square(q)*square(8*trg1 - 4*trg2*trg3)
                      - 4*(-8 + 2*square(trg3))*(-2*square(q)*square(trg1)
						 + 2*square(q)*trg1*trg2*trg3
						 - square(q)*square(trg3)
						 + 2*m*square(trg3)*omega)))
    / (2.*(-8 + 2*square(trg3)));
  double p1 = 0.5*(q*trg2 - trg3*p2
                   + sqrt(square(-q*trg2 + trg3*p2)
                          - 2*(square(q) - 2*m*omega
                               - 2*q*trg1*p2 + 2*square(p2))));

  return 0.5*square(p1)/m;
}

void He3eepnBase::setup(double Ebeam, double omega, double q, double the1, double phi1, double the2, double phi2)
{
  Eb=Ebeam;  
  thee=2*asin(sqrt((q*q-omega*omega)/(4*Eb*(Eb-omega))));
  al=-asin((Eb-omega)*sin(thee)/q);
  AA=cos(al)*cos(the1)+cos(phi1)*sin(al)*sin(the1);
  BB=cos(al)*cos(the2)+cos(phi2)*sin(al)*sin(the2);
  CC=cos(the1)*cos(the2)+cos(phi1)*cos(phi2)*sin(the1)*sin(the2)+
    sin(phi1)*sin(phi2)*sin(the1)*sin(the2);
}

double He3eepnBase::calculate(double p1,double targetmass, double omega, double q)
{
  const double mp2=m_proton*m_proton;
  const double mn2=m_neutron*m_neutron;
  const double M=targetmass;
  // const double M2=M*M;
  double p12=p1*p1;
  double q2=q*q;
  double A1a=(CC*p1-BB*q);
  double BB1=BB*BB-1;
  double CC1=CC*CC-1;
  double ABC=AA-BB*CC;

  double A1=mn2*A1a+sqrt(-mn2*A1a*A1a*(CC1*p12-mp2+2*ABC*p1*q+BB1*q2));
  double A2=mp2-mn2-CC1*p12-2*p1*q*ABC+q2-BB*BB*q2;
  double A=A1*A1/(A2*A2);
  double B=2*CC*p1*A1/A2;
  double D=2*q*(AA*p1+BB*A1/A2);

  return sqrt(mp2+p12)+sqrt(mn2+A)+sqrt(mp2+p12+q2+B+A-D)-M-omega;
}

double He3eepnBase::bisec(double start1, double start2, int trace, double targetmass, double omega, double q)
{
  double  x1=start1;
  double  x2=start2;
  int iter=0;
  do{

    double a=calculate(x1,targetmass, omega, q);
    double b=calculate(x2,targetmass, omega, q);
    if (a==0) {x2=x1;break;}
    if (b==0) {x1=x2;break;}

    if (trace) {
      std::cerr<< x1<<"\t"<<x2<<"\t"<<a<<"\t"<<b<<std::endl;
    }
    if ((a*b)<0){
      double c=calculate((x1+x2)/2,targetmass,omega,q);
      if ((c*b)<0) x1=(x1+x2)/2; else x2=(x1+x2)/2;
    }
    else
      if (fabs(a)<fabs(b)) {
	x2=x1-(x2-x1)/10;
      } else { x1=x2+(x2-x1)/10;}

    iter++;
  } while ((fabs(x1-x2)>1e-5) && (iter < 100));
  if (iter==100) {std::cerr<<iter<<std::endl;return -1;}
  return (x1>x2?x1:x2);//(x1+x2)/2;
}

double He3eepnBase::getWeight(FourVector EIn, FourVector EOut,
			      FourVector Proton, FourVector Neutron,
			      double targetmass, gridinterpolation* CS, int match, double T1rel)
{
  FourVector Photon = EIn-EOut;
  Photon_rotation(&Photon,&Proton,&Neutron);
  double 
    E0      = EIn.energy(),
    Eprime  = EOut.energy(),
    the     = EOut.theta(),
    omega   = Photon.energy(),
    q       = Photon.momentum(),
    q2      = omega*omega-q*q,
    thq     = Photon.theta(),
    th1     = Proton.theta(),
    ph1     = Proton.phi(),
    th2     = Neutron.theta(),
    ph2     = Neutron.phi(),
    weight  = 0;
  if (T1rel==-1)
    T1rel   = (Proton.energy()-Proton.mass())*1000;


  double  T1match=T1rel;
  double  T1maxnr = T1max_nr(q, omega, -1.0*thq, th1, ph1, th2, ph2)*1000;
  if (match==1)
    
    { 
      setup(E0, omega, q, th1, ph1, th2, ph2);

      double low=sqrt((T1maxnr*0.0009+m_proton)*(T1maxnr*0.0009+m_proton)-m_proton*m_proton);
      double high=sqrt((T1maxnr*0.0011+m_proton)*(T1maxnr*0.0011+m_proton)-m_proton*m_proton);

      double   p1max    = bisec(low,high,0,targetmass,omega,q);
      double   T1maxrel = 1000*(sqrt(p1max*p1max+m_proton * m_proton)-m_proton);
      T1match  = T1rel/T1maxrel*T1maxnr;
      if (T1maxrel<0.9*T1maxnr)       cout <<"\n\n\nWARNING T1 "<<T1maxrel<<"\t"<<T1maxnr<<"\t"<<T1rel<<endl<<endl<<endl;
    }
  double  T1Grid   = (T1match-CS->getGridT1min())/(T1maxnr-CS->getGridT1min())*100;
  th1/=rad;
  ph1/=rad;
  th2/=rad;
  ph2/=rad;
  if (ph1 < 0) ph1 += 360;
  //  cout <<"th1:"<<th1<<" ph1:"<<ph1<<" th2:"<<th2<<" ph2:"<<ph2<<endl;
  //   if  ((T1match < CS->getGridT1min()) ||
  //       (T1Grid  >= (CS->getT1steps()-1)*CS->getT1stepsize()))
  //     cout <<"Missed because of T1:"<< T1match<<"\t"<<T1Grid<<"\t" << CS->getGridT1min()<<"\t"<<(CS->getT1steps()-1)*CS->getT1stepsize()<<endl;

  if (((th1 >= CS->getth1min()) && 
       (th1 <  CS->getth1max()) &&
       (ph1 >= CS->getph1min()) && 
       (ph1 <  CS->getph1max()) &&
       (th2 >= CS->getth2min()) && 
       (th2 <  CS->getth2max()) &&
       (ph2 >= CS->getph2min()) && 
       (ph2 <  CS->getph2max()) &&
       (T1match >= CS->getGridT1min()) &&
       (T1Grid  < (CS->getT1steps()-1)*CS->getT1stepsize())) || true)
    {
   if ((t1mode==0) ||( (t1mode==1) && (T1Grid<50)) ||((t1mode==2) && (T1Grid<97)) ||( (t1mode==3) && (T1Grid>97)) || true) 
      {
	if (ps)
	  {
	    double
	      s       = targetmass*targetmass + 2*targetmass*omega + omega*omega - q*q,
	      epsilon = 1 / (1 - 2 * q * q / q2 * tan(the/2) * tan(the/2)),
	      kgamma  = (s - targetmass*targetmass) / (2 * targetmass),
	      Gamma   = alpha/(2*M_PI*M_PI) * Eprime/E0 * kgamma/(-q2) * 1/(1-epsilon);
	    weight=Gamma;
	  }
	else 
	  {
	    weight=1e10*CS->getCS(th1,ph1,th2,ph2,T1Grid);
	  }
      }

}    else weight=0;

  //  cout <<"post weight"<<weight<<endl;
  if (weight <0)
    {
      cout <<"====================================================================="<<endl;
      cout <<"weight: "<< weight<< "\tT1Grid:"<<T1Grid<<endl;    
      cout <<th1<<"\t"<<ph1<<"\t"<<th2<<"\t"<<ph2<<endl;
    }


  return weight;
}

double 
generateTriple::Eprime(double E0, double m0, double mm, double the, double phe,
		       double m1, double p1, double th1, double ph1, 
		       double m2, double p2, double th2, double ph2)
{ 
  double E1  = energy(m1, p1);
  double E2  = energy(m2, p2);

  return // next lines generated with Mathematica
    -(1/(E0 - E1 - E2 + m0 - E0 * cos(the)+
	 p1 *(cos(the)*cos(th1) + cos(phe-ph1)*sin(the)*sin(th1))
	 +p2*(cos(the)*cos(th2) + cos(phe-ph2)*sin(the)*sin(th2)))*
      (-(E1+E2-m0)*(E1-2*E0+E2-m0)-2*E0*p1*cos(th1)+mm*mm+p1*p1+p2*p2+2*p2
       *((-E0+p1*cos(th1))*cos(th2)+p1*cos(ph1-ph2)*sin(th1)*sin(th2))))/2;
}

double 
generateTriple::Jacobian(double E0, double m0, double mm, 
			 double the, double phe, 
			 double m1, double p1, double th1, double ph1, 
			 double m2, double p2, double th2, double ph2)
{
  double E1  = energy(m1, p1);
  double E2  = energy(m2, p2);

  return // next lines generated with Mathematica   dE'/dp_2
    ((pow(E0 - E1 + m0 - E0*cos(the) + p1*cos(th1)*cos(the) + 
	  p2*cos(th2)*cos(the) - E2 + 
	  p1*cos(ph1)*cos(phe)*sin(th1)*sin(the) + 
	  p1*sin(ph1)*sin(phe)*sin(th1)*sin(the) + 
	  p2*cos(ph2)*cos(phe)*sin(th2)*sin(the) + 
	  p2*sin(ph2)*sin(phe)*sin(th2)*sin(the),-2)*
      ((-4*E0*m0 - 4*E0*p1*cos(th1) - p1*p2*cos(ph1 - ph2 - th1 - th2) + 
	2*p1*p2*cos(th1 - th2) + p1*p2*cos(ph1 - ph2 + th1 - th2) - 
	4*E0*p2*cos(th2) + p1*p2*cos(ph1 - ph2 - th1 + th2) + 
	2*p1*p2*cos(th1 + th2) - p1*p2*cos(ph1 - ph2 + th1 + th2) - 
	2*pow(E1,2) - 2*pow(m0,2) - 2*pow(m2,2) + 2*pow(mm,2) + 
	2*pow(p1,2) + 4*E1*(E0 + m0 - E2) + 4*E0*E2 + 4*m0*E2)*
       (cos(th2)*cos(the) - p2/E2 + cos(ph2 - phe)*sin(th2)*sin(the)) - 
       (p1*(-cos(ph1 - ph2 - th1 - th2) + 2*cos(th1 - th2) + 
	    cos(ph1 - ph2 + th1 - th2) + cos(ph1 - ph2 - th1 + th2) + 
	    2*cos(th1 + th2) - cos(ph1 - ph2 + th1 + th2)) - 
	4 / E2 * (-((E0 - E1 + m0)*p2) + E0*cos(th2)*E2))*
       (E0 - E1 + m0 - E0*cos(the) + p1*cos(th1)*cos(the) + 
	p2*cos(th2)*cos(the) - E2 + p1*cos(ph1)*cos(phe)*sin(th1)*sin(the) + 
	p1*sin(ph1)*sin(phe)*sin(th1)*sin(the) + 
	p2*cos(ph2)*cos(phe)*sin(th2)*sin(the) + 
	p2*sin(ph2)*sin(phe)*sin(th2)*sin(the))))/4);
}

void
generateTriple::Transform(double in[8], double out[8])
{ 
  double 
    E0 = Reaction->electronIn.energy(),
    m0 = Reaction->getTarget()->getMass(),
    mm = Reaction->Out1.getMass() + Ex,
    m1 = Reaction->Decay1.getMass(),
    m2 = Reaction->Decay2.getMass(),
    Eout = Eprime(E0, m0, mm, in[0], in[1], m1, in[2], in[3], in[4], 
		  m2, in[5], in[6], in[7]),
    T1 = energy(m1, in[2]) - m1;

  out[0] = in[0];                 // cos(theta_e)
  out[1] = in[1];                 // phi_e
  out[2] = T1;                    // T1
  out[3] = in[3];                 // cos(theta_1)
  out[4] = in[4];                 // phi_1
  out[5] = Eout;                  // E'
  out[6] = in[6];                 // cos(theta_2)
  out[7] = in[7];                 // phi_2
}

double
generateTriple::JacobiDet(double the, double phie, 
			  double p1, double th1, double phi1,
			  double p2, double th2, double phi2) 
{
  const double epsilon = 1e-8;
  double m[8][8], x[8], y[8], in[8];
  in[0] = the; in[1] = phie;
  in[2] = p1; in[3] = th1; in[4] = phi1;
  in[5] = p2; in[6] = th2; in[7] = phi2;

  for (int i=0; i<8; i++) x[i]=in[i];
  for(int i=0; i<8; i++) {
    double temp = x[i] = in[i] + epsilon;
    Transform(x, y);
    for (int j=0;j<8;j++) m[i][j] = y[j];
    temp -= x[i] = in[i] - epsilon;
    Transform(x, y);
    for (int j=0;j<8;j++) m[i][j] = (m[i][j] - y[j])/temp;
  }

  double mm[8][8], ret=1;
  for (int i=0; i<8; i++) for(int j=0;j<8;j++) mm[i][j] = m[i][j];
  for (int s=0; s<8-1; s++) {
    if (mm[s][s]==0) { cerr<< "Can't calculate Determinant" <<endl; exit(-1);}
    for (int z=s+1; z<8; z++) {
      double faktor = mm[z][s]/mm[s][s];
      for(int i=0; i<8; i++) mm[z][i] -= mm[s][i] * faktor;
    }
  }
  for (int s=0;s<8;s++) ret *= mm[s][s];
  return ret;
}

const int tof_cs        = (getenv("TOF_CS") ? atoi(getenv("TOF_CS")) : 0);

generateTriple::generateTriple(reaction *r, simDetectorBase *Sime, 
			       simDetectorBase *SimD1, simDetectorBase *SimD2,
			       SIMUL *rundb, modeltype Model) 
{ 
  Reaction   = r; sime=Sime; simD1=SimD1; simD2=SimD2;
  targetmass = Reaction->getTarget()->getMass();
  dcte       = sin(sime->getDtheta());
  p1_0 = simD1->getMomentum();    p2_0  = simD2->getMomentum();
  dcte = sin(sime->getDtheta());  dphie = sime->getDphi();
  dct1 = sin(simD1->getDtheta()); dphi1 = simD1->getDphi();
  dp1 = simD1->getDmomentum();
  dct2 = sin(simD2->getDtheta()); dphi2 = simD2->getDphi();
  dp2 = simD2->getDmomentum();
  Ex   = rundb->excitation;

  ModelType = Model;
  switch(ModelType) {
  case TripleLabPP : 
    Label = "dE'dT_1!d^6![W]_e,1,2!";
    Unit  = "(GeV/c)^2![\\327]sr^3!";
    break;

  case TripleLabHe3 : 
    Label = "d[s]";
    Unit = "[m]b";
    CrossSection = new gridinterpolation;
    phasespace = (getenv("GRID") == NULL);
    break;

  case TripleLab   : 
    if (tof_cs == 1) {
      Label = "dp_e'!dp_1!d[W]_e'!d[W]_1!d[W]_2!";
      Unit  = "(GeV/c)^2![\\327]sr^3!"; 
    } else {
      Label = "dp_1!d[W]_1!d[W]_2!";
      Unit  = "GeV/c[\\327]sr^2!";
    }
    break;

  default          : 
    Label = "dp_1!d[W]_1!d[W]_2!";
    Unit = "GeV/c[\\327]sr^2!";
  }
}

double 
generateTriple::generateEvent(double helicity)
{
  double 
    E0 = Reaction->electronIn.energy(),
    p1 = p1_0 + (2*sobol()-1)*dp1,
    p2 = p2_0 + (2*sobol()-1)*dp2;   

  generateLabAngles(&Reaction->Decay1,      p1, 
		    simD1->getAngle(), simD1->getOop(), dct1, dphi1); 
  generateLabAngles(&Reaction->Decay2,      p2, 
		    simD2->getAngle(), simD2->getOop(), dct2, dphi2); 
  generateLabAngles(&Reaction->electronOut, 1,  
		    sime->getAngle(),  sime->getOop(), dcte, dphie); 
  
  double 
    the = Reaction->electronOut.theta(), phie = Reaction->electronOut.phi(),
    th1 = Reaction->Decay1.theta(),      phi1 = Reaction->Decay1.phi(),
    th2 = Reaction->Decay2.theta(),      phi2 = Reaction->Decay2.phi(),
    m0  = targetmass,
    mm  = Reaction->Out1.getMass() + Ex,
    m1  = Reaction->Decay1.getMass(),
    m2  = Reaction->Decay2.getMass(),
    Eout = Eprime(E0, m0, mm, the, phie, m1, p1, th1, phi1, m2, p2, th2, phi2);
  
  Reaction->electronOut.initPolar(Eout, Eout, the, phie); 

  double Jacobi = Jacobian(E0,m0,mm,the,phie,m1,p1,th1,phi1,m2,p2,th2,phi2);

  weight = 
    // INtegration volume is taken form member function integrationVolume()
    //8 * dct1 * dphi1 * dp1 *    // d\Omega_1 dp_1
    //8 * dct2 * dphi2 * dp2 *    // d\Omega_2 dp_2
    //4 * dcte * dphie *          // d\Omega_e 
    fabs(Jacobi);               // dE'/dp_2
  double q2=0;
  if (ModelType == TripleLab || ((ModelType == TripleLabHe3) && phasespace)) {
    q2      = - 4 * E0 * Eout * pow(sin(the / 2),2);
    double
      s       = m0*m0 + q2 + 2 * m0 * (E0 - Eout),
      epsilon = 1/(1 - 2*(pow(E0 - Eout,2) - q2) / q2 * pow(tan(the/2),2)),
      kgamma  = (s - m0*m0) / 2 / m0;
    Gamma   = alpha/2/M_PI/M_PI * Eout/E0 * kgamma / -q2 / (1-epsilon);
  }

  FourVector GridEIn, GridEOut, GridProton, GridNeutron;

  switch(ModelType) {
  case TripleLabPP: 
    weight *= fabs(p1/energy(m1, p1));
    break;

  case TripleLab:     
    if (tof_cs == 1) {
      weight *= 1;            //
    } else {  
      weight *= Gamma;        // virtual Photon Flux = d\Omega_e dE' \Gamma
    }
    break;

  case TripleLabHe3: 
    GridEIn    = (FourVector)(Reaction->electronIn);
    GridEOut   = (FourVector)(Reaction->electronOut);
    GridProton = (FourVector)(Reaction->Decay1);
    GridNeutron = (FourVector)(Reaction->Decay2);
    weight *= fabs(p1/energy(m1, p1));
    weight *= getWeight(GridEIn,GridEOut,GridProton,GridNeutron,targetmass,CrossSection,1,-1);
    break;

  default: 
    weight *= fabs(p1/energy(m1, p1));
  }

  return weight;
}

generateHe3eepn::generateHe3eepn(reaction *r, simDetectorBase *Sime, 
				 simDetectorBase *SimD1, simDetectorBase *SimD2,
				 SIMUL *rundb, modeltype Model)
{
  CrossSection = new gridinterpolation;
  omega_0    = CrossSection->getomega()/1000;
  q_0        = CrossSection->getq()/1000;
  m = 0.93904;
  Ebind = 0.007718;
  //#define MOD_DEBUG 1
  Reaction   = r; 
  sime=Sime; simD1=SimD1; simD2=SimD2;
  targetmass = Reaction->getTarget()->getMass();
  dphie = sime->getDphi()/sin(sime->getAngle() - sime->getDtheta());

  Target.initPolar(targetmass,0,0,0);

  p1_0 = simD1->getMomentum(); dp1 = simD1->getDmomentum();
  dct1 = sin(simD1->getDtheta()); dphi1 = simD1->getDphi();

  dct2 = sin(simD2->getDtheta()); dphi2 = simD2->getDphi();
  dp2 = simD2->getDmomentum();
  Ex   = rundb->excitation;
  if (getenv("DOMEGA") == NULL) domega=0.001;
  else domega = atof(getenv("DOMEGA"))/1000;
  if (getenv("DQ") == NULL) dq=0.001;
  else dq = atof(getenv("DQ"))/1000;
  std::cout << "Selected q-omega-range:\ndomega="<<domega*1000<<"MeV, dq="<<dq*1000<<"MeV/c\n"; 
  counter = 0;

  phasespace = (getenv("GRID") == NULL); //(getenv("He3eepnGRID") == NULL);

  if (phasespace) 
    {
      Label = "d[W]_1!d[W]_2!dT_1!";
      Unit = "GeV sr^2!";
      std::cout << "Calculating phasespace!" << std::endl;
    }
  else
    {
      Label = "d[s]";
      Unit = "[m]b";
      std::cout << "Calculating He3-model!" << std::endl;
    };

}

double generateHe3eepn::ftest(double w, double p1, double p2, double p3)
{
  return 2*m*(w-Ebind)-square(p1)-square(p2)-square(p3);
}

double generateHe3eepn::fp1guess(double C, double D, double E, double p3)
{
  return (D+sqrt(square(D)-4*E*(C+square(p3))))/2/E;
}

double generateHe3eepn::fp2root(double C, double D, double E, double p1, double p3)
{
  return C-D*p1+E*square(p1)+square(p3);
}

int generateHe3eepn::tstsort(double *p1, double *tst, int n, int usefabs)
{
  int i, j;

  for (i=(n-1);i>0;i--) {
    for (j=0;j<i;j++) {
      /* printf("i=%d j=%d\n",i,j); */
      int swap = (usefabs ? fabs(tst[j])>fabs(tst[j+1]) : tst[j]>tst[j+1]);
      if (swap) {
	double p, t;
	p = p1[j]; t = tst[j];
	p1[j] = p1[j+1]; tst[j] = tst[j+1];
	p1[j+1] = p; tst[j+1] = t;
      }
    }
  }

  return 0;
}

double generateHe3eepn::fp2(double A, double B, double SQRT, double p1)
{
  return A-B*p1+SQRT;
}

double generateHe3eepn::fp2test(double w, double A, double B, double SQRT, double p1, double p3)
{
  double p2p, p2m, tstp, tstm;

  p2p = fp2(A, B, SQRT, p1);
  p2m = fp2(A, B, -SQRT, p1);
  tstp = ftest(w, p1, p2p, p3);
  tstm = ftest(w, p1, p2m, p3);
  if ((p2p<0)||((p2m>0)&&(fabs(tstm)<fabs(tstp))))
    return tstm;

  return tstp;
}

double generateHe3eepn::cap(double x)
{
  const double c = 1.5;
  if (x>c) return c;
  if (x<-c) return -c;
  return x;
}
double generateHe3eepn::p3toT1(double q, double w, double thg,
			       double th1, double ph1, double th2, double ph2,
			       double p3, double T1guess, double p2guess)
{
  double A = q*(cos(th2)*cos(thg) + cos(ph2)*sin(th2)*sin(thg));
  double B = cos(th1)*cos(th2) + cos(ph1-ph2)*sin(th1)*sin(th2);
  double C = square(q)*(-1 + square(cos(th2)*cos(thg))
			+ square(cos(ph2)*sin(th2)*sin(thg))
			+ 0.5*cos(ph2)*sin(2*th2)*sin(2*thg));
  double D = q*(cos(th1)*(-2*cos(thg)*square(sin(th2))
			  + cos(ph2)*sin(2*th2)*sin(thg)) +
		sin(th1)*(-2*cos(ph1)*sin(thg) + cos(ph1 - ph2)
			  *(cos(thg)*sin(2*th2)
			    +2*cos(ph2)*square(sin(th2))*sin(thg))));
  double E = -1 + square(cos(th1)*cos(th2))
    + square(cos(ph1 - ph2)*sin(th1)*sin(th2))
    + 0.5*cos(ph1 - ph2)*sin(2*th1)*sin(2*th2);

  /*
   * test if start values give negative root or negative test function
   */

  double Mq, MA, MB, MC, MR;
  double p1[8], R, tst[8];
  double limit[6] = {-1.0, -1.0, -1.0, -0.15, -0.15, -0.15};
  int n, ii, loop;
#ifdef MOD_DEBUG
  int i;
#endif /* MOD_DEBUG */
  double f = 1;
  n = ii = 0;
  do {
    p1[ii] = sqrt(2*m*T1guess*f);
    R = fp2root(C, D, E, p1[ii], p3);
    if (R>0) {
      if ((tst[ii] = fp2test(w, A, B, sqrt(R), p1[ii], p3))>limit[ii]) {
#ifdef MOD_DEBUG
	printf("%2d/%2d: T1=%f\tp1=%f\tp3=%f\tR=%f\ttst=%f\n",
	       ii,n,f*T1guess,p1[ii],p3,R,tst[ii]);
#endif /* MOD_DEBUG */
	ii++;
      }
    }
    f = (f>1 ? 0.995/f : 1.005/f);
    n++;
  } while (n<30 && ii<6);

  /* special case where the characteristic root in the p2 formular is zero */
  p1[ii] = fp1guess(C, D, E, p3);
  R = fp2root(C, D, E, p1[ii], p3);
  if (R>=0 && p1[ii]>0) {
    tst[ii] = fp2test(w, A, B, sqrt(R), p1[ii], p3);
#ifdef MOD_DEBUG
    printf("%2d/%2d: T1=%f\tp1=%f\tp3=%f\tR=%f\ttst=%f\n",
	   ii,n,f*T1guess,p1[ii],p3,R,tst[ii]);
#endif /* MOD_DEBUG */
    ii++;
  }
  tstsort(p1, tst, ii, 1);
#ifdef MOD_DEBUG
  for (i=0;i<ii;i++) printf("%d: p1=%f\ttst=%f\n",i,p1[i],tst[i]);
#endif /* MOD_DEBUG */

  loop = 6;
  while (--loop && (ii>2) && (fabs(tst[0])>1E-5)) {
#ifdef MOD_DEBUG
    printf("loop %d: p1=%f\ttst=%f\n",loop,p1[0],fabs(tst[0]));
#endif /* MOD_DEBUG */

#ifdef NEVER
    if (tst[0]*tst[1] < 0) {
      /* different sign - simple root finding */
      p1[3] = (p1[1]*tst[0]-p1[0]*tst[1])/(tst[0]-tst[1]);
#else
      p1[3] = p1[0]+(p1[0]-p1[1])*cap(tst[0]/(tst[1]-tst[0]));
#endif
      R = fp2root(C, D, E, p1[3], p3);
      if (R<0) break;
      tst[3] = fp2test(w, A, B, sqrt(R), p1[3], p3);
#ifdef MOD_DEBUG
      for (i=3;i<4;i++) printf("%d# p1=%f\ttst=%f\n",i,p1[i],tst[i]);
#endif /* MOD_DEBUG */
      tstsort(p1, tst, 4, 1);
#ifdef MOD_DEBUG
      for (i=0;i<4;i++) printf("%d# p1=%f\ttst=%f\n",i,p1[i],tst[i]);
#endif /* MOD_DEBUG */
#ifdef NEVER
    }
#endif

    /* Muller's Method - Numerical Recipes p.371 */
    Mq = (p1[0]-p1[1])/(p1[1]-p1[2]);
    MA = Mq*tst[0]-Mq*(1+Mq)*tst[1]+square(Mq)*tst[2];
    MB = (2*Mq+1)*tst[0]-square(1+q)*tst[1]+square(q)*tst[2];
    MC = (1+Mq)*tst[0];
    MR = square(MB)-4.0*MA*MC;
#ifdef MOD_DEBUG
    printf("q=%f\tA=%f\tB=%f\tC=%f\tR=%f\n",Mq,MA,MB,MC,MR);
#endif /* MOD_DEBUG */
    if (MR>0) {
      int j = 3;
      p1[j] = p1[0]-(p1[0]-p1[1])*cap((2.0*MC)/(MB+sqrt(MR)));
      R = fp2root(C, D, E, p1[j], p3);
      if ((p1[j]>0) && (R>=0)) {
	tst[j] = fp2test(w, A, B, sqrt(R), p1[j], p3);
	j++;
      }
      p1[j] = p1[0]-(p1[0]-p1[1])*cap((2.0*MC)/(MB-sqrt(MR)));
      R = fp2root(C, D, E, p1[j], p3);
      if ((p1[j]>0) && (R>=0)) {
	tst[j] = fp2test(w, A, B, sqrt(R), p1[j], p3);
	j++;
      }
#ifdef MOD_DEBUG
      for (i=3;i<j;i++) printf("%d> p1=%f\ttst=%f\n",i,p1[i],tst[i]);
#endif /* MOD_DEBUG */
      tstsort(p1, tst, j, 1);
#ifdef MOD_DEBUG
      for (i=0;i<j;i++) printf("%d> p1=%f\ttst=%f\n",i,p1[i],tst[i]);
#endif /* MOD_DEBUG */
    } else break;
  }

#ifdef MOD_DEBUG
  printf("R: p1=%f\ttst=%f\n",p1[0],tst[0]);
#endif /* MOD_DEBUG */
  R = 0.5*p1[0]*p1[0]/m;
  return (fabs(tst[0])<0.05 ? R : -R);
}



double 
generateHe3eepn::generateEvent(double helicity)
{

  double E0,the,phe,p1,q2,Eprime;
  omega = omega_0 + (2 * sobol() - 1) * domega; //generate in omega with full width of 2MeV
  q = q_0 + (2 * sobol() - 1) * dq; //generate in q with full width of 2MeV
  E0 = Reaction->electronIn.energy(),
    Eprime = E0 - omega;
  if (Eprime < 0) return 0;
  q2 = omega * omega - q * q;
  the =  2 * asin(sqrt(-q2 / (4 * E0 * Eprime)));
  phe = (2 * sobol() - 1) * dphie;
  p1 = p1_0 + (2*sobol()-1)*dp1;



  Reaction->electronOut.initPolar(Eprime, Eprime, the, phe); 
  generateLabAngles(&Reaction->Decay1,      p1, 
		    simD1->getAngle(), simD1->getOop(), dct1, dphi1); //generate proton
  generateLabAngles(&Reaction->Decay2,      1, 
		    simD2->getAngle(), simD2->getOop(), dct2, dphi2); //generate neutron angles

  //Calculate neutron energy
  FourVector Photon = (FourVector)(Reaction->electronIn) - (FourVector)(Reaction->electronOut);

  FourVector Deuteron = Photon + Target - (FourVector)(Reaction->Decay1);

  //boost deuteron to its own restframe
  FourVector DeuteronCMS = Deuteron.Lorentz(-Deuteron); 
  double EdCMS = DeuteronCMS.energy();

  //Neutrons energy in deuteron restframe
  double EnCMS = (EdCMS * EdCMS - m_proton * m_proton + m_neutron * m_neutron) / (2 * EdCMS);

  FourVector Neutron = (FourVector)(Reaction->Decay2);
  Neutron.rot_phi(-Deuteron.phi());
  Neutron.rot_theta(-Deuteron.theta());
  double thn = Neutron.theta();
  double Ed = Deuteron.energy();
  double Pd = Deuteron.momentum();

  //Energy of neutron in lab frame
  //following lines are calculated with mathematica
  double En=0;
  if (Ed*Ed*(2*(EnCMS - m_neutron)*(EnCMS + m_neutron)*Ed*Ed + cos(2*thn)*m_neutron*m_neutron*Pd*Pd + 
	     (-2*EnCMS*EnCMS + m_neutron*m_neutron)*Pd*Pd)<0) 
    /*
      En = -(1/(Ed*(-2*Ed*Ed + Pd*Pd + cos(2*thn)*Pd*Pd))
      *(2*EnCMS*Ed*Ed*Ed*sqrt(1 - 1/(Ed*Ed)*Pd*Pd)));
      */
    return 0;
  else 
    En = -(1/(Ed*(-2*Ed*Ed + Pd*Pd + cos(2*thn)*Pd*Pd))*
	   (2*EnCMS*Ed*Ed*Ed*sqrt(1 - 1/(Ed*Ed)*Pd*Pd) + Pd*cos(thn)*sqrt(2)*
	    sqrt(Ed*Ed*(2*(EnCMS - m_neutron)*(EnCMS + m_neutron)*Ed*Ed 
			+ cos(2*thn)*m_neutron*m_neutron*Pd*Pd + 
			(-2*EnCMS*EnCMS + m_neutron*m_neutron)*Pd*Pd))));

  if (En < m_neutron) return 0; //check for nan
  double Pn = sqrt(En * En - m_neutron * m_neutron);
  thn = Reaction->Decay2.theta();
  double phn = Reaction->Decay2.phi();
  //  double thp = Reaction->Decay1.theta();
  //  double php = Reaction->Decay1.phi();

  Reaction->Decay2.initPolar(En, Pn, thn, phn);

  //   cout <<"Thetaproton"<<Reaction->Decay1.theta()*180/3.14152<<endl;
  //   cout <<"Tproton:"<<(Reaction->Decay1.energy()-m_proton)*1000<<endl;
  //   cout <<"Eneutron:"<<En<<endl;


  
  //FourVector Proton2 = Deuteron - (FourVector)(Reaction->Decay2);

  //double Oopq = 0;
  //if (Photon.phi() < 0) Oopq = Photon.phi() + 2 * M_PI; else Oopq = Photon.phi();
  //Oopq -= M_PI;

  //Jacobian dEprime dthe dEp / (domega dq dpp)
  double Jacobian = 1. / sqrt(1.-(q*q-omega*omega)/(4.*E0*(E0-omega))) * 1. / sqrt((q*q-omega*omega)/(4.*E0*(E0-omega)))
    * q / (2*E0*(E0-omega)) * sin(the)
    * Reaction->Decay1.momentum()/Reaction->Decay1.energy();
  weight = fabs(Jacobian);



  FourVector GridEIn     = (FourVector)(Reaction->electronIn);
  FourVector GridEOut    = (FourVector)(Reaction->electronOut);
  FourVector GridProton  = (FourVector)(Reaction->Decay1);
  FourVector GridNeutron = (FourVector)(Reaction->Decay2);

  weight *= getWeight(GridEIn,GridEOut,GridProton,GridNeutron,targetmass,CrossSection,1,-1);
  

  return weight;
}

double 
generateHe3eepn::debugOutput()
{
  FourVector Ein  = (FourVector)(Reaction->electronIn);
  FourVector Eout = (FourVector)(Reaction->electronOut);
  FourVector Photon = Ein - Eout;
  FourVector Proton = (FourVector)(Reaction->Decay1);
  FourVector Neutron = (FourVector)(Reaction->Decay2);
  FourVector Proton2=Target+Photon-Proton-Neutron;
  double Oopq = 0;
  if (Photon.phi() < 0) Oopq = Photon.phi() + 2 * M_PI; else Oopq = Photon.phi();
  Oopq -= M_PI;
  FourVector GridPhoton = Photon;
  FourVector GridProton = (FourVector)(Reaction->Decay1);
  FourVector GridNeutron = (FourVector)(Reaction->Decay2);
  GridPhoton.rot_phi(-Oopq); 
  GridProton.rot_phi(-Oopq); //rotate proton & neutron to inplane-photon
  GridNeutron.rot_phi(-Oopq);
  double T1nr=p3toT1(Photon.momentum(), Photon.energy(), -1.0*Photon.theta(), 
		     GridProton.theta(), GridProton.phi(),
		     GridNeutron.theta(), GridNeutron.phi(),
		     Proton2.momentum(), GridProton.Ekin(), GridNeutron.momentum())*1000;

  cout << "Photon(Grid):\nomega=" << Photon.energy() << "\tq=" << Photon.momentum() 
       << "\tthq=" << Photon.theta()/rad << "\tphq=" << GridPhoton.phi()/rad  
       << "\nProton1(Grid):\n th1=" << Proton.theta()/rad << "\tph1=" << Proton.phi()/rad 
       << "\tT1rel=" << Proton.Ekin() << "T1nr=" << T1nr
       << "\nNeutron(Grid):\n th2=" << Neutron.theta()/rad << "\tph2=" << Neutron.phi() 
       << "\tp_2=" << GridNeutron.momentum() << "\tT_2=" << GridNeutron.Ekin();
  return 4711; // why return value?
}

// RB Add-Ons following, should be repackaged some day ...


// ============================================================================

//  Universal two body decay routine for a --> c + d.
//  Example: calculate proton momentum in D(e,e'p)n:
//    a = gamma + target, c = proton, d = neutron
//
//  This routine solves the problem
//          (e_a,0,0,p_a) = (e_c, p_c) + (e_d, p_d),
//     with e_c^2 - e_c^2 = m_c^2, e_d^2 - e_d^2 = m_d^2,
//  if e_a, p_a and th_ac are given (e_a and p_a are a priori
//  independent from each other)
//
//                        (e_c,p_c), mass m_c
//                          /
//                         /
//     (e_a,0,0,p_a)      / th_ac
//     ----------------->*----------> z-axis
//                        '
//                         '
//                       (e_d,p_d), mass m_d
//
//  Be aware that there might be two solutions, depending on the kinematics!
//
//  The caller has to provide e_a, p_a, cos(th_ac), m_c and m_d.
//
//  The routine returns the following solution types:
//    1: One solution, e_c_1, p_c_1
//    2: One solution, e_c_2, p_c_2
//    3: Two solutions, both e_c_1, p_c_1 and e_c_2, p_c_2 are valid.
//  The kinematic variables
//  *) e_cm  = Lorentz-invariant of (e_a, p_a),
//  *) e_c_cm, p_c_cm  = energy and momentum of 'c' in the 'a' rest frame
//  *) e_c_1, p_c_1, e_c_2, p_c_2 = energies and momenta of 'c' in the
//     lab frame
//  are calculated appropriately.
//
//  If the reaction isn't possible, it returns:
//   -1: e_a < p_a  <==>  cm-invariant e_cm < 0
//   -2: e_cm < (m_c + m_d)
//   -3: Argument of sqrt is negative when solving the quadratic equation
//  The kinematic variables aren't necessarily well defined after
//  encountering such non-solutions.

int find_c(const double& e_a, const double& p_a, const double& cos_th_ac,
	   const double& m_c, const double& m_d,
	   double& e_cm, double& e_c_cm, double& p_c_cm,
	   double& e_c_1, double& p_c_1, double& e_c_2, double& p_c_2)
{
  if( e_a < p_a )
    return(-1);

  e_cm = sqrt(e_a*e_a - p_a*p_a);
  if( e_cm < (m_c + m_d) )
    return(-2);

  e_c_cm = (e_cm*e_cm + m_c*m_c - m_d*m_d) / (2 * e_cm);
  p_c_cm = sqrt(e_c_cm*e_c_cm - m_c*m_c);

  double p_a_par  = p_a * cos_th_ac;

  double f2 = e_a*e_a - p_a_par*p_a_par;
  double f1 = 2 * e_a * e_cm * e_c_cm;
  double f0 = e_cm*e_cm * e_c_cm*e_c_cm + m_c*m_c * p_a_par*p_a_par;
  double s2 = f1*f1 - 4*f0*f2;

  if( s2 < 0 )
    return(-3);

  //  quadratic equation might give two possible energies for particle C.
  double s = sqrt(s2);
  e_c_1 = (f1 + s)/(2.0*f2);
  e_c_2 = (f1 - s)/(2.0*f2);

  //  now find out which solutions are valid by verifying E-p-conservation
  double sin_th_ac = sqrt(1.0 - cos_th_ac*cos_th_ac);

  //  check solution #1
  double cmp_1;
  if( e_c_1 >= m_c ) {
    p_c_1 = sqrt(e_c_1*e_c_1 - m_c*m_c);

    double
      p_c_1_y =  p_c_1 * sin_th_ac,
      p_c_1_z =  p_c_1 * cos_th_ac,
      p_d_1_y = -p_c_1_y,
      p_d_1_z =  p_a - p_c_1_z,
      e_d_1   =  e_a - e_c_1;

    cmp_1 = sqrt(p_d_1_y*p_d_1_y + p_d_1_z*p_d_1_z + m_d*m_d) - e_d_1;
  }
  else {
    cerr << "find_c: supposedly impossible error #1 encountered, "
	 << " happy debugging! e_a = " << e_a << ", p_a = " << p_a
	 << ", cos_th_ac = " << cos_th_ac << "\n";
    exit(-255);
  }

  //  check solution #2
  double cmp_2;
  if( e_c_2 >= m_c ) {
    p_c_2 = sqrt(e_c_2*e_c_2 - m_c*m_c);

    double
      p_c_2_y =  p_c_2 * sin_th_ac,
      p_c_2_z =  p_c_2 * cos_th_ac,
      p_d_2_y = -p_c_2_y,
      p_d_2_z =  p_a - p_c_2_z,
      e_d_2   =  e_a - e_c_2;

    cmp_2 = sqrt(p_d_2_y*p_d_2_y + p_d_2_z*p_d_2_z + m_d*m_d) - e_d_2;
  }
  else
    cmp_2 = 1.0e99;     // just a huge value if solution #2 doesn't exist

  const double oom = 1e-6;
  if( zcomp(cmp_1, oom) && zcomp(cmp_2, oom) )
    return(3);                        //  both solutions 1 & 2 are valid
  else
    if( zcomp(cmp_1, oom) )
      return(1);                      //  only solution 1 is valid
    else
      if( zcomp(cmp_2, oom) )
	return(2);                    //  only solution 2 is valid
      else {
	cerr << "find_c: supposedly impossible error #2 encountered, "
	     << " happy debugging! e_a = " << e_a << ", p_a = " << p_a
	     << ", cos_th_ac = " << cos_th_ac << "\n";
	exit(-255);
      }
}


// ============================================================================

//  Compare float-type number with zero

int zcomp(const double &cmp, const double& oom)
{
  if( fabs(cmp) < oom )
    return 1;
  else
    return 0;
}

// ============================================================================

#ifdef NEVEREVEREVERNEVER
// -------------------  debug stuff start  -------------------

// Initialize manually for debugging purposes. Watch it: A zero polar
// angle for the incoming electron gives a floating point exception
// for some to me unknown reason!
double E_i     = 0.420109985;
double E_f     = 0.323821374;
double Theta_e = 1.29617344;
double Phi_e   = 0.0889228594;
double Theta_p = 0.666675345;
double Phi_p   = 3.21662035;

Reaction->electronIn.initPolar(energy(Reaction->electronIn.getMass(),
				      E_i), E_i, 0.00000001, 0.3);
Reaction->electronOut.initPolar(energy(Reaction->electronOut.getMass(),
				       E_f), E_f, Theta_e, Phi_e);
Reaction->Out1.initPolar(energy(Reaction->Out1.getMass(), 1.0),
			 1.0, Theta_p, Phi_p);

// aeexb and Simul++ have the sides of e and p changed
double phi_e_rot = Reaction->electronOut.phi();
if( phi_e_rot > 0 )
  phi_e_rot -= M_PI;
 else
   phi_e_rot += M_PI;

double phi_p_rot = Reaction->Out1.phi();
if( phi_p_rot > 0 )
  phi_p_rot -= M_PI;
 else
   phi_p_rot += M_PI;

// -------------------  debug stuff end  -------------------
#endif



generateHe3nr::generateHe3nr(reaction *r, simDetectorBase *Sime, 
			     simDetectorBase *SimD1, simDetectorBase *SimD2,
			     SIMUL *rundb, modeltype Model)
{


  // He3nrMODE: 1: Phasespace, 2: write out of events, 3: read back of cross sections
  // He3nrFILE: File for 2 and 3.

  Ebind = 0.007718;
     
  q_0=atof(getenv("He3nr_Q0"));
  dq=atof(getenv("He3nr_DQ"));
  omega_0=atof(getenv("He3nr_OMEGA0"));
  domega=atof(getenv("He3nr_DOMEGA"));

  if (getenv("He3nr_DT"))
    dt=atof(getenv("He3nr_DT"));
  else
    dt=M_PI/18000;

  if ((dq==0) || (domega==0))
    domdq=1;
  else
    domdq=4*dq*domega;

  std::cerr <<"4*dOmega*dQ ="<<domdq<<endl;

  if (getenv("He3nrMODE")==NULL)
    {
      std::cerr<<"Error: He3nr selected and He3nrMODE not set.\n 1: Phasespace\n 2: Dump of events for Golak Code\n 3: Load CrossSection\n";
      exit(-123);
    }


 

  dodE=false;
  if (getenv("He3nr_do_dE")&& atoi(getenv("He3nr_do_dE"))>0)
    {
      std::cerr<<"Warning: He3nr: Doing dE instead of dS!\n"<<std::endl;
      dodE=true;
    }

  
  He3HACK::mode=atoi(getenv("He3nrMODE"));
  switch (He3HACK::mode)
    {
    case -1:
      domdq/=4; //only generating in pi/2, not 2 Pi
      std::cout <<" Calculating relativistic Phasespace"<<std::endl;
    case 1:
      if (dodE)
	Label = "d[W]_1!d[W]_2!dE_1!";
      else
	Label = "d[W]_1!d[W]_2!dS!";

      Unit = "GeV sr^2!";
      std::cout << "Calculating phasespace!" << std::endl;
      break;
    case 2:case 3:
      if (getenv("He3nrFILE")==NULL)
	{
	  std::cerr<<"Error: He3nr selected with MODE 2 or 3 and  He3nrFILE not set.\n";
	  exit(-124); 
	}
      if (He3HACK::mode==2)
	He3HACK::listfile.open(getenv("He3nrFILE"));
      else
	He3HACK::csfile.open(getenv("He3nrFILE"));
      Label = "d[s]";
      Unit = "[m]b";
      std::cout << "Calculating He3-model!" << std::endl;
      break;
    default:
      std::cerr<<"Error: He3nr selected and He3nrMODE invalid.\n 1: Phasespace\n 2: Dump of events for Golak Code 3: Load CrossSection\n";
      exit(-125);
      break;
    }; 
  Reaction   = r;   
  sime=Sime; simD1=SimD1; simD2=SimD2;
  dphie = sime->getDphi()/sin(sime->getAngle() - sime->getDtheta());

  dcte = sin(sime->getDtheta()); 
  dct1 = sin(simD1->getDtheta()); dphi1 = simD1->getDphi();
  dct2 = sin(simD2->getDtheta()); dphi2 = simD2->getDphi();

  M=0.939038968336267; //from bf11.f
  std::cout << "Starting up" << std::endl;
}

void generateHe3nr::setupbisec(double t,double w,double qx_,double qy_,double qz_,
			       double p1x,double p1y,double p1z,
			       double p2x,double p2y,double p2z)
{
  consta=-(m_He3+w);
  calpha=cos(t);
  salpha=sin(t);
  cpx=p1x*calpha+p2x*salpha;
  cpy=p1y*calpha+p2y*salpha;
  cpz=p1z*calpha+p2z*salpha;
  qx=qx_;qy=qy_;qz=qz_;
   

}

double generateHe3nr::bisecfunc(double l)
{
  double cl=calpha*l;
  double sl=salpha*l;
  return  consta+sqrt(m_proton*m_proton+cl*cl)+sqrt(m_neutron*m_neutron+sl*sl)
    +sqrt(m_proton*m_proton+sqr(l*cpx-qx)+sqr(l*cpy-qy)+sqr(l*cpz-qz));

}

double generateHe3nr::bisec(double a,double b)
{

  double  r1=bisecfunc(a);
  double  r2=bisecfunc(b);
  if (r1*r2>0){
    std::cerr<<" Warning, no zero crossing! "<<r1<<"  "<<r2<<std::endl;
    return -1;
  }
  while(fabs(r1-r2)>1e-10)
    {
      double c=(a+b)/2;
      double r=bisecfunc(c);
      if (r*r1>0)
	{
	  r1=r;
	  a=c;
	}
      else
	{
	  r2=r;
	  b=c;
	}
         
    }
  return (a+b)/2;

}




double  generateHe3nr::generateEvent(double helicity)
{
 
  if (He3HACK::mode<3){
    
    double omega = omega_0 + (2 * sobol() - 1) * domega; 
    double q = q_0 + (2 * sobol() - 1) * dq;
    
    double  
      E0 = Reaction->electronIn.energy(),
      Eprime = E0 - omega;
    if (Eprime < 0) return 0;
    double
      q2 = omega * omega - q * q,
      the =  2 * asin(sqrt(-q2 / (4 * E0 * Eprime))), //calculate electron angles
      phe = (2 * sobol() - 1) * dphie;


    //    the=0.415123; phe=0;

    Reaction->electronOut.initPolar(Eprime, Eprime, the, phe); 
    generateLabAngles(&Reaction->Decay1,      1, 
		      simD1->getAngle(), simD1->getOop(), dct1, dphi1); //generate proton
    generateLabAngles(&Reaction->Decay2,      1, 
		      simD2->getAngle(), simD2->getOop(), dct2, dphi2); //generate neutron angles

    //    Reaction->Decay1.initPolar(1,2,53.0/180*M_PI, 168.0/180*M_PI);    
    //   Reaction->Decay2.initPolar(1,2,110.0/180*M_PI, 4/180*M_PI);    


    FourVector pvec=Reaction->Decay1;
    FourVector nvec=Reaction->Decay2;
	

    FourVector qvec = (FourVector)(Reaction->electronIn) - (FourVector)(Reaction->electronOut);  
    double weight;

    double p1=0,pn=0;

    pvec.setE(0);
    pvec*=1/pvec.momentum();
    
    nvec.setE(0);
    nvec*=1/nvec.momentum();       
    if (He3HACK::mode==-1) // relativisitc case
      {


	double t=sobol()*M_PI/2; //random t 

	double t1=t-dt;
	double t2=t+dt;
	double deltat=t2-t1;
      

	setupbisec(t,qvec[0],qvec[1],qvec[2],qvec[3],pvec[1],pvec[2],pvec[3],nvec[1],nvec[2],nvec[3]);
	double l=bisec(0,1); //bisec start is 0 and 2 gev;
	setupbisec(t1,qvec[0],qvec[1],qvec[2],qvec[3],pvec[1],pvec[2],pvec[3],nvec[1],nvec[2],nvec[3]);
	double l1=bisec(0,2); //bisec start is 0 and 2 gev;
	setupbisec(t2,qvec[0],qvec[1],qvec[2],qvec[3],pvec[1],pvec[2],pvec[3],nvec[1],nvec[2],nvec[3]);
	double l2=bisec(0,2); //bisec start is 0 and 2 gev;

	double p1=l*cos(t);
	double p2=l*sin(t);
	double p11=l1*cos(t1);
	double p21=l1*sin(t1);
	double p12=l2*cos(t2);
	double p22=l2*sin(t2);

	double T11=sqrt(m_proton*m_proton+p11*p11)-m_proton;
	double T12=sqrt(m_proton*m_proton+p12*p12)-m_proton;
	double T21=sqrt(m_neutron*m_neutron+p21*p21)-m_neutron;
	double T22=sqrt(m_neutron*m_neutron+p22*p22)-m_neutron;
    
	if (dodE)
	  weight=fabs(T12-T11)/deltat;
	else
	  weight=sqrt(sqr(T12-T11)+sqr(T22-T21))/deltat;
	//for now, dS is from 
	Reaction->Decay1.initPolar(sqrt(m_proton*m_proton+p1*p1),p1, Reaction->Decay1.theta(), Reaction->Decay1.phi());    
	Reaction->Decay2.initPolar(sqrt(m_neutron*m_neutron+p2*p2), p2, Reaction->Decay2.theta(), Reaction->Decay2.phi());

      }
    else{ //non rel. case

      qvec.setE(0);
      // so we have the angles of the proton, neutron and q.
      // we want to parmeterize an ellipse.
      double np1npn=-pvec*nvec;
      double np1q=-pvec*qvec;
      double npnq=-nvec*qvec;
      double qvec2=-qvec*qvec;

      double off=(4-np1npn*np1npn);
      double x0=(2*np1q- np1npn *npnq)/off;
      double y0=(2*npnq- np1npn*np1q)/off;
      //    double phi=-0.785398163397448; // -45degrees

      double zae=(2*Ebind*M*off-2*np1q*np1q+2*np1npn*np1q*npnq-2*npnq*npnq+4*qvec2-8*M*omega+np1npn*np1npn*(2*M*omega-qvec2));
      double tx=sqrt(zae/(off*(fabs(np1npn)-2)));
      double ty=sqrt(zae/(off*(-fabs(np1npn)-2)));
    
      double rx=(tx>ty)?tx:ty;
      double ry=(tx>ty)?ty:tx;

      double t=sobol()*2*M_PI; //random t
   
      p1=(rx*cos(t)-ry*sin(t))/sqrt(2)+x0;
      pn=(rx*cos(t)+ry*sin(t))/sqrt(2)+y0;

      if (p1<0 ) return 0;
      if (pn<0) return 0;
	
      Reaction->Decay1.initPolar(sqrt(M*M+p1*p1),p1, Reaction->Decay1.theta(), Reaction->Decay1.phi());    
      Reaction->Decay2.initPolar(sqrt(M*M+pn*pn), pn, Reaction->Decay2.theta(), Reaction->Decay2.phi());

      //dS phasespace
      if (dodE)
	weight=1.0/M*fabs(p1*((rx*sin(t)+ry*cos(t))/sqrt(2)));
      else
	weight=1.0/M*sqrt(sqr(p1*((rx*sin(t)+ry*cos(t))/sqrt(2)))+sqr(pn*((ry*cos(t)-rx*sin(t))/sqrt(2))));

      //dT phasespace
      //double weight=1.0/M*fabs(p1*((rx*sin(t)+ry*cos(t))/sqrt(2)));
      //    cout <<fabs(((rx*sin(t)+ry*cos(t))/sqrt(2)))<<endl;
      //dE  phasespace v2.
      //weight=fabs((rx*sin(t)+ry*cos(t))/sqrt(2)* Reaction->Decay1.momentum()/Reaction->Decay1.energy());
    }
  
    weight*=fabs(1. / sqrt(1.-(q*q-omega*omega)/(4.*E0*(E0-omega))) * 1. / sqrt((q*q-omega*omega)/(4.*E0*(E0-omega)))
		 * q / (2*E0*(E0-omega)) * sin(the)); // from he3eepn

    // gamma factor
    double
      s       = 9*M*M + 2*3*M*omega + omega*omega - q*q,
      epsilon = 1 / (1 - 2 * q * q / q2 * tan(the/2) * tan(the/2)),
      kgamma  = (s - 9*M*M) / (2 * 3*M),
      Gamma   = alpha/(2*M_PI*M_PI) * Eprime/E0 * kgamma/(-q2) * 1/(1-epsilon);

    if (He3HACK::mode==1 || He3HACK::mode==-1) weight*=Gamma;
      
    // saving for writeout
    if (He3HACK::mode==2)
      {
	FourVector p2=qvec-(FourVector) Reaction->Decay1-(FourVector) Reaction->Decay2;
	He3HACK::buf[3]=Eprime; //Electron out
	He3HACK::buf[4]=the;
	He3HACK::buf[5]=phe;
	He3HACK::buf[6]=p1*p1/2/M;
	He3HACK::buf[7]=pn*pn/2/M;
	He3HACK::buf[8]=omega-Ebind-He3HACK::buf[6]-He3HACK::buf[7]; // This should be the energy of the third particle 
	He3HACK::buf[9]=Reaction->Decay1.theta();
	He3HACK::buf[10]=Reaction->Decay1.phi();
	He3HACK::buf[11]=Reaction->Decay2.theta();
	He3HACK::buf[12]=Reaction->Decay2.phi();
	He3HACK::buf[13]=weight;

	He3HACK::buf[14]=p2.theta();
	He3HACK::buf[15]=p2.phi();


      }

    return weight;

  }
  if (He3HACK::mode==3){

    Reaction->electronOut.initPolar(He3HACK::buf[3],He3HACK::buf[3],He3HACK::buf[4],He3HACK::buf[5]);


    double p1=sqrt(He3HACK::buf[6]*2*M);
    double pn=sqrt(He3HACK::buf[7]*2*M);

    Reaction->Decay1.initPolar(sqrt(M*M+p1*p1),p1, He3HACK::buf[9],He3HACK::buf[10]);
    Reaction->Decay2.initPolar(sqrt(M*M+pn*pn), pn,  He3HACK::buf[11],He3HACK::buf[12]);

    double weight=1e10*He3HACK::buf[13];

    return weight;

  }


  return 0;
}
 
generateHe3fast::generateHe3fast(reaction *r, simDetectorBase *Sime, 
				 simDetectorBase *SimD1, simDetectorBase *SimD2,
				 SIMUL *rundb, modeltype Model)
{
  Ebind = 0.007718;
  //#define MOD_DEBUG 1
  Reaction   = r; 
  sime=Sime; simD1=SimD1; simD2=SimD2;
  targetmass = Reaction->getTarget()->getMass();
  dphie = sime->getDphi()/sin(sime->getAngle() - sime->getDtheta());

  Target.initPolar(targetmass,0,0,0);

  p1_0 = simD1->getMomentum(); dp1 = simD1->getDmomentum();
  dct1 = sin(simD1->getDtheta()); dphi1 = simD1->getDphi();

  dct2 = sin(simD2->getDtheta()); dphi2 = simD2->getDphi();
  dp2 = simD2->getDmomentum();
  Ex   = rundb->excitation;
  if (getenv("DOMEGA") == NULL) domega=0.001;
  else domega = atof(getenv("DOMEGA"))/1000;
  if (getenv("DQ") == NULL) dq=0.001;
  else dq = atof(getenv("DQ"))/1000;
  std::cout << "Selected q-omega-range:\ndomega="<<domega*1000<<"MeV, dq="<<dq*1000<<"MeV/c\n"; 



  phasespace = (getenv("GRID") == NULL); //(getenv("He3eepnGRID") == NULL);
  
  energycode= (getenv("NR_ENERGY") == NULL)?0:atoi(getenv("NR_ENERGY"));
  jacob=(getenv("NR_JACOB") == NULL)?0:atoi(getenv("NR_JACOB"));
  if (phasespace) 
    {
      Label = "d[W]_1!d[W]_2!dT_1!";
      Unit = "GeV sr^2!";
      std::cout << "Calculating phasespace!" << std::endl;
    }
  else
    {
      Label = "d[s]";
      Unit = "[m]b";
      std::cout << "Calculating He3-model!" << std::endl;
    };
  CrossSection = new gridinterpolation;
  omega_0    = CrossSection->getomega()/1000;
  q_0        = CrossSection->getq()/1000;
}

double  generateHe3fast::generateEvent(double helicity)
{
  omega = omega_0 + (2 * sobol() - 1) * domega; //generate in omega with full width of 2MeV
  q = q_0 + (2 * sobol() - 1) * dq; //generate in q with full width of 2MeV
  
  double  
    E0 = Reaction->electronIn.energy(),
    Eprime = E0 - omega;
  if (Eprime < 0) return 0;
  double
    q2 = omega * omega - q * q,
    the =  2 * asin(sqrt(-q2 / (4 * E0 * Eprime))), //calculate electron angles
    phe = (2 * sobol() - 1) * dphie;



  //we want to generate nonuniform in p1
  double  t=sobol();
  
  double r=t;
  if (!phasespace) r=(1-((exp(-7*t)-exp(-7))/(1-exp(-7))));

  //  double p1=p1_0+dp1*(2*r-1);

  Reaction->electronOut.initPolar(Eprime, Eprime, the, phe); 
  generateLabAngles(&Reaction->Decay1,      1, 
		    simD1->getAngle(), simD1->getOop(), dct1, dphi1); //generate proton
  generateLabAngles(&Reaction->Decay2,      1, 
		    simD2->getAngle(), simD2->getOop(), dct2, dphi2); //generate neutron angles
  FourVector Photon = (FourVector)(Reaction->electronIn) - (FourVector)(Reaction->electronOut);

  FourVector pd=(FourVector) Reaction->Decay1;
  FourVector phd=Photon;
  FourVector nd=(FourVector) Reaction->Decay2;
  
  Photon_rotation(&Photon,&pd,&nd);

  double T1maxnr=1000*T1max_nr(q, omega, -(phd.theta()), pd.theta() ,  pd.phi(), nd.theta(), nd.phi());
  double T1=(CrossSection->getGridT1min()+(T1maxnr-CrossSection->getGridT1min())*r);

  setup(E0, omega, q, pd.theta() ,  pd.phi(), nd.theta(), nd.phi());
  double p1max    = bisec(0.4,0.8,0,targetmass,omega,q);
  double T1maxrel = 1000*(sqrt(p1max*p1max+m_proton * m_proton)-m_proton);
  double T1match  = T1/T1maxnr*T1maxrel/1000;
  double p1=sqrt(T1match*(T1match+2*m_proton));
 
  Reaction->Decay1.initPolar(T1match+m_proton,p1, Reaction->Decay1.theta(), Reaction->Decay1.phi());    
 
  FourVector Deuteron = Photon + Target - (FourVector)(Reaction->Decay1);
  //boost deuteron to its own restframe
  FourVector DeuteronCMS = Deuteron.Lorentz(-Deuteron); 
  double EdCMS = DeuteronCMS.energy();

  //Neutrons energy in deuteron restframe
  double EnCMS = (EdCMS * EdCMS - m_proton * m_proton + m_neutron * m_neutron) / (2 * EdCMS);

  FourVector Neutron = (FourVector)(Reaction->Decay2);
  Neutron.rot_phi(-Deuteron.phi());
  Neutron.rot_theta(-Deuteron.theta());
  double thn = Neutron.theta();
  double Ed = Deuteron.energy();
  double Pd = Deuteron.momentum();

  double En=0;
  if (Ed*Ed*(2*(EnCMS - m_neutron)*(EnCMS + m_neutron)*Ed*Ed + cos(2*thn)*m_neutron*m_neutron*Pd*Pd + 
	     (-2*EnCMS*EnCMS + m_neutron*m_neutron)*Pd*Pd)<0) 
    /*
      En = -(1/(Ed*(-2*Ed*Ed + Pd*Pd + cos(2*thn)*Pd*Pd))
      *(2*EnCMS*Ed*Ed*Ed*sqrt(1 - 1/(Ed*Ed)*Pd*Pd)));
      */
    return 0;
  else 
    En = -(1/(Ed*(-2*Ed*Ed + Pd*Pd + cos(2*thn)*Pd*Pd))*
	   (2*EnCMS*Ed*Ed*Ed*sqrt(1 - 1/(Ed*Ed)*Pd*Pd) + Pd*cos(thn)*sqrt(2)*
	    sqrt(Ed*Ed*(2*(EnCMS - m_neutron)*(EnCMS + m_neutron)*Ed*Ed 
			+ cos(2*thn)*m_neutron*m_neutron*Pd*Pd + 
			(-2*EnCMS*EnCMS + m_neutron*m_neutron)*Pd*Pd))));

  if (En < m_neutron) return 0; //check for nan
  double Pn = sqrt(En * En - m_neutron * m_neutron);
  thn = Reaction->Decay2.theta();
  double phn = Reaction->Decay2.phi();
  //  double thp = Reaction->Decay1.theta();
  //  double php = Reaction->Decay1.phi();

  Reaction->Decay2.initPolar(En, Pn, thn, phn);

  double Jacobian = 1. / sqrt(1.-(q*q-omega*omega)/(4.*E0*(E0-omega))) * 1. / sqrt((q*q-omega*omega)/(4.*E0*(E0-omega)))
    * q / (2*E0*(E0-omega)) * sin(the)*(T1maxnr-CrossSection->getGridT1min())/1000;

  if (!phasespace)  Jacobian*=7*exp(-7*t)/(1-exp(-7));  // fix nonuniform scattering in dp;
  Jacobian*=T1maxrel/T1maxnr;


  weight = fabs(Jacobian);
  
  FourVector GridEIn     = (FourVector)(Reaction->electronIn);
  FourVector GridEOut    = (FourVector)(Reaction->electronOut);
  FourVector GridProton  = (FourVector)(Reaction->Decay1);
  FourVector GridNeutron = (FourVector)(Reaction->Decay2);
  

  //  cout <<"pre weight"<<weight<<endl;
  weight *= getWeight(GridEIn,GridEOut,GridProton,GridNeutron,targetmass,CrossSection,1,-1); //0 ,T1

  return weight;
}
 

const double HammerFit::gmp[][2] = 
  {{0, 1},
   {0.01, 0.996369}, {0.02, 0.993116}, {0.03, 0.9902},
   {0.04, 0.987588}, {0.05, 0.98525}, {0.06, 0.98316},
   {0.07, 0.981296}, {0.08, 0.979636}, {0.09, 0.978162},
   {0.1, 0.976858}, {0.11, 0.97571}, {0.12, 0.974705},
   {0.13, 0.97383}, {0.14, 0.973075}, {0.15, 0.972431},
   {0.16, 0.971889}, {0.17, 0.97144}, {0.18, 0.971079},
   {0.19, 0.970797}, {0.2, 0.97059}, {0.21, 0.970452},
   {0.22, 0.970378}, {0.23, 0.970362}, {0.24, 0.970402},
   {0.25, 0.970492}, {0.26, 0.97063}, {0.27, 0.970812},
   {0.28, 0.971035}, {0.29, 0.971297}, {0.3, 0.971593},
   {0.31, 0.971923}, {0.32, 0.972284}, {0.33, 0.972673},
   {0.34, 0.973089}, {0.35, 0.97353}, {0.36, 0.973994},
   {0.37, 0.97448}, {0.38, 0.974986}, {0.39, 0.97551},
   {0.4, 0.976052}, {0.41, 0.97661}, {0.42, 0.977183},
   {0.43, 0.977769}, {0.44, 0.978369}, {0.45, 0.978981},
   {0.46, 0.979603}, {0.47, 0.980236}, {0.48, 0.980879},
   {0.49, 0.98153}, {0.5, 0.982189}, {0.51, 0.982855},
   {0.52, 0.983528}, {0.53, 0.984207}, {0.54, 0.984892},
   {0.55, 0.985582}, {0.56, 0.986276}, {0.57, 0.986974},
   {0.58, 0.987676}, {0.59, 0.988381}, {0.6, 0.989089},
   {0.61, 0.9898}, {0.62, 0.990512}, {0.63, 0.991226},
   {0.64, 0.991942}, {0.65, 0.992659}, {0.66, 0.993376},
   {0.67, 0.994094}, {0.68, 0.994813}, {0.69, 0.995531},
   {0.7, 0.99625}, {0.71, 0.996968}, {0.72, 0.997685},
   {0.73, 0.998402}, {0.74, 0.999117}, {0.75, 0.999832},
   {0.76, 1.000545}, {0.77, 1.001256}, {0.78, 1.001966},
   {0.79, 1.002675}, {0.8, 1.003381}, {0.81, 1.004085},
   {0.82, 1.004787}, {0.83, 1.005487}, {0.84, 1.006185},
   {0.85, 1.006879}, {0.86, 1.007572}, {0.87, 1.008261},
   {0.88, 1.008948}, {0.89, 1.009632}, {0.9, 1.010313},
   {0.91, 1.010991}, {0.92, 1.011666}, {0.93, 1.012338},
   {0.94, 1.013007}, {0.95, 1.013672}, {0.96, 1.014334},
   {0.97, 1.014993}, {0.98, 1.015648}, {0.99, 1.016299},
   {1, 1.016948}, {1.1, 1.023226}, {1.2, 1.029118}, {1.3, 1.03461},
   {1.4, 1.039699}, {1.5, 1.044389}, {1.6, 1.048688}, {1.7, 1.052609},
   {1.8, 1.056164}, {1.9, 1.059368}, {2, 1.062236}, {2.1, 1.064782},
   {2.2, 1.067022}, {2.3, 1.06897}, {2.4, 1.07064}, {2.5, 1.072046},
   {2.6, 1.073201}, {2.7, 1.074118}, {2.8, 1.074809}, {2.9, 1.075285},
   {3, 1.075558}, {3.1, 1.075638}, {3.2, 1.075535}, {3.3, 1.075259},
   {3.4, 1.074819}, {3.5, 1.074224}, {3.6, 1.073481}, {3.7, 1.0726},
   {3.8, 1.071587}, {3.9, 1.070449}, {4, 1.069194}, {4.1, 1.067828},
   {4.2, 1.066357}, {4.3, 1.064788}, {4.4, 1.063124}, {4.5, 1.061373},
   {4.6, 1.059539}, {4.7, 1.057627}, {4.8, 1.055642}, {4.9, 1.053588},
   {5, 1.051469}};
const double HammerFit::gep[][2] = 
  {{0, 1}, {0.01, 0.997052}, {0.02, 0.994324}, {0.03, 0.991792},
   {0.04, 0.989435}, {0.05, 0.987237}, {0.06, 0.98518}, {0.07, 0.983252},
   {0.08, 0.981439}, {0.09, 0.97973}, {0.1, 0.978115}, {0.11, 0.976585},
   {0.12, 0.975133}, {0.13, 0.973751}, {0.14, 0.972432},
   {0.15, 0.971171}, {0.16, 0.969962}, {0.17, 0.9688}, {0.18, 0.967682},
   {0.19, 0.966603}, {0.2, 0.965559}, {0.21, 0.964547}, {0.22, 0.963564},
   {0.23, 0.962607}, {0.24, 0.961674}, {0.25, 0.960763},
   {0.26, 0.959871}, {0.27, 0.958996}, {0.28, 0.958136}, {0.29, 0.95729},
   {0.3, 0.956457}, {0.31, 0.955634}, {0.32, 0.954821}, {0.33, 0.954017},
   {0.34, 0.953219}, {0.35, 0.952428}, {0.36, 0.951641}, {0.37, 0.95086},
   {0.38, 0.950081}, {0.39, 0.949306}, {0.4, 0.948533}, {0.41, 0.947761},
   {0.42, 0.94699}, {0.43, 0.946219}, {0.44, 0.945448}, {0.45, 0.944676},
   {0.46, 0.943903}, {0.47, 0.943129}, {0.48, 0.942352},
   {0.49, 0.941574}, {0.5, 0.940792}, {0.51, 0.940008}, {0.52, 0.939221},
   {0.53, 0.93843}, {0.54, 0.937635}, {0.55, 0.936836}, {0.56, 0.936034},
   {0.57, 0.935227}, {0.58, 0.934415}, {0.59, 0.933599}, {0.6, 0.932778},
   {0.61, 0.931952}, {0.62, 0.931121}, {0.63, 0.930285},
   {0.64, 0.929444}, {0.65, 0.928597}, {0.66, 0.927745},
   {0.67, 0.926888}, {0.68, 0.926025}, {0.69, 0.925156}, {0.7, 0.924281},
   {0.71, 0.923401}, {0.72, 0.922515}, {0.73, 0.921624},
   {0.74, 0.920726}, {0.75, 0.919823}, {0.76, 0.918913},
   {0.77, 0.917998}, {0.78, 0.917077}, {0.79, 0.91615}, {0.8, 0.915217},
   {0.81, 0.914279}, {0.82, 0.913334}, {0.83, 0.912384},
   {0.84, 0.911427}, {0.85, 0.910465}, {0.86, 0.909497},
   {0.87, 0.908523}, {0.88, 0.907543}, {0.89, 0.906558}, {0.9, 0.905567},
   {0.91, 0.90457}, {0.92, 0.903567}, {0.93, 0.902559}, {0.94, 0.901545},
   {0.95, 0.900525}, {0.96, 0.8995}, {0.97, 0.898469}, {0.98, 0.897433},
   {0.99, 0.896392}, {1, 0.895345}, {1.1, 0.884586}, {1.2, 0.873335},
   {1.3, 0.861642}, {1.4, 0.849558}, {1.5, 0.837134}, {1.6, 0.82442},
   {1.7, 0.811466}, {1.8, 0.798317}, {1.9, 0.785019}, {2, 0.771612},
   {2.1, 0.758137}, {2.2, 0.744631}, {2.3, 0.731129}, {2.4, 0.717663},
   {2.5, 0.704264}, {2.6, 0.690962}, {2.7, 0.677782}, {2.8, 0.664749},
   {2.9, 0.651887}, {3, 0.639218}, {3.1, 0.62676}, {3.2, 0.614533},
   {3.3, 0.602554}, {3.4, 0.590838}, {3.5, 0.5794}, {3.6, 0.568253},
   {3.7, 0.557409}, {3.8, 0.54688}, {3.9, 0.536676}, {4, 0.526805},
   {4.1, 0.517277}, {4.2, 0.508098}, {4.3, 0.499276}, {4.4, 0.490816},
   {4.5, 0.482723}, {4.6, 0.475003}, {4.7, 0.467659}, {4.8, 0.460694},
   {4.9, 0.454112}, {5, 0.447914}};
   

//__________________________________________________________________________
//
//
// A class that calculates elastic CS for different elements 
// in a peaking approximation using different CS models. 
//
//__________________________________________________________________________
   
double 
generateElasticPeakingApprox::generateEvent(double)
{
  double E0 = Reaction->electronIn.energy();
  generateLabAngles(&Reaction->electronOut,1,sime->getAngle(), sime->getOop(), 
		    dcte,sime->getDphi());
  
  double theta = Reaction->electronOut.theta();
  double phi   = Reaction->electronOut.phi();

  Particle target = Reaction->target;
  double targetMass = target.getMass();
  //cout<<"Target Mass: "<<targetMass<<endl;
  
  //--- Now lets calculate the kinematics for an ideal elastic case,
  //where no energy losses.
  double Ep = ((-E0-targetMass)*(m_e_sqr+E0*targetMass)+(m_e_sqr-E0*E0)*cos(theta)*
	       sqrt(targetMass*targetMass-m_e_sqr/2+m_e_sqr*cos(2*theta)/2))/
              ((E0*E0-m_e_sqr)*pow(cos(theta),2)-pow(E0+targetMass,2));
     
  //--- Now let's calculate the energy losses
  double q2 = (Reaction->electronIn-Reaction->electronOut).square();
  //cerr<<"q2: "<<q2<<", e': "<<Reaction->electronOut.energy()<<endl;
  double t =alpha/M_PI*((log(fabs(q2)/m_e_sqr)-1));
  Reaction->electronIn-= E0 * securePow(sobol(), 1/t);
  
  //--- Now we need to recalculate the energy of the scattered electron:
  E0 = Reaction->electronIn.energy();
  Ep = ((-E0-targetMass)*(m_e_sqr+E0*targetMass)+(m_e_sqr-E0*E0)*cos(theta)*
	       sqrt(targetMass*targetMass-m_e_sqr/2+m_e_sqr*cos(2*theta)/2))/
              ((E0*E0-m_e_sqr)*pow(cos(theta),2)-pow(E0+targetMass,2));

  Reaction->electronOut.initPolar(Ep, sqrt(Ep*Ep-m_e_sqr), theta, phi); 
  
  FourVector Photon = Reaction->electronIn - Reaction->electronOut;
  Reaction->Out1 = Photon + *Reaction->getTarget();
  
  q2 = (Reaction->electronIn-Reaction->electronOut).square();
  t =alpha/M_PI*((log(fabs(q2)/m_e_sqr)-1));
  Reaction->electronOut-= Reaction->electronOut.energy() * securePow(sobol(), 1/t);
  

  //--- Now the proper FF parameterization needs to be chosen for the considered element
  static NuclearFormFactor *FF = NULL;  
  double Z = target.getCharge();
  double R,a; 
  
  //if      (FF==NULL && target==P_C12)  { cout<<"Using FF for 12C"<<endl; FF = new HofstadterFit();}
  
  if (FF==NULL && target==P_C12) { cout<<"Using FF for 12C"<<endl; 
	R=2.436; a=0.810; FF = new HelmsModel(Z, targetMass, R, a);}
  else if (FF==NULL && target==P_O16) { cout<<"Using FF for 16O<"<<endl; 
	R=2.777; a=0.839; FF = new HelmsModel(Z, targetMass, R, a);}
  else if (FF==NULL && target==P_Be9)  { cout<<"Using FF for Be9"<<endl; FF = new BerylliumModifiedExpFit();}  
  else if (FF==NULL && target==P_N14)  { cout<<"Using FF for 14N"<<endl; FF = new DallyFit();}  
  else if (FF==NULL && target==P_Ta181){ cout<<"Using FF for 181Ta"<<endl; FF = new TantalumUniformFit();}  
  else if (FF==NULL && target==P_Co59) { cout<<"Using FF for 59Co"<<endl; 
	R=(4.356+4.396)/2; a=0.915; FF = new HelmsModel(Z, targetMass, R, a);}
  else if (FF==NULL && target==P_Cr52) { cout<<"Using FF for 52Cr"<<endl; 
	R=4.173; a=0.924; FF = new HelmsModel(Z, targetMass, R, a);}
  else if (FF==NULL && target==P_Mn55) { cout<<"Using FF for 55Mn"<<endl; 
	R=4.26; a=0.91; FF = new HelmsModel(Z, targetMass, R, a);}
  else if (FF==NULL && target==P_Ni58) { cout<<"Using FF for 58Ni"<<endl; 
	R=4.356; a=0.922; FF = new HelmsModel(Z, targetMass, R, a);}
  else if (FF==NULL && target==P_Fe56) { cout<<"Using FF for 56Fe"<<endl; 
	R=4.286; a=0.936; FF = new HelmsModel(Z, targetMass, R, a);}
  else if (FF==NULL && target==P_Mo98) { cout<<"Using FF for 98Mo"<<endl; 
	R=5.190; a=1.007; FF = new HelmsModel(Z, targetMass, R, a);}
  else if (FF==NULL && target==P_W184) { cout<<"Using FF for 184W"<<endl; 
	R=6.5; a=1; FF = new HelmsModel(Z, targetMass, R, a); }
  else if (FF==NULL) {cerr<<"Target element: "<<target.getName()<<" is not supported by this model! Please add it!"<<endl; exit(0);}
	     
  return FF->CrossSection(E0, theta);
  
} 


//______________________________________________________________________________
//
//
// Simulation for elastic events in the Havar foil
// 
//
//______________________________________________________________________________


double 
generateElasticHavar::generateEvent(double helicity)
{

  //--- these are the Mol fractions:
  double xCo = 0.4155;
  double xCr = 0.2218;  
  double xMn = 0.0168;  
  double xMo = 0.0118;
  double xNi = 0.1293;
  double xW =  0.00878;    
  double xC =  0.00961;  
  double xFe = 0.1839;
  double xBe = 0.00;  
  
  double xSum = xCo + xCr + xMn + xMo + xNi + xW + xC + xFe + xBe;
  
  double elem=prndm()*xSum;
 
  double R,a; 

  //--- need to correct masses to the precise values;

  Particle Target;
  NuclearFormFactor *FF;

  if (elem < xCo) { Target = P_Co59; R=4.376; a=0.915;}// Target = P_Co59; R=4.276; a=0.915 worked once//from Friedrich paper: R=(4.356+4.396)/2.; a=0.915; }// only for SpekA for ISR_330 and ISR_195: R=4.68; a=0.78 and 10.6 mu wall // for SpekA and SpekB ISR_330 and ISR_195: R=4.74; a=0.85
  else if (elem< xCo + xCr) { Target = P_Cr52; R=4.173; a=0.924;}//Target = P_Cr52; R=4.073; a=0.924 worked once//from Friedrich paper: R=4.173; a=0.924; } // only for SpekA for ISR_330 and ISR_195: R=4.45; a=0.88 and 10.6 mu wall // for SpekA and SpekB ISR_330 and ISR_195: R=4.53; a=0.93
  else if (elem< xCo + xCr + xMn) { Target = P_Mn55; R=4.26; a=0.91; }
  else if (elem< xCo + xCr + xMn + xMo) { Target = P_Mo98; R=5.190; a=1.007; }
  else if (elem< xCo + xCr + xMn + xMo + xNi) { Target = P_Ni58; R=4.356; a=0.922;}//Target = P_Ni58; R=4.256; a=0.922 worked once//from Friedrich paper: R=4.356; a=0.922; } // only for SpekA for ISR_330 and ISR_195: R=4.66; a=0.81 and 10.6 mu wall // for SpekA and SpekB ISR_330 and ISR_195: R=4.73; a=0.87
  else if (elem< xCo + xCr + xMn + xMo + xNi + xW) {Target = P_W184; R=6.5; a=1; }
  else if (elem< xCo + xCr + xMn + xMo + xNi + xW + xC) {Target = P_C12; R=2.443; a=0.810; }
  else {Target = P_Fe56; R=4.286; a=0.936;}//Target = P_Fe56; R=4.186; a=0.936; worked once//from Friedrich paper: R=4.286; a=0.936;} // only for SpekA for ISR_330 and ISR_195: R=4.46; a=0.83 and 10.6 mu wall // for SpekA and SpekB ISR_330 and ISR_195: R=4.56; a=0.88
	     
	     
  Reaction->target = Target; //--- overwrite the target that is written in the reaction string. 
  double Z = Target.getCharge();
  double targetMass = Target.getMass();
  //cout<<"Z: "<<Z<<endl;
  //cout<<"Target Mass: "<<targetMass<<endl;
  
  double E0 = Reaction->electronIn.energy();
  generateLabAngles(&Reaction->electronOut,1,sime->getAngle(), sime->getOop(), 
		    dcte,sime->getDphi());
  
  double theta = Reaction->electronOut.theta();
  double phi   = Reaction->electronOut.phi();
  //cout<<"phi: "<<phi<<",  theta: "<<theta<<",   E0: "<<E0<<endl;
  
  //--- Now lets calculate the kinematics for an ideal elastic case, where no energy losses.
  double q2 = (Reaction->electronIn-Reaction->electronOut).square();
  //cerr<<"q2: "<<q2<<", e': "<<Reaction->electronOut.energy()<<endl;
  //--- ISR
  double t =alpha/M_PI*((log(fabs(q2)/m_e_sqr)-1));
  Reaction->electronIn-= E0 * securePow(sobol(), 1/t);
  
  //--- Now we need to recalculate the energy of the scattered electron:
  E0 = Reaction->electronIn.energy();
  double Ep = ((-E0-targetMass)*(m_e_sqr+E0*targetMass)+(m_e_sqr-E0*E0)*cos(theta)*
	       sqrt(targetMass*targetMass-m_e_sqr/2+m_e_sqr*cos(2*theta)/2))/
              ((E0*E0-m_e_sqr)*pow(cos(theta),2)-pow(E0+targetMass,2));

  Reaction->electronOut.initPolar(Ep, sqrt(Ep*Ep-m_e_sqr), theta, phi); 
  
  FourVector Photon = Reaction->electronIn - Reaction->electronOut;
  Reaction->Out1 = Photon + *Reaction->getTarget();
  
  //--- FSR
  q2 = (Reaction->electronIn-Reaction->electronOut).square();
  t =alpha/M_PI*((log(fabs(q2)/m_e_sqr)-1));
  Reaction->electronOut-= Reaction->electronOut.energy() * securePow(sobol(), 1./t);
  
  
  FF = new HelmsModel(Z, targetMass, R, a);
  double CS = FF->CrossSection(E0, theta);//*1.476377953;//*1.101943117;//factor, because for the ISR2013 beam time for SpekB the wall would otherwise not fit the measured width of 10.16mu; second factor to match 495MeV Setting
  delete FF; FF = NULL;
  
  return CS;
}  


//______________________________________________________________________________
//
//
// Simulation for elastic events in Cryogenic depositions
// 
//
//______________________________________________________________________________


double 
generateElasticCryogens::generateEvent(double helicity)
{

  //--- these are the Mol fractions:
  double xO2 = 0.2111;
  double xN2 = 0.7888;  
  double xSum = xO2 + xN2;
  
  double elem=prndm()*xSum;
 
  //--- need to correct masses to the precise values;

  Particle Target;
  NuclearFormFactor *FF;

  if (elem < xO2){ //cout<<"Using FF for 16O"<<endl; 
     Target = P_O16; double R=2.777; double a=0.839; FF = new HelmsModel(Target.getCharge(), Target.getMass(), R, a);}
  else { //cout<<"Using FF for 14N"<<endl; 
    Target = P_N14; FF = new DallyFit();}  
  
	   	     
  Reaction->target = Target; //--- overwrite the target that is written in the reaction string. 
  //double Z = Target.getCharge();
  double targetMass = Target.getMass();
  //cout<<"Z: "<<Z<<endl;
  //cout<<"Target Mass: "<<targetMass<<endl;
  
  double E0 = Reaction->electronIn.energy();
  generateLabAngles(&Reaction->electronOut,1,sime->getAngle(), sime->getOop(), 
		    dcte,sime->getDphi());
  
  double theta = Reaction->electronOut.theta();
  double phi   = Reaction->electronOut.phi();
  //cout<<"phi: "<<phi<<",  theta: "<<theta<<",   E0: "<<E0<<endl;
  
  //--- Now lets calculate the kinematics for an ideal elastic case, where no energy losses.
  double q2 = (Reaction->electronIn-Reaction->electronOut).square();
  //cerr<<"q2: "<<q2<<", e': "<<Reaction->electronOut.energy()<<endl;
  //--- ISR
  double t =alpha/M_PI*((log(fabs(q2)/m_e_sqr)-1));
  Reaction->electronIn-= E0 * securePow(sobol(), 1/t);
  
  //--- Now we need to recalculate the energy of the scattered electron:
  E0 = Reaction->electronIn.energy();
  double Ep = ((-E0-targetMass)*(m_e_sqr+E0*targetMass)+(m_e_sqr-E0*E0)*cos(theta)*
	       sqrt(targetMass*targetMass-m_e_sqr/2+m_e_sqr*cos(2*theta)/2))/
              ((E0*E0-m_e_sqr)*pow(cos(theta),2)-pow(E0+targetMass,2));

  Reaction->electronOut.initPolar(Ep, sqrt(Ep*Ep-m_e_sqr), theta, phi); 
  
  FourVector Photon = Reaction->electronIn - Reaction->electronOut;
  Reaction->Out1 = Photon + *Reaction->getTarget();
  
  //--- FSR
  q2 = (Reaction->electronIn-Reaction->electronOut).square();
  t =alpha/M_PI*((log(fabs(q2)/m_e_sqr)-1));
  Reaction->electronOut-= Reaction->electronOut.energy() * securePow(sobol(), 1/t);
  
  
  double CS = FF->CrossSection(E0, theta);
  delete FF; FF = NULL;
  //cout<<"FF: "<<FF->CrossSection(E0, theta)<<endl;
  return CS;
}  
   
   
//@
