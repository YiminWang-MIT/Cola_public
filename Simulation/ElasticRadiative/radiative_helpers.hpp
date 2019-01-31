#ifndef __RADIATIVE_HELPERS__
#define __RADIATIVE_HELPERS__

#include <complex>
#include "TLorentzVector.h"

/********************************************************************
Elastic Kinematic Variables used in Maximon and Tjon
 *******************************************************************/
class ElasticKinematics
{
public:
  ElasticKinematics(double initLeptonE, double scatLeptonTheta);
  double eps() const;
  double E1() const { return E1_; } // Initial lepton energy
  double theta() const { return theta_; } // Scattered lepton angle
  double p3() const { return p3_; } // Final lepton energy
  double E3() const { return E3_; } // Final lepton energy
  double E4() const { return E4_; } // Final proton energy
  double Q2() const { return Q2_; } // Momentum transfer
  double p4() const { return p4_; } // 3-momentum magnitude of final proton
  double eta() const { return eta_; } // Lab system recoil factor
  double rho() const { return rho_; } // Initial plus final proton 4-momentum
  double x() const { return x_; } // Maximum eta
  double tau() const { return tau_; } // Q^2/4M

private:
  double E1_;
  double theta_;
  double p3_;
  double E3_;
  double p4_;
  double E4_;
  double Q2_;
  double eta_;
  double rho_;
  double x_;
  double tau_;
};

/********************************************************************
Four Matrix Classes
 *******************************************************************/
class FourMat
{
public:
  FourMat();
  FourMat(const FourMat& rhs);
  ~FourMat();
  FourMat& operator=(const FourMat &rhs);

  double real[16];
  double imag[16];

  std::complex<double> tr() const;
  std::complex<double> multr(const FourMat& rhs) const;
  std::complex<double> get(int entry1, int entry2) const;
  void set(int entry1,int entry2, std::complex<double>);
  void set(int entry1,int entry2, double real, double comp);
  FourMat multGammaOnLeft(int ind) const;
  FourMat multGammaOnRight(int ind) const;
  FourMat& operator+=(const FourMat& rhs);
  FourMat operator+(const FourMat& rhs) const;
  FourMat& operator-=(const FourMat& rhs);
  FourMat operator-(const FourMat& rhs) const;
  FourMat& operator*=(double value);
  FourMat& operator*=(std::complex<double> value);
  FourMat operator*(const FourMat& rhs) const;
  bool operator==(const FourMat& rhs) const;
  void print() const;
	
protected:
  //std::complex<double> entries[4][4];	

  
};

class Gamma0 : public FourMat
{
public:
	Gamma0()
	{
		set(0,2,std::complex<double>(1.0,0.0));
		set(1,3,std::complex<double>(1.0,0.0));
		set(2,0,std::complex<double>(1.0,0.0));
		set(3,1,std::complex<double>(1.0,0.0));
	};
	~Gamma0(){};
};

class Gamma1 : public FourMat
{	
public:
	Gamma1()
	{
		set(0,3,std::complex<double>(1.0,0.0));
		set(1,2,std::complex<double>(1.0,0.0));
		set(2,1,std::complex<double>(-1.0,0.0));
		set(3,0,std::complex<double>(-1.0,0.0));
	}
	~Gamma1(){};
};

class Gamma2 : public FourMat
{
public:
	Gamma2()
	{
		set(0,3,std::complex<double>(0.0,-1.0));
		set(1,2,std::complex<double>(0.0,1.0));
		set(2,1,std::complex<double>(0.0,1.0));
		set(3,0,std::complex<double>(0.0,-1.0));
	}
	~Gamma2(){};
};

class Gamma3 : public FourMat
{
public:
	Gamma3()
	{
		set(0,2,std::complex<double>(1.0,0.0));
		set(1,3,std::complex<double>(-1.0,0.0));
		set(2,0,std::complex<double>(-1.0,0.0));
		set(3,1,std::complex<double>(1.0,0.0));
	}
	~Gamma3(){};
};

class Gamma5 : public FourMat
{
public:
	Gamma5()
	{
		set(0,0,std::complex<double>(-1.0,0.0));
		set(1,1,std::complex<double>(-1.0,0.0));
		set(2,2,std::complex<double>(1.0,0.0));
		set(3,3,std::complex<double>(1.0,0.0));
	}
	~Gamma5(){};
};

class IFourMat : public FourMat
{
public:
	IFourMat()
	{
		set(0,0,std::complex<double>(1.0,0.0));
		set(1,1,std::complex<double>(1.0,0.0));
		set(2,2,std::complex<double>(1.0,0.0));
		set(3,3,std::complex<double>(1.0,0.0));
	}
	~IFourMat(){};
};

