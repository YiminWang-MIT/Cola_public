#ifdef HAVE_CONFIG_H
#include "../config.h"
#endif
#include "rundatabase.h"
#include "QuasiRandom.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <getopt.h>
#include "Detector/pdcD.h"
#include "HMBook/hmbook.h"
#include "HMBook/hmplot.h"
extern "C" {
#include "HMBook/fitting.h"
}

// This is totally useless but needed to keep Parsermain.o happy
// to process the Ntuple commands.
// It might be useful if we would like to make ntuples with Simul++
// Not available yet.
//
#include "ntmode.h"
char *ntvarn[MAX_NT_VAR];  // Scalar name related to ntvar
char *ntdes[MAX_NT_VAR];    // NTvariable name
char *ntbck[MAX_NT_VAR];    // Block name for each ntvar
char *nttitle;     // Ntuple Title
int  ntiden, ntnvar;
///

int verbose = (getenv("VERBOSE") != NULL);
pdcD *PdcD;

double inline sqr(double a) { return a*a; }
const double deg2rad = M_PI/180;
const double rad2deg = 180/M_PI;

#define FLAG_VERBOSE 1

extern "C" void HMSetPredefinedStyle(int style);
extern int HMFrameNr;

inline double gauss(double x) {return 0.39894228040143267794*exp(-x*x*0.5);}
inline double gauss(double par[4], double x[]) {
  return par[0]/par[2]*gauss((x[0]-par[1])/par[2]) + par[3];
}

void fillArray(double *array, char *title, char spec, int length, char *out,
	       int mode, double initpara[], int flags) 
{
  for (int paddle=1; paddle<=length; paddle++) {
    char TitleTime[255];
    sprintf(TitleTime, title, spec, paddle);
    HIST *CoincTime = HMFindTitle(TitleTime);
    if (!CoincTime) {std::cerr<<"Can't find histogram "<<TitleTime<<std::endl; exit(-1);}
    
    if (out) {
      char HistTitle[80];
      sprintf(HistTitle, "Paddle Nr. %d", paddle);
      HMTitle(HistTitle);
    }

    FLOAT mni, mxi, mnp, mxp, fwhm, fwhmpos, rms, mean, inte;
    HMInfo1D(CoincTime,&mxi,&mxp,&mni,&mnp,&fwhm,&fwhmpos,&rms,&mean,&inte);
    if (mode!=2)
      array[paddle - 1] = mode==0 ? fwhmpos : mean;
    else {
      double binsize = (CoincTime->maxx - CoincTime->minx)/CoincTime->nx;
      double para[4]={inte*binsize, mean, rms, 0.0}, scale[4];
      if (initpara[2]!=0) for (int j=0; j<4; j++) para[j] = initpara[j];
      for (int j=0; j<4; j++) scale[j] = para[j] / 25.0;  

      double kov[4*4];
      double chisq; 
      int dof;      
      int nx = CoincTime->nx;
      double dx = CoincTime->maxx - CoincTime->minx;
      
      double *x     = new double[nx];
      double *y     = new double[nx];
      double *err   = new double[nx];
      
      for (int j=0; j<nx; j++) {
	x[j]   = CoincTime->minx + dx / nx * (j + 0.5);
	y[j]   = CoincTime->data[j];
	err[j] = 1/sqrt(fabs(y[j])+2);
      }
      
      if (CoincTime->entries != 0) {

      fit(nx, 1, (void*) x, y, err, 4, gauss, para, scale, 1e-14,
      	  &chisq, &dof);
      pchisq(chisq, dof);
      
      // Test unused paddles
      
      // Verbose output
      if (flags & FLAG_VERBOSE)
	printf("Paddle Nr.: %d  Chi-Prob: %f \n"\
	       "Para 0: %f  1: %f  2: %f  3: %f\n",
	       paddle, chisq, para[0], para[1], para[2], para[3]);
      
      // Fill shift tables
      array[paddle-1] = para[1];
      
      // Write postscript file
      if (out) {
	CoincTime->title="";
	double MaxY = (para[0]+para[3])*1.4, MinX = -10, MaxX = 10;
	if (MaxY < 5) MaxY = 5;
	
	//	HMMaximumX(MaxX); HMMinimumX(MinX);
	//HMMaximumY(MaxY); HMMinimumY(MinY);
	
	HMSetOption(FILL_COLOR, 255*256*256 + 255*256);
	HMHistogram(CoincTime);
	
	FLOAT Linie_x[2] = {(MinX+MaxX)/2+(MaxX-MinX)*0.1, 
			    (MinX+MaxX)/2+(MaxX-MinX)*0.1+0.155*(MaxX-MinX)};
	FLOAT Linie_y[2] = {MaxY*0.9-MaxY/100, MaxY*0.9-MaxY/100};
	
	HMSetOption(FILL_COLOR, 255*256*256 + 255*256 + 255);
	HMSetOption(COLOR_LABEL, 0);
	HMText(Linie_x[0], MaxY*0.90, "Fit-Result :"); 
	HMSame(); HMSetOption(LINE_STYLE, 0);
	HMSetOption(LINE_WIDTH, 0.04);
	HMPlotFunction(2, Linie_x, Linie_y,"","","","","");
	HMSetOption(LINE_WIDTH, 0.02);
	
	char Resulttext[100];
	
	sprintf(Resulttext, "MAX    : %4.0f counts", para[0] + para[3]);
	HMText(Linie_x[0], MaxY*0.90 - MaxY/28 - MaxY/38, Resulttext);
	
	sprintf(Resulttext, "MAX at: %5.3f ns", para[1]);
	HMText(Linie_x[0], MaxY*0.90 - MaxY/28*2 - MaxY/38, Resulttext);
	
	sprintf(Resulttext, "FWHM : %6.3f ns", para[2]*2.35);
	HMText(Linie_x[0], MaxY*0.90 - MaxY/28*3 - MaxY/38, Resulttext);
	
	sprintf(Resulttext, "Backgr.: %4.0f counts", para[3]);
	HMText(Linie_x[0], MaxY*0.90 - MaxY/28*4 - MaxY/38, Resulttext);
	
	HMPlotFit(para, (double *) kov, CoincTime->minx, CoincTime->maxx,2000);
	HMSetOption(COLOR_LABEL, 255);    
	HMSame();
	HMHistogram(CoincTime);
      } 
      }
    }
  }

}

