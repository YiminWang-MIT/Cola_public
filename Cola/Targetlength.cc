//
// Concept 1995 by	Michael O. Distler, 
//                      and Harald Merkel
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz 
//
// written 1997 by      Ingo Ewald    16.05.97
// last edit                          04.11.98
//

#include "QuasiRandom.h"
#include "Targetlength.h"
#include "rundatabase.h"
#include "Radiation.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include<limits>
using namespace std;

#define sign(x)  (x>0 ? 1 : (x<0 ? -1 : 0))
//
// All definitions in mm !!!!!!
//

// cylindric cryo 2014 (with new (better) functions)

double cryo_cyl_2014::diameter      =       0.0;                // see below !  
double cryo_cyl_2014::totalheight   = 25.000000;
double cryo_cyl_2014::Wall          =       0.0;
double cryo_cyl_2014::SnowWall      =       0.0;                // see below !

// old cylindric LH2-Targetcell

double cryo_cyl::diameter      =       0.0;                // see below !  
double cryo_cyl::totalheight   = 25.000000;
double cryo_cyl::Wall          =       0.0;
double cryo_cyl::SnowWall      =       0.0;                // see below !

// new flat LH2-Targetcell

double cryo_ewald::totallength =       0.0;                // see below !
double cryo_ewald::totalwidth  = 11.500000; 
double cryo_ewald::totalheight =  9.000000; // now 12mm but irrelevant to 
                                            // pres. analysis (kohlm,25/04/02)
double cryo_ewald::Wall        =       0.0;                // see below !
double cryo_ewald::SnowWall    =       0.0;                // see below !

// new flat LH2-Empty-Targetcell

double cryo_ewald_empty::totallength = 49.500000; 
double cryo_ewald_empty::totalwidth  = 11.500000; 
double cryo_ewald_empty::totalheight =  9.000000; 
double cryo_ewald_empty::Wall        =  0.010160; 


// cryo cell for isr_2013 beam time

double cryo_ewald_2014::totallength =  49.500000; 
double cryo_ewald_2014::totalwidth  =  11.500000; 
double cryo_ewald_2014::totalheight =  9.000000; 
double cryo_ewald_2014::Wall        =  0.01016;
double cryo_ewald_2014::SnowWall    =  0.0; 
double cryo_ewald_2014::AramidWindowThickness = 0.006;
double cryo_ewald_2014::icefactor   = 1.0; 

// Water-Calibration-Target

double calib_water::totallength     =  0.480000; 
double calib_water::totalwidth      = 10.000000; 
double calib_water::totalheight     = 10.000000; 
double calib_water::Wall            =  0.010000; 
double calib_water::angle_tar       =    0;                // see below !

// Helium-Targetcell

double helium::diameter             =    0;                // see below !
double helium::totalheight          =   55.000000;
double helium::Wall                 =    0;                // see below !
double helium::SnowWall             =  0.000000;           // see below !

// New MIT-Helium-Targetcell

double helium_new::diameter         =   80.000000;      // 80 oder 40 mm ?       
double helium_new::totalheight      =   55.000000;
double helium_new::Wall             =   0;              //init:0 see below !
double helium_new::SnowWall         =   0.000000;           // see below !
const double pi = 3.1415926;

// polarized He3-Targetcell

double helium_pol::diameter         = 90.0; // in mm
double helium_pol::Wall             =  2.0; // in mm
double helium_pol::BeThickness      = 0.02; // in mm
double helium_pol::CuThickness      = 0.02; // in mm

// polarized He3-Targetcell used 2007

double he_pol_07::diameter         = 96.0;  // in mm
double he_pol_07::Wall             = 2.0;   // in mm
double he_pol_07::BeThickness      = 0.05;  // in mm
double he_pol_07::AlThickness      = 0.0004;// in mm

// Solidstate-Target

double solidstate::totallength      =    0;         // see below !
double solidstate::totalwidth       =   20.000000;
double solidstate::totalheight      =   10.000000;
double solidstate::angle_tar        =    0;         // see below !

// Windowless Tube Target

double windowlesstube::totallength      =    0;         // see below !
double windowlesstube::totalwidth       =   7.500000; //cell radius [mm]
double windowlesstube::totalheight      =   10.000000;

// Gas jet Target
// Nothing here, gas jet target parameters are not static
// so that they can be modified during run time.


// Waterfall-Target

double H2O::totallength      =    0;                // see below !
double H2O::totalwidth       =   20.000000;
double H2O::totalheight      =   10.000000;
double H2O::angle_tar        =    0;                // see below !
double H2O::Wall             =    0.010;
double H2O::diameter         =  110.;

// Properties of Scattering Chamber Windows 

double Scat_Exit_Window_thickness         =   0.12;            // 120 um
double Spec_Entrance_Window_thickness     =   0.12;            // 120 um
double Air_between_Scat_Spec_thickness    =  140;//60.0;             // A:  6 cm
                                                               // B: 14 cm
double Kaos_Entrance_Window_thickness     =   0.356;           // 356 um
double Air_between_Scat_Kaos_thickness    =  600.;             // KAOS: 60 cm

double Spec_Exit_Window_thickness         =   0.077;           //  77 um
double Air_between_Spec_Det_thickness     = 150.0;
double Wirechamber_Gas_Foil_thickness     =   0.0125;          //   12.5 um
double Wirechamber_HV_Foil_thickness      =   0.005;           //   5 um
double Isobutane_thickness                =  36.0;
double Air_between_VDC_thickness          = 200.0;

double Air_between_VDC_Scint_thickness    = 100.0;

double ScintWrapping_thickness            =   0.100;           // 100 um
double dE_thickness                       =   3.0;
double Air_between_dE_ToF                 =  10.0;
double ToF_thickness                      =  10.0;




int 
cryo_cyl::setPara(double len, double, double density, double WallThick, 
		  double SnowThick, double SnowDensity)
{
  TargetMat->setDensity(density);
  if (len==0) diameter=20.0; else diameter=len;
  if (Wall==0) Wall=0.0075; else Wall=WallThick;

  Length = diameter;
  WallThickness=Wall;

  if (SnowThick < 0) SnowThick = 0;

  SnowWall = SnowThick;            // Thickness in mm
  SnowThickness = SnowWall;

  if (SnowDensity != 0) Snow->setDensity(SnowDensity);   // Density in g/cm^3

  return 1;
}

int 
cryo_cyl::Generate_Vertex(double random[], double x[], double /*wob_x*/, double /*wob_y*/, modeltype ModelType) 

{
    x[0] = rundb.sim.wobx * cos(random[0] * M_PI) + rundb.beam.offset.x - rundb.Target.offset_sim.x;
    x[1] = rundb.sim.woby * cos(random[1] * M_PI) + rundb.beam.offset.y - rundb.Target.offset_sim.y;
    x[2] = getLength() * (random[2] - .5);
  double r = diameter / 2;
  
  if ((x[0]*x[0] + x[2]*x[2]) >= r*r) return 0;  // not inside targetcell
  return 1;
}



//double cryo_cyl_2014::getLength_in_Target(double x, double, double z, double theta, double phi)
double cryo_cyl::getLength_in_Target(double x, double, double z, double theta, double phi)
{
  double result, result2;
  double r = diameter / 2.;
  double dx[3] = {sin(theta) * cos(phi),          
		  sin(theta) * sin(phi),
		  cos(theta)};
  
  if (fabs(dx[1]) == 1.) return -1;               // straight up or down
  if (fabs(x) > r) return -1;                     // not inside targetcell

  if ((x*x + z*z) >= r*r) z = sign(z)*(sqrt(r*r-x*x) - 0.05); // vertex z=edge
  
  double p   = 2 *(x * dx[0] + z * dx[2]);
  double q   = x*x + z*z - r*r;
  double p2q = p*p/4-q;
  
  double check1 = ((p2q < 0) ? 0 : - p/2 + sqrt(p2q));
  double check2 = ((p2q < 0) ? 0 : - p/2 - sqrt(p2q));
  
  double z1 = z + check1 * dx[2];
  double z2 = z + check2 * dx[2];
  
  // Wall !!!

  r += Wall;
  q = x*x + z*z - r*r;
  p2q = p*p/4-q;

  if (dx[2]>=0) {
    result  = (z1>z2 ? check1 : check2);
    result2 = (z1>z2 ? ((p2q < 0) ? 0 : - p/2 + sqrt(p2q)):
               	       ((p2q < 0) ? 0 : - p/2 - sqrt(p2q)));
    Length_in_Wall = result2 - result;
  }
  else {
    result  = (z1<z2 ? check1 : check2);
    result2 = (z1<z2 ? ((p2q < 0) ? 0 : - p/2 + sqrt(p2q)):
	               ((p2q < 0) ? 0 : - p/2 - sqrt(p2q)));
    Length_in_Wall = result2 - result;
  }

  // Snow !!!

  r += SnowWall;
  q = x*x + z*z - r*r;
  p2q = p*p/4-q;

  check1 = ((p2q < 0) ? 0 : - p/2 + sqrt(p2q));
  check2 = ((p2q < 0) ? 0 : - p/2 - sqrt(p2q));
  
  z1 = z + check1 * dx[2];
  z2 = z + check2 * dx[2];

  if (dx[2]>=0) {
    result2  = (z1>z2 ? check1 : check2);
    Length_through_Snow = result2 - result - Length_in_Wall;
  }
  else {
    result2  = (z1<z2 ? check1 : check2);
    Length_through_Snow = result2 - result - Length_in_Wall;
  }
  
  return result;
}

//######### cryo 2014 ############
int 
cryo_cyl_2014::setPara(double len, double, double density, double WallThick, 
		  double SnowThick, double SnowDensity)
{

  TargetMat->setDensity(density);
  if (len==0) diameter=20.0; else diameter=len; //diameter is without walls-> only the inside where the deuterium is
  if (Wall==0) Wall=0.0100; else Wall=WallThick;
  //std:cout<<density<<std::endl;
  //if (SnowWall==0) SnowWall=.1; else SnowWall=SnowThick;
  
  Length = diameter;
  WallThickness=Wall;

  //if (SnowThick < 0) SnowThick = 0;
  if (SnowThick < 0) SnowThickness = 0;

   // Thickness in mm
  SnowThickness = SnowWall;

  if (SnowDensity != 0) Snow->setDensity(SnowDensity);   // Density in g/cm^3

  return 1;

}


int 
cryo_cyl_2014::Generate_Vertex(double random[], double x[], double , double , modeltype ModelType) 
{

 
  double r_i = diameter / 2; // inner radius
  double r_o = r_i + WallThickness; // outer radius

  // ratio of the wall length and the total target length
  // TO DO: this variable should be used as a weight in Simul.cc
  //double lengthRatio = 1; 
   
  double randx = random[0];
  double randy = random[1];
  double randz = random[2];

  //get randomized vertex coordinates:

  x[0] = rundb.sim.wobx * cos(randx * M_PI) + rundb.beam.offset.x - rundb.Target.offset_sim.x;
  x[1] = rundb.sim.woby * cos(randy * M_PI) + rundb.beam.offset.y - rundb.Target.offset_sim.y;
  //x[2] = (getLength()+Wall+SnowWall) * (randz - .5); //getLength ist immer 20 mm
  x[2] = getLength() * (randz - .5); //getLength ist immer 20 mm
  //cout << "getLength, Wall, SnowWall: " << getLength() << "," << Wall << "," << SnowWall << endl;

  if(ModelType == ElasticDeuteron || ModelType == ElasticProton){

      if(x[0]*x[0] + x[2]*x[2] >= r_i*r_i){
        return 0;
      }
      else{
        return 1;
      }

  }


  if(ModelType == ElasticNucleus || ModelType == QuasiElasticNucleus|| ModelType == ElasticHavar){
      if(abs(x[0]) < r_i){// intersections with the inside  (i) and outside (o) of the foil
	  double z_i1 = -sqrt(r_i*r_i-x[0]*x[0]);
          double z_i2 =  sqrt(r_i*r_i-x[0]*x[0]);
          double z_o1 = -sqrt(r_o*r_o-x[0]*x[0]);
          double z_o2 =  sqrt(r_o*r_o-x[0]*x[0]);
          //lengthRatio = (z_i1-z_o1 + z_o2-z_i2) / (getLength()+Wall+SnowWall);

	  // shift vertex into the target wall
	  if(randz >= 0.5){
	      x[2] = (2*randz-1)*(z_o2-z_i2) + z_i2;
	  }
	  else{
	      x[2] = 2*randz*(z_i1-z_o1) + z_o1;
	  }
	  return 1;

      }

      else if(abs(x[0]) < r_o){
          // electron runs only through the foil => only two intersetions
          double z_1 = -sqrt(r_o*r_o-x[0]*x[0]);
          double z_2 =  sqrt(r_o*r_o-x[0]*x[0]);
          //lengthRatio = (z_2-z_1) / (getLength()+Wall+SnowWall);
          x[2] = randz*(z_2-z_1);
          return 1;
      }
      else{

	return 0;
      }
      }
}
//####################################################################################
//Attention: cryo_cyl_2014::getLength_in_Target is an overloaded function, since the modeltype is not needed everywhere where this function is called and modeltype is also not accessible everywhere
//###################################################################################
/*double cryo_cyl_2014::getLength_in_Target(double x, double, double z, double theta, double phi, modeltype ModelType)
{
  
  double result, result2;
  //x = 0;
  //z = 0;
  //theta = M_PI/4;
  //phi = 0;
  double r = diameter / 2.;
  double dx[3] = {sin(theta) * cos(phi),          
		  sin(theta) * sin(phi),
		  cos(theta)};
  

  if (fabs(dx[1]) == 1.) return -1;               // straight up or down
  if (fabs(x) > r && ModelType == ElasticDeuteron) return -1;                     // not inside targetcell

  if ((x*x + z*z) >= r*r && ModelType == ElasticDeuteron) z = sign(z)*(sqrt(r*r-x*x) - 0.05); // vertex z=edge for deuterium
  
  //Calculating distance from vertex to the point where the electron leaves the cell, using a triangle in 3D with corner: middle of the cell, vertex and point where the electron leaves the cell. Using the vectors: r = radius of the cell, a = (x,y,z) vector from middle of the cell to vertex and vector from vertex to point where electron leaves the cell. Via a quadratic equation you get the length of this last vector.

  double p   = 2 *(x * dx[0] + z * dx[2]);// variable p as usually seen in solution for quadratic equations (pq formular)
  double q   = x*x + z*z - r*r;// variable q as usually seen in solution for quadratic equations (pq formular)
  double p2q = p*p/4-q; //part of pq formular which is under the sqrt
  
  double check1 = ((p2q < 0) ? 0 : - p/2 + sqrt(p2q));//if p2q is < 0 the result of the quadratic formular would be imaginary -> only use real results
  double check2 = ((p2q < 0) ? 0 : - p/2 - sqrt(p2q));//second possible solution of quadratic formular (different sign)
  
  double z1 = z + check1 * dx[2];//only difference between z1 and z2 is the sign in front of the sqrt
  double z2 = z + check2 * dx[2];
  
  // Wall !!!
  
  r += Wall; //add thickness of wall to the inner radius to get outer radius
  q = x*x + z*z - r*r; // variables with outer radius
  p2q = p*p/4-q; // variables with outer radius
  
  if (dx[2]>=0) {//forward angles
    result  = (z1>z2 ? check1 : check2);//take check1 or to depending on which z is bigger, to get positive distance (for inner radius)
    result2 = (z1>z2 ? ((p2q < 0) ? 0 : - p/2 + sqrt(p2q)): //same for outer radius
               	       ((p2q < 0) ? 0 : - p/2 - sqrt(p2q)));
  
    //Definition: Backwall = Wall after vertex
    Distance_to_EndOfCell = result2;//Distance from vertex to End of cell (outer)
    Length_in_Backwall = result2-result;//Distance electron went through inside the back wall
    Distance_to_BackWall = result;//Distance from vertex to backwall (inner)
    
  }
  else {//backward angles
    result  = (z1<z2 ? check1 : check2);
    result2 = (z1<z2 ? ((p2q < 0) ? 0 : - p/2 + sqrt(p2q)):
	               ((p2q < 0) ? 0 : - p/2 - sqrt(p2q)));
    Distance_to_EndOfCell = result2;//Distance from vertex to End of cell (outer)
    Length_in_Backwall = result2-result;//Distance electron went through inside the back wall
    Distance_to_BackWall = result;//Distance from vertex to backwall (inner)
    
  }
  //cout<< x << "\t" << z << "\t" << phi << "\t" << theta << "\t" << result<< endl;
  
  
  
  // BackSnow !!!
  
     r += SnowWall;
  q = x*x + z*z - r*r;
  p2q = p*p/4-q;

  check1 = ((p2q < 0) ? 0 : - p/2 + sqrt(p2q));
  check2 = ((p2q < 0) ? 0 : - p/2 - sqrt(p2q));
  
  z1 = z + check1 * dx[2];
  z2 = z + check2 * dx[2];

  if (dx[2]>=0) {
    result2  = (z1>z2 ? check1 : check2);
    Length_through_Snow = result2 - result - Length_in_Backwall; //length through snow comes from here, why is there no method here for getLength_through_Snow like in Targetlength.h?
  }
  else {
    result2  = (z1<z2 ? check1 : check2);
    Length_through_Snow = result2 - result - Length_in_Backwall;
    }
  
//Distances for incoming electron (use same calculations, but imagine electron being scatterd by 180deg in the same plane it came inside the cell:
  double theta_in = M_PI;
  double phi_in = 0;
  double dx_in[3] = {sin(theta_in) * cos(phi_in),          
	   sin(theta_in) * sin(phi_in),
	   cos(theta_in)};
  //x=5;
  //z=0;
  //get all the variables back to the inner radius and the new angles
  r = diameter / 2.;
  p   = 2 *(x * dx_in[0] + z * dx_in[2]);
  q   = x*x + z*z - r*r;
  p2q = p*p/4-q;
  check1 = ((p2q < 0) ? 0 : - p/2 + sqrt(p2q));
  check2 = ((p2q < 0) ? 0 : - p/2 - sqrt(p2q));
  z1 = z + check1 * dx_in[2];
  z2 = z + check2 * dx_in[2];

  //Wall like before:
  r += Wall;
  q = x*x + z*z - r*r;
  p2q = p*p/4-q;

  if (dx_in[2]>=0) {//forward angles
    result  = (z1>z2 ? check1 : check2);//take check1 or to depending on which z is bigger, to get positive distance (for inner radius)
    result2 = (z1>z2 ? ((p2q < 0) ? 0 : - p/2 + sqrt(p2q)): //same for outer radius
               	       ((p2q < 0) ? 0 : - p/2 - sqrt(p2q)));

    Distance_to_BeginningOfCell = result2;//Distance from vertex to Beginning of cell (outer)
    Length_in_Frontwall = result2-result;//Distance electron went through inside the front wall
    Distance_to_FrontWall = result;//Distance from vertex to frontwall (inner)
  }
  else {//backward angles
    result  = (z1<z2 ? check1 : check2);
    result2 = (z1<z2 ? ((p2q < 0) ? 0 : - p/2 + sqrt(p2q)):
	               ((p2q < 0) ? 0 : - p/2 - sqrt(p2q)));
    Distance_to_BeginningOfCell = result2;//Distance from vertex to Beginning of cell (outer)
    Length_in_Frontwall = result2-result;//Distance electron went through inside the front wall
    Distance_to_FrontWall = result;//Distance from vertex to frontwall (inner)
    
    }

  
  //cout<< x << "\t" << z << "\t" << phi << "\t" << theta << "\t" << Distance_to_BackWall << "\t" << Distance_to_BackWall<< "\t" <<Distance_to_EndOfCell << "\t" << Distance_to_BeginningOfCell << "\t" << Distance_to_FrontWall << "\t" << Distacne_toBackWall << endl;

  return Distance_to_BackWall;
  }*/

double cryo_cyl_2014::getLength_in_Target(double x, double y, double z, double theta, double phi, modeltype ModelType)
{
  double r = diameter / 2.;
  if ((x*x + z*z) >= r*r && ModelType == ElasticDeuteron) z = sign(z)*(sqrt(r*r-x*x) - 0.05); // vertex z=edge for deuterium
  return getLength_in_Target_MAINSTUFF(x, y, z, theta, phi);
}

