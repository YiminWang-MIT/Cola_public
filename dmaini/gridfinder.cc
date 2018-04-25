//
// Gridfinder.cc
//
// written by Harld Merkel 11.98
//
// improved by Ingo Ewald, Jan M. Friedrich and Thomas Pospischil 12.98
//
// ported to be used at M.I.T. by Michael O. Distler 12.98
//
// commented by Jan C. Bernauer 09.03
//
// Input format
//
//   First line:  
//       Reaction BeamEnergy[GeV] SpecAngle[deg] RefMom[GeV] TargetZ[cm]
//   Other lines: 
//       theta0[mrad] phi0[mrad] x[mm] th[mrad] y[mm] phi[mrad] Eloss[MeV]
//
//   Valid reactions: H(e',A:e)p
//                    H(e',B:e)p
//                    H(e',C:e)p
//                    H(e',A:eB:p)
//
// Output format = input format of "dmaini"
//
//   All lines:   delta, x0, th0, y0, ph0, x, th, y, ph, nh
//
#define _DEFAULT_SOURCE 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <iostream>
#ifdef HAVE_ISTREAM
#include <istream>
#endif
#ifdef HAVE_OSTREAM
#include <ostream>
#endif
#include <fstream>
#include <iosfwd>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#ifndef __hpux__
#include <getopt.h>
#endif

#include "../Cola/Masses.h"
#include "HMBook/hmbook.h"
#include "HMBook/hmplot.h"

const double rad      = M_PI/180;

inline double sqr(double a) { return a*a;}


// Correction calculates the real coordinates of the vertex an the actual scattering angle

void Correction(double th, double phi, double thSpec, double z, double l,
	   double &x0, double &th0, double &y0, double &phi0, 
	   double &theta)
{
    // Calculate positions in spectrometer-space
    double x[] = {0, z * sin(thSpec), z * cos(thSpec)};  // target position (by rotating)
    double c[] = {l * tan(th/1e3), l * tan(phi/1e3), l}; // colli hole position 
    double diff[] = {c[0]-x[0], c[1]-x[1], c[2]-x[2]};   // direction

    x0    = c[0] - c[2] / diff[2] * diff[0];  //Backproject seen position to center of target
    th0   = atan2(diff[0], diff[2]) * 1e3;    // corresponding angle to hole
    y0    = c[1] - c[2] / diff[2] * diff[1];  // as above
    phi0  = atan2(diff[1], diff[2]) * 1e3;


    // rotate back to beamline-space
    double lab[]  = {diff[0], 
		     -sin(thSpec) * diff[2] + cos(thSpec) * diff[1], 
		     cos(thSpec) * diff[2] + sin(thSpec) * diff[1]};
    theta = acos(lab[2] / sqrt(sqr(lab[0]) + sqr(lab[1]) + sqr(lab[2]))); //calculate real scattering angle 
}


// finds the maximum in a histogramm
double findMaximum(HIST *id, double &x, double &y) 
{
    double max = 0;
    double dx = (id->maxx - id->minx) / id->nx;
    double dy = (id->maxy - id->miny) / id->ny;
    for(double xx = id->minx + dx/2; xx < id->maxx; xx += dx)
	for(double yy = id->miny + dy/2; yy < id->maxy; yy += dy)
	    if (HMExtract2(id,xx,yy)>max) max = HMExtract2(id,x = xx,y = yy);
    return max;
}


// removes the maximum (recursivly, floodfill)
void removeMaximum(HIST *id, double x, double y)
{
    double max = HMExtract2(id, x, y);
    double dx = (id->maxx - id->minx) / id->nx;
    double dy = (id->maxy - id->miny) / id->ny;
    HMFill(id, x, y, -max);
    if (HMExtract2(id, x + dx, y     ) < max) removeMaximum(id, x + dx, y);
    if (HMExtract2(id, x - dx, y     ) < max) removeMaximum(id, x - dx, y);
    if (HMExtract2(id, x     , y + dy) < max) removeMaximum(id, x     , y + dy);
    if (HMExtract2(id, x     , y - dy) < max) removeMaximum(id, x     , y - dy);
    if (HMExtract2(id, x + dx, y + dy) < max) removeMaximum(id, x + dx, y + dy);
    if (HMExtract2(id, x + dx, y - dy) < max) removeMaximum(id, x + dx, y - dy);
    if (HMExtract2(id, x - dx, y + dy) < max) removeMaximum(id, x - dx, y + dy);
    if (HMExtract2(id, x - dx, y - dy) < max) removeMaximum(id, x - dx, y - dy);
}


