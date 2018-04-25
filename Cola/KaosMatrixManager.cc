#include "KaosMatrixManager.h"

//ClassImp(MatrixManager);

MatrixManager::MatrixManager()
{}

MatrixManager::MatrixManager( std::string filename )
{
  FileName = filename;
  Initialize();
}

MatrixManager::~MatrixManager()
{
  std::cout << " Deconstructor of Matrix Manager " << std::endl;
}

void MatrixManager::Initialize()
{
  std::ifstream ifs;
  std::string line;
  //int itmp;
  //double dtmp;
  ifs.open(FileName.c_str());
  //
  while( getline(ifs,line) ){ if( line[0]=='#' || line.empty() ) continue; break; }
  line = line.substr(0,line.find_last_not_of(' ')+1);
  std::istringstream sline(line);
  sline >> nVariables;
  //
  while( getline(ifs,line) ){ if( line[0]=='#' || line.empty() ) continue; break; }
  line = line.substr(0,line.find_last_not_of(' ')+1);
  sline.clear(); sline.str(line);
  sline >> nCalc;
  //
  while( getline(ifs,line) ){ if( line[0]=='#' || line.empty() ) continue; break; }
  line = line.substr(0,line.find_last_not_of(' ')+1);
  sline.clear(); sline.str(line);
  sline >> nCoefficients;
  //
  MeanValues = new double [nCalc];
  while( getline(ifs,line) ){ if( line[0]=='#' || line.empty() ) continue; break; }
  line = line.substr(0,line.find_last_not_of(' ')+1);
  sline.clear(); sline.str(line);
  for( int i=0; i<nCalc; i++ ) sline >> MeanValues[i];
  //
  MinValues = new double [nVariables];
  while( getline(ifs,line) ){ if( line[0]=='#' || line.empty() ) continue; break; }
  line = line.substr(0,line.find_last_not_of(' ')+1);
  sline.clear(); sline.str(line);
  for( int i=0; i<nVariables; i++ ) sline >> MinValues[i];
  //
  MaxValues = new double [nVariables];
  while( getline(ifs,line) ){ if( line[0]=='#' || line.empty() ) continue; break; }
  line = line.substr(0,line.find_last_not_of(' ')+1);
  sline.clear(); sline.str(line);
  for( int i=0; i<nVariables; i++ ) sline >> MaxValues[i];
  //
  Coefficient = new double [nCalc*nCoefficients];
  Power = new int [nVariables*nCoefficients];
  int counter1=0;
  int counter2=0;
  while( getline(ifs,line) ){
    if( line[0]=='#' ) continue;
    line = line.substr(0,line.find_last_not_of(' ')+1);
    if(line.empty()) continue;
    sline.clear(); sline.str(line);
    for( int i=0; i<nVariables; i++ ){
      sline >> Power[counter1++];
    }
    for( int i=0; i<nCalc; i++ ){
      sline >> Coefficient[counter2+i*nCoefficients];
    }
    counter2++;
    if( nVariables*nCoefficients<counter1 ){ std::cerr << " !! too many (1)!! " << std::endl; exit(0); }
    if( nCalc*nCoefficients<counter2 ){ std::cerr << " !! too many (2)!! " << std::endl; exit(0); }
  }

  Var = new double[nVariables];
  Val = new double[nCalc];
#if 0
  std::cout << nVariables << "  "
	    << nCalc << "  "
	    << nCoefficients << std::endl;
  for( int i=0; i<nCalc; i++ ) std::cout << MeanValues[i] << "  "; std::cout << std::endl;
  for( int i=0; i<nVariables; i++ ) std::cout << MinValues[i] << "  "; std::cout << std::endl;
  for( int i=0; i<nVariables; i++ ) std::cout << MaxValues[i] << "  "; std::cout << std::endl;
  for( int i=0; i<nCoefficients; i++ ){
    for( int j=0; j<nVariables; j++ ) std::cout << Power[i*nVariables+j] << "  ";
    for( int j=0; j<nCalc; j++ ) std::cout << Coefficient[i+j*nCoefficients] << "  ";
    std::cout << std::endl;
  }
#endif
}

void MatrixManager::Calc( double *var )
{
  for( int i=0; i<nVariables; i++ ){
    Var[i] = var[i];
  }
  for( int i=0; i<nCalc; i++ ){
    double value = MeanValues[i];
    for( int j=0; j<nCoefficients; j++ ){
      double term = Coefficient[i*nCoefficients+j];
      for( int k=0; k<nVariables; k++ ){
	int power = Power[j*nVariables+k];
	double p1=1,p2=0,p3=0,r=0;
	double v=1+2./(MaxValues[k]-MinValues[k])*(Var[k]-MaxValues[k]);
	switch(power){
	case 0: r = 1; break;
	case 1: r = v; break;
	default:
	  p2 = v;
	  for( int l=2; l<=power; l++ ){
	    p3 = p2*v;
	    p1 = p2; p2 = p3;
	  }
	  r = p3;
	}
	term *= r;
      }
      value += term;
    }
    Val[i] = value;
  }
}

void MatrixManager::GetVar( double *var )
{
  // It's the users responsibility to make sure that var is of the right size.
  for( int i=0; i<nVariables; i++ ) var[i] = Var[i];
}

void MatrixManager::GetVal( double *val )
{
  // It's the users responsibility to make sure that val is of the right size.
  for( int i=0; i<nCalc; i++ ) val[i] = Val[i];
}

double MatrixManager::GetVal( int i )
{
  if( i<nCalc ) return Val[i];
  else          return -9999;
}

//ClassImp(KaosMatrixManager);

KaosMatrixManager::KaosMatrixManager()
{
  UsePCA = false;
}

