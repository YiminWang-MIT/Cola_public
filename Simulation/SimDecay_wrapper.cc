//
// $Id: SimDecay_wrapper.cc 2640 2015-04-01 14:03:00Z aqua $
//
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cstring>

#include "../Cola/Masses.h"
#include "../FourVector/FourVector.h"
#include "Matrix3D.h"
#include "CK_RungeKutta.h"
#include "Magnets.h"
#include "ran1.h"
#include "Colli.h"
#include "SimParticle.h"
#include "DSpectrometer.h"

#define QSPIN_DAT	"./meritve/trajektorija.dat"
#define QSPIN_B_DAT	"qspin_B.dat"
#define QSPIN_DPS	"qspin.dps"

using namespace std;


long int seed = -1; // seed for random generator

int roundit(double a){
  return (int)(a+0.5);
}

int save_hits(int M, int N, double a, double b, double xmin, double ymin, double x, double y, int **mat){	
  double xc = x - xmin;	
  double yc = y - ymin;	
  
  if (xc<0.0 || xc> a || yc<0.0 || yc>b){	
    return 0;
  }	
  else{	
    int i = roundit(1.0*M/a*xc);	
    int j = roundit(1.0*N/b*yc);	
    mat[i][j] = mat[i][j]+1; 
    #ifdef DEBUG
      cout<<"Matrix element is: "<<mat[i][j]<<endl;	
    #endif
  }	
  return 1;
}

int save_p(int N, double a, double xmin, double x, int *pp){       
  double xc = x - xmin;  
  
  if (xc<0.0 || xc> a ){       
    return 0;
  }     
  else{ 
    int i = roundit(1.0*N/a*xc);               
    pp[i] = pp[i]+1; 
    #ifdef DEBUG
      cout<<"PP is: "<<pp[i]<<endl;  
    #endif
  }     
  return 1;
}

int save_matrix(int M, int N, int **mat, char *name){	
  FILE   *store = NULL;	
  
  if (!(store= fopen(name, "w"))){
    fprintf(stderr, " Cannot open file %s for output!\n",name);
    return 0;
  }	
  
  for(int i = 0; i<=M; i++){	
    for(int j = 0; j<=N; j++){		
      fprintf(store,"%d ",mat[i][j]);	
    }	
    fprintf(store,"\n");	
  }	
  fclose(store);	
  return 1;
}

int save_vectorx(int M, int N, int **mat, char *name){	
  FILE   *store = NULL;	
  
  if (!(store= fopen(name, "w"))){
    fprintf(stderr, " Cannot open file %s for output!\n",name);
    return 0;
  }	
  
  for(int i = 0; i<=M; i++){	
    int sum = 0;
    for(int j = 0; j<=N; j++){		
      	sum += mat[i][j];
    }	
    fprintf(store,"%d\n",sum);
  }	
  fclose(store);	
  return 1;
}


int save_vectory(int M, int N, int **mat, char *name){	
  FILE   *store = NULL;	
  
  if (!(store= fopen(name, "w"))){
    fprintf(stderr, " Cannot open file %s for output!\n",name);
    return 0;
  }	
  
  for(int i = 0; i<=M; i++){	
    int sum = 0;
    for(int j = 0; j<=N; j++){		
      	sum += mat[j][i];
    }	
    fprintf(store,"%d\n",sum);
  }	
  fclose(store);	
  return 1;
}

int save_vec(int N, int *pp, char *name){  
  FILE   *store = NULL; 
  
  if (!(store= fopen(name, "w"))){
    fprintf(stderr, " Cannot open file %s for output!\n",name);
    return 0;
  }     
  
  for(int i = 0; i<=N; i++){    
    fprintf(store,"%d\n",pp[i]);
  }     
  fclose(store);        
  return 1;
}

