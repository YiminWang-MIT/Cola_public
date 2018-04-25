//
//      qspin - calculate proton spin precession in Spek A
//
//      V1.0, T.Pospischil, 20.01.98
//
//      V1.1  T.Pospischil,  2.11.98 (particle system oriented to hdc)
//      V1.2, T.Pospischil,  3.11.98 (qspin10290 (multitrack) integrated)
//      V1.3, T.Pospischil, 10.01.99 (VDC-Offsets eingearbeitet, nun qsdda665)
// 
using namespace std;
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "qspinSpectrometer.h"
#include "CK_RungeKutta.h"

#ifdef SPECA
qspinSpectrometer *spec = new qspinSpecA();
#endif
#ifdef OOPS
qspinSpectrometer *spec = new qspinOOPS();
#endif
#ifdef OHIPS
qspinSpectrometer *spec = new qspinOHIPS();
#endif

// some constants:
const double SpeedOfLight = 299.792458; // c in mm/nsec
const double M_p = 938.27231; // MeV
const double g_p = 5.58569478;

// some nasty global (!!) variables:
double gammaParticle;
double K_p;
int multitrack = 0;   // Flag, assume we want to calc only one track

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
FILE   *storefile=NULL;  // filehandle of open file for step storage
FILE   *storefile2=NULL; // filehandle for storage of magnetic field strength 
static Vector3D r_old(0.0,0.0,0.0);
static double act_length = 0.0;

// function to store contents of y[] and magnetic Field in files:
void StoreStep(double t,   // independent variable
	       double y[], // fields to store
	       int ysize,  // size of y (y[1..ysize] must exist!)
	       int stepN)  // number of the stored step
{
  if (multitrack) return;  // do not store steps for all 10290 tracks
  int i;

  // Normal step storage:
  fprintf(storefile,"%6d %8.2f  ", stepN, t);
  for (i=1; i<=ysize; i++) fprintf(storefile," %8.2f", y[i]);
  fprintf(storefile,"\n");
 
  // Store Magnetic field along the path:
  Vector3D r_act(y[1],y[2],y[3]);
  Vector3D B_act;
  spec->getMagField(B_act,r_act); // Calculate magnetic field at position r_act
  act_length += (r_act-r_old).abs(); r_old = r_act;
  fprintf(storefile2,"%10.4f %10.4f %10.4f %10.4f %10.4f \n",
	  act_length, B_act[1], B_act[2], B_act[3], B_act.abs());
  
  return;
}


// ============================= QSPIN =================================

