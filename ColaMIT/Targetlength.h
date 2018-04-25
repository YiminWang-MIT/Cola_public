// -*-c++-*-
// $Id: Targetlength.h 2216 2008-06-13 21:13:47Z distler $

#ifndef __TARGETLENGTH_H__
#define __TARGETLENGTH_H__

#include "Cola/Material.h"
#include "Cola/Reactions.h"
#include "Chamber/chlib++.h"

class target {
protected:
  const Material *WallMat;
  Material *TargetMat;
  double WallThickness;
  double Length_in_Wall;
  double Length;
  double SnowThickness;
  double Length_through_Snow;
  double Length_through_Spek;
  double Time_through_Spek;
public:
  target() {
    WallMat = TargetMat = NULL;
    Length = WallThickness = Length_in_Wall = 
      SnowThickness = Length_through_Snow = Length_through_Spek = 0;
  }
  virtual double getLength_in_Target(double x, double y, double z, 
				     double theta, double phi) = 0;
  virtual int Generate_Vertex(double x[], double wob_x, double wob_y) = 0;
  
  virtual int setPara(double length, double angle,
		      double density, double wallthck,
		      double snowthck, double snowdns) = 0;

  void setTargetMat(Material * mat) { TargetMat = mat; }
  Material * getTargetMat() { return TargetMat; }
  double getLength() { return Length; }
  double getLength_in_Wall() { return Length_in_Wall; }
  double getLength_through_Snow() { return Length_through_Snow; }
  double getLength_through_Spek() { return Length_through_Spek; }
  double getTime_through_Spek() { return Time_through_Spek; }

  /* Functions for the simulation */
  virtual void EnergyLossSim(Particle& P, double x, double y, double z,
			     int steps);
  virtual void EnergyLossSimBeam(Particle& P,double x, double y, double z,
				 int steps);
  void EnergyLossSimChamber(Particle& P);
  void EnergyLossSimSpec(Particle& P);

  /* Functions for the analysis */
  virtual void EnergyLossCorr(Particle& P, double x, double y, double z,
			      int steps);
  virtual void EnergyLossCorrBeam(Particle& P,double x, double y, double z,
				  int steps);
  void EnergyLossCorrChamber(Particle& P);
  void EnergyLossCorrSpec(Particle& P);
};

class cryo_cyl : public target {
private:
  static double diameter, totalheight, Wall, SnowWall;
public:
  cryo_cyl() {
    WallMat = Havar; TargetMat = LH2; Length = diameter; WallThickness = Wall;
    SnowThickness = SnowWall;
  }
  double getLength_in_Target(double x, double y, double z,
			     double theta, double phi);
  int Generate_Vertex(double x[], double wob_x, double wob_y);
  int setPara(double length, double angle,
	      double density, double wallthck,
	      double snowthck, double snowdns);
};

class solidstate : public target {
private:
  static double totallength, totalheight, totalwidth, angle_tar;
public:
  solidstate() { Length = totallength; }
  
  double getLength_in_Target(double x, double y, double z, 
			     double theta, double phi);
  int Generate_Vertex(double x[], double wob_x, double wob_y);
  int setPara(double length, double angle,
	      double density, double wallthck,
	      double snowthck, double snowdns);
};

target *SetTargetFromRunDB(const reaction *);

#endif


