#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "TMA/FastTM.h"
#include "TCCC.h"
#include <cmath>
#include <iostream>
#ifdef HAVE_ISTREAM
#include <istream>
#endif
#include <fstream>
#include <iosfwd>

TCCC::TCCC(int spec, const char *fname) {
  std::ifstream infile(fname);
  if (!infile) {
    std::cerr << "TCCC: unable to open file " << fname 
	      << " --- using defaults from TCCC.h" << std::endl;
    if (spec==0) {
      rzero = OHIPS_offset;
      central_ray = OHIPS_pathlen;
    } else if (spec==1) {
      rzero = OOPS_offset;
      central_ray = OOPS_pathlen;
    }
      maxpow_ndisp = maxpow_disp = 0;
    return;
  }
  infile >> rzero >> central_ray >> maxpow_ndisp >> maxpow_disp;

  ndisp = new double[maxpow_ndisp];
  disp  = new double[maxpow_disp*maxpow_disp];
  for (int i=0; i<maxpow_ndisp; i++) infile >> ndisp[i];
  for (int i=0; i<maxpow_disp*maxpow_disp; i++) infile >> disp[i];
  infile.close();
}

TCCC::~TCCC() {
  delete [] ndisp;
  delete [] disp;
}

double
TCCC::run(struct TargetCo *result) {
  double path = 0;
  for (int i=0; i<maxpow_ndisp; i++) {
    path += ndisp[i]*pow(result->ph, i);
    for (int j=0; j<maxpow_disp; j++) {
      path += disp[maxpow_ndisp*i+j]*pow(result->th, j)*pow(result->dp, i);
    }
  }
  return path;
}

