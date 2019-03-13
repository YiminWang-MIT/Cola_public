#include "radiative.h"
#include "radiative_helpers.h"

#include <TLorentzVector.h>
#include <TF1.h>
#include <Math/WrappedMultiTF1.h>
#include <Math/Functor.h>
#include <Math/Interpolator.h>
#include <iostream>
#include <iomanip>
#include <fstream>

GeneratorRadiative::GeneratorRadiative(int skip,unsigned long int seed):GeneratorBase(5,skip,seed)
{
  mP = getMass("proton");
  me = getMass("e-");
  alpha = 7.2973525698E-3;
  muP = 2.7928456;
  alphaCubedOver64PiSq = alpha * alpha * alpha / (64* M_PI * M_PI);
  cmSqMeVSq = 3.8937966E-22;
  qSqDipole = 710000.; // MeV^2
  usePointProtonFF = false;
  useKellyFF = false;

  pGamma = new Gamma();
  pI4 = new IFourMat();
  pSigma = new Sigma();
}


void GeneratorRadiative::setThetaRange(double thetamin,double thetamax)
{
  thetaMin=thetamin*M_PI/180.;
  thetaMax=thetamax*M_PI/180.;
  cosThetaMin=cos(thetaMin);
  cosThetaDelta=cos(thetaMax)-cosThetaMin;
  recalcWeight();
}

void GeneratorRadiative::setPhiRange(double range)
{
  phiRange=range*M_PI/180.;
  recalcWeight();
}

void GeneratorRadiative::setDeltaECut(bool use, double cut)
{
  useDeltaECut = use;
  deltaECut = cut;
}

void GeneratorRadiative::setSoftFraction(double frac)
{
  softFraction = frac;
}

void GeneratorRadiative::setPushPhoton(bool push)
{
  pushPhoton = push;
}


void GeneratorRadiative::setThetaDistribution(int d)
{
  thetaDistribution = d;
  if (d==0) recalcWeight();
}

void GeneratorRadiative::addPointProtonFFCalc(bool b)
{
  usePointProtonFF = b;
}

void GeneratorRadiative::addKellyFFCalc(bool b)
{
  useKellyFF = b;
}

void GeneratorRadiative::recalcWeight()
{
  phaseweight=-phiRange*4*cosThetaDelta; //This is the part of 4Pi 
}

double GeneratorRadiative::calcElasticCorr(const ElasticKinematics &e)
{
  // Z^0 Maximon and Tjon coefficient
  double MT0 = 13./6.*TMath::Log(e.Q2()/(me*me)) - 28./9. - 0.5*pow(TMath::Log(e.eta()),2)
               + TMath::DiLog(pow(TMath::Cos(e.theta()/2.),2)) - M_PI*M_PI/6.;

  // Z^1 Maximon and Tjon coefficient
  double MT1 = TMath::DiLog(1 - e.eta()/e.x()) - TMath::DiLog(1. - 1./(e.eta()*e.x()));

  // Z^2 Maximon and Tjon coefficient
  double MT2 = e.E4()/e.p4()*(-.5*pow(TMath::Log(e.x()),2) - TMath::Log(e.x())*TMath::Log(e.rho()*e.rho()/(mP*mP))
               - TMath::DiLog(1.-1./(e.x()*e.x())) + 2*TMath::DiLog(-1./e.x()) + M_PI*M_PI/6.
               + TMath::Log(e.x())) + 1.;

  // Add up all of the terms
  double delta_el = alpha/M_PI*(MT0 - 2.*beamCharge*MT1 + MT2);

  return exp(delta_el);
}

// Original Maximon and Tjon correction, useful for testing
double GeneratorRadiative::getMTjDelta(const ElasticKinematics &e, double deltaE)
{
  // Z^0 Maximon and Tjon delta E part
  double MT0 = -(TMath::Log(e.Q2()/(me*me))-1.)*TMath::Log(4.*e.E1()*e.E3()/pow(2.*e.eta()*deltaE,2));
    
  // Z^1 Maximon and Tjon deltaE part
  double MT1 = -TMath::Log(e.eta())*TMath::Log(e.Q2()*e.x()/pow(2.*e.eta()*deltaE,2));

  // Z^2 Maximon and Tjon delaE part 
  double MT2 = -(e.E4()/e.p4()*TMath::Log(e.x())-1.)*TMath::Log(mP*mP/pow((2.*e.eta()*deltaE),2));

  // Add up all of the terms
  double corr_deltaE = alpha/M_PI*(MT0 - 2.*beamCharge*MT1 + MT2);

  return TMath::Log(calcElasticCorr(e))+corr_deltaE;
}

double GeneratorRadiative::calcElasticCorrMoTsai(const ElasticKinematics &e)
{
  // Z^0 Mo and Tsai coefficient
  double MT0 = -13./6.*TMath::Log(e.Q2()/(me*me)) + 28./9. - 3.*TMath::Log(e.eta())*(TMath::Log(e.Q2()/(me*me)) - 1.)
    - TMath::DiLog((e.E3()-beamEnergy)/e.E3()) - TMath::DiLog((beamEnergy - e.E3())/beamEnergy);

  // Z^1 Mo and Tsai coefficient
  double MT1 = -6.*TMath::Power(TMath::Log(e.eta()),2.)
    + TMath::DiLog(1.-2.*e.E3()*e.E4()/beamEnergy/mP) - 2.*TMath::DiLog(1. - 2.*e.E3()/mP)
    - TMath::DiLog(1.-2.*beamEnergy*e.E4()/e.E3()/mP) + 2.*TMath::DiLog(1. - 2.*beamEnergy/mP);

  // Z^2 Mo and Tsai coefficient
  double MT2 = -TMath::Log(e.E4()/mP) + (e.E4()/e.p4())*( 0.5 * TMath::Log((e.E4()+e.p4())/(e.E4()-e.p4()))*TMath::Log((e.E4()+mP)/(2*mP)) 
						 - TMath::DiLog(-TMath::Sqrt((e.E4()-mP)*(e.E4()+e.p4())/((e.E4()+mP)*(e.E4()-e.p4()))))
						 + TMath::DiLog(TMath::Sqrt((e.E4()-mP)*(e.E4()-e.p4())/((e.E4()+mP)*(e.E4()+e.p4()))))
						 + TMath::DiLog(-TMath::Sqrt((e.E4()-mP)/(e.E4()+mP)))
						 - TMath::DiLog(TMath::Sqrt((e.E4()-mP)/(e.E4()+mP))));

  // Add up all of the terms
  double delta_el = (-alpha/M_PI*(MT0 - beamCharge * MT1 + MT2));

  return exp(delta_el);
}

double GeneratorRadiative::calcElasticCorrMeisYen(const ElasticKinematics &e)
{
  // Z^0 Meister and Yennie coefficient
  double MY0 = 13./6.*TMath::Log(e.Q2()/(me*me)) - 0.5 * TMath::Power(TMath::Log(e.eta()),2.) - 28./9.;


  // Z^1 Meister and Yennie coefficient
  double MY1 = (((mP > (2.*e.E3()))? (TMath::Power(TMath::Log(2.*e.E3()/mP),2.)) : 0.) - 
		((mP > (2.*e.E1()))? (TMath::Power(TMath::Log(2.*e.E1()/mP),2.)) : 0.));
  
  // Z^2 Meister and Yennie coefficient
  double MY2 = 1.5*TMath::Log(2.*e.E4()/mP) - 0.5*TMath::Power(TMath::Log(e.E4()/mP),2.);

  // Add up all of the terms
  double delta_el = (alpha/M_PI*(MY0 - beamCharge*MY1 + MY2));

  return exp(delta_el);
}

double GeneratorRadiative::getVpolLep(double Q2, double mass)
{
  double msq = mass*mass;
  return 2.*alpha/(3.*M_PI)*(sqrt(1.+4.*msq/Q2)*(1.-2.*msq/Q2)*TMath::Log(Q2*pow(1.+sqrt(1.+4.*msq/Q2),2)/(4.*msq))+4.*msq/Q2-5./3.);
}

