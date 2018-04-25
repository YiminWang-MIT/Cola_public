//
// Concept 1995 by	Michael O. Distler, 
//                      and Harald Merkel
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz 
//
// $Id: Targetlength.cc 2216 2008-06-13 21:13:47Z distler $
//

#include "Targetlength.h"
#include "rundatabase.h"
#include "Radiation.h"
#include "Cola/QuasiRandom.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define sign(x)  (x>0 ? 1 : (x<0 ? -1 : 0))

//
// All linear dimensions in [mm] 
//

// cylindrical LH2 target cell

double cryo_cyl::diameter      =  0.0; // see below
double cryo_cyl::totalheight   = 25.0;
double cryo_cyl::Wall          =  0.0;
double cryo_cyl::SnowWall      =  0.0; // see below

// Solid-state target

double solidstate::totallength =  0.0; // see below
double solidstate::totalwidth  = 20.0;
double solidstate::totalheight = 10.0;
double solidstate::angle_tar   =  0.0; // see below

// Scattering chamber windows etc.

double TgWall             = 0.0043;
double TgChamberWindow    = 0.127;
double TgChamberOHIPSAir  = 500.0;
double TgChamberOOPSAir   = 50.0;
double SpecEntranceWindow = 0.127;
double SpecExitWindow     = 0.127;
double SpecDetAir         = 100.0; // has to be checked
double SpecChamberWindow  = 0.05;  // has to be checked

int 
cryo_cyl::setPara(double len, double, double, double WallThick, 
		  double SnowThick, double SnowDensity)
{
  if (len==0) diameter = 16.0; else diameter = len;
  if (Wall==0) Wall = 0.0043; else Wall = WallThick;

  Length = diameter;
  WallThickness = Wall;

  if (SnowThick < 0) SnowThick = 0;

  SnowWall = SnowThick;            // Thickness in mm
  SnowThickness = SnowWall;

  if (SnowDensity != 0) Snow->setDensity(SnowDensity);   // Density in g/cm^3

  return 1;
}

int 
cryo_cyl::Generate_Vertex(double x[], double /*wob_x*/, double /*wob_y*/) 
{
  double r = diameter / 2;
  
  if ((x[0]*x[0] + x[2]*x[2]) >= r*r) return 0;  // outside of cell
  return 1;
}

double 
cryo_cyl::getLength_in_Target(double x, double y, double z,
			      double theta, double phi)
{
  double result, result2;
  double r = diameter / 2.;
  double dx[3] = {sin(theta) * cos(phi),          
		  sin(theta) * sin(phi),
		  cos(theta)};

  if (fabs(dx[1]) == 1.) return -1;               // straight up or down
  if (fabs(x) > r) return -1;                     // outside cell

  if ((x*x + z*z) >= r*r) z = sign(z)*(sqrt(r*r-x*x) - 0.05); // vertex z=edge
  
  double p   = 2 *(x * dx[0] + z * dx[2]);
  double q   = x*x + z*z - r*r;
  double p2q = p*p/4-q;

  double check1 = ((p2q < 0) ? 0 : - p/2 + sqrt(p2q));
  double check2 = ((p2q < 0) ? 0 : - p/2 - sqrt(p2q));
  
  double z1 = z + check1 * dx[2];
  double z2 = z + check2 * dx[2];
  
  // Wall:

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

  // Snow:

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

int 
solidstate::setPara(double len, double angle, double, double, double, double)
{
  totallength = len / 100 / TargetMat->getDensity();
  angle_tar = angle * M_PI/180;  
  
  Length = totallength / cos(angle_tar);
  
  return 1;
}

int 
solidstate::Generate_Vertex(double x[], double /*wob_x*/, double /*wob_y*/)
{
  if (fabs(x[2]) > totallength/2./cos(angle_tar)) return 0;  // outside of cell
  return 1; 
}

double 
solidstate::getLength_in_Target(double, double, double,
				double theta, double phi)
{
  double result;
  // phi around 0    : beam-right side
  // phi around +-pi : beam-left side
  double side = ( fabs(phi)<1.57 ? 1.0 : -1.0 );
  double inv_dx[3] = {sin(theta+angle_tar*side) * cos(phi),          
		      sin(theta+angle_tar*side) * sin(phi),
		      cos(theta+angle_tar*side)};

  if (fabs(inv_dx[1]) == 1.) return -1;               // straight up or down

  result = Length / 2 / fabs(inv_dx[2]);

  return result;  
}

void 
target::EnergyLossCorr(Particle& P, double x, double y, double z, int steps)
{
  double pathlength =
    getLength_in_Target(x, y, z, P.theta(), P.phi()) / 10;
  double SnowPath = Length_through_Snow/10;

  if (!(pathlength < 0)) {
    if (SnowPath>0) {
      for (int iii=0; iii<steps; iii++) {
	P -= Snow->dEdx(P, SnowPath/steps);
      }
    }
    if (WallMat) {
      P -= WallMat->dEdx(P, Length_in_Wall/10);
    }
    if (TargetMat) {
      for (int iii=0; iii<steps; iii++) {
	P -= TargetMat->dEdx(P, pathlength/steps);
      }
    }
  }
  else {
    if (!(pathlength==-0.1)) {
      std::cout << "Warning: pathlength through target is negative: "
		<< pathlength*10 << " mm !" << std::endl;
    }
  }
}

void 
target::EnergyLossSim(Particle& P, double x, double y, double z, int steps)
{
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
      
      old_momentum = P.momentum();
      LandauLoss(P, TargetMat, pathlength, halton[0], halton[1]);

      if (intRad) internalBremsstrahlung(P, TargetMat,halton[2],rundb.sim.q2); 
      if (exRadTar) externalBremsstrahlung(P, TargetMat,pathlength,halton[3]);
      
      TargetMat->MultiScat(P, old_momentum, pathlength);

      if (WallPath>0) {
	old_momentum = P.momentum();
	P += WallMat->dEdx(P, WallPath);
	LandauLoss(P, WallMat, WallPath, halton[4], halton[5]);
	if (exRadWall) externalBremsstrahlung(P, WallMat, WallPath, halton[6]);
	WallMat->MultiScat(P, old_momentum, WallPath);
      }
      
      if (SnowPath>0) {
   	old_momentum = P.momentum();
	P += Snow->dEdx(P, SnowPath);
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
		<< pathlength*10 << " mm !" << std::endl;
    }
  }
}

