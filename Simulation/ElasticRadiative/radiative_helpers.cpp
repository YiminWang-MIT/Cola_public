#include "radiative_helpers.h"
#include <iostream>

/********************************************************************
Elastic Kinematic Variables used in Maximon and Tjon
 *******************************************************************/

ElasticKinematics::ElasticKinematics(double initLeptonE, double leptonTheta)
{
  // Set lepton and proton mass
  double me = 0.510999;
  double mP = 938.272;

  // Calculate all of the Maximon and Tjon variables we need
  E1_ = initLeptonE;
  theta_ = leptonTheta;
  double p1 = sqrt(E1_*E1_ - me*me);
  p3_ = p1*(cos(theta_)*(E1_*mP + me*me) + (E1_+mP)*sqrt(mP*mP-pow(me*sin(theta_),2.)))
    / (pow(p1*sin(theta_),2.) + me*me + 2.*mP*E1_ + mP*mP);
  E3_ = sqrt(p3_*p3_ + me*me);

  //mainz kinematics
  //numerically equiv
  //E3_= (-(E1_ + mP)*(me*me+E1_*mP)
  //  + (me*me-E1_*E1_)*cos(theta_)*sqrt(mP*mP-pow(me*sin(theta_),2)))/
  // ((E1_*E1_-me*me)*cos(theta_)*cos(theta_)-(E1_+mP)*(E1_+mP));
  Q2_ = 2.*(E1_*E3_ - p1*p3_*cos(theta_) - me*me);
  E4_ = E1_ + mP - E3_;
  p4_ = sqrt(E4_*E4_ - mP*mP);
  eta_ = E1_/E3_;
  rho_ = sqrt(Q2_ + 4*mP*mP);
  x_ = pow((rho_ + sqrt(Q2_)),2)/(4*mP*mP);
  tau_ = Q2_/(4.*mP*mP);
}

double ElasticKinematics::eps() const
{
  return 1./(1.+2.*(1.+tau_)*tan(theta_/2.)*tan(theta_/2.));
}

/********************************************************************
Four Matrix
 *******************************************************************/

FourMat::FourMat()
{

  
  for (int i=0; i<16 ; i++)
    {
      real[i]=0;
      imag[i]=0;
    }
}

FourMat::FourMat(const FourMat& rhs)
{

  for (int i=0; i<16 ; i++)
    {
      real[i]=rhs.real[i];
      imag[i]=rhs.imag[i];
    }
}

FourMat::~FourMat()
{
}

FourMat& FourMat::operator=(const FourMat &rhs)
{
  if(!(this==&rhs))
    for (int i=0; i<16 ; i++)
      {
	real[i]=rhs.real[i];
	imag[i]=rhs.imag[i];
      }
  
  return *this;
}

std::complex<double> FourMat::tr() const
{
  return std::complex<double>(real[0]+ real[5]+real[10]+real[15],imag[0]+ imag[5]+imag[10]+imag[15]);
}


std::complex<double> FourMat::multr(const FourMat& rhs) const
{
  double t_r=0,t_i=0;
  for (int i=0;i<4;i++)
    {
      for(int k=0;k<4;k++)
	{
	int li=4*i+k;
	int ri=4*k+i;
	t_r+=real[li]*rhs.real[ri]-imag[li]*rhs.imag[ri];
	t_i+=real[li]*rhs.imag[ri]+imag[li]*rhs.real[ri];
	}
    }
  return std::complex<double>(t_r,t_i);
}

std::complex<double> FourMat::get(int entry1, int entry2) const
{
  return std::complex<double>(real[entry1*4+entry2],imag[entry1*4+entry2]);
}

void FourMat::set(int entry1,int entry2, std::complex<double> value)
{
  real[entry1*4+entry2]=std::real(value);
  imag[entry1*4+entry2]=std::imag(value);
}

void FourMat::set(int entry1,int entry2, double r_, double i_)
{
  real[entry1*4+entry2]=r_;
  imag[entry1*4+entry2]=i_;
}

