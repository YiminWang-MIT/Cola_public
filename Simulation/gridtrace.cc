//
//      gridfinder - calculate particle trace in Spek A
//
//      V1.0, Jan C. Bernauer, 06.10.03
//
//      based on qspin:
//      V1.0, T.Pospischil, 20.01.98
//
//      V1.1  T.Pospischil,  2.11.98 (particle system oriented to hdc)
//      V1.2, T.Pospischil,  3.11.98 (qspin10290 (multitrack) integrated)
//      V1.3, T.Pospischil, 10.01.99 (VDC-Offsets eingearbeitet, nun qsdda665)
// 

#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>

#include "qspinSpectrometer.h"
#include "CK_RungeKutta.h"

using namespace std;

qspinSpectrometer *spec = new qspinSpecA();

// some constants:
const double SpeedOfLight = 299.792458; // c in mm/nsec
const double M_p = 938.27231; // MeV
//const double M_p =0.511;
const double g_p = 5.58569478;
//const double g_p =2;

// some nasty global (!!) variables:
double gammaParticle;
double K_p;

// ------------------------------ magmove -------------------------------------
// routine which calculates the derivatives dydx[] of y[] with the ODEs 
// for the numerical Cash Karp Runge-Kutta solution:

void magmove(double t, 
	     double y[],      // INPUT:  variables
	     double dydx[])   // OUTPUT: their derivatives
{
  static Vector3D r;          // position
  static Vector3D v;          // velocity
  static Vector3D s;          // spin
  static Vector3D dr, dv, ds; // time derivates of position, velocity and spin
  static Vector3D B;          // Magnetic Field
  static Vector3D E_v;        // Vector in v-direction with length 1
  static Vector3D B_L, B_T;   // Fields longitudinal and transversal to v

  // Fill position, velocity and spin-Vector:
  r[1] = y[1]; r[2] = y[2]; r[3] = y[3];
  v[1] = y[4]; v[2] = y[5]; v[3] = y[6];
  s[1] = y[7]; s[2] = y[8]; s[3] = y[9]; 

  B[1] = t;         // Dummy, makes warning 't unused' disappear
  // Calculate magnetic field at position r:
  spec->getMagField(B,r); 

  // Split calulated B in a longitudinal and a transversal part to v:
  E_v = v/v.abs();            // Vector in v-direction with length 1
  B_L = (E_v * B) * E_v;
  B_T = B - B_L;

  // Set long. B-Field to zero for test purpose:
  // B_L = Vector3D(0.0,0.0,0.0);
  // B_T = Vector3D(0.0,0.0,0.0);

  // ++++++++++ ODEs: Ordinary Differential Equations to integrate 
  // (K_p = e/(M_p*gamma)):
  dr = v;
  dv = K_p * (v && B);
  ds = K_p * ( s && (g_p/2 * B_L + (1+gammaParticle*(g_p-2)/2) * B_T) ); 

  // Fill derivatives of position, velocity and spin-Vector to dydx:
  dydx[1]=dr[1]; dydx[2]=dr[2]; dydx[3]=dr[3];
  dydx[4]=dv[1]; dydx[5]=dv[2]; dydx[6]=dv[3];
  dydx[7]=ds[1]; dydx[8]=ds[2]; dydx[9]=ds[3];

  return;
}

// ---------------------------- StoreStep ------------------------------------


// function to store contents of y[] and magnetic Field in files:
void StoreStep(double t,   // independent variable
	       double y[], // fields to store
	       int ysize,  // size of y (y[1..ysize] must exist!)
	       int stepN)  // number of the stored step
{

  return;
}


// ============================= QSPIN =================================

