//
// $Id: DSpectrometerKAOS.cc 2348 2009-10-09 10:07:29Z merkel $
//

using namespace std;
#include <math.h>
#include "../Cola/Masses.h"
#include "DSpectrometer.h"
#include "SimDecay.h"
#include "ran1.h"
#include "KAOSfield.h"

#define QSPIN_DAT	"trajektorija.dat"
#define QSPIN_B_DAT	"qspin_B.dat"


// Global variables that we need. This is not the best way to do things. :(
double KAOSgammaParticle; // This variable holds the value of factor gamma = sqrt(p*p+M*M)/M; 
double KAOSK_p; // This variable stores the properties of the particle K_p = e/(M*gamma)
double KAOSg_p; // Gyromagnetic factor.


// Global variables for StoreStep
FILE   *KAOSstorefile=NULL;  // filehandle of open file for step storage
FILE   *KAOSstorefile2=NULL; // filehandle for storage of magnetic field strength 
static Vector3D KAOSr_old(0.0,0.0,0.0);  // Dani vektor pove lego delca, preden smo naredil naslednji korak
//static double KAOSact_length = 0.0;

const double KAOS_Ld = 1000.0; // length from target to detector. Unsicher!!!!!

KAOSField kaosField(1.95); // Max KAOS field is 1.9Tesla;



 DSpectrometerKAOS:: DSpectrometerKAOS(Collimator* colli, double lk, int fieldDirection){
   theta = 0.0;
   phi = 0.0;
   collimator = colli;
   Lk = lk;
   kaosField.B_Nom = fieldDirection*kaosField.B_Nom;
   cout<<"Kaos detector has been created!!"<<endl;
 }



// ------------------------------ getMagField ---------------------------------
// Calculates the magnetic field (vector3D B) in the absolute Coordinate system
// (ACS, x downwards) at a given position (vector3D r):

int KAOSgetMagField(Vector3D &B, Vector3D &r){
  //  int bval=0;
  
  // first we transform r from target-CS to own CS of KAOS.
  double koord[3]; 
  double Bfield[3] = {0.0, 0.0, 0.0};
   
  // Now we fix units and rotate the coordinate sys. around z-axis for 180 deg.
  // Magnetic field is given in this rotated coordinate system.
  koord[0] = -r[1]/10.0; // from mm to cm
  koord[1] = -r[2]/10.0; // from mm to cm
  koord[2] = (r[3]-KAOS_Ld)/10.0; // from mm to cm   
  
  if((koord[0] >= kaosField.xmin && koord[0] <= kaosField.xmax)&&
      (fabs(koord[1]) <= kaosField.ymax)&&
      (koord[2] >= kaosField.zmin && koord[2] <= kaosField.zmax))
  {
    kaosField.BerechneDipolFeldGlatt(koord, Bfield);        
  }
  
  // Once we have magnetic field in rotated CS, we rotate field back into our CS.
  B[1] = -Bfield[0]; 
  B[2] = -Bfield[1]; 
  B[3] = Bfield[2]; 
  
  return 0;
} 


// ------------------------------ magmove -------------------------------------
// Routine  calculates the derivatives dydx[] of y[] with the ODEs 
// for the numerical Cash-Karp Runge-Kutta solution:

