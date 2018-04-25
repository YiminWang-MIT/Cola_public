//								      -*-c++-*-
// AQUA: data AcQUisition for A1 experiments
//
// Copyright (c) 2001
//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
// $Id: pdcD.cc 2348 2009-10-09 10:07:29Z merkel $
//

#include <../Chamber/vdcRegression.h>
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <unistd.h>
#include "pdcD.h"
#include "spline.h"
#include "fitspline.h"
#include <ctype.h>

#define MAXPOW 18
#define fast_pow(a,i) pow_table[i] // Speed optimization (P. Merle)
int pow_table[MAXPOW];

void
init_pow_table()
{
  for (int i=0; i < MAXPOW; i++)  pow_table[i] = (int)pow(3,i);
}

void 
pdcDplane::InitSplines() {
  for (int i=0; i<SAMPLES; i++)
    stuetze[i] = varParam[i]
      = CELL_LENGTH * (1.0 - i / (SAMPLES-1.0));
  tmin = (maxcells==8) ? minX : minY;
  tmax = (maxcells==8) ? maxX : maxY;
}

void 
pdcDplane::ReadSplines(char* filename) {
  std::ifstream Eingabe(filename);
  for (int i=0; i<SAMPLES+2; i++) Eingabe >> varParam[i];
  Eingabe.close();
  if (varParam[3]==0) {
    std::cerr << "ERROR: rundb file(s) D.SNO.{X|Y} seem to be " <<
      "corrupt or non-existent" << std::endl;
    exit(0);
  }
  for (int i=0; i<SAMPLES; i++) stuetze[i] = varParam[i];
  tmin = (int) varParam[SAMPLES];
  tmax = (int) varParam[SAMPLES+1];

  if (flags & PDCD_DEBUG_FLAG) {
    std::cout << " *** ReadSplines *** \n mintime=" << varParam[SAMPLES] << 
      " maxtime=" << varParam[SAMPLES+1] << "\n";
    for (int i=0; i<SAMPLES+2; i++) 
      std::cout << " SN_" << i << "= " << varParam[i] << "\n" ;
  }
}

pdcD::pdcD(const char *aname, AquaTree *atree, const char *oname, 
  	   AquaTree *otree, int pdcflags)
  : pdcDbase(NULL, atree, otree, pdcflags)
{
  if (getenv("NDAT")) std::cout << "NDAT externally given. " << std::endl;
  if (DDR_CORRECTION_X) std::cerr << "DDR_CORRECTION_X = 1 " << std::endl;
  if (DDR_CORRECTION_Y) std::cerr << "DDR_CORRECTION_Y = 1 ; DDRSCALE = " <<
			  DDRSCALE << std::endl;

  char name[strlen(aname)+3];
  char nam2[strlen(oname)+8];
  sprintf(name, "%s.x", aname);
  sprintf(nam2, "%s.planeX", oname);
  planeX = new pdcDplane(this, Xcells, name, tree, nam2, onl, flags);
  sprintf(name, "%s.y", aname);
  sprintf(nam2, "%s.planeY", oname);
  planeY = new pdcDplane(this, Ycells, name, tree, nam2, onl, flags);

  sprintf(nam2, "%s.pdcOK", oname); pdcOK = (AquaNodeInt *)onl->find(nam2);
  sprintf(nam2, "%s.x", oname);         x = (AquaNodeDouble *)onl->find(nam2);
  sprintf(nam2, "%s.y", oname);         y = (AquaNodeDouble *)onl->find(nam2);
  sprintf(nam2, "%s.theta", oname);    th = (AquaNodeDouble *)onl->find(nam2);
  sprintf(nam2, "%s.phi", oname);      ph = (AquaNodeDouble *)onl->find(nam2);
  sprintf(nam2, "%s.dx", oname);       dx = (AquaNodeDouble *)onl->find(nam2);
  sprintf(nam2, "%s.dy", oname);       dy = (AquaNodeDouble *)onl->find(nam2);
  sprintf(nam2, "%s.dtheta", oname);  dth = (AquaNodeDouble *)onl->find(nam2);
  sprintf(nam2, "%s.dphi", oname);    dph = (AquaNodeDouble *)onl->find(nam2);

  init_pow_table();
}