// container for data read from file, a linked list
class data {
public:
    static class data *first; //first data 
    class data *next; // pointer to next data
    double th0, phi0, x, th, y, phi, ergloss;
    data(std::istream *in) {
	*in >> th0 >> phi0 >> x >> th >> y >> phi >> ergloss;
	next = first;    
	first = this;
    };
};
class data *data::first = NULL;


// a vertex in the grid
class gridpoint {
public:
    double ergloss;
    /// target coordinates 
    double th, ph;         
    double th_uncorr, ph_uncorr;         
    /// focal plane coordinates
    double x, theta, y, phi; 
    int ok;
};


// the actual class for grid finding
class sieveslit {
public:
    int weight;
    int bins;
    double xOff, *yOff, *phOff, *thOff;
    int    yOffDim, phOffDim, thOffDim;
    double thrange, phrange, dth, dph;
    int nth, nph;
    double gridth, gridph, l;
    double sigma;
    double lowestmaximum;
    double cut;
    gridpoint *grid, *soll;
    int ngrid; // numer of grids

    sieveslit(int nth, int nph, double gth, double gph, double laenge,
	      int Bins, double THrange, double PHrange);
    void readOffsets(char *file); // read offsets from a file

    // find the hoels in the histogramm
    void findGrid(std::istream *in, double specTheta, double targetz, 
		  double th_shift, double ph_shift,
		  double th_ph_shift, double ph_th_shift,
		  double th_scale, double ph_scale);
    // find the corresponding gridpoint using defaults as a starting point
    int  findNext(double th, double ph);
    void setParam(double cutRadius, double lowestMaximum);
    void output(std::ostream &out, double m, double inel, int mirror,
		double E0, double th, double cent, double z);
};


void sieveslit::readOffsets(char *file) {
    std::ifstream in(file);
    in >> xOff;
    in >> thOffDim; thOff = new double[thOffDim];
    for (int i=0;i<thOffDim;i++)in >> thOff[i];
    in >> yOffDim; yOff = new double[yOffDim];
    for (int i=0;i<yOffDim;i++)in >> yOff[i];
    in >> phOffDim; phOff = new double[phOffDim];
    for (int i=0;i<phOffDim;i++)in >> phOff[i];
    in.close();
}

void sieveslit::setParam(double cutRadius, double lowestMaximum)
{
    cut = cutRadius;
    lowestmaximum = lowestMaximum;
}


sieveslit::sieveslit(int Nth, int Nph, double gth, double gph, double laenge,
		     int Bins, double THrange, double PHrange)
{
    // some setup
    bins = Bins;
    l = laenge;
    thrange = THrange;
    phrange = PHrange;
    dth = thrange/bins*2;
    dph = phrange/bins*2;
    nth = Nth; nph = Nph; gridth = gth; gridph = gph;
    sigma = 1.75;
    grid = new gridpoint[nth*nph];
    soll = new gridpoint[nth*nph];
    xOff = 0.0;
    yOff = phOff = thOff = NULL;
    yOffDim = phOffDim = thOffDim = 0;
    for(int ith = -(nth-1)/2; ith <= (nth-1)/2 ; ith++) // set up all should-be gridpoints
	for(int iph = -(nph-1)/2; iph <= (nph-1)/2 ; iph++) {
	    soll[ith + (nth-1)/2 + (iph + (nph-1)/2) * nth].th_uncorr = ith*gridth; 
	    soll[ith + (nth-1)/2 + (iph + (nph-1)/2) * nth].ph_uncorr = iph*gridph; 
	    soll[ith + (nth-1)/2 + (iph + (nph-1)/2) * nth].ok = -1; 
	}
}

int sieveslit::findNext(double th, double ph) // find the next grid
{
    double min  = 1e32;
    int    imin = -1;
    for (int i=0; i<ngrid; i++)  // look at all grids
	if (sqr((th - grid[i].th)/gridth) + sqr((ph - grid[i].ph)/gridph) < min) {
	    min  = sqr((th-grid[i].th)/gridth) + sqr((ph-grid[i].ph)/gridph); // take the one with minimal distants
	    imin = i;
	}
    return min < 0.25 ? imin : -1;
}