void KAOSmagmove(double t,     // Time is independent variable
              double y[],      // INPUT:  variables
              double dydx[])   // OUTPUT: their derivatives
{
  static Vector3D r;          // Position vector
  static Vector3D v;          // Velocity vector
  static Vector3D s;          // Spin vector
  static Vector3D dr, dv, ds; // Derivetive vectors od position, velocity and spin
  static Vector3D B;          // Magnetic field vector
  static Vector3D E_v;        // Unit vector, that is parallel to velocity. 
  static Vector3D B_L, B_T;   // Vectors of transversal and longitudinal components of the magnetic field

  r[1] = y[1];  // x 
  r[2] = y[2];  // y 
  r[3] = y[3];  // z

  v[1] = y[4];  // vx 
  v[2] = y[5];  // vy
  v[3] = y[6];  // vz

  s[1] = y[7];  // sx
  s[2] = y[8];  // sy
  s[3] = y[9];  // sz

  // Calculate magnetic field at position r:
  KAOSgetMagField(B,r); 

  // Now we split calulated B into a longitudinal and a transversal part of B regarding to the velocity vector
  E_v = v/v.abs();
  B_L = (E_v * B) * E_v;
  B_T = B - B_L;


  // Now we calculate derivatives that we need for solving ODEti 
  dr = v; 
  dv = KAOSK_p * (v && B);  
  ds = KAOSK_p * ( s && (KAOSg_p/2 * B_L + (1+KAOSgammaParticle*(KAOSg_p-2)/2) * B_T) );

  dydx[1]=dr[1];  // dx 
  dydx[2]=dr[2];  // dy
  dydx[3]=dr[3];  // dz

  dydx[4]=dv[1];  // dvx
  dydx[5]=dv[2];  // dvy
  dydx[6]=dv[3];  // dvz

  dydx[7]=ds[1];  // dsx
  dydx[8]=ds[2];  // dsy
  dydx[9]=ds[3];  // dsz

  return;
}


// ---------------------------- StoreStep ------------------------------------
// Function stores the content of vector y[] into the file. y[] includes  current 
// value of position, velocity, spin and magnetic field 

void KAOSStoreStep(double t,   // independent variable
                double y[], // fields to store
                int ysize,  // size of y (y[1..ysize] must exist!)
                int stepN)  // number of the stored step
{

#ifdef TRAJECTORY
  int i; 

  fprintf(KAOSstorefile,"%6d %8.2f  ", stepN, t); 
  for (i=1; i<=ysize; i++) fprintf(KAOSstorefile," %8.2f", y[i]); 
  fprintf(KAOSstorefile,"\n"); 

  Vector3D r_act(y[1],y[2],y[3]); 
  Vector3D B_act; 
  KAOSgetMagField(B_act,r_act);  
  KAOSact_length += (r_act - KAOSr_old).abs(); KAOSr_old = r_act;  

  fprintf(KAOSstorefile2,"%10.4f %10.4f %10.4f %10.4f %10.4f \n",
          KAOSact_length, B_act[1], B_act[2], B_act[3], B_act.abs()); 
#endif 
}




//----------------------------------------------- Check position of particle ------------------------
// This method checks if the particle has crossed the VDC-chamber.
int KAOScheckPosition(double yvalues[])
{	  
  Vector3D Position(yvalues[1],yvalues[2],yvalues[3]);  
  Vector3D Velocity(yvalues[4],yvalues[5],yvalues[6]);

  Vector3D O_D_vdc_x1(-948.9, 0.0, 2553.0); // Unsicher !!!!!!
  
  double tma_x     =   0.13594537; // mm
  double tma_y     =   0.0; // mm     
  double tma_phi   =  0.0; // mrad 

  // Alpha is angle between z-axes of ACS and x-axes of VDC system and is approx. 
  double alpha = (0.0) * deg; 


   // Koordinates of VDC-cs given in ACS.
  Vector3D Ez_vdc( -cos(alpha), 0.0, -sin(alpha));
  Vector3D Ex_vdc( -sin(alpha), 0.0,  cos(alpha));
  Vector3D Ey_vdc = Ez_vdc && Ex_vdc; 
 
  // Rotation matrix, which rotates aroud x' for angle tma_phi 
  Matrix3D M_x(Ex_vdc, (tma_phi/1000.0) );  
  Ey_vdc = M_x * Ey_vdc;  
  Ez_vdc = M_x * Ez_vdc;

  
  // Now we calculate the position of center of VDC sisyem, given in ACS cs.
  Vector3D O_vdc = O_D_vdc_x1 -tma_x*Ex_vdc -tma_y*Ey_vdc;

  Vector3D DeltaX  = Position - O_vdc; // For given point of position we calculate the deviation from center of VDC (in ADC).
  Vector3D DeltaXc = Vector3D( Ex_vdc*DeltaX, Ey_vdc*DeltaX, Ez_vdc*DeltaX); // Now we transform this difference in to VDC cs. 
  
  if (DeltaXc[1]>-1000.0 && DeltaXc[3]>0.0) return 1;
     
  return 0;
}




