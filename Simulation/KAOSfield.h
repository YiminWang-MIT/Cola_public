//
// $Id: KAOSfield.h 2216 2008-06-13 21:13:47Z distler $
//

#ifndef __KAOS_H__
#define __KAOS_H__

#include <iostream>
#include <sstream>
#include <fstream>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>


union messpunkt { float f; char c[4];unsigned int i;};

typedef struct {
  double wert;
  double d2x;
  double d2y;
  double d2z;
} feldpunkt;

const int Splinerand = 5;

const double cm = 1.0;

const double feldStaerke = 1.95; //!!!!! Not true;


class KAOSField {
  public:
    float zmin,zmax,z0,xmin,xmax,x0,ymin,ymin_sp,ymax,y0;
    double B_Act; // Rescaled value of the field
    double B_Nom; // Maximal value of the field
    
    KAOSField(double Bfield);
    ~KAOSField();
    
    void BerechneDipolFeld(double xl[3], double *Bfield);
    void BerechneDipolFeldGlatt(double xl[3], double *Bfield);
    int ScaleField(double Fract);
  private:
    double kdx,kdy,kdz;
    int anzx, anzy, anzy_sp, anzz;
    
    messpunkt ****feld;
    feldpunkt *splinefeld[3];

    double SpektrometerWinkel;
    double SpektrometerOrt;  
    double SpektrometerHub;

    int getindex(int x, int y, int z) const;
    void cal2ndderivatex(feldpunkt *fp);
    void cal2ndderivatey(feldpunkt *fp);
    void cal2ndderivatez(feldpunkt *fp);
    double interpolate(feldpunkt *fp, double x, double y, double z) const;
   
};

#endif
