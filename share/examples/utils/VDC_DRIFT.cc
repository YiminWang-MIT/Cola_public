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

const int plot_all_histos = 0;

struct fitparam {
  double (* fitfun)(double[], double[]);
  int d;
  double par[15];
  double  sc[15];
  double cov[15*15];
};

double
VDC_error_shape(double par[5], double x[]) {
  double gg;
  double x3 = par[1]+fabs(par[3]);
  if (x[0]<x3) {
    double x1 = (x[0]-par[1])/par[2];
    gg = par[0] * exp(-0.5*x1*x1);
  }
  else {
    double x1 = (x3-par[1])/par[2];
    double g3 = par[0] * exp(-0.5*x1*x1);
    gg = g3 * exp(-fabs(par[4])*(x[0]-x3));
  }
  return gg;
}

double
poly2(double par[3], double x[]) {
  double x1 = x[0]-par[1];
  return par[0]+par[2]*x1*x1;
}

fitparam FitAndFill(HIST* h, double minx, double maxx, fitparam f, 
                    HIST* err = NULL, int prnt=1);

void PresentFit(fitparam f, double minx, double maxx,  float N = 100,
		double miny=-1e20, double maxy=1e20);

fitparam FitAndFill(HIST* h, double minx, double maxx, fitparam f, 
                    HIST* err_his, int prnt) {
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
    float err_stat = sqrt( yy>=1?yy:1 );
    err[i] = (err_his==NULL ? err_stat : err_his->data[nmin+i]);
  }
  if (prnt) printf("Prepared to fit %d points\n", n);
  
  double csq; int dof;
  fit(n, 1, (void*)x, y, err, f.d, f.fitfun, f.par, f.sc, 1e-10, 
      &csq, &dof);
  errormatrix(f.d, f.cov, chisquare, f.par, 1);
  if (prnt) {
    printf("Fitresult:\n");
    for (int i =0; i<f.d; i++) printf("par[%d] = %f +- %f\n", i, 
				      f.par[i], sqrt(fabs(f.cov[i*f.d+i])));
  }
  return f;
};

void PresentFit(fitparam f, double minx, double maxx, float N, 
		double miny, double maxy) {
  double oldx=-999, oldy;
  HMSetOption(LINE_COLOR, 0xFF0000);
  for (double x=minx; x<maxx; x+=(maxx-minx)/N) {
    double xx[1]; xx[0]=x;
    double y = f.fitfun(f.par, xx);
    if (oldx!=-999 && y>miny && y<maxy && oldy>miny && oldy<maxy) 
      HMLine(x, y, oldx, oldy);
    oldx = x;
    oldy = y;
  }
  HMSetOption(LINE_COLOR, 0x000000);
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
  if (ymin<ymax) { HMMinimumY(ymin); HMMaximumY(ymax); }
  if (ymax<ymin) { HMMinimumY(ymax); HMMaximumY(ymin); }
  return 0;
}

float HMx(HIST *h, int i) {
  return h->minx + (i+0.5)/h->nx * (h->maxx - h->minx);
}

int IdentifyDriftVelocity(char *hisfilename) {
  int c, cc=0, p1=-1, p2=-1;
  while ( (c = *(hisfilename+cc)) != 0) {
    if      (p1==-1 && c=='.'        ) p1=cc;
    else if (p2==-1 && (c<48 || 57<c)) p1=-1;
    if      (p1  >0 && c=='.'        ) p2=cc;
    cc++;
  }
  if (p1==-1 || p2==-1) {
    printf("Warning: no DriftVelocity name found in %s", hisfilename);
    return -1;
  }
  int dr_time;
  sscanf(hisfilename+p1+1, "%d", &dr_time);
  return dr_time;
}

void InterpretDV(HIST* e, HIST* err, int s_opt = 0);

void InterpretDV(HIST* e, HIST* err, int s_opt) {
  double minY = HMmin_nza_Y(e);
  double maxY = HMmaxY(e);
  double dY = maxY - minY;
  YLims(minY-dY*0.2, maxY+dY*0.2);
  HMDataAndError(e, err);
  fitparam efit = { poly2, 3, {0.07, 52.0, 0.02}, { 0.03, 0.1, 0.002} }; 
  double minfit = e->minx, maxfit = e->maxx;
  if (s_opt) {
    efit.par[1] = 8;
    efit.par[2] = 0.00008;
    if (s_opt==2) {            //Spek.C
      efit.par[1] = 23;
      minfit = 16;
    }
  }
  printf("Fitting %s...\n", e->title);
  efit = FitAndFill(e, minfit, maxfit, efit, err);
  PresentFit(efit, e->minx, e->maxx, 100, minY-0.1*dY, maxY+0.1*dY);
  char min_str[200];
  sprintf(min_str, "min:(%5.2f, %6.3f)", efit.par[1], efit.par[0]);
  HMText(0.7*e->minx+0.3*e->maxx, minY-0.14*dY, min_str);
}