pdcD::~pdcD()
{
  delete planeX;
  delete planeY;
}

int pdcD::handle()
{
  double a2;

  Reset();

  planeX->handle();
  planeY->handle();

  if (planeX->chisqr < 1e5 && planeY->chisqr < 1e5) { //&&
    *pdcOK = 1;

    *x  = -147.2 + planeX->a - 113.72 * planeX->b; // mm
    *th = atan(planeX->b) * 1000.0;                // mrad
    *y  =  -36.8 + planeY->a -  36.72 * planeY->b; // mm
    *ph = atan(planeY->b) * 1000.0;                // mrad

    a2 = planeX->l22*planeX->n - planeX->l12*planeX->l12;
    if (a2 == 0.0) *dx = *dth = FLT_MAX;
    else {
      *dx  = sqrt(planeX->chisqr*planeX->l22/a2);
      *dth = sqrt(planeX->chisqr*planeX->n/a2);
      *dth = *dth/(1+planeX->b*planeX->b) * 1000.0; // mrad
    }

    a2 = planeY->l22*planeY->n - planeY->l12*planeY->l12;
    if (a2 == 0.0) *dy = *dph = FLT_MAX;
    else {
      *dy  = sqrt(planeY->chisqr*planeY->l22/a2);
      *dph = sqrt(planeY->chisqr*planeY->n/a2);
      *dph = *dph/(1+planeY->b*planeY->b) * 1000.0; // mrad
    }
  } else *pdcOK = 0;

  return 0;
}

int pdcD::Reset()
{
  planeX->Reset();
  planeY->Reset();

  return 0;
}

pdcDplane::pdcDplane(pdcDbase *father, int maxCells,
		     const char *aname, AquaTree *atree,
		     const char *oname, AquaTree *otree, int pdcflags)
  : pdcDbase(father, atree, otree, pdcflags)
{
  cell = new pdcDcell *[maxcells = maxCells];
  for (int c=0; c<maxCells; c++) cell[c] = new pdcDcell(this, flags);
  subTree = tree->getShortSubTree(aname, 0);
  data = new wiretime[8*maxcells];
  length = new double[8*maxcells];
  calibTab = new timepathlength[CALIBTABSIZE];
  stuetze = new double[SAMPLES];
  varParam = new double[SAMPLES+2];

  // std::cerr << (maxcells==8 ? 'x' : 'y') << std::endl;
  char *sno = NULL;
  sno = (maxcells==8) ? rundb.pdc.x_snofile : rundb.pdc.y_snofile;
  if (sno==NULL) InitSplines(); else ReadSplines(sno);
  ndata = nr_of_iterations = 0;
  if (maxcells==8 && DDR_CORRECTION_X) {
    if (minX) tmin=minX; if (maxX) tmax=maxX;} // overwrite tmin/tmax (X)
  if (maxcells==2 && DDR_CORRECTION_Y) {
    if (minY) tmin=minY; if (maxY) tmax=maxY;} // overwrite tmin/tmax (Y)

  if (flags & PDCD_DEBUG_FLAG) {

    std::cerr << "tma  = "      << rundb.D.tma.filename << std::endl;
    if (sno)
      std::cerr << "sno  = "      << sno       << std::endl;
    std::cerr << "NDAT = "      << NDAT      << std::endl;
    std::cerr << "tmin = "      << tmin      << std::endl;
    std::cerr << "tmax = "      << tmax      << std::endl;

    for (int i=0; i<SAMPLES+2; i++) 
      std::cout << " SN_" << i << "= " << varParam[i] << std::endl;
  }
  
  if (pdcflags & PDCD_HISTO_FLAG) {
    char hname[64], xy = aname[strlen(aname)-1];
    sprintf(hname, "Spec. D/%c-layer/Drift Time", xy);  timeH = HMFind(hname);
    sprintf(hname, "Spec. D/%c-layer/Nr of Wire", xy);  wireH = HMFind(hname);
    sprintf(hname, "Spec. D/%c-layer/Drift Length", xy); lenH = HMFind(hname);
    sprintf(hname, "Spec. D/%c-layer/Tagged Wire", xy);  tagH = HMFind(hname);
    sprintf(hname, "Spec. D/%c-layer/Efficiency", xy);
    
    if ((effH = HMFind(hname))) {
      effH->unity = "";
      effH->labely = "Efficiency";
      effH->divide = tagH;
    }
    sprintf(hname, "Spec. D/%c-layer/Residuum", xy);     resH = HMFind(hname);
    sprintf(hname, "Spec. D/%c-layer/Staggering", xy); staggH = HMFind(hname);
  } else timeH = wireH = tagH = effH = lenH = resH = staggH = NULL;

  char name[strlen(oname)+16];
  AquaNode **raw_tmp = (AquaNode **)raw_time;
  sprintf(name, "%s.raw_time[%%d]", oname);
  onl->multifind(raw_tmp, 0, pdcDonlineLen-1, name);
  raw_tmp = (AquaNode **)raw_wire;
  sprintf(name, "%s.raw_wire[%%d]", oname);
  onl->multifind(raw_tmp, 0, pdcDonlineLen-1, name);
  raw_tmp = (AquaNode **)raw_len;
  sprintf(name, "%s.raw_length[%%d]", oname);
  onl->multifind(raw_tmp, 0, pdcDonlineLen-1, name);
  sprintf(name, "%s.multi", oname);     multi = (AquaNodeInt *)onl->find(name);
  sprintf(name, "%s.error", oname);     error = (AquaNodeInt *)onl->find(name);
  sprintf(name, "%s.pattern", oname); pattern = (AquaNodeInt *)onl->find(name);

  sprintf(name, "%s.x", oname);          x = (AquaNodeDouble *)onl->find(name);
  sprintf(name, "%s.slope", oname);     sl = (AquaNodeDouble *)onl->find(name);
  sprintf(name, "%s.chi_sqr", oname); chi2 = (AquaNodeDouble *)onl->find(name);

}

