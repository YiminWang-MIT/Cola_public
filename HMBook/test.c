#include "hmbook.h"
#include "hmplot.h"
#include <math.h>

float sinus(float x) { return 100.3*fabs(sin(x+.5)); }

double sinus2(double p[2], double x) 
{ return p[0] * pow(fabs(sin(x+p[1])),1.1); }

int main(int arc, char *argv[])
{
  HIST *h = HMBook1("Sinus","[f]","[#]","rad","",100,0.0,6.28);
  double par[2];
  par[0]=1;par[1]=1;
  HMFillFunction1(h,sinus);
  HMPipe();
  HMHistogram(h);
  HMFlush();
  getchar();
  HMFit1(h, sinus2, par, 2);
  HMFlush();
  getchar();
  
  HMWriteAll("test.his");
  HMClose();
  return 0;
}

















