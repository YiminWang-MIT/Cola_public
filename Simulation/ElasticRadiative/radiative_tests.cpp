#include <iostream>
#include <iomanip>
#include <fstream>
#include "TFile.h"
#include "TH2.h"
#include "gsl/gsl_rng.h"

#include "radiative.h"

void GeneratorRadiative::test1()
{
  // Some kinematic variables that we want to fix
  beamEnergy = 2010.;
  double thetaE = 30. * M_PI / 180.;
  double cosThetaE = cos(thetaE);
  double phiE = 180. * M_PI / 180.;
  ElasticKinematics el(beamEnergy, thetaE);
  double elasticE = el.E3();
  double deltaE = 10.;
  double E3 = elasticE - deltaE;
  double mom3 = sqrt(E3*E3 - me*me);
  double mom1 = sqrt(beamEnergy * beamEnergy - me*me);

  // Prepare the four-vectors
  p1.SetXYZM(0.,0.,mom1,me);
  p2.SetXYZM(0.,0.,0.,mP);
  p3.SetXYZM(mom3*sin(thetaE)*cos(phiE),mom3*sin(thetaE)*sin(phiE),mom3*cosThetaE,me);

  // Let's make an output file
  std::ofstream outfile("csTest.txt");
  outfile << "# thetaE fixed at " << thetaE * 180. / M_PI << " degrees\n"
      << "# phiE fixed at " << phiE * 180. / M_PI << " degrees\n"
      << "# deltaE fixed at " << deltaE << " MeV\n"
      << "# Column 1: thetaK [deg]\n"
      << "# Column 2: phiK [deg]\n"
      << "# Column 3: Modified M^2 (e-)\n"
      << "# Column 4: Modified M^2 (e+)\n"
      << "# Column 5: momk [MeV]\n"
      << "# Column 6: Kinematic Factor [cm^2]\n"
      << "# Column 7: Jacobian\n"
      << "# Column 8: Photon Prob Function\n";

  // Let's loop over thetaK and phiK
  for (double tK=0.001; tK<=180; tK+=1.)
  {
    std::cout << "Processing thetaK = " << tK << std::endl;
    double thetaK = tK * M_PI / 180.;
    double cosThetaK = cos(thetaK);

    for (double pK=0; pK <= 360; pK += 1.)
    {
      double phiK = pK * M_PI / 180.;

      double cosThetaEK = cosThetaEKfromGlob(thetaE,phiE,thetaK,phiK);
      //double thetaEK = acos(cosThetaEK); // We might need this at some point.
      //double phiEK = phiEKfromGlob(thetaE,phiE,thetaK,phiK); // We might need this too, just not yet.
      double photonProb = 0.5*photonDirFcn(cosThetaK,beamEnergy)+0.5*photonDirFcn(cosThetaEK,E3);

      // Construct four-vectors
      kMod.SetXYZT(sin(thetaK)*cos(phiK),sin(thetaK)*sin(phiK),cosThetaK,1.);
      double momk = photonMom(cosThetaE, cosThetaK, cosThetaEK, mom3, E3);
      k = kMod * momk;
      p4 = p1 + p2 - p3 - k;

      double kinFactor = E3 * alphaCubedOver64PiSq / (mP * beamEnergy * fabs(mP + beamEnergy*(1.-cosThetaE) - momk*  (1.-cosThetaEK)));
      double jacobian = jacKDeltaE(cosThetaK, cosThetaEK, elasticE, E3);

      beamCharge = 1;
      double lep, mix, had;
      bremMatrixElement(lep,mix,had,0,0);
      double pMat = lep + mix + had;
      double eMat = lep - mix + had;

      outfile << tK << " " << pK << " " << eMat << " " << pMat << " " << momk << " "
          << (cmSqMeVSq * kinFactor) << " " << jacobian << " "
          << photonProb << std::endl;
    }
  }
  outfile.close();
}

