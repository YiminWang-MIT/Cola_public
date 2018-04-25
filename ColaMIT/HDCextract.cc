//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/HDCextract.cc,v 2.4 2002-12-12 23:52:12 distler Exp $
//
// Main Part of the Cindy analysis program
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */
#include "Chamber/HDCoops.h"
#include "rundatabase.h"       // "run.db" Parser
#include "HDCraw.h"
#include "Aqua_oops1.h"
#include "Aqua/AquaUnzip.h"
#include "Aqua/CindyReader.h"
#include "Aqua/BatesReader.h"
#if defined(__GNU_LIBRARY__)
#  include <getopt.h>
#endif
#include <iostream>
#include <signal.h>

extern "C" {
  char *getenv(const char *);
}

struct oops1 oops1;

int gotSignal;
int quiet = 0; // ReadRundb.o has external declaration
int verbose = (getenv("VERBOSE") != NULL);

void
handleSignal(int sig)
{
    if ((sig == SIGINT) || (sig == SIGTERM)) gotSignal = 1;
}

int
hdcPrint(const char *str, HdcPacket & hdc)
{
  if (hdc.ok() != 63) return -1;
  if ((hdc.getXplane(1))->goodCheck()) return -1;
  if ((hdc.getYplane(1))->goodCheck()) return -1;
  if ((hdc.getXplane(2))->goodCheck()) return -1;
  if ((hdc.getYplane(2))->goodCheck()) return -1;
  if ((hdc.getXplane(3))->goodCheck()) return -1;
  if ((hdc.getYplane(3))->goodCheck()) return -1;

  std::cout << str
       << (hdc.getXplane(1))->coord() << '\t'
       << (hdc.getYplane(1))->coord() << '\t'
       << (hdc.getXplane(2))->coord() << '\t'
       << (hdc.getYplane(2))->coord() << '\t'
       << (hdc.getXplane(3))->coord() << '\t'
       << (hdc.getYplane(3))->coord() << std::endl;

  return 0;
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
	"         Extract HDC data.\n\n" 
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

  HdcApacket hdcA(NULL, ".det.oopsA.chamber");
  HdcBpacket hdcB(NULL, ".det.oopsB.chamber");
  HdcCpacket hdcC(NULL, ".det.oopsC.chamber");
  HdcDpacket hdcD(NULL, ".det.oopsD.chamber");

  int timeOut = 0;
  int initialized = 0;
  while (!gotSignal) {
    switch (reader->nextEvent(atree)) {
    case 0:
      timeOut = 0;
      if (!initialized) {
	memset(&rundb, 0, sizeof(rundatabase));

	char *run_db = getenv("RUN_DB");
	if (!run_db) run_db = "run.db";
	char runname[18];
	if (atree->getRunName())	
	  sprintf(runname, "run_%12s", atree->getRunName());
	//	else if (ct->getInputFilename()) 
	//	  strncpy(runname, ct->getInputFilename(),17);
	long int datatime =
	  atree->getTimestamp() ? atree->getTimestamp() : time(NULL);
	readRunDatabase(run_db, runname, NULL, datatime);

	hdcinit(hdcA, rundb.oopsA);
	hdcinit(hdcB, rundb.oopsB);
	hdcinit(hdcC, rundb.oopsC);
	hdcinit(hdcD, rundb.oopsD);
	initialized++;
      }

      if (atree->itemOK(&oops1.det.oopsA.scint[2].tdc.right)) {
	hdcPrint("A : ", hdcA);
      }

      if (atree->itemOK(&oops1.det.oopsB.scint[2].tdc.right)) {
	hdcPrint("B : ", hdcB);
      }

      if (atree->itemOK(&oops1.det.oopsC.scint[2].tdc.right)) {
	hdcPrint("C : ", hdcC);
      }

      if (atree->itemOK(&oops1.det.oopsD.scint[2].tdc.right)) {
	hdcPrint("D : ", hdcD);
      }

      hdcA.Reset();
      hdcB.Reset();
      hdcC.Reset();
      hdcD.Reset();
      break;
    case 1:
      timeOut++;
      break;
    default:
      return 0;
      break;
    }
  }

  return 0;
}