/********************************************************************
One-Index Object Classes
 *******************************************************************/

class OneIndex
{
public:
  OneIndex();
  OneIndex(const FourMat& entry0,const FourMat& entry1,const FourMat& entry2,const FourMat& entry3);
  OneIndex(const OneIndex& rhs);
  ~OneIndex();
  OneIndex& operator=(const OneIndex &rhs);
  FourMat slash(const TLorentzVector& vec) const;
  OneIndex leftMultiply(const FourMat& lhs) const;
  OneIndex rightMultiply(const FourMat& rhs) const;
  FourMat contract(const OneIndex& rhs) const;
  FourMat contractGammaOnRight() const;
  FourMat contractGammaOnLeft() const;
  std::complex<double> get(int fourIndex, int entry1, int entry2) const;
  const FourMat& get(int fourIndex) const;
  void set(int fourIndex, const FourMat& mat);
  void set(int fourIndex, int entry1, int entry2, std::complex<double> value);
  void set(int fourIndex, int entry1, int entry2, double real, double imag);
  OneIndex& operator+=(const OneIndex& rhs);
  OneIndex operator+(const OneIndex& rhs) const;
  OneIndex& operator-=(const OneIndex& rhs);
  OneIndex operator-(const OneIndex& rhs) const;
  OneIndex& operator*=(double value);
  OneIndex& operator*=(std::complex<double> value);
  bool operator==(const OneIndex& rhs) const;
  
protected:
  FourMat matrices[4];
};

class Gamma : public OneIndex {
public:
  Gamma() : OneIndex (Gamma0(), Gamma1(), Gamma2(), Gamma3()){};
  ~Gamma(){};
  FourMat slash(const TLorentzVector& vec) const;
};

/********************************************************************
Two-Tensor Classes
 *******************************************************************/

class TwoTensor
{
public:
  TwoTensor();
  TwoTensor(const TwoTensor& rhs);
  ~TwoTensor();
  TwoTensor& operator=(const TwoTensor &rhs);
  std::complex<double> selfContract() const;
  std::complex<double> contractInOrder(const TwoTensor& rhs);
  std::complex<double> contractOutOfOrder(const TwoTensor& rhs);
  std::complex<double> get(int ind1, int ind2) const;
  double getRe(int ind1, int ind2) const;
  double getIm(int ind1, int ind2) const;
  void set(int ind1, int ind2, std::complex<double> value);
  void set(int ind1, int ind2, double r, double i);	
  TwoTensor& operator+=(const TwoTensor& rhs);
  TwoTensor operator+(const TwoTensor& rhs) const;
  TwoTensor& operator-=(const TwoTensor& rhs);
  TwoTensor operator-(const TwoTensor& rhs) const;
  TwoTensor& operator*=(double value);
  TwoTensor& operator*=(std::complex<double> value);
  bool operator==(const TwoTensor& rhs) const;

protected:
  //  std::complex<double> entries[4][4];
  double real[16];
  double imag[16];
};

class Metric : public TwoTensor
{
public:
	Metric() : TwoTensor()
	{
		set(0,0,1.0,0.0);
		set(1,1,-1.0,0.0);
		set(2,2,-1.0,0.0);
		set(3,3,-1.0,0.0);
	};
	~Metric(){};
};

/********************************************************************
Two-Index Classes
 *******************************************************************/
class TwoIndex
{
public:
  TwoIndex();
  TwoIndex(const TwoIndex& rhs);
  ~TwoIndex();
  TwoIndex& operator=(const TwoIndex &rhs);
  OneIndex contractFirst(const TLorentzVector& vec) const;
  OneIndex contractSecond(const TLorentzVector& vec) const;
  OneIndex contractFirstOnLeft(const OneIndex& lhs) const;
  OneIndex contractSecondOnLeft(const OneIndex& lhs) const;
  OneIndex contractFirstOnRight(const OneIndex& rhs) const;
  OneIndex contractSecondOnRight(const OneIndex& rhs) const;
  TwoIndex contractFirstsOnLeft(const TwoIndex& lhs) const;	// Contracts lhs.lm ln to mn
  TwoIndex contractFirstsOnRight(const TwoIndex& rhs) const;	// Contracts lm rhs.ln to mn
  TwoIndex contractSecondFirstOnRight(const TwoIndex& rhs) const; // Contracts ml rhs.ln to mn
  TwoTensor contractSecondFirstOnRightTr(const TwoIndex& rhs) const; // Contracts ml rhs.ln to mn, make trace
  TwoIndex leftMultiply(const FourMat& lhs) const;
  TwoIndex rightMultiply(const FourMat& rhs) const;
  TwoTensor tr() const;
  const FourMat& get(int fourIndex1, int fourIndex2) const;
  std::complex<double> get(int fourIndex1, int fourIndex2, int matIndex1, int matIndex2) const;
  void set(int fourIndex1, int fourIndex2, const FourMat &matrix);
  void set(int fourIndex1, int fourIndex2, int matIndex1, int matIndex2, std::complex<double> value);
  void set(int fourIndex1, int fourIndex2, int matIndex1, int matIndex2, double real, double imag);
  TwoIndex& operator+=(const TwoIndex& rhs);
  TwoIndex operator+(const TwoIndex& rhs) const;
  TwoIndex& operator-=(const TwoIndex& rhs);
  TwoIndex operator-(const TwoIndex& rhs) const;
  TwoIndex& operator*=(double value);
  TwoIndex& operator*=(std::complex<double> value);
  bool operator==(const TwoIndex& rhs) const;
  
protected:
  FourMat matrices[4][4];
};