pdcDplane::~pdcDplane()
{
  for (int c=0; c<maxcells; c++) delete cell[c];
  delete [] cell;
  delete subTree;
  delete [] data;
  delete [] length;
  delete [] calibTab;
}

//
// Straight Line Fit, returns chisquare
//
double straightLineFit(double x[], double y[], int n, 
		       double &a, double &b, double &L12, double &L22)
{
  double L11=n, g1=0, g2=0, g3=0;
  L12 = L22 = 0;
  for(int i=0; i<n; i++) {
    L12 += x[i];
    L22 += x[i] * x[i]; 
    g1  += y[i];
    g2  += x[i] * y[i];
    g3  += y[i] * y[i];
  }
  double D = L11 * L22 - L12 * L12;
  if (D==0) return 1E10;
  a = (g1*L22 - g2*L12) / D;                     // Result: a: offset
  b = (g2*L11 - g1*L12) / D;                     //         b: slope
  return a*a*n+2*(a*b*L12-b*g2-a*g1)+b*b*L22+g3; //         chisquare
}

//  try one combination of wire sides, given by int combi. 
//  Every base 3 digit is for one wire 0=right, 1=left, 2=ommit;
//
double FitCombi(int combi, double x[], double y[], double l[], 
		int &n, double &a, double &b, double &l12, double &l22) 
{
  double yy[n], xx[n];
  int nn = 0;
  for (int i=0; i<n; i++) {
    switch ((combi/((int) fast_pow(3,i))) % 3) {
    case 0: xx[nn] = x[i];   yy[nn++] = y[i] + l[i]; break; // right side
    case 1: xx[nn] = x[i];   yy[nn++] = y[i] - l[i]; break; // left  side
    case 2: break;                                          // ommit wire
    }
  }
  n = nn;
  const double SPACE_RES = 0.2;  // realistic space resolution (wire) = 200um
  return straightLineFit(xx, yy, n, a, b, l12, l22) / SPACE_RES / SPACE_RES /n;
}