// fills a histogram with a 2D-Gauss
void FillGauss2D(HIST *id, double sigma, double x_m, double y_m) { 
    double dx=(id->maxx - id->minx) / id->nx;
    double dy=(id->maxy - id->miny) / id->ny;
    for(double xx = id->minx+dx/2; xx<id->maxx; xx += dx)
	for(double yy = id->miny+dy/2; yy<id->maxy; yy += dy)
	    if ( (fabs(xx-x_m) < sigma*2)  &&  (fabs(yy-y_m) < sigma*2) )
		HMFill(id,xx,yy,exp(-(sqr(xx-x_m)+sqr(yy-y_m))/2/sqr(sigma)));
    return;
}

//Zeros a histogram
void FillZero2D(HIST *id) {
    for (int i=0; i<id->nx; i++)
	for (int j=0; j<id->ny; j++) 
	    id->data[i*id->ny + j] = 0;
    return;
}


// here we go!
void sieveslit::findGrid(std::istream *in, double specth, double z, 
		    double th_shift, double ph_shift,
		    double th_ph_shift, double ph_th_shift,
		    double th_scale, double ph_scale)
{
    HIST *id  = HMBook2("Gaussian Smooth", "[q]_0!", "[f]_0!", "",
			"mrad", "mrad","",
			bins, -thrange, thrange, bins, -phrange, phrange);
    HIST *id2 = HMBook2("Grid",           "[q]_0!", "[f]_0!", "", 
			"mrad", "mrad","",
			bins, -thrange, thrange, bins, -phrange, phrange);

    HIST *th_ph = HMBook2("Gaussian Smooth in [q] and [f]", 
			  "[q]", "[f]", "", "mrad", "mrad","",
			  40, 600, 1000, 16, -80, 80);
    HIST *x_y   = HMBook2("Gaussian Smooth in x_vdc! and y_vdc!", 
			  "x_vdc!", "y_vdc!", "", "mm", "mm","",
			  150, -1000, 2000, 32, -200, 200);

    double th, ph;
    for(int ith = -(nth-1)/2; ith <= (nth-1)/2 ; ith++)
	for(int iph = -(nph-1)/2; iph <= (nph-1)/2 ; iph++) {
	    double x0, th0, y0, phi0, theta;
	    Correction(soll[ith + (nth-1)/2 + (iph + (nph-1)/2) * nth].th_uncorr,
		       soll[ith + (nth-1)/2 + (iph + (nph-1)/2) * nth].ph_uncorr,
		       specth*rad, z,l,x0,th0,y0,phi0,theta); //Correct theroretical to actual coords
	    soll[ith + (nth-1)/2 + (iph + (nph-1)/2) * nth].th = th0; 
	    soll[ith + (nth-1)/2 + (iph + (nph-1)/2) * nth].ph = phi0; 
	}
  
    int    count=0;
    while (!in->eof()) { // read all data
	data *d = new data(in);
	th = d->th0;
	ph = d->phi0;
	HMFill(id2, th, ph, 1);
	FillGauss2D(id, sigma, th, ph); 
	if (!(count++ % 100)) std::cout << "\r" << (count-1) << std::flush;
    }
    std::cout << "\r" << count << std::endl;
  
    HMScatter(id2);
    HMGray(id);

    ngrid = 0;
    double max = findMaximum(id, th, ph); // find the first maximum
    do {
	grid[ngrid].th = th; // save the maximum as a gridpoint
	grid[ngrid].ph = ph;
	ngrid ++;
	removeMaximum(id, th, ph); //remove the maximum
	const double da=M_PI/40;
	for (double a=0;a<2*M_PI;a+=da)
	    HMLine(th+cut*cos(a), ph+cut*sin(a), th+cut*cos(a+da), ph+cut*sin(a+da));
    }  while (ngrid<nth*nph && max*lowestmaximum<findMaximum(id, th, ph)); // repeat until no more or enough is found

    int j = (nth-1)/2+(nph-1)/2*nth; // now, try to identify the corresponding should-be grids
    for(j=0; j<nth*nph; j++) {
	int i = soll[j].ok = 
	    findNext((soll[j].th+th_shift+th_ph_shift*soll[j].ph)*th_scale, 
		     (soll[j].ph+ph_shift+ph_th_shift*soll[j].th)*ph_scale);
// look at all the should-bes and find the nearest grid from above
    
	if (i>=0) {
	    for (double a=0;a<2*M_PI;a+=M_PI/20)
		HMLine(soll[j].th+1.0*cos(a), 	      soll[j].ph+1.0*sin(a), 
		       soll[j].th+1.0*cos(a+M_PI/20), soll[j].ph+1.0*sin(a+M_PI/20));
	    HMLine(soll[j].th, soll[j].ph, grid[i].th, grid[i].ph); // draw some lines
	}
    }

    int index2 = (nth-1)/2 + (nph-1)/2 * nth;
    int index3 = (nth-1)   + (nph-1)   * nth;

    float th_min  = soll[0].th;        float ph_min  = soll[0].ph;
    float th_mean = soll[index2].th;   float ph_mean = soll[index2].ph;
    float th_max  = soll[index3].th;   float ph_max  = soll[index3].ph;

    ph_min = (ph_min < id->miny) ? id->miny : ph_min;
    ph_max = (ph_max > id->maxy) ? id->maxy : ph_max;

    HMLine(th_min, ph_min,  th_max, ph_min);
    HMLine(th_min, ph_mean, th_max, ph_mean);
    HMLine(th_min, ph_max,  th_max, ph_max);

    HMLine(th_min,  ph_min, th_min,  ph_max);
    HMLine(th_mean, ph_min, th_mean, ph_max);
    HMLine(th_max,  ph_min, th_max,  ph_max);

    
    // now, we try to optimize the first guess
    int hole_nr=0;
    for(int i=0;i<nth*nph;i++) {
	if (soll[i].ok>=0) {
	    int ok = soll[i].ok;
	    grid[ok].x = grid[ok].theta = grid[ok].y = grid[ok].phi = 0;
	    grid[ok].ok = 0;
	    grid[ok].ergloss =0;
	    data *d=data::first;
	    do {
		if (sqr(d->th0-grid[ok].th) +sqr(d->phi0-grid[ok].ph)< sqr(cut)) { // fill in all points which are near enough
		    grid[ok].ok++;
		    grid[ok].ergloss += d->ergloss;
		    FillGauss2D(x_y,   30, d->x,  d->y);  // make them big
		    FillGauss2D(th_ph, 15, d->th, d->phi);
		}
	    } while ((d=d->next));
	    if (!grid[ok].ok)
		std::cerr << "ERROR: gridfinder marked empty hole!" << std::endl;
	    double x_find, y_find, theta_find, phi_find;
	    findMaximum(x_y,   x_find,  y_find);  //now look for the maximum in this high res picture
	    findMaximum(th_ph, theta_find, phi_find); // " 
	    grid[ok].ergloss /= grid[ok].ok; // ??? Some weight 

	    HIST *th_ph_local = HMBook2("Gaussian Smooth in [q] and [f]", 
					"[q]", "[f]", "", "mrad", "mrad","",
					40, theta_find-20, theta_find+20, 
					40,   phi_find-20,   phi_find+20); 
	    HIST *x_y_local   = HMBook2("Gaussian Smooth in x_vdc! and y_vdc!", 
					"x_vdc!", "y_vdc!", "", "mm", "mm","",
					70, x_find-35, x_find+35, 
					40, y_find-20, y_find+20);
	    d=data::first;
	    do {
		if (sqr(d->th0-grid[ok].th) +sqr(d->phi0-grid[ok].ph)< sqr(cut)) { // and again!
		    FillGauss2D(x_y_local,   8, d->x,  d->y);
		    FillGauss2D(th_ph_local, 5, d->th, d->phi);
		}
	    } while ((d=d->next));
	    findMaximum(x_y_local,   x_find,  y_find);
	    findMaximum(th_ph_local, theta_find, phi_find);
	    grid[ok].x     = x_find; // save the new coords
	    grid[ok].theta = theta_find;
	    grid[ok].y     = y_find;
	    grid[ok].phi   = phi_find;

	    // HMSurface(x_y);
	    // HMSurface(th_ph);
	    // HMSurface(x_y_local);
	    // HMSurface(th_ph_local);

	    delete th_ph_local;
	    delete x_y_local;

	    FillZero2D(x_y);
	    FillZero2D(th_ph);

	    std::cout << "\r" << ++hole_nr << std::flush;
	}
    }
    std::cout << std::endl << std::flush;
}

