//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/Decode.cc,v 2.2 2002-12-12 12:24:12 distler Exp $
//
// Main Part of the Cindy analysis program
//

#define _DEFAULT_SOURCE    1
#include "Cindy/Cindy++.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */
#include <iostream>
#include <signal.h>
#include "oops1.h"

extern "C" int putenv(const char *);

struct oops1 oops1;

int gotSignal;

void
handleSignal(int sig)
{
    if ((sig == SIGINT) || (sig == SIGTERM)) gotSignal = 1;
}


int
main(int argc, char *argv[])
{
  CindyTree *ct = cindyTreeAlloc(&oops1);
  if (ct->decodeGetOpt(argc,argv)) return -1;

  gotSignal = 0;
  signal(SIGINT,  handleSignal);
  signal(SIGTERM, handleSignal);
  putenv("TZ=EST5EDT"); tzset(); // Timezone US Eastern

  int timeOut = 0;
  while (!gotSignal) {
    switch (ct->decodeNextEvent()) {
    case 0:
      timeOut = 0;
      break;
    case 1:
      timeOut++;
      if (!(timeOut%1000)) cout << " ... waiting for data ... " << endl;
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