int
main(int argc, char *argv[])
{
  int flags = 0;
  int mode = 0;
  char opt;
  char *infile = NULL;
  char *psfile = NULL;
  char spec = 'A';
  double initpara[4] = { 0, 0, 0, 0 };

  // Check arguments
  while ((opt = getopt(argc, argv, "hvo:f:ABC0:1:2:3:s:m:")) != EOF) {
    switch (opt) {
    case 'f': infile  = optarg;           break;
    case 'o': psfile = optarg;            break;
    case 'v': flags |= FLAG_VERBOSE;      break;
    case 'A': spec = 'A';                 break;
    case 'B': spec = 'B';                 break;
    case 'C': spec = 'C';                 break;
    case 'm': mode = atoi(optarg);        break;
    case '0': initpara[0] = atof(optarg); break;
    case '1': initpara[1] = atof(optarg); break;
    case '2': initpara[2] = atof(optarg); break;
    case '3': initpara[3] = atof(optarg); break;
    case 's': readRunDatabase("run.db", NULL, optarg, 0); break;
    case '?':
    case ':':
    case 'h': 
      std::cout << 
	"\nUSAGE:   " << argv[0] <<" [options]\n\n"
	"Options:\n" 
	"-f <file>          Input histogram file name.\n"
	"-o <file>          Output postscript file name.\n"
	"-s Setup           Setup for existing run.db entries.\n"
	"-m mode            0: using FWHM position for peak position.\n"
        "                   1: mean value\n"
        "                   2: gaussian fit\n"
	"-{A|B|C}           Spectrometer A/B/C\n"
	"-{0|1|2|3} <value> Specify start parameter for fit.\n"
	"-v                 Verbose output." << std::endl;
      return opt != 'h';
      break;
      default : std::cerr << "Invalid options." << std::endl; 
    }
  }

  // If no input file is defined we quit
  if (!infile) {
    std::cerr << "No input file specified." << std::endl;
    return -1;
  }
  if (rundb.setup==NULL) std::cerr << "WARNING: No Setup given!"<<std::endl;
  HMReadAll(infile);

  // Prepare output
  if (psfile) {
    HMSetPredefinedStyle(ST_OVERHEAD);
    HMSetOption(WIN_TITLE, 1.0);
    HMSetOption(ON_SCALE_X, 2.0);
    HMSetOption(LAB_X, 2.1);
    HMSetOption(LAB_Y, 1.35);
    HMSetOption(FONTSIZE_LABEL, 0.57);
    HMSetOption(FONTSIZE_AXIS, 0.50);
    HMSetOption(FONTSIZE_COMMENT, 0.48);
    HMSetOption(FONTSIZE_WINTITLE, 0.55);
    HMSetOption(FONTSIZE_TITLE, 0.75);
    HMSetOption(MRG_TITLE, 0.75);
    HMPlotFile(psfile);
    HMZone(1,1,1);
  }

  int MaxPad = spec == 'B' ? 14 : 15;
  double shift_dE_ToF[MaxPad];
  double shift_dE_ToF_1[MaxPad];

  fillArray(shift_dE_ToF,   "PH Spec. %c/paddle dE=ToF/%d", spec,
	    MaxPad, psfile, mode, initpara, flags);
  fillArray(shift_dE_ToF_1, "PH Spec. %c/paddle dE=ToF-1/%d", spec,
	    MaxPad, psfile, mode, initpara, flags);

  if (psfile) HMClose();

  double *offsetdE=NULL, *offsetToF=NULL;
  switch (spec) { 
  case 'A': offsetdE = rundb.A.scint.dE; offsetToF = rundb.A.scint.ToF; break;
  case 'B': offsetdE = rundb.B.scint.dE; offsetToF = rundb.B.scint.ToF; break;
  case 'C': offsetdE = rundb.C.scint.dE; offsetToF = rundb.C.scint.ToF; break;
  }

  double diff=0;
  printf("\t%c.scint.dE  = {%6.2f, ",spec, offsetdE[1]);
  for (int i=1; i<MaxPad; i++) {
    diff += shift_dE_ToF[i-1] - shift_dE_ToF_1[i];
    printf("%6.2f", offsetdE[i] - diff);
    if (i==MaxPad-1) puts("}"); else printf(",%s",(i+1)%5?" ":"\n\t\t       ");
  }

  diff=0;
  printf("\t%c.scint.ToF = {%6.2f, ",spec, offsetToF[1]);
  for (int i=2;i<=MaxPad;i++) { 
    diff += shift_dE_ToF[i-1] - shift_dE_ToF_1[i-1];
    printf("%6.2f", offsetToF[i-1] - diff);
    if (i==MaxPad) puts("}"); else printf(",%s", i%5 ? " " : "\n\t\t       ");
  }

  return 0;
}