// Numerically integrate Maximon and Tjon up to k_cut as a function of elastic cosTheta
void GeneratorRadiative::Initialize()
{

  k_cut = 1; // Photon energy cut for elastic vs. inelastic
  method3_deltaE_cut = 1; // Same cut but in deltaE
  double IntTol = 0.00001; // Tolerance for integration
  const int InterpolPoints = 1000; // Points for function interpolation

  // Initial momenta 
  p1.SetXYZM(0,0,TMath::Sqrt(beamEnergy*beamEnergy-me*me),me);
  p2.SetXYZM(0.,0.,0.,mP);
  //E1 = beamEnergy;

  // Functions to be integrated over the "elastic" region
  Btilde f_p1_p2(*this, p1, p2);
  Btilde f_p1_p3(*this, p1, p3);
  Btilde f_p1_p4(*this, p1, p4);
  Btilde f_p2_p3(*this, p2, p3);
  Btilde f_p2_p4(*this, p2, p4);
  Btilde f_p3_p4(*this, p3, p4);
  intSampletoKcut f_sample(*this);

  // Wrap the functions
  ROOT::Math::Functor1D wf_p1_p2(f_p1_p2);
  ROOT::Math::Functor1D wf_p1_p3(f_p1_p3);
  ROOT::Math::Functor1D wf_p1_p4(f_p1_p4);
  ROOT::Math::Functor1D wf_p2_p3(f_p2_p3);
  ROOT::Math::Functor1D wf_p2_p4(f_p2_p4);
  ROOT::Math::Functor1D wf_p3_p4(f_p3_p4);
  ROOT::Math::Functor wf_sample(f_sample,2);

  // Create the integrators
  i_p1_p2 = new ROOT::Math::GSLIntegrator(ROOT::Math::IntegrationOneDim::kADAPTIVE);
  i_p1_p3 = new ROOT::Math::GSLIntegrator(ROOT::Math::IntegrationOneDim::kADAPTIVE);
  i_p1_p4 = new ROOT::Math::GSLIntegrator(ROOT::Math::IntegrationOneDim::kADAPTIVE);
  i_p2_p3 = new ROOT::Math::GSLIntegrator(ROOT::Math::IntegrationOneDim::kADAPTIVE);
  i_p2_p4 = new ROOT::Math::GSLIntegrator(ROOT::Math::IntegrationOneDim::kADAPTIVE);
  i_p3_p4 = new ROOT::Math::GSLIntegrator(ROOT::Math::IntegrationOneDim::kADAPTIVE);
  i_sample = new ROOT::Math::GSLMCIntegrator(ROOT::Math::IntegrationMultiDim::kVEGAS);

  i_p1_p2->SetFunction(wf_p1_p2); // To calculate B(p1, p2, k_cut)
  i_p1_p2->SetRelTolerance(IntTol);

  i_p1_p3->SetFunction(wf_p1_p3); // To calculate B(p1, p3, k_cut)
  i_p1_p3->SetRelTolerance(IntTol);

  i_p1_p4->SetFunction(wf_p1_p4); // To calculate B(p1, p4, k_cut)
  i_p1_p4->SetRelTolerance(IntTol);

  i_p2_p3->SetFunction(wf_p2_p3); // To calculate B(p2, p3, k_cut)
  i_p2_p3->SetRelTolerance(IntTol);

  i_p2_p4->SetFunction(wf_p2_p4); // To calculate B(p2, p4, k_cut)
  i_p2_p4->SetRelTolerance(IntTol);

  i_p3_p4->SetFunction(wf_p3_p4); // To calculate B(p3, p4, k_cut)
  i_p3_p4->SetRelTolerance(IntTol);

  i_sample->SetFunction(wf_sample);
  ROOT::Math::IntegratorMultiDimOptions sample_opts = i_sample->Options();
  sample_opts.SetNCalls(100000); // Increase for precomputing values
  i_sample->SetOptions(sample_opts);

  // Create the interpolators
  const int vpolInterPoints = 7186;
  inter_vpol = new ROOT::Math::Interpolator(vpolInterPoints , ROOT::Math::Interpolation::kCSPLINE);
  inter_brem_ee = new ROOT::Math::Interpolator(InterpolPoints, ROOT::Math::Interpolation::kCSPLINE);
  inter_brem_ep = new ROOT::Math::Interpolator(InterpolPoints, ROOT::Math::Interpolation::kCSPLINE);
  inter_brem_pp = new ROOT::Math::Interpolator(InterpolPoints, ROOT::Math::Interpolation::kCSPLINE);
  inter_virt = new ROOT::Math::Interpolator(InterpolPoints, ROOT::Math::Interpolation::kCSPLINE);
  inter_prime = new ROOT::Math::Interpolator(InterpolPoints, ROOT::Math::Interpolation::kCSPLINE);

  double x_cosTheta[InterpolPoints];
  double y_brem_ee[InterpolPoints];
  double y_brem_ep[InterpolPoints];
  double y_brem_pp[InterpolPoints];
  double y_virt[InterpolPoints];
  double y_prime[InterpolPoints];

  for (int i = 0; i < InterpolPoints; i++)
  {
    // Theta angle for the lepton:
    double cosTheta = cosThetaMin + cosThetaDelta - i*cosThetaDelta/(InterpolPoints - 1);
    double theta = acos(cosTheta);

    // Generate elastic kinematic variables
    ElasticKinematics el(beamEnergy, theta);

    double E3 = el.E3();
    double mom3 = sqrt(E3*E3 - me*me);

    p3.SetXYZM(mom3*sin(theta),0,mom3*cosTheta,me);
    p4 = p1 + p2 - p3;
    double E4 = p4.E();

    x_cosTheta[i] = cosTheta;

    // Bremsstrahlung correction from the lepton term
    y_brem_ee[i] = -2.*alpha*(d_p1_p1()-2.*d_p1_p3()+d_p3_p3(E3));
    // Bremsstrahlung correction from the interference term
    y_brem_ep[i] = 4.*alpha*(d_p1_p2()-d_p1_p4()-d_p2_p3()+d_p3_p4());
    // Bremsstrahlung correction from the proton term
    y_brem_pp[i] = -2.*alpha*(d_p2_p2()-2.*d_p2_p4()+d_p4_p4(E4));
    // Approach of Maximon & Tjon to the TPE diagrams (J. Arrington, et al., arXiv:1105.0951):    
    y_prime[i] =-(alpha/M_PI)*(TMath::Log(beamEnergy/E3)*TMath::Log(el.Q2()*el.Q2()/(4.*mP*mP*beamEnergy*E3)) + 2.*TMath::DiLog(1. - 0.5*mP/beamEnergy) - 2.*TMath::DiLog(1. - 0.5*mP/E3));
    // Virtual photon polarization with just e+ and e- in loop
    y_virt[i] = (2.*alpha/M_PI)*(-5./9. + TMath::Log(el.Q2()/(me*me))/3.) + 
      (alpha/M_PI)*(3.*TMath::Log(el.Q2()/(me*me))/2. - 2.);
  }

  // Vacuum polarization -- From Fedor Ignatov's calculation, as used in ESEPP
  FILE *fvpol = fopen(Form("%s/.darklight/shared/generators/vpol.dat", getenv("COOKERHOME")),"r"); // Opening the file "vpol.dat"
  if (fvpol == NULL) { std::cerr << "Can't open file \"vpol.dat\"!" << std::endl; }
  int np_vpol = 0;
  char str_vpol[128];
  double s[vpolInterPoints];
  double rep[vpolInterPoints]; // Re(P(-s))

  while (!feof(fvpol)) // Reading from the file "vpol.dat"
  {
    str_vpol[0] = 0;
    fgets(str_vpol, 128, fvpol);
    if (feof(fvpol) || strlen(str_vpol) == 0) break; // The end or empty string

    if (str_vpol[0] != '/')
    {
      sscanf(str_vpol, "%lf %lf", &s[np_vpol], &rep[np_vpol]);
      rep[np_vpol] *= 2.;
      s[np_vpol] *= 1000000; // Convert to MeV^2
      np_vpol++;
    }
  }
  if (np_vpol!=vpolInterPoints)
    std::cerr << "Something went wrong reading \"vpol.dat\"! " << std::endl;

  fclose(fvpol); // Closing the file "vpol.dat"
  inter_vpol->SetData(np_vpol, s, rep); // Interpolating


  // Integrate over the deltaE and photon angle sampling distributions
  int sampleInterPoints;
  bool usePreComp = ((int)(beamEnergy+0.5)==2010&&cosThetaMin<0.999&&(cosThetaMin+cosThetaDelta)>-0.5&&k_cut>0.9999&&k_cut<1.0001&&softFraction>0.499&&softFraction<0.501&&!useDeltaECut);
  if (usePreComp)
    sampleInterPoints = 600;
  else 
  {
    sampleInterPoints = (125.*(thetaMax-thetaMin));
    if (sampleInterPoints<3)
      sampleInterPoints = 3;
  }

  //disable method 2 weight calculation
  sampleInterPoints=3;
  
  inter_sample = new ROOT::Math::Interpolator(sampleInterPoints, ROOT::Math::Interpolation::kCSPLINE);
  double x_sample[sampleInterPoints];
  double y_sample[sampleInterPoints];

  if (usePreComp) // Load precomputed integral
  {
    FILE *intdata = NULL;
    if (beamCharge==1)
      intdata = fopen(Form("%s/.darklight/shared/generators/sampleintegral_pos.dat", getenv("COOKERHOME")),"r");
    else if (beamCharge==-1)
      intdata = fopen(Form("%s/.darklight/shared/generators/sampleintegral_ele.dat", getenv("COOKERHOME")),"r");
    else
      std::cerr << "beamCharge is set to " << beamCharge << ". We don't recognize this." << std::endl;

    if (intdata == NULL) { std::cerr << "Can't open file sampleintegral data file!" << std::endl; }

    int np_sample = 0;
    char str_sample[256];

    while (!feof(intdata))
    {
      str_sample[0] = 0;
      char * fgetsResult = fgets(str_sample, 256, intdata);
      if (feof(intdata) || strlen(str_sample) == 0) break; // The end or empty string
      
      sscanf(str_sample, "%lf %lf", &x_sample[np_sample], &y_sample[np_sample]);
      np_sample++;
    }
    if (np_sample!=sampleInterPoints)
      std::cerr << "Something went wrong reading \"sampleintegral.dat\"! " << std::endl;
  }
  else // Calculate numerically
  {
    std::cout << "Computing the Method 2 sample integral numerically" << std::endl;
    for (int i = 0; i < sampleInterPoints; i++)
    {
      // Theta angle for the lepton:
      interTheta = thetaMin + i*(thetaMax - thetaMin)/(sampleInterPoints-1);

      // Generate elastic kinematic variables
      ElasticKinematics el(beamEnergy, interTheta);

      x_sample[i] = interTheta;

      // Sampling distribution integrated over all photon angles and k up to k_cut
      double intstart[2] = {-1., 0.};
      double intend[2] = {1., 2*M_PI};
      inter_elE = el.E3();

      // Z^0 deltaE-dependent part (from electron)
      double aExp = alpha/M_PI*(log(el.Q2()/(me*me)) - 1.);

      // Z^1 deltaE-dependent part (from proton)
      double bExp = -2.*alpha/M_PI*beamCharge*log(el.eta());

      // Z^2 deltaE-dependent part (from proton)
      double cExp = alpha/M_PI*(el.E4()*log(el.x())/el.p4() - 1.);

      // Calculate t (5.16 in Jan's thesis)
      inter_t = 2.*(aExp + bExp + cExp);
      inter_maxDeltaE = el.E3() - me; // it can't lose more energy than its own mass.
      if ((useDeltaECut)&&(deltaECut < (el.E3()-me)))
        {
          inter_maxDeltaE = deltaECut;
        }
      y_sample[i] = i_sample->Integral(intstart, intend);
    }
  }

  // Interpolate:
  inter_brem_ee->SetData(InterpolPoints, x_cosTheta, y_brem_ee); // Lepton term
  inter_brem_pp->SetData(InterpolPoints, x_cosTheta, y_brem_pp); // Proton term
  inter_brem_ep->SetData(InterpolPoints, x_cosTheta, y_brem_ep); // Interference term
  inter_virt->SetData(InterpolPoints, x_cosTheta, y_virt); // Virtual photon correction
  inter_prime->SetData(InterpolPoints, x_cosTheta, y_prime); // TPE contribution by Maximon & Tjon
  inter_sample->SetData(sampleInterPoints, x_sample, y_sample); // Sampling distribution

  // Interpolate Jan's form factor fits (from global cross section and polarized data)
  FILE *ffdata = fopen(Form("%s/.darklight/shared/generators/ff_splinefits.dat", getenv("COOKERHOME")),"r");
  if (ffdata == NULL) { std::cout << "Can't open file \"ff_splinefits.dat\"!" << std::endl; }

  inter_splineGE = new ROOT::Math::Interpolator(1000, ROOT::Math::Interpolation::kCSPLINE);
  inter_splineGEupper = new ROOT::Math::Interpolator(1000, ROOT::Math::Interpolation::kCSPLINE);
  inter_splineGElower = new ROOT::Math::Interpolator(1000, ROOT::Math::Interpolation::kCSPLINE);
  inter_splineGM = new ROOT::Math::Interpolator(1000, ROOT::Math::Interpolation::kCSPLINE);
  inter_splineGMupper = new ROOT::Math::Interpolator(1000, ROOT::Math::Interpolation::kCSPLINE);
  inter_splineGMlower = new ROOT::Math::Interpolator(1000, ROOT::Math::Interpolation::kCSPLINE);

  // Values stored in the data file
  double qsq[1001], ge_val[1001], ge_stat_err[1001], ge_sys_up[1001], ge_sys_down[1001], gm_val[1001], gm_stat_err[1001], gm_sys_up[1001], gm_sys_down[1001];
  
  // Set the values at Q^2=0
  qsq[0] = 0;
  ge_val[0] = 1;
  ge_stat_err[0] = 0;
  ge_sys_up[0] = 0;
  ge_sys_down[0] = 0;
  gm_val[0] = 1;
  gm_stat_err[0] = 0;
  gm_sys_up[0] = 0;
  gm_sys_down[0] = 0;

  int np_ff = 1;
  char str_ff[256];

  // Read the data file
  while (!feof(ffdata))
  {
    str_ff[0] = 0;
    char * fgetsResult = fgets(str_ff, 256, ffdata);
    if (feof(ffdata) || strlen(str_ff) == 0) break; // The end or empty string
    
    sscanf(str_ff, "%lf %lf %lf %lf %lf %lf %lf %lf %lf", &qsq[np_ff], &ge_val[np_ff], 
	   &ge_stat_err[np_ff], &ge_sys_up[np_ff], &ge_sys_down[np_ff], &gm_val[np_ff], 
	   &gm_stat_err[np_ff], &gm_sys_up[np_ff], &gm_sys_down[np_ff]);
    qsq[np_ff] *= 1000000; // Convert to MeV^2
    np_ff++;
  }

  // Calculate GE and GM bands
  double ge_up[1001], ge_down[1001], gm_up[1001], gm_down[1001];

  for (int q=0; q<np_ff; q++)
  {
    ge_up[q] = ge_val[q] + sqrt(pow(ge_stat_err[q],2) + pow(ge_sys_up[q],2));
    ge_down[q] = ge_val[q] - sqrt(pow(ge_stat_err[q],2) + pow(ge_sys_down[q],2));
    gm_up[q] = gm_val[q] + sqrt(pow(gm_stat_err[q],2) + pow(gm_sys_up[q],2));
    gm_down[q] = gm_val[q] - sqrt(pow(gm_stat_err[q],2) + pow(gm_sys_down[q],2));
  }

  fclose(ffdata);
  
  inter_splineGE->SetData(np_ff, qsq, ge_val);
  inter_splineGEupper->SetData(np_ff, qsq, ge_up);
  inter_splineGElower->SetData(np_ff, qsq, ge_down);
  inter_splineGM->SetData(np_ff, qsq, gm_val);
  inter_splineGMupper->SetData(np_ff, qsq, gm_up);
  inter_splineGMlower->SetData(np_ff, qsq, gm_down);

}

