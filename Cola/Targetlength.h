// -*-c++-*-

#ifndef __TARGETLENGTH_H__
#define __TARGETLENGTH_H__

#include "Model/Model.h"
#include "Material.h"
#include "Reactions.h"
#include "rundatabase.h"
#include "HMBook/hmbook.h"
#include "Chamber/chlib++.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "QuasiRandom.h"

//extern SobolSequence sobol;

class target {
protected:
  const Material *WallMat;
  Material *TargetMat;
  double WallThickness;
  double Length_in_Wall;
  double Length_in_Frontwall;
  double Length_in_Backwall;
  double Length_in_SideWall;
  double Distance_to_FrontWall;
  double Distance_to_BeginningOfCell;
  double Thickness_of_Frontwall;
  double Distance_to_BackWall;
  double Distance_to_EndOfCell;
  double WallPathAll;
  double Length;
  double SnowThickness;
  double Length_through_Snow;
  double Length_through_Spek;
  double Time_through_Spek;
public:
  target() {
    WallMat = TargetMat = NULL;
    Length = WallThickness = Length_in_Wall = Length_in_Frontwall = Length_in_Backwall =
      SnowThickness = Length_through_Snow = Length_through_Spek = Length_in_SideWall = Distance_to_FrontWall = Distance_to_BeginningOfCell = Thickness_of_Frontwall = Distance_to_EndOfCell = Distance_to_BackWall = 0;
  }
  virtual ~target() { ; }
  double diameter;
  virtual double getLength_in_Target(double x, double y, double z, 
				     double theta, double phi) = 0;
  
  //  int Get_Vertex(double * Vertex);
  virtual int Generate_Vertex(double random[], double x[], double wob_x, double wob_y, modeltype ModelType) = 0;
  
  virtual int setPara(double length, double angle,
		      double density, double wallthck,
		      double snowthck, double snowdns) = 0;

  void setTargetMat(Material * mat) { TargetMat = mat; }

  Material * getTargetMat() { return TargetMat; }

  virtual double getangle();
  virtual double getLength();
  virtual double gethalfLength(int model); // for cryo_ewald_2014
  virtual double getRadius(int model); // for cryo_ewald_2014
  
  double getLength_in_Wall() { return Length_in_Wall; }
  double getLength_in_BackWall() { return Length_in_Backwall; }

  double getLength_through_Snow() { return Length_through_Snow; } //this is not in Targetlenght.cc

  double getDistance_to_BeginningOfCell(){return Distance_to_BeginningOfCell;}
  double getDistance_to_EndOfCell(){return Distance_to_EndOfCell;}
  double getDistance_to_BackWall(){return Distance_to_BackWall;}
  double getDistance_to_FrontWall(){return Distance_to_FrontWall;}
  double getLength_in_Backwall() { return Length_in_Backwall; }
  double getLength_in_Frontwall() { return Length_in_Frontwall; }


  double getLength_through_Spek() { return Length_through_Spek; }
  double getTime_through_Spek() { return Time_through_Spek; }

//
// Functions to get the simulation right 
//

    virtual void EnergyLossSim(Particle& P, double x, double y, double z,
			       int steps, modeltype ModelType);
    virtual void EnergyLossSimBeam(Particle& P,double x, double y, double z,
				   int steps, modeltype ModelType);
  
    virtual void EnergyLossSimChamber(Particle& P);
    void EnergyLossSimKaos(Particle& P);
    void EnergyLossSimSpec(Particle& P, double phi, double theta, double y,
			   double& Eloss_dE, double& Energy_before_dE, 
			   double & Eloss_dE_Ref);

//
// Funktions to get the analysis right
//

    virtual void EnergyLossCorr(Particle& P, double x, double y, double z,
				int steps);
    virtual void EnergyLossCorrBeam(Particle& P,double x, double y, double z,
				    int steps);
    virtual void EnergyLossCorrChamber(Particle& P);
    void EnergyLossCorrSpec(Particle& P);

    void EnergyLossCorrKaos(Particle& P);   
    
};

class cryo_cyl : public target {
private:
  static double diameter, totalheight, Wall, SnowWall;
public:
  cryo_cyl() {
    WallMat=Havar; TargetMat=LH2; Length=diameter; WallThickness=Wall;
    SnowThickness=SnowWall;
  }
  virtual ~cryo_cyl() { ; }
  double getLength_in_Target(double x, double y, double z, 
			     double theta, double phi);

  

