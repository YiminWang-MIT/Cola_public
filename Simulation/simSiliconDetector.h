//                                                                    -*-c++-*-
// $Id: simSiliconDetector.h 2588 2014-10-30 10:31:33Z kohly $
//

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <iostream>
#include <fstream>
#include <iosfwd>
#include <cstdio>
#include "simDetectorBase.h"
#include "Model/Model.h"

using namespace std;

const int debugSI = getenv("DEBUGSI") != NULL;

const double MSX_width = 3.0; //cm
const double MSX_thickness = 0.1003; //cm
const double MSX_th[5] = {0.1, 0.1003, 0.1003, 0.1003, 0.1003}; //cm 

const double Absorber_width     = 2.4; //cm
const double Absorber_thickness = 0.1; //cm

const double Scint_width     = 2.4; //cm
const double Scint_thickness = 0.325; //cm (Scintillator + foil + tape)

const double VETO_width = 6.000; //cm
const double VETO_height = 4.000; //cm
const double VETO_thickness = 0.0300; //cm

const int    BB2_stripes   = 24;     //number of stripes in one dimension
const double BB2_width     = 2.4;    //cm
const double BB2_thickness = 0.0300; //cm
//const double BB2_distance  = 8.9;    //cm (distance from target)
//const double BB2_angularAccept = 2*atan2(BB2_width/2, BB2_distance);

//const double Si_CentralMomentum =  0.248; //[GeV/c]
const double Si_MomentumAcceptance = 0.0800; //[GeV/c] 

//const double Scint_distance  = BB2_distance-1.5;    //cm (distance from target)
//const double Absorber_distance  = BB2_distance-2.0;    //cm (distance from target)

///////////////////////////////////////////////////////////////////////////////
// This class is just for some simple vector algebra. Copied from elsewhere
//
class Point {
protected:
public:
  double x[3];
  Point(double a=0, double b=0, double c=0) {x[0]=a;x[1]=b;x[2]=c;};
  Point(double y[3]) {x[0]=y[0];x[1]=y[1];x[2]=y[2];};
  Point operator+(const Point &b) const {
    return Point(x[0]+b.x[0], x[1]+b.x[1], x[2]+b.x[2]);};
  Point operator-(const Point &b) const { 
    return Point(x[0]-b.x[0], x[1]-b.x[1], x[2]-b.x[2]);};
  Point operator*(const double b)const {return Point(x[0]*b,x[1]*b,x[2]*b);};
  double operator*(const Point b) const {
    return x[0]*b.x[0]+x[1]*b.x[1]+x[2]*b.x[2];};
  Point operator/(const double b)const {return Point(x[0]/b,x[1]/b,x[2]/b);};
  Point operator*=(const double b) {return Point(x[0]*=b,x[1]*=b,x[2]*=b);};
  Point operator/=(const double b) {return Point(x[0]/=b,x[1]/=b,x[2]/=b);};
  Point operator+=(const Point &b) {
    return Point(x[0]+=b.x[0], x[1]+=b.x[1], x[2]+=b.x[2]);};
  Point operator-=(const Point &b) {
    return Point(x[0]-=b.x[0], x[1]-=b.x[1], x[2]-=b.x[2]);};
  int null() const {return x[0]==0 && x[1]==0 && x[2]==0;};
  double operator[](const int i) const { return x[i];};
  void rotate(double theta) {
    double c=cos(theta), s= sin(theta);
    double tmp = x[0]*c+x[2]*s;  x[2] = -x[0]*s+x[2]*c;  x[0] = tmp;
  }
};

//siPlot is for debuging purpose only. It generates a postscript file
//to visual check the geometry. Can be removed when finished (or 
//better: should be extended for a decent event display).

class siPlot {
protected:
  ofstream *out;
  double a;
public:
  siPlot(char *name) { out = new ofstream(name);init();};

