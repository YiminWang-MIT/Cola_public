//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaJLab/codatest.cc,v 1.1.1.1 1999-01-24 21:44:07 distler Exp $
//
// test program for class CodaFile
//

#include <stdio.h>
#include "codafile.h"

/**@name codatest
 * @memo A debuging program that reads a coda file.
 */

//@{
//@}

int
main(int argc, char *argv[])
{
  CodaFile cf(0);

  if (argc > 1) {
    printf("usage: %s\n", argv[0]);
  }

  while (!cf.nextRecord()) {
    cf.print();
  }

  return 0;
} // end of main()
