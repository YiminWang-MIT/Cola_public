//
// $Id: DSpectrometerA.cc 2354 2009-11-13 13:58:04Z merkel $
//

#include <cmath>
#include <cstdlib>

#include "Cola/Masses.h"
#include "DSpectrometer.h"
#include "SimDecay.h"
#include "SpecA.h"
#include "ran1.h"
#include <iostream>
#include <cstdio>

#define QSPIN_DAT	"trajektorija.dat"
#define QSPIN_B_DAT	"qspin_B.dat"
#define QSPIN_DPS	"qspin.dps"

using namespace std;

// Global Variables:
double gammaParticle; //This variable represents the gamma factor:
                      // gamma = sqrt(p*p+M*M)/M; 
double K_p; // In given variable are stored all information of given particle:             
            // K_p = e/(M*gamma)
double g_p; // Giromagnetic gactor

// Global variables for StoreStep
FILE   *storefile=NULL;  // filehandle of open file for step storage
FILE   *storefile2=NULL; // filehandle for storage of magnetic field strength 
static Vector3D r_old(0.0,0.0,0.0);  // Given vector tells the position of the                                      // particle before we make next step
//static double act_length = 0.0;  // In this variable is stored the total       
                                 // length of the path of the particle


 DSpectrometerA:: DSpectrometerA(Collimator* colli, double lk, int fieldDirection){
   // Angles of the coolimator
   theta = 0.0;
   phi = 0.0;
   collimator = colli;
   Lk = lk;
   for(int i = 0; i<=6; i++) Sext.B_Nom[i] = fieldDirection*Sext.B_Nom[i];
   for(int i = 0; i<=6; i++) Quad.B_Nom[i] = fieldDirection*Quad.B_Nom[i];
   D1.B_Nom = fieldDirection*D1.B_Nom;
   D2.B_Nom = fieldDirection*D2.B_Nom;
 }


// ------------------------------ getMagField ---------------------------------
// Calculates the magnetic field (vector3D B) in the absolute Coordinate system 
// (ACS, x downwards) at a given position (vector3D r):

int getMagField(Vector3D &B, Vector3D &r){
  int bval=0; 
  B[1]=0.0; B[2]=0.0; B[3]=0.0; // In teh beginning we set B to the zero

  // Normally 'if ( (bval=Element.inside(r)) ) B=B+Element.GetField(bval,r);'
  // is enough, but adding if-statements about the rough position makes
  // the program running faster
 
  // First we check if we are inside Quadrupol. If so, we calculate it's 
  // contribution to the total value of the field.
  if ((r[3] < 2310))
  {
    if ( (bval=Quad.inside(r)) ) B=B+Quad.GetField(bval,r);    
  }

  // Now we check if we are inside Sextupol. If so, we calculate it's 
  // contribution to the total value of the field.
  if ((r[3] > 1600) && (r[3] < 3100))
  {
    if ( (bval=Sext.inside(r)) ) B=B+Sext.GetField(bval,r);    
  }
  
  // Now we check if we are inside First Dipol. If so, we calculate it's 
  // contribution to the total value of the field.
  if ((r[3] > 2400) && (r[1]>-2000))
  {
    if ( (bval=D1.inside(r)) ) B=B+D1.GetField(bval,r);    
  }
  
  // In the end we check, if we are inside Second Dipol. If so, we calculate  
  // it's contribution to the total value of the field.
  if ((r[3] > 3500) && (r[1]<-1000))
  {
    if ( (bval=D2.inside(r)) ) B=B+D2.GetField(bval,r);    
  }
  return 0;
} 


// ------------------------------ magmove -------------------------------------
// Routine which calculates the derivatives dydx[] of y[] with the ODEs 
// for the numerical Cash Karp Runge-Kutta solution:

void magmove(double t, 	      // time is indipendent variable
	     double y[],      // INPUT:  variables
	     double dydx[])   // OUTPUT: their derivatives
{
  static Vector3D r;          // Position vector
  static Vector3D v;          // Velocity vector
  static Vector3D s;          // Spin vector
  static Vector3D dr, dv, ds; // Vectors of time derivetives of position, velocity, spin 
  static Vector3D B;          // Vector of magntic field
  static Vector3D E_v;        // Unit vctor, parallel to velocity vector.
  static Vector3D B_L, B_T;   // Transversal and longitudinal magnetic field compoment vectors

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
  getMagField(B,r); 

  // Split calulated B in a longitudinal and a transversal part to v:
  E_v = v/v.abs();            // Now we create  unit vector, parallel to velocity vector.
  B_L = (E_v * B) * E_v;      // Longitudinal component of nagnetic field wector.
  B_T = B - B_L;	      // The difference gives us the transversal component of magnetic field.	


  
  // Now we calculate derivatives we need to calculate Ordinary Differential Equations
  dr = v; 
  dv = K_p * (v && B); 
  ds = K_p * ( s && (g_p/2 * B_L + (1+gammaParticle*(g_p-2)/2) * B_T) ); 

  // Now we put calculated values into vector dydx[], which is appropriate for solving ODE.
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

void StoreStep(double t,   // independent variable
	       double y[], // fields to store
	       int ysize,  // size of y (y[1..ysize] must exist!)
	       int stepN)  // number of the stored step
{

#ifdef TRAJECTORY
  int i; // Local variable for counting
  fprintf(storefile,"%6d %8.2f  ", stepN, t); //First we save the serial number and time at given step of integration.

  for (i=1; i<=ysize; i++) fprintf(storefile," %8.2f", y[i]); // Now we save position, velocity and spin.
  fprintf(storefile,"\n"); // We end the line
 
  Vector3D r_act(y[1],y[2],y[3]); //We create position vector, in which we specify where we want to calculate B.
  Vector3D B_act; // We create the magnetic field vector.
  getMagField(B_act,r_act); // We calculate the magnetic field at r_act; 
  act_length += (r_act-r_old).abs(); r_old = r_act; // We add the length of the path made in given integration step, to the total path
  fprintf(storefile2,"%10.4f %10.4f %10.4f %10.4f %10.4f \n",
	  act_length, B_act[1], B_act[2], B_act[3], B_act.abs()); // We save magnetic field into the file
#endif  
}

//----------------------------------------------- Check position of particle ------------------------
// This method checks if the particle has crossed the VDC-chamber.

int checkPosition(double yvalues[])
{
  // We create the position and velocity vector.
  Vector3D Position(yvalues[1],yvalues[2],yvalues[3]);
  Vector3D Velocity(yvalues[4],yvalues[5],yvalues[6]);

  // This vector specifies the position of central ray in the focal plane, given in ACS.
  Vector3D O_D_vdc_x1(-6081.59, 0.0, 4949.3); // Unsicher !!!!!!

  // Bending angle of central ray in spec A.
  double phi_bend_ref = 100.0238 *deg;

  // Here are specified the position and angle deviations from point zero. 
  // Tma-Offsets   (qsdda665.mxl):
  double tma_x     = 0.0; // mm
  double tma_theta = 787.9; // mrad 
  double tma_y     = 0.0;  // mm     
  double tma_phi   = 0.0; // mrad 
  
  
  double alpha= (phi_bend_ref-90.0*deg+tma_theta/1000.0); 
  
  Vector3D Ez_vdc( -cos(alpha), 0.0, -sin(alpha) ); // Coords of z-axis of  VDC system given in ACS 
  Vector3D Ex_vdc( -sin(alpha), 0.0,  cos(alpha) ); // Coords of x-axis of  VDC system given in ACS
  Vector3D Ey_vdc = Ez_vdc && Ex_vdc;  		    // Coords of y-axis of  VDC system given in ACS
   
  // Now we have to rotate this CS  aroud x'-VDC. 
  Matrix3D M_x(Ex_vdc, (tma_phi/1000.0) );  // Rotation matrix, which rotates aroud x' for angle tma_phi 
  Ey_vdc = M_x * Ey_vdc; // Now we rotate y'-axis  
  Ez_vdc = M_x * Ez_vdc; // We rotate z'-axis
  
  //We calculate the position of center of VDC cs. given in ACS. 
  Vector3D O_vdc = O_D_vdc_x1 -tma_x*Ex_vdc -tma_y*Ey_vdc;


  Vector3D DeltaX  = Position - O_vdc; // For given point of position we calculate the deviation from center of VDC (in ADC).
  Vector3D DeltaXc = Vector3D( Ex_vdc*DeltaX, Ey_vdc*DeltaX, Ez_vdc*DeltaX); // Now we transform this difference in to VDC cs.
 
  if (DeltaXc[1]>-1000.0 && DeltaXc[3]>0.0) return 1;
  
  return 0;
}

int insideSpectrometer(double x, double y, double z){

  double topz[9] ={712.5, 1906.0, 2125.0, 2562.0, 2879.0, 3818.0, 4068.0, 4568.0, 4569.0};
  double topx[9] ={-562.5, -562.5, -687.5, -687.5, -844.9, -1690.0, -1815.0, -3380.0, -4006.0 };

  double bottomz[8] = {712.5, 1906.0, 2125.0, 2562.0, 3943.0, 5101.0, 7250.0, 7254.0};
  double bottomx[8] = {562.5, 562.5, 687.5, 687.5, 1001.0, 250.0, -1815.0, -4006.0};


  double leftz[3] = {4569.0, 3844.0, 3175.0};
  double leftx[3] = {-4006.0, -4450.0, -6375.0};


  double rightz[3] = {7254.0, 6875.0, 6188.0};
  double rightx[3] = {-4006.0, -4450.0, -6375.0};

  if (z>7254.0) return 0; 
  if (z<0.0) return 0; 
  	
  if (x>-4006.0 ){
    for(int i = 0; i<9-1; i++){	
      if (z>=topz[i] && z<=topz[i+1]){			
        if (x <= (topx[i+1] - topx[i])/(topz[i+1] - topz[i])*(z - topz[i]) + topx[i]) return 0;
      } 
    }

    for(int i = 0; i<8-1; i++){		
      if (z>=bottomz[i] && z<=bottomz[i+1]){			
        if (x >= (bottomx[i+1] - bottomx[i])/(bottomz[i+1] - bottomz[i])*(z - bottomz[i]) + bottomx[i]) return 0;		
      }		
    }	
  }	
  else if(x> -6375.0){	
    if (z<3175 ) return 0;	
    for(int i = 0; i<3-1; i++){		
      if (z<=leftz[i] && z>=leftz[i+1]){			
        if (x >= (leftx[i+1] - leftx[i])/(leftz[i+1] - leftz[i])*(z - leftz[i]) + leftx[i]) return 0;		
      }	
    } 	
    for(int i = 0; i<3-1; i++){		
      if (z<=rightz[i] && z>=rightz[i+1]){			
        if (x <= (rightx[i+1] - rightx[i])/(rightz[i+1] - rightz[i])*(z - rightz[i]) + rightx[i]) return 0;		
      }	
    }	
  }		
  else if(x>-10000.0){	
    if (z<3175 || z>6188) return 0;	
  }
  
  if (z > 792.0 && z < 1808.0){
    // quadrupol
    if (x*x + y*y > 200.0*200.0) return 0;  
  }
  
  if (z > 2115.0 && z < 2585.0){
    // sextupol
    if (fabs(y)>120.0 || fabs(x)>350.0) return 0; 
  }
  
  if (z > 2585.0 && x > -4006.0 ){
    // dipols
    if (fabs(y) > 100.0)return 0; 
  }
  
  if (fabs(y) > 1000.0) return 0; // failsafe
 
  return 1; // if everything is OK!
}



int calculate_trajectory(SimParticle *simParticle1, double refP)
{ 	
  
  // First we set mass and gyromagnetic factor.
  double M = simParticle1->mass;	
  double e = simParticle1->charge;
  g_p = simParticle1->gfactor;
  
  // Number of ODEs (dimension of the arrays)
  int odenum = 9;

  // Vector containing start values for y:
  double *ystart = new double[odenum+1];


  Vector3D Position = simParticle1->position; // Initial position factor in mm !
  Vector3D Spin_tg = simParticle1->spin; // Spin vector of the particle
  double p = simParticle1->fVmomentum.momentum(); // Particle momentum in MeV/c
	
  // Now we rescale te field.
  //double errect = refP/630.0;  // 630 MeV  is ref. momentum 
  double errect = refP/665.0;  // 665 MeV is ref. momentum.
  Sext.ScaleField(errect);
  Quad.ScaleField(errect);
  D1.ScaleField(errect);
  D2.ScaleField(errect);
  
  // Zacetni vektor hitrosti kaze v isti smeri kot gibalna kolicina 
  // Initial velocity vector is parallel to momentum. 
  Vector3D Velocity(simParticle1->fVmomentum[1],
                    simParticle1->fVmomentum[2],
                    simParticle1->fVmomentum[3]);
   
  Velocity = Velocity/Velocity.abs(); 
  gammaParticle = sqrt(p*p+M*M)/M; 
  double betaParticle = sqrt(1.0 - 1.0/(gammaParticle*gammaParticle));
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
      
  
  K_p = e/M/gammaParticle* 89.87551787;   // factor K_p = e/(M*gamma)
  
  double starttime = 0;  // Initial time
  double endtime = simParticle1->livetime; // Integration time in ns
  int nok, nbad, nstored=0;  // Here will be stored numbers of good and bad integration steps
  double acttime;
  
  #ifdef DEBUG
    // User info before start:
    std::cout << "---------------------------------";
    std::cout << "---------------------------------" << std::endl;
    std::cout << " REF Momentum   : " << refP << " MeV/c" << std::endl;
    std::cout << " Particle Momentum   : " << simParticle1->fVmomentum.momentum() << " MeV/c" << std::endl;
    std::cout << " gamma of the particle: " << gammaParticle << std::endl;
    std::cout << " Start position   : " << Position << std::endl;
    std::cout << " Start velocity   : " << Velocity<<std::endl;
    std::cout << " Charge           : " << simParticle1->charge<<std::endl;
    std::cout << " Particle ID      : " << simParticle1->ID<<std::endl;
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
                   StoreStep, // function which writes the step to file
                   magmove,   // derivatives function
                   &acttime,  // actual value for the independent variable
                   checkPosition,
		   insideSpectrometer); 
  
  //Get END position, velocity and spin:
  // out of ystart!!!, unlucky name, hihi, but thats NUM REC people,
  // just kept some of their variable names, so i'm innocent :-)	
  Position[1] = ystart[1];  // Final x
  Position[2] = ystart[2];  // Final y
  Position[3] = ystart[3];  // Final z
  Velocity[1] = ystart[4];  // Final vx
  Velocity[2] = ystart[5];  // Final vy
  Velocity[3] = ystart[6];  // Final vz	
  
  betaParticle = Velocity.abs()/SpeedOfLight; // Final beta factor of the particle
  Vector3D Spin_end(ystart[7],ystart[8],ystart[9]); // we create new spin vector.
  
  #ifdef DEBUG
    // User info (results):
    std::cout << " Number of good steps   : " << nok  << std::endl;
    std::cout << " Number of bad steps    : " << nbad << std::endl;
    std::cout << " Number of stored steps : " << nstored << std::endl;
    std::cout << " End position   : " << Position << std::endl;
    std::cout << " End velocity   : " << Velocity <<std::endl;
    std::cout << ", beta = "<< betaParticle <<std::endl;
    std::cout << " Charge           : " << simParticle1->charge<<std::endl;
    std::cout << " Particle ID      : " << simParticle1->ID<<std::endl;
    std::cout << " End spin (ACS) : " << Spin_end << std::endl;	
  #endif

  // Now we save new particle's position and momentum into the simParticle
  simParticle1->position = Position;

  Vector3D gibalnaK = Velocity/Velocity.abs()*simParticle1->fVmomentum.momentum();

  simParticle1->fVmomentum = FourVector( simParticle1->fVmomentum[0],gibalnaK[1],gibalnaK[2],gibalnaK[3]);
  
  simParticle1->spin = Spin_end;
  
  #ifdef DEBUG
    std::cout << " End position 2  : " << simParticle1->position << std::endl;
    std::cout<<"Trajektorija izracunana!"<<std::endl;    
  #endif

  return res;
}