  void init() {
    *out << "%!PS-ADobe 2.0\n"
	 << "72 2.54 div dup scale 10 15 translate\n"
	 << "0.001 setlinewidth\n"
	 << "/m {moveto} def\n/l {lineto} def\n/s {stroke} def\n"
	 <<"/n {newpath} def\n-11 0 m 11 0 l\n0 15 m 0 -15 l s\n";
    a=200;
  };

  inline double x3d(Point x){
    const double c=cos(20*M_PI/180), s= sin(20*M_PI/180);
    return a/(a-x[1]*s+x[2]*c)*x[0];
  };

  inline double y3d(Point x) {
    const double c=cos(20*M_PI/180), s= sin(20*M_PI/180);
    return a/(a-x[1]*s+x[2]*c)*(x[1]*c+x[2]*s);
  };

  void line(Point x1, Point x2) {
    double x0=11/2, y0=15/2,s=0.5;
    char str[256];
    sprintf(str, "n %.4f %.4f %.4f %.4f m l s\nn %.4f %.4f %.4f %.4f m l s\n",
	    x0+x3d(x1)*s, -y0+y3d(x1)*s,  x0+x3d(x2)*s, -y0+y3d(x2)*s,
	    -x0+x1[0]*s,  -y0+x1[1]*s,   -x0+x2[0]*s,   -y0+x2[1]*s);
    *out << str;
    sprintf(str, "n %.4f %.4f %.4f %.4f m l s\nn %.4f %.4f %.4f %.4f m l s\n",
	    x0+x1[0]*s,    y0+x1[2]*s,    x0+x2[0]*s,    y0+x2[2]*s,
	    -x0+x1[1]*s,   y0+x1[2]*s,   -x0+x2[1]*s,    y0+x2[2]*s);
    *out << str;
  };

  void box(Point x, Point dx[3]) {
    Point X[8]={x -dx[0]/2 -dx[1]/2 -dx[2]/2, x -dx[0]/2 +dx[1]/2 -dx[2]/2,
		x +dx[0]/2 +dx[1]/2 -dx[2]/2, x +dx[0]/2 -dx[1]/2 -dx[2]/2,
		x -dx[0]/2 -dx[1]/2 +dx[2]/2, x -dx[0]/2 +dx[1]/2 +dx[2]/2,
		x +dx[0]/2 +dx[1]/2 +dx[2]/2, x +dx[0]/2 -dx[1]/2 +dx[2]/2};
    line(X[0], X[1]); line(X[1],X[2]); line(X[2],X[3]);line(X[3],X[0]);
    line(X[4], X[5]); line(X[5],X[6]); line(X[6],X[7]);line(X[7],X[4]);
    line(X[0], X[4]); line(X[1],X[5]); line(X[2],X[6]);line(X[3],X[7]);
   }
  void page(){ *out << "showpage\n"
		 "72 2.54 div dup scale 10 15 translate\n"
		 "0.001 setlinewidth\n-11 0 m 11 0 l\n0 15 m 0 -15 l s\n"; };
  void close(){ page(); out->close(); };
  void color(double r,double g,double b){
    *out << r << " " << g << " " << b << " setrgbcolor\n"; 
  };
};

///////////////////////////////////////////////////////////////////////////////
// This is a single silicon strip. The msx detectors are also treated as
// single strip.

class strip {
  Point x;           // middle Point of a strip
  Point dx[3];       // extension vectors
  double pathlength; // pathlength in strip for passing particle
public :
  strip(Point x, Point dx) {
    this->x = x;
    this->dx[0] = Point(dx[0], 0, 0);
    this->dx[1] = Point(0, dx[1], 0);
    this->dx[2] = Point(0, 0, dx[2]);
  }
  inline void move(Point x) { 
    this->x += x; 
  }
  inline void rotate(double theta) {
    x.rotate(theta); 
    for(int i=0;i<3;i++) 
      dx[i].rotate(theta);
  }
  inline void move(Point x, double theta) { 
    rotate(theta); move(x); 
  }
  inline void draw(siPlot p) {p.box(x,dx);}
  inline Point getX() const {return x;}

