//
// Created 1993-95 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Cindy/proctest.cc,v 4.8 2002-12-12 12:24:41 distler Exp $
//
// Testprogramm fuer die procbuf - class
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <iostream>
#ifdef HAVE_ISTREAM
#include <istream>
#endif
#ifdef HAVE_OSTREAM
#include <ostream>
#endif
#include <fstream>
#include <iosfwd>
// #include <procbuf.h>

const int MAXBUF = 32768;

int
main(int argc, char *argv[])
{
  char *progname = argv[0];
  char *input = argv[1];
  char *output = argv[2];
  std::ifstream *in;
  procbuf *inz;
  std::ofstream *out;
  procbuf *outz;
  char buf[MAXBUF];
  int pos = 0;
  int haveread = MAXBUF;

  if (argc != 3) {
    std::cerr << "Usage: " << progname << " infile outfile" << std::endl;
    return 0;
  }

  String incom = String("/usr/local/gnu/bin/gzip -cd ") + String(input);
  inz = new procbuf(incom.chars(),ios::in);
  in = new ifstream(inz->fd());

  String outcom = String("/usr/local/gnu/bin/gzip > ") + String(output);
  outz = new procbuf(outcom.chars(),ios::out);
  out  = new ofstream(outz->fd());

  while (haveread > 0) {

    in->read(buf,MAXBUF);
    haveread = in->gcount();
    pos += haveread;
    std::cout << "read: " << haveread << "\tpos: " << pos << std::endl;

    if (haveread > 0) {
      out->write(buf,haveread);
    }

  }

  return 0;
}