FourMat FourMat::multGammaOnRight(int ind) const
{
  FourMat result;

  if (ind == 0)
    {
      result.set(0,0,get(0,2)); result.set(0,1,get(0,3)); result.set(0,2,get(0,0)); result.set(0,3,get(0,1));
      result.set(1,0,get(1,2)); result.set(1,1,get(1,3)); result.set(1,2,get(1,0)); result.set(1,3,get(1,1));
      result.set(2,0,get(2,2)); result.set(2,1,get(2,3)); result.set(2,2,get(2,0)); result.set(2,3,get(2,1));
      result.set(3,0,get(3,2)); result.set(3,1,get(3,3)); result.set(3,2,get(3,0)); result.set(3,3,get(3,1));
    }
  else if (ind == 1)
    {
      result.set(0,0,-get(0,3)); result.set(0,1,-get(0,2)); result.set(0,2,get(0,1)); result.set(0,3,get(0,0));
      result.set(1,0,-get(1,3)); result.set(1,1,-get(1,2)); result.set(1,2,get(1,1)); result.set(1,3,get(1,0));
      result.set(2,0,-get(2,3)); result.set(2,1,-get(2,2)); result.set(2,2,get(2,1)); result.set(2,3,get(2,0));
      result.set(3,0,-get(3,3)); result.set(3,1,-get(3,2)); result.set(3,2,get(3,1)); result.set(3,3,get(3,0));
    }
  else if (ind == 2)
    {
      result.set(0,0,-get(0,3)); result.set(0,1,get(0,2)); result.set(0,2,get(0,1)); result.set(0,3,-get(0,0));
      result.set(1,0,-get(1,3)); result.set(1,1,get(1,2)); result.set(1,2,get(1,1)); result.set(1,3,-get(1,0));
      result.set(2,0,-get(2,3)); result.set(2,1,get(2,2)); result.set(2,2,get(2,1)); result.set(2,3,-get(2,0));
      result.set(3,0,-get(3,3)); result.set(3,1,get(3,2)); result.set(3,2,get(3,1)); result.set(3,3,-get(3,0));
      result *=std::complex<double>(0.,1.);
    }
  else if (ind == 3)
    {
      result.set(0,0,-get(0,2)); result.set(0,1,get(0,3)); result.set(0,2,get(0,0)); result.set(0,3,-get(0,1));
      result.set(1,0,-get(1,2)); result.set(1,1,get(1,3)); result.set(1,2,get(1,0)); result.set(1,3,-get(1,1));
      result.set(2,0,-get(2,2)); result.set(2,1,get(2,3)); result.set(2,2,get(2,0)); result.set(2,3,-get(2,1));
      result.set(3,0,-get(3,2)); result.set(3,1,get(3,3)); result.set(3,2,get(3,0)); result.set(3,3,-get(3,1));
    }
  else
    {
      std::cerr << "Bad index in multGammaOnRight!\n";
      exit (-1);
    }
  return result;
}

FourMat FourMat::multGammaOnLeft(int ind) const
{
  FourMat result;
  if (ind == 0)
    {
      result.set(0,0,get(2,0)); result.set(0,1,get(2,1)); result.set(0,2,get(2,2)); result.set(0,3,get(2,3));
      result.set(1,0,get(3,0)); result.set(1,1,get(3,1)); result.set(1,2,get(3,2)); result.set(1,3,get(3,3));
      result.set(2,0,get(0,0)); result.set(2,1,get(0,1)); result.set(2,2,get(0,2)); result.set(2,3,get(0,3));
      result.set(3,0,get(1,0)); result.set(3,1,get(1,1)); result.set(3,2,get(1,2)); result.set(3,3,get(1,3));
    }
  else if (ind == 1)
    {
      result.set(0,0,get(3,0)); result.set(0,1,get(3,1)); result.set(0,2,get(3,2)); result.set(0,3,get(3,3));
      result.set(1,0,get(2,0)); result.set(1,1,get(2,1)); result.set(1,2,get(2,2)); result.set(1,3,get(2,3));
      result.set(2,0,-get(1,0)); result.set(2,1,-get(1,1)); result.set(2,2,-get(1,2)); result.set(2,3,-get(1,3));
      result.set(3,0,-get(0,0)); result.set(3,1,-get(0,1)); result.set(3,2,-get(0,2)); result.set(3,3,-get(0,3));
    }
  else if (ind == 2)
    {
      result.set(0,0,-get(3,0)); result.set(0,1,-get(3,1)); result.set(0,2,-get(3,2)); result.set(0,3,-get(3,3));
      result.set(1,0,get(2,0)); result.set(1,1,get(2,1)); result.set(1,2,get(2,2)); result.set(1,3,get(2,3));
      result.set(2,0,get(1,0)); result.set(2,1,get(1,1)); result.set(2,2,get(1,2)); result.set(2,3,get(1,3));
      result.set(3,0,-get(0,0)); result.set(3,1,-get(0,1)); result.set(3,2,-get(0,2)); result.set(3,3,-get(0,3));
      result *=std::complex<double>(0.,1.);      
    }
  else if (ind == 3)
    {
      result.set(0,0,get(2,0)); result.set(0,1,get(2,1)); result.set(0,2,get(2,2)); result.set(0,3,get(2,3));
      result.set(1,0,-get(3,0)); result.set(1,1,-get(3,1)); result.set(1,2,-get(3,2)); result.set(1,3,-get(3,3));
      result.set(2,0,-get(0,0)); result.set(2,1,-get(0,1)); result.set(2,2,-get(0,2)); result.set(2,3,-get(0,3));
      result.set(3,0,get(1,0)); result.set(3,1,get(1,1)); result.set(3,2,get(1,2)); result.set(3,3,get(1,3));
    }
  else
    {
      std::cerr << "Bad index in multGammaOnLeft()!\n";
      exit (-1);
    }
  return result;
}

FourMat& FourMat::operator+=(const FourMat& rhs)
{
  for (int i=0; i<16 ; i++)
    {
      real[i]+=rhs.real[i];
      imag[i]+=rhs.imag[i];
    }
  return *this;
}

FourMat FourMat::operator+(const FourMat& rhs) const
{
  FourMat theCopy = *this;
  theCopy += rhs;
  return theCopy;
}

FourMat& FourMat::operator-=(const FourMat& rhs)
{
  for (int i=0; i<16 ; i++)
    {
      real[i]-=rhs.real[i];
      imag[i]-=rhs.imag[i];
    }
  return *this;
}

FourMat FourMat::operator-(const FourMat& rhs) const
{
  FourMat theCopy = *this;
  theCopy -= rhs;
  return theCopy;
}

FourMat& FourMat::operator*=(double value)
{
  for (int i=0; i<16 ; i++)
    {
      real[i]*=value;
      imag[i]*=value;
      }
  return *this;	
}