void GeneratorRadiative::test2()
{
  // Open an output file
  std::ofstream outfile("csData.txt");
  outfile << "#Column 1: thetaE [deg]\n";
  outfile << "#Column 2: deltaE [MeV]\n";
  outfile << "#Column 3: CS [cm^2 / sr / MeV]\n\n";

  // Establish some fixed kinematics
  beamEnergy = 2010.;
  beamCharge = -1;
  double phiE = 0.;

  // Loop over thetaE and deltaE
  double tE = 20.;
  //for (double tE = 10. ; tE<=100. ; tE+=10.)
    {
      std::cout << "Processing thetaE = " << tE << std::endl;
      double thetaE = tE * M_PI / 180.;
      ElasticKinematics el(beamEnergy, thetaE);

      for (double deltaE = 10. ; deltaE <= 100. ; deltaE += 10.)
    //for (double deltaE = .1 ; deltaE < elasticE ; deltaE += .1)
    //      for (double deltaE = 1. ; deltaE < elasticE ; deltaE += (elasticE/10.))
      {
    double integral = test_integral(thetaE, phiE, deltaE, el);
    outfile << tE << " " << deltaE << " " << integral << "\n";
      }
      outfile << "\n\n";
    }
}

double GeneratorRadiative::test_integral(double thetaE, double phiE, double deltaE, ElasticKinematics &el)
{
  // Prepare the fixed kinematics
  double cosThetaE = cos(thetaE);
  double E3 = el.E3() - deltaE;
  double mom3 = sqrt(E3*E3 - me*me);
  p1.SetXYZM(0.,0.,sqrt(beamEnergy*beamEnergy-me*me),me);
  p2.SetXYZM(0.,0.,0.,mP);
  p3.SetXYZM(mom3*sin(thetaE)*cos(phiE),mom3*sin(thetaE)*sin(phiE),mom3*cos(thetaE),me);

  // Prepare the integral
  double integral = 0.;

  int nSteps = 1000;
  double dSteps = ((double) nSteps);
  double dCosThetaK = (2. / dSteps);
  double dPhiK = (2. * M_PI / dSteps);

  // Loop over the photon direction
  bool firstTime = true;
  double lastCS = 0.;
  for (int i=0 ; i <= nSteps ; i++)
  {
    double cosThetaK = -1. + dCosThetaK * i;
    double thetaK = acos(cosThetaK);

    for (int j=0 ; j <= nSteps ; j++)
    {
      double phiK = 0. + dPhiK * j;
      double cosThetaEK = cosThetaEKfromGlob(thetaE,phiE,thetaK,phiK);
      //double thetaEK = acos(cosThetaEK);  // Not needed quite yet
      //double phiEK = phiEKfromGlob(thetaE,phiE,thetaK,phiK); // Not needed quite yet
      double momk = photonMom(cosThetaE, cosThetaK, cosThetaEK, mom3, E3);

      // Construct remaining 4 vectors
      kMod.SetXYZT(sin(thetaK)*cos(phiK),sin(thetaK)*sin(phiK),cosThetaK,1.);
      k = kMod * momk;
      p4 = p1 + p2 - p3 - k;
      /*
      std::cout << "New calculation\n";
      std::cout << "\tthetaE = " << thetaE * 180. / M_PI << "\n";
      std::cout << "\tdeltaE = " << deltaE << "\n";
      std::cout << "\tcos(thetaK) = " << cosThetaK << "\n";
      std::cout << "\tthetaK = " << (thetaK * 180. / M_PI) << "\n";
      std::cout << "\tphiK = " << (phiK * 180. / M_PI) << "\n";
      std::cout << "\tK = " << momk << "\n";
      */
      // Calculate the cross section
      double lep, mix, had;
      bremMatrixElement(lep,mix,had,0,0);

      double kinFactor = E3 * alphaCubedOver64PiSq / (mP * beamEnergy * fabs(mP + beamEnergy*(1.-cosThetaE) - momk*  (1.-cosThetaEK)));
      double jacobian = jacKDeltaE(cosThetaK, cosThetaEK, el.E3(), E3);
      double cs = (cmSqMeVSq * (lep+mix+had) * kinFactor * jacobian / deltaE);

      //std::cout << "\tcs = " << cs << "\n";

      // Add the cross section to the integral
      if (firstTime)
        firstTime = false;
      else
        integral += 0.5*(cs+lastCS)*dPhiK*dCosThetaK;

      lastCS = cs;
    }
  }

  return integral;
}

void GeneratorRadiative::test3()
{
  double energies[4] = {1000.0, 2000.0, 4000.0, 8000.0};

  std::ofstream out("born_data.txt");

  out << "# Test of the born cross section\n";
  out << "# Column 1: theta [deg]\n";
  out << "# Column 2: rosenbluth formula [cm^2 / sr]\n";
  out << "# Column 3: radiative helpers [cm^2 / sr]\n";

  for (int i=0 ; i<4 ; i++)
    {
      out << "\n\n";

      beamEnergy = energies[i];

      for (int j=1 ; j < 180 ; j++)
	{
	  double thetaE = (double)j;
	  double thetaERad = thetaE * M_PI / 180.;
	  ElasticKinematics el(beamEnergy, thetaERad);
	  out << thetaE << " " << bornCrossSection(el,0,0) << " " << test_born(thetaERad) << "\n";
	}
    }

  out.close();
}

