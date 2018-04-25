#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fstream.h>

#include "HMBook/hmbook.h"
#include "HMBook/hmplot.h"
#include "TMA/TraceMachine.h"      // Transport matrices

int
main(int argc, char *argv[])
{
  if (argc<3) {
    cerr << "Usage: "<< argv[0] << " "<<"TMA-File pathlengthfile" << endl;
    exit(1);
  }

  TracMachine tpA(argv[1], 37.4, 785, 1.2, 4.2);
  
  double h = 0.1;
  double offset_x, offset_theta, offset_y, offset_phi, scale_x, scale_y;
  
  ifstream fp(argv[2]);
  if (!fp.is_open()) perror("Can't open out.lis");
  fp >>offset_x >>offset_theta >>offset_y >>offset_phi >>scale_x >>scale_y;

  double x,y,theta,phi,l;
  struct target_ko *result = NULL;

  y   = offset_y;
  phi = offset_phi;

  HIST *id1=HMBook2("x [\\253] [q]",	   "x","[q]","l",
		   "cm","mrad","cm",	   50, -500, 1000, 50,  -70,   70);
  HIST *id2=HMBook2("y [\\253] [f]",	   "y","[f]","l",
		   "cm","mrad","cm",	   50, -50, 50,  50,  -70,   70);
  HIST *id_x     = HMBook1("x [\\256] l",   
			   "dl/dx",   "counts", "cm/cm",  "", 200, 0,0.2); 
  HIST *id_theta = HMBook1("[q] [\\256] l", 
			   "dl/d[q]", "counts", "cm/mrad","", 200, 0,1.0); 
  HIST *id_y     = HMBook1("y [\\256] l",   
			   "dl/dy",   "counts", "cm/cm",  "", 200, 0,0.2); 
  HIST *id_phi   = HMBook1("[f] [\\256] l", 
			   "dl/d[f]", "counts", "cm/mrad","", 200, 0,0.3); 

  for(x = -500+1500.0/100; x < 1000; x += 1500.0/50) {
    for(theta = -70 + 140.0/100; theta < 70; theta += 140.0/50) {
      result = tpA.run(x, theta + offset_theta, y, phi);
      HMFill(id1,x, theta, result->len);
    }
  }
  x=offset_x;
  theta = offset_theta;
  for(y = -50+100.0/100; y < 50; y += 100.0/50) {
    for(phi = -70+140.0/100; phi < 70; phi += 140.0/50) {
      result = tpA.run(x, theta, y + offset_y, phi + offset_phi);
      HMFill(id2, y, phi, result->len);
    }
  }
 
  while (!fp.eof()) {
    fp >> x >> theta >> y >> phi >> l;
 
    result = tpA.run(x, theta, y, phi);

    double error[4];
    error[0]  = (result = tpA.run(x+h, theta, y, phi),result->len);
    error[0] -= (result = tpA.run(x-h, theta, y, phi),result->len);
    error[0] /= 2*h;
    error[1]  = (result = tpA.run(x, theta+h, y, phi),result->len);
    error[1] -= (result = tpA.run(x, theta-h, y, phi),result->len);
    error[1] /= 2*h;
    error[2]  = (result = tpA.run(x, theta, y+h, phi),result->len);
    error[2] -= (result = tpA.run(x, theta, y-h, phi),result->len);
    error[2] /= 2*h;
    error[3]  = (result = tpA.run(x, theta, y, phi+h),result->len);
    error[3] -= (result = tpA.run(x, theta, y, phi-h),result->len);
    error[3] /= 2*h;

    HMFill(id_x,    fabs(error[0]), 0, 1);    
    HMFill(id_theta,fabs(error[1]), 0, 1);    
    HMFill(id_y,    fabs(error[2]), 0, 1);    
    HMFill(id_phi,  fabs(error[3]), 0, 1);    
    static int i=0;
    if (!(++i % 1000)) cerr << "\r" << i <<flush;
  }
  cerr << "\n";

  HMWriteAll("Pathlengthresolution.his");

  HMSetPredefinedStyle(ST_OVERHEAD);
  HMSetFillRGB(1,1,0);

  HMPlotFile("Pathlengthresolution.ps");

  HMZone(2,2,1);
  HMTitle("Magnification");
  HMHistogram(id_x);
  HMHistogram(id_theta);
  HMHistogram(id_y);
  HMHistogram(id_phi);

  HMZone(1,2,1);
  HMTitle("Pathlength");
  HMSetOption(LAB_X, 2.3);
  HMSurface(id1); 
  HMSurface(id2); 

  HMClose();
  return 0;
}

