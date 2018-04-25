#ifndef KAOSMATRIXMAN_
#define KAOSMATRIXMAN_ 1

#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h> /* added by P.A., includes exit() */
#include <vector>
#include <cmath>

#include "PCAManager.h"

class MatrixManager //: public TObject
{
 public:
  MatrixManager();
  MatrixManager(std::string filename);
  ~MatrixManager();

 private:
  std::string FileName;
  int nVariables;
  int nCalc;
  int nCoefficients;
  double *MeanValues;
  double *MinValues, *MaxValues;
  double *Coefficient;
  int *Power;
  
  double *Var, *Val;

 public:
  void Initialize();
  int nvar() { return nVariables; }
  int nval() { return nCalc; }
  void Calc( double *var );
  
  // It's the users responsibility to make sure that var is of the right size.
  void GetVar( double *var );
  // It's the users responsibility to make sure that val is of the right size.
  void GetVal( double *val );

  double GetVal( int i );

  //ClassDef(MatrixManager,1);
};

class KaosMatrixManager //: public TObject
{
 public:
  KaosMatrixManager();
  KaosMatrixManager(std::string filename );
  KaosMatrixManager(std::string filename, std::string pca1file, std::string pca2file );
  ~KaosMatrixManager();

 private:
  std::vector <MatrixManager *> matrix_container;
  PCAManager *PCA1, *PCA2;
  bool UsePCA;

  int nVariables;
  int nCalc;
  double *Var, *Val;

 public:
  void SetMatrixFile( const std::string &filename );
  void SetPCAFile( const std::string &filename1, const std::string &filename2 );
  void Calc( double *x, double *y );
  void CalcAsCola( double *x, double *y );
  PCAManager *pca1() { return PCA1; }
  PCAManager *pca2() { return PCA2; }
  // It's the users responsibility to make sure that var is of the right size.
  void GetVar( double *var );
  // It's the users responsibility to make sure that val is of the right size.
  void GetVal( double *val );

  bool CalcFocalPlanePosition( double hitxG, double hityG, double hitzG, double theta_T, double phi_T,
			       double *fe_param,
			       double &xf, double &xfp, double &yf, double &yfp );

  //ClassDef(KaosMatrixManager,1);
};

#endif