double cryo_cyl_2014::getLength_in_Target(double x, double y, double z, double theta, double phi)
{
  double r = diameter / 2.;
  double dx[3] = {sin(theta) * cos(phi),          
		  sin(theta) * sin(phi),
		  cos(theta)};

  if (fabs(dx[1]) == 1.) return -1;               // straight up or down
  if (fabs(x) > r) return -1;                     // not inside targetcell

  if ((x*x + z*z) >= r*r) z = sign(z)*(sqrt(r*r-x*x) - 0.05); // vertex z=edge
  return getLength_in_Target_MAINSTUFF(x, y, z, theta, phi);
}

double cryo_cyl_2014::getLength_in_Target_MAINSTUFF(double x, double y, double z, double theta, double phi)
{
 
  double result, result2;
  double r = diameter / 2.;
  double dx[3] = {sin(theta) * cos(phi),          
		  sin(theta) * sin(phi),
		  cos(theta)};
  
  //if (fabs(dx[1]) == 1.) return -1;               // straight up or down
  //if (fabs(x) > r) return -1;                     // not inside targetcell

  //if ((x*x + z*z) >= r*r) z = sign(z)*(sqrt(r*r-x*x) - 0.05); // vertex z=edge
  
  double p   = 2 *(x * dx[0] + z * dx[2]);
  double q   = x*x + z*z - r*r;
  double p2q = p*p/4-q;
  
  double check1 = ((p2q < 0) ? 0 : - p/2 + sqrt(p2q));
  double check2 = ((p2q < 0) ? 0 : - p/2 - sqrt(p2q));
  
  double z1 = z + check1 * dx[2];
  double z2 = z + check2 * dx[2];
  
  // Wall !!!

  r += Wall;
  q = x*x + z*z - r*r;
  p2q = p*p/4-q;
  if (dx[2]>=0) {//forward angles
    result  = (z1>z2 ? check1 : check2);//take check1 or to depending on which z is bigger, to get positive distance (for inner radius)
    result2 = (z1>z2 ? ((p2q < 0) ? 0 : - p/2 + sqrt(p2q)): //same for outer radius
               	       ((p2q < 0) ? 0 : - p/2 - sqrt(p2q)));
  
    //Definition: Backwall = Wall after vertex
    Distance_to_EndOfCell = result2;//Distance from vertex to End of cell (outer)
    Length_in_Backwall = result2-result;//Distance electron went through inside the back wall
    Distance_to_BackWall = result;//Distance from vertex to backwall (inner)
    //Length_in_Wall = result2 - result;
  }
  else {//backward angles
    result  = (z1<z2 ? check1 : check2);
    result2 = (z1<z2 ? ((p2q < 0) ? 0 : - p/2 + sqrt(p2q)):
	               ((p2q < 0) ? 0 : - p/2 - sqrt(p2q)));
    Distance_to_EndOfCell = result2;//Distance from vertex to End of cell (outer)
    Length_in_Backwall = result2-result;//Distance electron went through inside the back wall
    //Length_in_Wall = result2 - result;
    Distance_to_BackWall = result;//Distance from vertex to backwall (inner)
    
    }
  //cout  << x <<", "<< z << ", "<< phi <<", "<< theta << ", "<<Distance_to_EndOfCell<<", " << Length_in_Backwall << " , "<< Distance_to_BackWall <<endl;
    
  // Snow !!!

    r += SnowWall;
  q = x*x + z*z - r*r;
  p2q = p*p/4-q;

  check1 = ((p2q < 0) ? 0 : - p/2 + sqrt(p2q));
  check2 = ((p2q < 0) ? 0 : - p/2 - sqrt(p2q));
  
  z1 = z + check1 * dx[2];
  z2 = z + check2 * dx[2];

  if (dx[2]>=0) {
    result2  = (z1>z2 ? check1 : check2);
    Length_through_Snow = result2 - result - Length_in_Backwall; //length through snow comes from here, why is there no method here for getLength_through_Snow like in Targetlength.h?
  }
  else {
    result2  = (z1<z2 ? check1 : check2);
    Length_through_Snow = result2 - result - Length_in_Backwall;
    }

  //Distances for incoming electron (use same calculations, but imagine electron being scatterd by 180deg in the same plane it came inside the cell:
  
    theta = M_PI;
  phi = 0;
  double dx_in[3] = {sin(theta) * cos(phi),          
	   sin(theta) * sin(phi),
	   cos(theta)};
  //get all the variables back to the inner radius and the new angles
  r = diameter / 2.;
  p   = 2 *(x * dx_in[0] + z * dx_in[2]);
  q   = x*x + z*z - r*r;
  p2q = p*p/4-q;
  check1 = ((p2q < 0) ? 0 : - p/2 + sqrt(p2q));
  check2 = ((p2q < 0) ? 0 : - p/2 - sqrt(p2q));
  z1 = z + check1 * dx[2];
  z2 = z + check2 * dx[2];

  //Wall like before:
  r += Wall;
  q = x*x + z*z - r*r;
  p2q = p*p/4-q;

  if (dx_in[2]>=0) {//forward angles
    result  = (z1>z2 ? check1 : check2);//take check1 or to depending on which z is bigger, to get positive distance (for inner radius)
    result2 = (z1>z2 ? ((p2q < 0) ? 0 : - p/2 + sqrt(p2q)): //same for outer radius
               	       ((p2q < 0) ? 0 : - p/2 - sqrt(p2q)));

    Distance_to_BeginningOfCell = result2;//Distance from vertex to Beginning of cell (outer)
    Length_in_Frontwall = result2-result;//Distance electron went through inside the front wall
    Distance_to_FrontWall = result;//Distance from vertex to frontwall (inner)
  }
  else {//backward angles
    result  = (z1<z2 ? check1 : check2);
    result2 = (z1<z2 ? ((p2q < 0) ? 0 : - p/2 + sqrt(p2q)):
	               ((p2q < 0) ? 0 : - p/2 - sqrt(p2q)));
    Distance_to_BeginningOfCell = result2;//Distance from vertex to Beginning of cell (outer)
    Length_in_Frontwall = result2-result;//Distance electron went through inside the front wall
    Distance_to_FrontWall = result;//Distance from vertex to frontwall (inner)
    
  }
  
  return Distance_to_BackWall; 

  }

int 
cryo_ewald::setPara(double len, double, double density, double WallThick,
		    double SnowThick, double SnowDensity)
{
  if (WallThick==0) Wall=0.010160; else Wall=WallThick;
  WallThickness=Wall;
  
  if (len==0) totallength=49.500000; else totallength=len;
  Length = totallength;

  TargetMat->setDensity(density);
  
  if (SnowThick < 0) SnowThick = 0;

  SnowWall = SnowThick;            // Thickness in mm
  SnowThickness = SnowWall;
  
  if (SnowDensity != 0) Snow->setDensity(SnowDensity);   // Density in g/cm^3

  return 1;
}

int 
cryo_ewald::Generate_Vertex(double random[], double x[], double /*wob_x*/, double /*wob_y*/, modeltype ModelType)
{
  if(strcmp(rundb.target, "Cryo.Wall"))
    {
      x[0] = rundb.sim.wobx * cos(random[0] * M_PI) + rundb.beam.offset.x - rundb.Target.offset_sim.x;
      x[1] = rundb.sim.woby * cos(random[1] * M_PI) + rundb.beam.offset.y - rundb.Target.offset_sim.y;
      x[2] = getLength() * (random[2] - .5);
    }
  else
    {

      x[0] = rundb.sim.wobx * cos(random[0] * M_PI) +
	rundb.beam.offset.x;
      x[1] = rundb.sim.woby * cos(random[1] * M_PI) +
	rundb.beam.offset.y;
      double sign_z = (random[2] > 0.5 ? -1 : 1);
  
      double R = 5.75;
  
      double z = getLength()/2 - (R - sqrt(fabs(R*R - x[0]*x[0]))) - 0.05;
  
      x[2] = z*sign_z;
    }
  
      double r = totalwidth / 2;
      double l = totallength / 2. - totalwidth / 2.;
  
      if (fabs(x[0]) >= r) return 0;                 // not inside targetcell
      if (fabs(x[2]) >= l + r) return 0;             // not inside targetcell
      if (fabs(x[2]) > l) {

	if ((fabs(x[2]) - l)*(fabs(x[2]) - l) + x[0]*x[0] >= r*r) return 0; 
      }  
      return 1;
}

double ReductionFactor(double u) 
{
  return 1 / (1+exp(0.220*(u-19.9))) / (1+exp(1.69*(u-25.4)));
}

double 
cryo_ewald::getLength_in_Target(double x, double, double z, double theta, double phi)
{
  int    outsideflag = 0;
  double result = 0;
  double result2 = 0;
  double r = totalwidth / 2.;
  double l = totallength / 2. - totalwidth / 2.;
  double g_x0 = 0;
  double k_z0 = 0;
  double dx[3] = {sin(theta) * cos(phi),
		  sin(theta) * sin(phi),
		  cos(theta)};

  if (fabs(dx[1]) == 1.) return -1;            // straight up or down
  if (fabs(x) >= r) return -1;                 // not inside targetcell
  
  if (fabs(z) >= totallength / 2) outsideflag = 1;   
                                               // not inside targetcell
  if (fabs(z) > l) {
      if ((fabs(z) - l)*(fabs(z) - l) + x*x >= r*r) outsideflag = 1; 
    }  
  
  if (outsideflag == 1) z = sign(z)*(sqrt(r*r-x*x) + l - 0.05);
                                               // vertex z=edge 

  Length_in_Wall = fabs(Wall / dx[0]);
  Length_through_Snow = fabs(SnowWall / dx[0]);
  
  g_x0 = (dx[0] >= 0 ? r : -r);
  double check = (z + (g_x0 - x) * dx[2] / dx[0]);
  
  if ((fabs(check)) <= l) {
    result = (g_x0 - x) / dx[0];
  }
  else {
    k_z0 = (check>=0 ? l : -l);
    
    double p   = 2 *(x * dx[0] + (z-k_z0) * dx[2]);
    double q   = x*x + (z-k_z0)*(z-k_z0) - r*r;
    double p2q = p*p/4-q;
    
    double check1 = ((p2q < 0) ? 0 : - p/2 + sqrt(p2q));
    double check2 = ((p2q < 0) ? 0 : - p/2 - sqrt(p2q));
    
    double z1 = z + check1 * dx[2];
    double z2 = z + check2 * dx[2];
    
    // Wall !!!
    
    r += Wall;
    q = x*x + (z-k_z0)*(z-k_z0) - r*r;
    p2q = p*p/4-q;
    
    if (dx[2]>=0) {
      result  = (z1>z2 ? check1 : check2);
      result2 = (z1>z2 ? ((p2q < 0) ? 0 : - p/2 + sqrt(p2q)):
		 ((p2q < 0) ? 0 : - p/2 - sqrt(p2q)));
      Length_in_Wall = result2 - result;
    }
    else {
      result  = (z1<z2 ? check1 : check2);
      result2 = (z1<z2 ? ((p2q < 0) ? 0 : - p/2 + sqrt(p2q)):
		 ((p2q < 0) ? 0 : - p/2 - sqrt(p2q)));
      Length_in_Wall = result2 - result;
    }
  }
  
  // Snow !!!

  r += SnowWall;

  g_x0 = (dx[0] >= 0 ? r : -r);
  check = (z + (g_x0 - x) * dx[2] / dx[0]);
  
  if ((fabs(check)) <= l) {
    result2 = (g_x0 - x) / dx[0];
    Length_through_Snow = result2 - result - Length_in_Wall;
  }
  else {
    k_z0 = (check>=0 ? l : -l);
    
    double p   = 2 *(x * dx[0] + (z-k_z0) * dx[2]);
    double q   = x*x + (z-k_z0)*(z-k_z0) - r*r;
    double p2q = p*p/4-q;
    
    double check1 = ((p2q < 0) ? 0 : - p/2 + sqrt(p2q));
    double check2 = ((p2q < 0) ? 0 : - p/2 - sqrt(p2q));
    
    double z1 = z + check1 * dx[2];
    double z2 = z + check2 * dx[2];
    
    q = x*x + (z-k_z0)*(z-k_z0) - r*r;
    p2q = p*p/4-q;
    
    if (dx[2]>=0) {
      result2  = (z1>z2 ? check1 : check2);
      Length_through_Snow = result2 - result - Length_in_Wall;
    }
    else {
      result2  = (z1<z2 ? check1 : check2);
      Length_through_Snow = result2 - result - Length_in_Wall;
    }
  }

  // parametrization of wall-coordinates for better snow correction
  double uwall = check;
  if (check > l) {
    double zz = z + result * dx[2] - l;
    double xx = x + result * dx[0];
    if (fabs(phi)<M_PI/2)
	uwall = l + fabs(atan2(zz, xx)) * r;
    else
	uwall = l + fabs(atan2(zz,-xx)) * r;
  }
  
  if (!rundb.Target.snowparam && theta < M_PI*0.999) {
    Length_through_Snow *= ReductionFactor(uwall);
  //
#ifdef DEBUG
    static ostream *out = NULL;
    if (!out) {
//    stdio_filebuf<char> fb(popen("gs -q - -dBATCH","w"), ios::out); 
//    out = new ostream(&fb);
      out = new ofstream("debug.ps");
    }
    out->precision(2);
    out->setf(ios::fixed);
    *out << "72 25.4 div dup scale 50 30 translate 0.2 setlinewidth\n"
	 << "/Helvetica findfont 4 scalefont setfont"<<endl
	 << "-20 5 moveto 20 5 lineto -20 -5 moveto 20 -5 lineto "
	 << "20 0 5 -90 90 arc stroke -20 0 5 90 270 arc stroke "
	 << z << " " << -x << " moveto "
	 << dx[2]*50<<" "<< -dx[0]*50<<" rlineto stroke "
	 << "0  5 moveto "<<uwall
	 <<" 5 lineto 1 0 0 setrgbcolor stroke 0 setgray "
	 << "0 -5 moveto "<<uwall
	 <<" -5 lineto 0 1 0 setrgbcolor stroke 0 setgray "
	 <<"-40 10 moveto (Theta: " 
	 << theta/M_PI*180 <<"  ) show (Phi: "
	 << phi /M_PI*180  <<"  ) show (u: "
	 << uwall          <<"  ) show (check: "
	 << check          <<"  ) show (delta: "
	 << ReductionFactor(uwall) << ") show showpage"<<endl;
#endif
  }
  return result;
}

int 
cryo_ewald_empty::setPara(double, double, double, double, double, double)
{
  return 1;
}

int 
cryo_ewald_empty::Generate_Vertex(double random[], double x[], double /*wob_x*/, double /*wob_y*/, modeltype ModelType)
{

  x[0] = rundb.sim.wobx * cos(random[0] * M_PI) +
    rundb.beam.offset.x;
  x[1] = rundb.sim.woby * cos(random[1] * M_PI) +
    rundb.beam.offset.y;
  double sign_z = (random[3] > 0.5 ? -1 : 1);
  
  double r = 5.75;
  
  double z = getLength()/2 - (r - sqrt(fabs(r*r - x[0]*x[0]))) - 0.05;
  
  x[2] = z*sign_z;
  
  r = totalwidth / 2;
  double l = totallength / 2. - totalwidth / 2.;
  
  if ((fabs(x[0]) >= r) || (fabs(x[0]) <= r-Wall)) return 0; //Compared to cryo_ewald::Generate_vertex this is wrong, because the wall should go from r to r+wall
  // not inside Wall
  if ((fabs(x[2]) >= l + r))return 0;             // not inside Wall
  if (fabs(x[2]) > l) {
      if (((fabs(x[2])-l)*(fabs(x[2])-l) + x[0]*x[0] >= r*r) ||
	  ((fabs(x[2])-l)*(fabs(x[2])-l) + x[0]*x[0] <= (r-Wall)*(r-Wall)))
	return 0; 
  }  
  return 1;
}

double 
cryo_ewald_empty::getLength_in_Target(double x, double, double z, double theta, double phi)
{
  int    outsideflag = 0;
  double result = 0;
  double result2 = 0;
  double r = totalwidth / 2.;
  double l = totallength / 2. - totalwidth / 2.;
  double g_x0 = 0;
  double k_z0 = 0;
  double dx[3] = {sin(theta) * cos(phi),
		  sin(theta) * sin(phi),
		  cos(theta)};

  if (fabs(dx[1]) == 1.) return -1;            // straight up or down
  if (fabs(x) >= r) return -1;                 // not inside targetcell
  
  if (fabs(z) >= totallength / 2) outsideflag = 1;   
                                               // not inside targetcell
  if (fabs(z) > l) {
      if ((fabs(z) - l)*(fabs(z) - l) + x*x >= r*r) outsideflag = 1; 
    }  
  
  if (outsideflag == 1) z = sign(z)*(sqrt(r*r-x*x) + l - 0.05);
                                               // vertex z=edge  
  
  Length_in_Wall = fabs(Wall / dx[0]);
  
  g_x0 = (dx[0] >= 0 ? r : -r);
  double check = (z + (g_x0 - x) * dx[2] / dx[0]);
  
  if ((fabs(check)) <= l) {
    result = (g_x0 - x) / dx[0];
    return result;
  }
  
  k_z0 = (check>=0 ? l : -l);
  
  double p   = 2 *(x * dx[0] + (z-k_z0) * dx[2]);
  double q   = x*x + (z-k_z0)*(z-k_z0) - r*r;
  double p2q = p*p/4-q;

  double check1 = ((p2q < 0) ? 0 : - p/2 + sqrt(p2q));
  double check2 = ((p2q < 0) ? 0 : - p/2 - sqrt(p2q));
  
  double z1 = z + check1 * dx[2];
  double z2 = z + check2 * dx[2];
  
  r += Wall;
  q = x*x + (z-k_z0)*(z-k_z0) - r*r;
  p2q = p*p/4-q;

  if (dx[2]>=0) {
    result  = (z1>z2 ? check1 : check2);
    result2 = (z1>z2 ? ((p2q < 0) ? 0 : - p/2 + sqrt(p2q)):
               	       ((p2q < 0) ? 0 : - p/2 - sqrt(p2q)));
    Length_in_Wall = result2 - result;
  }
  else {
    result  = (z1<z2 ? check1 : check2);
    result2 = (z1<z2 ? ((p2q < 0) ? 0 : - p/2 + sqrt(p2q)):
	               ((p2q < 0) ? 0 : - p/2 - sqrt(p2q)));
    Length_in_Wall = result2 - result;
  }
  
  return result;
}

//################### cryo isr_2013 ########


double cryo_ewald_2014::getLength()
{
  if ( modeltype2 == 0 )
    {
      return Length;
    }
  else if ( modeltype2 == 1 )
    {
      return Length + 2 * Wall;
    }
  else if (modeltype2 == 2)
    {
      return Length + 2 * Wall + 2 * SnowWall;
    }
  else
    {
      return Length;
    }
}

double cryo_ewald_2014::gethalfLength(int model)
{
  if ( model == 0 )//hydrogen
    {
      return Length / 2;
    }
  else if ( model == 1 ) // Wall
    {
      return Length / 2 + Wall;
    }
  else if ( model == 2 ) // Snow
    {
      return Length / 2+ Wall + SnowWall;
    }
  else if ( model == 3 )
    {
      return Length / 2 - totalwidth / 2;
    }
  else  
    {
      return Length / 2;
    }
}

double cryo_ewald_2014::getRadius(int model)
{
  if ( model == 0 )//hydrogen
    {
      return totalwidth / 2;
    }
  else if ( model == 1 ) // Wall
    {
      return totalwidth / 2 + Wall;
    }
  else if ( model == 2 ) // Snow
    {
      return totalwidth / 2 + Wall + SnowWall;
    }
  else 
    {
      return totalwidth / 2;
    }
}


