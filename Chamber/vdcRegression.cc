//
// Created 1993 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Chamber/vdcRegression.cc,v 2.14 2002-11-08 20:27:26 distler Exp $
//
// Implementation of class VdcRegression
//

static char rcsid[] =
 "@(#)$Id: vdcRegression.cc 2560 2014-05-21 14:51:35Z distler $";

#include "vdcRegression.h"
#include <iostream>

VdcRegression::VdcRegression()
{
  reset();
}


VdcRegression::~VdcRegression()
{
  if (rcsid[0] == '@') return;
}


void
VdcRegression::add(double x)
{
  addw(x, 1.0);
}

void
VdcRegression::addw(double x, double w)
{
  double invw2 = 1.0/(w*w);

  N++; sumN += invw2; sumX += x*invw2; sumXX += x*x*invw2;
}


void
VdcRegression::add(double x, double y)
{
  addw(x, y, 1.0);
}

void
VdcRegression::addw(double x, double y, double w)
{
  double invw2 = 1.0/(w*w);

  N++; sumN += invw2;
  sumX += x*invw2; sumY  += y*invw2;
  sumXX += x*x*invw2; sumXY += x*y*invw2; sumYY += y*y*invw2;
  //std::cout << N << '\t' << x << '\t' << y << '\t' << w << std::endl;
}


void
VdcRegression::remove(double x)
{
  removew(x, 1.0);
}

void
VdcRegression::removew(double x, double w)
{
  double invw2 = 1.0/(w*w);

  N--; sumN -= invw2; sumX -= x*invw2; sumXX -= x*x*invw2;
}


void
VdcRegression::remove(double x, double y)
{
  removew(x, y, 1.0);
}

void
VdcRegression::removew(double x, double y, double w)
{
  double invw2 = 1.0/(w*w);

  N--; sumN -= invw2;
  sumX -= x*invw2; sumY  -= y*invw2;
  sumXX -= x*x*invw2; sumXY -= x*y*invw2; sumYY -= y*y*invw2;
}


void
VdcRegression::reset(void)
{
  N = 0;
  sumN = sumX = sumY = sumXX = sumXY = sumYY = 0.0;
}


double
VdcRegression::x0(void)
{
  return x_;
}

double
VdcRegression::dx0(void)
{
  return dx_;
}

double
VdcRegression::y0(void)
{
  return y_;
}

double
VdcRegression::dy0(void)
{
  return dy_;
}

double
VdcRegression::sl0(void)
{
  return sl_;
}

double
VdcRegression::dsl0(void)
{
  return dsl_;
}

double
VdcRegression::chi(void)
{
  if (chi2 == FLT_MAX)
    return FLT_MAX;

  return sqrt(chi2);
}

double
VdcRegression::chisqr(void)
{
  return chi2;
}

void
VdcRegression::print(char *com)
{
  std::cout << com << "\tN: " << N << " (" << sumN
	    << ")\tX: " << sumX << "\tY: " << sumY << std::endl
	    << "\tXX: " << sumXX << "\tXY: "
	    << sumXY << "\tYY: " << sumYY << std::endl;
  std::cout << "\tx0: " << x0() << " +/- " << dx0() << std::endl
	    << "\tsl: " << sl0() << " +/- " << dsl0()
	    << std::endl << std::endl;
}

void
VdcRegression::calc()
{
  double a0, a1, a2;
  //double dy_2, dsl_2, cov_y_sl;

  a0   = sumXX * sumY - sumX * sumXY;
  a1   = sumXY * sumN - sumX * sumY;
  a2   = sumXX * sumN - sumX * sumX;

  if (a2 == 0.0) {
    y_ = dy_ = sl_ = dsl_ = chi2 = FLT_MAX;
  } else {
    y_ = a0 / a2;
    sl_ = a1 / a2;
    if (N <= 2) {
      dy_ = dsl_ = chi2 = FLT_MAX;
      //dy_2 = dsl_2 = cov_y_sl = FLT_MAX;
    } else {
      chi2 = fabs((a2 * sumYY - a0 * sumY - a1 * sumXY) / (a2 * (N - 2)));
      dy_  = sqrt(chi2 * sumXX / a2);
      dsl_ = sqrt(chi2 * sumN / a2);
      //dy_2     = sumXX / a2;
      //dsl_2    =  sumN / a2;
      //cov_y_sl = -sumX / a2;
    }
  }

  if (a1 == 0.0) {
    x_ = dx_ = FLT_MAX;
  } else {
    x_ = - a0 / a1;
    if (N <= 2) {
      dx_ = FLT_MAX;
    } else {
      //dx_ = sqrt(chi2 * a2 * (a1*a1 * sumXX + a0*a0 * sumN)) / (a1*a1);
      dx_ = a2 / (a1 * a1) * sqrt(chi2 * (a1 * sumXY + a0 * sumY));
      //dx_ = 0.1;
    }
  }
}

VdcRegression
VdcRegression::operator+(const VdcRegression & b)
{
  VdcRegression sum;

  sum.N = N + b.N;
  sum.sumN = sumN + b.sumN;
  sum.sumX = sumX + b.sumX;
  sum.sumY = sumY + b.sumY;
  sum.sumXX = sumXX + b.sumXX;
  sum.sumXY = sumXY + b.sumXY;
  sum.sumYY = sumYY + b.sumYY;

  return sum;
}

VdcRegression
VdcRegression::operator+(const double offset)
{
  VdcRegression sum;

  sum.N = N;
  sum.sumN = sumN;
  sum.sumX = sumX;
  sum.sumY = sumY + sumN * offset;
  sum.sumXX = sumXX;
  sum.sumXY = sumXY + sumX * offset;
  sum.sumYY = sumYY + (2 * sumY + sumN * offset) * offset;

  return sum;
}