FourMat& FourMat::operator*=(std::complex<double> value)
{
  double rv=std::real(value);
  double iv=std::imag(value);

  for (int i=0; i<16 ; i++)
    {
      double r=real[i]*rv-imag[i]*iv;
      imag[i]=(real[i]*iv+imag[i]*rv);
      real[i]=r;
    }
  return *this;
}

FourMat FourMat::operator*(const FourMat& rhs) const
{
  FourMat theCopy;
  
  for (int i=0;i<4;i++)
    {
      for (int j=0;j<4;j++)
	{
	  double re=0,im=0;
	  for(int k=0;k<4;k++)
	    {
	      int li=4*i+k;
	      int ri=4*k+j;

	      re+=real[li]*rhs.real[ri]-imag[li]*rhs.imag[ri];
	      im+=real[li]*rhs.imag[ri]+imag[li]*rhs.real[ri];
	    }
	  theCopy.real[4*i+j]=re;
	  theCopy.imag[4*i+j]=im;
	}
    }
  
  return theCopy;
}

bool FourMat::operator==(const FourMat& rhs) const
{
  bool result = true;
  
  for (int i=0; i<16 ; i++)
    {
      result &= (real[i]==rhs.real[i]);
      result &= (imag[i]==rhs.imag[i]);
    }
  
  return result;
}

void FourMat::print() const
{
  for (int i=0; i<4 ; i++)
    {
      std::cout << "[ ";
      for (int j=0; j<4 ; j++)
	std::cout << real[i*4+j]<<","<<imag[i*4+j]<<"i ";
      std::cout << "]\n";
    }
}

/********************************************************************
One Index
 *******************************************************************/

OneIndex::OneIndex()
{}

OneIndex::OneIndex(const FourMat& entry0,const FourMat& entry1,const FourMat& entry2,const FourMat& entry3)
{
	matrices[0]=entry0;
	matrices[1]=entry1;
	matrices[2]=entry2;
	matrices[3]=entry3;
}

OneIndex::OneIndex(const OneIndex& rhs)
{
	for (int i=0; i<4; i++)
		matrices[i]=rhs.get(i);
}

OneIndex::~OneIndex()
{}

OneIndex& OneIndex::operator=(const OneIndex &rhs)
{
	if (!(this==&rhs))
	{
		for (int i=0; i<4; i++)
			matrices[i]=rhs.get(i);
	}
	return *this;
}

FourMat OneIndex::slash(const TLorentzVector& vec) const
{
	FourMat slashed = matrices[0];
	slashed *= vec[3];
	
	for (int i=1; i<4; i++)
	{
		FourMat thisTerm = matrices[i];
		thisTerm *= vec[i-1];
		slashed -= thisTerm;
	}
	
	return slashed;	
}

OneIndex OneIndex::leftMultiply(const FourMat& lhs) const
{
  return OneIndex(lhs * matrices[0],
		  lhs * matrices[1],
		  lhs * matrices[2],
		  lhs * matrices[3]);
}

OneIndex OneIndex::rightMultiply(const FourMat& rhs) const
{	
  return OneIndex(matrices[0] * rhs,
		  matrices[1] * rhs,
		  matrices[2] * rhs,
		  matrices[3] * rhs);
}

FourMat OneIndex::contract(const OneIndex& rhs) const
{
  return ((matrices[0]*rhs.get(0))
	  -(matrices[1]*rhs.get(1))
	  -(matrices[2]*rhs.get(2))
	  -(matrices[3]*rhs.get(3)));
}

FourMat OneIndex::contractGammaOnRight() const
{
  FourMat result;
  result += matrices[0].multGammaOnRight(0);
  result -= matrices[1].multGammaOnRight(1);
  result -= matrices[2].multGammaOnRight(2);
  result -= matrices[3].multGammaOnRight(3);
  return result;
}

FourMat OneIndex::contractGammaOnLeft() const
{
  FourMat result;
  result += matrices[0].multGammaOnLeft(0);
  result -= matrices[1].multGammaOnLeft(1);
  result -= matrices[2].multGammaOnLeft(2);
  result -= matrices[3].multGammaOnLeft(3);
  return result;
}

std::complex<double> OneIndex::get(int fourIndex, int entry1, int entry2) const
{
	return matrices[fourIndex].get(entry1,entry2);
}

const FourMat& OneIndex::get(int fourIndex) const
{
	return matrices[fourIndex];
}

void OneIndex::set(int fourIndex, const FourMat& mat)
{
	matrices[fourIndex] = mat;
}

void OneIndex::set(int fourIndex, int entry1, int entry2, std::complex<double> value)
{
	matrices[fourIndex].set(entry1,entry2,value);
}

void OneIndex::set(int fourIndex, int entry1, int entry2, double real, double imag)
{
	matrices[fourIndex].set(entry1,entry2,real,imag);
}

OneIndex& OneIndex::operator+=(const OneIndex& rhs)
{
	for (int i=0 ; i<4 ; i++)
		matrices[i]+=rhs.get(i);
	
	return *this;
}

OneIndex OneIndex::operator+(const OneIndex& rhs) const
{
	OneIndex theCopy = *this;
	theCopy += rhs;
	return theCopy;
}

OneIndex& OneIndex::operator-=(const OneIndex& rhs)
{
	for (int i=0 ; i<4 ; i++)
		matrices[i]-=rhs.get(i);
	
	return *this;
}