int cryo_ewald_2014::setPara(double len, double angle, double density, double WallThick, double SnowThick, double SnowDensity)//The TargetMat is chosen with the Modeltype (hydrogen, wall or ice), the Material for the different parts is hard coded here, LH2 for the inner part, Havar for the middle part and FrozenAir for the outer part. The density for the hydrogen is set with Target.Density in the run.db, the density for the ice is set with Target.SnowDensity in the run.db, angle will be used to make the ice on the side of the cell thicker, because, where the beam goes through there is less ice
{
  if ( density > 0 )
    {
      LH2->setDensity ( density );
    }
  else
    {
      LH2->setDensity ( 1e-60 );
    }

  if (SnowThick < 0)
    {
      SnowThick = 0;
    }
  SnowWall = SnowThick;            // Thickness in mm
  SnowThickness = SnowWall;
  if ( len > 0 ) // with Target.Length in the run.db the whole form of the cell can be increased or decreased. Increase can be due to the expansion of the cell because of pressure when liquid hydrogen flows through the cell.
    {
      totalwidth = totalwidth * len / totallength; 
      totallength = len;
    }

  Length = totallength;
  if (SnowDensity > 0) 
    {
      FrozenAir->setDensity(SnowDensity);   // Density in g/cm^3
      N2->setDensity(SnowDensity);
    }
  else
    {
      FrozenAir->setDensity(1e-60);   // Density in g/cm^3
    }
  if(angle > 1.)// only if the ice on the side wall is thicker than at the beam entrance/exit makes sense
    {
      icefactor = angle;
    }
  if ( WallThick == 0 ) 
    {
      Wall = 0.01016;
    }
  else if ( WallThick < 0 )
    {
      Wall = 0;
    }
  else
    {
      Wall = WallThick;
    }
  WallThickness = Wall;
  return 1;
}

double cryo_ewald_2014::getLength_in_Target(double x, double y, double z, double theta, double phi)
{
  double result = 0;
  double r[3] = {totalwidth / 2, totalwidth / 2 + Wall, totalwidth / 2 + Wall + SnowWall*icefactor}; // round part of the cell, beam sees thicker ice (in most cases) 
  if( (theta == M_PI && phi == 0) || z >= totallength / 2 - totalwidth / 2 -0.5)  // if beam is reconstructed to the beam line or if particle is at the end of the cell, then the particle does not see the ice of the side which could be thicker
    {
      r[2] = totalwidth / 2 + Wall + SnowWall;    
    }

  double r2[3] = {totalwidth / 2, totalwidth / 2 + Wall, totalwidth / 2 + Wall + SnowWall*icefactor}; // make ice thicker on the side with icefactor
  double l = totallength / 2 - totalwidth / 2;    // straight part of cell
  double dx[3] = {sin(theta) * cos(phi),          
		  sin(theta) * sin(phi),
		  cos(theta)};

  if (fabs(dx[1]) == 1) // straight up or down
    {
      return -1;
    }
  if (fabs(x) > r[2] )  // not inside targetcell
    {
      return -1;
    }
  if ( x*x > (r[2] ) * (r[2] ) - (fabs(z) - l ) * (fabs(z) - l ) && fabs(z) > l) // outside cell, fabs(z) because it can be positive or negative 
    {
      z = sign(z) * ( sqrt( (r[2] ) * (r[2] ) - x*x) + l -0.000001 ); // if reconstructed event is out of cell, set to to the edge of the snow to calculate the energy correction as good as possible
    }
  if ( (x*x == r[0]*r[0] && fabs(z) <= l ) || (x*x == ( r[1] ) * ( r[1] ) && fabs(z) <= l ) || (x*x == ( r[2] ) * ( r[2] ) && fabs(z) <= l ) ) // if event is directly on the long side: change coordinates, so that the crossing point with the wall can be calculated
    {
      z = sign(z) * ( fabs(z) - 0.0000001 );
    }

 // start value is vertex in case the vertex is outside of hydrogen or the wall; just for consistency for snow also
  double xhits[6] = {x,x,x,x,x,x};// nearest crossing point of hydrogen edge, farest crossing point of hydrogen edge, nearest crossing point of wall edge, farest crossing point of wall edge, vertex ( snow has only one crossing point ), crossing point for snow; the nearest crossing points can be the vertex position if only one point is crossed
  double zhits[6] = {z,z,z,z,z,z};
  double p, q, p2, q2; // p-q formula for crossing point of particle track with round part of hydrogen edge
  p = 2 * ( x * dx[0] + z * dx[2] - dx[2] * l ) / ( dx[0] * dx[0] + dx[2] *dx[2] );  // p is the same for hydrogen, wall and snow because it only knows about the center of the circle
  p2 = 2 * ( x * dx[0] + z * dx[2] + dx[2] * l ) / ( dx[0] * dx[0] + dx[2] *dx[2] ); 



  for ( int j = 0; j < 3; j++)
    {
      int i = 2 * j;
      if ( ( r2[j] - x ) / dx[0] > 0 && fabs(( r2[j] - x ) / dx[0] * dx[2] + z) < l ) //upper long part of cell edge, check if this is hit and if this is in the right direction
	{
	  xhits[i] = r2[j];
	  zhits[i] = z + ( r2[j] - x ) / dx[0] * dx[2];
	  i++;
	}
      if( ( -r2[j] - x ) / dx[0] > 0 && fabs(( -r2[j] - x ) / dx[0] * dx[2] + z) < l )  // lower long part of cell edge, check if this is hit and if this is in the right direction
	{
	  xhits[i] = -r2[j];
	  zhits[i] = z + ( -r2[j] - x ) / dx[0] * dx[2];
	  i++;
	}

      q = ( x*x + z*z - r[j]*r[j] + l*l - 2*z*l ) / ( dx[0] * dx[0] + dx[2] *dx[2] );
      q2 = ( x*x + z*z - r[j]*r[j] + l*l + 2*z*l ) / ( dx[0] * dx[0] + dx[2] *dx[2] );
      bool pqbig = ( p*p/4 - q >= 0 ? ( -p/2 + sqrt( p*p/4 - q ) > 0 ? ( fabs( z + (-p/2 + sqrt( p*p/4 - q )) * dx[2] ) >= l ? true : false ) : false ) : false ); // check if sqrt (p*p/4 - q) works and then if the crossing point would be in the direction of the particle ( the crossing point could mathematically be in the other direction ) and then if the crossing point of the particle hits the outer round part of the circle; check if long distance from vertex is hit
      bool pqlittle = ( p*p/4 - q >= 0 ? ( -p/2 - sqrt( p*p/4 - q ) > 0 ? ( fabs( z + (-p/2 - sqrt( p*p/4 - q )) * dx[2] ) >= l ? true : false ) : false ) : false );// check if short distance from vertex is hit
      bool p2q2big = ( p2*p2/4 - q2 >= 0 ? ( -p2/2 + sqrt( p2*p2/4 - q2 ) > 0 ? ( fabs( z + (-p2/2 + sqrt( p2*p2/4 - q2 )) * dx[2] ) >= l ? true : false ) : false ) : false );
      bool p2q2little = ( p2*p2/4 - q2 >= 0 ? ( -p2/2 - sqrt( p2*p2/4 - q2 ) > 0 ? ( fabs( z + (-p2/2 - sqrt( p2*p2/4 - q2 )) * dx[2] ) >= l ? true : false ) : false ) : false );
      if ( pqbig )
	{
	  xhits[i] = x + ( -p/2 + sqrt( p*p/4 - q )) * dx[0];
	  zhits[i] = z + ( -p/2 + sqrt( p*p/4 - q )) * dx[2];
	  i++;
	}
      if ( i > 2 * j + 2 )
	{
	  std::cout << "Error in pathlength calculation: too many crossings points found\n";
	  return 0;
	}
      if ( pqlittle )
	{
	  xhits[i] = x + ( -p/2 - sqrt( p*p/4 - q )) * dx[0];
	  zhits[i] = z + ( -p/2 - sqrt( p*p/4 - q )) * dx[2];
	  i++;
	}
      if ( i > 2 * j + 2 )
	{
	  std::cout << "Error in pathlength calculation: too many crossings points found\n";
	  return 0;
	}
      if ( p2q2big ) 
	{
	  xhits[i] = x + ( -p2/2 + sqrt( p2*p2/4 - q2 )) * dx[0];
	  zhits[i] = z + ( -p2/2 + sqrt( p2*p2/4 - q2 )) * dx[2];
	  i++;
	}
      if ( i > 2 * j + 2 )
	{
	  std::cout << "Error in pathlength calculation: too many crossings points found\n";
	  return 0;
	}
      if ( p2q2little ) 
	{
	  xhits[i] = x + ( -p2/2 - sqrt( p2*p2/4 - q2 )) * dx[0];
	  zhits[i] = z + ( -p2/2 - sqrt( p2*p2/4 - q2 )) * dx[2];
	  i++;
	}
      if ( i > 2 * j + 2 )
	{
	  std::cout << "Error in pathlength calculation: too many crossings points found\n";
	  return 0;
	}
      if ( j == 2 && i > 2 * j + 1 )
	{
	  std::cout << i <<" Error in pathlength calculation: 2 crossings points found for snow found, x: " << x << ", z: " << z << ", xhits1: " << xhits[4] << ", xhits2: " << xhits[5  ]<< ", zhits1: " << zhits[4] << ", zhits2: " << zhits[5] << "\n";
	  return 0;
	}
    }

  for ( int j = 0; j < 6; j = j + 2 ) // sorts the results, so the following calculation will be correct
    {
      if ( sqrt ( ( x - xhits[j] ) * ( x - xhits[j] ) + ( z - zhits[j] ) * ( z - zhits[j] ) ) > sqrt ( ( x - xhits[j+1] ) * ( x - xhits[j+1] ) + ( z - zhits[j+1] ) * ( z - zhits[j+1] ) ) )
	{
	  double i = xhits[j];
	  xhits[j] = xhits[j+1];
	  xhits[j+1] = i;
	  i = zhits[j];
	  zhits[j] = zhits[j+1];
	  zhits[j+1] = i;
	}												     
    }

  // std::cout << "xh: " << xhits[0] << ", zh: " << zhits[0] << ", xh2: " <<  xhits[1] << ", zh2: " << zhits[1] << ", xw: " << xhits[2] << ", zw: " <<  zhits[2] << ", xw2: " << xhits[3] << ", zw2: " << zhits[3] << ", xs2: " << xhits[5] << ", zs2: " << zhits[5] << endl<<endl;
  // if Length_in_Wall > 0 than result > 0 and Length_in_Wall2 and Length_through_Snow2; if Length_through_Snow > 0 than Length_in_Wall > 0 and result > 0 and Length_in_Wall2 and Length_through_Snow2
  result = sqrt( ( xhits[0] - xhits[1] ) * ( xhits[0] - xhits[1] ) + ( zhits[0] - zhits[1] ) * ( zhits[0] - zhits[1] )) / fabs ( cos(phi) ); // cos(phi) for length in y-direction; length in hydrogen
  Length_in_Wall = sqrt( ( xhits[2] - xhits[0] ) * ( xhits[2] - xhits[0] ) + ( zhits[2] - zhits[0] ) * ( zhits[2] - zhits[0] )) / fabs ( cos(phi) ); // cos(phi) for length in y-direction; length in first crossing of wall, if vertex in wall or snow, otherwise is 0
  Length_through_Snow = sqrt( ( xhits[4] - xhits[2] ) * ( xhits[4] - xhits[2] ) + ( zhits[4] - zhits[2] ) * ( zhits[4] - zhits[2] )) / fabs ( cos(phi) ); // cos(phi) for length in y-direction; length in first crossing of snow, if vertex in snow, otherwise is 0
  Length_in_Wall2 = sqrt( ( xhits[1] - xhits[3] ) * ( xhits[1] - xhits[3] ) + ( zhits[1] - zhits[3] ) * ( zhits[1] - zhits[3] )) / fabs ( cos(phi) ); // cos(phi) for length in y-direction; length of second crossing of wall
 Length_through_Snow2 = sqrt( ( xhits[3] - xhits[5] ) * ( xhits[3] - xhits[5] ) + ( zhits[3] - zhits[5] ) * ( zhits[3] - zhits[5] )) / fabs ( cos(phi) ); // cos(phi) for length in y-direction; length of second crossing of snow
 // std::cout << "x: " << xhits[4] <<", y: " <<  y << ", z: " << zhits[4] << ", dx[0]: " << dx[0] << ", dx[2]: " << dx[2] << ", theta: " << theta << ", phi: " << phi <<", snow1: " << Length_through_Snow << ", wall1: " << Length_in_Wall << ", path: " << result << ", wall2: " << Length_in_Wall2 << ", snow2: " << Length_through_Snow2 << "\n"<<endl;
 return result;
}

int cryo_ewald_2014::Generate_Vertex(double random[], double x[], double /*wob_x*/, double /*wob_y*/, modeltype Modeltype)
{
    x[0] = rundb.sim.wobx * cos(random[0] * M_PI) + rundb.beam.offset.x - rundb.Target.offset_sim.x;
    x[1] = rundb.sim.woby * cos(random[1] * M_PI) + rundb.beam.offset.y - rundb.Target.offset_sim.y;
  if(modeltype2 == 0) // hydrogen simulation
    {
      x[2] = 2 * gethalfLength(0) * (random[2] - .5);
      double r = totalwidth / 2;       // width and round part of cell
      double l = totallength / 2 - totalwidth / 2;    // straight part of cell
  
      if (fabs(x[0]) > r) 
	{
	  return 0;                 // not inside targetcell
	}
      if (fabs(x[2]) > l) 
	{
	  if ((fabs(x[2]) - l)*(fabs(x[2]) - l) + x[0]*x[0] > r*r) 
	    {
	      return 0;       // not inside targetcell
	    }
	}  
      return 1;
    }
  if(modeltype2 == 1) // wall simulation
    {
      double r = totalwidth / 2;       // width and round part of cell
      double r2 = totalwidth / 2 + Wall;  // width and round part of cell with wall
      double l = totallength / 2 - totalwidth / 2;    // straight part of cell
      x[2] = sign((random[2]-0.5))*(l+pow(r*r-x[0]*x[0],0.5)) + ((random[2]-0.5)*(pow(r2*r2-x[0]*x[0],0.5)-pow(r*r-x[0]*x[0],0.5))*2);

      if (fabs(x[0]) > r2 || (fabs(x[0]) < r && fabs(x[2]) < l )) 
	{
	  return 0;                 // not inside wall
	}
      if (fabs(x[2]) > l) 
	{
	  if ((fabs(x[2]) - l)*(fabs(x[2]) - l) + x[0]*x[0] > r2*r2 ||(fabs(x[2]) - l)*(fabs(x[2]) - l) + x[0]*x[0] < r*r) 
	    {
	      return 0;       // not inside wall
	    }
	}  
      return 1;
      }

  if(modeltype2 == 2) // snow simulation
    {
      double r = totalwidth / 2 + Wall;       // width and round part of cell with wall
      double r2 = totalwidth / 2 + Wall + SnowWall;  // width and round part of cell with wall and snow
      double l = totallength / 2. - totalwidth / 2.;    // straight part of cell
      x[2] = sign((random[2]-0.5))*(l+pow(r*r-x[0]*x[0],0.5)) + ((random[2]-0.5)*(pow(r2*r2-x[0]*x[0],0.5)-pow(r*r-x[0]*x[0],0.5))*2);
  
      if (fabs(x[0]) > r2 || (fabs(x[0]) < r && fabs(x[2]) < l )) 
	{
	  return 0;                 // not inside snow
	}
      if (fabs(x[2]) > l) 
	{
	  if ( (fabs(x[2]) - l)*(fabs(x[2]) - l) + x[0]*x[0] > r2*r2 || (fabs(x[2]) - l)*(fabs(x[2]) - l) + x[0]*x[0] < r*r ) 
	    {
	      return 0;       // not inside snow
	    }
	}  
      return 1;
      }
  std::cout << "Warning no proper target chosen (Cryo.2014_hydrogen, Cryo.2014_wall or Cryo.2014_ice)\n";
  return 0; // not proper modeltype chosen
}

void cryo_ewald_2014::EnergyLossSim(Particle& P, double x, double y, double z, int steps, modeltype Modeltype)// energy loss simulation for the simulation from the vertex point to the end of the cell

  //bss: halton numbering:
  //0: internalBr
  //1: Landaul frozenair
  //2: Landaul frozenair
  //3: external frozenair
  //4: Laundaul wall
  //5: Landaul wall
  //6: external wall
  //7: Landaul LH2
  //8: Landaul LH2
  //9: external LH2
  //10: Landaul wall2
  //11: Landaul wall2
  //12: externl wall2
  //13: Landaul frozenair2
  //14: Landaul frozenair2
  //15: external frozenair2

{
  x -= rundb.Target.offset_sim.x; //bss: this function is called from deep inside of simDetectorBase or so, it gets targetpos_hall coords!!! here we wanna have targetpos_tar coords for ELOSS in target!
  y -= rundb.Target.offset_sim.y; 
  z -= rundb.Target.offset_sim.z; 

  double old_momentum;
  // calcultate the loss in the order the electron is going through the materials of the cell, it could be, that if the particle vertex is in the ice that only SnowPath2 is greater than 0

  double pathlength = getLength_in_Target(x, y, z, P.theta(), P.phi()) / 10;
  double SnowPath1 = Length_through_Snow / 10;
  double WallPath1 = Length_in_Wall / 10;
  double WallPath2 = Length_in_Wall2 / 10;
  double SnowPath2 = Length_through_Snow2 / 10;

  static int exRadWall = ((int)rundb.Target.externalRadiation) & 0x1;
  static int exRadTar  = ((int)rundb.Target.externalRadiation) & 0x2;
  static int exRadSnow = ((int)rundb.Target.externalRadiation) & 0x8;
  static int intRadSnow =  ((int)rundb.Target.internalRadiation) & 0x8;        //internal Radiation in Snow
  static int intRadWall =  ((int)rundb.Target.internalRadiation) & 0x1;        //internal Radiation in Wall
  static int intRadTar  =  ((int)rundb.Target.internalRadiation) & 0x2;        //internal Radiation in Target

  // every path gets Landau loss, external Bremsstrahlung and multiscattering and for the vertex position also internal Bremsstrahlung for electrons, if particle is heavier than only dEdx is done and multiscattering
  if ( pathlength >= 0 ) // pathlength is 0, if particle only goes through wall or snow
    {
      if (P == P_electron) 
	{	  
	  // internalBremsstrahlung is only used once, there where the vertex is, this is found out by looking which the first path greater than 0 is
	  old_momentum = P.momentum();	  
	  if ( SnowPath1 > 0 ) // can only happen if vertex is in the snow
	    {
	      if ( intRadSnow )
		{
		  internalBremsstrahlung ( P, FrozenAir, halton[0], rundb.sim.q2 ); // only for vertex point
		}
	      LandauLoss ( P, FrozenAir, SnowPath1, halton[1], halton[2] );	  
	      if ( exRadSnow ) 
		{
		  externalBremsstrahlung ( P, FrozenAir, SnowPath1, halton[3] );      
		}
	      FrozenAir->MultiScat( P, old_momentum, SnowPath1 );
	      old_momentum = P.momentum();
	    }
	  if (  WallPath1 > 0 && SnowPath1 == 0  && intRadWall ) // can only happen if vertex is in the wall
	    {
	      internalBremsstrahlung ( P, WallMat, halton[0], rundb.sim.q2 ); // only for vertex point
	    }
	  if ( WallPath1 > 0 )
	    {
	      LandauLoss ( P, WallMat, WallPath1, halton[4], halton[5] );	  
	      if ( exRadWall ) 
		{
		  externalBremsstrahlung ( P, WallMat, WallPath1, halton[6] );      
		}
	      WallMat->MultiScat ( P, old_momentum, WallPath1 );
	      old_momentum = P.momentum();
	    }
	  if ( WallPath1 == 0 && SnowPath1 == 0 && pathlength > 0 ) // can only happen if vertex is in the hydrogen
	    {
	      if ( intRadTar )
		{ 
		  internalBremsstrahlung ( P, LH2, halton[0], rundb.sim.q2 ); // only for vertex point
		}
	    }
	  if ( pathlength > 0 )
	    {
	      LandauLoss ( P, LH2, pathlength, halton[7], halton[8] );	  
	      if ( exRadTar ) 
		{
		  externalBremsstrahlung ( P, LH2, pathlength, halton[9] );      
		}
	      LH2->MultiScat ( P, old_momentum, pathlength );
	      old_momentum = P.momentum();
	    }
	  if ( ( pathlength == 0 && WallPath2 > 0 ) && intRadWall ) // can only happen if vertex is in the wall
	    {
	      internalBremsstrahlung ( P, WallMat, halton[0], rundb.sim.q2 ); // only for vertex point
	    }
	  if ( WallPath2 > 0 ) 
	    {
	      LandauLoss ( P, WallMat, WallPath2, halton[10], halton[11] );	  
	      if ( exRadWall ) 
		{
		  externalBremsstrahlung ( P, WallMat, WallPath2, halton[12] );      
		}
	      WallMat->MultiScat ( P, old_momentum, WallPath2 );
	      old_momentum = P.momentum();
	    }
	  if ( ( WallPath2 == 0  && SnowPath2 > 0 ) && intRadSnow ) // can only happen if vertex is in the snow
	    {
	      internalBremsstrahlung ( P, FrozenAir, halton[0], rundb.sim.q2 ); // only for vertex point
	    }
	  if ( SnowPath2 > 0 ) 
	    {
	      LandauLoss ( P, FrozenAir, SnowPath2, halton[13], halton[14] );	  
	      if ( exRadSnow ) 
		{
		  externalBremsstrahlung ( P, FrozenAir, SnowPath2, halton[15] );      
		}
	      FrozenAir->MultiScat ( P, old_momentum, SnowPath2 );
	      old_momentum = P.momentum();
	    }
	}
      else
	{
	  if ( SnowPath1 > 0 ) 
	    {
	      for ( int i = 0; i < steps; i++ ) 
		{
		  old_momentum = P.momentum();
		  P += FrozenAir->dEdx(P, SnowPath1/steps);// decreases P because dEdx is negative, this decreases the momentum for the amount lost in the target after the vertex
		  FrozenAir->MultiScat(P, old_momentum, SnowPath1/steps);
		}
	    }
	  if ( WallPath1 > 0 ) 
	    {
	      for ( int i = 0; i < steps; i++ ) 
		{
		  old_momentum = P.momentum();
		  P += WallMat->dEdx(P, WallPath1/steps);
		  WallMat->MultiScat(P, old_momentum, WallPath1/steps);
		}
	    }
	  if ( pathlength > 0 ) 
	    {
	      for ( int i = 0; i < steps; i++ ) 
		{
		  old_momentum = P.momentum();
		  P += LH2->dEdx(P, pathlength/steps);
		  LH2->MultiScat(P, old_momentum, pathlength/steps);
		}
	    }
	  if ( WallPath2 > 0 ) 
	    {
	      for ( int i = 0; i < steps; i++ ) 
		{
		  old_momentum = P.momentum();
		  P += WallMat->dEdx(P, WallPath2/steps);
		  WallMat->MultiScat(P, old_momentum, WallPath2/steps);
		}
	    }
	  if ( SnowPath2 > 0 ) 
	    {
	      for ( int i = 0; i < steps; i++ ) 
		{
		  old_momentum = P.momentum();
		  P += FrozenAir->dEdx(P, SnowPath2/steps);
		  FrozenAir->MultiScat(P, old_momentum, SnowPath2/steps);
		}
	    }
	}
    }
  else 
    {
      if (!(pathlength==-0.1)) 
	{
	  std::cout << "Warning: pathlength through target is negativ: " << pathlength*10 << " mm !\n";
	}
    }
}
  
