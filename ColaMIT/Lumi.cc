#define _POSIX_C_SOURCE 2
#include "../config.h"
#include "Cindy/Cindy++.h"
#include "oops1.h"
#include "online.h"
#include "rundatabase.h"
#include "Luminosity.h"
#include "Reactions.h"
#include "Targetlength.h"
#include "QuasiRandom.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#if defined(__GNU_LIBRARY__)
#  include <getopt.h>
#endif

#define env_def(name,var,default) char *name = (char *)getenv(var);\
                                  if (!name) name= default;

SobolSequence sobol;
int verbose    = (getenv("VERBOSE") != NULL);

time_t 
dateFromFilename(const char *name)
{ 
  struct tm t;
  for(unsigned int i=0; i<strlen(name); i++) {
    if (sscanf(&name[i],"%02d%02d%02d%02d%02d%02d",
	       &t.tm_year, &t.tm_mon, &t.tm_mday,
	       &t.tm_hour, &t.tm_min, &t.tm_sec) == 6) {
      t.tm_mon--;
      t.tm_isdst = -1;
      return mktime(&t);
   }
  }
  cerr << "ERROR: Can't extract start time from \""<<name<<"\"."<<endl;
  exit (-1);
  return 0;
} 

int main(int argc, char *argv[])
{
  struct oops1 OOPS;
  CindyTree *ct  = cindyTreeAlloc(&OOPS);

  env_def(lumi_out,  "LUMI_OUT",  "luminosity.out");
  env_def(lumi_trip, "LUMI_TRIP", "luminosity.trip.out");
  env_def(run_db,    "RUN_DB",    "run.db");

  rundb.target = NULL;
  rundb.Target.length = rundb.Target.angle = 0; 
  rundb.O.PS = 1;
  rundb.A.PS = rundb.B.PS = rundb.C.PS = rundb.D.PS = 1;
  rundb.O.ScalerSingle = 1;
  rundb.A.ScalerSingle = rundb.B.ScalerSingle = rundb.C.ScalerSingle =
  rundb.D.ScalerSingle = 1; 
  rundb.A.ScalerDouble = rundb.B.ScalerDouble = rundb.C.ScalerDouble =
  rundb.D.ScalerDouble = 1; 
    
  long events          = 0;
  int  maxevents       = 0;
  int  finished        = 0;

  target     *Target     = NULL;
  luminosity *Luminosity = NULL;

  int flags = ignoreExpFlag;     // Parse options

  char status[255];

  extern int quiet;
  char opt, *in = NULL, *update=NULL;

#if defined(__GNU_LIBRARY__)
  struct option lo[] = {
    {"quiet",     0, NULL,'q'},    {"help",      0, NULL,'h'},
    {"query",     1, NULL,'V'},    {"verbose",   0, NULL,'v'},
    {"update",    1, NULL,'u'},    {"input",     1, NULL,'f'},
    {"output",    1, NULL,'o'},    {"tripoff",   0, NULL,'y'},
    {"zip",       0, NULL,'z'},    {"end",       1, NULL,'n'},
    {NULL, 0, NULL, 0}};
  while ((opt = getopt_long(argc, argv, "qhvn:o:zf:V:",lo,NULL))!=EOF) {
#else
  while ((opt = getopt(argc, argv, "qhvn:o:zf:V:")) != EOF) {
#endif
    switch (opt) {
    case '?':                    // unknown option
    case ':':                    // missing argument
    case 'h':             
      cout << 
	"\nUsage:   " << argv[0] <<" [options]\n\n"
	"         Luminosity calculation of Cola++.\n\n" 
	"Options:\n\n" 
	"-h      --help        This help.\n"
	"-f <f>  --input=<f>   Input filename.\n"
	"-o <f>  --output=<f>  Output file name for statistics.\n"
	"-v      --verbose     Output of the 'run.db' contents to stdout.\n"
	"-V <run>--query=<run> Query database contents by runname.\n"
	"-q      --quiet       Output of comment lines will be suppressed.\n"
	"-z      --zip         Decompress input with gzip.\n"
	"-n <n>  --end         Terminate after <n> events.\n"
	"configured for experiment '" << ct->getExperimentName() << "'\n"
	"compiled "__DATE__" "__TIME__"\n"
	"for detailed help see:\n"
	"http:/""/wwwa1.kph.uni-mainz.de/A1/trigger/doc/Lumi++.html\n\n";
      return 0;
    case 'f': in              = optarg;       break;
    case 'q': quiet           = 1;            break;
    case 'o': lumi_out        = optarg;       break;
    case 'v': verbose         = 1;            break;
    case 'V': verbose         = 1;
              readRunDatabase(run_db, optarg, NULL, dateFromFilename(optarg));
	      exit(0);
    case 'z': flags          |= zipFlag;      break;
    case 'n': maxevents       = atoi(optarg); break;
    }
  }
  if (ct->putOpt(flags, in, NULL)) return -1;

  char runname[17] = "run_????????????";
  while(!finished) { 
    // event-loop

    switch (ct->analyseNextEvent()) {
    case  1: continue;                              break; // timeOut
    case  0: if (! maxevents || events < maxevents) break;
    case -1: finished = 1; continue;                break; // end of event-loop
    }

    if (events==0) {
      // start of event-loop -> do some initializations
      if (ct->getRunName())	
	sprintf(runname, "run_%12s", ct->getRunName());
      else if (ct->getInputFilename()) 
	strncpy(runname, ct->getInputFilename(),17);

      readRunDatabase(run_db, runname, NULL, ct->getTimeOfDataTaking());
      reaction *Reaction =  new reaction(rundb.reaction);
      Target = SetTargetFromRunDB(Reaction);
      Luminosity = new luminosity(ct, Target, Reaction, rundb.Ebeam);
      Luminosity->
	setReadoutScaler((int) rundb.O.PS, (int) rundb.O.ScalerSingle,
			 (int) rundb.O.ScalerDouble,
			 (int) rundb.A.PS, (int) rundb.A.ScalerSingle,
			 (int) rundb.A.ScalerDouble,
			 (int) rundb.B.PS, (int) rundb.B.ScalerSingle,
			 (int) rundb.B.ScalerDouble,
			 (int) rundb.C.PS, (int) rundb.C.ScalerSingle,
			 (int) rundb.C.ScalerDouble,
			 (int) rundb.D.PS, (int) rundb.D.ScalerSingle,
			 (int) rundb.D.ScalerDouble);
    }

    // increase Luminosity
    Luminosity->handle(&OOPS);
    
    // output of statusline
    if (!(++events % 1000)) {
      sprintf(status, "Events: %ld (int. Luminosity=%.1f (pbarn)^-1, "
	      "%4.1f%% dead time)", events, Luminosity->getIntegrated()/1e6,
	      Luminosity->getDeadtime());
      cerr << "\r" << status << " \r" << flush;
    }
  }

  // output of last statusline
  sprintf(status, "Events: %ld (int. Luminosity=%.1f (pbarn)^-1, "
	  "%4.1f%% dead time)", events, Luminosity->getIntegrated()/1e6,
	  Luminosity->getDeadtime());
  cerr << "\r" << status << " \r" << endl;

  // output of statistics
  Luminosity->print(lumi_out, lumi_trip);

  return 0;
}
