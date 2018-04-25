//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/TMA/ftmTest.cc,v 1.4 2002-11-26 17:11:27 distler Exp $
//

#include <iostream>
#include <time.h>
#include "FastTM.h"

int
main(int argv, char *argc[])
{
  if (argv != 2) {
    std::cerr << "Usage : " << argc[0] << " <filename>" << std::endl;
    return -1;
  }

  TargetCo *result = NULL;
  TMAC *tma = new TMAC(argc[1]);

  double x = 14.738;
  double t = -11.003;
  double y = 22.42;
  double p = 2.808;

  long i=0;
  double start = (double)clock();
  for (;i<100000;i++) result = tma->run(x, t, y, p);
  std::cout << i << ": " << ((double)clock()-start)/CLOCKS_PER_SEC << " sec\n";

  std::cout << "dp  = " << result->dp << std::endl;
  std::cout << "th  = " << result->th << std::endl;
  std::cout << "y0  = " << result->y0 << std::endl;
  std::cout << "ph  = " << result->ph << std::endl;
  std::cout << "len = " << result->len << std::endl;

  return 0;
}
