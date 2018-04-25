#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <math.h>
#include <string>
#include "Cola/Material.h"
#include "Cola/Masses.h" 
#include <fstream>
#include <iomanip> 

using namespace std;

int main(int argc, char *argv[])
{
  
  cout.setf(ios::fixed, ios::floatfield) ;
  cout.precision(5) ;     

  string par = argv[2] ;
  double momentum  =  atof(argv[1]) ;

  Particle pp = P_electron;
  pp.setMomentum(0,0,momentum) ;
 
  double path = 0.16 ;  // in cm !
  double beta, gamma, stepsize=0.01; 
  
  //Define element here!
  Element alum(13, 27, 2.7, -4.2395, 0.080, 3.63, 0.1708, 3.0127) ;
  Element he3(2, 3, 0.06, -11.1393, 0.13443, 5.8347, 2.2017, 3.6122) ; 
  Element si28(14, 28 , 2.33, -4.4351, 0.14921, 3.2546, 0.2014, 2.8715) ; 
  

  std::string proton ("proton") ;
  std::string electron ("electron") ;
  std::string positron ("positron") ;
  std::string piplus ("piplus") ;
  std::string piminus ("piminus") ;
  std::string kplus ("kplus") ;
  std::string kminus ("kminus") ;
  std::string muplus ("muplus") ;
  std::string muminus ("muminus") ;

  cout << "Particle: " << par << "\n" <<  endl;
  
  if(!par.compare("proton")){   
    pp=P_proton ;
    pp.setMomentum(0,0,momentum) ; 
    stepsize = 0.05 ;
  }else if(par == positron){
    pp=P_positron ;
    pp.setMomentum(0,0,momentum) ;
  }else if(par == piplus){
    pp=P_piplus ;
    pp.setMomentum(0,0,momentum) ;
    stepsize = 0.05 ;
  }else if(par == piminus){
    pp=P_piminus ;
    pp.setMomentum(0,0,momentum) ;
    stepsize = 0.05 ;
  }else if(par == kplus){
    pp=P_kplus ;
    pp.setMomentum(0,0,momentum) ;
  }else if(par == kminus){
    pp=P_kminus ;
    pp.setMomentum(0,0,momentum) ;
    stepsize = 0.05 ;
  }else if(par == muplus || par == muminus){
    pp = P_muplus ;
    pp.setMomentum(0,0,momentum) ;
    stepsize = 0.05 ;
  };

  beta = pp.momentum() / pp.energy() ;
  gamma= 1/sqrt(1-beta*beta) ;
  
  // Energy in GeV

  cout << "#########################" << endl ;
  cout << "Paramter for Ionisation loss and dens. Correction" << endl ;
  cout << "\nDensity [g/cm^3] " << si28.getDensity() << endl ;
  cout << "I:\t" << si28.IonisationPotential << endl ;
  cout << "C:\t" << si28.DC_C << endl ;
  cout << "a:\t" << si28.DC_a << endl ;
  cout << "m:\t" << si28.DC_m << endl ;
  cout << "X0:\t" << si28.DC_x0 << endl ;
  cout << "X1:\t" << si28.DC_x1 << endl ;
  cout << "##########################" << endl ;
  cout << "Kinematik\n" << endl ; 
  cout << "Momentum (3-vector): \t " << momentum << endl ;
  cout << "Beta:                \t " << beta << endl ;
  cout << "Gamma:               \t " << gamma << endl ; 
  cout << "T_kin [GeV]:               \t"  << pp.energy() - pp.getMass() << endl ;
  cout << "##########################" << endl ; 
  cout << "\nRadiation Length [g/cm^2]: \t"  << si28.RadiationLength << endl ;
  cout << "Thicknessbremstrahlung [cm]: \t " << si28.ThicknessBremsstrahlung << endl ;
  cout << "Landauparamter:              \t"  << si28.LandauPar << endl;
  cout << "BetheBloch dE/dx   [MeVcm^2/g]: \t " << si28.BetheBloch(pp)*1000 << endl ;
  cout << "Most propable Loss [MeVcm^2/g]: \t"  << si28.MostProbEloss(pp, path)*1000/(si28.getDensity()*path) << endl ;
  

  //ofstream myfile;
  FILE *file;
  file = fopen("Eloss.dat","w") ;
  fprintf(file, "  T[GeV]   -<dE/dx>_m[MeVcm^2/g] -(dE/dx)_mpl[MeVcm^2/g]  \n") ;
  for(int j=0; j<4; j++){
    for(int i=1; i<200; i++){
      pp.setMomentum(0,0,pow(10,-1+j)*stepsize*i*0.01) ; 
      double T = (pp.energy() - pp.getMass())*1000 ;
      fprintf(file,"%.5e\t%.5e\t%.5e\n",T, abs(si28.BetheBloch(pp))*1000, abs(si28.MostProbEloss(pp, path)*1000/(si28.getDensity()*path)));
    };
  };
  fclose(file);
  
  return 0;
}

  