//
// try all combinations...
//
double reconst(double x[], double y[], double length[],
	       int &n, double &a, double &b, double &l12, double &l22)
{  a = b = l12 = l22 = 0;
   if (n>10) return 1e10; // Verwerfe Spuren mit mehr als ... Drähten 
   double minchisqr = 1e11, mina=0, minb=0, ll22, ll12;
   int minn = 0;
   for (int combi=0; combi < fast_pow(3,n); combi++) {
     int signchange=0;
     int wires=0;
     int lastsign=0;
     for (int i=0; i<n; i++) {
       char digit = combi/((int) fast_pow(3,i)) % 3;
       if (digit<2) { 
	 if (lastsign!=digit) {
	   lastsign=digit;
	   signchange++;
	 }
	 wires++;
       }
     }
     // Verwende mehr als 1/2 der angesprochenen Drähte  UND
     // lasse max. 1 Sign-Wechsel zu (L/R):
     if (signchange<2 && wires*2 > n) {
       // if (signchange<2 && wires*3 > n*2) { // mehr als 2/3 ...
       int nn = n;
       double chisqr = FitCombi(combi, x, y, length, nn, a, b, ll12, ll22);
       if (minchisqr > chisqr) { 
	 minchisqr = chisqr; mina = a; minb = b; minn = nn;
	 l22 = ll22; l12 = ll12;
       }
     }
   }
   a = mina;
   b = minb;
   n = minn;
   return minchisqr;
}

void pdcDplane::FillCalibTab(u_int16_t t, double l, int ndata) {
  calibTab[ndata].time = t;
  calibTab[ndata].length = l;
}

void pdcDplane::WriteSplineNodes(double *sn, int iter, int xory) {
  char ext_name[100];
  sprintf(ext_name, "SNO%c_%05d_%d_%03d.DAT", 
	  (maxcells==8 ? 'x':'y'), NDAT, SAMPLES, nr_of_iterations);
  std::ofstream Ausgabe(ext_name);
  for (int i=0; i<SAMPLES+2; i++) Ausgabe << sn[i] << std::endl;
  Ausgabe << SAMPLES << " (SAMPLES)" << std::endl;
  Ausgabe.close();
}

double testfkt(double t, double *param) {
  spline ts(SAMPLES, param, param[SAMPLES], param[SAMPLES+1], spline::NATURAL);
  if (t<param[SAMPLES])   return 18.5;
  if (t>param[SAMPLES+1]) return -0.1;
  return ts(t);
}