int main(int argc, char* argv[])
{
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++ define VDC-system:
  // Durchtrittspkt. des Ref.-Strahls durch vdc x1-Ebene (ACS):
  Vector3D O_D_vdc_x1(-6081.59, 0.0, 4949.3); // unsicher !!
  // Soll-Ablenkwinkel des Referenzstrahls (incl. Randfelder):
  double phi_bend_ref = 100.0238 *deg;

  double tma_x     =  37; // mm   
  double tma_theta = 786.33; // mrad 
  double tma_y     =   -0.56; // mm   
  double tma_phi   =   4.45; // mrad 
  // Orientierung des VDC-Systems:
  double alpha= (phi_bend_ref-90.0*deg+tma_theta/1000.0); // main Ang of VDC
  Vector3D Ez_vdc( -cos(alpha), 0.0, -sin(alpha) );
  Vector3D Ex_vdc( -sin(alpha), 0.0,  cos(alpha) );
  Vector3D Ey_vdc = Ez_vdc && Ex_vdc;
  Matrix3D M_x(Ex_vdc, (tma_phi/1000.0) );  // Phi-Offset der VDC
  Ey_vdc = M_x * Ey_vdc;  Ez_vdc = M_x * Ez_vdc; 
  // Urspungslage des VDC-Systems:
  Vector3D O_vdc = O_D_vdc_x1 -tma_x*Ex_vdc -tma_y*Ey_vdc;

  // ++++++++++++++++++++++++++ Only make data files for boundarys:
  if ( (argc==2) && !(strcmp(argv[1],"-showbound")) ) {
    // --- store vdc-origin in data file:
    FILE *tempfile;
    double AxLen=300.0;//  double tma_phi   =  -1.15; // mrad    

    if (!(tempfile=fopen("dat/vdc_origin.dat","w")))
      fprintf(stderr," Couldn't open dat/vdc_origin.dat for output!\n");
    else {
      fprintf(tempfile,"%10.4f %10.4f %10.4f %10.4f\n",
		  O_vdc[3],-O_vdc[1],(AxLen*Ez_vdc)[3],-(AxLen*Ez_vdc)[1]);
      fprintf(tempfile,"%10.4f %10.4f %10.4f %10.4f\n",
	      O_vdc[3],-O_vdc[1],(AxLen*Ex_vdc)[3],-(AxLen*Ex_vdc)[1]);
      fclose(tempfile);
    }
    return spec->showBoundary();
  }
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++ check boundarys:
  else if ( (argc==2) && !(strcmp(argv[1],"-checkbound")) ) {
    return spec->checkBoundary();
  }

      
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ main functionality:

  // Number of ODEs (dimension of the arrays)
  int odenum = 9;
  // Vector containing start values for y:
  double *ystart = new double[odenum+1];


  double p, dp, th, ph, p_ref, y0,x0;       // Spectrometer-Target-coordinates

  cin>>p_ref;
  while (!cin.eof())
  {

      cin >>dp >>th >>y0 >> ph;    // %
      x0=0;
      Vector3D Spin_tg(0,0,0);
      p = (1.0 + dp/100)*p_ref; // MeV/c

      spec->scaleField(p_ref);

      // calc start position and velocity:
      Vector3D Position(x0, y0, 0); // im mm ! 
      Vector3D Velocity(tan(th/1000.0), tan(ph/1000.0), 1.0); // Richtung
      Velocity = Velocity/Velocity.abs(); // Normierung des Betrags auf 1.0
      gammaParticle = sqrt(p*p+M_p*M_p)/M_p;
      double betaParticle = sqrt(1.0 - 1.0/(gammaParticle*gammaParticle));
      Velocity = Velocity * SpeedOfLight * betaParticle;

      // store Start_Velocity:
      Vector3D Start_Velocity(tan(th/1000.0), tan(ph/1000.0), 1.0); // Richtung
      // Normierung des Betrags auf 1.0
      Start_Velocity = Start_Velocity/Start_Velocity.abs(); 

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
      
      // K_p = e/(M_p*gamma)
      // K_p = M_p/(gammaParticle*10000);           // [K_p] = 1/(Tesla*nsec)
    K_p = 0.09578831/gammaParticle;      

      
      double starttime = 0;
      double endtime = 1000.0; // nsec, speed of light particle goes 1m in 3.3ns
      // this is just security max., normal stop should be made by y[1] < y1min
      
      int nok, nbad, nstored=0;

      double acttime;

      // +++++++++++++++++++++++++++++++++++  integrate ODEs:
      odeint(ystart,    // Start values
	     odenum,    // number of ODEs (= dimension of the arrays)
	     starttime, // start value for the independent variable
	     endtime,   // stop at this value of the independent variable
	     -8800,     // stop if y[1] smaller than y1min (mm)
	     5e-6,      // required accuracy
	     1,         // guessed first stepsize
	     1e-20,     // min. allowe      cout <<" odeint lives"<<endl;d stepsize (can be zero)
	     &nok,      // number of good steps taken
	     &nbad,     // number of bad (retried and fixed) steps taken
	     0.2,       // interval size for step storing (nsec)
	     100000,    // max number of steps to store
	     nstored,   // number of stored steps
	     StoreStep, // function which writes the step to file
	     magmove,   // derivatives function
	     &acttime); // actual value for the independent variable


      // --- Get END position, velocity and spin:
      // out of ystart!!!, unlucky name, hihi, but thats NUM REC people,
      // just kept some of their variable names, so i'm innocent :-)
      Position[1] = ystart[1];
      Position[2] = ystart[2];
      Position[3] = ystart[3];
      Velocity[1] = ystart[4];
      Velocity[2] = ystart[5];
      Velocity[3] = ystart[6];
      betaParticle = Velocity.abs()/SpeedOfLight;

      
      // +++++++ Calculate+Store Position and Velocity in the VDC-Coord.-Sys:
      // --- trace back to VDC x1-plane:
      double t = ( Ez_vdc * (O_vdc - Position) ) / (Ez_vdc * Velocity);
      Vector3D X_v  = Position + t * Velocity; // pos in vdc, ACS
      Vector3D X_vO  = X_v - O_vdc;            // diff vec to vdc origin, ACS 
      // project diff vec to VDC-CS:
      Vector3D X_vdc = Vector3D( Ex_vdc*X_vO, Ey_vdc*X_vO, Ez_vdc*X_vO);
      // project velocity to VDC-CS:
      Vector3D V_vdc =Vector3D(Ex_vdc*Velocity,Ey_vdc*Velocity,Ez_vdc*Velocity);
      
	double pathlength= (acttime + t)*Velocity.abs();
	cout <<setprecision(20);
	cout <<X_vdc[1]<<"\t"<<1000*atan(V_vdc[1]/V_vdc[3])<<"\t"<<X_vdc[2]<<"\t"<< 1000*atan(V_vdc[2]/V_vdc[3]);
	cout <<"\t"<<dp<<"\t"<<th<<"\t"<<y0<<"\t"<<ph<<"\t"<<pathlength;
	if (!cin.eof()) cout <<endl;
}

	return 0;

}
