//
// Created 1995-97 by	Michael O. Distler,
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz
//
// $Header: /tmp/cvsroot/Cola/Cola/Decode.cc,v 2.2 2002-12-12 12:23:55 distler Exp $
//
// Main Part of Cindy analysis program
//

#define _DEFAULT_SOURCE    1
#include "Cindy/Cindy++.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include ABC_H


int gotSignal;

void
handleSignal(int sig)
{
    if ((sig == SIGINT) || (sig == SIGTERM)) gotSignal = 1;
}


int
main(int argc, char *argv[])
{
  struct ABC ABC;
  CindyTree *ct = cindyTreeAlloc(&ABC);
  if (ct->decodeGetOpt(argc,argv)) return -1;

  gotSignal = 0;
  signal(SIGINT,  handleSignal);
  signal(SIGTERM, handleSignal);
  putenv("TZ=MET");  tzset(); // Timezone Central Europe

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