void 
target::EnergyLossCorrBeam(Particle& P,double x, double y, double z, int steps)
{
  double pathlength = getLength_in_Target(x, y, z, M_PI, 0.) / 10;
  
  if (Mylar) P += Mylar->dEdx(P, TgChamberWindow/10);
  if (WallMat) P += WallMat->dEdx(P, Length_in_Wall/10);
  if (!(pathlength < 0)) {
    if (TargetMat) { 
      for (int iii=0; iii<steps; iii++) {
	P += TargetMat->dEdx(P, pathlength/steps);
      }
    }
  }
  else {
    if (!(pathlength==-0.1)) {
      std::cout << "Warning: pathlength through target is negative: "
		<< pathlength*10 << " mm !" << std::endl;
    }
  }
}

void 
target::EnergyLossSimBeam(Particle& P,double x, double y, double z, int steps)
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
      P += WallMat->dEdx(P, WallPath);
      LandauLoss(P, WallMat, WallPath, halton[10], halton[11]);
      if (exRadWall) externalBremsstrahlung(P, WallMat, WallPath, halton[12]);
      WallMat->MultiScat(P, old_momentum, WallPath);
    }
    
    //static double equivradTarget =
    //  equivalentRadiator(TargetMat, rundb.sim.q2);

    old_momentum = P.momentum();
    // P += TargetMat->dEdx(P, pathlength);  warum nicht dieses?
    LandauLoss(P, TargetMat, pathlength, halton[13], halton[14]);

    if (intRad) internalBremsstrahlung(P, TargetMat, halton[15],rundb.sim.q2); 
    if (exRadTar) externalBremsstrahlung(P, TargetMat, pathlength, halton[16]);

    TargetMat->MultiScat(P, old_momentum, pathlength);
  } 
  else {
    if (!(pathlength==-0.1)) {
      std::cout << "Warning: pathlength through target is negative: "
		<< pathlength*10 << " mm !" << std::endl;
    }
  }
}

void 
target::EnergyLossCorrChamber(Particle& P)
{
  if (!Mylar || !Air) return;
  if (P == P_electron) {
    P -= Mylar->dEdx(P, SpecEntranceWindow/10);
    P -= Air->dEdx(P, TgChamberOHIPSAir/10);
    P -= Mylar->dEdx(P, TgChamberWindow/10);
  } else if (P == P_proton || P == P_piplus) {
    P -= Mylar->dEdx(P, SpecEntranceWindow/10);
    P -= Air->dEdx(P, TgChamberOOPSAir/10);
    P -= Mylar->dEdx(P, TgChamberWindow/10);
  }
}