OneIndex OneIndex::operator-(const OneIndex& rhs) const
{
	OneIndex theCopy = *this;
	theCopy -= rhs;
	return theCopy;
}

OneIndex& OneIndex::operator*=(double value)
{
	for (int i=0 ; i<4 ; i++)
		matrices[i]*=value;
	
	return *this;
}

OneIndex& OneIndex::operator*=(std::complex<double> value)
{
	for (int i=0 ; i<4 ; i++)
		matrices[i]*=value;
	
	return *this;
}

bool OneIndex::operator==(const OneIndex& rhs) const
{
	bool result = true;
	
	for (int i=0 ; i<4 ; i++)
		result &= (matrices[i]==rhs.get(i));
		
	return result;
}

/********************************************************************
Gamma
 *******************************************************************/

FourMat Gamma::slash(const TLorentzVector& vec) const
{
  FourMat result;
  result.set(0,2,vec.T()-vec.Z(),0.);
  result.set(0,3,-vec.X(),vec.Y());
  result.set(1,2,-vec.X(),-vec.Y());
  result.set(1,3,vec.T()+vec.Z());
  result.set(2,0,vec.T()+vec.Z());
  result.set(2,1,vec.X(),-vec.Y());
  result.set(3,0,vec.X(),vec.Y());
  result.set(3,1,vec.T()-vec.Z());
  return result;
}

/********************************************************************
Two Tensor
 *******************************************************************/

TwoTensor::TwoTensor()
{
}

TwoTensor::TwoTensor(const TwoTensor& rhs)
{
  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      {
	real[4*i+j] = rhs.getRe(i,j);
	imag[4*i+j] = rhs.getIm(i,j);
      }
}

TwoTensor::~TwoTensor()
{
}

TwoTensor& TwoTensor::operator=(const TwoTensor &rhs)
{
  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      {
	real[4*i+j] = rhs.getRe(i,j);
	imag[4*i+j] = rhs.getIm(i,j);
      }
  return *this;
}

std::complex<double> TwoTensor::selfContract() const
{
  return std::complex<double>(real[0] - real[5] - real[10] - real[15],
			      imag[0] - imag[5] - imag[10] - imag[15]);
}

std::complex<double> TwoTensor::contractInOrder(const TwoTensor& rhs)
{
  double realResult = 0.;
  double imagResult = 0.;
  
  for (int i=0;i<4;i++)
    for (int j=0; j<4; j++)
      {
	double sign = ((i==0)?1.:-1.)*((j==0)?1.:-1.);
	realResult += (sign * (real[4*i+j] * rhs.getRe(i,j) - imag[4*i+j] * rhs.getIm(i,j)));
	imagResult += (sign * (imag[4*i+j] * rhs.getRe(i,j) + real[4*i+j] * rhs.getIm(i,j)));
      }
  
  return std::complex<double>(realResult,imagResult);
}

std::complex<double> TwoTensor::contractOutOfOrder(const TwoTensor& rhs)
{
  double realResult = 0.;
  double imagResult = 0.;
  
  for (int i=0;i<4;i++)
    for (int j=0; j<4; j++)
      {
	double sign = ((i==0)?1.:-1.)*((j==0)?1.:-1.);
	realResult += (sign * (real[4*i+j] * rhs.getRe(j,i) - imag[4*i+j] * rhs.getIm(j,i)));
	imagResult += (sign * (imag[4*i+j] * rhs.getRe(j,i) + real[4*i+j] * rhs.getIm(j,i)));
      }
  
  return std::complex<double>(realResult,imagResult);
}

std::complex<double> TwoTensor::get(int ind1, int ind2) const
{
  return std::complex<double>(real[4*ind1+ind2],imag[4*ind1+ind2]);
}

double TwoTensor::getRe(int ind1, int ind2) const
{
  return real[4*ind1+ind2];
}

double TwoTensor::getIm(int ind1, int ind2) const
{
  return imag[4*ind1+ind2];
}

void TwoTensor::set(int ind1, int ind2, std::complex<double> value)
{
  real[4*ind1 + ind2] = std::real(value);
  imag[4*ind1 + ind2] = std::imag(value);
}

void TwoTensor::set(int ind1, int ind2, double r, double i)
{
  real[4*ind1 + ind2] = r;
  imag[4*ind1 + ind2] = i;
}

TwoTensor& TwoTensor::operator+=(const TwoTensor& rhs)
{
  for (int i=0; i<4; i++)
    for (int j=0; j<4 ; j++)
      {
	real[4*i + j] += rhs.getRe(i,j);
	imag[4*i + j] += rhs.getIm(i,j);
      }

  return *this;
}

TwoTensor TwoTensor::operator+(const TwoTensor& rhs) const
{
  TwoTensor sum = *this;
  sum+=rhs;
  return sum;
}

TwoTensor& TwoTensor::operator-=(const TwoTensor& rhs)
{
  for (int i=0; i<4; i++)
    for (int j=0; j<4 ; j++)
      {
	real[4*i + j] -= rhs.getRe(i,j);
	imag[4*i + j] -= rhs.getIm(i,j);
      }

  return *this;	
}

TwoTensor TwoTensor::operator-(const TwoTensor& rhs) const
{
  TwoTensor sum = *this;
  sum-=rhs;
  return sum;
}

TwoTensor& TwoTensor::operator*=(double value)
{
  for (int i=0; i<16; i++)
    {
      real[i] *= value;
      imag[i] *= value;
    }
  
  return *this;
}