void cryo_ewald_2014::EnergyLossSimBeam(Particle& P, double x, double y, double z, int steps, modeltype Modeltype)// energy loss simulation for the simulation from the vertex point to beam entrance point of the cell
{
  double old_momentum;
  // calcultate the loss in the order the electron is going through the materials of the cell, it could be, that if the particle vertex is in the ice that only SnowPath2 is greater than 0

  double pathlength = getLength_in_Target(x, y, z, M_PI, 0.) / 10;
  double SnowPath1 = Length_through_Snow / 10;
  double WallPath1 = Length_in_Wall / 10;
  double WallPath2 = Length_in_Wall2 / 10;
  double SnowPath2 = Length_through_Snow2 / 10;

  static int exRadWall = ((int)rundb.Target.externalRadiation) & 0x1;
  static int exRadTar  = ((int)rundb.Target.externalRadiation) & 0x2;
  static int exRadSnow = ((int)rundb.Target.externalRadiation) & 0x8;
  static int intRadSnow =  ((int)rundb.Target.internalRadiation) & 0x8;        //internal Radiation in Snow
  static int intRadWall =  ((int)rundb.Target.internalRadiation) & 0x1;        //internal Radiation in Wall
  static int intRadTar  =  ((int)rundb.Target.internalRadiation) & 0x2;        //internal Radiation in Target

  // every path gets Landau loss, external Bremsstrahlung and multiscattering and for the vertex position also internal Bremsstrahlung for electrons, if particle is heavier than only dEdx is done and multiscattering
  if ( pathlength >= 0 ) // pathlength is 0, if particle only goes through wall or snow
    {	  
      // internalBremsstrahlung is only used once, there where the vertex is, this is found out by looking which the first path greater than 0 is
      old_momentum = P.momentum();	  
      if ( SnowPath1 > 0 ) // can only happen if vertex is in the snow
	{
	  if ( intRadSnow )
	    {
	      internalBremsstrahlung ( P, FrozenAir, halton[16], rundb.sim.q2 ); // only for vertex point
	    }
	  LandauLoss ( P, FrozenAir, SnowPath1, halton[17], halton[18] );	  
	  if ( exRadSnow ) 
	    {
	      externalBremsstrahlung ( P, FrozenAir, SnowPath1, halton[19] );      
	    }
	  FrozenAir->MultiScat( P, old_momentum, SnowPath1 );
	  old_momentum = P.momentum();
	}
      if (  WallPath1 > 0 && SnowPath1 == 0  && intRadWall ) // can only happen if vertex is in the wall
	{
	  internalBremsstrahlung ( P, WallMat, halton[16], rundb.sim.q2 ); // only for vertex point
	}
      if ( WallPath1 > 0 )
	{
	  LandauLoss ( P, WallMat, WallPath1, halton[20], halton[21] );	  
	  if ( exRadWall ) 
	    {
	      externalBremsstrahlung ( P, WallMat, WallPath1, halton[22] );      
	    }
	  WallMat->MultiScat ( P, old_momentum, WallPath1 );
	  old_momentum = P.momentum();
	}
      if ( WallPath1 == 0 && SnowPath1 == 0 && pathlength > 0 ) // can only happen if vertex is in the hydrogen
	{
	  if ( intRadTar )
	    { 
	      internalBremsstrahlung ( P, LH2, halton[16], rundb.sim.q2 ); // only for vertex point
	    }
	}
      if ( pathlength > 0 )
	{
	  LandauLoss ( P, LH2, pathlength, halton[23], halton[24] );	  
	  if ( exRadTar ) 
	    {
	      externalBremsstrahlung ( P, LH2, pathlength, halton[25] );      
	    }
	  LH2->MultiScat ( P, old_momentum, pathlength );
	  old_momentum = P.momentum();
	}
      if ( ( pathlength == 0 && WallPath2 > 0 ) && intRadWall ) // can only happen if vertex is in the wall
	{
	  internalBremsstrahlung ( P, WallMat, halton[16], rundb.sim.q2 ); // only for vertex point
	}
      if ( WallPath2 > 0 ) 
	{
	  LandauLoss ( P, WallMat, WallPath2, halton[26], halton[27] );	  
	  if ( exRadWall ) 
	    {
	      externalBremsstrahlung ( P, WallMat, WallPath2, halton[28] );      
	    }
	  WallMat->MultiScat ( P, old_momentum, WallPath2 );
	  old_momentum = P.momentum();
	}
      if ( ( WallPath2 == 0  && SnowPath2 > 0 ) && intRadSnow ) // can only happen if vertex is in the snow
	{
	  internalBremsstrahlung ( P, FrozenAir, halton[16], rundb.sim.q2 ); // only for vertex point
	}
      if ( SnowPath2 > 0 ) 
	{
	  LandauLoss ( P, FrozenAir, SnowPath2, halton[29], halton[30] );	  
	  if ( exRadSnow ) 
	    {
	      externalBremsstrahlung ( P, FrozenAir, SnowPath2, halton[31] );      
	    }
	  FrozenAir->MultiScat ( P, old_momentum, SnowPath2 );
	  old_momentum = P.momentum();
	}
    }
  else 
    {
      if (!(pathlength==-0.1)) 
	{
	  std::cout << "Warning: pathlength through target is negativ: " << pathlength*10 << " mm !\n";
	}
    }
}

void cryo_ewald_2014::EnergyLossCorr(Particle& P, double x, double y, double z, int steps)// effective energy loss for analysis from the vertex point to the end of the cell
{
  // calcultate the loss in the order the electron is going through the materials of the cell, it could be, that if the particle vertex is in the ice that only SnowPath2 is greater than 0
  double pathlength = getLength_in_Target ( x, y, z, P.theta(), P.phi()) / 10;
  double SnowPath1 = Length_through_Snow / 10;
  double WallPath1 = Length_in_Wall / 10;
  double WallPath2 = Length_in_Wall2 / 10;
  double SnowPath2 = Length_through_Snow2 / 10;

  if ( !( pathlength < 0 ) ) //no error in pathlenght calculation
    {
      if ( SnowPath1 > 0 ) 
	{
	  for ( int i=0; i<steps; i++ ) 
	    {
	      P -= FrozenAir->dEdx ( P, SnowPath1 / steps );// increases P because dEdx is negative, this increases the momentum for the amount lost in the target after the vertex
	    }
	}
      if ( WallMat && WallPath1 > 0 )
	{
	  for ( int i=0; i<steps; i++ ) 
	    {
	      P -= WallMat->dEdx ( P, WallPath1 );
	    }
	}
      else if (! (WallMat) ) 
	{
	  std::cout << "Warning: no wall material selected!\n ";
	}
      if ( TargetMat && pathlength > 0 ) 
	{
	  for ( int i=0; i<steps; i++ ) 
	    {
	      P -= LH2->dEdx ( P, pathlength / steps );
	    }
	}
      else if (! (TargetMat) ) 
	{
	  std::cout << "Warning: no target material selected!\n ";
      }
      if ( WallMat && WallPath2 > 0 )
	{
	  for ( int i=0; i<steps; i++ ) 
	    {
	      P -= WallMat->dEdx ( P, WallPath2 );
	    }
	}
      else if (! (WallMat) ) 
	{
	std::cout << "Warning: no wall material selected!\n ";
	}
      if ( SnowPath2 > 0 ) 
	{
	  for ( int i=0; i<steps; i++ ) 
	    {
	      P -= FrozenAir->dEdx ( P, SnowPath2 / steps );
	  }
	}
    }
  else {
    if (!(pathlength==-0.1)) {
      std::cout << "Warning: pathlength through target is negativ: "
		<< pathlength*10 << " mm !\n";
    }
  }
}

void cryo_ewald_2014::EnergyLossCorrBeam(Particle& P, double x, double y, double z, int steps)// effective energy loss for the analysis from the vertex to the beam entrance of the cell
  //bss halton:
  //16: internalBr
  //17: Landau frozenair
  //18: Landau frozenair
  //19: external frozenair
  //20: Landau wall
  //21: Landau wall
  //22: external wall
  //23: Landau LH2
  //24: Landau LH2
  //25: external LH2
  //26: Landau wall2
  //27: Landau wall2
  //28: external wall2
  //29: Landau frozenair2
  //30: Landau frozenair2
  //31: external frozenair2

{
  // calcultate the loss in the order the electron is going through the materials of the cell, it could be, that if the particle vertex is in the ice that only SnowPath2 is greater than 0
  double pathlength = getLength_in_Target(x, y, z, M_PI, 0.) / 10;
  double SnowPath1 = Length_through_Snow / 10;
  double WallPath1 = Length_in_Wall / 10;
  double WallPath2 = Length_in_Wall2 / 10;
  double SnowPath2 = Length_through_Snow2 / 10;

  if ( !( pathlength < 0 ) ) //no error in pathlenght calculation
    {
      if ( SnowPath1 > 0 ) 
	{
	  for ( int i=0; i<steps; i++ ) 
	    {
	      P += FrozenAir->dEdx ( P, SnowPath1 / steps );// decreases P because dEdx is negative, this decreases the momentum for the amount lost in the target before the vertex
	    }
	}
      if ( WallMat && WallPath1 > 0 )
	{
	  for ( int i=0; i<steps; i++ ) 
	    {
	      P += WallMat->dEdx ( P, WallPath1 );
	    }
	}
      else if (! (WallMat) ) 
	{
	  std::cout << "Warning: no wall material selected!\n ";
	}
      if ( TargetMat && pathlength > 0 ) 
	{
	  for ( int i=0; i<steps; i++ ) 
	    {
	      P += LH2->dEdx ( P, pathlength / steps );
	    }
	}
      else if (! (TargetMat) ) 
	{
	  std::cout << "Warning: no target material selected!\n ";
	}
      if ( WallMat && WallPath2 > 0 )
	{
	  for ( int i=0; i<steps; i++ ) 
	    {
	      P += WallMat->dEdx ( P, WallPath2 );
	    }
	}
      else if (! (WallMat) ) 
	{
	  std::cout << "Warning: no wall material selected!\n ";
	}
      if ( SnowPath2 > 0 ) 
	{
	  for ( int i=0; i<steps; i++ ) 
	    {
	      P += FrozenAir->dEdx ( P, SnowPath2 / steps );
	    }
	}
    }
  else {
    if (!(pathlength==-0.1)) {
      std::cout << "Warning: pathlength through target is negativ: "
		<< pathlength*10 << " mm !\n";
    }
  }
}

void 
cryo_ewald_2014::EnergyLossSimChamber(Particle& P)
{
  //bss halton
  // 32: Landau Mylar
  // 33: Landau Mylar
  // 34: external Mylar
  // 35: Landau Kevlar
  // 36: Landau Kevlar
  // 37: external Kevlar
  // 38: Landau air
  // 39: Landau air
  // 40: external air
  // 41: Landau Mylar
  // 42: Landau Mylar
  // 43: external Mylar

  double old_momentum;
  static double my1=Scat_Exit_Window_thickness/10;
  static double kev=AramidWindowThickness/10;
  static double air=Air_between_Scat_Spec_thickness/10;
  static double my2=Spec_Entrance_Window_thickness/10;  
  static int exRadWindow = ((int)rundb.Target.externalRadiation) & 0x4;

  old_momentum = P.momentum();
  if (P==P_electron) {
    LandauLoss(P, Mylar, my1, halton[32], halton[33]);
    if (exRadWindow) externalBremsstrahlung(P, Mylar, my1, halton[34]); 
  } else   P += Mylar->dEdx(P, my1);
  Mylar->MultiScat(P, old_momentum, my1);

  old_momentum = P.momentum();
  if (P==P_electron) {
    LandauLoss(P, Kevlar, kev, halton[35], halton[36]);
    if (exRadWindow) externalBremsstrahlung(P, Kevlar, kev, halton[37]); 
  } else   P += Kevlar->dEdx(P, kev);
  Kevlar->MultiScat(P, old_momentum, kev);

  old_momentum = P.momentum();
  if (P==P_electron) {
    LandauLoss(P, Air,   air, halton[38], halton[39]);
    if (exRadWindow) externalBremsstrahlung(P, Air,   air, halton[40]); 
  } else P += Air->dEdx(P, air);
  Air->MultiScat(P, old_momentum, air);
    
  old_momentum = P.momentum();
  if (P==P_electron) {
    LandauLoss(P, Mylar, my2, halton[41], halton[42]);
    if (exRadWindow) externalBremsstrahlung(P, Mylar, my2, halton[43]);  
  } else P += Mylar->dEdx(P, my2);  
  Mylar->MultiScat(P, old_momentum, my2);

}

void 
cryo_ewald_2014::EnergyLossCorrChamber(Particle& P)
{
  P -= Mylar->dEdx(P, Spec_Entrance_Window_thickness/10);
  P -= Kevlar->dEdx(P, AramidWindowThickness/10);
  P -= Air->dEdx(P, Air_between_Scat_Spec_thickness/10);
  P -= Mylar->dEdx(P, Scat_Exit_Window_thickness/10);
}

//########## end cryo isr_2013 ###########

int 
calib_water::setPara(double, double angle, double, double, double, double)
{
  angle_tar = angle * M_PI/180;  
  
  Length = totallength / cos(angle_tar);
  WallThickness = Wall / cos(angle_tar);
  
  return 1;
}

int 
calib_water::Generate_Vertex(double random[], double x[], double /*wob_x*/, double /*wob_y*/, modeltype ModelType)
{
    x[0] = rundb.sim.wobx * cos(random[0] * M_PI) + rundb.beam.offset.x - rundb.Target.offset_sim.x;
    x[1] = rundb.sim.woby * cos(random[1] * M_PI) + rundb.beam.offset.y - rundb.Target.offset_sim.y;
    x[2] = getLength() * (random[2] - .5);
  if (fabs(x[2]) > totallength / 2. / cos(angle_tar)) return 0;  
                                                 // not inside target  
  return 1; 
}

double 
calib_water::getLength_in_Target(double, double, double, double theta, double phi)
{
  double result;
  double dx[3] = {sin(theta+angle_tar) * cos(phi),          
		  sin(theta+angle_tar) * sin(phi),
		  cos(theta+angle_tar)};

  if (fabs(dx[1]) == 1.) return -1;               // straight up or down

  result = totallength / 2 / fabs(dx[2]);
  Length_in_Wall = WallThickness / fabs(dx[2]);

  return result;  
}

int 
helium::setPara(double len, double, double Dens, double, 
		double SnowThick, double SnowDensity)
{
  diameter = len;
  Length = diameter;

  double r = diameter / 2;

  // Helium-Cell before High-Pressure-Deformation 
  // in mm

  double Wall_before =  0.10000;
  double diam_before = 66.00000;

  Wall = sqrt(Wall_before*Wall_before + 2 * Wall_before * diam_before/2
         + r*r) - r;
  WallThickness=Wall;
    
  TargetMat->setDensity(Dens);

  if (SnowThick < 0) SnowThick = 0;

  SnowWall = SnowThick;            // Thickness in mm
  SnowThickness = SnowWall;

  if (SnowDensity != 0) Snow->setDensity(SnowDensity);   // Density in g/cm^3

  return 1;
}

int 
helium::Generate_Vertex(double random[], double x[], double /*wob_x*/, double /*wob_y*/, modeltype ModelType)
{
    x[0] = rundb.sim.wobx * cos(random[0] * M_PI) + rundb.beam.offset.x - rundb.Target.offset_sim.x;
    x[1] = rundb.sim.woby * cos(random[1] * M_PI) + rundb.beam.offset.y - rundb.Target.offset_sim.y;
    x[2] = getLength() * (random[2] - .5);
  double r = diameter / 2;
  
  if ((x[0]*x[0] + x[2]*x[2]) >= r*r) return 0;  // not inside targetcell
  return 1;
}

double 
helium::getLength_in_Target(double x, double, double z, double theta, double phi)
{
  double result, result2;
  double r = diameter / 2;
  double dx[3] = {sin(theta) * cos(phi),          
		  sin(theta) * sin(phi),
		  cos(theta)};
  
  if (fabs(dx[1]) == 1.) return -1;               // straight up or down
  if (fabs(x) > r) return -1;                     // not inside targetcell

  if ((x*x + z*z) >= r*r) z = sign(z)*(sqrt(r*r-x*x) - 0.05); // vertex z=edge
  
  double p   = 2 *(x * dx[0] + z * dx[2]);
  double q   = x*x + z*z - r*r;
  double p2q = p*p/4-q;

  double check1 = ((p2q < 0) ? 0 : - p/2 + sqrt(p2q));
  double check2 = ((p2q < 0) ? 0 : - p/2 - sqrt(p2q));
  
  double z1 = z + check1 * dx[2];
  double z2 = z + check2 * dx[2];

  // Wall !!!
  
  r += Wall;
  q = x*x + z*z - r*r;
  p2q = p*p/4-q;

  if (dx[2]>=0) {
    result  = (z1>z2 ? check1 : check2);
    result2 = (z1>z2 ? ((p2q < 0) ? 0 : - p/2 + sqrt(p2q)):
               	       ((p2q < 0) ? 0 : - p/2 - sqrt(p2q)));
    Length_in_Wall = result2 - result;
  }
  else {
    result  = (z1<z2 ? check1 : check2);
    result2 = (z1<z2 ? ((p2q < 0) ? 0 : - p/2 + sqrt(p2q)):
	               ((p2q < 0) ? 0 : - p/2 - sqrt(p2q)));
    Length_in_Wall = result2 - result;
  }

  // Snow !!!

  r += SnowWall;
  q = x*x + z*z - r*r;
  p2q = p*p/4-q;

  check1 = ((p2q < 0) ? 0 : - p/2 + sqrt(p2q));
  check2 = ((p2q < 0) ? 0 : - p/2 - sqrt(p2q));
  
  z1 = z + check1 * dx[2];
  z2 = z + check2 * dx[2];

  if (dx[2]>=0) {
    result2  = (z1>z2 ? check1 : check2);
    Length_through_Snow = result2 - result - Length_in_Wall;
  }
  else {
    result2  = (z1<z2 ? check1 : check2);
    Length_through_Snow = result2 - result - Length_in_Wall;
  }
    
  return result;
}