int KAOSinsideSpectrometer(double x, double y, double z){

  double topz[6] ={435.897, 743.59, 799.0, 1504.0, 2303.0, 2590.0};
  double topx[6] ={-717.95,-717.95, -744.0,  -744.0, -1437.0, -1497.0 };
  
  double bottomz[2] = {743.6, 4077.0};
  double bottomx[2] = {1000.0, -179.5};
  
  for(int i = 0; i<6-1; i++){       
    if (z>=topz[i] && z<=topz[i+1]){                  
      if (x <= (topx[i+1] - topx[i])/(topz[i+1] - topz[i])*(z - topz[i]) + topx[i]) return 0;
    } 
  }
  
  for(int i = 0; i<2-1; i++){           
    if (z>=bottomz[i] && z<=bottomz[i+1]){                    
      if (x >= (bottomx[i+1] - bottomx[i])/(bottomz[i+1] - bottomz[i])*(z - bottomz[i]) + bottomx[i]) return 0;               
    }         
  }   
  
  if (z < 0.0 || z > 4077.0) return 0;
  if (x > 1000.0 || x < -1500.0) return 0; 
  //inside dipole magnet the gap is 20cm wide. 
  if (x>-722.0 && fabs(y)>100.0) return 0; 
  
  return 1; // if everything is OK!
}

int KAOScalculate_trajectory(SimParticle *simParticle1, double refP){ 
  // First we set gyromagnetic ratio and mass
  double M = simParticle1->mass;	
  double e = simParticle1->charge;
  KAOSg_p = simParticle1->gfactor;
  
  // Number of ODEs (dimension of the arrays)
  int odenum = 9;

  // Vector containing start values for y:
  double *ystart = new double[odenum+1];

  Vector3D Position = simParticle1->position; // Initial position vector in mm !
  Vector3D Spin_tg = simParticle1->spin; // Spin vector
  double p = simParticle1->fVmomentum.momentum(); // Particle momentum in MeV/c
	
  //Now we rescale the magnetic field to the appropriate value for given  ref. momentum. 
  double errect = refP/1200.0;  // Unsicher!!!!!!!!!!!!!!!! 
  kaosField.ScaleField(errect);
  
  //Initial velocity vector is parallel to the momentum vector.
  Vector3D Velocity(simParticle1->fVmomentum[1],
                    simParticle1->fVmomentum[2],
                    simParticle1->fVmomentum[3]);  
  
  Velocity = Velocity/Velocity.abs(); 
  KAOSgammaParticle = sqrt(p*p+M*M)/M; 
  double betaParticle = sqrt(1.0 - 1.0/(KAOSgammaParticle*KAOSgammaParticle)); 
  Velocity = Velocity * SpeedOfLight * betaParticle; 

  // Vector containing start values for y:
  ystart[1] = Position[1]; // x-Coordinate in mm
  ystart[2] = Position[2]; // y-Coordinate in mm
  ystart[3] = Position[3]; // z-Coordinate in mm
  ystart[4] = Velocity[1]; // x-Velocity in mm/nsec
  ystart[5] = Velocity[2]; // y-Velocity in mm/nsec
  ystart[6] = Velocity[3]; // z-Velocity in mm/nsec
  ystart[7] = Spin_tg[1];  // x component of the spin at the target (%)
  ystart[8] = Spin_tg[2];  // y component of the spin at the target (%)
  ystart[9] = Spin_tg[3];  // z component of the spin at the target (%)
  	

  KAOSK_p = e/M/KAOSgammaParticle* 89.87551787 ; // factor K_p = e/(M*gamma) 
  	
  double starttime = 0; 
  double endtime = simParticle1->livetime; // Integration time in nsec
  int nok, nbad, nstored=0; // Here will be stored numbers of good and bad integration steps   
  double acttime;
   
  #ifdef DEBUG	
    // User info before start:
    std::cout << "---------------------------------";
    std::cout << "---------------------------------" << std::endl;
    std::cout << " Momentum   : " << refP << " MeV/c" << std::endl;
    std::cout << " gamma of the particle: " << KAOSgammaParticle << std::endl;
    std::cout << " Start position   : " << Position << std::endl;
    std::cout << " Start velocity   : " << Velocity;
    std::cout << ", beta :"<< betaParticle <<std::endl;
    std::cout << " Start spin (ACS) : " << Spin_tg  << std::endl;
    std::cout << " -----------------------------";
    std::cout << " calculating, please wait .........." << std::endl;
  #endif

  int res= odeint3(ystart,    // Start value
                   odenum,    // number of ODEs (= dimension of the arrays)
                   starttime, // start value for the independent variable
                   endtime,   // stop at this value of the independent variable
                   -8800,     // stop if y[1] smaller than y1min (mm)
                   5e-6,      // required accuracy
                   1,         // guessed first stepsize
                   1e-20,     // min. allowed stepsize (can be zero)
                   &nok,      // number of good steps taken
                   &nbad,     // number of bad (retried and fixed) steps taken
                   0.2,       // interval size for step storing (nsec)
                   100000,    // max number of steps to store
                   nstored,   // number of stored steps
                   KAOSStoreStep, // function which writes the step to file
                   KAOSmagmove,   // derivatives function
                   &acttime,  // actual value for the independent variable
                   KAOScheckPosition,
                   KAOSinsideSpectrometer); 

  // out of ystart!!!, unlucky name, hihi, but thats NUM REC people,
  // just kept some of their variable names, so i'm innocent :-)	
  Position[1] = ystart[1];  // Final x	
  Position[2] = ystart[2];  // Final y	
  Position[3] = ystart[3];  // Final z	
  Velocity[1] = ystart[4];  // Final vx	
  Velocity[2] = ystart[5];  // Final vy	
  Velocity[3] = ystart[6];  // Final vz	
  betaParticle = Velocity.abs()/SpeedOfLight; 
  Vector3D Spin_end(ystart[7],ystart[8],ystart[9]); 

  #ifdef DEBUG
    // User info (results):
    std::cout << " Number of good steps   : " << nok  << std::endl;
    std::cout << " Number of bad steps    : " << nbad << std::endl;
    std::cout << " Number of stored steps : " << nstored << std::endl;
    std::cout << " End position   : " << Position << std::endl;
    std::cout << " End velocity   : " << Velocity;
    std::cout << ", beta = "<< betaParticle <<std::endl;
    std::cout << " End spin (ACS) : " << Spin_end << std::endl;	
  #endif

  // Now we save new particle's position and momentum into the simParticle
  simParticle1->position = Position;		
  Vector3D gibalnaK = Velocity/Velocity.abs()*simParticle1->fVmomentum.momentum();
  simParticle1->fVmomentum = FourVector(simParticle1->fVmomentum[0],
                                 gibalnaK[1],
                                 gibalnaK[2],
                                 gibalnaK[3]); 
  simParticle1->spin = Spin_end;
  
  #ifdef DEBUG  	
    std::cout << " End position 2  : " << simParticle1->position << std::endl;
    std::cout<<"Trajectory is calculated!"<<std::endl;    
  #endif

  return res;
}


