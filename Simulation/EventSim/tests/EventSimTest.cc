//
// Institut für Kernphysik, Universität Mainz	tel. +49 6131 39-25802
// 55099 Mainz, Germany				fax  +49 6131 39-22964
//
//

#include <iostream>
#if defined(__GNU_LIBRARY__)
#  include <getopt.h>
#endif

#include "Aqua_abcn.h"
#include "../EventSimReader.h"

int printHelp(const char *progname)
{
  std::cerr << progname << " - EventSim test program" << std::endl;
  std::cerr << "Usage:" << std::endl;
  std::cerr << "\t-h,       --help         this help" << std::endl;
  std::cerr << "\t-v        --version      print version info" << std::endl;
  return 0;
}

int printVersion(const char *progname)
{
  std::cerr << progname << " - $Revision: 2216 $" << std::endl;
  return 0;
}

int main(int argc, char *argv[])
{
  int opt;
  char *progname = argv[0];
  char *dataFile = NULL;
  char *options = "vh?";

#if defined(__GNU_LIBRARY__)
  struct option lo[] = {
    {"help",      0, NULL, 'h'},
    {"version",   0, NULL, 'v'},
    {NULL, 0, NULL, 0}};
  while ((opt = getopt_long(argc, argv, options, lo, NULL)) != EOF) {
#else
  while ((opt = getopt(argc, argv, options)) != EOF) {
#endif
    switch (opt) {
    case 'v':
	printVersion(progname);
	return 0;
    default:
	printHelp(progname);
	return 0;
    }
  }

  // parse input parameter (if any)
  AquaIO *aquaIO = NULL;
  // main loop
  struct abcn *abcn = NULL;
  AquaReader *reader = NULL;
  reader = (AquaReader *)new EventSimReader(aquaIO);
  AquaTree *tree = NULL;
  tree = (AquaTree *)new Aqua_abcn(abcn = new struct abcn);

  reader->initTree(tree);

  int timeout = 0;
  while (reader && tree && ((timeout = reader->nextEvent(tree))>=0)) {
    if (timeout) { usleep(10000); continue; }
    tree->decode();
  }

  delete reader;
  delete tree;
  delete abcn;

  return 0;
}