//#####Helium_new#####

int 
helium_new::setPara(double, double, double Dens, double WallThick, 
		    double SnowThick, double SnowDensity)
{
  WallThick = 0.25 ;   // normalerweise nicht deklariert, daher = 0, Eh.: mm
  Wall = WallThick;

  Length = diameter;

  WallThickness=Wall;
   
  TargetMat->setDensity(Dens);

  if (SnowThick < 0) SnowThick = 0;
  
  SnowWall = SnowThick;            // Thickness in mm
  SnowThickness = SnowWall;
  
  if (SnowDensity != 0) Snow->setDensity(SnowDensity);   // Density in g/cm^3
    
  return 1;
}

int 
helium_new::Generate_Vertex(double random[], double x[], double /*wob_x*/, double /*wob_y*/, modeltype ModelType)
{
    x[0] = rundb.sim.wobx * cos(random[0] * M_PI) + rundb.beam.offset.x - rundb.Target.offset_sim.x;
    x[1] = rundb.sim.woby * cos(random[1] * M_PI) + rundb.beam.offset.y - rundb.Target.offset_sim.y;
    x[2] = getLength() * (random[2] - .5);
  double r = diameter / 2;
  
  if ((x[0]*x[0] + x[2]*x[2]) >= r*r) return 0;  // not inside targetcell
  return 1;
}



double 
helium_new::getLength_in_Target(double x, double, double z, double theta, double phi)
{

  double result, result2;
  double r = diameter / 2;
  double dx[3] = {sin(theta) * cos(phi),          
		  sin(theta) * sin(phi),
		  cos(theta)};
  
  if (fabs(dx[1]) == 1.) return -1;               // straight up or down
  if (fabs(x) > r)   return -1;                   //not inside targetcell
  
 
  if ((x*x + z*z) >= r*r){
    z = sign(z)*(sqrt(r*r-x*x)-0.05) ;
  }


  double p   = 2 *(x * dx[0] + z * dx[2]);
  double q   = x*x + z*z - r*r;
  double p2q = p*p/4-q;

  double check1 = ((p2q < 0) ? 0 : - p/2 + sqrt(p2q));
  double check2 = ((p2q < 0) ? 0 : - p/2 - sqrt(p2q));
  
  double z1 = z + check1 * dx[2];
  double z2 = z + check2 * dx[2];
 
  // Wall !!!
 
  r += Wall;
  q = x*x + z*z - r*r;
  p2q = p*p/4-q;

  if (dx[2]>=0) {
    result  = (z1>z2 ? check1 : check2);
    result2 = (z1>z2 ? ((p2q < 0) ? 0 : - p/2 + sqrt(p2q)):
               	       ((p2q < 0) ? 0 : - p/2 - sqrt(p2q)));
    Length_in_Wall = result2 - result;
  }
  else {
    result  = (z1<z2 ? check1 : check2);
    result2 = (z1<z2 ? ((p2q < 0) ? 0 : - p/2 + sqrt(p2q)):
	               ((p2q < 0) ? 0 : - p/2 - sqrt(p2q)));
    Length_in_Wall = result2 - result;
  }
  

  //Length_in_Wall: vertical Correction
  
   double vec_r0[3] = {sin(theta) * cos(phi) ,          
		  0,
		  cos(theta) };
  
   double vec_r[3] = {sin(theta) * cos(phi),          
		  sin(theta) * sin(phi),
		  cos(theta) };

   double length_r, length_r0, scalar_prod ;
   long double h ;


   length_r = sqrt((vec_r[0]*vec_r[0])+ (vec_r[1]*vec_r[1]) + (vec_r[2]*vec_r[2])) ;
   length_r0 = sqrt((vec_r0[0]*vec_r0[0])+ (vec_r0[1]*vec_r0[1]) + (vec_r0[2]*vec_r0[2])) ;
   
   scalar_prod = (vec_r[0]*vec_r0[0])+(vec_r[1]*vec_r0[1]) + (vec_r[2]*vec_r0[2]) ;
   
   // cos(alpha) = <r,r0> / |r|*|r0| 

   h = (scalar_prod / (length_r * length_r0)) ;
   Length_in_Wall /= h ;
   
   
   if(Length_in_Wall > 1){
     Length_in_Wall = 0.25 ;
   };    

  //a_angle = (float)rundb.A.angle ;    
  //b_angle = (float)rundb.B.angle ;
  
  // Snow !!!

  r += SnowWall;
  q = x*x + z*z - r*r;
  p2q = p*p/4-q;

  check1 = ((p2q < 0) ? 0 : - p/2 + sqrt(p2q));
  check2 = ((p2q < 0) ? 0 : - p/2 - sqrt(p2q));
  
  z1 = z + check1 * dx[2];
  z2 = z + check2 * dx[2];

  if (dx[2]>=0) {
    result2  = (z1>z2 ? check1 : check2);
    Length_through_Snow = result2 - result - Length_in_Wall;
  }
  else {
    result2  = (z1<z2 ? check1 : check2);
    Length_through_Snow = result2 - result - Length_in_Wall;
  } 
  return result;
}


void 
helium_new::EnergyLossSim(Particle& P, double x, double y, double z, int steps)
{
  x -= rundb.Target.offset_sim.x; //bss: this function is called from deep inside of simDetectorBase or so, it gets targetpos_hall coords!!! here we wanna have targetpos_tar coords for ELOSS in target!
  y -= rundb.Target.offset_sim.y; 
  z -= rundb.Target.offset_sim.z; 

  double old_momentum;
  double pathlength = getLength_in_Target(x, y, z, P.theta(), P.phi()) / 10;
  double WallPath = Length_in_Wall/10;
  double SnowPath = Length_through_Snow/10;

  if (!(pathlength < 0)) {
    if (P == P_electron) {

      static int exRadWall = ((int)rundb.Target.externalRadiation) & 0x1;
      static int exRadTar  = ((int)rundb.Target.externalRadiation) & 0x2;
      static int exRadSnow = ((int)rundb.Target.externalRadiation) & 0x8;
      static int intRad    =  (int)rundb.Target.internalRadiation;
      
      //static double equivradTgt = 
      //  equivalentRadiator(TargetMat, rundb.sim.q2);
   
      old_momentum = P.momentum();
      LandauLoss(P, TargetMat, pathlength, halton[0], halton[1]);

      if (intRad) internalBremsstrahlung(P, TargetMat,halton[2],rundb.sim.q2); 
      if (exRadTar) externalBremsstrahlung(P, TargetMat,pathlength,halton[3]);
      
      TargetMat->MultiScat(P, old_momentum, pathlength);

      if (WallPath>0) {
	old_momentum = P.momentum();
	LandauLoss(P, WallMat, WallPath, halton[4], halton[5]);
	if (exRadWall) externalBremsstrahlung(P, WallMat, WallPath, halton[6]);
	WallMat->MultiScat(P, old_momentum, WallPath);
      }
      
      if (SnowPath>0) {
   	old_momentum = P.momentum();
	LandauLoss(P, Snow, SnowPath, halton[7], halton[8]);
	if (exRadSnow) externalBremsstrahlung(P, Snow, SnowPath, halton[9]);
	Snow->MultiScat(P, old_momentum, SnowPath);
      }
    }
    else {
      for (int iii=0; iii<steps; iii++) {
	old_momentum = P.momentum();
	P += TargetMat->dEdx(P, pathlength/steps);
	TargetMat->MultiScat(P, old_momentum, pathlength/steps);
      }
      
      if (WallPath>0) {
	old_momentum = P.momentum();
	P += WallMat->dEdx(P, WallPath);
	WallMat->MultiScat(P, old_momentum, WallPath);
      }
      
      if (SnowPath>0) {
	for (int iii=0; iii<steps; iii++) {
	  old_momentum = P.momentum();
	  P += Snow->dEdx(P, SnowPath/steps);
	  Snow->MultiScat(P, old_momentum, SnowPath/steps);
	}
      }
    } 
  }
  else {
    if (!(pathlength==-0.1)) {
      std::cout << "Warning: pathlength through target is negativ: "
      << pathlength*10 << " mm !\n";
    }
  }
}

void 
helium_new::EnergyLossSimBeam(Particle& P,double x, double y, double z, int)
{
  double old_momentum;
  double pathlength = getLength_in_Target(x, y, z, M_PI, 0.) / 10;
  double WallPath = Length_in_Wall/10;

  static int exRadWall = ((int)rundb.Target.externalRadiation) & 0x1;
  static int exRadTar  = ((int)rundb.Target.externalRadiation) & 0x2;
  // static int exRadWin  = ((int)rundb.Target.externalRadiation) & 0x4;
  static int intRad    =  (int)rundb.Target.internalRadiation;

  if (!(pathlength < 0)) {
    
    if (WallPath>0) {
      old_momentum = P.momentum();
      LandauLoss(P, WallMat, WallPath, halton[10], halton[11]);
      if (exRadWall) externalBremsstrahlung(P, WallMat, WallPath, halton[12]);
      WallMat->MultiScat(P, old_momentum, WallPath);
    }
    
    //static double equivradTarget =
    //  equivalentRadiator(TargetMat, rundb.sim.q2);

    old_momentum = P.momentum();
    LandauLoss(P, TargetMat, pathlength, halton[13], halton[14]);

    if (intRad) internalBremsstrahlung(P, TargetMat, halton[15],rundb.sim.q2); 
    if (exRadTar) externalBremsstrahlung(P, TargetMat, pathlength, halton[16]);

    TargetMat->MultiScat(P, old_momentum, pathlength);
  } 
  else {
    if (!(pathlength==-0.1)) {
      std::cout << "Warning: pathlength through target is negativ: "
      		<< pathlength*10 << " mm !\n";
    }
  }
}

void 
helium_new::EnergyLossCorr(Particle& P, double x, double y, double z, int steps)
{
  double pathlength = getLength_in_Target(x, y, z, P.theta(), P.phi()) / 10;
  double SnowPath = Length_through_Snow/10;

  if (!(pathlength < 0)) {
    if (SnowPath>0) {
      for (int iii=0; iii<steps; iii++) {
	P -= Snow->dEdx(P, SnowPath/steps);
      }
    }
    if (WallMat) P -= WallMat->dEdx(P, Length_in_Wall/10);
    if (TargetMat) {
      for (int iii=0; iii<steps; iii++) {
	P -= TargetMat->dEdx(P, pathlength/steps);
      }
    }
  }
  else {
    if (!(pathlength==-0.1)) {
      std::cout << "Warning: pathlength through target is negativ: "
		<< pathlength*10 << " mm !\n";
    }
  }
}

void 
helium_new::EnergyLossCorrBeam(Particle& P,double x, double y, double z, int steps)
{
  double pathlength = getLength_in_Target(x, y, z, M_PI, 0.) / 10;

  if (!(pathlength < 0)) {
    if (TargetMat) { 
      for (int iii=0; iii<steps; iii++) {
	P += TargetMat->dEdx(P, pathlength/steps);
      }
    }
  }
  else {
    if (!(pathlength==-0.1)) {
      std::cout << "Warning: pathlength through target is negativ: "
		<< pathlength*10 << " mm !\n";
    }
  }
  if (WallMat) 
    P += WallMat->dEdx(P, Length_in_Wall/10);
}

//##### Ende Helium_new#####

int 
helium_pol::setPara(double, double, double density, double wallthck,
		    double, double)
{
  if (density>0) TargetMat->setDensity(density);
  if (wallthck>0) WallThickness = wallthck;

  SnowThickness = 0;

  return 1;
}

int 
helium_pol::Generate_Vertex(double random[], double x[], double /*wob_x*/, double /*wob_y*/, modeltype ModelType)
{
    x[0] = rundb.sim.wobx * cos(random[0] * M_PI) + rundb.beam.offset.x - rundb.Target.offset_sim.x;
    x[1] = rundb.sim.woby * cos(random[1] * M_PI) + rundb.beam.offset.y - rundb.Target.offset_sim.y;
    x[2] = getLength() * (random[2] - .5);
  double r = diameter / 2;
  
  if ((x[0]*x[0] + x[1]*x[1] + x[2]*x[2]) >= r*r)
    return 0;  // not inside targetcell

  return 1;
}


double 
helium_pol::getLength_in_Target(double, double, double z,
				double theta, double)
{
  // first version simulates spherical target, only.
  // out-of-plane angle is ignored

  double r = diameter / 2;

  if (fabs(z) > r) return -1;                     // not inside targetcell

  double zsin = z * sin(theta);

  return - z * cos(theta) + sqrt(r*r - zsin*zsin);
}


void 
helium_pol::EnergyLossCorrBeam(Particle& P,
			       double, double, double z, int steps)
{
  double pathlength = 10.0 + z / 10;

  P += Be->dEdx(P, 0.1*BeThickness);
  P += Cu->dEdx(P, 0.1*CuThickness);

  if (pathlength > 0.0) {
    if (TargetMat) { 
      for (int iii=0; iii<steps; iii++)
	P += TargetMat->dEdx(P, pathlength/steps);
    }
  }
}

void helium_pol::EnergyLossSimBeam(Particle& P,
				   double x, double y, double z, int, modeltype ModelType)
{
  double old_momentum;
  double pathlength = 10.0 + z / 10;

  static int exRadWall = ((int)rundb.Target.externalRadiation) & 0x1;
  static int exRadTar  = ((int)rundb.Target.externalRadiation) & 0x2;
  static int intRad    =  (int)rundb.Target.internalRadiation;

  if (!(pathlength < 0)) {

    // simulate copper-window, only
    old_momentum = P.momentum();
    //    P += Cu->dEdx(P, 0.1*CuThickness);   Not needed: LandauLoss corrects for most probable loss.
    LandauLoss(P, Cu, 0.1*CuThickness, halton[10], halton[11]);
    if (exRadWall) externalBremsstrahlung(P, Cu, 0.1*CuThickness, halton[12]);
    WallMat->MultiScat(P, old_momentum, 0.1*CuThickness);
    
    old_momentum = P.momentum();
    //P+=TargetMat->dEdx(P,pathlength);   Not needed: LandauLoss corrects for most probable loss.
    LandauLoss(P, TargetMat, pathlength, halton[13], halton[14]);

    if (intRad) internalBremsstrahlung(P, TargetMat, halton[15],rundb.sim.q2); 
    if (exRadTar) externalBremsstrahlung(P, TargetMat, pathlength, halton[16]);

    TargetMat->MultiScat(P, old_momentum, pathlength);
  } else {
    if (!(pathlength==-0.1)) {
      std::cout << "Warning: pathlength through target is negativ: "
		<< pathlength*10 << " mm !\n";
    }
  }
}

void helium_pol::EnergyLossSimChamber(Particle& P)
{
  double old_momentum;

  static double air=40.0;
  static double my2=Spec_Entrance_Window_thickness/10;  
  static int exRadWindow = ((int)rundb.Target.externalRadiation) & 0x4;

  old_momentum = P.momentum();
  if(P==P_electron) {
    LandauLoss(P, Air,   air, halton[20], halton[21]);
    if (exRadWindow) externalBremsstrahlung(P, Air,   air, halton[22]); 
  }else P+= Air->dEdx(P, air);
  Air->MultiScat(P, old_momentum, air);
    
  old_momentum = P.momentum();
  if (P==P_electron) {    
    LandauLoss(P, Mylar, my2, halton[23], halton[24]);
    if (exRadWindow) externalBremsstrahlung(P, Mylar, my2, halton[25]); }
  else P += Mylar->dEdx(P, my2);  
  Mylar->MultiScat(P, old_momentum, my2);



}

void 
helium_pol::EnergyLossCorrChamber(Particle& P)
{
  P -= Air->dEdx(P, 40.0);
  P -= Mylar->dEdx(P, Scat_Exit_Window_thickness/10);
}
//!!!
int 
he_pol_07::setPara(double, double, double density, double wallthck,
		    double, double)
{
  if (density>0) TargetMat->setDensity(density);
  if (wallthck>0) WallThickness = wallthck;

  SnowThickness = 0;

  return 1;
}

int 
he_pol_07::Generate_Vertex(double random[], double x[], double /*wob_x*/, double /*wob_y*/, modeltype ModelType)
{
    x[0] = rundb.sim.wobx * cos(random[0] * M_PI) + rundb.beam.offset.x - rundb.Target.offset_sim.x;
    x[1] = rundb.sim.woby * cos(random[1] * M_PI) + rundb.beam.offset.y - rundb.Target.offset_sim.y;
    x[2] = getLength() * (random[2] - .5);
  double r = diameter / 2;
  
  if ((x[0]*x[0] + x[1]*x[1] + x[2]*x[2]) >= r*r)
    return 0;  // not inside targetcell

  return 1;
}


double 
he_pol_07::getLength_in_Target(double x, double y, double z,
			       double theta, double phi)
{
  // first version simulates spherical target, only.
  // out-of-plane angle is ignored

  double r = diameter / 2;

  if (fabs(z) > r) return -1;                     // not inside targetcell

  double dx = sin(theta) * cos(phi);
  double dy = sin(theta) * sin(phi);
  double dz = cos(theta);

  double 
    t1 = -(x*dx+y*dy+z*dz-
	   sqrt((x*dx+y*dy+z*dz)*(x*dx+y*dy+z*dz)-(x*x+y*y+z*z-r*r))),
    wx = x + t1*dx,
    wy = y + t1*dy,
    wz = z + t1*dz,
    thetawall = acos(wz/sqrt(wx*wx+wy*wy+wz*wz)) / M_PI * 180,
    walladd = 1;
  
  if (thetawall>wmin && thetawall<wmax && fileopen) 
    walladd =  wallcorr[(int)((thetawall-wmin)*wbins/((wmax-wmin)*1.0))];

  double
    wall = r + rundb.Target.wallthickness,// * walladd,
    t2 = -(x*dx+y*dy+z*dz-
	   sqrt((x*dx+y*dy+z*dz)*(x*dx+y*dy+z*dz)-
		(x*x+y*y+z*z-wall*wall)));
  
  Length_in_Wall = (t2 - t1)*walladd;
  
  return sqrt((x+dx*t1)*(x+dx*t1)+(y+dy*t1)*(y+dy*t1)+(z+dz*t1)*(z+dz*t1));
}


void 
he_pol_07::EnergyLossCorrBeam(Particle& P,
			       double, double, double z, int steps)
{
  double pathlength = 12.5 + z / 10;

  P += Be->dEdx(P, 0.1*BeThickness);
  P += Al->dEdx(P, 0.1*AlThickness);

  if (pathlength > 0.0) {
    if (TargetMat) { 
      for (int iii=0; iii<steps; iii++)
	P += TargetMat->dEdx(P, pathlength/steps);
    }
  }
}

void 
he_pol_07::EnergyLossSimBeam(Particle& P,
			     double x, double y, double z, int, modeltype ModelType)
{
  double old_momentum;
  double pathlength = 12.5 + z / 10;

  static int exRadWall = ((int)rundb.Target.externalRadiation) & 0x1;
  static int exRadTar  = ((int)rundb.Target.externalRadiation) & 0x2;
  static int intRad    =  (int)rundb.Target.internalRadiation;

  if (!(pathlength < 0)) {

    // simulate Al-window, only
    old_momentum = P.momentum();
    //    P += Cu->dEdx(P, 0.1*CuThickness);   Not needed: LandauLoss corrects for most probable loss.
    LandauLoss(P, Al, 0.1*AlThickness, halton[10], halton[11]);
    if (exRadWall) externalBremsstrahlung(P, Al, 0.1*AlThickness, halton[12]);
    WallMat->MultiScat(P, old_momentum, 0.1*AlThickness);
    
    old_momentum = P.momentum();
    //P+=TargetMat->dEdx(P,pathlength);   Not needed: LandauLoss corrects for most probable loss.
    LandauLoss(P, TargetMat, pathlength, halton[13], halton[14]);

    if (intRad) internalBremsstrahlung(P, TargetMat, halton[15],rundb.sim.q2); 
    if (exRadTar) externalBremsstrahlung(P, TargetMat, pathlength, halton[16]);

    TargetMat->MultiScat(P, old_momentum, pathlength);
  } else {
    if (!(pathlength==-0.1)) {
      std::cout << "Warning: pathlength through target is negativ: "
		<< pathlength*10 << " mm !\n";
    }
  }
}

