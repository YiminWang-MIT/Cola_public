#include "PCAManager.h"

PCAManager::PCAManager()
{
  EigenVectors = MeanValues = SigmaValues = 0;
}

PCAManager::PCAManager( std::string filename )
{
  FileName = filename;
  EigenVectors = MeanValues = SigmaValues = 0;
  Initialize();
}

PCAManager::~PCAManager()
{
  if( EigenVectors!=0 ) delete EigenVectors;
  if( MeanValues!=0 ) delete MeanValues;
  if( SigmaValues!=0 ) delete SigmaValues;
}

void PCAManager::X2P( double *x, double *p )
{
  // It's the users responsibility to make sure that both x and p are of the right size.
  for( int i=0; i<nVariables; i++ ){
    p[i]=0;
    for( int j=0; j<nVariables; j++ ){
      p[i] += (x[j] - MeanValues[j])*EigenVectors[j*nVariables+i]/SigmaValues[j];
    }
  }
}

void PCAManager::P2X( double *p, double *x )
{
  // It's the users responsibility to make sure that both x and p are of the right size.
  for( int i=0; i<nVariables; i++ ){
    x[i] = MeanValues[i];
    for( int j=0; j<nVariables; j++ ){
      x[i] += p[j]*SigmaValues[i]*EigenVectors[i*nVariables+j];
    }
  }
}

void PCAManager::Initialize()
{
  std::ifstream ifs;
  std::string line;
  //  int itmp;
  //  double dtmp;
  ifs.open(FileName.c_str());
  while( getline(ifs,line) ){ if( line[0]=='#' || line.empty() ) continue; break; }
  std::istringstream sline(line); sline >> nVariables;
  MeanValues   = new double [nVariables];
  SigmaValues   = new double [nVariables];
  EigenVectors = new double [nVariables*nVariables];
  // Mean
  while( getline(ifs,line) ){ if( line[0]=='#' || line.empty() ) continue; break; }
  line = line.substr(0,line.find_last_not_of(' ')+1);
  sline.clear(); sline.str(line);
  int counter=0;
  while( !sline.eof() ){
    sline >> MeanValues[counter++];
    if( nVariables<counter ){
      std::cerr << " 1 too many values are written in file[" << FileName << ']' << std::endl;
      return;
    }
  }
  // Sigma
  while( getline(ifs,line) ){ if( line[0]=='#' || line.empty() ) continue; break; }
  line = line.substr(0,line.find_last_not_of(' ')+1);
  sline.clear(); sline.str(line);
  counter=0;
  while( !sline.eof() ){
    sline >> SigmaValues[counter++];
    if( nVariables<counter ){
      std::cerr << " 2 too many values are written in file[" << FileName << ']' << std::endl;
      return;
    }
  }
  // EigenValue
  counter=0;
  while( getline(ifs,line) ){
    if( line[0]=='#' ) continue;// break; }
    line = line.substr(0,line.find_last_not_of(' ')+1);
    if( line.empty() ) continue;
    sline.clear(); sline.str(line); 
    while( !sline.eof() ){
      sline >> EigenVectors[counter++];
      if( nVariables*nVariables<counter ){
	std::cerr << " 3 too many values are written in file[" << FileName << ']' << std::endl;
	return;
      }
    }
  }
  ifs.close();
#if 0
  std::cout << " #var=" << nVariables << std::endl;
  std::cout << " Mean=" << std::endl;
  for( int i=0; i<nVariables; i++ ){
    std::cout << "  " << MeanValues[i];
  }
  std::cout << std::endl;
  std::cout << " Sigma=" << std::endl;
  for( int i=0; i<nVariables; i++ ){
    std::cout << "  " << SigmaValues[i];
  }
  std::cout << std::endl;
  std::cout << " EigenVector=" << std::endl;
  for( int i=0; i<nVariables; i++ ){
    for( int j=0; j<nVariables; j++ ){
      std::cout << "  " << EigenVectors[i*nVariables+j];
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
  std::cout << " Finish !! " << std::endl;
#endif
}
