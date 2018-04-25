//                                                                    -*-c++-*-
// Authors:
//
// Michael O. Distler                        mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// Christian Kunz                            mailto:kunz@mit.edu
// MIT, Lab for Nuclear Science, 26-567      tel.   (617) 253-1521
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/HELcheck.cc,v 2.4 2002-12-12 23:52:12 distler Exp $
//
// Main Part of the Cindy analysis program to check the helicity pattern
//

// #define PRINT
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */
#include "rundatabase.h"       // "run.db" Parser
#include "Aqua_oops1.h"
#include "Aqua/AquaUnzip.h"
#include "Aqua/CindyReader.h"
#include "Aqua/BatesReader.h"
#if defined(__GNU_LIBRARY__)
#  include <getopt.h>
#endif
#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <string.h>

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
	"         Helicity check program of ColaMIT.\n\n" 
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

  int timeOut = 0;
  int initialized = 0;
  float helcoll = 0;
  float helok   = 0;
  float helbad  = 0;
  float helund  = 0;
  float badlam  = 0;
  int harr[2][16];
  char carr[2][16];
  char garr[2][16];
  int idxold = 0;
  int helidx = 0;
  memset(harr, 0, 2*16*sizeof(int));
  memset(carr, ' ', 2*16*sizeof(char));
  memset(garr, ' ', 2*16*sizeof(char));
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
	initialized++;
      }

      static int newbit, timeslot, helicity, gunbit;
      static char helchar, gunchar;

      // Spectrometer event 8
      if (atree->itemOK(&oops1.det.misc.helicity)) {
	if (oops1.det.misc.helicity & 0x8) {
	  gunbit = 1; gunchar = 'G';
	} else {
	  gunbit = 0; gunchar = 'B';
	}
      }

      // Beam event 10
      if (atree->itemOK(&oops1.beam.flag)) badlam++;
      if (atree->itemOK(&oops1.beam.helicity) ||
	  atree->itemOK(&oops1.beam.timeslot)) {
	newbit   = (oops1.beam.helicity & 0x4) >> 2;
	timeslot = oops1.beam.timeslot & 0xf;
	if (oops1.beam.helicity & 0x8) {
	  gunbit = 1; gunchar = 'g';
	} else {
	  gunbit = 0; gunchar = 'b';
	}
	switch (oops1.beam.helicity & 0x3) {
	case 1:  helicity = -1; helchar = '-'; break;
	case 2:  helicity =  1; helchar = '+'; break;
	default: helicity =  0; helchar = '0'; helbad++; break;
	}
      }

      // Scaler event 5
      if (atree->itemOK(&oops1.old_scaler.ohips.event5)) {
	gunchar = 'S';
      }

      // Target event 13
      if (atree->itemOK(&oops1.target.bottom.pressure)) {
	gunchar = 'T';
      }

      helidx = timeslot + (newbit ? 0 : 10);
      if (helidx < idxold) {
	int idx, doprint;
	for (idx=1,doprint=0; idx<=10; idx++) {
	  if (harr[0][idx] != 0 && garr[1][idx] == ' ') helund++;
	  if (harr[1][idx] != 0 && garr[0][idx] == ' ') helund++;
	  if (harr[1][idx]*harr[0][idx] == -1) helok++;
	  if (harr[1][idx]*harr[0][idx] == 1) {
	    helcoll++;
#ifdef PRINT
	    doprint++;
#endif
	  }
	}
	if (doprint) {
	  for (idx=1; idx<=10; idx++) 
	    std::cout << garr[1][idx] << carr[1][idx] << ' ';
	  std::cout << std::endl;
	  for (idx=1; idx<=10; idx++) 
	    std::cout << garr[0][idx] << carr[0][idx] << ' ';
	  std::cout << "       " << helbad << " / " << helcoll << " / "; 
	  std::cout << helund << " / " << helok << std::endl << std::endl;
	}
	memset(harr,   0, 2*16*sizeof(int));
	memset(carr, ' ', 2*16*sizeof(char));
	memset(garr, ' ', 2*16*sizeof(char));
      }
      idxold = helidx;

      harr[newbit][timeslot] = helicity;
      carr[newbit][timeslot] = helchar;
      garr[newbit][timeslot] = gunchar;

      break;
    case 1:
      timeOut++;
      break;
    default:
      // Final Result  
      float sum = (helok + helcoll) * 2 + helbad + helund; 

      std::cout << "Ivalid bits:\t" << helbad << "\t"
		<< (helbad / sum) << std::endl;
      std::cout << "Hel. Collision:\t" << helcoll * 2 << "\t";
      std::cout << (helcoll*2 / sum) << std::endl;
      std::cout << "Undetermined:\t" << helund << "\t";
      std::cout << (helund / sum) << std::endl;
      std::cout << "Helicity OK:\t" << helok * 2 << "\t";
      std::cout << (helok *2 / sum) << std::endl;
      std::cout << "No LAM ev10:\t" << badlam << "\t";
      std::cout << (badlam / sum) << std::endl;
      std::cout << "Total: \t\t" << sum << std::endl;

      return 0;
      break;
    }
  }
  return 0;
}