int save_matrix_KAOS(int M, int N, double **mat, char *name)
{	
  FILE   *store = NULL;	
  if (!(store= fopen(name, "w"))){
    fprintf(stderr, " Cannot open file %s for output!\n",name);
    return 0;
  }	
  for(int i = 0; i<=M; i++){	
    for(int j = 0; j<=N; j++){	
      fprintf(store,"%f ",mat[i][j]);
    }	
    fprintf(store,"\n");	
  }	
  fclose(store);	
  return 1;
}

int DSpectrometerKAOS::fieldMagnitude(){

  int xN = 200;
  int zN = 200;

  double x0 = -1500.0;
  double z0 = 0000.0;
  double xL = 3500.0;
  double zL = 5000.0;
  Vector3D X;
  Vector3D B;

  double **field = new double* [zN+1];	
  for(int i=0; i<zN+1; i++) field[i] = new double[xN+1];
	
  double **XX = new double* [zN+1];        
  for(int i=0; i<zN+1; i++) XX[i] = new double[xN+1];
  
  double **ZZ = new double* [zN+1];        
  for(int i=0; i<zN+1; i++) ZZ[i] = new double[xN+1];

  for(int i = 0; i< zN+1 ; i++){
    for(int j = 0; j<xN+1; j++){	
      X[1] = -(x0 +xL/xN*j);		
      X[2] = 0.0;		
      X[3] = z0 +zL/zN*i; 		
      
      KAOSgetMagField(B, X);
      		
      field[i][j] = sqrt(B[1]*B[1] + B[2]*B[2] + B[3]*B[3]);
      
      XX[i][j] = X[1];
      ZZ[i][j] = X[3];
    }	
  }
  save_matrix_KAOS( zN, xN,field, "fieldKAOS.dat");
  save_matrix_KAOS( zN, xN,XX, "ikoordx.dat");
  save_matrix_KAOS( zN, xN,ZZ, "ikoordz.dat");
  return 0;
}

