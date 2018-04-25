//
// Copyright (c) 1994-98 by	Michael O. Distler,
//				Institut fuer Kernphysik,
//				Johannes Gutenberg-Universitaet Mainz
//
// All rights reserved.
//
// $Header: /tmp/cvsroot/Cola/Cindy/CindyExtract.cc,v 4.16 2003-08-05 09:36:29 distler Exp $
//

static const char copyright[] =
 "@(#)Copyright (c) 1994-98 by Michael O. Distler, Institut fuer Kernphysik,\
 Johannes Gutenberg-Universitaet Mainz. All rights reserved.";

static const char rcsid[] =
 "@(#)$Id: CindyExtract.cc 2640 2015-04-01 14:03:00Z aqua $";

#define _DEFAULT_SOURCE 1
#define _POSIX_C_SOURCE 2
#define _XOPEN_SOURCE_EXTENDED
#include "CindyStream.h"
#include <cstring>
#include <unistd.h>

const int zip	= 0x1;
const int tar	= 0x2;
const int gz	= 0x3;

void
extractHelp(char *progname)
{
    std::cerr << std::endl << progname << ':' << std::endl;
    std::cerr << '\t' << "cindy++ extract program" << std::endl;
    std::cerr << " -f <file>\t" << "input file. default: STDIN" << std::endl;
    std::cerr << " -g\t\t"      << "input is gnutar file." << std::endl;
    std::cerr << " -o <file>\t" << "output file. default: none" << std::endl;
    std::cerr << " -r [#,#]\t"  << "global range." << std::endl;
    std::cerr << " -r {#,#}\t"  << "event number range." << std::endl;
    std::cerr << " -z\t\t"      << "uncompress input."
	      << std::endl << std::endl;
}

class Range {
  int Min, Max;
public:
  Range(int min, int max) { Min = min; Max = max; }
  int operator () (int test) { return ((Min<=test)&&(test<=Max)); }
};

int main(int argc, char *argv[])
{
  Range *global = NULL;
  Range *range = NULL;
  int opt, flags=0;
  CindyInBase *in = NULL;
  CindyOutStream *out = NULL;
  struct behdr *header;
  char *input = "-";
  char *program = argv[0];

  while (EOF != (opt = getopt(argc, argv, "f:go:r:zh?"))) {
    switch (opt) {
    case 'f':
      input = optarg;
      break;
    case 'g':
      flags |= tar;
      break;
    case 'r':
      long min, max;
      if (2 == sscanf(optarg, "[%ld,%ld]", &min, &max)) {
	global = new Range(min, max);
	break;
      }
      if (2 == sscanf(optarg, "{%ld,%ld}", &min, &max)) {
	range = new Range(min, max);
	break;
      }
      std::cerr << *argv << ": wrong range" << std::endl;
      return 0;
      break;
    case 'o':
      out = new CindyOutStream(optarg, NULL);
      break;
    case 'z':
      flags |= zip;
      break;
    default:
      extractHelp(program);
      return 0;
    }
  }

  if (index(input, '@')) {
    in = NULL;
    std::cerr << "Pipes are disabled" << std::endl;
  } else if (index(input, ':')) {
    in = NULL;
    std::cerr << "TCPIP connections are disabled" << std::endl;
  } else if (input == index(input, '-'))
    switch (flags) {
    case zip:
      in = new CindyInProc("gzip -cd"); break;
      break;
    case tar:
      in = new CindyInProc("tar xfO -"); break;
      break;
    case gz:
      in = new CindyInProc("tar xzfO -"); break;
      break;
    default:
      in = new CindyInStd(); break;
    }
  else
    switch (flags) {
    case zip:
      in = new CindyInProc("gzip -cd ",input); break;
      break;
    case tar:
      in = new CindyInProc("tar xfO ",input); break;
      break;
    case gz:
      in = new CindyInProc("tar xzfO ",input); break;
      break;
    default:
      in = new CindyInFile(input); break;
    }

  if (!in) return -1;
  if (!out) return -1;

  long count = 0;
  while ( (header = in->nextHeader()) ) {
    if ( !(header->b_flags & BEtype) ) count++;
    if ( (header->b_flags & BEtype) ||
	(global && (*global)(count)) || (range && (*range)(header->b_events)) )
      out->putData(header,(Uint2 *)(header+1));
  }

  return 0;
}
