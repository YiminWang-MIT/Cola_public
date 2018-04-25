//								-*-c++-*-
// Copyright (c) 1995 by	Michael O. Distler,
//				Institut fuer Kernphysik,
//				Johannes Gutenberg-Universitaet Mainz
//
// All rights reserved.
//
// $Header: /tmp/cvsroot/Cola/Cindy/Card.cc,v 4.8 2002-12-12 12:24:41 distler Exp $
//

#include <iostream>
#include "CindyCard.h"

int
main()
{
  CindyCard card("cindy.card");

  std::cout << "spec"
	    << '\t' << "angle"
	    << '\t' << "refmom"
	    << '\t' << "peak"
	    << '\t' << "delta"
	    << '\t' << "min"
	    << '\t' << "max"
	    << '\t' << "flag"
	    << '\t' << "bins" << std::endl;
  std::cout << "  A"
	    << '\t' << card.specA.angle
	    << '\t' << card.specA.refMom
	    << '\t' << card.specA.peak[0]
	    << '\t' << card.specA.delta[0]
	    << '\t' << card.specA.min[0]
	    << '\t' << card.specA.max[0]
	    << '\t' << card.specA.flag[0]
	    << '\t' << card.specA.bins[0] << std::endl;
  std::cout << "  B"
	    << '\t' << card.specB.angle
	    << '\t' << card.specB.refMom
	    << '\t' << card.specB.peak[1]
	    << '\t' << card.specB.delta[1]
	    << '\t' << card.specB.min[1]
	    << '\t' << card.specB.max[1]
	    << '\t' << card.specB.flag[1]
	    << '\t' << card.specB.bins[1] << std::endl;
  std::cout << "  C"
	    << '\t' << card.specC.angle
	    << '\t' << card.specC.refMom
	    << '\t' << card.specC.peak[2]
	    << '\t' << card.specC.delta[2]
	    << '\t' << card.specC.min[2]
	    << '\t' << card.specC.max[2]
	    << '\t' << card.specC.flag[2]
	    << '\t' << card.specC.bins[2] << std::endl;

  return 0;
}