int KAOSthroughSpectrometer(SimParticleGroup *simParticleGroup1, double refP)
{
  for (int i=0; i<simParticleGroup1->getNumber(); i++){
    if ( simParticleGroup1->simParticle[i].status == 1)
    {
      int res = KAOScalculate_trajectory(&simParticleGroup1->simParticle[i], refP);
      // res == 3, if we dropped out of the spectrometer
      // res == 2, if simParticle reached the VDC successfuly
      // res == 1, if the integration took more time than t_max
      // res == 0, if integration step in routine odeint is to small
      // res == -1, if we ecxeed the maximal number of integration steps in odeint
  
      while (res == 1)  
      {
        // In this position particle decays.
        #ifdef DEBUG
          std::cout<<"Particle je razpadel znotraj magnetnega sistema: "<<std::endl;
        #endif

        int dres = simParticleDecay(&simParticleGroup1->simParticle[i], simParticleGroup1);
        if (dres < 0 ){
          #ifdef DEBUG
            std::cerr<<"Unknown decay mode!"<<std::endl;
          #endif
          return 0; 
        }
           
        // Now we follow new particle through the spectrometer.
        res = KAOScalculate_trajectory(&simParticleGroup1->simParticle[i], refP);
        
        if (simParticleGroup1->simParticle[i].position[3]<0.0 && res!=3){
          #ifdef DEBUG
            std::cerr<<"Out of spectrometer!"<<std::endl;
          #endif
          simParticleGroup1->simParticle[i].status = 0;
          break; 
        }           
        if (res==3) {
          #ifdef DEBUG
            std::cerr<<"Out of spectrometer!"<<std::endl;
          #endif
          simParticleGroup1->simParticle[i].status = 0;
          break;
        }
      }
      if (res==2) simParticleGroup1->simParticle[i].status = 1;
      else  simParticleGroup1->simParticle[i].status = 0;
    }
  }
  return 1;
}

int KAOSsave_particle_position(SimParticle *simParticle1, double cas){
#ifdef TRAJECTORY
  // We save information about current position of the particle.
  fprintf(KAOSstorefile,"%6d %8.2f %8.2f %8.2f %8.2f  %8.2f %8.2f %8.2f  %8.2f %8.2f %8.2f\n ", 0, cas, simParticle1->position[1], simParticle1->position[2], simParticle1->position[3], simParticle1->fVmomentum.beta()[0]*SpeedOfLight,simParticle1->fVmomentum.beta()[1]*SpeedOfLight, simParticle1->fVmomentum.beta()[2]*SpeedOfLight, simParticle1->spin[1], simParticle1->spin[2], simParticle1->spin[3]); 
#endif  
  return 1;
}

int KAOSfield_and_loss_free_move(SimParticle *simParticle1, double cas, int nstep){
  
  // Now we calculate velocity components.
  double vx = simParticle1->fVmomentum[1]/simParticle1->mass/simParticle1->fVmomentum.gamma() * SpeedOfLight; // In [mm/nsec]
  double vy = simParticle1->fVmomentum[2]/simParticle1->mass/simParticle1->fVmomentum.gamma() * SpeedOfLight; // In [mm/nsec]
  double vz = simParticle1->fVmomentum[3]/simParticle1->mass/simParticle1->fVmomentum.gamma() * SpeedOfLight; // In [mm/nsec]
  
  double dt  = cas/nstep;
  for(int i = 0; i<nstep; i++){
    simParticle1->position = simParticle1->position + dt*Vector3D(vx,vy,vz);
    KAOSsave_particle_position(simParticle1, 0.0); // var. time is not set correctly !!!!!
    if ( simParticle1->position[3]<0.0 ){
      simParticle1->status = 0;
      break;
    }
  }
  return 1;
}