void 
sieveslit::output(std::ostream &out, double m, double inel, int mirror,
		  double E0, double th, double ref, double z)
{
    double momentum, off;
    for (int i=0; i<nth*nph; i++) 
	if (soll[i].ok >= 0) {      
	    double x0, th0, y0, phi0, theta;
	    Correction(soll[i].th_uncorr, soll[i].ph_uncorr, 
		       th*rad, z,l, x0, th0, y0, phi0, theta);
	    if (mirror) {
		double theta_e = 2 * atan(1 / tan(theta) / (1 + E0 / m)); 
		double E_e     = E0 / (1 + 2 * E0 / m * sqr(sin(theta_e/2)));
		double E_p     = sqrt((E0*E0 + E_e*E_e - 2 * E0 * E_e * cos(theta_e)) 
				      + m*m) - grid[soll[i].ok].ergloss/1000.0;
		momentum = sqrt(E_p*E_p - m*m);
	    } else 
		momentum = (E0-inel-0.5*sqr(inel)/m) /
		    (1.0 + 2.0 * E0 / m * sqr(sin(theta/2)))
		    - grid[soll[i].ok].ergloss/1000.0;
	    out.setf(std::ios::fixed, std::ios::floatfield);
	    out << std::setw(8) << std::setprecision(4)
		<< (momentum-ref)/ref*100 << " ";
	    out << std::setw(6) << std::setprecision(3) <<   x0 << " ";
	    out << std::setw(7) << std::setprecision(3) <<  th0 << " ";
	    out << std::setw(7) << std::setprecision(4) <<   y0 << " ";
	    out << std::setw(7) << std::setprecision(3) << phi0 << " ";
	    out << std::setw(7) << std::setprecision(3)
		<< (grid[soll[i].ok].x - xOff)*0.1 << " ";
	    off = 0;
	    for (int j=0;j<thOffDim;j++) 
		off += thOff[j] * pow(grid[soll[i].ok].x - xOff, j);
	    out << std::setw(8) << std::setprecision(3)
		<< grid[soll[i].ok].theta - off << " ";
	    off = 0;
	    for (int j=0;j<yOffDim;j++) 
		off += yOff[j] * pow(grid[soll[i].ok].x - xOff, j);
	    out << std::setw(7) << std::setprecision(3)
		<< (grid[soll[i].ok].y - off)*0.1 << " ";
	    off = 0;
	    for (int j=0;j<phOffDim;j++) 
		off += phOff[j] * pow(grid[soll[i].ok].x - xOff, j);
	    out << std::setw(8) << std::setprecision(4)
		<< grid[soll[i].ok].phi - off << " ";
	    out.setf(std::ios::right, std::ios::adjustfield);
	    out << std::setw(4) << (weight ? 1 : grid[soll[i].ok].ok) << std::endl;
	}
}