  int Generate_Vertex(double random[], double x[], double wob_x, double wob_y, modeltype ModelType);
  int setPara(double length, double angle,
	      double density, double wallthck,
	      double snowthck, double snowdns);
};


class cryo_cyl_2014 : public target {
private:
  static double diameter, totalheight, Wall, SnowWall;
  int modeltype2;
public:
  cryo_cyl_2014(int model) {
    WallMat=Havar; TargetMat=LH2; Length=diameter; WallThickness=Wall;
    SnowThickness=SnowWall;
    modeltype2 = model;
  }
  virtual ~cryo_cyl_2014() { ; }
  double getLength_in_Target(double x, double y, double z, 
			     double theta, double phi, modeltype);
  double getLength_in_Target(double x, double y, double z, 
			     double theta, double phi);
  double getLength_in_Target_MAINSTUFF(double x, double y, double z, 
			     double theta, double phi);
  //int Get_Vertex(double *Vertex);
  //double gethalfLength(int model);
  //double getRadius(int model);
  int Generate_Vertex(double random[], double x[], double wob_x, double wob_y, modeltype ModelType);
  int setPara(double length, double angle,
	      double density, double wallthck,
	      double snowthck, double snowdns);
  
  virtual void EnergyLossSim(Particle& P, double x, double y, double z,
			       int steps, modeltype ModelType);
  virtual void EnergyLossSimBeam(Particle& P,double x, double y, double z,
				 int steps, modeltype ModelType);
  virtual void EnergyLossCorr(Particle& P, double x, double y, double z,
			      int steps);
  virtual void EnergyLossCorrBeam(Particle& P,double x, double y, double z,
				  int steps);
  //virtual void EnergyLossCorrBeam(Particle& P,double x, double y, double z,
  //				    int steps, modeltype ModelType);
  double getLength_in_Frontwall() { return Length_in_Frontwall; }
};


class cryo_ewald : public target {
private:
  static double totallength, totalwidth, totalheight, Wall, SnowWall;
public:
  cryo_ewald() {
    WallMat=Havar; TargetMat=LH2; Length=totallength; WallThickness=Wall;
    SnowThickness=SnowWall;
  }
  virtual ~cryo_ewald() { ; }
  double getLength_in_Target(double x, double y, double z, 
			     double theta, double phi);
  int Generate_Vertex(double random[], double x[], double wob_x, double wob_y, modeltype ModelType);
  int setPara(double length, double angle,
	      double density, double wallthck,
	      double snowthck, double snowdns);
};

class cryo_ewald_empty : public target {
private:
  static double totallength, totalwidth, totalheight, Wall;
public:
  cryo_ewald_empty() {
    WallMat=Havar; TargetMat=H2; Length=totallength; WallThickness=Wall;
  }
  virtual ~cryo_ewald_empty() { ; }
  double getLength_in_Target(double x, double y, double z, 
			     double theta, double phi);
  int Generate_Vertex(double random[], double x[], double wob_x, double wob_y, modeltype ModelType);
  int setPara(double length, double angle,
	      double density, double wallthck,
	      double snowthck, double snowdns);
};

//new cryo cell for isr_2013 beam time 
class cryo_ewald_2014 : public target {
private:
  static double totallength, totalwidth, totalheight, Wall, SnowWall, AramidWindowThickness, icefactor;
  double Length_through_Snow2, Length_in_Wall2;
  int modeltype2;
public:
  cryo_ewald_2014(int model) {
    WallMat=Havar; 
    TargetMat=LH2;
    WallThickness=Wall;
    SnowThickness=SnowWall;
    modeltype2 = model;
    }
  virtual ~cryo_ewald_2014() { ; }
  double getLength();
  double gethalfLength(int model);
  double getRadius(int model);
  double getLength_in_Target(double x, double y, double z, double theta, double phi);
  int Generate_Vertex(double random[], double x[], double wob_x, double wob_y, modeltype Modeltype );
  int setPara(double length, double angle,
	      double density, double wallthck,
	      double snowthck, double snowdns);
  