TwoTensor& TwoTensor::operator*=(std::complex<double> value)
{
  double r = std::real(value);
  double i = std::imag(value);

  for (int j=0; j<16; j++)
    {
      double tempR = real[j]*r - imag[j]*i;
      imag[j] = imag[j]*r + real[j]*i;
      real[j] = tempR;
    }
	
  return *this;
}

bool TwoTensor::operator==(const TwoTensor& rhs) const
{
  bool result = true;
  
  for (int i=0; i<4; i++)
    for (int j=0; j<4 ; j++)
      {
	result &= (real[4*i+j]==rhs.getRe(i,j));
	result &= (imag[4*i+j]==rhs.getIm(i,j));
      }
  return result;
}

/********************************************************************
Two Index
 *******************************************************************/

TwoIndex::TwoIndex()
{}

TwoIndex::TwoIndex(const TwoIndex& rhs)
{
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			matrices[i][j]=rhs.get(i,j);
}

TwoIndex::~TwoIndex()
{}

TwoIndex& TwoIndex::operator=(const TwoIndex &rhs)
{
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			matrices[i][j]=rhs.get(i,j);
	
	return *this;
}

OneIndex TwoIndex::contractFirst(const TLorentzVector& vec) const
{
  OneIndex result;
  
  for (int i=0; i<4; i++)
    {
      FourMat tempA=matrices[0][i];
      tempA *= vec[3];
      
      for (int j=1; j<4; j++)
	{
	  FourMat tempB = matrices[j][i];
	  tempB *= vec[j-1];
	  tempA -= tempB;
	}
      
      result.set(i,tempA);
    }
  
  return result;
}

OneIndex TwoIndex::contractSecond(const TLorentzVector& vec) const
{
  OneIndex result;
  
  for (int i=0; i<4; i++)
    {
      FourMat tempA=matrices[i][0];
      tempA *= vec[3];
      
      for (int j=1; j<4; j++)
	{
	  FourMat tempB = matrices[i][j];
	  tempB *= vec[j-1];
	  tempA -= tempB;
	}
      
      result.set(i,tempA);
    }
  
  return result;	
}

OneIndex TwoIndex::contractFirstOnLeft(const OneIndex& lhs) const
{
	OneIndex result;
	
	for (int i=0 ; i < 4 ; i++)
	{
		FourMat tempA = (lhs.get(0) * matrices[0][i]);
		
		for ( int j=1 ; j < 4 ; j++)
			tempA -= (lhs.get(j) * matrices[j][i]);
		
		result.set(i,tempA);
	}

	return result;
}

OneIndex TwoIndex::contractSecondOnLeft(const OneIndex& lhs) const
{
	OneIndex result;
	
	for (int i=0 ; i < 4 ; i++)
	{
		FourMat tempA = (lhs.get(0) * matrices[i][0]);
		
		for ( int j=1 ; j < 4 ; j++)
			tempA -= (lhs.get(j) * matrices[i][j]);
		
		result.set(i,tempA);
	}
	
	return result;
}

OneIndex TwoIndex::contractFirstOnRight(const OneIndex& rhs) const
{
	OneIndex result;
	
	for (int i=0 ; i < 4 ; i++)
	{
		FourMat tempA = (matrices[0][i] * rhs.get(0));
		
		for ( int j=1 ; j < 4 ; j++)
			tempA -= (matrices[j][i] * rhs.get(j) );
		
		result.set(i,tempA);
	}
	
	return result;
}

OneIndex TwoIndex::contractSecondOnRight(const OneIndex& rhs) const
{
  OneIndex result;
  
  for (int i=0 ; i < 4 ; i++)
    {
      FourMat tempA = (matrices[i][0] * rhs.get(0));
      
      for ( int j=1 ; j < 4 ; j++)
	tempA -= (matrices[i][j] * rhs.get(j) );
      
      result.set(i,tempA);
    }
	
  return result;
}

TwoIndex TwoIndex::contractFirstsOnLeft(const TwoIndex& lhs) const
{
  TwoIndex result;
  
  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      {
	FourMat temp = lhs.get(0,i) * matrices[0][j];
	
	for (int k=1; k<4; k++)
	  temp -= lhs.get(k,i)*matrices[k][j];
	
	result.set(i,j,temp);
      }
  
  return result;
}

TwoIndex TwoIndex::contractFirstsOnRight(const TwoIndex& rhs) const
{
  TwoIndex result;
  
  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      {
	FourMat temp = matrices[0][i] * rhs.get(0,j);
	
	for (int k=1; k<4; k++)
	  temp -= matrices[k][i] * rhs.get(k,j);
	
	result.set(i,j,temp);
      }
  
  return result;
}

TwoIndex TwoIndex::contractSecondFirstOnRight(const TwoIndex& rhs) const
{
  TwoIndex result;
  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      {
	FourMat temp = matrices[i][0] * rhs.get(0,j);
	for (int k=1 ; k<4 ; k++)
	  temp -= matrices[i][k] * rhs.get(k,j);
	result.set(i,j,temp);
      }
  return result;
}

TwoTensor TwoIndex::contractSecondFirstOnRightTr(const TwoIndex& rhs) const
{
  TwoTensor result;
  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      {
	std::complex<double> temp = matrices[i][0].multr(rhs.get(0,j));
	for (int k=1 ; k<4 ; k++)
	  temp -= matrices[i][k].multr( rhs.get(k,j));
	result.set(i,j,temp);
      }
  return result;


}