int main(int argc, char* argv[])
{
#ifdef SPECA
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++ define VDC-system:
  // Durchtrittspkt. des Ref.-Strahls durch vdc x1-Ebene (ACS):
  Vector3D O_D_vdc_x1(-6081.59, 0.0, 4949.3); // unsicher !!
  // Soll-Ablenkwinkel des Referenzstrahls (incl. Randfelder):
  double phi_bend_ref = 100.0238 *deg;
  // Tma-Offsets   (qsdda665.mxl):         (qsdda495.mxl2): 
  double tma_x     =   46.7; // mm          =  37.4; // mm   
  double tma_theta =  787.9; // mrad        = 785.0; // mrad 
  double tma_y     =   -5.1; // mm          =   1.2; // mm   
  double tma_phi   =  -1.15; // mrad        =   4.2; // mrad 
  // Orientierung des VDC-Systems:
  double alpha= (phi_bend_ref-90.0*deg+tma_theta/1000.0); // main Ang of VDC
  Vector3D Ez_vdc( -cos(alpha), 0.0, -sin(alpha) );
  Vector3D Ex_vdc( -sin(alpha), 0.0,  cos(alpha) );
  Vector3D Ey_vdc = Ez_vdc && Ex_vdc;
  Matrix3D M_x(Ex_vdc, (tma_phi/1000.0) );  // Phi-Offset der VDC
  Ey_vdc = M_x * Ey_vdc;  Ez_vdc = M_x * Ez_vdc; 
  // Urspungslage des VDC-Systems:
  Vector3D O_vdc = O_D_vdc_x1 -tma_x*Ex_vdc -tma_y*Ey_vdc;
#endif
#ifdef OOPS
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++ define HDC-system:
  // Durchtrittspkt. des Ref.-Strahls durch vdc x1-Ebene (ACS):
  Vector3D O_D_vdc_x1(-808.978, 0.0, 4052.424); // unsicher !!
  // Soll-Ablenkwinkel des Referenzstrahls (incl. Randfelder):
  double phi_bend_ref = 21.7232*deg;
  // Tma-Offsets
  double tma_x     =    0.0; // mm
  double tma_theta =    0.0; // mrad
  double tma_y     =    0.0; // mm
  double tma_phi   =    0.0; // mrad
  // Orientierung des HDC-Systems:
  double alpha= (phi_bend_ref-90.0*deg+tma_theta/1000.0); // main Ang of HDC
  Vector3D Ez_vdc( -cos(alpha), 0.0, -sin(alpha) );
  Vector3D Ex_vdc( -sin(alpha), 0.0,  cos(alpha) );
  Vector3D Ey_vdc = Ez_vdc && Ex_vdc;
  Matrix3D M_x(Ex_vdc, (tma_phi/1000.0) );  // Phi-Offset der VDC
  Ey_vdc = M_x * Ey_vdc;  Ez_vdc = M_x * Ez_vdc; 
  // Urspungslage des VDC-Systems:
  Vector3D O_vdc = O_D_vdc_x1 -tma_x*Ex_vdc -tma_y*Ey_vdc;
#endif
#ifdef OHIPS
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++ define VDC-system:
  // Durchtrittspkt. des Ref.-Strahls durch vdc x1-Ebene (ACS):
  Vector3D O_D_vdc_x1(-4166.1, 0.0, 6666.2);
  double alpha= (45.0*deg); // main Ang of VDC
  // Durchtrittspunkt des Referenzstrahls durch Szintillator 2
  // Vector3D O_D_vdc_x1(-5100.0, 0.0, 6669.56);
  // double alpha= (0.0); // main Ang of VDC

  // Soll-Ablenkwinkel des Referenzstrahls (incl. Randfelder):
  // double phi_bend_ref = 90.0*deg;
  // Tma-Offsets
  double tma_x     =    0.0; // mm
  double tma_theta =    0.0; // mrad
  double tma_y     =    0.0; // mm
  double tma_phi   =    0.0; // mrad
  // Orientierung des VDC-Systems:
  Vector3D Ez_vdc( -cos(alpha), 0.0, -sin(alpha) );
  Vector3D Ex_vdc( -sin(alpha), 0.0,  cos(alpha) );
  Vector3D Ey_vdc = Ez_vdc && Ex_vdc;
  Matrix3D M_x(Ex_vdc, (tma_phi/1000.0) );  // Phi-Offset der VDC
  Ey_vdc = M_x * Ey_vdc;  Ez_vdc = M_x * Ez_vdc; 
  // Urspungslage des VDC-Systems:
  Vector3D O_vdc = O_D_vdc_x1 -tma_x*Ex_vdc -tma_y*Ey_vdc;
#endif

  // ++++++++++++++++++++++++++ Only make data files for boundarys:
  if ( (argc==2) && !(strcmp(argv[1],"-showbound")) ) {
#ifdef SPECA
    // --- store vdc-origin in data file:
    FILE *tempfile;
    double AxLen=300.0;
    if (!(tempfile=fopen("dat/vdc_origin.dat","w")))
      fprintf(stderr," Couldn't open dat/vdc_origin.dat for output!\n");
    else {
      fprintf(tempfile,"%10.4f %10.4f %10.4f %10.4f\n",
		  O_vdc[3],-O_vdc[1],(AxLen*Ez_vdc)[3],-(AxLen*Ez_vdc)[1]);
      fprintf(tempfile,"%10.4f %10.4f %10.4f %10.4f\n",
	      O_vdc[3],-O_vdc[1],(AxLen*Ex_vdc)[3],-(AxLen*Ex_vdc)[1]);
      fclose(tempfile);
    }
#endif
#ifdef OHIPS
    // --- store vdc-origin in data file:
    FILE *tempfile;
    double AxLen=300.0;
    if (!(tempfile=fopen("dat/vdc_origin.dat","w")))
      fprintf(stderr," Couldn't open dat/vdc_origin.dat for output!\n");
    else {
      fprintf(tempfile,"%10.4f %10.4f %10.4f %10.4f\n",
		  O_vdc[3],-O_vdc[1],(AxLen*Ez_vdc)[3],-(AxLen*Ez_vdc)[1]);
      fprintf(tempfile,"%10.4f %10.4f %10.4f %10.4f\n",
	      O_vdc[3],-O_vdc[1],(AxLen*Ex_vdc)[3],-(AxLen*Ex_vdc)[1]);
      fclose(tempfile);
    }
#endif
    return spec->showBoundary();
  }
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++ check boundarys:
  else if ( (argc==2) && !(strcmp(argv[1],"-checkbound")) ) {
    return spec->checkBoundary();
  }
  else if ( (argc==5) && !(strcmp(argv[1],"-10290")) ){
    multitrack = 1;
  }

  // User info:
  else if (argc<9){
    fprintf(stderr,"\n qspin - calculate proton spin precession in Spek A\n\n");
    fprintf(stderr,"         V1.3, T.Pospischil, 10.2.99\n\n");
    fprintf(stderr,"\n usage: qspin <dp/%c> <th/mrad> <y0/mm> <ph/mrad> <p_ref/MeV/c>\n",37);
    fprintf(stderr,"              <Sx_tg> <Sy_tg> <Sz_tg>\n\n");
    fprintf(stderr,"   or   qspin -showbound\n");
    fprintf(stderr,"   or   qspin -checkbound\n\n");
    fprintf(stderr,"   or   qspin -10290 <Sx_tg> <Sy_tg> <Sz_tg>\n\n");
    return 1;
  }
  
  // ++++++++++++++++++++++++++++++++++++++++++++++++++++++ main functionality:

  // Number of ODEs (dimension of the arrays)
  int odenum = 9;
  // Vector containing start values for y:
  double *ystart = new double[odenum+1];

  FILE *trackdatei=NULL;
  FILE *docdatei=NULL;

  if (multitrack){
    // open file to store the results of the 10290 calculated tracks:
    if (!(trackdatei=fopen("QS10290","w"))){
      fprintf(stderr," Couldn't open QS10290 for output!\n");
      return 1;
    }
  }
  else {
    // open file to store positions, velocity and spin stepwise:
    char filename[128];
    strcpy(filename, spec->getBasename()); strcat(filename, ".dat");
    if (!(storefile = fopen(filename, "w"))) {
      fprintf(stderr, " Cannot open file %s for output!\n", filename);
      return 2;
    }
    // open file to store magnetic fields stepwise:
    strcpy(filename, spec->getBasename()); strcat(filename, "_B.dat");
    if (!(storefile2 = fopen(filename, "w"))) {
      fprintf(stderr, " Cannot open file %s for output!\n", filename);
      return 2;
    }
    // open file to store start values and result in the psplot-text format:
    strcpy(filename, spec->getBasename()); strcat(filename, ".dps");
    if (!(docdatei=fopen(filename, "w"))){
      fprintf(stderr, " Couldn't open %s for output!\n", filename);
      return 1;
    }
  }

  int TrackNum=0;
  double dp_val[5] = {-5.7794, 0.0327, 5.6206, 11.0182, 16.2521};
  double p, dp, th, ph, p_ref, y0;       // Spectrometer-Target-coordinates
  double cmd_Sx, cmd_Sy, cmd_Sz;         // Spin comp. read from command line 
  int dp_c; // dp-counter

  // Loop for multitrack (is left after first track for single track calc):
  for (dp_c = 0; dp_c < 5; dp_c++){
    dp = dp_val[dp_c];
    for (y0 = -30.0; y0 <= 30.0; y0+=10.0)
    for (ph = -105.0; ph <= 105.0; ph+=35.0)
    for (p_ref = 480.0; p_ref <= 630.0; p_ref+=30.0)
    for (th = -75.0; th <= 75.0; th+=25.0){

      if (!(multitrack)){
	// particle coordinates in Spectrometer-Target-System from command line:
        dp = atof(argv[1]);    // %
	th = atof(argv[2]);    // th_tg in mrad
	ph = atof(argv[4]);    // ph_tg in mrad
	y0 = atof(argv[3]);    // y0_tg in mm
	p_ref = atof(argv[5]); // MeV/c
	// multitrack: start spin at the target in % from command line::
	cmd_Sx = atof(argv[6]);
	cmd_Sy = atof(argv[7]);
	cmd_Sz = atof(argv[8]);
      }
      else {
	// single track: start spin at the target in % from command line::
	cmd_Sx = atof(argv[2]);
	cmd_Sy = atof(argv[3]);
	cmd_Sz = atof(argv[4]);
      }

      Vector3D Spin_tg(cmd_Sx,cmd_Sy,cmd_Sz);
      p = (1.0 + dp/100)*p_ref; // MeV/c

      spec->scaleField(p_ref);

      // calc start position and velocity:
      Vector3D Position(0, y0, 0); // im mm ! 
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
      K_p = 0.09578831/gammaParticle;           // [K_p] = 1/(Tesla*nsec)
      
      double starttime = 0;
      double endtime = 1000.0; // nsec, speed of light particle goes 1m in 3.3ns
      // this is just security max., normal stop should be made by y[1] < y1min
      
      int nok, nbad, nstored=0;

      double acttime;

      if (!(multitrack)){
	// user info before start:
	cout << "---------------------------------";
	cout << "---------------------------------" << endl;
	cout << " Reference Momentum   : " << p_ref << " MeV/c" << endl;
	cout << " gamma of the particle: " << gammaParticle << endl;
	cout << " Start position   : " << Position << endl;
	cout << " Start velocity   : " << Velocity;
	cout << ", beta = "<< betaParticle <<endl;
	cout << " Start spin (ACS) : " << Spin_tg  << endl;
	cout << " -----------------------------";
	cout << " calculating, please wait .........." << endl;
      }
      
      // +++++++++++++++++++++++++++++++++++  integrate ODEs:
      odeint(ystart,    // Start values
	     odenum,    // number of ODEs (= dimension of the arrays)
	     starttime, // start value for the independent variable
	     endtime,   // stop at this value of the independent variable
#ifdef SPECA
	     -8800,     // stop if y[1] smaller than y1min (mm)
#endif
#ifdef OOPS
	     -1200,     // stop if y[1] smaller than y1min (mm)
#endif
#ifdef OHIPS
	     -6500,     // stop if y[1] smaller than y1min (mm)
#endif
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
      Vector3D Spin_end(ystart[7],ystart[8],ystart[9]);
      
      // Calculate End Spin in particle system (rot to HDC-System)
      double winkel_hdc=10.0*deg;                        // main Angle of HDC
      Vector3D Ez_hdc( -cos(winkel_hdc), 0.0, -sin(winkel_hdc) );
      Vector3D Ex_hdc( -sin(winkel_hdc), 0.0,  cos(winkel_hdc) );
      Vector3D Ey_hdc = Ez_hdc && Ex_hdc;
      Vector3D DAxis_to_HDC;
      double Dangle_to_HDC;
      Vector3D SEAR_H = rotVec(Spin_end, Velocity, Ez_hdc, 
			       DAxis_to_HDC, Dangle_to_HDC);
      Vector3D Spin_end_particle(SEAR_H*Ex_hdc, SEAR_H*Ey_hdc, SEAR_H*Ez_hdc);

      // +++++++ Calculate+Store Position and Velocity in the VDC-Coord.-Sys:
      // --- trace back to VDC x1-plane:
      double t = ( Ez_vdc * (O_vdc - Position) ) / (Ez_vdc * Velocity);
      Vector3D X_v  = Position + t * Velocity; // pos in vdc, ACS
      Vector3D X_vO  = X_v - O_vdc;            // diff vec to vdc origin, ACS 
      // project diff vec to VDC-CS:
      Vector3D X_vdc = Vector3D( Ex_vdc*X_vO, Ey_vdc*X_vO, Ez_vdc*X_vO);
      // project velocity to VDC-CS:
      Vector3D V_vdc =Vector3D(Ex_vdc*Velocity,Ey_vdc*Velocity,Ez_vdc*Velocity);
      
      if (!(multitrack)){
	// user info (results):
	cout << " Number of good steps   : " << nok  << endl;
	cout << " Number of bad steps    : " << nbad << endl;
	cout << " Number of stored steps : " << nstored << endl;
	cout << " End position   : " << Position << endl;
	cout << " End velocity   : " << Velocity;
	cout << ", beta = "<< betaParticle <<endl;
	cout << " End spin (ACS) : " << Spin_end << endl;
	cout << " End spin (hdc particle system) : ";
	cout << Spin_end_particle << endl;

	// --- tell user vdc-coords.:
	cout << " VDC-Coordinates (ACS): " << X_v << endl;
	cout << " VDC-Coordinates (x/mm, th/mrad, y/mm, ph/mrad): ";
	fprintf(stdout,"( %.1f %.1f %.1f %.1f )\n", 
		X_vdc[1], 1000*atan(V_vdc[1]/V_vdc[3]),
		X_vdc[2], 1000*atan(V_vdc[2]/V_vdc[3])  );
	// 	cout << " VDC-Coordinates (x/mm, th/mrad, y/mm, ph/mrad): ";
	// 	cout << X_vdc[1];
	// 	cout << " " << 1000*atan(V_vdc[1]/V_vdc[3]);
	// 	cout << " " << X_vdc[2];
	// 	cout << " " << 1000*atan(V_vdc[2]/V_vdc[3]) << endl;
	double pathlength= (acttime + t)*Velocity.abs();
	cout << " Flighttime to VDC: "  << (acttime+t);
	cout << " nsec, Pathlength to VDC: " << pathlength << " mm" << endl;
	double phi_bend = 180/3.14159265*
	  acos(Start_Velocity*Velocity/(Velocity.abs()*Start_Velocity.abs()));
	cout << " Dipolfield approx: ph_bend = " << phi_bend;
	cout << " deg, chi = " << gammaParticle*(g_p/2-1)*phi_bend;
	cout << " deg " << endl;
	cout << "---------------------------------";
	cout << "---------------------------------" << endl;

	// Write file for data2ps track docu:
	fprintf(docdatei,"START COORDS:\n{sQ}{_0}{^tg} = %.2f mrad\n", th);
	fprintf(docdatei,"{sF}{_0}{^tg} = %.2f mrad\n", ph);
	fprintf(docdatei,"y{_0}{^tg} = %.2f mm\n", y0);
	fprintf(docdatei,"{sD}p = %.2f %c\n", dp, 37); 
	fprintf(docdatei,"p{_ref} = %.2f MeV/c\n", p_ref); 
	fprintf(docdatei,"p = %.2f MeV/c\n", p); 
	fprintf(docdatei,"START SPIN:\n \\(S{_x}, S{_y}, S{_z}\\){_ACS}\n");
	fprintf(docdatei,"( %6.2f, %6.2f, %6.2f )\n",
		Spin_tg[1],Spin_tg[2],Spin_tg[3]);
	fprintf(docdatei,"\nEND SPIN:\n");
	fprintf(docdatei,"\\(S{_x}, S{_y}, S{_z}\\){_particle}\n");
	fprintf(docdatei,"( %6.2f, %6.2f, %6.2f )\n",
		Spin_end_particle[1],Spin_end_particle[2],Spin_end_particle[3]);

	// close some files:
	fclose(docdatei);
	fclose(storefile);
	fclose(storefile2);
	return 0;
      }
      else {
	cerr << ++TrackNum << endl;
	fprintf(trackdatei,
		"%5d %6.1f %6.1f %6.1f %9.4f %7.1f %7.3f",
		TrackNum, th, ph, y0, dp, p_ref, p);
	fprintf(trackdatei,
		" %10.4f %10.4f %10.4f", 
		Spin_tg[1], Spin_tg[2], Spin_tg[3]);
	fprintf(trackdatei,
		" %10.4f %10.4f %10.4f %10.4f", 
		X_vdc[1], 1000*atan(V_vdc[1]/V_vdc[3]),
		X_vdc[2], 1000*atan(V_vdc[2]/V_vdc[3])  );
	fprintf(trackdatei,
		" %10.4f %10.4f %10.4f\n", 
		Spin_end_particle[1],Spin_end_particle[2],Spin_end_particle[3]);
      }
    } // for 10290 ...
  }
  
  // close some files:
  fclose(trackdatei);
  return 0;
}
