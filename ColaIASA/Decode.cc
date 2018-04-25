//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaIASA/Decode.cc,v 1.3 2002-12-12 12:24:48 distler Exp $
//
// Main Part of the Cindy analysis program
//

#define _DEFAULT_SOURCE    1
#include "Cindy/Cindy++.h"
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include "iasa0.h"

struct iasa0 iasa0;

int gotSignal;

void
handleSignal(int sig)
{
    if ((sig == SIGINT) || (sig == SIGTERM)) gotSignal = 1;
}


int
main(int argc, char *argv[])
{
  CindyTree *ct = cindyTreeAlloc();
  if (ct->decodeGetOpt(argc,argv)) return -1;

  gotSignal = 0;
  signal(SIGINT,  handleSignal);
  signal(SIGTERM, handleSignal);
  putenv("TZ=EET"); tzset(); // Timezone Eastern Europe

  int timeOut = 0;
  while (!gotSignal) {
    switch (ct->decodeNextEvent()) {
    case 0:
      timeOut = 0;
      break;
    case 1:
      if (!(timeOut%1000)) cout << " ... waiting for data ... " << endl;
      timeOut++;
      break;
    default:
      ct->closeIO();
      return 0;
      break;
    }
  }

  ct->closeIO();
  return 0;
}