TwoIndex TwoIndex::leftMultiply(const FourMat& lhs) const
{
	TwoIndex result;
	
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			result.set(i,j,lhs * matrices[i][j]);
	
	return result;
}

TwoIndex TwoIndex::rightMultiply(const FourMat& rhs) const
{
	TwoIndex result;
	
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			result.set(i,j,matrices[i][j]*rhs);
	
	return result;
}

TwoTensor TwoIndex::tr() const
{
	TwoTensor result;
	
	for (int i=0;i<4; i++)
		for (int j=0 ; j<4 ; j++)
			result.set(i,j,matrices[i][j].tr());
	
	return result;
}

const FourMat& TwoIndex::get(int fourIndex1, int fourIndex2) const
{
	return matrices[fourIndex1][fourIndex2];
}

std::complex<double> TwoIndex::get(int fourIndex1, int fourIndex2, int matIndex1, int matIndex2) const
{
	return matrices[fourIndex1][fourIndex2].get(matIndex1,matIndex2);
}

void TwoIndex::set(int fourIndex1, int fourIndex2, const FourMat &matrix)
{
	matrices[fourIndex1][fourIndex2] = matrix;
}

void TwoIndex::set(int fourIndex1, int fourIndex2, int matIndex1, int matIndex2, std::complex<double> value)
{
	matrices[fourIndex1][fourIndex2].set(matIndex1, matIndex2,value);
}

void TwoIndex::set(int fourIndex1, int fourIndex2, int matIndex1, int matIndex2, double real, double imag)
{
	matrices[fourIndex1][fourIndex2].set(matIndex1, matIndex2,real,imag);
}

TwoIndex& TwoIndex::operator+=(const TwoIndex& rhs)
{
	for (int i=0; i<4 ; i++)
		for (int j=0 ; j<4 ; j++)
			matrices[i][j]+= rhs.get(i,j);
	
	return *this;
}

TwoIndex TwoIndex::operator+(const TwoIndex& rhs) const
{
	TwoIndex theCopy = *this;
	theCopy+=rhs;
	return theCopy;
}

TwoIndex& TwoIndex::operator-=(const TwoIndex& rhs)
{
	for (int i=0; i<4 ; i++)
		for (int j=0 ; j<4 ; j++)
			matrices[i][j]-= rhs.get(i,j);
	
	return *this;
}

TwoIndex TwoIndex::operator-(const TwoIndex& rhs) const
{
	TwoIndex theCopy = *this;
	theCopy-=rhs;
	return theCopy;
}

TwoIndex& TwoIndex::operator*=(double value)
{
	for (int i=0; i<4 ; i++)
		for (int j=0 ; j<4 ; j++)
			matrices[i][j]*= value;
	
	return *this;
}

TwoIndex& TwoIndex::operator*=(std::complex<double> value)
{
	for (int i=0; i<4 ; i++)
		for (int j=0 ; j<4 ; j++)
			matrices[i][j]*= value;
	
	return *this;
}

bool TwoIndex::operator==(const TwoIndex& rhs) const
{
	bool result = true;
	
	for (int i=0; i<4 ; i++)
		for (int j=0 ; j<4 ; j++)
			result &= (matrices[i][j] == rhs.get(i,j));
	
	return result;
}

/********************************************************************
Three Tensor
 *******************************************************************/

ThreeTensor::ThreeTensor()
{
  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      for (int k=0; k<4 ; k++)
	{
	  real[i][j][k] = 0.;
	  imag[i][j][k] = 0.;
	}
}

ThreeTensor::ThreeTensor(const ThreeTensor& rhs)
{
  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      for (int k=0; k<4 ; k++)
	{
	  real[i][j][k] = rhs.getReal(i,j,k);
	  imag[i][j][k] = rhs.getImag(i,j,k);
	}
}

ThreeTensor::~ThreeTensor()
{
}

ThreeTensor& ThreeTensor::operator=(const ThreeTensor &rhs)
{
  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      for (int k=0; k<4 ; k++)
	{
	  real[i][j][k] = rhs.getReal(i,j,k);
	  imag[i][j][k] = rhs.getImag(i,j,k);
	}

  return *this;
}

std::complex<double> ThreeTensor::contract123(const ThreeTensor& rhs)
{
  double realResult = 0.;
  double imagResult = 0.;

  for (int i=0;i<4;i++)
    for (int j=0; j<4; j++)
      for (int k=0; k<4 ; k++)
	{
	  double sign = ((i==0)?1.:-1.)*((j==0)?1.:-1.)*((k==0)?1.:-1.);
	  realResult += (sign * (real[i][j][k] * rhs.getReal(i,j,k) - imag[i][j][k] * rhs.getImag(i,j,k)));
	  imagResult += (sign * (imag[i][j][k] * rhs.getReal(i,j,k) + real[i][j][k] * rhs.getImag(i,j,k)));
	}

  return std::complex<double>(realResult,imagResult);
}

std::complex<double> ThreeTensor::get(int ind1, int ind2, int ind3) const
{
  return std::complex<double>(real[ind1][ind2][ind3],imag[ind1][ind2][ind3]);
}

double ThreeTensor::getReal(int ind1, int ind2, int ind3) const
{
  return real[ind1][ind2][ind3];
}

double ThreeTensor::getImag(int ind1, int ind2, int ind3) const
{
  return imag[ind1][ind2][ind3];
}