KaosMatrixManager::KaosMatrixManager( std::string filename )
{
  UsePCA = false;
  SetMatrixFile(filename);
}

KaosMatrixManager::KaosMatrixManager( std::string filename, std::string pca1file, std::string pca2file )
{
  UsePCA = false;
  SetMatrixFile(filename);
  SetPCAFile(pca1file,pca2file);
}

KaosMatrixManager::~KaosMatrixManager()
{
}

void KaosMatrixManager::SetMatrixFile( const std::string &filename )
{
  MatrixManager *mat = new MatrixManager(filename);
  matrix_container.push_back(mat);

  int nn = matrix_container.size();
  if( 1<nn ){
    if( nVariables != matrix_container[nn-1]->nvar() ){
      std::cerr << " #Variables is not consistent !!! " << std::endl;
      exit(-1);
    }
    if( nCalc != matrix_container[nn-1]->nval() ){
      std::cerr << " #Calc is not consistent !!! " << std::endl;
      exit(-1);
    }
  }
  else{
    nVariables = matrix_container[nn-1]->nvar();
    nCalc = matrix_container[nn-1]->nval();
    Var = new double[nVariables];
    Val = new double[nCalc];
  }
}

void KaosMatrixManager::SetPCAFile( const std::string &filename1, const std::string &filename2 )
{
  PCA1 = new PCAManager(filename1);
  PCA2 = new PCAManager(filename2);
  UsePCA = true;
}

void KaosMatrixManager::Calc( double *x, double *y )
{
  if( UsePCA ){
    PCA1->X2P(x,Var);
  }
  else{
    for( int i=0; i<nVariables; i++ ) Var[i] = x[i];
  }
  
  for( int i=0; i<nCalc; i++ ) Val[i] = 0;
  for( int i=0; i<(int)matrix_container.size(); i++ ){
    matrix_container[i]->Calc(Var);
    for( int j=0; j<matrix_container[i]->nval(); j++ ){
      Val[j] += matrix_container[i]->GetVal(j);
    }
  }

  if( UsePCA ){
    PCA2->P2X(Val,y);
  }
  else{
    for( int i=0; i<nCalc; i++ ) y[i] = Val[i];
  }
}

void KaosMatrixManager::CalcAsCola( double *x, double *y )
{
  if( UsePCA ){
    PCA1->X2P(x,Var);
  }
  else{
    for( int i=0; i<nVariables; i++ ) Var[i] = x[i];
  }
  
  for( int i=0; i<nCalc; i++ ) Val[i] = 0;
  for( int i=0; i<(int)matrix_container.size(); i++ ){
    matrix_container[i]->Calc(Var);
    for( int j=0; j<matrix_container[i]->nval(); j++ ){
      Val[j] += matrix_container[i]->GetVal(j);
    }
  }

  if( UsePCA ){
    PCA2->P2X(Val,y);
    y[0] *= 100; // % -> dp/p
    y[1] = y[1]; // cm -> cm
    y[2] = y[2]/1000.; // mrad -> rad
    y[3] = y[3]/1000.; // mrad -> rad
  }
  else{
    for( int i=0; i<nCalc; i++ ) y[i] = Val[i];
  }
}

void KaosMatrixManager::GetVar( double *var )
{
  // It's the users responsibility to make sure that var is of the right size.
  if( UsePCA ){
    for( int i=0; i<nVariables; i++ ) var[i] = Var[i];
  }
  else{
    for( int i=0; i<nVariables; i++ ) var[i] = -9999;
  }
}

void KaosMatrixManager::GetVal( double *val )
{
  // It's the users responsibility to make sure that val is of the right size.
  if( UsePCA ){
    for( int i=0; i<nCalc; i++ ) val[i] = Val[i];
  }
  else{
    for( int i=0; i<nCalc; i++ ) val[i] = -9999;
  }
}

bool KaosMatrixManager::CalcFocalPlanePosition( double hitxG, double hityG, double hitzG, double theta, double phi,
						double *fe_param,
						double &xf, double &xfp, double &yf, double &yfp )
{
  // this is just copy from KaosManager
  double deg2rad = 3.141592/180.;//TMath::DegToRad();

  // !!! this function is for new coordinate system, namely, 2<use_tof
  double A = fe_param[0];//Geo_fe0;
  double B = fe_param[1];//Geo_fe1;
  double C = fe_param[2];//Geo_fe2;
  double x_R = fe_param[3];//Geo_fe3;
  double theta_R = fe_param[4]*deg2rad;//Geo_fe4*deg2rad;
#if 0
  std::cout << " A:" << A << " B:" << B << " C:" << C << std::endl
	    << " x_R::" << x_R << " theta_R:" << theta_R << std::endl;
#endif

  double theta_T = theta*deg2rad;
  double phi_T = phi*deg2rad;
  double tant = tan(theta_T);
    
  double x_T = hitxG - hitzG*tant;
  double y_T = hityG - tan(phi_T)*sqrt( pow(hitxG,2) + pow(hitzG,2) );
    
  double discriminant = pow(B-1./tant,2) - 4.*A*(C+x_T/tant);
  double x_i=0, y_i=0, z_i=0;
  if(0<discriminant){
    x_i = (-1.*(B-1./tant)-sqrt(discriminant))/(2.*A);
    z_i = (x_i-x_T)/tant;
    y_i = y_T+z_i/cos(theta_T)*tan(phi_T);
      
    xf = (x_i-x_R-z_i*tan(theta_R))*cos(theta_R); // mm
    yf = y_i; // mm
    xfp = (theta_T-theta_R)*1000.; // mrad
    yfp = phi_T*1000.;             // mrad

  }

  return true;
}