double pdcDplane::CorrectDDR(int wire, double path)
{
  // deltaLength := 
  // timeToLength(TDC) - Spurabstand zum Draht aus der Rekonstruktion
  double length_calc, deltaLength;
  double corrTolerance = 0.5; // max. deltaLength for correction
  double errY[CALIBTABSIZE];  // sigmaY
  double calibTabTime[CALIBTABSIZE], calibTabLength[CALIBTABSIZE];
  double scale[SAMPLES+2], acure = 5e-4;

  length_calc = a + b * (data[wire].wire%8) * Z_DISTANCE
    - 18.4 * 2 * (data[wire].wire/8 + 0.5) 
    + ((data[wire].wire)%2 == 0 ? -STAGGERING : STAGGERING);

  deltaLength = fabs(length[wire]) - fabs(length_calc);
  if (flags & PDCD_DEBUG_FLAG)
    std::cout << "ndata=" << ndata << "  Iter=" << nr_of_iterations <<
      "  time="  << data[wire].time << 
      " length_calc:" << length_calc << 
      "  -w- " << (data[wire].wire%8) << std::endl;

  if (fabs(length_calc) < 18.4) {

    // Fill CalibTab ignoring "exotic" events (>0: "right hand tracks")
    if (ndata<CALIBTABSIZE)
      if (fabs(deltaLength)<corrTolerance)
	if (data[wire].time >= tmin && data[wire].time <= tmax)
	  FillCalibTab(data[wire].time, fabs(length_calc), ndata++);

    if (ndata==CALIBTABSIZE) {
      WriteCalibTab(ndata);
      for(int i=0;i<CALIBTABSIZE;i++) errY[i] = 0.2;

      for (int i=0; i<CALIBTABSIZE; i++) {
	calibTabTime[i]   = calibTab[i].time;
	calibTabLength[i] = calibTab[i].length;
      }

      for(int i=0; i<SAMPLES; i++) scale[i] = DDRSCALE;
      scale[SAMPLES] = scale[SAMPLES+1] = TIMESCALE;

      if (nr_of_iterations==0) {
	for(int i=0; i<SAMPLES; i++) varParam[i]= stuetze[i];
	varParam[SAMPLES]   = tmin;
	varParam[SAMPLES+1] = tmax;
      }

      struct timeval t1; gettimeofday(&t1, NULL); // usleep(10000);
	
      double chifit = 
	fitSplineData(NDAT, calibTabTime, calibTabLength, errY, SAMPLES+2,
		      testfkt, varParam, scale, acure);
      varParam[SAMPLES]   = tmin = (int) varParam[SAMPLES];
      varParam[SAMPLES+1] = tmax = (int) varParam[SAMPLES+1];
      for (int i=0; i<SAMPLES; i++) stuetze[i] = varParam[i];

      std::cout << " chifit=" << chifit << 
	" chired=" << chifit/(NDAT-SAMPLES-1) << " NDAT=" << NDAT << 
	" SAMPLES=" << SAMPLES << std::endl;

      struct timeval t2; gettimeofday(&t2, NULL);
      std::cout << "\nt2-t1 = " << (t2.tv_usec-t1.tv_usec)
		<< " usec" << std::endl;

      fprintf(stdout, "acure   SAMPLES xscl     tscl     NDAT   ");
      fprintf(stdout, "chifit   chired     tmin     tmax   plane \n");
      fprintf(stdout, "%4.2e ", acure );
      fprintf(stdout, "%3i %7.2f  %7.2f  %7i %8.1f %8.3f %8i %8i %c erg\n", 
	      SAMPLES, scale[SAMPLES-1], scale[SAMPLES],
	      NDAT, chifit, chifit/(NDAT-SAMPLES-1), tmin, tmax, 
	      (maxcells==8 ? 'x' : 'y'));
      
      double xx=tmin;
      while (xx<=tmax) {
	std::cout << "plot " << xx << "\t" << testfkt(xx, varParam) << 
	  "\t" << varParam[SAMPLES] << " - " << varParam[SAMPLES+1] << 
	  " Iter= " << nr_of_iterations << "_" << std::endl;
	xx+=50;
      }

      WriteSplineNodes(varParam, nr_of_iterations, maxcells);
      
      if (nr_of_iterations == MAX_ITERATION_NR) {
	std::cerr << "MAX_ITERATION_NR (" << MAX_ITERATION_NR <<
	  ") reached." << std::endl;
	exit(0); // Verlasse das Programm!
      }
      nr_of_iterations++;
      ndata = 0; // da capo: neue Iteration
    }
  }
  return 0;
}


void pdcDplane::WriteCalibTab(int ndata) {
  /* for diagnostical purposes only */
  char ext_name[100];
  sprintf(ext_name, "calib_%05d_%03d_%c.tab", 
	  NDAT, nr_of_iterations, (maxcells==8 ? 'x':'y'));
  std::ofstream Ausgabe(ext_name);
  for (int i=0; i<ndata; i++) Ausgabe << i << "\t" << calibTab[i].time << "\t" 
				      << calibTab[i].length << std::endl;
  Ausgabe.close();
}

