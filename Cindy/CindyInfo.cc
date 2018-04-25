//
// Copyright (c) 1994-98 by	Michael O. Distler,
//				Institut fuer Kernphysik,
//				Johannes Gutenberg-Universitaet Mainz
//
// All rights reserved.
//
// $Header: /tmp/cvsroot/Cola/Cindy/CindyInfo.cc,v 4.15 2003-08-05 09:36:29 distler Exp $
//

static const char copyright[] =
 "@(#)Copyright (c) 1994-98 by Michael O. Distler, Institut fuer Kernphysik,\
 Johannes Gutenberg-Universitaet Mainz. All rights reserved.";

static const char rcsid[] =
 "@(#)$Id: CindyInfo.cc 2640 2015-04-01 14:03:00Z aqua $";

#define _DEFAULT_SOURCE 1
#define _POSIX_C_SOURCE 2
#define _XOPEN_SOURCE_EXTENDED
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <unistd.h>
#include "CindyStream.h"

const int zip	= 0x1;
const int tar	= 0x2;
const int gz	= 0x3;

int main(int argc, char *argv[])
{
  int opt, flags=0;
  CindyInBase *in = NULL;
  struct behdr *header;
  char *input = "-";

  while (EOF != (opt = getopt(argc, argv, "f:gzh?"))) {
    switch (opt) {
    case 'f':
      input = optarg;
      break;
    case 'g':
      flags |= tar;
      break;
    case 'z':
      flags |= zip;
      break;
    default:
      std::cerr << *argv << ": Cindy++ Info" << std::endl;
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

  while ( (header=in->nextHeader()) ) {
    std::cout << "len:\t" << std::dec << header->b_len;
    std::cout << "\tformat:\t" << header->b_format;
    std::cout << "\tcount:\t" << header->b_events;
    std::cout << "\tflags:\t" << std::hex << header->b_flags << std::endl;
  }

  return 0;
}
