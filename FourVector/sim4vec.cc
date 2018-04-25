//
// Created 1994 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/FourVector/sim4vec.cc,v 3.3 2002-11-26 18:11:50 distler Exp $
//
// Test Programm fuer die FourVector Library
//

#include "FourVector.h"
#include "Rand.h"
#include <iostream>
#include <time.h>

int
main()
{
  ACG gen(time(0),20);

  SimFourVector sv(0.001, 0.005, &gen);
  FourVector v;

  v.initPolar(0.1, momentum(0.1, 0.000511), M_PI_4, M_PI_4);
  v.print("v  :");

  sv.init(v);
  sv.print("sv1:");

  sv.init(v);
  sv.print("sv2:");

  sv.init(v);
  sv.print("sv3:");

  sv.initPolar(0.1, momentum(0.1, 0.000511), M_PI_4, M_PI_4);
  sv.print("sv4:");

  SimulFourVector s4v(&gen);

  s4v.init(v, 0.001, 0.005);
  s4v.print("sv1:");

  s4v.init(v, 0.001, 0.005);
  s4v.print("sv2:");

  s4v.init(v, 0.001, 0.005);
  s4v.print("sv3:");

  s4v.initPolar(0.1, momentum(0.1, 0.000511), M_PI_4, M_PI_4, 0.001, 0.005);
  s4v.print("sv4:");

  return 0;
}