  virtual void EnergyLossSim(Particle& P, double x, double y, double z, int steps, modeltype Modeltype);
  virtual void EnergyLossSimBeam(Particle& P,double x, double y, double z, int steps, modeltype Modeltype);
  virtual void EnergyLossCorr(Particle& P, double x, double y, double z, int steps);
  virtual void EnergyLossCorrBeam(Particle& P,double x, double y, double z, int steps);
  virtual void EnergyLossSimChamber(Particle& P);
  virtual void EnergyLossCorrChamber(Particle& P);
};


class calib_water : public target {
private:
  static double totallength, totalwidth, totalheight, angle_tar, Wall;
public:
  calib_water() {
    WallMat=Fe; TargetMat=Water; Length=totallength; WallThickness=Wall;
  }
  virtual ~calib_water() { ; }
  double getLength_in_Target(double x, double y, double z, 
			     double theta, double phi);
  int Generate_Vertex(double random[], double x[], double wob_x, double wob_y, modeltype ModelType);
  int setPara(double length, double angle,
	      double density, double wallthck,
	      double snowthck, double snowdns);
};

class helium : public target {
private:
  static double diameter, totalheight, Wall, SnowWall;
public:
  helium() {
    WallMat=Fe; TargetMat=He3; Length=diameter; WallThickness=Wall;
    SnowThickness=SnowWall;
  }
  virtual ~helium() { ; }
  double getLength_in_Target(double x, double y, double z, 
			     double theta, double phi);
  int Generate_Vertex(double random[], double x[], double wob_x, double wob_y, modeltype ModelType);
  int setPara(double length, double angle,
	      double density, double wallthck,
	      double snowthck, double snowdns);
};

class helium_new : public target {
private:
  static double diameter, totalheight, Wall, SnowWall;
public:
  helium_new() {
    WallMat=Al; TargetMat=He3; Length=diameter; WallThickness=Wall;
    SnowThickness=SnowWall;
  }
  virtual ~helium_new() { ; }
  double getLength_in_Target(double x, double y, double z, 
			     double theta, double phi);
  int Generate_Vertex(double random[], double x[], double wob_x, double wob_y, modeltype ModelType);
  int setPara(double length, double angle,
	      double density, double wallthck,
	      double snowthck, double snowdns);

  virtual void EnergyLossSim(Particle& P, double x, double y, double z,
			       int steps);
  virtual void EnergyLossSimBeam(Particle& P,double x, double y, double z,
				   int steps);
  virtual void EnergyLossCorr(Particle& P, double x, double y, double z,
			      int steps);
  virtual void EnergyLossCorrBeam(Particle& P,double x, double y, double z,
				  int steps);  
};

class helium_pol : public target {
private:
  static double BeThickness, CuThickness;
  static double diameter, Wall;
public:
  helium_pol() {
    WallMat=Quartz; TargetMat=He3; Length=diameter; WallThickness=Wall;
  }
  virtual ~helium_pol() { ; }
  double getLength_in_Target(double x, double y, double z, 
			     double theta, double phi);
  int Generate_Vertex(double random[], double x[], double wob_x, double wob_y, modeltype ModelType);
  int setPara(double length, double angle,
	      double density, double wallthck,
	      double snowthck, double snowdns);
  void EnergyLossCorrBeam(Particle& P, double x, double y, double z, int);
  void EnergyLossSimBeam(Particle& P,double x, double y, double z, int, modeltype ModelType);
  void EnergyLossCorrChamber(Particle& P);
  void EnergyLossSimChamber(Particle& P);
};

//polarized target cell used in G_en and TriplePol 2007
//M. Weinriefer
class he_pol_07 : public target {
private:
  static double BeThickness, AlThickness;
  static double diameter, Wall;
  std::ifstream walldata;
  double *wallcorr;
  int wmin, wmax, wbins;
  bool fileopen;
public:
  he_pol_07() {
    WallMat=Quartz; TargetMat=He3; Length=diameter; WallThickness=Wall;
    walldata.open(rundb.Target.wallfile, std::fstream::in);
    if (!walldata.is_open()) {
      std::cerr << "Error opening walldata file! Using default wall values!" << std::endl;
      fileopen = false;
    } else {
      fileopen = true;
      wmin  = (int)rundb.Target.wallparam[0];
      wmax  = (int)rundb.Target.wallparam[1];
      wbins = (int)rundb.Target.wallparam[2];
      wallcorr = new double[wbins];
      double tmp;
      for (int i=0; i<wbins; i++)
	walldata >> tmp >> wallcorr[i];
    }
  }
  virtual ~he_pol_07() { ; }
  double getLength_in_Target(double x, double y, double z, 
			     double theta, double phi);
  int Generate_Vertex(double random[], double x[], double wob_x, double wob_y, modeltype ModelType);
  int setPara(double length, double angle,
	      double density, double wallthck,
	      double snowthck, double snowdns);
  void EnergyLossCorrBeam(Particle& P, double x, double y, double z, int);
  void EnergyLossSimBeam(Particle& P,double x, double y, double z, int, modeltype ModelType);
  void EnergyLossCorrChamber(Particle& P);
  void EnergyLossSimChamber(Particle& P);
};