double GeneratorRadiative::test_born(double thetaE)
{
  double phiE=0.;
  double eMom=beamEnergy*mP/(mP + beamEnergy*(1. - cos(thetaE)));

  p1.SetXYZM(0.,0.,beamEnergy,me);
  p2.SetXYZM(0.,0.,0.,mP);
  p3.SetXYZM(eMom*sin(thetaE)*cos(phiE),eMom*sin(thetaE)*sin(phiE),eMom*cos(thetaE),me);
  p4 = p1 + p2 - p3;

  // We'll need slash matrices
  FourMat p1Slash = pGamma->slash(p1);
  FourMat p2Slash = pGamma->slash(p2);
  FourMat p3Slash = pGamma->slash(p3);
  FourMat p4Slash = pGamma->slash(p4);

  // We'll need matrices with added mass terms:
  FourMat p1AndMass = *pI4; p1AndMass *= me; p1AndMass += p1Slash;
  FourMat p2AndMass = *pI4; p2AndMass *= mP; p2AndMass += p2Slash;
  FourMat p3AndMass = *pI4; p3AndMass *= me; p3AndMass += p3Slash;
  FourMat p4AndMass = *pI4; p4AndMass *= mP; p4AndMass += p4Slash;
  
  // Fill the proton currents
  OneIndex pc2m4,pc4m2;
  fillProtonCurrent(pc2m4,p2-p4,0,0);
  fillProtonCurrent(pc4m2,p4-p2,0,0);

  // Create the simple two tensors and two index objects
  TwoTensor bareLepTensor, bareHadTensor;
  for (int ka=0 ; ka < 4 ; ka++)
    for (int nu=0 ; nu < 4 ; nu++)
      {
    bareLepTensor.set(ka,nu,(p1AndMass * pGamma->get(ka) * p3AndMass * pGamma->get(nu)).tr());
    bareHadTensor.set(ka,nu,(p2AndMass * pc2m4.get(ka) * p4AndMass * pc4m2.get(nu)).tr());
      }

  std::complex<double> matElement = bareLepTensor.contractInOrder(bareHadTensor);
  double p1mp3 = (p1 - p3).Mag2();
  matElement *= (4*M_PI*M_PI*alpha*alpha/(p1mp3*p1mp3));

  double kinTerm = eMom*eMom/(64*M_PI*M_PI*beamEnergy*beamEnergy*mP*mP);

  return (cmSqMeVSq*std::real(matElement)*kinTerm);
}