int GeneratorRadiative::generateEvent(GeneratorEvent *ev)
{
  double cosTheta=cos(theta);
  /****************************************************************
  A quick note about the weight:
  There are the following factors that need to be included in order of their inclusion
  - Lepton Solid Angle Factor (stored in GeneratorRadiative::phaseweight)
  - Elastic Correction Factor
  - Inelastic Correction Factor
  - Photon Angle Factor
  - Bremsstrahlung Cross-Section incl.
      - Modified Matrix Element
      - Phase space factors
      - Proper Jacobian between photon energy and delta E for the lepton
  ****************************************************************/

  //std::cout << "New event\n";
  //std::cout << "\tBeam charge is: " << beamCharge << "\n";

  //build theta:
  /*
  double cosTheta, theta;
  switch (thetaDistribution)
    {
    case 1: // Rutherford
      {
	double cosThetaMax = cosThetaMin+cosThetaDelta;
	cosTheta = (-cosThetaDelta*qrndNumbers[0] + cosThetaMax*(1.-cosThetaMin))/(1.-cosThetaMin-qrndNumbers[0]*cosThetaDelta);
	phaseweight = -4*phiRange*cosThetaDelta*(1.-cosTheta)*(1.-cosTheta) / (1.-cosThetaMin)/(1.-cosThetaMax);
    theta=acos(cosTheta);
	break;
      }
    case 2: // Flat in theta
      {
        theta=thetaMin+(thetaMax-thetaMin)*qrndNumbers[0];
        cosTheta = cos(theta);
        phaseweight = 4*phiRange*(thetaMax-thetaMin)*sin(theta);

        break;
      }
    default : // 0 i.e. Flat in cosTheta
      {
	cosTheta = cosThetaMin+cosThetaDelta*qrndNumbers[0];
    theta=acos(cosTheta);
	break;
      }
    }

  //build phi
  double r=qrndNumbers[1];
  double side=0;
  if (r>=0.5)
    {
      side=M_PI;
      r-=0.5;
    }
  double phi=side+phiRange*4*(r-0.25);
*/
  //Took phi and theta from outside
  // Generate elastic kinematic variables
  ElasticKinematics el(beamEnergy, theta);

  // Z^0 deltaE-dependent part (from electron)
  double aExp = alpha/M_PI*(log(el.Q2()/(me*me)) - 1.);
  double aWeight = pow(el.eta(), aExp);

  // Z^1 deltaE-dependent part (from interference)
  double bExp = -2.*alpha/M_PI*beamCharge*log(el.eta());
  double bWeight = pow(4*pow(beamEnergy, 2)/(el.Q2()*el.x()), bExp);

  // Z^2 deltaE-dependent part (from proton)
  double cExp = alpha/M_PI*(el.E4()*log(el.x())/el.p4() - 1.);
  double cWeight = pow(4*pow(beamEnergy,2)/(mP*mP), cExp);

  //build deltaE
  // Calculate t (5.16 in Jan's thesis)
  double weightDeltaE = (aWeight*bWeight*cWeight);
  double t = 2.*(aExp + bExp + cExp);
  double maxDeltaE = el.E3() - me; // it can't lose more energy than its own mass.
  if ((useDeltaECut)&&(deltaECut < (el.E3()-me)))
    {
      maxDeltaE = deltaECut;
      weightDeltaE *= pow(maxDeltaE/(el.E3()-me),t);
    }
  double deltaE;
  double r = qrndNumbers[2];
  if (r<softFraction)
    {
      r = (r/softFraction);
      deltaE = pow(r,1./t) * maxDeltaE;
    }
  else
    {
      r = (r - softFraction)/(1. - softFraction);
      deltaE = r * maxDeltaE * beamEnergy / (beamEnergy + maxDeltaE * (r-1.));
    }
  double E3 = el.E3() - deltaE;
  double mom3 = sqrt(E3*E3 - me*me);
  // Include the weight
  double weightSoftFrac = 1./(softFraction*t + (1.-softFraction)*beamEnergy*(beamEnergy-maxDeltaE)*pow(deltaE/maxDeltaE,1.-t)
			      /((beamEnergy-deltaE)*(beamEnergy-deltaE)));
  
  //build photon direction
  double cosThetaK, thetaK, phiK, cosThetaEK, thetaEK, phiEK; // only two of these are independent
  // decide if we favor the incoming or outgoing lepton
  if (qrndNumbers[3] > 0.5)
    {
      // incoming lepton!
      phiK = (qrndNumbers[3] - 0.5) * 4. * M_PI;
      cosThetaK = genPhotonCosTh(beamEnergy,qrndNumbers[4]);
      thetaK = acos(cosThetaK);

      // Calculate the other two angles
      cosThetaEK = cosThetaEKfromGlob(theta,phi,thetaK,phiK);
      thetaEK = acos(cosThetaEK);
      phiEK = phiEKfromGlob(theta,phi,thetaK,phiK);
    }
  else
    {
      // outgoing lepton!
      phiEK = qrndNumbers[3] * 4 * M_PI;
      cosThetaEK = genPhotonCosTh(E3,qrndNumbers[4]);
      thetaEK = acos(cosThetaEK);

      // Calculate the other two angles
      cosThetaK = cosThetaKfromRel(theta, thetaEK, phiEK);
      thetaK = acos(cosThetaK);
      phiK = phiKfromRel(theta, phi, thetaEK, phiEK);
    }
  // Now produce the correct weight
  double kweight = (2 / (photonDirFcn(cosThetaK,beamEnergy)
				 + photonDirFcn(cosThetaEK,E3)));

  // Construct four-vectors
  p1 = ev->lepton_prescatter.momentum;
  p2.SetXYZM(0.,0.,0.,mP);
  p3.SetXYZM(mom3*sin(theta)*cos(phi),mom3*sin(theta)*sin(phi),mom3*cosTheta,me);
  kMod.SetXYZT(sin(thetaK)*cos(phiK),sin(thetaK)*sin(phiK),cosThetaK,1.);
  double momk = photonMom(cosTheta, cosThetaK, cosThetaEK, mom3, E3);
  k = kMod * momk;
  p4 = p1 + p2 - p3 - k;

  // Lastly, apply the cross-section
  // Note, the kinematic factor and the jacobian are the two places where we will neglect electron mass.
  double kinFactor = E3 * alphaCubedOver64PiSq / (mP * beamEnergy * fabs(mP + beamEnergy*(1.-cosTheta) - momk*(1.-cosThetaEK)));
  double jacobian = jacKDeltaE(cosThetaK, cosThetaEK, el.E3(), E3);

//   *****************************************************************
//     Set the multiple event weights
//   *****************************************************************

  // Set dipole weight
  double lep_dipole, mix_dipole, had_dipole;
  bremMatrixElement(lep_dipole,mix_dipole,had_dipole, 0, 0);
  double matElement_dipole = lep_dipole+mix_dipole+had_dipole;
  ev->weight.set_extra("method1_dipole", phaseweight * weightDeltaE * weightSoftFrac * kweight * cmSqMeVSq * matElement_dipole * kinFactor * jacobian * calcElasticCorr(el));

  // Calculate soft bremsstrahlung cross section
  TLorentzVector p3el, p4el; // Elastic 4-vectors
  p3el.SetXYZM(el.p3()*sin(theta)*cos(phi),el.p3()*sin(theta)*sin(phi),el.p3()*cosTheta,me);
  p4el = p1 + p2 - p3el;
  double softLept = p1*p1/((k*p1)*(k*p1)) - 2.*p1*p3el/((k*p1)*(k*p3el)) + p3el*p3el/((k*p3el)*(k*p3el));
  double softProt = p2*p2/((k*p2)*(k*p2)) - 2.*p2*p4el/((k*p2)*(k*p4el)) + p4el*p4el/((k*p4el)*(k*p4el));
  double softMix = p1*p2/((k*p1)*(k*p2)) - p1*p4el/((k*p1)*(k*p4el)) - p2*p3el/((k*p2)*(k*p3el)) + p3el*p4el/((k*p3el)*(k*p4el));
  // With Born CS factored out
  double softFactor = -alpha*momk*(softLept + 2*beamCharge*softMix + softProt)/(4*M_PI*M_PI);

  ev->weight.set_extra("method1_dipole_soft", phaseweight * weightDeltaE * weightSoftFrac * kweight * momk * softFactor * bornCrossSection(el, 0, 0) * jacobian * calcElasticCorr(el));


  // Set Jan FF weight
  double lep_Jan, mix_Jan, had_Jan;
  bremMatrixElement(lep_Jan,mix_Jan,had_Jan, 1, 1);
  double matElement_Jan = lep_Jan+mix_Jan+had_Jan;
  // Jan FF is the default weight
  ev->weight = phaseweight * weightDeltaE * weightSoftFrac * kweight * cmSqMeVSq * matElement_Jan * kinFactor * jacobian * calcElasticCorr(el);
  ev->weight.set_extra("method1_Jan_soft", phaseweight * weightDeltaE * weightSoftFrac * kweight * momk * softFactor * bornCrossSection(el, 1, 1) * jacobian * calcElasticCorr(el));

  return ev->weight.get_default();

  /*
  // GE: Upper error
  double lep_Jan_GEu, mix_Jan_GEu, had_Jan_GEu;
  bremMatrixElement(lep_Jan_GEu,mix_Jan_GEu,had_Jan_GEu, 3, 1);
  double matElement_Jan_GEu = lep_Jan_GEu+mix_Jan_GEu+had_Jan_GEu;
  ev->weight.set_extra("method1_Jan_GEu", phaseweight * weightDeltaE * weightSoftFrac * kweight * cmSqMeVSq * matElement_Jan_GEu * kinFactor * jacobian * calcElasticCorr(el));
  // GE: Lower error
  double lep_Jan_GEl, mix_Jan_GEl, had_Jan_GEl;
  bremMatrixElement(lep_Jan_GEl,mix_Jan_GEl,had_Jan_GEl, 4, 1);
  double matElement_Jan_GEl = lep_Jan_GEl+mix_Jan_GEl+had_Jan_GEl;
  ev->weight.set_extra("method1_Jan_GEl", phaseweight * weightDeltaE * weightSoftFrac * kweight * cmSqMeVSq * matElement_Jan_GEl * kinFactor * jacobian * calcElasticCorr(el));

  // GM: Upper error
  double lep_Jan_GMu, mix_Jan_GMu, had_Jan_GMu;
  bremMatrixElement(lep_Jan_GMu,mix_Jan_GMu,had_Jan_GMu, 1, 3);
  double matElement_Jan_GMu = lep_Jan_GMu+mix_Jan_GMu+had_Jan_GMu;
  ev->weight.set_extra("method1_Jan_GMu", phaseweight * weightDeltaE * weightSoftFrac * kweight * cmSqMeVSq * matElement_Jan_GMu * kinFactor * jacobian * calcElasticCorr(el));
  // GM: Lower error
  double lep_Jan_GMl, mix_Jan_GMl, had_Jan_GMl;
  bremMatrixElement(lep_Jan_GMl,mix_Jan_GMl,had_Jan_GMl, 1, 4);
  double matElement_Jan_GMl = lep_Jan_GMl+mix_Jan_GMl+had_Jan_GMl;
  ev->weight.set_extra("method1_Jan_GMl", phaseweight * weightDeltaE * weightSoftFrac * kweight * cmSqMeVSq * matElement_Jan_GMl * kinFactor * jacobian * calcElasticCorr(el));

  if (usePointProtonFF)
  {
    // Set point proton weight
    double lep_pp, mix_pp, had_pp;
    bremMatrixElement(lep_pp,mix_pp,had_pp, -1, -1);
    double matElement_pp = lep_pp+mix_pp+had_pp;
    ev->weight.set_extra("method1_pp", phaseweight * weightDeltaE * weightSoftFrac * kweight * cmSqMeVSq * matElement_pp * kinFactor * jacobian * calcElasticCorr(el));
  }

  if (useKellyFF)
  {
    // Set Kelly parameterization FF weight
    double lep_Kelly, mix_Kelly, had_Kelly;
    bremMatrixElement(lep_Kelly,mix_Kelly,had_Kelly, 2, 2);
    double matElement_Kelly = lep_Kelly+mix_Kelly+had_Kelly;
    ev->weight.set_extra("method1_Kelly", phaseweight * weightDeltaE * weightSoftFrac * kweight * cmSqMeVSq * matElement_Kelly * kinFactor * jacobian * calcElasticCorr(el));
  }

  // ** Mo+Tsai weight calculation **
  // Z^0 deltaE-dependent part (from electron)
  double aExpMoTsai = aExp;
  double aWeightMoTsai = pow(el.eta(), -2.*aExpMoTsai);
  // Z^1 deltaE-dependent part (from proton)
  double bExpMoTsai = bExp;
  double bWeightMoTsai = pow(el.eta(), -2.*bExpMoTsai);
  // Z^2 deltaE-dependent part (from proton)
  double cExpMoTsai = alpha/M_PI*(el.E4()/el.p4()*log((el.E4()+el.p4())/mP) - 1.);
  double cWeightMoTsai = pow(beamEnergy/mP, 2*cExpMoTsai);

  double moTsaiWeight = aWeightMoTsai * bWeightMoTsai * cWeightMoTsai;
  double moTsaiT = 2.*(aExpMoTsai+bExpMoTsai+cExpMoTsai);
  ev->weight.set_extra("method1_moTsai", phaseweight * weightSoftFrac * kweight * cmSqMeVSq * matElement_Jan * kinFactor * jacobian
		       * calcElasticCorrMoTsai(el) * moTsaiWeight * pow(maxDeltaE,t) * pow(el.E3()-me,-moTsaiT) * pow(deltaE,moTsaiT - t));

  // ** Meister+Yennie weight calculation **
  // Z^0 deltaE-dependent part (from electron)
  double aExpMeisYen = aExpMoTsai;
  double aWeightMeisYen = pow(el.eta(), aExpMeisYen);
  // Z^1 deltaE-dependent part (from proton)
  double bExpMeisYen = bExpMoTsai;
  double bWeightMeisYen = pow(sqrt(el.eta())*beamEnergy/el.E4(), bExpMeisYen);
  // Z^2 deltaE-dependent part (from proton)
  double cExpMeisYen = cExpMoTsai;
  double cWeightMeisYen = pow(beamEnergy*beamEnergy/mP/el.E4(), 2*cExpMeisYen);

  double meisYenWeight = aWeightMeisYen * bWeightMeisYen * cWeightMeisYen;
  double meisYenT = 2.*(aExpMeisYen+bExpMeisYen+cExpMeisYen);
  ev->weight.set_extra("method1_meisYen", phaseweight * weightSoftFrac * kweight * cmSqMeVSq * matElement_Jan * kinFactor * jacobian
		       * calcElasticCorrMeisYen(el) * meisYenWeight * pow(maxDeltaE,t) * pow(el.E3()-me,-meisYenT) * pow(deltaE,meisYenT-t));

  // Vacuum polarization weights
  double corr_wo_vpol = calcElasticCorr(el)/exp((2.*alpha/M_PI)*(-5./9. + TMath::Log(el.Q2()/(me*me))/3.));
  double factor_vpolLep = corr_wo_vpol/pow(1.-(getVpolLep(el.Q2(), me) + getVpolLep(el.Q2(), 105.658) + getVpolLep(el.Q2(), 1776.82))/2.,2);
  double factor_vpolFull = corr_wo_vpol/pow(1.-(inter_vpol->Eval(el.Q2()))/2.,2);
  ev->weight.set_extra("method1_vpolLep", phaseweight * weightDeltaE * weightSoftFrac * kweight * cmSqMeVSq * matElement_Jan * kinFactor * jacobian * factor_vpolLep);
  ev->weight.set_extra("method1_vpolFull", phaseweight * weightDeltaE * weightSoftFrac * kweight * cmSqMeVSq * matElement_Jan * kinFactor * jacobian * factor_vpolFull);
  */

  // ** Non-exponentiated weight calculations **
  // Kinematics are approximately elastic, use soft photon

  // To divide out the sampling distribution
  double otherWeight = weightSoftFrac*kweight*phaseweight*pow(maxDeltaE/deltaE,t);
  /*
  // Method 2 -- k cut
  double weight2_MTj;
  double weight2_MoT;
  double weight2_vpol; // MTj but with vacuum polarization from data
  double weight2_soft;
  double weight2_dipole;
  double weight2_Born;
  if (momk < k_cut)
  {
    double elastic_corr_MoT = inter_brem_ee->Eval(cosTheta)
                             -beamCharge*inter_brem_ep->Eval(cosTheta)
                             +inter_brem_pp->Eval(cosTheta)
                             +inter_virt->Eval(cosTheta);
    double elastic_corr_MTj = elastic_corr_MoT-beamCharge*inter_prime->Eval(cosTheta);
    double elastic_corr_vpol = elastic_corr_MTj -(2.*alpha/M_PI)*(-5./9. + TMath::Log(el.Q2()/(me*me))/3.) + inter_vpol->Eval(el.Q2()); // Subtract off e polarization term, add in total polarization from data

    double bornweight = bornCrossSection(el, 1, 1)/inter_sample->Eval(theta)*phaseweight;
    weight2_MTj = bornweight*(1.+elastic_corr_MTj);
    weight2_MoT = bornweight*(1.+elastic_corr_MoT);
    weight2_vpol = bornweight*(1.+elastic_corr_vpol);
    weight2_soft = weight2_MTj;
    weight2_dipole = bornCrossSection(el, 0, 0)*(1.+elastic_corr_MTj)/inter_sample->Eval(theta)*phaseweight;
    weight2_Born = bornCrossSection(el, 1, 1)/inter_sample->Eval(theta)*phaseweight;
  }
  // Use inelastic calculation
  else {
    weight2_MTj = cmSqMeVSq * matElement_Jan * kinFactor * jacobian * otherWeight;
    weight2_MoT = weight2_MTj;
    weight2_vpol = weight2_MTj;
    weight2_soft = momk * softFactor * bornCrossSection(el, 1, 1) * jacobian * otherWeight;
    weight2_dipole = cmSqMeVSq * matElement_dipole * kinFactor * jacobian * otherWeight;
    weight2_Born = 0;
  }
  ev->weight.set_extra("method2", weight2_MTj);
  ev->weight.set_extra("method2_moTsai", weight2_MoT);
  ev->weight.set_extra("method2_vpol", weight2_vpol);
  ev->weight.set_extra("method2_soft", weight2_soft);
  ev->weight.set_extra("method2_dipole", weight2_dipole);
  ev->weight.set_extra("method2_Born", weight2_Born);

  // Method 3 - DeltaE cut
  double weight3, weight3_soft, weight3_exp, weight3_Born;
  if (deltaE<method3_deltaE_cut)
  {
    // Integral over deltaE and photon angles of sampling distribution
    double sampleintegral = softFraction*pow(method3_deltaE_cut/maxDeltaE,t)+(1.-softFraction)*method3_deltaE_cut*(beamEnergy-maxDeltaE)/(maxDeltaE*(beamEnergy-method3_deltaE_cut));

    weight3 = bornCrossSection(el, 1, 1)*(1.+getMTjDelta(el, method3_deltaE_cut))/sampleintegral*phaseweight;
    weight3_soft = weight3;
    weight3_Born = bornCrossSection(el, 1, 1)/sampleintegral*phaseweight;
  }
  else
  {
    weight3 = cmSqMeVSq * matElement_Jan * kinFactor * jacobian * otherWeight;
    weight3_soft = momk * softFactor * bornCrossSection(el, 1, 1) * jacobian * otherWeight;
    weight3_Born = 0;
  }

  ev->weight.set_extra("method3", weight3);
  ev->weight.set_extra("method3_soft", weight3_soft);
  ev->weight.set_extra("method3_Born", weight3_Born);
  */
  // ***************************************************************** 
  
  // Create and pushback the Generator Particles
  GeneratorParticle e,p;
  e.particle=ev->lepton_prescatter.particle;
  e.momentum = p3;
  ev->particles.push_back(e);

  p.particle="proton";
  p.momentum = p4;
  ev->particles.push_back(p);

  if (pushPhoton){
    GeneratorParticle kParticle;
    kParticle.particle="gamma";
    kParticle.momentum = k;
    ev->particles.push_back(kParticle);
  }
  
}