void 
he_pol_07::EnergyLossSimChamber(Particle& P)
{
  double old_momentum;

  static double air=40.0;
  static double my2=Spec_Entrance_Window_thickness/10;  
  static int exRadWindow = ((int)rundb.Target.externalRadiation) & 0x4;

  old_momentum = P.momentum();
  if(P==P_electron) {
    LandauLoss(P, Air,   air, halton[20], halton[21]);
    if (exRadWindow) externalBremsstrahlung(P, Air,   air, halton[22]); 
  }else P+= Air->dEdx(P, air);
  Air->MultiScat(P, old_momentum, air);
    
  old_momentum = P.momentum();
  if (P==P_electron) {    
    LandauLoss(P, Mylar, my2, halton[23], halton[24]);
    if (exRadWindow) externalBremsstrahlung(P, Mylar, my2, halton[25]); }
  else P += Mylar->dEdx(P, my2);  
  Mylar->MultiScat(P, old_momentum, my2);
}

void 
he_pol_07::EnergyLossCorrChamber(Particle& P)
{
  P -= Air->dEdx(P, 40.0);
  P -= Mylar->dEdx(P, Scat_Exit_Window_thickness/10);
}
//!!!
int 
solidstate::setPara(double len, double angle, double, double, double, double)
{
  totallength = len / 100 / TargetMat->getDensity(); //len [mg/cm**2] -> totallength [mm]

  angle_tar = angle * M_PI/180;  
  
  Length = totallength / cos(angle_tar);
  
  return 1;
}

int 
solidstate::Generate_Vertex(double random[], double x[], double /*wob_x*/, double /*wob_y*/, modeltype ModelType)
{
    x[0] = rundb.sim.wobx * cos(random[0] * M_PI) + rundb.beam.offset.x - rundb.Target.offset_sim.x;
    x[1] = rundb.sim.woby * cos(random[1] * M_PI) + rundb.beam.offset.y - rundb.Target.offset_sim.y;
    if (rundb.Target.vertexdistribution != 0) 
      x[2] = getLength() * (random[2] - .5);

    if (fabs(x[2]) > totallength / 2. / cos(angle_tar)) 
      return 0; // not inside target  
  return 1; 
}

double 
solidstate::getLength_in_Target(double x, double y, double z, double theta, double phi)
{
  double result;
  // phi around 0    : Side of Spec B/C
  // phi around +-pi : Side of Spec A
  double side = ( fabs(phi) < M_PI/2 ? 1.0 : -1.0 );
  double inv_dx[3] = {sin(theta+angle_tar*side) * cos(phi),          
		      sin(theta+angle_tar*side) * sin(phi),
		      cos(theta+angle_tar*side)};

  if (fabs(inv_dx[1]) == 1.) return -1;               // straight up or down

  double zbar = Length / 2. - z;

  double thetabar = theta+angle_tar*side; 
  if (thetabar > M_PI/2) zbar = Length / 2. + z;

  double length = zbar * fabs(cos(angle_tar)) / fabs(cos(thetabar));
  if (length > totalwidth) result = totalwidth;
  else result = fabs(length);

  // the original code did not take any vertex distribution into account
  // if rundb.Target.vertexresolution is not set, the original code is used
  if (rundb.Target.vertexdistribution == 0) 
    result = totallength / 2 / fabs(inv_dx[2]);

  return result;  
}

double solidstate::getangle ( )
{
  return angle_tar;
}

int 
gasjet::setPara(double len, double, double density, double, double, double)
{
  //TargetMat->setDensity(density); // [g/cm**3]
  //totallength = len; // [mm]
  //Length = totallength; //historically left over from Length = totallength / cos(angle_tar), where angle_tar flew out..
  //cout << "density " << density << " g/cm**3\n";
  //cout << "length=" << totallength <<" mm\n";
  //cout << "target length=" << totallength/10. *density <<" g/cm**2\n";
  return 1;
}

int 
gasjet::Generate_Vertex(double random[], double x[], double /*wob_x*/, double /*wob_y*/, modeltype ModelType)
{
  //returns targetpos_tar 
  x[0] = rundb.sim.wobx * cos(random[0] * M_PI) + rundb.beam.offset.x - rundb.Target.offset_sim.x + random[3] * gasjet::totalwidth;
  x[1] = rundb.sim.woby * cos(random[1] * M_PI) + rundb.beam.offset.y - rundb.Target.offset_sim.y + random[4] * gasjet::totalheight;
  x[2] = gasjet::totallength * (random[2]); //homogeneous

  return 1; 
}

double 
gasjet::GaussianCDF(double x)
{
  // http://people.math.sfu.ca/~cbm/aands/page_932.htm
  // Fomula 26.2.17
  double tempx = x;
  x = abs(x);
  double b1=0.319381530, b2=-0.356563782, b3=1.781477937, b4=-1.821255978, b5=1.330274429;
  double phix = 1/sqrt(2*M_PI)*exp(-0.5*x*x);
  double t=1./(1+0.2316419*x);
  double cdfx = phix*(b1*t+b2*t*t+b3*pow(t,3)+b4*pow(t,4)+b5*pow(t,5));
  if (tempx>0)
    return 1-cdfx;
  else
    return cdfx;
}

double 
gasjet::getLength_in_Target(double x, double y, double z, double theta, double phi)
{
  // x,y,z are targetpos_tar

  // The actual length is an integration of the target density
  // The target has 2D Gaussian distribution in x-z plane with the same variable stored in totallength
  // The target thickness is invariant in y direction
  //
  // Absolute distance in x-z plane
  double distance_to_center, depth_in_target;
  //circle in x-z plane, y does not change thickness
  if (theta==0){ //incoming
    // actuall z and x if phi=0
    distance_to_center = x;
    depth_in_target = z;
  }
  else{ // outgoing
    double direction_x=sin(theta), direction_z=cos(theta);
    distance_to_center = -direction_x*z + direction_z*x;
    depth_in_target = direction_x*x+direction_z*z;
  }

  //normalized to std
  distance_to_center/=gasjet::totallength;
  depth_in_target=depth_in_target/gasjet::totallength;

  double thickness = exp(-pow(distance_to_center,2)/2) * gasjet::GaussianCDF(-depth_in_target);//from here to +inf = from -inf to here

  //thickness adjustion from y
  return thickness/abs(cos(phi));
}

void gasjet::EnergyLossSim(Particle& P, double x, double y, double z, int steps, modeltype Modeltype)// energy loss simulation for the simulation from the vertex point to the end of the cell
{
  // x,y,z are targetpos_tar
  double pathlength = getLength_in_Target(x, y, z, P.theta(), P.phi());

  double deltaE = gasjet::TargetMat->dEdx(P, pathlength);
  std::cout << "Energy loss = " << deltaE << std::endl;
  P += deltaE;

  return;
}
  
void gasjet::EnergyLossSimBeam(Particle& P, double x, double y, double z, int steps, modeltype Modeltype)// energy loss simulation for the simulation from the vertex point to beam entrance point of the cell
{
  // x,y,z are targetpos_tar
  double pathlength = getLength_in_Target(x, y, z, P.theta(), P.phi());

  double deltaE = gasjet::TargetMat->dEdx(P, pathlength);
  P += deltaE;

  std::cout << "Energy loss = " << deltaE << std::endl;

  return;
}

int 
windowlesstube::setPara(double len, double, double density, double, double, double)
{
  TargetMat->setDensity(density); // [g/cm**3]
  totallength = len; // [mm]
  Length = totallength; //historically left over from Length = totallength / cos(angle_tar), where angle_tar flew out..
  cout << "density " << density << " g/cm**3\n";
  cout << "length=" << totallength <<" mm\n";
  cout << "target length=" << totallength/10. *density <<" g/cm**2\n";
  return 1;
}

int 
windowlesstube::Generate_Vertex(double random[], double x[], double /*wob_x*/, double /*wob_y*/, modeltype ModelType)
{
    x[0] = rundb.sim.wobx * cos(random[0] * M_PI) + rundb.beam.offset.x - rundb.Target.offset_sim.x;
    x[1] = rundb.sim.woby * cos(random[1] * M_PI) + rundb.beam.offset.y - rundb.Target.offset_sim.y;
    //x[2] = getLength() * (random[2] - .5); //homogeneous
    x[2] = getLength() * (random[2] < 0.5 ? sqrt(random[2]/2)-.5 : (0.5-sqrt((1-random[2])/2))); //triangular shape

    
    //cout << "length=" << totallength <<"\n";

    //if (fabs(x[2]) > totallength / 2.) 
    //      return 0; // not inside target  
  return 1; 
}

double 
windowlesstube::getLength_in_Target(double x, double y, double z, double theta, double phi)
{
  //this is not yet so good... improve when needed... assumes a flat foil target, which is wrong here, most of the time result would be cell diameter...7.5mm at the time -> set it to zero as long as not needed (very thin gas target..)
  return 0;
  double result;
  double inv_dx[3] = {sin(theta) * cos(phi),          
		      sin(theta) * sin(phi),
		      cos(theta)};

  if (fabs(inv_dx[1]) == 1.) return -1;               // straight up or down

  double zbar = Length / 2. - z;
  if (theta > M_PI/2) zbar = Length / 2. + z;

  double length = zbar / fabs(cos(theta));
  if (length > totalwidth)
    result = totalwidth;
  else
    result = fabs(length);
  cout << "length " << result << "\n";
  return result;  
}


int 
H2O::setPara(double len, double angle, double, double, double, double)
{
  totallength = len / 100 / TargetMat->getDensity();
  
  angle_tar = angle * M_PI/180;

  Length=totallength / cos(angle_tar);

  return 1;
}

int 
H2O::Generate_Vertex(double random[], double x[], double /*wob_x*/, double /*wob_y*/, modeltype ModelType)
{
    x[0] = rundb.sim.wobx * cos(random[0] * M_PI) + rundb.beam.offset.x - rundb.Target.offset_sim.x;
    x[1] = rundb.sim.woby * cos(random[1] * M_PI) + rundb.beam.offset.y - rundb.Target.offset_sim.y;
    x[2] = getLength() * (random[2] - .5);
  if (fabs(x[2]) > totallength / 2. / cos(angle_tar)) return 0; 
                                                // not inside target 
  return 1;
}

double 
H2O::getLength_in_Target(double x, double, double, double theta, double phi)
{
  double r = diameter / 2;
  double result, result2, result3;
  double dx[3] = {sin(theta+angle_tar) * cos(phi),          
		  sin(theta+angle_tar) * sin(phi),
		  cos(theta+angle_tar)};

  if (fabs(dx[1]) == 1.) return -1;               // straight up or down

  result = totallength / 2 / fabs(dx[2]);

  double p   = 2 * x * dx[0];
  double q   = x*x - r*r;
  double p2q = p*p/4-q;
 
  double check1 = ((p2q < 0) ? 0 : - p/2 + sqrt(p2q));
  double check2 = ((p2q < 0) ? 0 : - p/2 - sqrt(p2q));
  
  double z1 = check1 * dx[2];
  double z2 = check2 * dx[2];
  
  r += Wall;
  q = x*x - r*r;
  p2q = p*p/4-q;

  if (dx[2]>=0) {
    result2 = (z1>z2 ? check1 : check2);
    result3 = (z1>z2 ? ((p2q < 0) ? 0 : - p/2 + sqrt(p2q)):
               	       ((p2q < 0) ? 0 : - p/2 - sqrt(p2q)));
    Length_in_Wall = result3 - result2;
  }
  else {
    result2 = (z1<z2 ? check1 : check2);
    result3 = (z1<z2 ? ((p2q < 0) ? 0 : - p/2 + sqrt(p2q)):
	               ((p2q < 0) ? 0 : - p/2 - sqrt(p2q)));
    Length_in_Wall = result3 - result2;
  }
  
  return result;
}

void 
target::EnergyLossSim(Particle& P, double x, double y, double z, int steps, modeltype ModelType)
//cryo_cyl_2014::EnergyLossSim(Particle& P, double x, double y, double z, int steps, modeltype ModelType)

{
  x -= rundb.Target.offset_sim.x; //bss: this function is called from deep inside of simDetectorBase or so, it gets targetpos_hall coords!!! here we wanna have targetpos_tar coords for ELOSS in target!
  y -= rundb.Target.offset_sim.y; 
  z -= rundb.Target.offset_sim.z; 

  double old_momentum;
  double pathlength = getLength_in_Target(x, y, z, P.theta(), P.phi()) / 10;
  double WallPath = Length_in_Wall/10;
  double SnowPath = Length_through_Snow/10;
  //cout<< x << "\t" << z << "\t" << P.phi() << "\t" << P.theta() << "\t" << pathlength*10 << "\t" << pathlength<< "\t" <<pathlength*10 << "\t" <<endl;
  if (!(pathlength < 0)) {
    if (P == P_electron) {

      static int exRadWall = ((int)rundb.Target.externalRadiation) & 0x1;
      static int exRadTar  = ((int)rundb.Target.externalRadiation) & 0x2;
      static int exRadSnow = ((int)rundb.Target.externalRadiation) & 0x8;
      static int intRad    =  (int)rundb.Target.internalRadiation;
      
      //static double equivradTgt = 
      //  equivalentRadiator(TargetMat, rundb.sim.q2);
   
      old_momentum = P.momentum();
      LandauLoss(P, TargetMat, pathlength, halton[0], halton[1]);

      if (intRad) internalBremsstrahlung(P, TargetMat,halton[2],rundb.sim.q2); 
      if (exRadTar) externalBremsstrahlung(P, TargetMat,pathlength,halton[3]);
      
      TargetMat->MultiScat(P, old_momentum, pathlength);

      if (WallPath>0) {
	old_momentum = P.momentum();
	LandauLoss(P, WallMat, WallPath, halton[4], halton[5]);
	if (exRadWall) externalBremsstrahlung(P, WallMat, WallPath, halton[6]);
	WallMat->MultiScat(P, old_momentum, WallPath);
      }
      
      if (SnowPath>0) {
   	old_momentum = P.momentum();
	LandauLoss(P, Snow, SnowPath, halton[7], halton[8]);
	if (exRadSnow) externalBremsstrahlung(P, Snow, SnowPath, halton[9]);
	Snow->MultiScat(P, old_momentum, SnowPath);
      }
    }
    else {
      for (int iii=0; iii<steps; iii++) {
	old_momentum = P.momentum();
	P += TargetMat->dEdx(P, pathlength/steps);
	TargetMat->MultiScat(P, old_momentum, pathlength/steps);
      }
      
      if (WallPath>0) {
	old_momentum = P.momentum();
	P += WallMat->dEdx(P, WallPath);
	WallMat->MultiScat(P, old_momentum, WallPath);
      }
      
      if (SnowPath>0) {
	for (int iii=0; iii<steps; iii++) {
	  old_momentum = P.momentum();
	  P += Snow->dEdx(P, SnowPath/steps);
	  Snow->MultiScat(P, old_momentum, SnowPath/steps);
	}
      }
    } 
  }
  else {
    if (!(pathlength==-0.1)) {
      std::cout << "Warning: pathlength through target is negativ: "
		<< pathlength*10 << " mm !\n";
    }
  }
}

void 
target::EnergyLossSimBeam(Particle& P,double x, double y, double z, int, modeltype ModelType)

{
  double old_momentum;
  double pathlength = getLength_in_Target(x, y, z, M_PI, 0.) / 10;
  double WallPath = Length_in_Wall/10;
  
  static int exRadWall = ((int)rundb.Target.externalRadiation) & 0x1;
  static int exRadTar  = ((int)rundb.Target.externalRadiation) & 0x2;
  // static int exRadWin  = ((int)rundb.Target.externalRadiation) & 0x4;
  static int intRad    =  (int)rundb.Target.internalRadiation;

  if (!(pathlength < 0)) {
    
    if (WallPath>0) {
      old_momentum = P.momentum();
      LandauLoss(P, WallMat, WallPath, halton[10], halton[11]);
      if (exRadWall) externalBremsstrahlung(P, WallMat, WallPath, halton[12]);
      WallMat->MultiScat(P, old_momentum, WallPath);
    }
    
    //static double equivradTarget =
    //  equivalentRadiator(TargetMat, rundb.sim.q2);

    old_momentum = P.momentum();
    LandauLoss(P, TargetMat, pathlength, halton[13], halton[14]);

    if (intRad) internalBremsstrahlung(P, TargetMat, halton[15],rundb.sim.q2); 
    if (exRadTar) externalBremsstrahlung(P, TargetMat, pathlength, halton[16]);

    TargetMat->MultiScat(P, old_momentum, pathlength);
  } 
  else {
    if (!(pathlength==-0.1)) { //bss: why should it be -0.1??
      std::cout << "Warning: pathlength through target is negativ: "
      	<< pathlength*10 << " mm !\n";
    }
  }
}

void 
target::EnergyLossCorr(Particle& P, double x, double y, double z, int steps)

{
  double pathlength = getLength_in_Target(x, y, z, P.theta(), P.phi()) / 10;
  double SnowPath = Length_through_Snow/10;

  if (!(pathlength < 0)) {
    if (SnowPath>0) {
      for (int iii=0; iii<steps; iii++) {
	P -= Snow->dEdx(P, SnowPath/steps);
      }
    }
    if (WallMat) P -= WallMat->dEdx(P, Length_in_Wall/10);
    if (TargetMat) {
      for (int iii=0; iii<steps; iii++) {
	P -= TargetMat->dEdx(P, pathlength/steps);
      }
    }
  }
  else {
    if (!(pathlength==-0.1)) {
      std::cout << "Warning: pathlength through target is negativ: "
		<< pathlength*10 << " mm !\n";
    }
  }
}

void 
target::EnergyLossCorrBeam(Particle& P,double x, double y, double z, int steps)

{
  double pathlength = getLength_in_Target(x, y, z, M_PI, 0.) / 10;
 
  if (!(pathlength < 0)) {
    if (TargetMat) { 
      for (int iii=0; iii<steps; iii++) {
	P += TargetMat->dEdx(P, pathlength/steps);
      }
    }
  }
  else {
    if (!(pathlength==-0.1)) {
      std::cout << "Warning: pathlength through target is negativ: "
		<< pathlength*10 << " mm !\n";
    }
  }
  if (WallMat) 
    P += WallMat->dEdx(P, Length_in_Wall/10);
}

//########cryo 2014###########