int main(int argc, char* argv[])
{
   clock_t start, stop;
   cout<<"--------------------------------------------------------------------"<<endl;
   cout<<"                                simDecay                            "<<endl;
   cout<<"--------------------------------------------------------------------"<<endl;
    
#ifdef TRAJECTORY
   cout<<"TRAJECTORY"<<endl;
#else
   cout<<"NO TRAJECTORY"<<endl;
#endif	
   
   A28 kolliA28 ;  //We create new  28msr collimator for specA	
   A21 kolliA21 ;  //We create new  21msr collimator for specA
   
   double lkA = 565.5+80.0; // Distance from target to collimator + thickness of collimator
	
   Bcolli kolliB(0.02,0.07);  // We create collimator for SpecB. The acceptance of colli is 5.6msr
   double lkB = 3400.0; // Distance from target to collimator + thickness of collimator !!!! Nicht richtig
   
   allColli kolliKAOS; // We create collimator for Spec KAOS. The acceptance of collimator is 4PI
   double lkKAOS = 800.0; // Distance from target to collimator + thickness of collimator !!!! Nicht richtig

	
   //DSpectrometerA spectroA(&kolliA28,lkA,1); // Now we create new spectrometer A, with 28msr colli.
   DSpectrometerA spectroA(&kolliA21,lkA,1); // Now we create new spectrometer A, with 21msr colli.

   DSpectrometerB spectroB(&kolliB,lkB,-1); // Now we create new spectrometer B, with 5.6msr colli.
   
   DSpectrometerKAOS spectroKAOS(&kolliKAOS,lkKAOS,1); // Now we create new spectrometer KAOS
   
   
   DSpectrometer *spectro;

   //spectro = &spectroA; // We choose a spectrometer we want to use in our simulation
   spectro = &spectroB; // We choose a spectrometer we want to use in our simulation
   //spectro = &spectroKAOS; // We choose a spectrometer we want to use in our simulation

   if ( (argc==2) && !(strcmp(argv[1],"-showbound")) ){	
     cout<<"Show boundaries of magnetic fields!"<<endl;
     spectro->showbound();
     return 0;	
   }
	
   if ( (argc==2) && !(strcmp(argv[1],"-checkbound")) ){
     cout<<"Check magnetic fields!"<<endl;
     spectro->checkbound();
     return 0;
   }
		
   if ( (argc==2) && !(strcmp(argv[1],"-checkinside")) ){	
     cout<<"Checks what is inside spectrometer"<<endl;
     spectro->checkInsideSpectrometer();	
     return 0;	
   }
		
   if ((argc==2) && !(strcmp(argv[1],"-fieldmag"))){
     cout<<"Checks what is inside spectrometer"<<endl;	
     spectroKAOS.fieldMagnitude();	
     return 0;	
   }

	
   FILE   *storepions=NULL;	
   FILE   *storemuons=NULL;
   FILE   *storekaons=NULL;
   FILE   *storepositrons=NULL;	
   FILE   *timedep=NULL; 
   FILE   *tocke=NULL; 
   
  // Open file to store pions
  if (!(storepions= fopen("./meritve/datapions.dat", "w"))){
    fprintf(stderr, " Cannot open file %s for output!\n", "datapions.dat");
    return 2;	
  }
  	
  // Open file to store muons	
  if (!(storemuons= fopen("./meritve/datamuons.dat", "w"))){ 
    fprintf(stderr, " Cannot open file %s for output!\n", "datamuons.dat");
    return 2;	
  }
  
  // Open file to store kaons 
  if (!(storekaons= fopen("./meritve/datakaons.dat", "w"))){ 
    fprintf(stderr, " Cannot open file %s for output!\n", "datakaons.dat");
    return 2;   
  }
  
  // Open file to store positrons 
  if (!(storepositrons= fopen("./meritve/datapositrons.dat", "w"))){ 
    fprintf(stderr, " Cannot open file %s for output!\n", "datapositrons.dat");
    return 2;   
  }
  
  // V ta fajl bomo zapisovali podatke o casovni zahtevnosti 
  if (!(timedep= fopen("./meritve/timedependence.dat", "w"))){ 
    fprintf(stderr, " Cannot open file %s for output!\n", "timedependence.dat");
    return 2;   
  }
  
    // V ta fajl bomo zapisovali podatke o casovni zahtevnosti 
  if (!(tocke= fopen("./meritve/tocke.dat", "w"))){ 
    fprintf(stderr, " Cannot open file %s for output!\n", "tocke.dat");
    return 2;   
  }

	
  int M_VDC = 200; 	
  int N_VDC = 200;	
  double a_VDC = 7000.0;// Length of VDC chamber. - Approx!!!	
  double b_VDC = 800.0; // Width of VDC chamber - Approx!!!		
  double xmin_VDC = -3000.0; // Minumum value in x-direction on VDC. - Approx	
  double ymin_VDC = -400.0; // Minumum value in y-direction on VDC. - Approx
  
  double lth_VDC = 2000.0;
  double lph_VDC = 500.0;	
  double th_min =  0.0;	
  double ph_min = -250.0;
  
  
  double a_TG  = 80.0;
  double dpmin_TG = -40.0;
  double y0min_TG = -50.0;
  double b_TG = 100.0;
  
  double lth_TG = 240.0;    
  double lph_TG = 240.0;      
  double thmin_TG = -120.0;      
  double phmin_TG = -120.0;
  

  // Now we create matrices for saving data ( particle position, angles, etc.) at VDC-chamber 
  int **VDC_count_pion = new int* [M_VDC+1];	
  for(int i=0; i<M_VDC+1; i++) VDC_count_pion[i] = new int[N_VDC+1];
  
  int **VDC_count_muon = new int* [M_VDC+1];
  for(int i=0; i<M_VDC+1; i++) VDC_count_muon[i] = new int[N_VDC+1];
  
  int **VDC_count_kaon = new int* [M_VDC+1];  
  for(int i=0; i<M_VDC+1; i++) VDC_count_kaon[i] = new int[N_VDC+1];

  int **VDC_count_positron = new int* [M_VDC+1];  
  for(int i=0; i<M_VDC+1; i++) VDC_count_positron[i] = new int[N_VDC+1];
  
  int **VDC_count_pion_koti = new int* [M_VDC+1];
  for(int i=0; i<M_VDC+1; i++) VDC_count_pion_koti[i] = new int[N_VDC+1];
	
  int **VDC_count_muon_koti = new int* [M_VDC+1];	
  for(int i=0; i<M_VDC+1; i++) VDC_count_muon_koti[i] = new int[N_VDC+1];
	
  int **VDC_count_kaon_koti = new int* [M_VDC+1];       
  for(int i=0; i<M_VDC+1; i++) VDC_count_kaon_koti[i] = new int[N_VDC+1];
  
  int **VDC_count_positron_koti = new int* [M_VDC+1];       
  for(int i=0; i<M_VDC+1; i++) VDC_count_positron_koti[i] = new int[N_VDC+1];
  
  int **TG_count_koti = new int* [M_VDC+1];       
  for(int i=0; i<M_VDC+1; i++) TG_count_koti[i] = new int[N_VDC+1];

  int **TG_count = new int* [M_VDC+1];       
  for(int i=0; i<M_VDC+1; i++) TG_count[i] = new int[N_VDC+1];
  // sedaj matriko napolnimo z niclami
	
  for(int i = 0; i< M_VDC+1 ; i++){
    for(int j = 0; j<N_VDC+1; j++){	
      VDC_count_pion[i][j] = 0;		
      VDC_count_pion_koti[i][j] = 0;		
      VDC_count_muon[i][j] = 0;	
      VDC_count_muon_koti[i][j] = 0;	
      VDC_count_kaon[i][j] = 0; 
      VDC_count_kaon_koti[i][j] = 0;    
      VDC_count_positron[i][j] = 0; 
      VDC_count_positron_koti[i][j] = 0; 
      TG_count[i][j] = 0; 
      TG_count_koti[i][j] = 0; 
    }	
  }
  
  double pMax = 600;
  double EMax = 600;
  int pN = 800;
  int EN = pN;
  int pPion[pN+1];
  int pMuon[pN+1];
  int pKaon[pN+1];
  int EPion[pN+1];
  int EMuon[pN+1];
  int EKaon[pN+1];  
  
  for (int i = 0; i<=pN; i++){
        pPion[i] = 0;
        pMuon[i] = 0;
        pKaon[i] = 0;
        EPion[i] = 0;
        EMuon[i] = 0;
        EKaon[i] = 0;
  }
  
  

  // We define some variables
  double xd,yd,zd,pd,dpd; //, sxd, syd,szd;

  int BAD = 0;	
  int GOOD = 0;	
  int UNKNOWN = 0;
  int KOLI = 0;	
  int PIONI = 0;	
  int MUONI = 0;
  int KAONI = 0;
  int POSITRONI = 0;
  
  //  int issix = 0;
	
  start = clock();
  for(int count = 0; count <10000; count++){	
    
      
    cerr<<"Iteration No.: "<<count<<endl; 

    if (count %100 == 0){
      stop= clock();
      fprintf(timedep,"%d %f \n", count, (float)((stop - start)/CLOCKS_PER_SEC));
      
    }
    
    xd = 0.0; // Coordinate x0 in [mm]	
    yd = 0.0; // Coordinate y0 in [mm]	
    zd = 0.0; // Coordinate z0 in [mm]	
    pd = 227.0; // Reference momentum of spectrometer in MeV/c	
	
    //    sxd = 0.0; // x-coordinate  of spin in ACS cs.	
    //    syd = 0.0; // y-coordinate  of spin in ACS cs.
    //    szd = 0.0; // z-coordinate  of spin in ACS cs.


    // Now we generate initial spherical angles th and ph for the initial particle
    double phiSP = 2.0*M_PI*ran1(&seed);  // We generate azimuthal angle
    double thetaMAX = 10.2*deg; // We define max polar angle
    double thetaSP = acos((1.0 - cos(thetaMAX))*ran1(&seed) + cos(thetaMAX)); // We calculate polar angle

    #ifdef DEBUG
      cout<<"Azimuthal angle: "<<phiSP<<", Polar angle: "<<thetaSP<<endl;
    #endif

    dpd = 30.0*(2.0*ran1(&seed)-1.0); // Calculate the momentum deviation from ref. momentum
    
    #ifdef DEBUG
      cout<<"dp = "<<dpd<<endl;
    #endif
    
    // Initial position vector.
    Vector3D vecx(xd,yd,zd);

    // We create the momentum vector which must point in proper dirrection.
    Vector3D vecp(cos(phiSP)*sin(thetaSP), sin(phiSP)*sin(thetaSP), cos(thetaSP)); 
    vecp = vecp/vecp.abs()*pd*(1.0+dpd/100.0); 
 
    
    // ------------------ NOW WE CHOOSE INITIAL PARCILE in the target --------------

    /*
    // ***************** PROTON ******************************
    double masa = P_proton.getMass()*1000.0;
    double Energy = sqrt(vecp.abs()*vecp.abs() + masa*masa);
    Vector3D spin(0,0,70.0);
    
    SimParticle simParticle1(P_proton.getID(), vecx, FourVector(Energy, vecp[1],vecp[2],vecp[3]),spin);
    */
    
    
    // **************** PION *********************************
    double masa = P_piplus.getMass()*1000.0;
    double Energy = sqrt(vecp.abs()*vecp.abs() + masa*masa);
    Vector3D spin(0,0,0.0);
    
    SimParticle simParticle1(P_piplus.getID(), vecx, FourVector(Energy, vecp[1],vecp[2],vecp[3]),spin);
    
    
    /*
    // **************** KAON *********************************
    double masa = P_kplus.getMass()*1000.0;
    double Energy = sqrt(vecp.abs()*vecp.abs() + masa*masa);
    Vector3D spin(0.0,0.0,0.0);
    
    SimParticle simParticle1(P_kplus.getID(), vecx, FourVector(Energy, vecp[1],vecp[2],vecp[3]),spin);
    */
    /*
    // **************** MUON *********************************
    double masa = P_muplus.getMass()*1000.0;
    double Energy = sqrt(vecp.abs()*vecp.abs() + masa*masa);
    Vector3D spin(0,0,0.0);
    
    SimParticle simParticle1(P_muplus.getID(), vecx, FourVector(Energy, vecp[1],vecp[2],vecp[3]),spin);
    */
    
    /*
    // **************** POSITRON *********************************
    double masa = P_positron.getMass()*1000.0;
    double Energy = sqrt(vecp.abs()*vecp.abs() + masa*masa);
    Vector3D spin(0.,0.,70.0);
    
    SimParticle simParticle1(P_positron.getID(), vecx, FourVector(Energy, vecp[1],vecp[2],vecp[3]),spin);
    */


    #ifdef DEBUG    
      cout<<"--------------------------- START PARTICE ---------------------"<<endl;
      cout<<"ID of the particle: "<< simParticle1.ID<<endl;
      cout<<"Status: "<<simParticle1.status<<endl;
      cout<<"Mass of the particle: "<< simParticle1.mass<<endl;
      cout<<"Charge of the particle: "<<simParticle1.charge<<endl;
      cout<<"Decay mode: "<< simParticle1.decaymode<<endl;
      cout<<"Livetime of the SimParticle: "<< simParticle1.livetime<<" ns"<<endl;
      cout<<"Position vector: "<< simParticle1.position <<" mm"<<endl;
      cout<<"Magnitude of the momentum vector: "<<simParticle1.fVmomentum.momentum()<<" MeV"<<endl;
      cout<<"Energy: "<<simParticle1.fVmomentum.energy()<<" MeV"<<endl;
      cout<<"Spin: "<<simParticle1.spin<<endl;
      cout<<"--------------------------------------------------------------"<<endl;
    #endif 
    // Now we create a group of particles
    SimParticleGroup simParticleGroup1;
    
    // Now we add our particle into the group
    simParticleGroup1.add(simParticle1);
    //simParticleGroup1.add(simParticle2);
    
     // We set a flag.
    //    if (simParticle1.decaymode == 6) issix = 1;
    //    else issix=0;
    
    if(!spectro->simulate(&simParticleGroup1,pd)){
      UNKNOWN++; // if simulate return 0, that means, that there was unknow decay mode.
    }
    else{
      // Now we examine (check) each final particle in VCD-chamber. 
      for(int i=0; i<simParticleGroup1.getNumber(); i++){

        if (simParticleGroup1.simParticle[i].status==1){
          GOOD++;	
          Vector3D VDCPosition;
          double theta;	
          double phi;
          Vector3D Velocity(simParticleGroup1.simParticle[i].fVmomentum.beta()[0],
                            simParticleGroup1.simParticle[i].fVmomentum.beta()[1],
                            simParticleGroup1.simParticle[i].fVmomentum.beta()[2]);
      	
          Velocity = Velocity*SpeedOfLight;
      
          if (simParticleGroup1.simParticle[i].ID==P_piplus.getID()) PIONI++;	
          else if(simParticleGroup1.simParticle[i].ID==P_muplus.getID()) MUONI++;
          else if(simParticleGroup1.simParticle[i].ID==P_kplus.getID()) KAONI++;
          else if(simParticleGroup1.simParticle[i].ID==P_positron.getID()) POSITRONI++;
      
          int aJeSlo = spectro->position_at_VDC(simParticleGroup1.simParticle[i].position, Velocity, &VDCPosition, &theta, &phi);
          if (aJeSlo){
            if (simParticleGroup1.simParticle[i].ID == P_muplus.getID()){
	      // In this case the final particle is Muon
	      #ifdef DEBUG	
                cout<<"Position of the MUON on VDC: "<<VDCPosition<<" mm"<<endl;
                cout<<"Angle theta on VDC: "<<theta<<",  Phi: "<<phi<<endl;
              #endif

	      fprintf(storemuons,"%f %f %f %f %f\n",VDCPosition[1], VDCPosition[2], VDCPosition[3], theta, phi);
              save_hits(M_VDC, N_VDC, a_VDC, b_VDC, xmin_VDC, ymin_VDC,VDCPosition[1],VDCPosition[2],VDC_count_muon );	
              save_hits(M_VDC, N_VDC, lth_VDC, lph_VDC, th_min, ph_min, theta, phi, VDC_count_muon_koti );
              save_p(pN, pMax, 0.0, simParticleGroup1.simParticle[i].fVmomentum.momentum(), pMuon );
              save_p(EN, EMax, 0.0, simParticleGroup1.simParticle[i].fVmomentum.energy(), EMuon );
            }
            else if (simParticleGroup1.simParticle[i].ID == P_piplus.getID()) {
              // In this case the final particle is Pion
              #ifdef DEBUG
                cout<<"Position of the PION on VDC: "<<VDCPosition<<" mm"<<endl;
                cout<<"Angle theta on VDC: "<<theta<<",  Phi: "<<phi<<endl;
              #endif

              fprintf(storepions,"%f %f %f %f %f\n",VDCPosition[1], VDCPosition[2], VDCPosition[3], theta, phi);
              save_hits(M_VDC, N_VDC, a_VDC, b_VDC, xmin_VDC, ymin_VDC, VDCPosition[1], VDCPosition[2], VDC_count_pion );
              save_hits(M_VDC, N_VDC, lth_VDC, lph_VDC, th_min, ph_min, theta, phi, VDC_count_pion_koti );
              save_p(pN, pMax, 0.0, simParticleGroup1.simParticle[i].fVmomentum.momentum(), pPion);
              save_p(EN, EMax, 0.0, simParticleGroup1.simParticle[i].fVmomentum.energy(), EPion);
            }
            else if (simParticleGroup1.simParticle[i].ID == P_kplus.getID()){
	      // In this case the final particle is Kaon.
	      #ifdef DEBUG	
                cout<<"Position of the KAON on VDC: "<<VDCPosition<<" mm"<<endl;
                cout<<"Angle theta on VDC: "<<theta<<",  Phi: "<<phi<<endl;
              #endif

              fprintf(storekaons,"%f %f %f %f %f\n",VDCPosition[1], VDCPosition[2], VDCPosition[3], theta, phi);
              save_hits(M_VDC, N_VDC, a_VDC, b_VDC, xmin_VDC, ymin_VDC, VDCPosition[1], VDCPosition[2], VDC_count_kaon );
              save_hits(M_VDC, N_VDC, lth_VDC, lph_VDC, th_min, ph_min, theta, phi, VDC_count_kaon_koti );
              save_p(pN, pMax, 0.0, simParticleGroup1.simParticle[i].fVmomentum.momentum(), pKaon );
              save_p(EN, EMax, 0.0, simParticleGroup1.simParticle[i].fVmomentum.energy(), EKaon );
            }
            else if (simParticleGroup1.simParticle[i].ID == P_positron.getID()){
              // In this case the final particle is positron
	      #ifdef DEBUG
                cout<<"Position of the  POSITRON on VDC: "<<VDCPosition<<" mm"<<endl;
                cout<<"Angle theta on VDC: "<<theta<<",  Phi: "<<phi<<endl;
              #endif
 
              fprintf(storepositrons,"%f %f %f %f %f\n",VDCPosition[1], VDCPosition[2], VDCPosition[3], theta, phi);
              save_hits(M_VDC, N_VDC, a_VDC, b_VDC, xmin_VDC, ymin_VDC, VDCPosition[1], VDCPosition[2], VDC_count_positron );
              save_hits(M_VDC, N_VDC, lth_VDC, lph_VDC, th_min, ph_min, theta, phi, VDC_count_positron_koti );
            }
	    // We save initial coortinates and momenta of all initial particles that came 
            // successfuly (or. their products) to the focal plane..    	
            fprintf(tocke,"%f %f %f %f %f\n",dpd, yd, zd, 1000.0*atan(vecp[1]/vecp[3]), 1000.0*atan(vecp[2]/vecp[3]));
            save_hits(M_VDC, N_VDC, a_TG, b_TG, dpmin_TG, y0min_TG, dpd, yd, TG_count );
            save_hits(M_VDC, N_VDC, lth_TG, lph_TG, thmin_TG, phmin_TG, 1000.0*atan(vecp[1]/vecp[3]), 1000.0*atan(vecp[2]/vecp[3]), TG_count_koti );
          }
          else{
            cout<<"No go!\a"<<endl;
          }

        }
        else if (simParticleGroup1.simParticle[i].status==-1){
          KOLI++;
        }
        else{	
          BAD++; 
        }
      }
    }
    
    
  }
  
  cout<<"Number of GOOD solutions: "<<GOOD<<endl;	
  cout<<"Number of BAD solutions: "<<BAD<<endl;
  cout<<"Number of KOLI solutions: "<<KOLI<<endl;	
  cout<<"Number of KAONS in the end: "<<KAONI<<endl;    
  cout<<"Number of PIONS in the end: "<<PIONI<<endl;	
  cout<<"Number of MUONS in the end: "<<MUONI<<endl;
  cout<<"Number of POSITRONS in the end: "<<POSITRONI<<endl;
	
  // Now we save all our data into files.
  save_matrix(M_VDC,N_VDC,VDC_count_pion, "./meritve/countpion.dat");	
  save_matrix(M_VDC,N_VDC,VDC_count_muon, "./meritve/countmuon.dat");	
  save_matrix(M_VDC,N_VDC,VDC_count_kaon, "./meritve/countkaon.dat");
  save_matrix(M_VDC,N_VDC,VDC_count_positron, "./meritve/countpositron.dat");
  save_matrix(M_VDC,N_VDC,VDC_count_pion_koti, "./meritve/countpionkoti.dat");	
  save_matrix(M_VDC,N_VDC,VDC_count_muon_koti, "./meritve/countmuonkoti.dat");
  save_matrix(M_VDC,N_VDC,VDC_count_kaon_koti, "./meritve/countkaonkoti.dat");
  save_matrix(M_VDC,N_VDC,VDC_count_positron_koti, "./meritve/countpositronkoti.dat");

  save_vectorx(M_VDC,N_VDC,VDC_count_pion, "./meritve/his1pionN.dat");	
  save_vectory(M_VDC,N_VDC,VDC_count_pion, "./meritve/his2pionN.dat");
  save_vectorx(M_VDC,N_VDC,VDC_count_pion_koti, "./meritve/his3pionN.dat");
  save_vectory(M_VDC,N_VDC,VDC_count_pion_koti, "./meritve/his4pionN.dat");

  save_vectorx(M_VDC,N_VDC,VDC_count_kaon, "./meritve/his1kaonN.dat");	
  save_vectory(M_VDC,N_VDC,VDC_count_kaon, "./meritve/his2kaonN.dat");
  save_vectorx(M_VDC,N_VDC,VDC_count_kaon_koti, "./meritve/his3kaonN.dat");
  save_vectory(M_VDC,N_VDC,VDC_count_kaon_koti, "./meritve/his4kaonN.dat");

  save_vectorx(M_VDC,N_VDC,VDC_count_muon, "./meritve/his1muonN.dat");	
  save_vectory(M_VDC,N_VDC,VDC_count_muon, "./meritve/his2muonN.dat");
  save_vectorx(M_VDC,N_VDC,VDC_count_muon_koti, "./meritve/his3muonN.dat");
  save_vectory(M_VDC,N_VDC,VDC_count_muon_koti, "./meritve/his4muonN.dat");
  
  save_vectorx(M_VDC,N_VDC,VDC_count_positron, "./meritve/his1positronN.dat");	
  save_vectory(M_VDC,N_VDC,VDC_count_positron, "./meritve/his2positronN.dat");
  save_vectorx(M_VDC,N_VDC,VDC_count_positron_koti, "./meritve/his3positronN.dat");
  save_vectory(M_VDC,N_VDC,VDC_count_positron_koti, "./meritve/his4positronN.dat");
  
  save_vec(pN,pMuon, "./meritve/his5pMuonN.dat");  
  save_vec(pN,pPion, "./meritve/his5pPionN.dat");
  save_vec(pN,pKaon, "./meritve/his5pKaonN.dat");
  save_vec(EN,EMuon, "./meritve/his6EMuonN.dat");  
  save_vec(EN,EPion, "./meritve/his6EPionN.dat");
  save_vec(EN,EKaon, "./meritve/his6EKaonN.dat");
  
  save_vectorx(M_VDC,N_VDC,TG_count, "./meritve/hisTGdp.dat");
  save_vectory(M_VDC,N_VDC,TG_count, "./meritve/hisTGy0.dat");
  save_vectorx(M_VDC,N_VDC,TG_count_koti, "./meritve/hisTGth.dat");
  save_vectory(M_VDC,N_VDC,TG_count_koti, "./meritve/hisTGph.dat");
  fclose(storepions);	
  fclose(storemuons);
  	
  cout<<"The End! \a"<<endl;	
}