void 
target::EnergyLossSimChamber(Particle& P)
{
  double old_momentum;

  static double my1=SpecEntranceWindow/10;
  static double airOHIPS=TgChamberOHIPSAir/10;
  static double airOOPS=TgChamberOOPSAir/10;
  static double my2=SpecEntranceWindow/10;  

  static int exRadWindow = ((int)rundb.Target.externalRadiation) & 0x4;

  if (P == P_electron) {
    old_momentum = P.momentum();
    P += Mylar->dEdx(P, my1);
    Mylar->MultiScat(P, old_momentum, my1);
    
    old_momentum = P.momentum();
    P += Air->dEdx(P, airOHIPS);
    Air->MultiScat(P, old_momentum, airOHIPS);
    
    old_momentum = P.momentum();
    P += Mylar->dEdx(P, my2);  
    Mylar->MultiScat(P, old_momentum, my2);

    LandauLoss(P, Mylar, my1, halton[17], halton[18]);
    if (exRadWindow) externalBremsstrahlung(P, Mylar, my1, halton[19]); 
    LandauLoss(P, Air, airOHIPS, halton[20], halton[21]);
    if (exRadWindow) externalBremsstrahlung(P, Air, airOHIPS, halton[22]); 
    LandauLoss(P, Mylar, my2, halton[23], halton[24]);
    if (exRadWindow) externalBremsstrahlung(P, Mylar, my2, halton[25]); 
  } else if (P == P_proton || P == P_piplus) {
    old_momentum = P.momentum();
    P += Mylar->dEdx(P, my1);
    Mylar->MultiScat(P, old_momentum, my1);
    
    old_momentum = P.momentum();
    P += Air->dEdx(P, airOOPS);
    Air->MultiScat(P, old_momentum, airOOPS);
    
    old_momentum = P.momentum();
    P += Mylar->dEdx(P, my2);  
    Mylar->MultiScat(P, old_momentum, my2);

    LandauLoss(P, Mylar, my1, halton[26], halton[27]);
    LandauLoss(P, Air, airOOPS, halton[28], halton[29]);
    LandauLoss(P, Mylar, my2, halton[30], halton[31]);
  }
}

void 
target::EnergyLossCorrSpec(Particle& P) 
{
  class matlist {
    const Material * mat;
    double d;
  public:
    matlist() { mat = NULL; d = 0.0; }
    matlist(Material const * const m, double th) { mat = m; d = th; }
    ~matlist() { ; }
    double dE(Particle &p) { return mat->dEdx(p, d/10); }
  };

   matlist * mlist[] = {
    new matlist(Mylar, SpecExitWindow),
    new matlist(Air, SpecDetAir),
    new matlist(Mylar, SpecChamberWindow),
    NULL
  };
  
  int mlist_entries = 0; while (mlist[mlist_entries++]);

  double steps = 20; 

  for (int j=mlist_entries; j>0; j--)
    {
      for (int iii=0; iii<steps; iii++)
	{
	  P -= mlist[j]->dE(P) / steps / cos(M_PI/4);
	}
    }
}

void 
target::EnergyLossSimSpec(Particle& P) 
{
  P += 0;
}

target *
SetTargetFromRunDB(const reaction * Reaction)
{
  target *Target = NULL;
  if (rundb.target) {
    if (!strcmp(rundb.target, "Cryo.Cylinder" )) Target = new cryo_cyl; 
    if (!strcmp(rundb.target, "Solidstate"    )) Target = new solidstate; 
  }
  if (!Target) {
    std::cerr << "No valid Target defined in 'run.db', using default"
	      << " 'Target=\"Cryo.Cylinder\"'" << std::endl;
    Target = new cryo_cyl; 
  }  
  
  if (*Reaction->getTarget() == P_H1)  Target->setTargetMat(LH2);
  if (*Reaction->getTarget() == P_H2)  Target->setTargetMat(LD2);
  if (*Reaction->getTarget() == P_C12) Target->setTargetMat(C12);
  
  Target->setPara(rundb.Target.length, rundb.Target.angle,
		  rundb.Target.density, rundb.Target.wallthickness,
		  rundb.Target.snowthickness, rundb.Target.snowdensity);
  
  return Target;
}