sieveslit A( 7, 11, 23.42,  19.52,   64.0, 100, 80.0, 120.0);
sieveslit B(15,  5, 10.09,  10.09,  294.5, 100, 80.0,  30.0);
sieveslit C( 9, 11, 19.20,  22.00,   45.5, 100, 80.0, 120.0);
// sieveslit O( 5,  5, 22.375, 10.675, 160.0, 100, 60.0,  35.0);
sieveslit O( 7, 5, 23.70, 9.48, 107.2, 150, 90.0,  30.0);
sieveslit O2k( 7, 5, 16.9, 6.65, 150.654, 150, 90.0,  30.0);

struct Reaction {
    char *string;
    sieveslit *sieve;
    int Mainz;
    int thetaSign;
    int mirror;
    double m_target;
    double inelastic;
} reactions[] = {
    { "1H(e,A:e')p",	  &A,   1, -1, 0, m_H1,    0.0     },
    { "12C(e,A:e')12C",	  &A,   1, -1, 0, m_C12,    0.0     },
    { "1H(e,B:e')p",	  &B,   1,  1, 0, m_H1,    0.0     },
    { "1H(e,C:e')p",	  &C,   1,  1, 0, m_H1,    0.0     },
    { "1H(e,A:e'B:p)",	  &B,   1,  1, 1, m_H1,    0.0     },
    { "1H(e,O:e')p",	  &O,   0, -1, 0, m_H1,    0.0     },
    { "12C(e,O':e')12C",	  &O2k, 0, -1, 0, m_C12,   0.0     },
    { "12C(e,O:e')12C",	  &O,   0, -1, 0, m_C12,   0.0     },
    { "12C(e,O:e')12C*",	  &O,   0, -1, 0, m_C12,   4.44E-3 },
    { "181Ta(e,O:e')181Ta", &O,   0, -1, 0, m_Ta181, 0.0     }
};
int Nreactions = sizeof(reactions)/sizeof(struct Reaction);
  