void 
cryo_cyl_2014::EnergyLossSim(Particle& P, double x, double y, double z, int steps, modeltype ModelType)
{
  x -= rundb.Target.offset_sim.x; //bss: this function is called from deep inside of simDetectorBase or so, it gets targetpos_hall coords!!! here we wanna have targetpos_tar coords for ELOSS in target!
  y -= rundb.Target.offset_sim.y; 
  z -= rundb.Target.offset_sim.z; 

  //EnergyLoss after vertex
  double r = diameter/2;
  double old_momentum;
  //double pathlength = getLength_in_Target(x, y, z, P.theta(), P.phi()) / 10;
  double pathlength = getLength_in_Target(x, y, z, P.theta(), P.phi(), ModelType) / 10;
  //double WallPath = Length_in_Backwall/10;
  double BackWallPath = Length_in_Backwall/10;
  double SnowPath = Length_through_Snow/10;
  double SideWallPath = Distance_to_EndOfCell/10;
  double WallPath = getLength_in_Frontwall()/10; //Length in Wall
  //cout<< x << "\t" << z << "\t" << P.phi() << "\t" << P.theta() << "\t" << pathlength*10 << "\t" << pathlength<< "\t" <<pathlength*10 << "\t" <<endl;
  static int exRadWall = ((int)rundb.Target.externalRadiation) & 0x1;
  static int exRadTar  = ((int)rundb.Target.externalRadiation) & 0x2;
  static int exRadSnow = ((int)rundb.Target.externalRadiation) & 0x8;
  static int intRadWall    =  (int)rundb.Target.internalRadiation & 0x1;
  static int intRadTar  =  ((int)rundb.Target.internalRadiation) & 0x2; 
  static int intRadSnow  =  ((int)rundb.Target.internalRadiation) & 0x8; 

  if(ModelType == ElasticDeuteron || ModelType == ElasticProton){
  if (!(pathlength < 0)) {
    if (P == P_electron) {
      
      old_momentum = P.momentum();

      if (intRadTar) internalBremsstrahlung(P, TargetMat,halton[2],rundb.sim.q2); 
      LandauLoss(P, TargetMat, pathlength, halton[0], halton[1]);
      if (exRadTar) externalBremsstrahlung(P, TargetMat,pathlength,halton[3]);
      
      TargetMat->MultiScat(P, old_momentum, pathlength);

      if (BackWallPath>0){
	old_momentum = P.momentum();
	LandauLoss(P, WallMat, BackWallPath, halton[4], halton[5]);
	if (exRadWall) externalBremsstrahlung(P, WallMat, BackWallPath, halton[6]);
	WallMat->MultiScat(P, old_momentum, BackWallPath);
      }
      
      if (SnowPath>0) {
   	old_momentum = P.momentum();
	LandauLoss(P, Snow, SnowPath, halton[7], halton[8]);
	if (exRadSnow) externalBremsstrahlung(P, Snow, SnowPath, halton[9]);
	Snow->MultiScat(P, old_momentum, SnowPath);
      }
    }
    else {

      for (int iii=0; iii<steps; iii++) {
	old_momentum = P.momentum();
	P += TargetMat->dEdx(P, pathlength/steps);
	TargetMat->MultiScat(P, old_momentum, pathlength/steps);
      }
      if (BackWallPath>0) {
	for (int iii=0; iii<steps; iii++){
	  old_momentum = P.momentum();
	  P += WallMat->dEdx(P, BackWallPath/steps);
	  WallMat->MultiScat(P, old_momentum, BackWallPath/steps);
	}
      }
      if (SnowPath>0) {
	for (int iii=0; iii<steps; iii++) {
	  old_momentum = P.momentum();
	  P += Snow->dEdx(P, SnowPath/steps);
	  Snow->MultiScat(P, old_momentum, SnowPath/steps);
	}
      }
    } 
  }
  else {
    if (!(pathlength==-0.1)) {
      std::cout << "Warning: pathlength through target is negativ: "
		<< pathlength*10 << " mm !\n";
    }
  }
  }
  if( (ModelType == ElasticNucleus ||  ModelType == QuasiElasticNucleus || ModelType == ElasticHavar) && ((x*x+z*z) > r*r) && (x*x+z*z) <=(r+WallThickness)*(r+WallThickness)){// inside wall
    if((z < 0) & (fabs(x) <= 10.01)){//vertex in wall in front of deuterium //bss: bitwise AND operator is nonsense here!! should be & -> &&
      if (!(WallPath < 0)) {
	if (P == P_electron) {
	 
	  old_momentum = P.momentum();
	  if (intRadWall) internalBremsstrahlung(P, WallMat,halton[26],rundb.sim.q2);
	  LandauLoss(P, WallMat, WallPath, halton[0], halton[1]);
	  if (exRadWall) externalBremsstrahlung(P, WallMat, WallPath, halton[3]);
	  WallMat->MultiScat(P, old_momentum, WallPath);
	  
	  //after front wall electron goes through deuterium:
	  old_momentum = P.momentum(); //save momentum of the particle before doing the energyloss
	  LandauLoss(P, TargetMat, pathlength, halton[13], halton[14]);
	  if (exRadTar) externalBremsstrahlung(P, TargetMat, pathlength, halton[16]);
	  TargetMat->MultiScat(P, old_momentum, pathlength);
	  
	  //and then through the backwall:
	  old_momentum = P.momentum(); 
	  LandauLoss(P, WallMat, BackWallPath, halton[17], halton[18]); 
	  if (exRadWall) externalBremsstrahlung(P, WallMat, BackWallPath, halton[12]); 
	  WallMat->MultiScat(P, old_momentum, BackWallPath); 
	  
	  if (SnowPath>0) {//back snow
	    old_momentum = P.momentum();
	    LandauLoss(P, Snow, SnowPath, halton[7], halton[8]);
	    if (exRadSnow) externalBremsstrahlung(P, Snow, SnowPath, halton[9]);
	    Snow->MultiScat(P, old_momentum, SnowPath);
	  }//Schnee Ende
      	}//P-electron Ende
	else {
	  if (WallPath>0) {
	    for (int iii=0; iii<steps; iii++){
	      old_momentum = P.momentum();
	      P += WallMat->dEdx(P, WallPath/steps);
	      WallMat->MultiScat(P, old_momentum, WallPath/steps);
	    }
	  }
	  for (int iii=0; iii<steps; iii++) {
	    old_momentum = P.momentum();
	    P += TargetMat->dEdx(P, pathlength/steps);
	    TargetMat->MultiScat(P, old_momentum, pathlength/steps);
	  }
	  if (BackWallPath>0) {
	    for (int iii=0; iii<steps; iii++){
	      old_momentum = P.momentum();
	      P += WallMat->dEdx(P, BackWallPath/steps);
	      WallMat->MultiScat(P, old_momentum, BackWallPath/steps);
	    }
	  }
	  if (SnowPath>0) {
	    for (int iii=0; iii<steps; iii++) {
	      old_momentum = P.momentum();
	      P += Snow->dEdx(P, SnowPath/steps);
	      Snow->MultiScat(P, old_momentum, SnowPath/steps);
	    }
	  }
	}//else Ende
      }//WallPath Ende
      else {
	if (!(WallPath==-0.1)) {
        std::cout << "Warning(EnergyLossSim): BackWallpath through target is negativ: "
          	      << WallPath*10 << " mm !\n";
        }
      }//else Ende
      }//z<0 Ende
    if((z > 0) & (fabs(x) <= 10.01)){//vertex in wall behind deuterium //bss: bitwise AND operator is nonsense here!! should be & -> &&
       if (!(WallPath < 0)) {
	 if (P == P_electron) {
	     //through the backwall:
	     old_momentum = P.momentum(); 
	     if (intRadWall) internalBremsstrahlung(P, WallMat, halton[27],rundb.sim.q2);
	     LandauLoss(P, WallMat, BackWallPath, halton[17], halton[18]); 
	     if (exRadWall) externalBremsstrahlung(P, WallMat, BackWallPath, halton[12]); 
	     WallMat->MultiScat(P, old_momentum, BackWallPath); 
	          
	     if (SnowPath>0) {//back snow
	       old_momentum = P.momentum();
	       LandauLoss(P, Snow, SnowPath, halton[7], halton[8]);
	       if (exRadSnow) externalBremsstrahlung(P, Snow, SnowPath, halton[9]);
	       Snow->MultiScat(P, old_momentum, SnowPath);
	    }
	 }//P-Elektron Ende
	 else {
	  if (WallPath>0) {
	    for (int iii=0; iii<steps; iii++){
	      old_momentum = P.momentum();
	      P += WallMat->dEdx(P, WallPath/steps);
	      WallMat->MultiScat(P, old_momentum, WallPath/steps);
	    }
	  }
	  if (SnowPath>0) {
	    for (int iii=0; iii<steps; iii++) {
	      old_momentum = P.momentum();
	      P += Snow->dEdx(P, SnowPath/steps);
	      Snow->MultiScat(P, old_momentum, SnowPath/steps);
	    }
	   }
	  }
       }//WallPath<0 ende
       else {
        if (!(WallPath==-0.1)) {
          std::cout << "Warning(EnergyLossSim): BackWallpath through target is negativ: "
          	      << WallPath*10 << " mm !\n";
        }
       }
    }//Wallbehind deuterium ende

     if(fabs(x)>10){//Sidewall, path through deuterium = 0
      if (!(SideWallPath < 0)) { 
	if (P == P_electron) {
	
	  old_momentum = P.momentum();
	  if (intRadWall) internalBremsstrahlung(P, WallMat,halton[26],rundb.sim.q2); //should be the first energyloss after vertex
	  LandauLoss(P, WallMat, SideWallPath, halton[0], halton[1]);
	  if (exRadWall) externalBremsstrahlung(P, WallMat, SideWallPath, halton[3]);
	  WallMat->MultiScat(P, old_momentum, SideWallPath);

	  if (SnowPath>0) {//back snow
	    old_momentum = P.momentum();
	    LandauLoss(P, Snow, SnowPath, halton[7], halton[8]);
	    if (exRadSnow) externalBremsstrahlung(P, Snow, SnowPath, halton[9]);
	    Snow->MultiScat(P, old_momentum, SnowPath);
	  }
  	}
	else {
	  if (SideWallPath>0) {
	    for (int iii=0; iii<steps; iii++) {
	      old_momentum = P.momentum();
	      P += WallMat->dEdx(P, SideWallPath/steps);
	      WallMat->MultiScat(P, old_momentum, SideWallPath/steps);
	    }
	  }
  	  if (SnowPath>0) {
	    for (int iii=0; iii<steps; iii++) {
	      old_momentum = P.momentum();
	      P += Snow->dEdx(P, SnowPath/steps);
	      Snow->MultiScat(P, old_momentum, SnowPath/steps);
	    }
	  }
  	}
      }//sideWallPath <0 Ende
      else {
	if (!(SideWallPath==-0.1)) {
	   std::cout << "Warning: SideWallpath through target is negativ: "
	      << SideWallPath*10 << " mm !\n";
	 }
      } 
    }//SideWall Ende
    
  }//ElasticNucleus Ende
  
  //inside snow
  if (ModelType == ElasticCryogens && (x*x + z*z) > (r+WallThickness)*(r+WallThickness) && (x*x + z*z) <= (r+WallThickness+SnowThickness)*(r+WallThickness+SnowThickness)){
    if(z < 0 && fabs(x) <= 10+WallThickness){// ice in front ot deuterium
      if (!(SnowPath < 0)) {
	if (P == P_electron) {
	
	  old_momentum = P.momentum();
	  if (intRadSnow) internalBremsstrahlung(P, Snow,halton[26],rundb.sim.q2); //should be the first energyloss after vertex
	  LandauLoss(P, Snow, SnowPath, halton[0], halton[1]);
	  if (exRadSnow) externalBremsstrahlung(P, Snow,SnowPath,halton[3]);
	  Snow->MultiScat(P, old_momentum, SnowPath);
	
	  if (WallPath>0) {//front wall
	    old_momentum = P.momentum();
	    LandauLoss(P, WallMat, WallPath, halton[4], halton[5]);
	    if (exRadWall) externalBremsstrahlung(P, WallMat, WallPath, halton[6]);
	    WallMat->MultiScat(P, old_momentum, WallPath);
	  }

	  if (!(pathlength < 0)) {//deuterium after vertex
	    old_momentum = P.momentum();
	    LandauLoss(P, TargetMat, pathlength, halton[0], halton[1]);
	    if (exRadTar) externalBremsstrahlung(P, TargetMat,pathlength,halton[3]);
	    TargetMat->MultiScat(P, old_momentum, pathlength);
	  }
	  
	  if (BackWallPath>0) {//back wall
	    old_momentum = P.momentum();
	    LandauLoss(P, WallMat, BackWallPath, halton[4], halton[5]);
	    if (exRadWall) externalBremsstrahlung(P, WallMat, BackWallPath, halton[6]);
	    WallMat->MultiScat(P, old_momentum, BackWallPath);
	  }
      
	  if (SnowPath>0) {//back snow
	    old_momentum = P.momentum();
	    LandauLoss(P, Snow, SnowPath, halton[7], halton[8]);
	    if (exRadSnow) externalBremsstrahlung(P, Snow, SnowPath, halton[9]);
	    Snow->MultiScat(P, old_momentum, SnowPath);
	  }
	}
	else {
	  if (SnowPath>0) {
	    for (int iii=0; iii<steps; iii++) {
	      old_momentum = P.momentum();
	      P += Snow->dEdx(P, SnowPath/steps);
	      Snow->MultiScat(P, old_momentum, SnowPath/steps);
	    }
	  }
	  if (WallPath>0) {
	    for (int iii=0; iii<steps; iii++){
	       old_momentum = P.momentum();
	       P += WallMat->dEdx(P, WallPath/steps);
	       WallMat->MultiScat(P, old_momentum, WallPath/steps);
	    }
	  }
	  for (int iii=0; iii<steps; iii++) {
	    old_momentum = P.momentum();
	    P += TargetMat->dEdx(P, pathlength/steps);
	    TargetMat->MultiScat(P, old_momentum, pathlength/steps);
	  }
      
	  if (BackWallPath>0) {
	    for (int iii=0; iii<steps; iii++){
	       old_momentum = P.momentum();
	       P += WallMat->dEdx(P, BackWallPath/steps);
	       WallMat->MultiScat(P, old_momentum, BackWallPath/steps);
	    }
	  }
	
	  if (SnowPath>0) {
	    for (int iii=0; iii<steps; iii++) {
	      old_momentum = P.momentum();
	      P += Snow->dEdx(P, SnowPath/steps);
	      Snow->MultiScat(P, old_momentum, SnowPath/steps);
	    }
	  }
	} 
      }
      else {
	if (!(SnowPath==-0.1)) {
	  std::cout << "Warning: Snowpath through target is negativ: "
		    << SnowPath*10 << " mm !\n";
	}
      }
    }
    if(z >= 0 && fabs(x) <= 10+WallThickness){//ice behind deuterium
      if (!(SnowPath < 0)) {
	if (P == P_electron) {
	
	  old_momentum = P.momentum();
	  if (intRadSnow) internalBremsstrahlung(P, Snow,halton[26],rundb.sim.q2); //should be the first energyloss after vertex
	  LandauLoss(P, Snow, SnowPath, halton[0], halton[1]);
	  if (exRadSnow) externalBremsstrahlung(P, Snow, SnowPath, halton[3]);
	  Snow->MultiScat(P, old_momentum, SnowPath);
	}
	else {
	  if (SnowPath>0) {
	    for (int iii=0; iii<steps; iii++) {
	      old_momentum = P.momentum();
	      P += Snow->dEdx(P, SnowPath/steps);
	      Snow->MultiScat(P, old_momentum, SnowPath/steps);
	    }
	  }
	}
      }
      else {
	if (!(SnowPath==-0.1)) {
	  std::cout << "Warning: Snowpath through target is negativ: "
		    << SnowPath*10 << " mm !\n";
	}
      }
    }
    if(fabs(x) > 10+WallThickness){//ice on the sides
      if (!(SnowPath < 0)) {
	if (P == P_electron) {
	
	  old_momentum = P.momentum();
	  if (intRadSnow) internalBremsstrahlung(P, Snow,halton[26],rundb.sim.q2); //should be the first energyloss after vertex
	  LandauLoss(P, Snow, SnowPath, halton[0], halton[1]);
	  if (exRadSnow) externalBremsstrahlung(P, Snow, SnowPath, halton[3]);
	  Snow->MultiScat(P, old_momentum, SnowPath);

	}
	else {
	  if (SnowPath>0) {
	    for (int iii=0; iii<steps; iii++) {
	      old_momentum = P.momentum();
	      P += Snow->dEdx(P, SnowPath/steps);
	      Snow->MultiScat(P, old_momentum, SnowPath/steps);
	    }
	  }
	}
      }
      else {
	if (!(SnowPath==-0.1)) {
	  std::cout << "Warning: Snowpath through target is negativ: "
		    << SnowPath*10 << " mm !\n";
	}
      }
    }
  }
}

