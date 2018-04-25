#ifndef PCAMANAGER_
#define PCAMANAGER_ 1

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <cmath>

class PCAManager
{
 public:
  PCAManager();
  PCAManager( std::string filename );
  ~PCAManager();

 private:
  std::string FileName;

  int nVariables;
  double *MeanValues;
  double *SigmaValues;
  double *EigenVectors;

 public:
  void Initialize();
  // It's the users responsibility to make sure that both x and p are of the right size.
  void X2P( double *x, double *p );
  // It's the users responsibility to make sure that both x and p are of the right size.
  void P2X( double *p, double *x );
};
#endif