void GeneratorRadiative::bremMatrixElement(double &lep, double &mix, double &had, int FFTypeE, int FFTypeM)
{
  // We'll need slash matrices
  FourMat p1Slash = pGamma->slash(p1);
  FourMat p2Slash = pGamma->slash(p2);
  FourMat p3Slash = pGamma->slash(p3);
  FourMat p4Slash = pGamma->slash(p4);
  FourMat kSlash = pGamma->slash(k);

  //std::cout << p3.T() << " " << p3.X() << " " << p3.Y() << " " << p3.Z() << "\n";

  // We'll need matrices with added mass terms:
  FourMat p1AndMass = *pI4; p1AndMass *= me; p1AndMass += p1Slash;
  FourMat p2AndMass = *pI4; p2AndMass *= mP; p2AndMass += p2Slash;
  FourMat p3AndMass = *pI4; p3AndMass *= me; p3AndMass += p3Slash;
  FourMat p4AndMass = *pI4; p4AndMass *= mP; p4AndMass += p4Slash;
  
  // Fill the proton currents
  OneIndex pcK,pcmK,pc2m4,pc4m2,pc2m4mK,pc4Km2;
  fillProtonCurrent(pcK,k,FFTypeE,FFTypeM);
  fillProtonCurrent(pcmK,-k,FFTypeE,FFTypeM);
  fillProtonCurrent(pc2m4,p2-p4,FFTypeE,FFTypeM);
  fillProtonCurrent(pc4m2,p4-p2,FFTypeE,FFTypeM);
  fillProtonCurrent(pc2m4mK,p2-p4-k,FFTypeE,FFTypeM);
  fillProtonCurrent(pc4Km2,p4+k-p2,FFTypeE,FFTypeM);

  // Fill the Dirac Propagator Terms
  FourMat p1DP = *pI4; p1DP *= me; p1DP += (p1Slash - kSlash); p1DP *= (0.5/p1.Dot(kMod));
  FourMat p2DP = *pI4; p2DP *= mP; p2DP += (p2Slash - kSlash); p2DP *= (0.5/p2.Dot(kMod));
  FourMat p3DP = *pI4; p3DP *= me; p3DP += (p3Slash + kSlash); p3DP *= (0.5/p3.Dot(kMod));
  FourMat p4DP = *pI4; p4DP *= mP; p4DP += (p4Slash + kSlash); p4DP *= (0.5/p4.Dot(kMod));


  //precompute some quantities
  OneIndex p1gamma, p3gamma, p2px2m4,p4px2m4;
  //  FourMat p1gamma[4],p3gamma[4],p2px2m4[4],p4px2m4[4];
  for (int la=0 ; la<4; la++)
    {
      p1gamma.set(la,p1AndMass.multGammaOnRight(la));
      p2px2m4.set(la,p2AndMass * pc2m4.get(la));
      p3gamma.set(la,p3AndMass.multGammaOnRight(la));
      p4px2m4.set(la,p4AndMass * pc4m2.get(la));
    }

  // Create the simple two tensors and two index objects
  TwoTensor bareLepTensor, bareHadTensor;
  TwoIndex ATensor, BTensor, CTensor;
  for (int ka=0 ; ka < 4 ; ka++)
    {
      FourMat T1=p3DP.multGammaOnLeft(ka);
      FourMat T2=p1DP.multGammaOnRight(ka);
      FourMat T3=pcmK.get(ka) * p4DP;
      FourMat T4=p2DP * pcmK.get(ka);
      FourMat T5=pc2m4mK.get(ka) * p4DP;
      FourMat T6= p2DP * pc2m4mK.get(ka);
      for (int nu=0 ; nu < 4 ; nu++)
	{
	  bareLepTensor.set(ka,nu,p1gamma.get(ka).multr(p3gamma.get(nu)));
	  bareHadTensor.set(ka,nu,p2px2m4.get(ka).multr(p4px2m4.get(nu)));
	  ATensor.set(ka,nu,T1.multGammaOnRight(nu) - T2.multGammaOnLeft(nu));
	  BTensor.set(ka,nu,(T3 * pc4Km2.get(nu)) - (pc4Km2.get(nu) * T4));
	  CTensor.set(ka,nu,(T5 * pcK.get(nu)) - (pcK.get(nu) *T6));
	}
    }
  // Create the three tensor objects
  ThreeTensor lep3Tensor1, lep3Tensor2, had3Tensor1, had3Tensor2;
  
  // Do some precomputation
  TwoIndex ti1=ATensor.leftMultiply(p1AndMass);
  TwoIndex ti2=CTensor.leftMultiply(p2AndMass);
  TwoIndex ti3=ATensor.leftMultiply(p3AndMass);
  TwoIndex ti4=BTensor.leftMultiply(p4AndMass);

  for (int ka=0 ; ka < 4 ; ka++)
    for (int nu=0 ; nu < 4 ; nu++)
	for (int la=0 ; la<4; la++)
	  {
	    lep3Tensor1.set(la,ka,nu,ti3.get(ka,nu).multr(p1gamma.get(la)));
	    lep3Tensor2.set(ka,nu,la,ti1.get(ka,nu).multr(p3gamma.get(la)));
	    had3Tensor1.set(ka,nu,la,ti2.get(ka,nu).multr(p4px2m4.get(la)));
	    had3Tensor2.set(la,ka,nu,ti4.get(ka,nu).multr(p2px2m4.get(la)));
	  }  
  // Fill the pure-bremstrahlung tensors
  TwoTensor bremLepTensor, bremHadTensor;
  bremLepTensor = (ti1.contractSecondFirstOnRightTr(ti3));
  bremHadTensor = (ti2.contractSecondFirstOnRightTr(ti4));
 
  // Contract the tensors to get the matrix element terms
  std::complex<double> lepTerm = bremLepTensor.contractInOrder(bareHadTensor);
  std::complex<double> mixTerm = lep3Tensor1.contract123(had3Tensor1) + lep3Tensor2.contract123(had3Tensor2);
  std::complex<double> hadTerm = bareLepTensor.contractInOrder(bremHadTensor);

  // Divide out the hard-photon propagator denominators. Minus sign comes from photon spin sum.
  double p1mp3mk = (p1 - p3 - k).Mag2();
  double p1mp3 = (p1 - p3).Mag2();
  lepTerm /= (-p1mp3mk*p1mp3mk);
  mixTerm /= (-p1mp3mk*p1mp3);
  hadTerm /= (-p1mp3*p1mp3);

  // Form the matrix element, taking care to get the sign right on the mixterm.
  mixTerm *= beamCharge;
  lep = std::real(lepTerm);
  mix = std::real(mixTerm);
  had = std::real(hadTerm);

  // Test if the terms make sense
  if (fabs(mix) > (lep+had))
    {
      std::cout << "Our matrix element isn't right.  Mixed term is too large!\n";
      std::cout << "Lepterm " << std::real(lepTerm) << " + " << std::imag(lepTerm) << " i\n";
      std::cout << "Mixterm " << std::real(mixTerm) << " + " << std::imag(mixTerm) << " i\n";
      std::cout << "Hadterm " << std::real(hadTerm) << " + " << std::imag(hadTerm) << " i\n";
      std::cout << "M1: " << p1.Mag() << "\n";
      std::cout << "M2: " << p2.Mag() << "\n";
      std::cout << "M3: " << p3.Mag() << "\n";
      std::cout << "M4: " << p4.Mag() << "\n";
      std::cout << "MK: " << k.Mag() << "\n";
      std::cout << "E1+E2-E3-E4-k: " << (p1+p2-p3-p4-k).T() << " " 
		<< (p1+p2-p3-p4-k).X() << " " << (p1+p2-p3-p4-k).Y() << " " << (p1+p2-p3-p4-k).Z() << "\n";
      exit(-1);
    }
}