int pdcDplane::handle()
{
  int wires = subTree->pack((u_int16_t *)data, 8*maxcells);
  *multi = wires;

  // Event Number: (debug only)
  AquaNodeInt *nr = (AquaNodeInt *)onl->find("online.nr");
  if (flags & PDCD_DEBUG_FLAG)
    std::cout << "Event: " << *nr << " multi: " << wires << std::endl;
  
  if (wires < 1) {chisqr=1e10; return 0;}

  for (int w=0; w<wires; w++) {
    int c = data[w].wire/8;
    if ((0<=c)&&(c<maxcells)) {
      length[w] = timeToLength(data[w].time);

      if (flags & PDCD_DEBUG_FLAG)
	std::cout << (maxcells==8 ? 'x' : 'y') 
		  << " wire " << data[w].wire%8
		  << " time " << data[w].time
		  << " len "  << length[w] << std::endl;

      // ignore wires with negative length
      if (length[w] < 0) continue;
      if (timeH) HMFill(timeH, data[w].time, 0.0, 1.0);
      if (wireH) HMFill(wireH, data[w].wire, 0.0, 1.0);
      if ( lenH) HMFill( lenH,    length[w], 0.0, 1.0);
      if ((data[w].time > 600) && (data[w].time < 3000)) {
	if (data[w].wire%2) // odd wire numbers
	  {
	    if (tagH) HMFill(tagH, data[w].wire-1, 0.0, 1);
	    if ((effH) && (data[w].wire-1 == data[w-1].wire))
	      HMFill(effH, data[w].wire-1, 0.0, 1.0);
	  } else { // even wire numbers
	    if (tagH) HMFill(tagH, data[w].wire+1, 0.0, 1);
	    if ((effH) && (data[w].wire+1 == data[w+1].wire))
	      HMFill(effH, data[w].wire+1, 0.0, 1.0);
	  }
      }
      if (w<pdcDonlineLen) {
	*raw_time[w] = data[w].time;
	*raw_wire[w] = data[w].wire;
	*raw_len[w] = length[w];
      }
    }
  }

  for (int w=wires; w<pdcDonlineLen; w++) {
    *raw_time[w] = *raw_wire[w] = -1;
    *raw_len[w] = -1;
  }

  //
  // Brute force reconstruction, very slow (H. Merkel)
  //
  {
    a=b=chisqr=0.0;
    n=time=0; 
    double x[(int) *multi], y[(int) *multi], l[(int) *multi];
    if ((n=*multi)<4) {a = b = time = 0; chisqr=1e10; return 0;}

    for (int cell=0;cell<8;cell++)
      for (int w = 0; w<8; w++)
	for (int k=0;k<*multi; k++)
	  if(data[k].wire == (cell*8+w)) { 
	    x[k] = w * Z_DISTANCE;
	    y[k] = (cell + 0.5) * 18.4 * 2
		   + (w%2 == 0 ? STAGGERING : - STAGGERING);
	    l[k] = length[k];
	  }
    
    struct timeval t1;  gettimeofday(&t1, NULL);
    chisqr = reconst(x, y, l, n, a, b, l12, l22);
    if (flags & PDCD_DEBUG_FLAG)
      fprintf(stdout, "Event %d %c chi2= %8.3f m %i \n",(int)*nr,
	      (maxcells==8 ? 'x' : 'y'),chisqr,wires);
    struct timeval t2;  gettimeofday(&t2, NULL);
    time = (t2.tv_sec - t1.tv_sec) * 1000000 + t2.tv_usec - t1.tv_usec;
  }

    //if ((data[w].wire%8 > 0) && (data[w].wire%8 < 7) && (maxcells==8)) // X16
    //if (data[w].time >= minTimeX && data[w].time <= maxTimeX)
  if (DDR_CORRECTION_X) if (maxcells==8) 
    for (int w=0; w<wires; w++) CorrectDDR(w, length[w]);

  if (DDR_CORRECTION_Y) if (maxcells==2) 
    for (int w=0; w<wires; w++) CorrectDDR(w, length[w]);

  for (int w=0; w<wires; w++) {
    double lcalc, resid, residPrim;
    lcalc = a + b * ((data[w].wire)%8) * Z_DISTANCE 
            - 18.4 * 2 * (data[w].wire/8 + 0.5);
    residPrim = fabs(length[w]) - fabs(lcalc);
    lcalc = lcalc + ((data[w].wire)%2 == 0 ? -STAGGERING : STAGGERING);
    resid = fabs(length[w]) - fabs(lcalc);
    if (resH) HMFill (resH, resid, 0, 1);
    if (staggH) HMFill (staggH, residPrim, 0, 1);
  }

  *error = 0;
  *pattern = 42;

  *x = a;
  *sl = b;
  *chi2 = chisqr;

  return 0;
}

int pdcDplane::Reset()
{
  for (int c=0; c<maxcells; c++) cell[c]->Reset();
  return 0;
}

int pdcDcell::handle() { return 0; }

int pdcDcell::Reset() { return 0; }

double 
pdcDplane::timeToLength(u_int16_t t) {
  spline ddrSplineX(SAMPLES, stuetze, tmin, tmax, spline::NATURAL); 
  if (t>tmax) return 0.0;       // return ddrSpline(tmin);
  else if (t<tmin) return 18.4; // return ddrSpline(tmax);
  else return ddrSplineX(t);
}
