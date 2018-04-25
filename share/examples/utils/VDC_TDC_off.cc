//
// Jan M. Friedrich  10/99
//

#include <iostream>
#include <std.h>
#include <math.h>
#include "/usr/local/a1/include/HMBook/hmbook.h"
#include "/usr/local/a1/include/HMBook/hmplot.h"

extern "C"
{
  //#include "/local/home/friedric/common/Cola/HMBook/fitting.h"
#include "/users/kpha1/friedric/Cola/HMBook/fitting.h"
}

struct fitparam {
  double (* fitfun)(double[], double[]);
  int d;
  double par[15];
  double  sc[15];
  double cov[15*15];
};

double
VDC_TDC_shape(double par[7], double x[]) {
  double gg;
  double x1, y1, y2;
  if (x[0]<par[1]) {
    x1 = (x[0]-par[1])/par[2];
    y1 = 0;
    y2 = par[0];
  }
  else if (x[0]<par[3]) {
    x1 = (x[0]-par[3])/par[6];
    y1 = par[0];
    y2 = par[5];
  }
  else {
    x1 = (x[0]-par[3])/par[4];
    y1 = 0;
    y2 = par[0] + par[5];
  }
  return y1 + y2 * exp(-0.5*x1*x1);
}

fitparam FitAndFill(HIST* h, double minx, double maxx, fitparam f) {
  
  double dx = h->maxx - h->minx;
  
  int n=0, nmin=0;
  for (int i=0; i<h->nx; i++) {
    double xx = h->minx+ dx/h->nx*(i+0.5);
    if (xx>minx && xx<maxx) { if (n==0) nmin=i; n++;}
  }
  
  double x[n], y[n], err[n];
  for (int i=0; i<n; i++) {
    x[i]   = h->minx+ dx/h->nx*(nmin+i+0.5);
    double yy = h->data[nmin+i];
    y[i]   = yy;
    err[i] = sqrt( yy>=1?yy:1 );
  }
  printf("Prepared to fit %d points\n", n);

  double csq; int dof;
  fit(n, 1, (void*)x, y, err, f.d, f.fitfun, f.par, f.sc, 1e-10, 
      &csq, &dof);
  printf("Fitresult:\n");
  errormatrix(f.d, f.cov, chisquare, f.par, 1e-10);
  printf("Fitresult:\n");
  for (int i =0; i<f.d; i++) printf("par[%d] = %f +- %f\n", i, 
				    f.par[i], sqrt(fabs(f.cov[(f.d+1)*i])));
  return f;
}

void PresentFit(fitparam f, double minx, double maxx, float dx) {
  double oldx=-999, oldy;
  for (double x=minx; x<maxx; x+=dx) {
    double xx[1]; xx[0]=x;
    double y = f.fitfun(f.par, xx);
    if (oldx!=-999) HMLine(x, y, oldx, oldy);
    oldx = x;
    oldy = y;
  }
}

int NewTitle(HIST *h, char* source) { h->title  = source; }
int NewXtit( HIST *h, char* source) { h->labelx = source; }

float HMmaxY(HIST *h) {
  float maxY = h->data[0];
  for (int i=1; i < h->nx; i++) if (h->data[i] > maxY) maxY = h->data[i];
  return maxY;
}

float HMXofYmax(HIST *h) {
  float maxY = h->data[0];
  int ii=0;
  for (int i=1; i < h->nx; i++) 
    if (h->data[i] > maxY) {ii=i; maxY = h->data[i];}
  float ratio=(float)ii/(float)h->nx;
  return h->minx + (h->maxx-h->minx) * ratio;
}

float HMmin_nza_Y(HIST *h) { // Smallest non-zero absolute value
  float minY = 1;
  for (int i=1; i < h->nx; i++) 
    if (fabs(h->data[i]) < minY && fabs(h->data[i]) > 0) 
      minY = h->data[i];
  return minY;
}

int XLims(float xmin, float xmax) { 
  HMMinimumX(xmin); HMMaximumX(xmax);
  return 0;
}

int YLims(float ymin, float ymax) { 
  HMMinimumY(ymin); HMMaximumY(ymax);
  return 0;
}

float HMx(HIST *h, int i) {
  return h->minx + (i+0.5)/h->nx * (h->maxx - h->minx);
}

