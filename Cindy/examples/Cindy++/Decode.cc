//
// Decode.cc: Das Cindy++ decode program
//

#include "ana++/Cindy++.h"
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