int
main(int argc, char *argv[])
{
  if (argc<2) {
    printf("Format: VDC_DRIFT File1.4xx.his File2.5xx.his ... \n");
    return 0;
  }

  int s_opt = 0;

  const int ihistos = 12;
  char hname_err[ihistos][200] = 
  { "Spec. A/Focal Plane/[D]x (disp.)",
    "Spec. B/Focal Plane/[D]x (disp.)",
    "Spec. C/Focal Plane/[D]x (disp.)", 
    "Spec. A/Focal Plane/[D]y",
    "Spec. B/Focal Plane/[D]y",
    "Spec. C/Focal Plane/[D]y", 
    "Spec. A/Focal Plane/[D][q] (disp.)",
    "Spec. B/Focal Plane/[D][q] (disp.)",
    "Spec. C/Focal Plane/[D][q] (disp.)",
    "Spec. A/Focal Plane/[D][f]",
    "Spec. B/Focal Plane/[D][f]",
    "Spec. C/Focal Plane/[D][f]" };

  fitparam errfit[ihistos] = {
    {VDC_error_shape,5,{1,1, 0.01, 0.03, 12}, { 1, 0.01, 0.001, 0.001, 0.1} }, 
    {VDC_error_shape,5,{1,1, 0.02, 0.03, 18}, { 1, 0.01, 0.001, 0.001, 0.1} }, 
    {VDC_error_shape,5,{1,1, 0.02, 0.03, 12}, { 1, 0.01, 0.001, 0.001, 0.1} }, 
    {VDC_error_shape,5,{1,1, 0.02, 0.06, 10}, { 1, 0.01, 0.001, 0.001, 0.1} }, 
    {VDC_error_shape,5,{1,1, 0.03, 0.01, 15}, { 1, 0.01, 0.001, 0.001, 0.1} }, 
    {VDC_error_shape,5,{1,1, 0.03, 0.02, 12}, { 1, 0.01, 0.001, 0.001, 0.1} }, 
    {VDC_error_shape,5,{1,1, 0.001,0.005,70}, { 1, 0.01,0.0001,0.0001, 1  } }, 
    {VDC_error_shape,5,{1,1, 0.002,0.003,90}, { 1, 0.01,0.0001,0.0001, 1  } }, 
    {VDC_error_shape,5,{1,1, 0.002,0.003,80}, { 1, 0.01,0.0001,0.0001, 1  } }, 
    {VDC_error_shape,5,{1,1, 0.01, 0.02, 15}, { 1, 0.01, 0.001, 0.001, 0.1} }, 
    {VDC_error_shape,5,{1,1, 0.01, 0.02, 15}, { 1, 0.01, 0.001, 0.001, 0.1} }, 
    {VDC_error_shape,5,{1,1, 0.01, 0.02, 15}, { 1, 0.01, 0.001, 0.001, 0.1} } 
  };

  double     xmin[ihistos];
  double     xmax[ihistos];
  double fit_xmin[ihistos];
  double fit_xmax[ihistos];

  int dri_time = IdentifyDriftVelocity(argv[1]);
  double dvmin = ((double)dri_time) / (dri_time<1000?10:100);
  dri_time = IdentifyDriftVelocity(argv[argc-1]);
  double dvmax = ((double)dri_time) / (dri_time<1000?10:100);
  double deltadv = (dvmax-dvmin)/((double)(argc-2));

  HIST** errxDV  = new HIST*[3];
  HIST** errxDVe = new HIST*[3];
  HIST** erryDV  = new HIST*[3];
  HIST** erryDVe = new HIST*[3];
  HIST** errtDV  = new HIST*[3];
  HIST** errtDVe = new HIST*[3];
  HIST** errpDV  = new HIST*[3];
  HIST** errpDVe = new HIST*[3];

  char hn[3][20] = { "Spek. A", "Spek. B", "Spek. C"};

  char s_opt_xlabel[30] = "deviation of s-dr.vel.";
  char s_opt_xunit[5] = "%_o!";

  for (int i=0; i<3; i++) {
    errxDV[i]  = HMBook1(hn[i],"drift velocity", "most probable [D]x", 
			 "[m]m/ns", "mm", 
			 argc-1, dvmin-deltadv/2, dvmax+deltadv/2); 
    errxDVe[i] = HMBook1("","", "", "", "", 
			 argc-1, dvmin-deltadv/2, dvmax+deltadv/2); 

    erryDV[i]  = HMBook1(hn[i],"drift velocity", "most probable [D]y", 
			 "[m]m/ns", "mm", 
			 argc-1, dvmin-deltadv/2, dvmax+deltadv/2); 
    erryDVe[i] = HMBook1("","", "", "", "", 
			 argc-1, dvmin-deltadv/2, dvmax+deltadv/2); 

    errtDV[i]  = HMBook1(hn[i],"drift velocity", "most probable [Dq]", 
			 "[m]m/ns", "^o!", 
			 argc-1, dvmin-deltadv/2, dvmax+deltadv/2); 
    errtDVe[i] = HMBook1("","", "", "", "", 
			 argc-1, dvmin-deltadv/2, dvmax+deltadv/2); 

    errpDV[i]  = HMBook1(hn[i],"drift velocity", "most probable [Df]", 
			 "[m]m/ns", "^o!", 
			 argc-1, dvmin-deltadv/2, dvmax+deltadv/2); 
    errpDVe[i] = HMBook1("","", "", "", "", 
			 argc-1, dvmin-deltadv/2, dvmax+deltadv/2); 
  }
  double Nmax; HIST *errx;
  
  double old_MPv[ihistos];

  HMSetPredefinedStyle(ST_OVERHEAD);

  for (int dvel=0; dvel<argc-1; dvel++) {
    
    int dri_vel = IdentifyDriftVelocity(argv[dvel+1]);
    
    printf("Evaluating error histograms for %d\n", dri_vel);
    
    if (dvel==0) {
      if (dri_vel<4800) {
	printf("\n****\nFit for deviation of s-plane driftvelocity\n****\n");
	s_opt = 1;
	HMPlotFile("VDC_DRIFT_S.ps");
	HMTitle("Deviation of Drift Velocity in S planes");
	HMZone(2,2,1);
	for (int i=0; i<3; i++) {
	  erryDV[i]->labelx = s_opt_xlabel;
	  erryDV[i]->unitx = s_opt_xunit;
	  errpDV[i]->labelx = s_opt_xlabel;
	  errpDV[i]->unitx = s_opt_xunit;
	}
      }
      else {
	HMPlotFile("VDC_DRIFT.ps");
	HMTitle("Optimal Drift Velocity in VDCs");
	HMZone(2,2,1);
      }
    }
    
    double drift_vel = ((double)dri_vel) / (dri_vel<1000?10:100);

    for (int i=0; i<ihistos; i++) {
      errx = HMScanFile(argv[dvel+1], hname_err[i]);
      if (errx==NULL) continue;
      if (plot_all_histos) HMHistogram(errx);
      Nmax = HMmaxY(errx);
      float XofYmax = HMXofYmax(errx);
      xmin[i] = fit_xmin[i] = 0.0;
      if (i>5 && i<9) xmax[i] = fit_xmax[i] = 0.2; 
      else            xmax[i] = fit_xmax[i] = 0.5; 
      printf("Fitting %s...\n", hname_err[i]);
      errfit[i].par[0] = Nmax;
      errfit[i].par[1] = XofYmax;
      errfit[i]=FitAndFill(errx, fit_xmin[i], fit_xmax[i], errfit[i],NULL,0);
      if (plot_all_histos) PresentFit(errfit[i], fit_xmin[i], fit_xmax[i]);
      
      if (dvel==0) old_MPv[i]=0.95*errfit[i].par[1];

      double MPv = errfit[i].par[1]; // Most probable value
      double errFromFit = sqrt(fabs(errfit[i].cov[1*errfit[i].d+1]));
      double diffErr = fabs(MPv-old_MPv[i])/3;
      double errMPv = diffErr;
      if (errMPv<MPv/1000) errMPv = MPv/1000; 

      if      (i<3) {
	HMFill(errxDV [i], drift_vel, 0, MPv);
	HMFill(errxDVe[i], drift_vel, 0, errMPv);
      }
      else if (i<6) {
	HMFill(erryDV [i-3], drift_vel, 0, MPv);
	HMFill(erryDVe[i-3], drift_vel, 0, errMPv);
      }
      else if (i<9) {
	HMFill(errtDV [i-6], drift_vel, 0, MPv);
	HMFill(errtDVe[i-6], drift_vel, 0, errMPv);
      }
      else if (i<12) {
	HMFill(errpDV [i-9], drift_vel, 0, MPv);
	HMFill(errpDVe[i-9], drift_vel, 0, errMPv);
      }

      old_MPv[i] = MPv;
    }
  }
  
  for (int i=0; i<3; i++) {
    if (s_opt == 0) InterpretDV(errxDV[i], errxDVe[i], 0);
    if (i<2) InterpretDV(erryDV[i], erryDVe[i], s_opt);     // Spec. C
    else     InterpretDV(erryDV[i], erryDVe[i], s_opt?2:0); // special
    if (s_opt == 0) InterpretDV(errtDV[i], errtDVe[i], 0);
    if (i<2) InterpretDV(errpDV[i], errpDVe[i], s_opt);
    else     InterpretDV(errpDV[i], errpDVe[i], s_opt?2:0);
  }
  
  HMClose();
  return 0;
}













