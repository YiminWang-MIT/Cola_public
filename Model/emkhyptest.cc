#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

#include "HMBook/hmbook.h"
#include "HMBook/hmplot.h"

int main(int argc, char *argv[])
{
  if (argc!=3) {
    std::cout << "Usage: emkhyptest data chn (1=Lambda; 2=Sigma)" << std::endl; 
    exit(0);
  }
  char filename[50]; 
  sprintf(filename, "%s.out", argv[1]);

  const int n = 6;
  const int m = 100;

  double y[m][n];
  double x[m];
  
  int i= 0;

  std::ifstream infile(filename);
  while (!infile.eof() && i<m) {
    infile >> x[i] >> y[i][0] >> y[i][1] >> y[i][2]   
	   >> y[i][3] >> y[i][4] >> y[i][5];
    //    cout << "reading cos theta_K =" <<  x[i]
    //	 << "  sigma_K =" <<  y[i][5] << endl;
    i++;
  }
  infile.close();

  HIST *dsdth= HMBook1("d[s]_o!dcos[Q]", "cos [Q]^*!_K!", 
		       "d[s]_o!/dcos [Q]^*!_K!", "", "[m]b/sr", 100, -0.1, 0.9);

  for(int j= 0; j<i; j++){HMFill(dsdth, x[j], 1, y[j][5]/1000);}

  sprintf(filename, "emkhyptest-%s-chn-%s.his", argv[1], argv[2]);
  HMWriteAll(filename);

  sprintf(filename, "emkhyptest-%s-th-chn-%s.ps", argv[1], argv[2]);
  HMPlotFile(filename);
  HMTitle("KMaid [g] p [\256] K^+!Y");
  HMSetOption( 9, 2. ); // LAB_X
  HMSetOption(10, 1.5); // LAB_Y
  HMSetOption(15, 0.8); // FONTSIZE_COMMENT
  HMSetOption(13, 0.5); // FONTSIZE_AXIS
  HMSetOption(14, 0.8); // FONTSIZE_LABEL
  HMSetOption(37, 0.1); // LINE_WIDTH
  HMMinimumY(0); HMMaximumY(2.0);
  HMFunction(dsdth);
  HMClose();

  return 0;
}