void GeneratorRadiative::test4() // This function will be used to test the data files produced by novosibirsk
{
  // Parameter set
  int paramSet = 2;

  // Establish some kinematic constants
  double sixteenPiCubedAlphaCubed = 16. * pow(M_PI*alpha,3.);
  beamCharge = -1.;
  beamEnergy = 2010.;
  double mom1 = sqrt(beamEnergy*beamEnergy - me*me);
  double phiE = 0.;
  double thetaK=0, phiK=0;
  if (paramSet ==1)
    {
      thetaK = 10. * M_PI/ 180.;
      phiK = 120. * M_PI/180.;
    }
  else if (paramSet ==2)
    {
      thetaK = 90. * M_PI/ 180.;
      phiK = 20. * M_PI/180.;
    }
  double cosThetaK = cos(thetaK);

  // Prepare the initial four-vectors
  p1.SetXYZM(0.,0.,mom1,me);
  p2.SetXYZM(0.,0.,0.,mP);
  kMod.SetXYZT(sin(thetaK)*cos(phiK),sin(thetaK)*sin(phiK),cosThetaK,1.);

  // Prepare the output file
  std::ofstream out("mitData.txt");

  for (int j = 1 ; j<=1996 ; j+= 5)
    {
      std::cout << "Working on k=" << j <<".\n";

      double momK = ((double)j);
      k = kMod * momK;

      for (int i = 1 ; i<= 179 ; i++)
	{
	  double thetaE = ((double)i) * M_PI/180.;
	  double cosThetaE = cos(thetaE);
	  double cosThetaEK = cosThetaEKfromGlob(thetaE,phiE,thetaK,phiK);
	  //double thetaEK = acos(cosThetaEK); // Not needed quite yet
	  //double phiEK = phiEKfromGlob(thetaE,phiE,thetaK,phiK); // Not needed quite yet
	  
	  // Let's figure out the electron energy.
	  double tempA = beamEnergy + mP - momK;
	  double tempB = mom1*cosThetaE - momK*cosThetaEK;
	  double tempCSq = mP*(beamEnergy - momK) + me*me - p1.Dot(k);
	  bool valid = true;
	  double tempDisc = (tempCSq * tempCSq - me*me*(tempA*tempA - tempB*tempB));
	  if (tempDisc < 0) valid = false;
	  double mom3 = (tempB*tempCSq + tempA*sqrt(tempDisc))/(tempA*tempA - tempB*tempB);
	  //double E3 = sqrt(mom3*mom3 + me*me); // Not needed quite yet
	  //double otherK = photonMom(cosThetaE, cosThetaK, cosThetaEK, mom3, E3); // Not needed quite yet

	  // Set the remaining four vectors
	  p3.SetXYZM(mom3*sin(thetaE)*cos(phiE),mom3*sin(thetaE)*sin(phiE),mom3*cosThetaE,me);
	  p4 = p1+p2-p3-k;
	  if (p4.T() < mP) valid = false;
	  if (!(p4.P() > 0.)) valid = false;
	  if (fabs(p4.Mag() - mP) > 0.001) valid = false;

	  // Get the matrix element.
	  double lep = 0.;
	  double mix = 0.;
	  double had = 0.;
	  double p1mp3mk = (p1 - p3 - k).Mag2();
	  double p1mp3 = (p1 - p3).Mag2();
	  if (valid) bremMatrixElement(lep, mix, had, 0, 0);
	  double factor = sixteenPiCubedAlphaCubed / (momK*momK);
	  out << i << " " << momK << " " << lep << " " << mix << " " << had
	      << " " << factor << " " << p1mp3mk << " " << p1mp3 << " " << "\n";
	}
    }
  out.close();
}

void GeneratorRadiative::test5()
{
  // Create kinematics
  beamEnergy = 2010.;
  beamCharge = -1;
  double mom1 = sqrt(beamEnergy*beamEnergy - me*me);
  double thetaE = 30.*M_PI/180.;
  double cosThetaE = cos(thetaE);
  double phiE = 0.;
  double thetaK = 20.*M_PI/180.;
  double cosThetaK = cos(thetaK);
  double phiK = 120.*M_PI/180.;
  double cosThetaEK = cosThetaEKfromGlob(thetaE,phiE,thetaK,phiK);
  // double thetaEK = acos(cosThetaEK);
  // double phiEK = phiEKfromGlob(thetaE,phiE,thetaK,phiK);
  
  for (int i=0 ; i<100 ; i++)
    {
      double mom3 = 15.*((double) i);
      double E3 = sqrt(mom3*mom3 + me*me);
      double momK = photonMom(cosThetaE, cosThetaK, cosThetaEK, mom3, E3);
      
      p1.SetXYZM(0.,0.,mom1,me);
      p2.SetXYZM(0.,0.,0.,mP);
      p3.SetXYZM(mom3*sin(thetaE)*cos(phiE),mom3*sin(thetaE)*sin(phiE),mom3*cos(thetaE),me);
      kMod.SetXYZM(sin(thetaK)*cos(phiK),sin(thetaK)*sin(phiK),cos(thetaK),0.);
      k = kMod * momK;
      p4 = p1 + p2 - p3 - k;
      
      // Calculate the Matrix Element
      double lep, mix, had;
      bremMatrixElement(lep,mix,had,0,0);
      
      // Boost the kinematics
      TVector3 boostVec(0.7,0.0,-0.5);
      p1.Boost(boostVec);
      p2.Boost(boostVec);
      p3.Boost(boostVec);
      p4.Boost(boostVec);
      k.Boost(boostVec);
      double boostMomK = k.T();
      kMod = k * (1./boostMomK);
      
      // Recalculate the Matrix Element
      double lepBoost, mixBoost, hadBoost;
      bremMatrixElement(lepBoost,mixBoost,hadBoost,0,0);
      
      std::cout << mom3 << " " 
		<< lep/(momK*momK) << " " << lepBoost/(boostMomK*boostMomK) << " " 
		<< mix/(momK*momK) << " " << mixBoost/(boostMomK*boostMomK) << " " 
		<< had/(momK*momK) << " " << hadBoost/(boostMomK*boostMomK) << std::endl;
    }
}
