//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/Statistic.cc,v 2.4 2002-12-12 23:52:12 distler Exp $
//
// Main Part of the Cindy analysis program
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */
#include "StatPage.h"
#include "Aqua_oops1.h"
#include "Aqua/AquaUnzip.h"
#include "Aqua/CindyReader.h"
#include "Aqua/BatesReader.h"
#include <iostream>
#include <signal.h>
#if defined(__GNU_LIBRARY__)
#  include <getopt.h>
#endif

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
  int cindyFormat = 0;
  char opt, *input = NULL;

#if defined(__GNU_LIBRARY__)
  struct option lo[] = {
    {"cindy",   0, NULL, 'c'},
    {"input",   1, NULL, 'f'},
    {"help",    0, NULL, 'h'},
    {NULL, 0, NULL, 0}};
  while ((opt = getopt_long(argc, argv, "cf:h", lo, NULL)) != EOF) {
#else
  while ((opt = getopt(argc, argv, "cf:h")) != EOF) {
#endif
    switch (opt) {
    case '?':                    // unknown option
    case ':':                    // missing argument
    case 'h':             
      std::cout << 
	"\nUsage:   " << argv[0] <<" [options]\n\n"
	"         Statistic calculation of ColaMIT.\n\n" 
	"Options:\n\n" 
	"-h      --help        This help.\n"
	"-f <f>  --input=<f>   Input filename.\n"
	"-c      --cindy       Read file in old cindy format.\n";
      return 0;
    case 'f': input = optarg; break;
    case 'c': cindyFormat = 1; break;
    }
  }

  // parse input parameter (if any)
  AquaIO *aquaIO = NULL;
  if (!input || (*input=='-')) {
    aquaIO = new AquaUnzip(0);
  } else aquaIO = new AquaUnzip(input);

  struct oops1 oops1;
  AquaTree *atree = new Aqua_oops1(&oops1);
  AquaReader *reader = (cindyFormat ?
			(AquaReader *)new CindyReader(aquaIO) :
			(AquaReader *)new BatesReader(aquaIO));
  reader->initTree(atree);

  gotSignal = 0;
  signal(SIGINT,  handleSignal);
  signal(SIGTERM, handleSignal);
  putenv("TZ=EST5EDT"); tzset(); // Timezone US Eastern

  statPage sp(atree);
  int timeOut = 0;
  while (!gotSignal) {
    switch (reader->nextEvent(atree)) {
    case 0:
      timeOut = 0;
      sp.handle(&oops1);
      break;
    case 1:
      timeOut++;
      if (!(timeOut%1000))
	std::cout << " ... waiting for data ... " << std::endl;
      break;
    default:
      sp.print(&std::cout);
      return 0;
      break;
    }
  }

  return 0;
}