int throughSpectrometer(SimParticleGroup *simParticleGroup1, double refP)
{
  for (int i=0; i<simParticleGroup1->getNumber(); i++){
    if ( simParticleGroup1->simParticle[i].status == 1)
    {
      int res = calculate_trajectory(&simParticleGroup1->simParticle[i], refP);
      // res == 3, if we dropped out of the spectrometer
      // res == 2, if simParticle reached the VDC successfuly
      // res == 1, if the integration took more time than t_max
      // res == 0, if integration step in routine odeint is to small
      // res == -1, if we ecxeed the maximal number of integration steps in odeint
  
      while (res == 1)	
      {
        // In this position simParticle decays.
	#ifdef DEBUG
          std::cout<<"Particle has decayed inside magnetic field: "<<std::endl;
        #endif

        int dres = simParticleDecay(&simParticleGroup1->simParticle[i], simParticleGroup1);
        if (dres < 0 ){
	  #ifdef DEBUG	
            std::cout<<"Unknown decay mode!"<<std::endl;
          #endif
          return 0; 
        }
           
        // Now we follow new particle through the spectrometer.
        res = calculate_trajectory(&simParticleGroup1->simParticle[i], refP);
        
        if (simParticleGroup1->simParticle[i].position[3]<0.0 && res!=3){
          #ifdef DEBUG
            std::cout<<"Out of spectrometer!"<<std::endl;
          #endif

          simParticleGroup1->simParticle[i].status = 0;
          exit(1);
          //return -1; 
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



int save_particle_position(SimParticle *simParticle1, double cas){
#ifdef TRAJECTORY
  // We save information about current position of the particle.
  fprintf(storefile,"%6d %8.2f %8.2f %8.2f %8.2f  %8.2f %8.2f %8.2f  %8.2f %8.2f %8.2f\n ", 0, cas, simParticle1->position[1], simParticle1->position[2], simParticle1->position[3], simParticle1->fVmomentum.beta()[0]*SpeedOfLight,simParticle1->fVmomentum.beta()[1]*SpeedOfLight, simParticle1->fVmomentum.beta()[2]*SpeedOfLight, simParticle1->spin[1], simParticle1->spin[2], simParticle1->spin[3]); 
#endif  
  return 1;
}

int field_and_loss_free_move(SimParticle *simParticle1, double cas, int nstep){
  
  // Now we calculate velocity components.
  double vx = simParticle1->fVmomentum[1]/simParticle1->mass/simParticle1->fVmomentum.gamma() * SpeedOfLight; // In [mm/nsec]
  double vy = simParticle1->fVmomentum[2]/simParticle1->mass/simParticle1->fVmomentum.gamma() * SpeedOfLight; // In [mm/nsec]
  double vz = simParticle1->fVmomentum[3]/simParticle1->mass/simParticle1->fVmomentum.gamma() * SpeedOfLight; // In [mm/nsec]
  
  double dt  = cas/nstep;
  for(int i = 0; i<nstep; i++){
    simParticle1->position = simParticle1->position + dt*Vector3D(vx,vy,vz);
    save_particle_position(simParticle1, 0.0); // var. time is not set correctly !!!!!
  }
  return 1;
}

int DSpectrometerA::fromTargetToSpectrometer(SimParticleGroup *simParticleGroup1)
{

  double target[3];

  // We must examine every particle in the goup.
  for(int i = 0; i<simParticleGroup1->getNumber(); i++){
    #ifdef DEBUG
      cout<<"Serial number of the particle is: "<<i<<endl;
    #endif
    // First we check the status of the particle.
    if (simParticleGroup1->simParticle[i].status==1){
      // Now we calculate the z-component of the velocity.
      double vz = simParticleGroup1->simParticle[i].fVmomentum[3]/simParticleGroup1->simParticle[i].mass/simParticleGroup1->simParticle[i].fVmomentum.gamma() * SpeedOfLight; // In [mm/nsec]
        
      #ifdef DEBUG
        std::cout<<"vz component of the velocity: "<<vz<<std::endl;
      #endif
      //First we calculate the time, needed for particle to travel distance Lk to the colli  
  
      double tk = (Lk - simParticleGroup1->simParticle[i].position[3])/vz;
      while (tk >= simParticleGroup1->simParticle[i].livetime){
        // In this case particle will decay
        // first we move to the position, where particle will decay:
        field_and_loss_free_move(&simParticleGroup1->simParticle[i],simParticleGroup1->simParticle[i].livetime,10); 
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
      // Now we shell check if the particle will come through colli. 
      // Now we have to rotate the coordinates of the particles. We execute the rotation.      
      target[0] = simParticleGroup1->simParticle[i].position[2];        
      target[1] = -simParticleGroup1->simParticle[i].position[1];    
      target[2] = simParticleGroup1->simParticle[i].position[3];

      FourVector momSpec2(simParticleGroup1->simParticle[i].fVmomentum[0]/1000.0,
                          simParticleGroup1->simParticle[i].fVmomentum[2]/1000.0,
                          -simParticleGroup1->simParticle[i].fVmomentum[1]/1000.0,
                          simParticleGroup1->simParticle[i].fVmomentum[3]/1000.0);

      if (collimator->accepted(target, momSpec2, 0.0, 0.0, 0.0)){
        // Livetime of the particle is shortened for the path already traveled.
        simParticleGroup1->simParticle[i].livetime = simParticleGroup1->simParticle[i].livetime - tk; 
        // Now we move our particle behind the collimator.
        field_and_loss_free_move(&simParticleGroup1->simParticle[i], tk,10);
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


int DSpectrometerA::simulate(SimParticleGroup *simParticleGroup1, double refp){	
#ifdef TRAJECTORY
  // Open file to store positions, velocity and spin stepwise:
  if (!(storefile = fopen(QSPIN_DAT, "w"))) {	
    fprintf(stderr, " Cannot open file %s for output!\n", QSPIN_DAT);
    return 2;	
  }	
  // Open file to store magnetic fields stepwise:	
  if (!(storefile2 = fopen(QSPIN_B_DAT, "w"))) {
    fprintf(stderr, " Cannot open file %s for output!\n", QSPIN_B_DAT);
    fclose(storefile);
    return 2;	
  }	
#endif
   
  if (!fromTargetToSpectrometer(simParticleGroup1)){
    cout<<"Unknown decay mode!"<<endl;
#ifdef TRAJECTORY
    fclose(storefile);
    fclose(storefile2);
#endif
    return 0;
  }


  if (!throughSpectrometer(simParticleGroup1,refp)){
    cout<<"Unknown decay mode!"<<endl;
#ifdef TRAJECTORY
    fclose(storefile);
    fclose(storefile2);
#endif
    return 0;
  }     	
    
#ifdef TRAJECTORY
  fclose(storefile);
  fclose(storefile2);	
#endif
  return 1;
}

// This method calculates the position of the final particle in VDC-plane in VDC-coordinate system.
int DSpectrometerA::position_at_VDC(Vector3D Position, Vector3D Velocity, Vector3D *VDCPosition, double *theta, double *phi)
{

  // For explanation see above...
  Vector3D O_D_vdc_x1(-6081.59, 0.0, 4949.3); // Unsicher !!!!!!
  double phi_bend_ref = 100.0238 *deg;
  double tma_x     =  0.0; // mm
  double tma_theta =  787.897; // mrad 
  double tma_y     =  0.0; // mm     
  double tma_phi   =  0.0; // mrad 

  double alpha= (phi_bend_ref-90.0*deg+tma_theta/1000.0); 
  
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
    std::cout<<"Time back to VDC: "<<t<<endl;
    std::cout<<"VDC: Position: "<<Position<<std::endl;
    std::cout<<"VDC: Velocity: "<<Velocity<<std::endl;
  #endif
  if(t>0.0){
    //exit(1);
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
	
  return 1;
}

int DSpectrometerA::showbound(){

    // --- write magnet boundary data files:
    Quad.showBoundary("meritve/Aquad.dat", 50);
    Quad.showEntranceBoundary("meritve/Aquad_En.dat", 50);
    Quad.showExitBoundary("meritve/Aquad_Ex.dat", 50);
    Sext.showBoundary("meritve/Asext.dat", 50);
    Sext.showEntranceBoundary("meritve/Asext_En.dat", 50);
    Sext.showExitBoundary("meritve/Asext_Ex.dat", 50);
    D1.showBoundary("meritve/Ad1.dat", 50);
    D1.showEntranceBoundary("meritve/Ad1_En.dat", 50);
    D1.showExitBoundary("meritve/Ad1_Ex.dat", 50);
    D2.showBoundary("meritve/Ad2.dat", 50);
    D2.showEntranceBoundary("meritve/Ad2_En.dat", 50);
    D2.showExitBoundary("meritve/Ad2_Ex.dat", 50);

    return 1;
}

int DSpectrometerA::checkbound(){
    FILE *inquad_En, *inquad, *inquad_Ex;
    FILE *insext_En, *insext, *insext_Ex;
    FILE *ind1_En, *ind1, *ind1_Ex;
    FILE *ind2_En, *ind2, *ind2_Ex;
    FILE *outside;
    inquad_En=fopen("ps/Ain_quad_En.dat","w");
    inquad   =fopen("ps/Ain_quad.dat","w");
    inquad_Ex=fopen("ps/Ain_quad_Ex.dat","w");
    insext_En=fopen("ps/Ain_sext_En.dat","w");
    insext   =fopen("ps/Ain_sext.dat","w");
    insext_Ex=fopen("ps/Ain_sext_Ex.dat","w");
    ind1_En=fopen("ps/Ain_d1_En.dat","w");
    ind1   =fopen("ps/Ain_d1.dat","w");
    ind1_Ex=fopen("ps/Ain_d1_Ex.dat","w");
    ind2_En=fopen("ps/Ain_d2_En.dat","w");
    ind2   =fopen("ps/Ain_d2.dat","w");
    ind2_Ex=fopen("ps/Ain_d2_Ex.dat","w");
    outside=fopen("ps/Aoutside.dat","w");
    if ( ind1_En && ind1 && ind1_Ex && ind2_En && ind2 && ind2_Ex && 
	 inquad_En && inquad && inquad_Ex && insext_En && insext && insext_Ex 
	 && outside ) {
      Vector3D X;
      int inval, j;
      std::cout << " please wait ..... " << std::endl;
      for(j=0; j<=30000; j++){
	X[3] = 8000.0*(rand()/(RAND_MAX+1.0));
	X[1] = -6000.0 + 8000.0*(rand()/(RAND_MAX+1.0));
	if ( (inval=Quad.inside(X)) ) {
	  if ((inval & Magnet::HomogenField) && ((rand()/(RAND_MAX+1.0))>0.5))
	    fprintf(inquad,"%8.3f %8.3f \n",X[3], -1.0*X[1]);
	  if ((inval & Magnet::EntranceField) && ((rand()/(RAND_MAX+1.0))>0.5))
	    fprintf(inquad_En,"%8.3f %8.3f \n",X[3], -1.0*X[1]);
	  if ((inval & Magnet::ExitField) && ((rand()/(RAND_MAX+1.0))>0.5))
	    fprintf(inquad_Ex,"%8.3f %8.3f \n",X[3], -1.0*X[1]);
	}
	if ( (inval=Sext.inside(X)) ) {
	  if ((inval & Magnet::HomogenField) && ((rand()/(RAND_MAX+1.0))>0.5))
	    fprintf(insext,"%8.3f %8.3f \n",X[3], -1.0*X[1]);
	  if ((inval & Magnet::EntranceField) && ((rand()/(RAND_MAX+1.0))>0.5))
	    fprintf(insext_En,"%8.3f %8.3f \n",X[3], -1.0*X[1]);
	  if ((inval & Magnet::ExitField) && ((rand()/(RAND_MAX+1.0))>0.5))
	    fprintf(insext_Ex,"%8.3f %8.3f \n",X[3], -1.0*X[1]);
	}
	if ( (inval=D1.inside(X)) ) {
	  if ((inval & Magnet::HomogenField) && ((rand()/(RAND_MAX+1.0))>0.5))
	    fprintf(ind1,"%8.3f %8.3f \n",X[3], -1.0*X[1]);
	  if ((inval & Magnet::EntranceField) && ((rand()/(RAND_MAX+1.0))>0.5))
	    fprintf(ind1_En,"%8.3f %8.3f \n",X[3], -1.0*X[1]);
	  if ((inval & Magnet::ExitField) && ((rand()/(RAND_MAX+1.0))>0.5))
	    fprintf(ind1_Ex,"%8.3f %8.3f \n",X[3], -1.0*X[1]);
	}
	if ( (inval=D2.inside(X)) ) {
	  if ((inval & Magnet::HomogenField) && ((rand()/(RAND_MAX+1.0))>0.5))
	    fprintf(ind2,"%8.3f %8.3f \n",X[3], -1.0*X[1]);
	  if ((inval & Magnet::EntranceField) && ((rand()/(RAND_MAX+1.0))>0.5))
	    fprintf(ind2_En,"%8.3f %8.3f \n",X[3], -1.0*X[1]);
	  if ((inval & Magnet::ExitField) && ((rand()/(RAND_MAX+1.0))>0.5))
	    fprintf(ind2_Ex,"%8.3f %8.3f \n",X[3], -1.0*X[1]);
	}
	else if (!D1.inside(X)) 
	  fprintf(outside,"%8.3f %8.3f \n",X[3], -1.0*X[1]);

      }
      fclose(ind1); fclose(ind1_En); fclose(ind1_Ex);
      fclose(ind2); fclose(ind2_En); fclose(ind2_Ex);
      fclose(insext); fclose(insext_En); fclose(insext_Ex);
      fclose(inquad); fclose(inquad_En); fclose(inquad_Ex);
      fclose(outside);

    } else {
      fprintf(stderr," Couldn't open some boundary check data file!\n");
      return 0;
    }
    return 1;
}

int DSpectrometerA::checkInsideSpectrometer(){
    FILE *inside;
    FILE *outside;
    inside=fopen("AinsideSpec.dat","w");
    outside=fopen("AoutsideSpec.dat","w");
    if ( inside && outside) {
      Vector3D X;
      int inval, j;
      std::cout << " please wait ..... " << std::endl;
      for(j=0; j<=50000; j++){
	X[3] = 8000.0*(rand()/(RAND_MAX+1.0));
	X[1] = 2000.0 - 12000.0*(rand()/(RAND_MAX+1.0));
	if ((inval=insideSpectrometer(X[1], 0.0, X[3]))) fprintf(inside,"%8.3f %8.3f \n",X[1], 1.0*X[3]);	
	else  fprintf(outside,"%8.3f %8.3f \n",X[1], 1.0*X[3]);

      }
      fclose(inside);fclose(outside);

    } else {
      fprintf(stderr," Couldn't open some boundary check data file!\n");
      return 0;
    }
    return 1;
}