void ThreeTensor::set(int ind1, int ind2, int ind3, std::complex<double> value)
{
  real[ind1][ind2][ind3] = std::real(value);
  imag[ind1][ind2][ind3] = std::imag(value);
}

void ThreeTensor::set(int ind1, int ind2, int ind3, double re, double im)
{
  real[ind1][ind2][ind3] = re;
  imag[ind1][ind2][ind3] = im;
}

ThreeTensor& ThreeTensor::operator+=(const ThreeTensor& rhs)
{
  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      for (int k=0; k<4 ; k++)
	{
	  real[i][j][k]+=rhs.getReal(i,j,k);
	  imag[i][j][k]+=rhs.getImag(i,j,k);
	}
  return *this;
}

ThreeTensor ThreeTensor::operator+(const ThreeTensor& rhs) const
{
  ThreeTensor sum = *this;
  sum += rhs;
  return sum;
}

ThreeTensor& ThreeTensor::operator-=(const ThreeTensor& rhs)
{
  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      for (int k=0; k<4 ; k++)
	{
	  real[i][j][k]+=rhs.getReal(i,j,k);
	  imag[i][j][k]+=rhs.getImag(i,j,k);
	}
	
  return *this;
}

ThreeTensor ThreeTensor::operator-(const ThreeTensor& rhs) const
{
  ThreeTensor sum = *this;
  sum -= rhs;
  return sum;
}

ThreeTensor& ThreeTensor::operator*=(double value)
{
  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      for (int k=0; k<4 ; k++)
	{
	  real[i][j][k] *= value;
	  imag[i][j][k] *= value;
	}
  return *this;
}

ThreeTensor& ThreeTensor::operator*=(std::complex<double> value)
{
  double re = std::real(value);
  double im = std::imag(value);

  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      for (int k=0; k<4 ; k++)
	{
	  double origRe = real[i][j][k];
	  real[i][j][k] = real[i][j][k]*re - imag[i][j][k]*im;
	  imag[i][j][k] = origRe*im + imag[i][j][k]*re;
	}
  return *this;
}

bool ThreeTensor::operator==(const ThreeTensor& rhs) const
{
  bool result = true;
  
  for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
      for (int k=0; k<4 ; k++)
	{
	  result &= (real[i][j][k]==rhs.getReal(i,j,k));
	  result &= (imag[i][j][k]==rhs.getImag(i,j,k));
	}

  return result;
}

/********************************************************************
Three Index
 *******************************************************************/

ThreeIndex::ThreeIndex()
{	
}

ThreeIndex::ThreeIndex(const ThreeIndex& rhs)
{
	for (int i=0;i<4;i++)
		for (int j=0;j<4;j++)
			for (int k=0; k<4; k++)
				matrices[i][j][k]=rhs.get(i,j,k);
}

ThreeIndex::~ThreeIndex()
{
	
}

ThreeIndex& ThreeIndex::operator=(const ThreeIndex &rhs)
{
	for (int i=0;i<4;i++)
		for (int j=0;j<4;j++)
			for (int k=0; k<4; k++)
				matrices[i][j][k]=rhs.get(i,j,k);
	
	return *this;
}


TwoIndex ThreeIndex::contractFirst(const TLorentzVector& vec) const
{
  TwoIndex result;
  
  for (int i=0;i<4;i++)
    for (int j=0;j<4;j++)
      {
	FourMat tempA = matrices[0][i][j];
	tempA *= vec[3];
	
	for (int k=1;k<4 ; k++)
	  {
	    FourMat tempB = matrices[k][i][j];
	    tempB*=vec[k-1];
	    tempA-=tempB;
	  }
	
	result.set(i,j,tempA);
      }
  return result;
}

TwoIndex ThreeIndex::contractSecond(const TLorentzVector& vec) const
{
  TwoIndex result;
  
  for (int i=0;i<4;i++)
    for (int j=0;j<4;j++)
      {
	FourMat tempA = matrices[i][0][j];
	tempA *= vec[3];
	
	for (int k=1;k<4 ; k++)
	  {
	    FourMat tempB = matrices[i][k][j];
	    tempB*=vec[k-1];
	    tempA-=tempB;
	  }
	
	result.set(i,j,tempA);
      }
  return result;
}

TwoIndex ThreeIndex::contractThird(const TLorentzVector& vec) const
{
  TwoIndex result;
  
  for (int i=0;i<4;i++)
    for (int j=0;j<4;j++)
      {
	FourMat tempA = matrices[i][j][0];
	tempA *= vec[3];
	
	for (int k=1;k<4 ; k++)
	  {
	    FourMat tempB = matrices[i][j][k];
	    tempB*=vec[k-1];
	    tempA-=tempB;
	  }
	
	result.set(i,j,tempA);
      }
  return result;
}

TwoIndex ThreeIndex::contractFirstOnLeft(const OneIndex& lhs) const
{
	TwoIndex result;

	for (int i=0;i<4;i++)
		for (int j=0;j<4;j++)
		{
			FourMat tempA = lhs.get(0) * matrices[0][i][j];
			
			for (int k=1;k<4 ; k++)
			{
				FourMat tempB = lhs.get(k)*matrices[k][i][j];
				tempA-=tempB;
			}
			
			result.set(i,j,tempA);
		}
	return result;
}

