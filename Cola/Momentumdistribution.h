//                                                                    -*-c++-*-
// Momentum Distribution for nucleons in 12C
// 
// used for Simulation
// Use:
//   pwave12C.transform(sobol()) * 1e-3;
// to generate a momentum p^2 dp
// 
//#include "HMBook/hmbook.h"
//#include "HMBook/hmplot.h"
//#include <iostream>
//#include <fstream>
//#include <math.h>

class momDistrib {
private:
  float *x;
  float *y;
  float *integral;
  int n;
public:
  ~momDistrib() { delete [] x; delete [] y; delete [] integral;};
  momDistrib(int n, const float [][2], float x1, float y1, float x2, float y2);
  float transform(float y) const;  
};

momDistrib::momDistrib(int nn, const float tab[][2], float x1, float y1, 
		       float x2, float y2)
{ 
  n = nn;
  x        = new float[n];
  y        = new float[n];
  integral = new float[n];
  integral[0]=0;
  for(int i = 0; i<n; i++) {
    x[i] = (tab[i][0] - x1) / (x2 - x1) * 300.0;
    y[i] = (tab[i][1] - y1) / (y2 - y1) * 50;
    y[i] *= x[i] * x[i];
    if (i>0) integral[i] = integral[i-1] + (y[i] + y[i-1])/2 * (x[i] - x[i-1]);
  }
  for(int i=0; i<n; i++) y[i]/=integral[n-1];
  for(int i=0; i<n; i++) integral[i]/=integral[n-1];
}

float momDistrib::transform(float z) const
{
  for(int i=1; i<n; i++)
    if (integral[i]>z && integral[i-1] <= z) {
      double m = (y[i] - y[i-1]) / (x[i] - x[i-1]);
      return x[i-1]-(y[i-1]-sqrt(y[i-1]*y[i-1] + 2*m* (z-integral[i-1])))/m;
    }
  return x[n-1];
}

// Momentum Distribution from 
//   S. Frullani and J. Mougey in
//   Advances in Nuclear Physics (J.W.Negele and E. Vogt, Editors), Vol. 14,
//   Plenum Press, New York (1984)

const float pwave12Ctab[25][2] = {
  { 313, 1180},  { 361, 1147},  { 402, 1094},  { 439, 1036},  { 487,  925},
  { 554,  766},  { 653,  555},  { 701,  469},  { 720,  447},  { 731,  436},
  { 764,  410},  { 782,  395},  { 797,  388},  { 835,  392},  { 875,  403},
  { 890,  414},  { 975,  495},  {1048,  584},  {1204,  810},  {1303,  939},
  {1359,  999},  {1469, 1099},  {1591, 1169},  {1713, 1206},  {1842, 1235}};

const float  swave12Ctab[17][2] = {
  { 313, 1391},  { 387, 1413},  { 435, 1450},  { 498, 1506},  { 528, 1539},
  { 590, 1628},  { 635, 1691},  { 760, 1905},  { 816, 1987},  { 908, 2120},
  { 989, 2212},  {1093, 2323},  {1218, 2405},  {1348, 2460},  {1477, 2486},
  {1636, 2508},  {1861, 2520}};

const momDistrib swave12C(17, swave12Ctab, 313, 2520, 1861, 1243);
const momDistrib pwave12C(25, pwave12Ctab, 313, 1243, 1846,  310);

/*
int main(int, char*[])
{
  ifstream f("/users/kpha1/merkel/p12c.tab");

  HMSetPredefinedStyle(ST_OVERHEAD);
  HMSetOption(FONTSIZE_COMMENT,0.35);
  HMPlotFile("Momentum12C.ps");
  HMTitle("Momentum Density in ^12!C(e,e'p)^11!B");
  HMZone(1,2,1);
  HMPlotFunction(pwave12C.n, pwave12C.x, pwave12C.y,
		 "DWIA Momentum Distribution p_3/2!",
		 "p","[r]","MeV/c","(GeV/c)^3!");
  HMPlotFunction(pwave12C.n, pwave12C.x, pwave12C.integral,
		 "DWIA Momentum Distribution p_3/2!",
		 "p","[r]","MeV/c","(GeV/c)^3!");
  HMPlotFunction(swave12C.n, swave12C.x, swave12C.y,
		 "DWIA Momentum Distribution s_1/2!",
		 "p","[r]","MeV/c","(GeV/c)^3!");
  HMPlotFunction(swave12C.n, swave12C.x, swave12C.integral,
		 "DWIA Momentum Distribution s_1/2!",
		 "p","[r]","MeV/c","(GeV/c)^3!");

  HIST *id1 = HMBook1("DWIA Momentum Distribution s_1/2!",
		      "p","[r]","MeV/c","(GeV/c)^3!",	  100,0.0,300.0);
  HIST *id2 = HMBook1("DWIA Momentum Distribution p_1/2!",
		      "p","[r]","MeV/c","(GeV/c)^3!",	  100,0.0,300.0);
  for (int i=0;i<10000;i ++) HMFill(id1, swave12C.transform(i/10000.0),0,1);
  for (int i=0;i<10000;i ++) HMFill(id2, pwave12C.transform(i/10000.0),0,1);
  HMHistogram(id1);
  HMHistogram(id2);
  HMClose();

  return 0;
}
*/