class Sigma : public TwoIndex
{
public:
  Sigma() : TwoIndex()
  {
    for (int i=0; i<4; i++)
      for (int j=0; j<4; j++)
	{
	  matrices[i][j]=(Gamma().get(i) * Gamma().get(j)) - (Gamma().get(j) * Gamma().get(i));
	  matrices[i][j]*=std::complex<double>(0.0,0.5);
	}
  };
  ~Sigma(){};
};

/********************************************************************
Three-Tensor Classes
 *******************************************************************/

class ThreeTensor
{
public:
  ThreeTensor();
  ThreeTensor(const ThreeTensor& rhs);
  ~ThreeTensor();
  ThreeTensor& operator=(const ThreeTensor &rhs);
  std::complex<double> contract123(const ThreeTensor& rhs);
  std::complex<double> get(int ind1, int ind2, int ind3) const;
  double getReal(int ind1, int ind2, int ind3) const;
  double getImag(int ind1, int ind2, int ind3) const;
  void set(int ind1, int ind2, int ind3, std::complex<double> value);
  void set(int ind1, int ind2, int ind3, double re, double im);
  ThreeTensor& operator+=(const ThreeTensor& rhs);
  ThreeTensor operator+(const ThreeTensor& rhs) const;
  ThreeTensor& operator-=(const ThreeTensor& rhs);
  ThreeTensor operator-(const ThreeTensor& rhs) const;
  ThreeTensor& operator*=(double value);
  ThreeTensor& operator*=(std::complex<double> value);
  bool operator==(const ThreeTensor& rhs) const;
  
protected:
  double real[4][4][4];
  double imag[4][4][4];
};

/********************************************************************
Three-Index Classes
 *******************************************************************/

class ThreeIndex
{
public:
  ThreeIndex();
  ThreeIndex(const ThreeIndex& rhs);
  ~ThreeIndex();
  ThreeIndex& operator=(const ThreeIndex &rhs);
  TwoIndex contractFirst(const TLorentzVector& vec) const;
  TwoIndex contractSecond(const TLorentzVector& vec) const;
  TwoIndex contractThird(const TLorentzVector& vec) const;
  TwoIndex contractFirstOnLeft(const OneIndex& lhs) const;
  TwoIndex contractSecondOnLeft(const OneIndex& lhs) const;
  TwoIndex contractThirdOnLeft(const OneIndex& lhs) const;
  TwoIndex contractFirstOnRight(const OneIndex& rhs) const;
  TwoIndex contractSecondOnRight(const OneIndex& rhs) const;
  TwoIndex contractThirdOnRight(const OneIndex& rhs) const;
  ThreeTensor tr() const;
  const FourMat& get(int fourIndex1, int fourIndex2, int fourIndex3) const;
  std::complex<double> get(int fourIndex1, int fourIndex2, int fourIndex3, int matIndex1, int matIndex2) const;
  void set(int fourIndex1, int fourIndex2, int fourIndex3, const FourMat &matrix);
  void set(int fourIndex1, int fourIndex2, int fourIndex3, int matIndex1, int matIndex2, std::complex<double> value);
  void set(int fourIndex1, int fourIndex2, int fourIndex3, int matIndex1, int matIndex2, double real, double imag);
  ThreeIndex& operator+=(const ThreeIndex& rhs);
  ThreeIndex operator+(const ThreeIndex& rhs) const;
  ThreeIndex& operator-=(const ThreeIndex& rhs);
  ThreeIndex operator-(const ThreeIndex& rhs) const;
  ThreeIndex& operator*=(double value);
  ThreeIndex& operator*=(std::complex<double> value);
  bool operator==(const ThreeIndex& rhs) const;
  
protected:
	FourMat matrices[4][4][4];
};

#endif