double GeneratorRadiative::photonMom(double cosThetaE, double cosThetaK, double cosThetaEK, double mom3, double E3)
{
  double mom1 = sqrt(beamEnergy*beamEnergy - me*me);

  return (me*me + mP*(beamEnergy - E3) - (beamEnergy*E3 - mom1*mom3*cosThetaE)) /
    (mP + beamEnergy - mom1*cosThetaK - E3 + mom3*cosThetaEK);
}

double GeneratorRadiative::jacKDeltaE(double cosThetaK, double cosThetaEK, double Eel, double E3)
{
  return fabs((mP + beamEnergy * (1.-cosThetaK) - Eel * (1.-cosThetaEK))/((mP + beamEnergy * (1.-cosThetaK) - E3 * (1.-cosThetaEK))));
}

double GeneratorRadiative::getLeptonSlowness(double lEnergy) const
{
  // Lepton slowness := oneOverBeta-1 = 1/sqrt(1-oneOverGammaSq)-1
  const double oneOverGammaSq = me*me/(lEnergy*lEnergy);
  // Use the Taylor expansion to not lose precision if oneOverGammaSq is small
  if (oneOverGammaSq<0.0001)
    return oneOverGammaSq*(0.5+oneOverGammaSq*(0.375+oneOverGammaSq*(5./16.+oneOverGammaSq*35./128.)));
  else
    return 1./sqrt(1.-oneOverGammaSq)-1.;
}

