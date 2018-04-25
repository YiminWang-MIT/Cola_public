//
// $Id: DSpectrometerB.cc 2354 2009-11-13 13:58:04Z merkel $
//

#include <cmath>
#include <cstdlib>

#include "Cola/Masses.h"
#include "DSpectrometer.h"
#include "SimDecay.h"
#include "ran1.h"
#include <iostream>
#include <cstdio>

using namespace std;

#define QSPIN_DAT	"trajektorija.dat"
#define QSPIN_B_DAT	"qspin_B.dat"


// Global variables needed in out calculation. This is not the the best way :( 
double BgammaParticle; // This variable holds the value of factor gamma = sqrt(p*p+M*M)/M; 
double BK_p; // This variable stores the properties of the particle K_p = e/(M*gamma)
double Bg_p; // Gyromagnetic factor.

// Global variables for StoreStep
FILE   *Bstorefile=NULL;  // filehandle of open file for step storage
FILE   *Bstorefile2=NULL; // filehandle for storage of magnetic field strength 
static Vector3D Br_old(0.0,0.0,0.0);  // This vector cantains the position of the particle before making next step in the integration.
//static double Bact_length = 0.0;

DipolB BD; // We create the dipole for spectrometer B.


 DSpectrometerB:: DSpectrometerB(Collimator* colli, double lk, int fieldDirection){
   theta = 0.0;
   phi = 0.0;
   collimator = colli;
   Lk = lk;
   BD.B_Nom = fieldDirection*BD.B_Nom;
 }



// ------------------------------ getMagField ---------------------------------
// Calculates the magnetic field (vector3D B) in the absolute Coordinate system
// (ACS, x downwards) at a given position (vector3D r):

int BgetMagField(Vector3D &B, Vector3D &r){
  int bval=0;
   
  B[1]=0.0; B[2]=0.0; B[3]=0.0; // Polje B na zacetku nastavimo na nic. 
  if ( (bval=BD.inside(r)) ) B=B+BD.GetField(bval,r); 
  return 0;
} 

// ------------------------------ magmove -------------------------------------
// Routine which calculates the derivatives dydx[] of y[] with the ODEs 
// for the numerical Cash Karp Runge-Kutta solution:

void Bmagmove(double t,        // Time is independent variable.
              double y[],      // INPUT:  variables
              double dydx[])   // OUTPUT: their derivatives
{
  static Vector3D r;          // Position vector
  static Vector3D v;          // Velocity vector
  static Vector3D s;          // Spin vector
  static Vector3D dr, dv, ds; // derivetive vectors od position, velocity and spin 
  static Vector3D B;          // Magnetic field vector
  static Vector3D E_v;        // Unit vector, that is parallel to velocity. 
  static Vector3D B_L, B_T;   // Vectors of transversal and longitudinal components of the magnetic field
  
  // First we fill position, velocity and spin vectors.
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
  BgetMagField(B,r); 

  // Now we split calulated B into a longitudinal and a transversal part of B regarding to the velocity vector.
  E_v = v/v.abs();            // Unit vector parallel to the velocity
  B_L = (E_v * B) * E_v;      // Longitudinal component of the mag. field.
  B_T = B - B_L;	      // Transversal component of the mag. field.


  // Now we calculate derivatives that we need for solving ODE
  dr = v; 
  dv = BK_p * (v && B);  
  ds = BK_p * ( s && (Bg_p/2 * B_L + (1+BgammaParticle*(Bg_p-2)/2) * B_T) ); 

  // Now we put all values into vector.dydx[].
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

void BStoreStep(double t,   // independent variable
                double y[], // fields to store
                int ysize,  // size of y (y[1..ysize] must exist!)
                int stepN)  // number of the stored step
{

#ifdef TRAJECTORY
  int i; //Local variable for counting
  
  fprintf(Bstorefile,"%6d %8.2f  ", stepN, t); 
  for (i=1; i<=ysize; i++) fprintf(Bstorefile," %8.2f", y[i]); 
  fprintf(Bstorefile,"\n"); 
 
  Vector3D r_act(y[1],y[2],y[3]); 
  Vector3D B_act; 
  BgetMagField(B_act,r_act); 
  Bact_length += (r_act-Br_old).abs(); Br_old = r_act; 
  fprintf(Bstorefile2,"%10.4f %10.4f %10.4f %10.4f %10.4f \n",
	  Bact_length, B_act[1], B_act[2], B_act[3], B_act.abs());
#endif 
}


//----------------------------------------------- Check position of particle ------------------------
// This method checks if the particle has crossed the VDC-chamber.

int BcheckPosition(double yvalues[])
{	

  // First we create position and velocity vector.
  Vector3D Position(yvalues[1],yvalues[2],yvalues[3]);	
  Vector3D Velocity(yvalues[4],yvalues[5],yvalues[6]);

  

  
  // Durchtrittspkt. des Ref.-Strahls durch vdc x1-Ebene (ACS):
  // This vector represents the position of central trajectory in the VDC plane and is given in ADC.
  Vector3D O_D_vdc_x1(-6285.0, 0.0, 5327.13); // Unsicher !!!!!!
  
  // Deviation angle of spec B
  double phi_bend_ref = 110.0 *deg;
  
  // Here are specified the position and angle deviations from point zero. 
  double tma_x     =  85.8134; // mm
  double tma_theta =  46.7*deg; // mrad 
  double tma_y     =   0.0; // mm     
  double tma_phi   =  0.0; // mrad 
  

  // Alpha is angle between z-axes of ACS and x-axes of VDC system and is approx. 55deg
  double alpha= (phi_bend_ref-90.0*deg+tma_theta); 
  
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




int BinsideSpectrometer(double x, double y, double z){
   
  
  
  double topz[14] = { 4289.1,  4610.3, 4755.2, 4890.8, 4950.9, 5049.2, 5160.0, 5258.3, 5342.2, 5444.5, 5508.4, 5537.7, 5570.8,5587.3};
  
  double topx[14] = {-885.59, -885.59, -935.71, -979.81, -1015.9, -1092.2, -1174.4, -1285.2, -1383.7, -1544.4, -1661.3, -1767.2, -1914.4,-2049.1 };

  double bottomz[24] = {2968.2, 4388.5, 4389.3, 4671.4, 4989.4, 5096.6, 5293.9, 5518.5, 5723.2, 5916.2, 6222.8, 6419.3, 6638.0, 6729.4, 6931.7, 7093.4, 7222.1, 7365.5, 7503.1, 7605.7, 7727.5, 7763.6, 7810.0, 7810.7 };
  
  double bottomx[24] = { 928.75, 929.31, 961.88, 961.88, 930.95, 915.38, 877.32, 796.08, 719.14, 639.02, 484.4, 340.78, 185.77, 109.44, -96.05, -276.43, -457.8, -680.12, -941.91, -1185.4, -1557.1, -1717.5, -2099.3, -2594.2 };


  double leftz[9] = { 5585.3, 5570.8, 5554.2, 5542.7, 5517.8 , 5468.6 , 5466.0, 4444.0, 3769.0};

  double leftx[9] = {-2177.5, -2347.3, -2411.5, -2488.6,-2520.1, -2672.3, -3330.0,-3950.0,-5890.0};


  double rightz[6] = {7810.7, 7786.8, 7739.1, 7664.1, 7502.0, 6809.0};
  
  double rightx[6] = {-2594.2, -2849.0, -3087.4, -3332.7, -3960.0, -5896.0};
  
  if (z>7811.0) return 0; 
  if (z<0.0) return 0; 
        
  if (x>-2049.0 ){
    for(int i = 0; i<14-1; i++){ 
      if (z>=topz[i] && z<=topz[i+1]){                  
        if (x <= (topx[i+1] - topx[i])/(topz[i+1] - topz[i])*(z - topz[i]) + topx[i]) return 0;
      } 
    }

    for(int i = 0; i<24-1; i++){         
      if (z>=bottomz[i] && z<=bottomz[i+1]){                    
        if (x >= (bottomx[i+1] - bottomx[i])/(bottomz[i+1] - bottomz[i])*(z - bottomz[i]) + bottomx[i]) return 0;               
      }         
    }   
  }     
  else if(x> -5890.0){  
    if (z<3769 ) return 0;      
    for(int i = 0; i<9-1; i++){         
      if (z<=leftz[i] && z>=leftz[i+1]){                        
        if (x >= (leftx[i+1] - leftx[i])/(leftz[i+1] - leftz[i])*(z - leftz[i]) + leftx[i]) return 0;           
      } 
    }   
    for(int i = 0; i<6-1; i++){         
      if (z<=rightz[i] && z>=rightz[i+1]){                      
        if (x <= (rightx[i+1] - rightx[i])/(rightz[i+1] - rightz[i])*(z - rightz[i]) + rightx[i]) return 0;             
      } 
    }   
  }             
  else if(x>-9125.0){  
    if (z<3775 || z>6800) return 0;     
  }
  
  if (z > 4535.0 && x> - 3243.0){
    if((-x-2345.0)*(-x-2345.0)+(z-4520.0)*(z-4520.0) >2970.0*2970.0) return 0;
  }
  if (z > 4469.0 && x> - 2752.0){
    if((-x-2135.0)*(-x-2135.0)+(z-4295.0)*(z-4295.0) <1585.0*1585.0) return 0;
  }
  else if(x<-9125.0) return 0;

  
  // We check if the particle stays inside the dipole> 
  // This values are only approx.
  if ( z> 4469.0 && x > -3100.0){
    const double tilt = 3.495000 * M_PI/180; // pole piece tilt angle
    const double gap = 20; // gap width at 1.5Tline
    const double xe = 450; // entrance point
    const double T15Line = 55 * M_PI/180; // angle of 1.5T line
    const double s55=sin(T15Line), c55=cos(T15Line);
    const double D = gap/tilt;
    
    double xi[3] = {-x,-y, z};
    double xr[3] = {c55*xi[0]-s55*(xi[2]-xe)-D, xi[1], s55*xi[0]+c55*(xi[2]-xe)};
    double phi  = atan2(xr[1], -xr[0]);
    if (fabs(phi)>tilt/2.0) return 0;
  }
  else if (fabs(y) > 1000.0) return 0; // failsafe
 
  return 1; // if everything is OK!
}

int Bcalculate_trajectory(SimParticle *simParticle1, double refP){ 
  // First we set gyromagnetic ratio and mass
  double M = simParticle1->mass;	
  double e = simParticle1->charge;
  Bg_p = simParticle1->gfactor;
  
  // Number of ODEs (dimension of the arrays)
  int odenum = 9;

  // Vector containing start values for y:
  double *ystart = new double[odenum+1];

  Vector3D Position = simParticle1->position; // Initial position vector in mm !
  Vector3D Spin_tg = simParticle1->spin; // Spin vector
  double p = simParticle1->fVmomentum.momentum(); // Particle momentum in MeV/c
	
  //Now we rescale the magnetic field to the appropriate value for given  ref. momentum. 
  double errect = refP/810.0;  // 810 MeV is central momentum. 
  BD.ScaleField(errect);
  
  //Initial velocity vector is parallel to the momentum vector.
  Vector3D Velocity(simParticle1->fVmomentum[1],
                    simParticle1->fVmomentum[2],
                    simParticle1->fVmomentum[3]);  
  
  Velocity = Velocity/Velocity.abs(); 
  BgammaParticle = sqrt(p*p+M*M)/M;
  double betaParticle = sqrt(1.0 - 1.0/(BgammaParticle*BgammaParticle)); 
  
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
  	
  // e - is in [eV] 
  BK_p = e/M/BgammaParticle* 89.87551787 ; // factor K_p = e/(M*gamma) 
  	
  double starttime = 0;  // Initial time
  double endtime = simParticle1->livetime; // Integration time in nsec
  int nok, nbad, nstored=0;// Here will be stored numbers of good and bad integration steps  
  double acttime;

  #ifdef DEBUG	
    // User info before start:
    std::cout << "---------------------------------";
    std::cout << "---------------------------------" << std::endl;
    std::cout << " Momentum   : " << refP << " MeV/c" << std::endl;
    std::cout << " gamma of the particle: " << BgammaParticle << std::endl;
    std::cout << " Start position   : " << Position << std::endl;
    std::cout << " Start velocity   : " << Velocity;
    std::cout << ", beta :"<< betaParticle <<std::endl;
    std::cout << " Start spin (ACS) : " << Spin_tg  << std::endl;
    std::cout << " -----------------------------";
    std::cout << " calculating, please wait .........." << std::endl;
  #endif

  int res= odeint3(ystart,    // Start values
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
                   BStoreStep, // function which writes the step to file
                   Bmagmove,   // derivatives function
                   &acttime,  // actual value for the independent variable
                   BcheckPosition,
                   BinsideSpectrometer); 

  // out of ystart!!!, unlucky name, hihi, but thats NUM REC people,
  // just kept some of their variable names, so i'm innocent :-)	
  Position[1] = ystart[1];  // Final x	
  Position[2] = ystart[2];  // Final y	
  Position[3] = ystart[3];  // Final z	
  Velocity[1] = ystart[4];  // Final vx	
  Velocity[2] = ystart[5];  // Final vy	
  Velocity[3] = ystart[6];  // Final vz	
  betaParticle = Velocity.abs()/SpeedOfLight; // We calculate the beta factor of final particle
  
  // Spin vector of final particle
  Vector3D Spin_end(ystart[7],ystart[8],ystart[9]); 

  #ifdef DEBUG
    // User info (results):
    std::cout << " Act TIME   : " << acttime<< std::endl;
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


int save_matrixB(int M, int N, double **mat, char *name)
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

int DSpectrometerB::fieldMagnitude(){

  int xN = 1000;
  int zN = 1000;

  double x0 = -1000.0;
  double z0 = 4000.0;
  double xL = 5000.0;
  double zL = 4000.0;
  Vector3D X;
  Vector3D B;

  double **field = new double* [zN+1];	
  for(int i=0; i<zN+1; i++) field[i] = new double[xN+1];
	
  for(int i = 0; i< zN+1 ; i++){
    for(int j = 0; j<xN+1; j++){	
      X[1] = -(x0 +xL/xN*j);		
      X[2] = 0.0;		
      X[3] = z0 +zL/zN*i; 		
      B = BD.GetField(1,X);		
      field[i][j] = sqrt(B[1]*B[1] + B[2]*B[2] + B[3]*B[3]) ;	
    }	
  }
  save_matrixB( zN, xN,field, "fieldB.dat");
  return 0;
}

int BthroughSpectrometer(SimParticleGroup *simParticleGroup1, double refP)
{
  for (int i=0; i<simParticleGroup1->getNumber(); i++){
    if ( simParticleGroup1->simParticle[i].status == 1)
    {
      int res = Bcalculate_trajectory(&simParticleGroup1->simParticle[i], refP);
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
          std::cerr<<"Unknown decay mode!"<<std::endl;
          return 0; 
        }
           
        // Now we follow new particle through the spectrometer.
        res = Bcalculate_trajectory(&simParticleGroup1->simParticle[i], refP);
        
        if (simParticleGroup1->simParticle[i].position[3]<0.0 && res!=3){
          #ifdef DEBUG
            std::cerr<<"Out of spectrometer!"<<std::endl;
          #endif
          simParticleGroup1->simParticle[i].status = 0;
          //exit(1);
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

int Bsave_particle_position(SimParticle *simParticle1, double cas){
#ifdef TRAJECTORY
  // We save information about current position of the particle.
  fprintf(Bstorefile,"%6d %8.2f %8.2f %8.2f %8.2f  %8.2f %8.2f %8.2f  %8.2f %8.2f %8.2f\n ", 0, cas, simParticle1->position[1], simParticle1->position[2], simParticle1->position[3], simParticle1->fVmomentum.beta()[0]*SpeedOfLight,simParticle1->fVmomentum.beta()[1]*SpeedOfLight, simParticle1->fVmomentum.beta()[2]*SpeedOfLight, simParticle1->spin[1], simParticle1->spin[2], simParticle1->spin[3]); 
#endif  
  return 1;
}

int Bfield_and_loss_free_move(SimParticle *simParticle1, double cas, int nstep){
  
  // Now we calculate velocity components.
  double vx = simParticle1->fVmomentum[1]/simParticle1->mass/simParticle1->fVmomentum.gamma() * SpeedOfLight; // In [mm/nsec]
  double vy = simParticle1->fVmomentum[2]/simParticle1->mass/simParticle1->fVmomentum.gamma() * SpeedOfLight; // In [mm/nsec]
  double vz = simParticle1->fVmomentum[3]/simParticle1->mass/simParticle1->fVmomentum.gamma() * SpeedOfLight; // In [mm/nsec]
  
  double dt  = cas/nstep;
  for(int i = 0; i<nstep; i++){
    simParticle1->position = simParticle1->position + dt*Vector3D(vx,vy,vz);
    Bsave_particle_position(simParticle1, 0.0); // var. time is not set correctly !!!!!
    if ( simParticle1->position[3]<0.0 ){
      simParticle1->status = 0;
      break;
    }
  }
  return 1;
}

int DSpectrometerB::fromTargetToSpectrometer(SimParticleGroup *simParticleGroup1)
{
   
  double target[3];
  
  // We must examine every particle in the goup.
  for(int i = 0; i<simParticleGroup1->getNumber(); i++){
    #ifdef DEBUG
      cout<<"The serial number of the particle : "<<i<<endl;
    #endif
    // First we check the status of the particle.
    if (simParticleGroup1->simParticle[i].status==1){
   
      // Now we calculate the z-component of the velocity.
      double vz = simParticleGroup1->simParticle[i].fVmomentum[3]/simParticleGroup1->simParticle[i].mass/simParticleGroup1->simParticle[i].fVmomentum.gamma() * SpeedOfLight; // In [mm/nsec]
        
      #ifdef DEBUG
        std::cout<<"vz component of the velocity: "<<vz<<std::endl;
      #endif
 
      // First we calculate the time, needed for particle to travel distance Lk to the kolli  
  
      double tk = (Lk - simParticleGroup1->simParticle[i].position[3])/vz;
      #ifdef DEBUG
        std::cout<<"TK Time: "<<tk<<std::endl;
      #endif

      while (tk >= simParticleGroup1->simParticle[i].livetime ){
    
        // In this case particle will decay
        // first we move to the position, where particle will decay:
        Bfield_and_loss_free_move(&simParticleGroup1->simParticle[i],simParticleGroup1->simParticle[i].livetime,10); 
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
        // Once we have new particle we again calculate time, needed for particle to come to the collimator:
        vz = simParticleGroup1->simParticle[i].fVmomentum[3]/simParticleGroup1->simParticle[i].mass/simParticleGroup1->simParticle[i].fVmomentum.gamma() * SpeedOfLight; 
        tk = (Lk - simParticleGroup1->simParticle[i].position[3])/vz; 
      }
      // If tk<tau, particle will not decay before it comes to the colli.
      // Now we check if the particle will come through colli. 
      // Now we have to rotate the coordinates of the particles. We execute the rotation.      
      target[0] = simParticleGroup1->simParticle[i].position[2]/10.0;        
      target[1] = -simParticleGroup1->simParticle[i].position[1]/10.0;    
      target[2] = simParticleGroup1->simParticle[i].position[3]/10.0;
      
      
      FourVector momSpec2(simParticleGroup1->simParticle[i].fVmomentum[0],
                          simParticleGroup1->simParticle[i].fVmomentum[2],
                          -simParticleGroup1->simParticle[i].fVmomentum[1],
                          simParticleGroup1->simParticle[i].fVmomentum[3]);
                          
            
      if (collimator->accepted(target, momSpec2, 0.0, 0.0,0.0)){
        // Livetime of the particle is shortened for the path already traveled.        
        simParticleGroup1->simParticle[i].livetime = simParticleGroup1->simParticle[i].livetime - tk; 
        // Now we move our particle behind the collimator.
        Bfield_and_loss_free_move(&simParticleGroup1->simParticle[i], tk,10);
        simParticleGroup1->simParticle[i].status = 1;
      }
      else{
        if ((int)simParticleGroup1->simParticle[i].ID == P_piplus.getID()){
        #ifdef DEBUG  
	  std::cout<<"KOLI: th = "<<atan( simParticleGroup1->simParticle[i].position[1]/    simParticleGroup1->simParticle[i].position[3])<<" ph = "<<atan( simParticleGroup1->simParticle[i].position[2]/    simParticleGroup1->simParticle[i].position[3])<<std::endl;
	#endif
        }
        else{
	  #ifdef DEBUG
	    std::cerr<<"Crashed on colli!"<<std::endl;
	  #endif
	}
        
        simParticleGroup1->simParticle[i].status = -1;
      } 
    }
  }
  return 1;
}


int DSpectrometerB::simulate(SimParticleGroup *simParticleGroup1, double refp){
#ifdef DEBUG
    cerr<<"Ref Momentum: "<<refp<<endl;
#endif

#ifdef TRAJECTORY
  // Open file to store positions, velocity and spin stepwise:	
  if (!(Bstorefile = fopen(QSPIN_DAT, "w"))){
    fprintf(stderr, " Cannot open file %s for output!\n", QSPIN_DAT);
    return 2;	
  }	
  
  // Open file to store magnetic fields stepwise:
  if (!(Bstorefile2 = fopen(QSPIN_B_DAT, "w"))){
    fprintf(stderr, " Cannot open file %s for output!\n", QSPIN_B_DAT);
    fclose(Bstorefile);
    return 2;	
  }
#endif

  if (!fromTargetToSpectrometer(simParticleGroup1)){
    cout<<"Unknown decay mode!"<<endl;
#ifdef TRAJECTORY
    fclose(Bstorefile);
    fclose(Bstorefile2);
#endif
    return 0;
  }
                
  if (!BthroughSpectrometer(simParticleGroup1,refp)){
    cout<<"Unknown decay mode!"<<endl;
#ifdef TRAJECTORY
    fclose(Bstorefile);
    fclose(Bstorefile2);
#endif
    return 0;
  }    
  	
#ifdef TRAJECTORY
  fclose(Bstorefile);	
  fclose(Bstorefile2);
#endif
  	
  return 1;
}


// This method calculates the position of the final particle in VDC-plane in VDC-coordinate system.
int DSpectrometerB::position_at_VDC(Vector3D Position, Vector3D Velocity, Vector3D *VDCPosition, double *theta, double *phi)
{
  // For explanation see above...
  Vector3D O_D_vdc_x1(-6285.0, 0.0, 5327.13); // Unsicher !!!!!!
  double phi_bend_ref = 110.0 *deg;
  double tma_x     =   85.8134; // mm
  double tma_theta =   46.7*deg; // mrad 
  double tma_y     =   0.0; // mm     
  double tma_phi   =  0.0; // mrad 

  double alpha= (phi_bend_ref-90.0*deg+tma_theta); 
  
  #ifdef DEBUG
    std::cout<<"Alpha: "<<alpha<<std::endl;
  #endif
  
  Vector3D Ez_vdc( -cos(alpha), 0.0, -sin(alpha) ); 
  Vector3D Ex_vdc( -sin(alpha), 0.0,  cos(alpha) );
  Vector3D Ey_vdc = Ez_vdc && Ex_vdc;
  Matrix3D M_x(Ex_vdc, (tma_phi/1000.0) ); 
  Ey_vdc = M_x * Ey_vdc;
  Ez_vdc = M_x * Ez_vdc;

  Vector3D O_vdc = O_D_vdc_x1 -tma_x*Ex_vdc -tma_y*Ey_vdc;

  // Calculate+Store Position and Velocity in the VDC-Coord.-Sys:
  double t = ( Ez_vdc * (O_vdc - Position) ) / (Ez_vdc * Velocity);
  
  #ifdef DEBUG
    std::cout<<"VDC0: Position: "<<Position<<std::endl;
    std::cout<<"VDC0: Velocity: "<<Velocity<<std::endl;
  #endif
  
  if(t>0.0){
    return 0;
  }

  Vector3D X_v  = Position + t * Velocity;
  Vector3D DeltaX  = X_v - O_vdc; 
  Vector3D DeltaXc = Vector3D( Ex_vdc*DeltaX, Ey_vdc*DeltaX, Ez_vdc*DeltaX); 
  Vector3D v_VDC= Vector3D( Ex_vdc*Velocity, Ey_vdc*Velocity, Ez_vdc*Velocity);
  
  // This is the position of the particle in focal plane given in VDC-coordinate system.
  *VDCPosition = Vector3D(DeltaXc[1],DeltaXc[2], DeltaXc[3]);

  // Now we calculate the angles of final particle
  *theta = 1000.0*atan(v_VDC[1]/v_VDC[3]);
  *phi = 1000.0*atan(v_VDC[2]/v_VDC[3]);
  
  #ifdef DEBUG
    std::cout<<"VDC1: Position: "<<*VDCPosition<<std::endl;
  #endif

  return 1;
}



int DSpectrometerB::showbound(){

    // --- write magnet boundary data files:
    BD.showBoundary("meritve/Bd.dat", 50);
    return 1;
}

int DSpectrometerB::checkbound(){
  FILE *ind1;
  FILE *outside;
  ind1   =fopen("meritve/Bin_d.dat","w");

  outside=fopen("meritve/Boutside.dat","w");
  if ( ind1 && outside ) {
    Vector3D X;
    int inval, j;
    std::cout << " please wait ..... " << std::endl;
    for(j=0; j<=50000; j++){
      
      X[3] = 7500.0; 
      X[2] = -500.0 + 1000.0*(rand()/(RAND_MAX+1.0));
      X[1] = 1000.0 - 4200.0*(rand()/(RAND_MAX+1.0));
        
      if ( (inval=BD.inside(X)) ) {
        if ((inval & Magnet::HomogenField) && ((rand()/(RAND_MAX+1.0))>0.5))
          fprintf(ind1,"%8.3f %8.3f \n",X[2], -1.0*X[1]);
      }
      else if (!BD.inside(X)) 
        fprintf(outside,"%8.3f %8.3f \n",X[2], -1.0*X[1]);

    }
    fclose(ind1);
    fclose(outside);

  } else {
    fprintf(stderr," Couldn't open some boundary check data file!\n");
    return 0;
  }
  
    return 1;
}



int DSpectrometerB::checkInsideSpectrometer(){

  FILE *inside;
  FILE *outside;
  inside=fopen("meritve/BinsideSpec.dat","w");
  outside=fopen("meritve/BoutsideSpec.dat","w");
  if ( inside && outside) {
    Vector3D X;
    int inval, j;
    std::cout << " please wait ..... " << std::endl;
    for(j=0; j<=50000; j++){
      X[3] = 8000.0*(rand()/(RAND_MAX+1.0));
      X[1] = 2000.0 - 12000.0*(rand()/(RAND_MAX+1.0));
      if ((inval=BinsideSpectrometer(X[1], 0.0, X[3]))) fprintf(inside,"%8.3f %8.3f \n",X[1], 1.0*X[3]);       
      else  fprintf(outside,"%8.3f %8.3f \n",X[1], 1.0*X[3]);

    }
    fclose(inside);fclose(outside);

  } else {
    fprintf(stderr," Couldn't open some boundary check data file!\n");
    return 0;
  }
  return 1;
    return 0;
}