TwoIndex ThreeIndex::contractSecondOnLeft(const OneIndex& lhs) const
{
	TwoIndex result;
	
	for (int i=0;i<4;i++)
		for (int j=0;j<4;j++)
		{
			FourMat tempA = lhs.get(0) * matrices[i][0][j];
			
			for (int k=1;k<4 ; k++)
			{
				FourMat tempB = lhs.get(k)*matrices[i][k][j];
				tempA-=tempB;
			}
			
			result.set(i,j,tempA);
		}
	return result;
}

TwoIndex ThreeIndex::contractThirdOnLeft(const OneIndex& lhs) const
{
	TwoIndex result;
	
	for (int i=0;i<4;i++)
		for (int j=0;j<4;j++)
		{
			FourMat tempA = lhs.get(0) * matrices[i][j][0];
			
			for (int k=1;k<4 ; k++)
			{
				FourMat tempB = lhs.get(k)*matrices[i][j][k];
				tempA-=tempB;
			}
			
			result.set(i,j,tempA);
		}
	return result;
}

TwoIndex ThreeIndex::contractFirstOnRight(const OneIndex& rhs) const
{
	TwoIndex result;
	
	for (int i=0;i<4;i++)
		for (int j=0;j<4;j++)
		{
			FourMat tempA = matrices[0][i][j] * rhs.get(0);
			
			for (int k=1;k<4 ; k++)
			{
				FourMat tempB = matrices[k][i][j] * rhs.get(k);
				tempA-=tempB;
			}
			
			result.set(i,j,tempA);
		}
	return result;
}

TwoIndex ThreeIndex::contractSecondOnRight(const OneIndex& rhs) const
{
	TwoIndex result;
	
	for (int i=0;i<4;i++)
		for (int j=0;j<4;j++)
		{
			FourMat tempA = matrices[i][0][j] * rhs.get(0);
			
			for (int k=1;k<4 ; k++)
			{
				FourMat tempB = matrices[i][k][j] * rhs.get(k);
				tempA-=tempB;
			}
			
			result.set(i,j,tempA);
		}
	return result;
}

TwoIndex ThreeIndex::contractThirdOnRight(const OneIndex& rhs) const
{
	TwoIndex result;
	
	for (int i=0;i<4;i++)
		for (int j=0;j<4;j++)
		{
			FourMat tempA = matrices[i][j][0] * rhs.get(0);
			
			for (int k=1;k<4 ; k++)
			{
				FourMat tempB = matrices[i][j][k] * rhs.get(k);
				tempA-=tempB;
			}
			
			result.set(i,j,tempA);
		}
	return result;
}

ThreeTensor ThreeIndex::tr() const
{
	ThreeTensor result;
	
	for (int i=0;i<4;i++)
		for (int j=0;j<4;j++)
			for (int k=0;k<4;k++)
				result.set(i,j,k,matrices[i][j][k].tr());
	
	return result;
}

const FourMat& ThreeIndex::get(int fourIndex1, int fourIndex2, int fourIndex3) const
{
	return matrices[fourIndex1][fourIndex2][fourIndex3];
}

std::complex<double> ThreeIndex::get(int fourIndex1, int fourIndex2, int fourIndex3, int matIndex1, int matIndex2) const
{
	return matrices[fourIndex1][fourIndex2][fourIndex3].get(matIndex1,matIndex2);
}

void ThreeIndex::set(int fourIndex1, int fourIndex2, int fourIndex3, const FourMat &matrix)
{
	matrices[fourIndex1][fourIndex2][fourIndex3] = matrix;
}

void ThreeIndex::set(int fourIndex1, int fourIndex2, int fourIndex3, int matIndex1, int matIndex2, std::complex<double> value)
{
	matrices[fourIndex1][fourIndex2][fourIndex3].set(matIndex1,matIndex2,value);
}

void ThreeIndex::set(int fourIndex1, int fourIndex2, int fourIndex3, int matIndex1, int matIndex2, double real, double imag)
{
	matrices[fourIndex1][fourIndex2][fourIndex3].set(matIndex1,matIndex2,real,imag);
}

ThreeIndex& ThreeIndex::operator+=(const ThreeIndex& rhs)
{
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			for (int k=0; k<4; k++)
				matrices[i][j][k] += rhs.get(i,j,k);
	
	return *this;
}

ThreeIndex ThreeIndex::operator+(const ThreeIndex& rhs) const
{
	ThreeIndex sum = *this;
	sum += rhs;
	return sum;
}

ThreeIndex& ThreeIndex::operator-=(const ThreeIndex& rhs)
{
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			for (int k=0; k<4; k++)
				matrices[i][j][k] -= rhs.get(i,j,k);
	
	return *this;
}

ThreeIndex ThreeIndex::operator-(const ThreeIndex& rhs) const
{
	ThreeIndex sum = *this;
	sum -= rhs;
	return sum;
}

ThreeIndex& ThreeIndex::operator*=(double value)
{
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			for (int k=0; k<4; k++)
				matrices[i][j][k] *= value;
	
	return *this;
}

ThreeIndex& ThreeIndex::operator*=(std::complex<double> value)
{
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			for (int k=0; k<4; k++)
				matrices[i][j][k] *= value;
	
	return *this;
}

bool ThreeIndex::operator==(const ThreeIndex& rhs) const
{
	bool result = true;
	
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			for (int k=0; k<4; k++)
				result &= (matrices[i][j][k] == rhs.get(i,j,k));
	
	return result;	
}
