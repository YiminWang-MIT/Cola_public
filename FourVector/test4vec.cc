//
// Created 1994 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/FourVector/test4vec.cc,v 3.4 2006-06-27 12:33:56 distler Exp $
//
// Test Programm fuer die FourVector Library
//

#include "FourVector.h"

int
main()
{
//  FourVector v(8,3,4,5);
  float x[4] = { 8, 3, 4, 5 };
  FourVector v(x);
  FourVector v1, v2;

  v1.initPolar(v.energy(),v.momentum(),v.theta(),v.phi());
  v2.initPolar(v.energy(),v.momentum(),v.varTheta(),v.varPhi());
  v.print("v0 :"); v1.print("   :"); v2.print("   :");
  std::cout << v.theta() << '\t' << v.phi() << std::endl;
  std::cout << v.varTheta() << '\t' << v.varPhi() << std::endl;

  v.rot_phi(M_PI_2); 
  v1.initPolar(v.energy(),v.momentum(),v.theta(),v.phi());
  v2.initPolar(v.energy(),v.momentum(),v.varTheta(),v.varPhi());
  v.print("v1 :"); v1.print("   :"); v2.print("   :");
  std::cout << v.theta() << '\t' << v.phi() << std::endl;
  std::cout << v.varTheta() << '\t' << v.varPhi() << std::endl;

  v.rot_phi(M_PI_2); 
  v1.initPolar(v.energy(),v.momentum(),v.theta(),v.phi());
  v2.initPolar(v.energy(),v.momentum(),v.varTheta(),v.varPhi());
  v.print("v2 :"); v1.print("   :"); v2.print("   :");
  std::cout << v.theta() << '\t' << v.phi() << std::endl;
  std::cout << v.varTheta() << '\t' << v.varPhi() << std::endl;

  v.rot_phi(M_PI_2); 
  v1.initPolar(v.energy(),v.momentum(),v.theta(),v.phi());
  v2.initPolar(v.energy(),v.momentum(),v.varTheta(),v.varPhi());
  v.print("v3 :"); v1.print("   :"); v2.print("   :");
  std::cout << v.theta() << '\t' << v.phi() << std::endl;
  std::cout << v.varTheta() << '\t' << v.varPhi() << std::endl;

  v.rot_phi(M_PI_2); v.rot_theta(M_PI_2); 
  v1.initPolar(v.energy(),v.momentum(),v.theta(),v.phi());
  v2.initPolar(v.energy(),v.momentum(),v.varTheta(),v.varPhi());
  v.print("v4 :"); v1.print("   :"); v2.print("   :");
  std::cout << v.theta() << '\t' << v.phi() << std::endl;
  std::cout << v.varTheta() << '\t' << v.varPhi() << std::endl;

  v.rot_phi(M_PI_2); 
  v1.initPolar(v.energy(),v.momentum(),v.theta(),v.phi());
  v2.initPolar(v.energy(),v.momentum(),v.varTheta(),v.varPhi());
  v.print("v5 :"); v1.print("   :"); v2.print("   :");
  std::cout << v.theta() << '\t' << v.phi() << std::endl;
  std::cout << v.varTheta() << '\t' << v.varPhi() << std::endl;

  v.rot_phi(M_PI_2); 
  v1.initPolar(v.energy(),v.momentum(),v.theta(),v.phi());
  v2.initPolar(v.energy(),v.momentum(),v.varTheta(),v.varPhi());
  v.print("v6 :"); v1.print("   :"); v2.print("   :");
  std::cout << v.theta() << '\t' << v.phi() << std::endl;
  std::cout << v.varTheta() << '\t' << v.varPhi() << std::endl;

  v.rot_phi(M_PI_2); 
  v1.initPolar(v.energy(),v.momentum(),v.theta(),v.phi());
  v2.initPolar(v.energy(),v.momentum(),v.varTheta(),v.varPhi());
  v.print("v7 :"); v1.print("   :"); v2.print("   :");
  std::cout << v.theta() << '\t' << v.phi() << std::endl;
  std::cout << v.varTheta() << '\t' << v.varPhi() << std::endl << std::endl;

  std::cout << "x :\t" << x[0] << '\t' << x[1] << '\t'
	    << x[2] << '\t' << x[3] << std::endl;
  v.fill(x);
  std::cout << "x :\t" << x[0] << '\t' << x[1] << '\t'
	    << x[2] << '\t' << x[3] << std::endl;

#ifdef NEVER
  std::cout << std::endl << "test particle function" << std::endl;
  const double M_PROTON = 0.93827231; // create 1GeV/c proton
  FourVector proton(energy(M_PROTON, 1.0), 1.0, 0.0, 0.0);
  proton.print("p 90deg");
  proton.rot_phi(0.2); // rotate 11.5 degree out-of-plane
  proton.print("p oop");
  proton.rot_theta(0.5-M_PI_2);
  proton.print("p 29deg"); // rotate to forward angle

  proton = particle(M_PROTON, 1.0, 0.5, 0.2, 0.0, 0.0);
  proton.print("p 29deg"); // rotate to forward angle

  proton = particle(M_PROTON, 1.0, 0.4, 0.1, 0.1, 0.1);
  proton.print("p 29deg"); // rotate to forward angle
#endif /* NEVER */

  return 0;
}