class solidstate : public target {
private:
  static double totallength, totalheight, totalwidth, angle_tar;
public:
  solidstate() { Length = totallength; }
  virtual ~solidstate() { ; }
  
  double getLength_in_Target(double x, double y, double z, 
			     double theta, double phi);
  int Generate_Vertex(double random[], double x[], double wob_x, double wob_y, modeltype ModelType);
  int setPara(double length, double angle,
	      double density, double wallthck,
	      double snowthck, double snowdns);
  double getangle ( );
};

class windowlesstube : public target {
private:
  static double totallength, totalheight, totalwidth;
public:
  windowlesstube() { Length = totallength; }
  virtual ~windowlesstube() { ; }
  
  double getLength_in_Target(double x, double y, double z, 
			     double theta, double phi);
  int Generate_Vertex(double random[], double x[], double wob_x, double wob_y, modeltype ModelType);
  int setPara(double length, double angle,
	      double density, double wallthck,
	      double snowthck, double snowdns);
};

class gasjet: public target {
private:
  static double totallength, totalheight, totalwidth;
public:
  gasjet() { Length = totallength; cout << "Gasjet length = " << totallength << endl;}
  virtual ~gasjet() { ; }
  
  double getLength_in_Target(double x, double y, double z, 
			     double theta, double phi);
  int Generate_Vertex(double random[], double x[], double wob_x, double wob_y, modeltype ModelType);
  int setPara(double length, double angle,
	      double density, double wallthck,
	      double snowthck, double snowdns);
};


class stripe_stack : public solidstate {
  int Generate_Vertex(double random[], double x[], double wob_x, double wob_y, modeltype ModelType) {
    x[0] = rundb.sim.wobx * cos(random[0] * M_PI) + rundb.beam.offset.x - rundb.Target.offset_sim.x;
    x[1] = rundb.sim.woby * cos(random[1] * M_PI) + rundb.beam.offset.y - rundb.Target.offset_sim.y;
    x[2] = getLength() * (random[2] - .5);
    if (fabs(x[2]) > getLength() / 2. / cos(getangle())) return 0;
    x[2] += (int) (rndm()*12)*10-55; // 12 stripes with 10mm distance
    return 1;
  }
};

class twofoils : public solidstate {
  int Generate_Vertex(double random[], double x[], double wob_x, double wob_y, modeltype ModelType) {
    x[0] = rundb.sim.wobx * cos(random[0] * M_PI) + rundb.beam.offset.x - rundb.Target.offset_sim.x;
    x[1] = rundb.sim.woby * cos(random[1] * M_PI) + rundb.beam.offset.y - rundb.Target.offset_sim.y;
    x[2] = getLength() * (random[2] - .5);
    if (fabs(x[2]) > getLength() / 2. / cos(getangle())) return 0;
    x[2] += ((int)(rndm()+0.5))*48.2-24.1; // 2 stripes with 48.2mm distance
    return 1;
  }
};

class H2O : public target {
private:
  static double totallength, totalheight, totalwidth, angle_tar,
    Wall, diameter;
public:
  H2O() {
    WallMat=Fe; TargetMat=Water; Length=totallength; WallThickness=Wall;
  }
  virtual ~H2O() { ; }
  double getLength_in_Target(double x, double y, double z, 
			     double theta, double phi);
  int Generate_Vertex(double random[], double x[], double wob_x, double wob_y, modeltype ModelType);
  int setPara(double length, double angle,
	      double density, double wallthck,
	      double snowthck, double snowdns);
};

target *SetTargetFromRunDB(const reaction *);

#endif