double GeneratorRadiative::photonDirFcn(double cosTheta, double lEnergy) const
{
  const double y = getLeptonSlowness(lEnergy);
  const double norm = 4*M_PI*((1.+y)*log(2./y+1.)-2.);
  const double denomTerm = 1. + y - cosTheta;

  return (1 - cosTheta * cosTheta)/(denomTerm*denomTerm*norm);
}

double GeneratorRadiative::photonDirCumDist(double cosTheta, double lEnergy) const
{
  const double y = getLeptonSlowness(lEnergy);
  const double norm = 2*((1.+y)*log(2./y+1.)-2);
  const double logTerm = 2*(1.+y)*log((2.+y)/(1.-cosTheta+y));
  const double otherTerm = -(1.+cosTheta)*(1.+y/(1.-cosTheta+y));

  return (logTerm+otherTerm)/norm;
}

double GeneratorRadiative::cosThetaKfromRel(double thetaE, double thetaEK, double phiEK) const
{
  return (cos(thetaEK)*cos(thetaE) - sin(thetaEK)*cos(phiEK)*sin(thetaE));
}

double GeneratorRadiative::phiKfromRel(double thetaE, double phiE, double thetaEK, double phiEK) const
{
  double cosThetaK = cosThetaKfromRel(thetaE, thetaEK, phiEK);
  double sinThetaK = sqrt(1 - cosThetaK*cosThetaK);
  double temp = (cos(thetaEK)-cos(thetaE)*cosThetaK)/(sin(thetaE)*sinThetaK);
  
  // Do some sanitizing
  if (temp > 1.) temp = 1.; 
  if (temp < -1.) temp = -1.;
  
  if (sin(phiEK)>0)
    return phiE + acos(temp);
  //else
  return phiE - acos(temp);
}