int DSpectrometerKAOS::fromTargetToSpectrometer(SimParticleGroup *simParticleGroup1)
{
   
  double target[3];
  
  // We must examine every particle in the goup.
  for(int i = 0; i<simParticleGroup1->getNumber(); i++){
    #ifdef DEBUG
      cout<<"Serial number of the particle: "<<i<<endl;
    #endif

    // First we check the status of the particle.
    if (simParticleGroup1->simParticle[i].status==1){
      double vz = simParticleGroup1->simParticle[i].fVmomentum[3]/simParticleGroup1->simParticle[i].mass/simParticleGroup1->simParticle[i].fVmomentum.gamma() * SpeedOfLight; // In [mm/nsec]
      #ifdef DEBUG  
        std::cout<<"vz component of the velocity: "<<vz<<std::endl;
      #endif
 
      //First we calculate the time, needed for particle to travel distance Lk to the kolli  
      double tk = (Lk - simParticleGroup1->simParticle[i].position[3])/vz;
      while (tk >= simParticleGroup1->simParticle[i].livetime ){
    
        // In this case particle will decay
        // first we move to the position, where particle will decay:
        KAOSfield_and_loss_free_move(&simParticleGroup1->simParticle[i],simParticleGroup1->simParticle[i].livetime,10); 
        // Now we make decay:
        #ifdef DEBUG
          std::cout<<"Particle with ID: "<<simParticleGroup1->simParticle[i].ID<<", has decayed at: "<<simParticleGroup1->simParticle[i].position<<std::endl;
        #endif
        int dres = simParticleDecay(&simParticleGroup1->simParticle[i], simParticleGroup1);
        if (dres < 0 ){
          #ifdef DEBUG
            std::cerr<<"Unknown decay mode!\a"<<std::endl;
          #endif
          return 0; 
        }
        // Once we have new particle we again calculate time, need for particle to come
        // to the collimator:
        vz = simParticleGroup1->simParticle[i].fVmomentum[3]/simParticleGroup1->simParticle[i].mass/simParticleGroup1->simParticle[i].fVmomentum.gamma() * SpeedOfLight; 
        tk = (Lk - simParticleGroup1->simParticle[i].position[3])/vz; 
      }
      // Once we get tk<tau, particle will not decay before it comes to the colli.
      // Now we shell check if the particle will come through colli. 
      target[0] = simParticleGroup1->simParticle[i].position[1];        
      target[1] = simParticleGroup1->simParticle[i].position[2];    
      target[2] = simParticleGroup1->simParticle[i].position[3];
                
      if (collimator->accepted(target, simParticleGroup1->simParticle[i].fVmomentum, 0.0, 0.0,0.0)){      
        simParticleGroup1->simParticle[i].livetime = simParticleGroup1->simParticle[i].livetime - tk; 
        KAOSfield_and_loss_free_move(&simParticleGroup1->simParticle[i], tk,10);
        simParticleGroup1->simParticle[i].status = 1;
      }
      else{
        #ifdef DEBUG
          std::cerr<<"Crashed on colli!"<<std::endl;
        #endif 
        simParticleGroup1->simParticle[i].status = -1;
      } 
    }
  }
  return 1;
}


int DSpectrometerKAOS::simulate(SimParticleGroup *simParticleGroup1, double refp){

#ifdef TRAJECTORY
  // Open file to store positions, velocity and spin stepwise:	
  if (!(KAOSstorefile = fopen(QSPIN_DAT, "w"))){
    fprintf(stderr, " Cannot open file %s for output!\n", QSPIN_DAT);
    return 2;	
  }	
  
  // Open file to store magnetic fields stepwise:
  if (!(KAOSstorefile2 = fopen(QSPIN_B_DAT, "w"))){
    fprintf(stderr, " Cannot open file %s for output!\n", QSPIN_B_DAT);
    return 2;	
  }
#endif
 
  if (!fromTargetToSpectrometer(simParticleGroup1)){
    #ifdef DEBUG
      cout<<"Unknown decay mode!"<<endl;
    #endif
    return 0;
  }
                
  if (!KAOSthroughSpectrometer(simParticleGroup1,refp)){
    #ifdef DEBUG
      cout<<"Unknown decay mode!"<<endl;
    #endif
    return 0;
  }    
  	
#ifdef TRAJECTORY
  fclose(KAOSstorefile);	
  fclose(KAOSstorefile2);
#endif
  	
  return 1;
}