int
main(int argc, char *argv[])
{
  if (argc!=2) {
    printf("Format: VDC_TDC_off File.his \n");
    return 0;
  }

  HMSetPredefinedStyle(ST_OVERHEAD);
  HMPlotFile("VDC_TDC_off.ps");
  HMTitle("Offsets for VDCs");
  HMZone(2,2,1);

  const int ihistos = 12;
  char hname_drift[ihistos][200] = 
  { "Spec. A/x1-layer/Drift Time",
    "Spec. A/s1-layer/Drift Time",
    "Spec. A/x2-layer/Drift Time",
    "Spec. A/s2-layer/Drift Time",
    "Spec. B/x1-layer/Drift Time",
    "Spec. B/s1-layer/Drift Time",
    "Spec. B/x2-layer/Drift Time",
    "Spec. B/s2-layer/Drift Time",
    "Spec. C/x1-layer/Drift Time",
    "Spec. C/s1-layer/Drift Time",
    "Spec. C/x2-layer/Drift Time",
    "Spec. C/s2-layer/Drift Time"};

  double     xmin[ihistos];
  double     xmax[ihistos];
  double fit_xmin[ihistos];
  double fit_xmax[ihistos];
  double   offset[ihistos];
  double      sdt[ihistos];
  
  double Nmax; HIST *drift;
  for (int i=0; i<ihistos; i++) {

    drift = HMScanFile(argv[1], hname_drift[i]);
    if (drift==NULL) continue;
    Nmax = HMmaxY(drift);
    float XofYmax = HMXofYmax(drift);
    
    if (i<8) {  //Spek A/B
      xmin[i] = fit_xmin[i] = XofYmax- 525;
      xmax[i] = fit_xmax[i] = XofYmax+ 90; 
    }
    else     {  //Spek C
      xmin[i] = fit_xmin[i] = XofYmax-1075;
      xmax[i] = fit_xmax[i] = XofYmax+ 150; 
    }
    
    printf("XofYmax = %f\n", XofYmax);
    
    printf("Fitting %s...\n", hname_drift[i]);
    XLims(xmin[i], xmax[i]);
    HMHistogram(drift);
    fitparam driftfit = { VDC_TDC_shape, 7,
			{Nmax/2, XofYmax-435, 10.0, XofYmax, 3.0, Nmax/2, 30}, 
			{  10,             3,  0.5,       3, 0.2,   10,  1} }; 
    
    driftfit = FitAndFill(drift, fit_xmin[i], fit_xmax[i], driftfit);

    float rw = fabs(driftfit.par[4]); //width of outer right gaussian
    float lw = fabs(driftfit.par[2]); //width of outer right gaussian

    offset[i] = driftfit.par[3]+2.2*rw;  
    // 1.5*rw would be formally correct if linear shape is assumed
    // starting from 1*sigma of the gaussian falling slope
    // 2.2*rw seems to work better, probably due to the fact that
    // the wires do not all have the same offset 

    sdt[i] = 12000 / (driftfit.par[3]+1.5*rw-driftfit.par[1]+1.5*lw);

    HMSetOption(LINE_COLOR, 0xFF0000);
    PresentFit(driftfit, fit_xmin[i], fit_xmax[i], 1);
    HMSetOption(LINE_COLOR, 0x000000);

    XLims(XofYmax-4*rw, XofYmax+8*rw);
    HMHistogram(drift);
    HMSetOption(LINE_COLOR, 0xFF0000);
    PresentFit(driftfit, XofYmax-4*rw, XofYmax+8*rw, rw/20);
    HMSetOption(LINE_COLOR, 0x000000);
  }
  HMClose();
  FILE* rundb = fopen("rundb.VDC.TDC", "w");
  char sugg[100] = "// suggested drift times (mircom/channel):";
  char sugg2[100] = "// for 1 channel = 0.5 ns (mircom/ns)    :";
  if (offset[0]) {
    fprintf(rundb, "A.VDC.TDC = { %5.1f, %5.1f, %5.1f, %5.1f}\n",
	    offset[0], offset[1], offset[2], offset[3]);
    fprintf(rundb, "%s %5.2f, %5.2f, %5.2f, %5.2f\n", sugg,
	    sdt[0], sdt[1], sdt[2], sdt[3]);
    fprintf(rundb, "%s %5.2f, %5.2f, %5.2f, %5.2f\n", sugg2,
	    sdt[0]*2, sdt[1]*2, sdt[2]*2, sdt[3]*2);
  }
  if (offset[4]) {
    fprintf(rundb, "B.VDC.TDC = { %5.1f, %5.1f, %5.1f, %5.1f}\n",
	    offset[4], offset[5], offset[6], offset[7]);
    fprintf(rundb, "%s %5.2f, %5.2f, %5.2f, %5.2f\n", sugg,
	    sdt[4], sdt[5], sdt[6], sdt[7]);
    fprintf(rundb, "%s %5.2f, %5.2f, %5.2f, %5.2f\n", sugg2,
	    sdt[4]*2, sdt[5]*2, sdt[6]*2, sdt[7]*2);
  }
  if (offset[8]) {
    fprintf(rundb, "C.VDC.TDC = { %5.1f, %5.1f, %5.1f, %5.1f}\n",
	    offset[8], offset[9], offset[10], offset[11]);
    fprintf(rundb, "%s %5.2f, %5.2f, %5.2f,%5.2f\n", sugg,
	    sdt[8], sdt[9], sdt[10], sdt[11]);
    fprintf(rundb, "%s %5.2f, %5.2f, %5.2f, %5.2f\n", sugg2,
	    sdt[8]*2, sdt[9]*2, sdt[10]*2, sdt[11]*2);
  }
  fclose(rundb);
  return 0;
}