double GeneratorRadiative::cosThetaEKfromGlob(double thetaE, double phiE, double thetaK, double phiK) const
{
  return (sin(thetaE)*sin(thetaK)*cos(phiE-phiK) + cos(thetaE)*cos(thetaK));
}

double GeneratorRadiative::phiEKfromGlob(double thetaE, double phiE, double thetaK, double phiK) const
{
  double cosThetaEK = cosThetaEKfromGlob(thetaE, phiE, thetaK,phiK);
  double sinThetaEK = sqrt(1 - cosThetaEK*cosThetaEK);

  double temp = -(cos(thetaK)-cosThetaEK*cos(thetaE))/(sin(thetaE)*sinThetaEK);
  
  // do some sanitizing
  if (temp > 1.) temp = 1.; 
  if (temp < -1.) temp = -1.;

  if (sin(phiK-phiE)>0)
    return acos(temp);
  //else
  return 2*M_PI - acos(temp);
}

double GeneratorRadiative::genPhotonCosTh(double lEnergy, double real) const
{
  double cosThMin = -1;
  double cosThMax = 1;
  double cosTheta = (cosThMin + cosThMax)/2.;
  double result = photonDirCumDist(cosTheta,lEnergy);

  while (fabs(real - result) > 0.00000001)
  { 
    if (real > result)
      cosThMin = cosTheta;
    else
      cosThMax = cosTheta;

    cosTheta = (cosThMin + cosThMax)/2.;
    result = photonDirCumDist(cosTheta,lEnergy);
  }

  return cosTheta;
}

double GeneratorRadiative::diracFormFactor(double QSq, int FFTypeE, int FFTypeM)
{
  double tau = QSq/(4.*mP*mP);
  return (electricFormFactor(QSq, FFTypeE)+tau*magneticFormFactor(QSq, FFTypeM))/(1+tau);
}

