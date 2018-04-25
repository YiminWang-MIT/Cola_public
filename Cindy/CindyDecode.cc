//
// Created 1993-95 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Cindy/CindyDecode.cc,v 4.9 2002-12-12 12:24:41 distler Exp $
//
// Main Part of Cindy analysis program
//

#include "Cindy++.h"
#include <iostream>

int
main(int argc, char *argv[])
{
  CindyTree ct(0);
  if (ct.decodeGetOpt(argc,argv)) return -1;
  if (ct.nextArchiveUnpackAndCompile()) return -1;

  while (!ct.decodeNextEvent()) {}

  return 0;
}