    //This returns intersection of a plane and a line:
    //The plane is defined by point x and extension vectors dx1 and dx2.
    //The line goes through point x0 and has direction xr.
    Point checkplane(Point x,Point dx1,Point dx2,Point x0,Point xr){
    Point normal(dx1[1]*dx2[2] - dx1[2]*dx2[1], 
		 dx1[2]*dx2[0] - dx1[0]*dx2[2], 
		 dx1[0]*dx2[1] - dx1[1]*dx2[0]);
    double xrnormal = xr*normal;
    if (xrnormal==0) return 0.0;
    double d = ((x - x0)*normal)/(xrnormal); 
    if (d<0) return 0.0;
    Point res = x0 + xr * d;
    if (fabs((res-x) * dx1) > dx1 * dx1/2) return 0.0;
    if (fabs((res-x) * dx2) > dx2 * dx2/2) return 0.0;
    return res;
  }
    //This returns the intersection of the front strip plane with a line
    //defined by going through x0 and having direction xr
    Point check(Point x0, Point xr, siPlot* si) 
  {
    Point xc[]={ checkplane(x-dx[2]/2,dx[0],dx[1],x0,xr),
		 checkplane(x+dx[2]/2,dx[0],dx[1],x0,xr),
		 checkplane(x-dx[0]/2,dx[2],dx[1],x0,xr),
		 checkplane(x+dx[0]/2,dx[2],dx[1],x0,xr),
		 checkplane(x-dx[1]/2,dx[0],dx[2],x0,xr),
		 checkplane(x+dx[1]/2,dx[0],dx[2],x0,xr)};
    
    int intersects = 0;
    pathlength = 0.0;
    Point xx[2];
    for (int i=0;i<6;i++) if (!xc[i].null()) xx[intersects++] =xc[i];
    if (intersects >  2) 
      cerr << "WARNING: More than 2 intersection points with a block!\n";
    if (intersects == 1) 
      cerr << "WARNING: Exact 1 intersection point with a block!\n";
    //return xx[0];
    if (intersects==2) { 
      if (debugSI) {
	si->color(0, 0.9, 0);
	draw(*si);
	si->color(1, 0, 0);
	si->line(xx[0], xx[1]);
	si->color(0, 0, 0);
      }
      pathlength = sqrt((xx[1]-xx[0])*(xx[1]-xx[0]));
      return xx[0];
    }
    return 0.0;
  };
  double getPathlength() { return pathlength; }
  //double getPath(Particle part){
    

};

class Absorber : public strip {
public:
  Absorber(Point dx, double theta=0) : strip(dx, Point(Absorber_width, Absorber_width, Absorber_thickness)) {}
};

class Scint : public strip {
public:
  Scint(Point dx, double theta=0) : strip(dx, Point(Scint_width, Scint_width, Scint_thickness)) {}
};

class MSX : public strip {
public:
  double thickness;
  MSX(Point dx, double theta=0) : strip(dx, Point(MSX_width, MSX_width, MSX_thickness)){}
  void setThickness(double thick) {this->thickness=thick;}
};

class VETO : public strip {
public:
    VETO(Point dx, double theta=0) : strip(dx, Point(VETO_width, VETO_height, VETO_thickness)) {}
};

class BB2 {
public:
  strip *hori[24];
  strip *vert[24];
  
