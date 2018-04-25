//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaJLab/HallAtest.cc,v 1.1 1999-02-03 23:00:44 distler Exp $
//
// test program for class HallAfile and Digitizer
//

#include "HallAfile.h"
#include <stdio.h>

struct hrs hrs;

/**@name HallAtest
 * @memo A debuging program that reads a coda file.
 */

//@{
//@}

int
main(int argc, char *argv[])
{
  CindyTree *ct = cindyTreeAlloc();
  HallAfile hAf(ct, 0);

  if (argc > 1) {
    printf("usage: %s\n", argv[0]);
  }

  while (!hAf.nextRecord()) {
    hAf.print();
  }

  return 0;
} // end of main()
