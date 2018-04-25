//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/qtest.cc,v 2.1 2002-03-05 03:49:54 distler Exp $
//
// test program for class Qtape
//

#include <stdio.h>
#include "qtape.h"

int
main(int argc, char *argv[])
{
  Qtape qt(0);

  if (argc > 1) {
    printf("usage: %s\n", argv[0]);
  }

  while (Qtape::EoF != qt.nextRecord()) {
    qt.print();
  }

  return 0;
} // end of main()