  BB2(Point xx, double theta=0)
  {
    int i=0;
    Point dXvert(BB2_width/BB2_stripes, BB2_width, BB2_thickness);
    Point dXhori(BB2_width, BB2_width/BB2_stripes, BB2_thickness);
    for(double x=-BB2_width/2; x<BB2_width/2; x+=BB2_width/BB2_stripes) {
      Point 
	Xh(0,x+BB2_width/BB2_stripes/2,BB2_thickness), 
	Xv(x+BB2_width/BB2_stripes/2,0,0);
      hori[i] = new strip(Xh, dXhori); 
      vert[i] = new strip(Xv, dXvert); 
      hori[i]->rotate(theta);
      hori[i]->move(xx);
      vert[i]->rotate(theta);
      vert[i++]->move(xx);
    }
  }
  void move(Point x, double theta) {
    for(int i=0;i<BB2_stripes;i++) {
      hori[i]->rotate(theta); hori[i]->move(x);
      vert[i]->rotate(theta); vert[i]->move(x);
    }
  }
  void draw(siPlot p) {
    for(int i=0;i<BB2_stripes;i++) {
      hori[i]->draw(p);
      vert[i]->draw(p);
    }
  }
    //This class returns middle point of the BB2 strips
    Point checkstrip(Point x0, Point xr, siPlot* si) {
    Point resvert, res;
    for (int i=0;i<BB2_stripes;i++) {
      Point tmp = vert[i]->check(x0,xr,si);
      if (tmp[0]!=0 || tmp[1]!=0 || tmp[2]!=0) resvert = vert[i]->getX();
    } 
    if (resvert[0]!=0 || resvert[1]!=0 || resvert[2]!=0)   
      for (int i=0;i<BB2_stripes;i++) {
	Point tmp = hori[i]->check(x0, xr, si);
	if (!tmp.null()) {
	    res = Point(resvert[0], hori[i]->getX()[1], resvert[2]);
	}
      }
    return res;
  }
    //This class returns any point on the BB2 strips
    Point check(Point x0, Point xr, siPlot* si) {
	Point resvert, res;
	for (int i=0;i<BB2_stripes;i++) {
	    Point tmp = vert[i]->check(x0,xr,si);
	    if (tmp[0]!=0 || tmp[1]!=0 || tmp[2]!=0) resvert = tmp;
	} 
	for (int i=0;i<BB2_stripes;i++) {
	    Point tmp = hori[i]->check(x0, xr, si);
	    if (!tmp.null()) {
		res = Point(resvert[0], tmp[1], resvert[2]);
	    }
	}
	return res;
    }
};

class simSiliconDetector  : public simDetectorBase {
  Absorber *absorber;
  Scint *scint;
  BB2 *bb2;
  MSX *msx[5];
  VETO *veto;
  
  AquaNodeDouble *E_A[24], *E_B[24], *E_bb2, *E_MSX[5], *E_VETO, *simtheta, *simphi, *simenergy, *simmomentum;
  AquaNodeDouble *EnergyParticle, *MomentumParticle, *ThetaParticle, *PhiParticle, *Eloss_corr, *E_scint;
  AquaNodeDouble *si_theta,*si_phi;
  AquaNodeUShort *OK;
  siPlot *siplot;

public:
  simSiliconDetector(class AquaTree *outtree, Particle *p, double angle_, double oopang=0, double distance_=8.0, \
		     double centmom_=0.240, double simResolution_[8]=0, double simAcceptScale_=1.0);
  ~simSiliconDetector();
  virtual int energyLossCorrection(class target *, double x[3]) { return 0; }
  virtual int resolution(struct TargetCoord *){ return 0; }
  virtual int check(Particle vf, double x[3],
		    class target *, struct TargetCoord *, struct simFocalCoord *fc, modeltype ModelType);
  void move(Point x, double theta) {
    for(int i=0;i<5;i++) msx[i]->move(x,theta); 
    absorber->move(x, theta);
    scint->move(x,theta);
    bb2->move(x, theta);
    veto->move(x, theta);
  };
  void draw(siPlot p) { 
    absorber->draw(p);
    scint->draw(p);
    bb2->draw(p);
    for(int i=0;i<5;i++) msx[i]->draw(p); 
    veto->draw(p);
  };
  void check(double x[3],double dx[3]);
private:
  double angle,distance;
  double Absorber_distance;
  double ResSci, ResBB2, ResMSX[5], ResVeto;
  void setSteps(double E);
  double steps;
  // Get Silicon Detector coordinates and transfer them to
  //(x,y,z)-coordinate system of the particle
  //Better use initparticle
  Point getAngles(double XZ, double Y);  
  // Initalize FourVector from spectrometer coordinates (copied from FourVector class)
  FourVector initparticle(double m, double p, double th0, double th, double ph);
  double deadlayer(Particle P);
  bool accept(Particle vf);
};