double GeneratorRadiative::pauliFormFactor(double QSq, int FFTypeE, int FFTypeM)
{
  double tau = QSq/(4.*mP*mP);
  return (magneticFormFactor(QSq, FFTypeM)-electricFormFactor(QSq, FFTypeE))/(1+tau);
}

double GeneratorRadiative::dipoleFormFactor(double QSq)
{
  double temp = 1.+QSq/qSqDipole;
  return 1./(temp * temp);
}

double GeneratorRadiative::electricFormFactor(double QSq, int formFactorType)
{
  // Point-like proton
  if (formFactorType==-1)
    return 1.;
  // Dipole form factor case
  if (formFactorType==0)
    return dipoleFormFactor(QSq);
  // Jan's spline best fit
  if (formFactorType==1)
    return inter_splineGE->Eval(QSq);
  // G_E parameterization from Kelly (2004)
  if (formFactorType==2)
  {
    double tau = QSq/(4.*mP*mP);

    double a1 = -0.24;
    double b1 = 10.98;
    double b2 = 12.82;
    double b3 = 21.97;

    return (1+a1*tau)/(1+b1*tau+b2*tau*tau+b3*pow(tau,3));
  }
  // Jan spline fit upper error
  if (formFactorType==3)
    return inter_splineGEupper->Eval(QSq);
  // Jan spline fit lower error
  if (formFactorType==4)
    return inter_splineGElower->Eval(QSq);

  std::cerr << "The index '" << formFactorType << "' does not correspond to a form factor type!" << std::endl;
  return 1.;
}

double GeneratorRadiative::magneticFormFactor(double QSq, int formFactorType)
{
  // Point-like proton
  if (formFactorType==-1)
    return muP;
  // Dipole form factor case
  if (formFactorType==0)
    return muP*dipoleFormFactor(QSq);
  // Jan's spline best fit
  if (formFactorType==1)
    return muP*inter_splineGM->Eval(QSq);
  // G_M parameterization from Kelly (2004)
  if (formFactorType==2)
  {
    double tau = QSq/(4.*mP*mP);

    double a1 = 0.12;
    double b1 = 10.97;
    double b2 = 18.86;
    double b3 = 6.55;

    return muP*(1+a1*tau)/(1+b1*tau+b2*tau*tau+b3*pow(tau,3));
  }
  // Jan spline fit upper bound
  if (formFactorType==3)
    return muP*inter_splineGMupper->Eval(QSq);
  // Jan spline fit lower bound
  if (formFactorType==4)
    return muP*inter_splineGMlower->Eval(QSq);

  std::cerr << "The index '" << formFactorType << "' does not correspond to a form factor type!" << std::endl;
  return muP;
}

void GeneratorRadiative::fillProtonCurrent(OneIndex &current, const TLorentzVector &photon, int FFTypeE, int FFTypeM)
{
  double QSq = fabs(photon.Mag2());

  current = pSigma->contractSecond(photon);
  current *= std::complex<double>(0.,pauliFormFactor(QSq, FFTypeE, FFTypeM)/2.0/mP);
  OneIndex temp(*pGamma);
  temp *= diracFormFactor(QSq, FFTypeE, FFTypeM);
  current += temp; 
}

double GeneratorRadiative::bornCrossSection(const ElasticKinematics &e, int FFTypeE, int FFTypeM) // in units of cm^2 / sr
{
  const double constantFactor = 2.*alpha*alpha * cmSqMeVSq; //2.0*alpha*alpha*(cm*MeV)^2
  double gE = electricFormFactor(e.Q2(), FFTypeE);
  double gM =  magneticFormFactor(e.Q2(), FFTypeM);
  const double csMott = constantFactor*e.E3()*e.E3()*e.E3()*(1.+cos(e.theta()))/(e.Q2()*e.Q2()*beamEnergy);
  return csMott* (e.eps()*gE*gE + e.tau()*gM*gM) / (e.eps()*(1.+e.tau()) );
}

// Integrate over P(deltaE) and P(Omega_gamma)
double GeneratorRadiative::intSampletoKcut::operator()(const double *x) const
{
  double cosThetaK = x[0];
  double phiK = x[1];

  double E1 = g.beamEnergy;
  double cosTheta = cos(g.interTheta);
  double cosThetaEK = cosTheta*cosThetaK+sin(g.interTheta)*sin(acos(cosThetaK))*cos(phiK);
  double k = g.k_cut;

  // Calculate deltaE for this k_cut
  double inelasticE = (g.mP*(E1-k)-E1*k*(1.-cosThetaK))/(g.mP+E1*(1.-cosTheta)-k*(1.-cosThetaEK));
  double deltaE_k = g.inter_elE-inelasticE;

  // deltaE integral
  double deltaEint = g.softFraction*pow(deltaE_k/g.inter_maxDeltaE,g.inter_t)+(1-g.softFraction)*deltaE_k*(E1-g.inter_maxDeltaE)/(g.inter_maxDeltaE*(E1-deltaE_k));
  // Photon angle part of integrand
  double avgE3 = (g.inter_elE+inelasticE)/2.;
  double ksample = (g.photonDirFcn(cosThetaK,E1) + g.photonDirFcn(cosThetaEK,avgE3))/2.;

  return deltaEint*ksample;
}

double GeneratorRadiative::Btilde::operator()(double x) const
{
  TLorentzVector p_x = x*v1+(1.-x)*v2;
  return (TMath::Log(4.*g.k_cut*g.k_cut/(p_x*p_x)) + (p_x.E())*TMath::Log((p_x.E() - (p_x.Vect()).Mag())/(p_x.E() + (p_x.Vect()).Mag()))/((p_x.Vect()).Mag()))/(p_x*p_x);
}

double GeneratorRadiative::d_p1_p1() // B(p1, p1, k_cut)
{
  return 0.5*(TMath::Log(2.*k_cut/me) + 
	      beamEnergy*TMath::Log(me/(beamEnergy + TMath::Sqrt(beamEnergy*beamEnergy - me*me)))
	      /TMath::Sqrt(beamEnergy*beamEnergy - me*me)
	      )/M_PI;
}

double GeneratorRadiative::d_p1_p2() // B(p1, p2, k_cut)
{
  return (p1*p2)*i_p1_p2->Integral(0., 1.)/(4.*M_PI);
}

double GeneratorRadiative::d_p1_p3() // B(p1, p3, k_cut)
{
  return (p1*p3)*i_p1_p3->Integral(0., 1.)/(4.*M_PI);
}

double GeneratorRadiative::d_p1_p4() // B(p4, p4, k_cut)
{
  return (p1*p4)*i_p1_p4->Integral(0., 1.)/(4.*M_PI);
}

double GeneratorRadiative::d_p2_p2() // B(p2, p2, k_cut)
{
  return (TMath::Log(2.*k_cut/mP) - 1.)/(2.*M_PI);
}

double GeneratorRadiative::d_p2_p3() // B(p2, p3, k_cut)
{
  return (p2*p3)*i_p2_p3->Integral(0., 1.)/(4.*M_PI);
}

double GeneratorRadiative::d_p2_p4() // B(p2, p4, k_cut)
{
  return (p2*p4)*i_p2_p4->Integral(0., 1.)/(4.*M_PI);
}

double GeneratorRadiative::d_p3_p3(double E3) // B(p3, p3, k_cut)
{
  return 0.5*(TMath::Log(2.*k_cut/me) + E3*TMath::Log(me/(E3 + TMath::Sqrt(E3*E3 - me*me)))/TMath::Sqrt(E3*E3 - me*me))/M_PI;
}

double GeneratorRadiative::d_p3_p4() // B(p3, p4, k_cut)
{
  return (p3*p4)*i_p3_p4->Integral(0., 1.)/(4.*M_PI);
}

double GeneratorRadiative::d_p4_p4(double E4) // B(p4, p4, k_cut)
{
  return 0.5*(TMath::Log(2.*k_cut/mP) + E4*TMath::Log(mP/(E4 + TMath::Sqrt(E4*E4 - mP*mP)))/TMath::Sqrt(E4*E4 - mP*mP))/M_PI;
}
