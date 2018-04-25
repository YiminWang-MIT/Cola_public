//                                                                    -*-c++-*-
// Author:
//
// Michael O. Distler                        mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/Filter.cc,v 2.6 2002-12-12 23:52:12 distler Exp $
//
// Main Part of the Cindy analysis program to filter data
//

// #define PRINT
#define _DEFAULT_SOURCE    1
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include "rundatabase.h"       // "run.db" Parser
#include "Aqua_oops1.h"
#include "Aqua/AquaUnzip.h"
#include "Aqua/AquaCompress.h"
#include "Aqua/CindyReader.h"
#include "Aqua/BatesReader.h"
#if defined(__GNU_LIBRARY__)
#  include <getopt.h>
#endif

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
main(int argc, char *argv[])
{
  int opt, cindyFormat = 0;
  int useGZIP = 0;
  char *input = NULL;
  char *output = NULL;

#if defined(__GNU_LIBRARY__)
  struct option lo[] = {
    {"cindy",   0, NULL, 'c'},
    {"input",   1, NULL, 'f'},
    {"output",  1, NULL, 'o'},
    {"gzip",    0, NULL, 'z'},
    {"help",    0, NULL, 'h'},
    {NULL, 0, NULL, 0}};
  while ((opt = getopt_long(argc, argv, "cf:o:zh", lo, NULL)) != EOF) {
#else
  while ((opt = getopt(argc, argv, "cf:o:zh")) != EOF) {
#endif
    switch (opt) {
    case '?':                    // unknown option
    case ':':                    // missing argument
    case 'h':             
      std::cout << 
	"\nUsage:   " << argv[0] <<" [options]\n\n"
	"         Filter program of ColaMIT.\n\n" 
	"Options:\n\n" 
	"-h         --help           This help.\n"
	"-f <name>  --input=<name>   Input filename.\n"
        "-o <name>  --output=<name>  write data to file\n"
	"-z         --gzip           compress output using gzip\n"
	"-c         --cindy          Read file in old cindy format.\n";
      return 0;
    case 'c': cindyFormat = 1; break;
    case 'f': input = optarg; break;
    case 'o': output = optarg; break;
    case 'z': useGZIP = 1; break;
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

  if (output) {
    reader->initOutput(useGZIP ?
		       new AquaGzip(output) : new AquaIO(output, 0644));
  } else reader->initOutput(new AquaIO(1));

  gotSignal = 0;
  signal(SIGINT,  handleSignal);
  signal(SIGTERM, handleSignal);
  putenv("TZ=EST5EDT"); tzset(); // Timezone US Eastern

  int timeOut = 0;
  int doWrite = 0;
  int initialized = 0;
  while (!gotSignal) {
    switch (reader->nextEvent(atree)) {
    case 0:
      if (!initialized) {
	char *run_db = getenv( "RUN_DB" );
	if (!run_db) run_db =  "run.db";
	memset(&rundb, 0, sizeof(rundatabase));
	char runname[32];
	if (atree->getRunName())	
	  sprintf(runname, "run_%12s", atree->getRunName());
	//	else if (ct->getInputFilename()) 
	//	  strncpy(runname, ct->getInputFilename(),17);
	long int datatime =
	  atree->getTimestamp() ? atree->getTimestamp() : time(NULL);
	readRunDatabase(run_db, runname, NULL, datatime);
	if (rundb.tof.min >= rundb.tof.max) {
	  std::cerr << argv[0] << ":\tTime-of-flight cut does not make sense."
		    << std::endl << "\t\tCheck the run data base."
		    << std::endl;
	  return(1);
	}
	initialized = 1;
      }
      doWrite = 0;
      timeOut = 0;
      if (atree->itemOK(&oops1.det.flag) &&
	  atree->itemOK(&oops1.det.coincidence.tdc[2]) &&
	  ((oops1.det.flag == 65) || (oops1.det.flag == 66) ||
	   (oops1.det.flag == 74) || (oops1.det.flag == 75) ||
	   (oops1.det.flag == 77)) &&
	  (rundb.tof.min<oops1.det.coincidence.tdc[2]) &&
	  (oops1.det.coincidence.tdc[2]<rundb.tof.max)) doWrite++;
      if (atree->itemOK(&oops1.old_scaler.ohips.event5) ||
	  atree->itemOK(&oops1.target.bottom.pressure)) doWrite++;
      if (doWrite) reader->writeEvent();
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