int
main(int argc, char *argv[])
{
    std::istream *in  = &std::cin;
    std::ostream *out = &std::cout;
    char *plotfile = "gridfinder.ps"; 
    char *infile = "stdin";
    int weight = 0;
    int out_to_file = 0;
    char *outfilename =NULL;
    double cutRadius = 3.5;
    double lowestMaximum = 1.0/9;
    char opt;
    char *offsets = NULL;
    double th_shift   =0.0, ph_shift=0.0;    // shift assignment of the holes 
    double th_ph_shift=0.0, ph_th_shift=0.0; // shift assignment of the holes 
    double th_scale = 1.0; double ph_scale = 1.0;// scaling factors of Sieve Slit

    while ((opt = getopt(argc, argv, "i:o:p:hwr:c:O:x:y:a:b:f:g:")) != EOF) {
	switch (opt) {
	    case '?':
	    case ':':
	    case 'h':  std::cerr << 
				"\nUSAGE:\t" << argv[0] << " [options]\n\n"
				"Options:\n"
				"\t-i <file>\tInput file instead of stdin\n"
				"\t-o <file>\tOutput file instead of stdout\n"
				"\t-p <file>\tPostscript output for debug/check\n"
				"\t-h       \tThis help\n"
				"\t-r radius\tCut radius in mrad, default: 3.5mrad\n"
				"\t-c frac  \tFraction of lowest maximum, default 1/9\n"
				"\t-O <file>\ttake Offsets from file\n"
				"\t-w       \tWeight with 1 instead of number of events\n"
				"\t-x <th_shift>\tshift hole assignment in theta0 (mrad)\n"
				"\t-y <ph_shift>\tshift hole assignment in phi0 (mrad)\n"
				"\t-a <th_ph-sh>\tlike x, but phi0-dependent\n"
				"\t-b <ph_th-sh>\tlike y, but theta0-dependent\n"
				"\t-f <th_scale>\tscaling factor in theta0\n"
				"\t-g <ph_scale>\tscaling factor in phi0\n\n"
				"compiled "__DATE__" "__TIME__"\n\n"; 
		exit(-1);
	    case 'i' : in  = (std::istream *) new std::ifstream(infile=optarg); break;
	    case 'o' : out_to_file =1; outfilename=optarg; break;
	    case 'p' : plotfile      = optarg;       break;
	    case 'w' : weight        = -1;           break;
	    case 'r' : cutRadius     = atof(optarg); break;
	    case 'c' : lowestMaximum = atof(optarg); break;
	    case 'O' : offsets = optarg;             break;
	    case 'x' : th_shift = atof(optarg);      break;
	    case 'y' : ph_shift = atof(optarg);      break;
	    case 'a' : th_ph_shift = atof(optarg);   break;
	    case 'b' : ph_th_shift = atof(optarg);   break;
	    case 'f' : th_scale    = atof(optarg);   break;
	    case 'g' : ph_scale    = atof(optarg);   break;
	    default : std::cerr << "ERROR: Can't analyze options!" << std::endl;
		exit(-1); break;
	}
    }
  
    char   reactionStr[255];
    double E0, spectrometerTheta, refMomentum, targetz;

    *in >> reactionStr;
    *in >> E0 >> spectrometerTheta >> refMomentum >> targetz;

    sieveslit *sieve = NULL;
    Reaction *reaction = NULL;
    for (int i=0; i<Nreactions; i++) {
	if (!strcmp(reactionStr,reactions[i].string)) {
	    reaction = reactions+i;
	    sieve = reactions[i].sieve;
	}
    }

    HMPlotFile(plotfile);
    char title[255];
    char* rtmp, *rundate = infile;
    if ((rtmp = strstr(infile, "run"))) rundate = rtmp+3;
    if ((rtmp = strstr(infile, "run_"))) rundate = rtmp+4;
    if ((rtmp = strchr(rundate,'.'))) *rtmp = 0;
    sprintf(title,"%s (run %s)  at %.1f     [q]=%.1f^o!  z!=%.2fcm\n",
	    reaction->string, rundate,
	    refMomentum*1000, spectrometerTheta, targetz);
    HMZone(1,2,1);

    HMTitle(title);

    sieve->readOffsets(offsets);
    sieve->setParam(cutRadius, lowestMaximum);
    sieve->weight = weight;

    sieve->findGrid(in, reaction->thetaSign*spectrometerTheta, targetz, th_shift,
		    ph_shift, th_ph_shift, ph_th_shift, th_scale, ph_scale);

    if (out_to_file) out = (std::ostream *) new std::ofstream(outfilename);
    sieve->output(*out, reaction->m_target, reaction->inelastic,
		  reaction->mirror, E0,
		  reaction->thetaSign*spectrometerTheta, refMomentum, targetz);

    HMClose();
    return 0;
}