int DSpectrometerKAOS::position_at_VDC(Vector3D Position, Vector3D Velocity, Vector3D *VDCPosition, double *theta, double *phi)
{
  // For explanation see above..
  Vector3D O_D_vdc_x1(-948.9, 0.0, 2553.0); // Unsicher !!!!!!
  double tma_x     =   0.13594537; // mm
  double tma_y     =   0.0; // mm     
  double tma_phi   =  0.0; // mrad 
  double alpha = (0.0) * deg;

  Vector3D Ez_vdc( -cos(alpha), 0.0, -sin(alpha) );
  Vector3D Ex_vdc( -sin(alpha), 0.0,  cos(alpha) );
  Vector3D Ey_vdc = Ez_vdc && Ex_vdc;
  Matrix3D M_x(Ex_vdc, (tma_phi/1000.0) );  
  Ey_vdc = M_x * Ey_vdc;
  Ez_vdc = M_x * Ez_vdc; 
  Vector3D O_vdc = O_D_vdc_x1 -tma_x*Ex_vdc -tma_y*Ey_vdc;

  double t = ( Ez_vdc * (O_vdc - Position) ) / (Ez_vdc * Velocity); 
  
  #ifdef DEBUG
    std::cout<<"VDC: Position: "<<Position<<std::endl;
    std::cout<<"VDC: Velocity: "<<Velocity<<std::endl;
  #endif
 
  if(t>0.0){
    return 0;
  }

  Vector3D X_v  = Position + t * Velocity; 
  Vector3D DeltaX  = X_v - O_vdc; 
  Vector3D DeltaXc = Vector3D( Ex_vdc*DeltaX, Ey_vdc*DeltaX, Ez_vdc*DeltaX); 
  Vector3D v_VDC= Vector3D( Ex_vdc*Velocity, Ey_vdc*Velocity, Ez_vdc*Velocity); 
  
  // Calculate+Store Position and Velocity in the VDC-Coord.-Sys:
  *VDCPosition = Vector3D(DeltaXc[1],DeltaXc[2], DeltaXc[3]);

  #ifdef DEBUG
    cout<<"Position at VDC: [ "<<DeltaXc[1]<<", "<<DeltaXc[2]<<", "<<DeltaXc[3]<<" ]"<<endl;
  #endif

  // Now we calculate the angles of final particle
  *theta = 1000.0*atan(v_VDC[1]/v_VDC[3]);
  *phi = 1000.0*atan(v_VDC[2]/v_VDC[3]);

  return 1;
}



int DSpectrometerKAOS::showbound(){

    return 0;
}

int DSpectrometerKAOS::checkbound(){

   return 0;
}





int DSpectrometerKAOS::checkInsideSpectrometer(){
  FILE *inside;
  FILE *outside;
  inside=fopen("KAOSinsideSpec.dat","w");
  outside=fopen("KAOSoutsideSpec.dat","w");
  if ( inside && outside) {
    Vector3D X;
    int inval, j;

    #ifdef DEBUG
      std::cout << " please wait ..... " << std::endl;
    #endif

    for(j=0; j<=50000; j++){
      X[3] = 5000.0*(rand()/(RAND_MAX+1.0));
      X[1] = 2000.0 - 4000.0*(rand()/(RAND_MAX+1.0));
      if ((inval=KAOSinsideSpectrometer(X[1], 0.0, X[3]))) fprintf(inside,"%8.3f %8.3f \n",X[1], 1.0*X[3]);       
      else  fprintf(outside,"%8.3f %8.3f \n",X[1], 1.0*X[3]);

    }
    fclose(inside);fclose(outside);

  } else {
    fprintf(stderr," Couldn't open some boundary check data file!\n");
    return 0;
  }
  return 1;
}