void 
cryo_cyl_2014::EnergyLossSimBeam(Particle& P,double x, double y, double z, int, modeltype ModelType)
{//energyloss before vertex
  double r = diameter/2;
  double old_momentum;
  double pathlength = getLength_in_Target(x, y, z, M_PI, 0., ModelType) / 10;
  double WallPath = fabs(getLength_in_Frontwall())/10; //Length in Wall
  double BackWallPath = getLength_in_Backwall()/10; //Length in Backwall
  double SideWallPath = Distance_to_BeginningOfCell/10; //for vertex in wall with fabs(x)>1, path through deuterium is then: 0;
  double SnowPath = Length_through_Snow/10;
  

  static int exRadSnow = ((int)rundb.Target.externalRadiation) & 0x8; //external Radiation in Snow
  static int exRadWall = ((int)rundb.Target.externalRadiation) & 0x1;
  static int exRadTar  = ((int)rundb.Target.externalRadiation) & 0x2;
  static int intRadWall =  ((int)rundb.Target.internalRadiation) & 0x1;        //internal Radiation in Wall
  static int intRadTar  =  ((int)rundb.Target.internalRadiation) & 0x2;        //internal Radiation in Target
  static int intRadSnow =  ((int)rundb.Target.internalRadiation) & 0x8;        //internal Radiation in Snow
  
  //decide if vertex is inside deuterium, wall or ice and use energy losses depending on which model was uses and where the vertex is
  //inside deuterium
  if (ModelType == ElasticDeuteron || ModelType == ElasticProton){
  if (!(pathlength < 0)) {//pathlength has to be positive when target material is hit, else you wouldn't be inside the targetcell
    
    if (SnowPath>0) {
   	old_momentum = P.momentum();  //save momentum of the particle before doing the energyloss
	LandauLoss(P, Snow, SnowPath, halton[7], halton[8]);
	if (exRadSnow) externalBremsstrahlung(P, Snow, SnowPath, halton[9]);
	Snow->MultiScat(P, old_momentum, SnowPath);
    }
    if (WallPath>0) {
      old_momentum = P.momentum();
      LandauLoss(P, WallMat, WallPath, halton[10], halton[11]);
      if (exRadWall) externalBremsstrahlung(P, WallMat, WallPath, halton[12]);
      WallMat->MultiScat(P, old_momentum, WallPath);
    }
    //=> particle lost energy and changed momentum direction in wall
    old_momentum = P.momentum();
    LandauLoss(P, TargetMat, pathlength, halton[13], halton[14]);

    if (intRadTar) internalBremsstrahlung(P, TargetMat, halton[15],rundb.sim.q2); 
    if (exRadTar) externalBremsstrahlung(P, TargetMat, pathlength, halton[16]);

    TargetMat->MultiScat(P, old_momentum, pathlength);
  } 
  else {
    if (!(pathlength==-0.1)) { 
      std::cout << "Warning: pathlength through target is negativ: "
      	<< pathlength*10 << " mm !\n";
    }
  }
  } 
  
   if (ModelType == ElasticNucleus || ModelType == QuasiElasticNucleus && ((x*x + z*z) > r*r) && ((x*x + z*z) <= (r+WallThickness)*(r+WallThickness))){
      if(z < 0 && fabs(x) <= 10.01){// wall in front of deuterium
       if (!(WallPath < 0)) {  
	 if (SnowPath>0) {
	   old_momentum = P.momentum();  
	   LandauLoss(P, Snow, SnowPath, halton[7], halton[8]);
	   if (exRadSnow) externalBremsstrahlung(P, Snow, SnowPath, halton[9]);
	   Snow->MultiScat(P, old_momentum, SnowPath);
	 }
  	old_momentum = P.momentum(); 
	LandauLoss(P, WallMat, WallPath, halton[10], halton[11]); 
	if (exRadWall) externalBremsstrahlung(P, WallMat, WallPath, halton[12]); 
	WallMat->MultiScat(P, old_momentum, WallPath); 
	if (intRadWall) internalBremsstrahlung(P, WallMat, halton[26],rundb.sim.q2); //Scattering is here inside the wall, so also the internal radiation has to be inside the wall
       }
       }
        if(z > 0 && fabs(x) <= 10.01){// vertex inside wall behind deuterium
    
      if (!(BackWallPath < 0)) {  
	if (SnowPath>0) {
	  old_momentum = P.momentum();  //save momentum of the particle before doing the energyloss
	  LandauLoss(P, Snow, SnowPath, halton[7], halton[8]);
	  if (exRadSnow) externalBremsstrahlung(P, Snow, SnowPath, halton[9]);
	  Snow->MultiScat(P, old_momentum, SnowPath);
	}
	if(WallPath>0){
	  old_momentum = P.momentum(); 
	  LandauLoss(P, WallMat, WallPath, halton[10], halton[11]); 
	  if (exRadWall) externalBremsstrahlung(P, WallMat, WallPath, halton[12]); 
	  WallMat->MultiScat(P, old_momentum, WallPath); 
	} 
	
	//after front wall electron goes through deuterium:
	old_momentum = P.momentum(); //save momentum of the particle before doing the energyloss
	if (exRadTar) externalBremsstrahlung(P, TargetMat, pathlength, halton[16]);
	TargetMat->MultiScat(P, old_momentum, pathlength);
	
	//and then through part of the backwall:
	old_momentum = P.momentum(); 
	LandauLoss(P, WallMat, BackWallPath, halton[17], halton[18]); 
	if (exRadWall) externalBremsstrahlung(P, WallMat, BackWallPath, halton[12]); 
	WallMat->MultiScat(P, old_momentum, BackWallPath); 
	if (intRadWall) internalBremsstrahlung(P, WallMat, halton[27],rundb.sim.q2); //Scattering is here inside the wall, so also the internal radiation has to be inside the wall
	 }
      }
   
    if(fabs(x)>10){//Sidewall, path through deuterium = 0
       if (!(SideWallPath < 0)) { 
	if (SnowPath>0) {
	  old_momentum = P.momentum();  
	  LandauLoss(P, Snow, SnowPath, halton[7], halton[8]);
	  if (exRadSnow) externalBremsstrahlung(P, Snow, SnowPath, halton[9]);
	  Snow->MultiScat(P, old_momentum, SnowPath);
	}
	old_momentum = P.momentum();  
        LandauLoss(P, WallMat, SideWallPath, halton[10], halton[11]); 
	if (exRadWall) externalBremsstrahlung(P, WallMat, SideWallPath, halton[12]); 
	WallMat->MultiScat(P, old_momentum, SideWallPath); 
	if (intRadWall) internalBremsstrahlung(P, WallMat, halton[31],rundb.sim.q2);
	}
      else {
	if (!(WallPath==-0.1)) {
	  std::cout << "Warning: WallPath through wall is negativ: "
	  	    << WallPath*10 << " mm !\n";
	}
      }
    }
  }
  
  
  //inside snow
  if(ModelType == ElasticCryogens){
  if ((x*x + z*z) > (r+WallThickness)*(r+WallThickness) && (x*x + z*z) <= (r+WallThickness+SnowThickness)*(r+WallThickness+SnowThickness)){ //here needs the model for ice to be added
    if(z < 0 && fabs(x) <= 10+WallThickness){// ice in front ot deuterium
      if (!(SnowPath < 0)) {
	if (SnowPath>0) {
	  old_momentum = P.momentum();  
	  LandauLoss(P, Snow, SnowPath, halton[7], halton[8]);
	  if (exRadSnow) externalBremsstrahlung(P, Snow, SnowPath, halton[9]);
	  Snow->MultiScat(P, old_momentum, SnowPath);
	  if (intRadSnow) internalBremsstrahlung(P, Snow, halton[31],rundb.sim.q2);
	}
      }
      else {
	if (!(WallPath==-0.1)) {
	  std::cout << "Warning: SnowPath through Snow is negativ: "
		    << WallPath*10 << " mm !\n";
	}
      }

    }
    if(z >= 0 && fabs(x) <= 10+WallThickness){//ice behind deuterium
      old_momentum = P.momentum();
      if (intRadSnow) internalBremsstrahlung(P, Snow,halton[26],rundb.sim.q2); //should be the first energyloss after vertex
      LandauLoss(P, Snow, SnowPath, halton[0], halton[1]);
      if (exRadSnow) externalBremsstrahlung(P, Snow,SnowPath,halton[3]);
      Snow->MultiScat(P, old_momentum, SnowPath);
	
      if (WallPath>0) {//front wall
	old_momentum = P.momentum();
	LandauLoss(P, WallMat, WallPath, halton[4], halton[5]);
	if (exRadWall) externalBremsstrahlung(P, WallMat, WallPath, halton[6]);
	WallMat->MultiScat(P, old_momentum, WallPath);
      }

      if (!(pathlength < 0)) {//deuterium after vertex
	old_momentum = P.momentum();
	LandauLoss(P, TargetMat, pathlength, halton[0], halton[1]);
	if (exRadTar) externalBremsstrahlung(P, TargetMat,pathlength,halton[3]);
	TargetMat->MultiScat(P, old_momentum, pathlength);
      }
	  
      if (BackWallPath>0) {//back wall
	old_momentum = P.momentum();
	LandauLoss(P, WallMat, BackWallPath, halton[4], halton[5]);
	if (exRadWall) externalBremsstrahlung(P, WallMat, BackWallPath, halton[6]);
	WallMat->MultiScat(P, old_momentum, BackWallPath);
      }

      if (!(SnowPath < 0)) {
	if (SnowPath>0) {
	  old_momentum = P.momentum();  
	  LandauLoss(P, Snow, SnowPath, halton[7], halton[8]);
	  if (exRadSnow) externalBremsstrahlung(P, Snow, SnowPath, halton[9]);
	  Snow->MultiScat(P, old_momentum, SnowPath);
	  if (intRadSnow) internalBremsstrahlung(P, Snow, halton[31],rundb.sim.q2);
      }
      else {
	if (!(WallPath==-0.1)) {
	  std::cout << "Warning: SnowPath through Snow is negativ: "
		    << WallPath*10 << " mm !\n";
	}
      }
      }
    }
    if(fabs(x) > 10+WallThickness){//ice on the sides
      if (!(SnowPath < 0)) {
	if (SnowPath>0) {
	  old_momentum = P.momentum();  
	  LandauLoss(P, Snow, SnowPath, halton[7], halton[8]);
	  if (exRadSnow) externalBremsstrahlung(P, Snow, SnowPath, halton[9]);
	  Snow->MultiScat(P, old_momentum, SnowPath);
	  if (intRadSnow) internalBremsstrahlung(P, Snow, halton[31],rundb.sim.q2);
      }
      else {
	if (!(WallPath==-0.1)) {
	  std::cout << "Warning: SnowPath through Snow is negativ: "
		    << WallPath*10 << " mm !\n";
	}
      }

    }
    
  }
    }
  }
  

}

void 
cryo_cyl_2014::EnergyLossCorrBeam(Particle& P,double x, double y, double z, int steps)
{//Corrections for energy loss before vertex

double r = diameter/2;
double pathlength = getLength_in_Target(x, y, z, M_PI, 0.) / 10;
double SnowPath = Length_through_Snow/10;

  if (!(pathlength < 0)) {
    if (TargetMat) { 
      for (int iii=0; iii<steps; iii++) {
	P += TargetMat->dEdx(P, pathlength/steps);
      }
    }
  }
  else {
    if (!(pathlength==-0.1)) {
      std::cout << "Warning: pathlength through target is negativ: "
		<< pathlength*10 << " mm !\n";
    }
  }
  if (WallMat) 
    P += WallMat->dEdx(P, Length_in_Frontwall/10);
  if (SnowPath>0) {
    for (int iii=0; iii<steps; iii++) {
      P -= Snow->dEdx(P, SnowPath/steps);
    }
  }
  //in wall:
  if (((x*x + z*z) > r*r) && ((x*x + z*z) <= (r+WallThickness)*(r+WallThickness))){
    if(z < 0 && fabs(x) <= 10){// wall in front of deuterium
      if (WallMat) 
	P += WallMat->dEdx(P, Length_in_Frontwall/10);
      if (SnowPath>0) {
	for (int iii=0; iii<steps; iii++) {
	  P += Snow->dEdx(P, SnowPath/steps);
	}
      }
    }
    if(fabs(x)>10){//Sidewall, path through deuterium = 0
      if (WallMat) 
	P += WallMat->dEdx(P, Distance_to_BeginningOfCell/10);
      if (SnowPath>0) {
	for (int iii=0; iii<steps; iii++) {
	  P -= Snow->dEdx(P, SnowPath/steps);
	}
      }
    }
  }

}

void 
cryo_cyl_2014::EnergyLossCorr(Particle& P, double x, double y, double z, int steps)
{//corrections for energyloss after vertex
double r = diameter/2;
double pathlength = getLength_in_Target(x, y, z, P.theta(), P.phi()) / 10;
double SnowPath = Length_through_Snow/10;

  if (!(pathlength < 0)) {
    if (SnowPath>0) {
      for (int iii=0; iii<steps; iii++) {
	P -= Snow->dEdx(P, SnowPath/steps);
      }
    }
    //if (WallMat) P -= WallMat->dEdx(P, Length_in_Wall/10);
    if (WallMat) P -= WallMat->dEdx(P, Length_in_Backwall/10);
    if (TargetMat) {
      for (int iii=0; iii<steps; iii++) {
	P -= TargetMat->dEdx(P, pathlength/steps);
      }
    }
  }
  else {
    if (!(pathlength==-0.1)) {
      std::cout << "Warning: pathlength through target is negativ: "
		<< pathlength*10 << " mm !\n";
    }
  }

  if (((x*x + z*z) > r*r) && ((x*x + z*z) <= (r+WallThickness)*(r+WallThickness))){//in wall
    if(z>=0 && fabs(x) <= 10){//wall behind deuterium
      if (WallMat) 
	P -= WallMat->dEdx(P, Length_in_Backwall/10);
      if (SnowPath>0) {
	for (int iii=0; iii<steps; iii++) {
	  P -= Snow->dEdx(P, SnowPath/steps);
	}
      }
    }
    if(fabs(x)>10){//Sidewall, path through deuterium = 0
      if (WallMat) 
	P -= WallMat->dEdx(P, Distance_to_EndOfCell/10);
      if (SnowPath>0) {
	for (int iii=0; iii<steps; iii++) {
	  P -= Snow->dEdx(P, SnowPath/steps);
	}
      }
    }
  }
}


//###################
void 
target::EnergyLossSimChamber(Particle& P)
{
  double old_momentum;

  static double my1=Scat_Exit_Window_thickness/10;
  static double air=Air_between_Scat_Spec_thickness/10;
  static double my2=Spec_Entrance_Window_thickness/10;  
  static int exRadWindow = ((int)rundb.Target.externalRadiation) & 0x4;

  old_momentum = P.momentum();
  if (P==P_electron) {
    LandauLoss(P, Mylar, my1, halton[17], halton[18]);
    if (exRadWindow) externalBremsstrahlung(P, Mylar, my1, halton[19]); 
  } else   P += Mylar->dEdx(P, my1);
  Mylar->MultiScat(P, old_momentum, my1);

  old_momentum = P.momentum();
  if (P==P_electron) {
    LandauLoss(P, Air,   air, halton[20], halton[21]);
    if (exRadWindow) externalBremsstrahlung(P, Air,   air, halton[22]); 
  } else P += Air->dEdx(P, air);
  Air->MultiScat(P, old_momentum, air);
    
  old_momentum = P.momentum();
  if (P==P_electron) {
    LandauLoss(P, Mylar, my2, halton[23], halton[24]);
    if (exRadWindow) externalBremsstrahlung(P, Mylar, my2, halton[25]);  
  } else P += Mylar->dEdx(P, my2);  
  Mylar->MultiScat(P, old_momentum, my2);

}

void 
target::EnergyLossCorrChamber(Particle& P)
{
  P -= Mylar->dEdx(P, Spec_Entrance_Window_thickness/10);
  P -= Air->dEdx(P, Air_between_Scat_Spec_thickness/10);
  P -= Mylar->dEdx(P, Scat_Exit_Window_thickness/10);
}

void 
target::EnergyLossSimKaos(Particle& P)
{
  double old_momentum;

  static double my1=Scat_Exit_Window_thickness/10;
  static double air=Air_between_Scat_Kaos_thickness/10;
  static double my2=Kaos_Entrance_Window_thickness/10;  
  static int exRadWindow = ((int)rundb.Target.externalRadiation) & 0x4;

  old_momentum = P.momentum();
  if (P==P_electron) {
    LandauLoss(P, Mylar, my1, halton[17], halton[18]);
    if (exRadWindow) externalBremsstrahlung(P, Mylar, my1, halton[19]); 
  } else   P += Mylar->dEdx(P, my1);
  Mylar->MultiScat(P, old_momentum, my1);

  old_momentum = P.momentum();
  if (P==P_electron) {
    LandauLoss(P, Air,   air, halton[20], halton[21]);
    if (exRadWindow) externalBremsstrahlung(P, Air,   air, halton[22]); 
  } else P += Air->dEdx(P, air);
  Air->MultiScat(P, old_momentum, air);
    
  old_momentum = P.momentum();
  if (P==P_electron) {
    LandauLoss(P, Mylar, my2, halton[23], halton[24]);
    if (exRadWindow) externalBremsstrahlung(P, Mylar, my2, halton[25]);  
  } else P += Mylar->dEdx(P, my2);  
  Mylar->MultiScat(P, old_momentum, my2);

}

void 
target::EnergyLossCorrKaos(Particle& P)
{
  P -= Mylar->dEdx(P, Kaos_Entrance_Window_thickness/10);
  P -= Air->dEdx(P, Air_between_Scat_Kaos_thickness/10);
  P -= Mylar->dEdx(P, Scat_Exit_Window_thickness/10);
}

double target::getRadius ( int model )
 {
   return Length / 2;
 }

double target::gethalfLength ( int model )
{
  return Length / 2;
}

double target::getLength ( )
{
  return Length;
}

double target::getangle ( )
{
  return 0;
}


target *
SetTargetFromRunDB(const reaction * Reaction)
{
  target *bssTarget = NULL;
  bssTarget = new cryo_ewald;
  bssTarget->setTargetMat(Havar);
  
  target *Target = NULL;
  if (rundb.target) {
    if (!strcmp(rundb.target, "Cryo"          )) Target = new cryo_ewald;
    if (!strcmp(rundb.target, "Cryo.Wall"     )) Target = new cryo_ewald; //Havar "Wall"
    if (!strcmp(rundb.target, "Cryo.Empty"    )) Target = new cryo_ewald_empty;
    if (!strcmp(rundb.target, "Cryo.2014_hydrogen"    )) Target = new cryo_ewald_2014(0);  // cryo for isr_2013 beam time
    if (!strcmp(rundb.target, "Cryo.2014_wall"    )) Target = new cryo_ewald_2014(1);  // cryo for isr_2013 beam time wall simulation
    if (!strcmp(rundb.target, "Cryo.2014_ice"    )) Target = new cryo_ewald_2014(2);  // cryo for isr_2013 beam time ice simulation
    if (!strcmp(rundb.target, "Cryo.Helium"   )) Target = new helium; 
    if (!strcmp(rundb.target, "Cryo.HeliumNew")) Target = new helium_new;
    if (!strcmp(rundb.target, "Cryo.Cylinder" )) Target = new cryo_cyl; 
    if (!strcmp(rundb.target, "Cryo.Cylinder2014" )) Target = new cryo_cyl_2014(0); //cryo 2014
    if (!strcmp(rundb.target, "Cryo.Cylinder2014_wall" )) Target = new cryo_cyl_2014(1); //cryo 2014 wall
    if (!strcmp(rundb.target, "Solidstate"    )) Target = new solidstate; 
    if (!strcmp(rundb.target, "WindowlessTube"    )) Target = new windowlesstube; 
    if (!strcmp(rundb.target, "Stripes"       )) Target = new stripe_stack; 
    if (!strcmp(rundb.target, "Waterfall"     )) Target = new H2O; 
    if (!strcmp(rundb.target, "TwoFoils"      )) Target = new twofoils;
    if (!strcmp(rundb.target, "CalibWater"    )) Target = new calib_water;
    if (!strcmp(rundb.target, "He3pol"        )) Target = new helium_pol;
    if (!strcmp(rundb.target, "He3pol07"      )) Target = new he_pol_07;
    if (!strcmp(rundb.target, "gasjet"      ))
      Target = new gasjet(rundb.Target.size_length, rundb.Target.size_height, rundb.Target.size_width, rundb.Target.flow);
  }

  cout << "Target: " << rundb.target << endl;

  if (!Target) {
    std::cerr << "No Target defined in 'run.db', using default 'Target=\"Cryo\"'\n";
    Target=new cryo_ewald; 
  }  

  if (*Reaction->getTarget() == P_H1)
    if (strcmp(rundb.target, "gasjet"))
      Target->setTargetMat(LH2); 
  
  if (*Reaction->getTarget() == P_H2)       Target->setTargetMat(LD2);
  if (*Reaction->getTarget() == P_He3)      Target->setTargetMat(He3);
  if (*Reaction->getTarget() == P_He4)      Target->setTargetMat(He4);
  if (*Reaction->getTarget() == P_Be9)      Target->setTargetMat(Be);
  if (*Reaction->getTarget() == P_C12)      Target->setTargetMat(C12pure);
 

  if (*Reaction->getTarget() == P_N14)      Target->setTargetMat(N14);

  if (*Reaction->getTarget() == P_Cu63)     Target->setTargetMat(Cu);
  if (*Reaction->getTarget() == P_Ta181)    Target->setTargetMat(Ta);
  if (*Reaction->getTarget() == P_C13)      Target->setTargetMat(C12pure);
  if ((*Reaction->getTarget() == P_O16) || 
      (!strcmp(rundb.target, "Waterfall"))) Target->setTargetMat(Water);
  if (((*Reaction->getTarget() == P_O16) || ( *Reaction->getTarget() == P_H1)) && 
      (!strcmp(rundb.target, "CalibWater"))) Target->setTargetMat(Water);
  if ((*Reaction->getTarget() == P_H1) &&   
      (!strcmp(rundb.target, "Solidstate")))
    { 
      //if (!strcmp(rundb.target, "gasjet")) Target ->setTargetMat(H2);
      //else {
        Target->setTargetMat(CH2);
        std::cerr<<"Debug: CH2 Material selected\n";
      //}
    } 
  
  //--- Entries for the two foil target for ISR2014 (Simulation of empty cell)
  if ((!strcmp(rundb.target, "TwoFoils"))){
    if (*Reaction->getTarget() == P_Cr52)     Target->setTargetMat(Havar);
    if (*Reaction->getTarget() == P_Mn55)     Target->setTargetMat(Havar);
    if (*Reaction->getTarget() == P_Fe56)     Target->setTargetMat(Havar);
    if (*Reaction->getTarget() == P_Ni58)     Target->setTargetMat(Havar);
    if (*Reaction->getTarget() == P_Co59)     Target->setTargetMat(Havar);
    if (*Reaction->getTarget() == P_Cu63)     Target->setTargetMat(Havar);
    if (*Reaction->getTarget() == P_Mo98)     Target->setTargetMat(Havar);
    if (*Reaction->getTarget() == P_Ta181)    Target->setTargetMat(Havar);
    if (*Reaction->getTarget() == P_W184)     Target->setTargetMat(Havar);
    if (*Reaction->getTarget() == P_Be9)      Target->setTargetMat(Havar);
    if (*Reaction->getTarget() == P_C12)      Target->setTargetMat(Havar);

    if (*Reaction->getTarget() == P_N14)      Target->setTargetMat(FrozenAir);
    if (*Reaction->getTarget() == P_O16)      Target->setTargetMat(FrozenAir);

  }

  cout << "Reaction target: " << Reaction->getTarget()->getName() << endl;
  cout << "Properties:" << endl;
  Target->getTargetMat()->print();

  if (!strcmp(rundb.target, "Cryo.Wall")) Target->setTargetMat(LH2);
  if (!strcmp(rundb.target, "Cryo.Empty")) Target->setTargetMat(Vacuum);
  if (!strcmp(rundb.target, "Cryo.2014_hydrogen")) Target->setTargetMat(LH2);
  if (!strcmp(rundb.target, "Cryo.2014_wall")) Target->setTargetMat(Havar);
  if (!strcmp(rundb.target, "Cryo.2014_ice")) Target->setTargetMat(N2);
  Target->setPara(rundb.Target.length, rundb.Target.angle,
		  rundb.Target.density, rundb.Target.wallthickness,
		  rundb.Target.snowthickness, rundb.Target.snowdensity);
   
  return Target;
}

/* Test-Code

main()
{
  target *Target = new H2O;
  Target->setPara(65. , 10.0);
  double x = 0;
  double y = 0.;
  double z = 0.0;
  double theta = 0. ;
  double phi = 0. ;

  printf("%f    Test", Target->Length);
  printf("%f    Test2 /n", Target->WallThickness);

  for(theta=-M_PI; theta<=M_PI;theta+=2*pi/16.0) {

    double l = Target->getLength_in_Target(x,y,z, theta, phi);
    printf("%f    %f     %f\n",theta*180/pi,l,Target->Length_in_Wall);
    //printf("%f    %f     %f\n",theta*180/pi,l);
  }
  
  return 0;
}

*/

