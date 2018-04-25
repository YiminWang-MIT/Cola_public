//
// Copyright (c) 1993 by   Michael O. Distler,
//                         Institut fuer Kernphysik,
//                         Johannes Gutenberg-Universitaet Mainz
//
// All rights reserved.
//
// $Header: /tmp/cvsroot/Cola/HBook/ntest.cc,v 1.17 1998-08-30 22:10:24 distler Exp $
//

static const char rcsid[] =
 "@(#)$Id: ntest.cc 2354 2009-11-13 13:58:04Z merkel $";

#include <time.h>
#include "HBook.h"
#include "Rand.h"

const int PAWCsize = 100000;	
struct pawccc {
  float h[PAWCsize];
} pawc_;			// don't change the name 'pawc'

const int CHTsize = 2;
char *cht[CHTsize] = {
  "xxx", "yyy"
};				// use the same string length for
				// all variables. Fill with spaces
				// if necessary

int
main(int argc, char *argv[])
{
  if (argc && argv); // keep compiler happy

  ACG gen(time(0),20);
  Normal normal(50.0,100.0,&gen);

  NTuple pawc(PAWCsize);	// don't delete this line
				// ignore the compiler warning
  NTuple nt((const char *)"NTuple 1",
	    (const char *)"ntuple.dat",
	    CHTsize,(const char **)cht);

  float fn[CHTsize];		// float !! do not use double
  for (int i=0; i<100000; i++) {
    fn[0] = normal();
    fn[1] = normal();
    nt.book(fn);
  }

  return 0;
}
