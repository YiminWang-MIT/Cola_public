//
// Concept 1995 by	Michael O. Distler, 
//                      and Harald Merkel
//			Institut fuer Kernphysik,
//			Johannes Gutenberg-Universitaet Mainz 
//
// written 1997 by      Marco Kahrau    22.07.97
//

#define _POSIX_C_SOURCE 2
#include "../config.h"
#include "Luminosity.h"
#include "Aqua_abcn.h"
#include "Aqua/AquaUnzip.h"
#include "Aqua/CindyReader.h"
#include "Aqua/MainzReader.h"
#include "Detector/pdcD.h"
#include "QuasiRandom.h"
#include "rundatabase.h"
#include <unistd.h>
#if defined(__GNU_LIBRARY__)
#  include <getopt.h>
#endif

pdcD *PdcD;
// This is totally useless but needed to keep Parsermain.o happy
// to process the Ntuple commands.
//
#include "ntmode.h"
char *ntvarn[MAX_NT_VAR];  // Scalar name related to ntvar
char *ntdes[MAX_NT_VAR];    // NTvariable name
char *ntbck[MAX_NT_VAR];    // Block name for each ntvar
char *nttitle;     // Ntuple Title
int  ntiden, ntnvar;

#define env_def(name,var,default) char *name = (char *)getenv(var);\
                                  if (!name) name= default;

SobolSequence sobol;
int verbose      = (getenv("VERBOSE") ? atoi(getenv("VERBOSE")) : 0);
time_t starttime = 0;

time_t 
dateFromFilename(const char *name)
{ 
  struct tm t;
  for(unsigned int i=0; i<strlen(name); i++) {
    if (sscanf(&name[i],"%02d%02d%02d%02d%02d%02d",
	       &t.tm_year, &t.tm_mon, &t.tm_mday,
	       &t.tm_hour, &t.tm_min, &t.tm_sec) == 6) {
      if (t.tm_year<70) t.tm_year += 100;
      t.tm_mon--;
      t.tm_isdst = -1;
      return mktime(&t);
    }
  }
  std::cerr << "ERROR: Can't extract start time from \""<<name<<"\"."<<std::endl;
  exit (-1);
  return 0;
} 

int main(int argc, char *argv[])
{
  env_def(lumi_out,  "LUMI_OUT",  "luminosity.out");
  env_def(lumi_trip, "LUMI_TRIP", "luminosity.trip.out");
  env_def(run_db,    "RUN_DB",    "run.db");

  rundb.target = NULL;
  rundb.turns = rundb.Target.length = rundb.Target.angle = 0; 
  rundb.A.Prescaler    = rundb.B.Prescaler    = rundb.C.Prescaler    = 1;
  rundb.A.ScalerSingle = rundb.B.ScalerSingle = rundb.C.ScalerSingle = 1; 
  rundb.A.ScalerDouble = rundb.B.ScalerDouble = rundb.C.ScalerDouble = 1;
  rundb.A.PreStrobe    = rundb.B.PreStrobe    = rundb.C.PreStrobe    = 1;
  rundb.A.StrobeDead   = rundb.B.StrobeDead   = rundb.C.StrobeDead   = 90; //ns

  long events          = 0;
  int  maxevents       = 0;
  int  finished        = 0;
  int  trippoffControl = 0;

  target     *Target     = NULL;
  luminosity *Luminosity = NULL;

  char status[255];

  extern int quiet;
  int cindyFormat = 0;
  char opt, *in = NULL, *update=NULL;

#if defined(__GNU_LIBRARY__)
  struct option lo[] = {
    {"cindy",   0, NULL, 'c'}, {"end",     1, NULL, 'n'}, {"help",    0, NULL, 'h'},
    {"input",   1, NULL, 'f'}, {"output",  1, NULL, 'o'}, {"query",   1, NULL, 'V'},
    {"quiet",   0, NULL, 'q'}, {"tripoff", 0, NULL, 'y'}, {"update",  1, NULL, 'u'},
    {"verbose", 0, NULL, 'v'}, {NULL, 0, NULL, 0}};
  while ((opt = getopt_long(argc, argv, "Tqhvyn:o:cf:u:V:t:",lo,NULL))!=EOF) {
#else
  while ((opt = getopt(argc, argv, "Tqhvyn:o:cf:u:V:t:")) != EOF) {
#endif
    switch (opt) {
    case '?':                    // unknown option
    case ':':                    // missing argument
    case 'h':             
      std::cout << 
	"\nUsage:   " << argv[0] <<" [options]\n\n"
	"         Luminosity calculation of Cola++.\n\n" 
	"Options:\n\n" 
	"-h      --help        This help.\n"
	"-f <f>  --input=<f>   Input filename.\n"
	"-o <f>  --output=<f>  Output file name for statistics.\n"
	"-v      --verbose     Output of the 'run.db' contents to stdout.\n"
	"-V <run>--query=<run> Query database contents by runname.\n"
	"-q      --quiet       Output of comment lines will be suppressed.\n"
        "-T                    Try to extract start time from filename.\n"
        "-t YYMMDDhhmmss       Start time\n"
	"-c      --cindy       Read file in old cindy format.\n"
	"-n <n>  --end         Terminate after <n> events.\n"
	"-u <db> --update=<db> "
	"Update sql database, e.g. \"//a1ipinull1/a1rundb\"\n"
	"-y      --tripoff     Enable VDC trippoff control.\n\n"
	"compiled "__DATE__" "__TIME__"\n"
	"for detailed help see:\n"
	"http:/""/wwwa1.kph.uni-mainz.de/A1/trigger/doc/Lumi++.html\n\n";
      return 0;
    case 'f': in              = optarg;       break;
    case 'q': quiet           = 1;            break;
    case 'o': lumi_out        = optarg;       break;
    case 'u': update          = optarg;       break;
    case 'v': verbose         = 1;            break;
    case 'T': starttime       = -1;           break;
    case 't': starttime = dateFromFilename(optarg); break;
    case 'V': verbose         = 1;
              readRunDatabase(run_db, optarg, NULL, dateFromFilename(optarg));
	      exit(0);
    case 'n': maxevents       = atoi(optarg); break;
    case 'y': trippoffControl = 1;            break;
    case 'c': cindyFormat = 1;                break;
    }
  }

  // parse input parameter (if any)
  AquaIO *aquaIO = NULL;
  if (!in || (*in=='-')) {
    aquaIO = new AquaUnzip(0);
  } else aquaIO = new AquaUnzip(in);

  struct abcn abcn;
  AquaTree *atree = new Aqua_abcn(&abcn);
  AquaReader *reader = (cindyFormat ?
			(AquaReader *)new CindyReader(aquaIO) :
			(AquaReader *)new MainzReader(aquaIO));
  reader->initTree(atree);

  if (starttime==-1)  starttime = dateFromFilename(in);

  char runname[32] = "run_????????????";
  while(!finished) { 
    // event-loop

    switch (reader->nextEvent(atree)) {
    case  1: continue;                              break; // timeOut
    case  0: if (! maxevents || events < maxevents) break;
    case -1: finished = 1; continue;                break; // end of event-loop
    }

    if (events==0) {
      // start of event-loop -> do some initializations
	if (atree->getRunName()) {
	if ((strlen(atree->getRunName()) > 12) &&
	    (strncmp(atree->getRunName(),"run", 3))) {
		strcpy(runname, atree->getRunName());
	    } else sprintf(runname, "run_%12s", atree->getRunName());
	}
      //      else if (ct->getInputFilename()) 
      //	strncpy(runname, ct->getInputFilename(),17);

      readRunDatabase(run_db, runname, NULL,
		      starttime ? starttime : atree->getTimestamp());
      reaction *Reaction =  new reaction(rundb.reaction);
      Target = SetTargetFromRunDB(Reaction);
      if (!rundb.turns) rundb.turns = ((int)(rundb.Ebeam + 0.5) - 180.0) / 7.5;
      Luminosity = new luminosity(atree, Target, Reaction, rundb.turns);
      Luminosity->
	setReadoutScaler((int) rundb.A.Prescaler, (int) rundb.A.ScalerSingle,
			 (int) rundb.A.ScalerDouble, (int) rundb.A.PreStrobe,
			 (double) rundb.A.StrobeDead,
			 (int) rundb.B.Prescaler, (int) rundb.B.ScalerSingle,
			 (int) rundb.B.ScalerDouble, (int) rundb.B.PreStrobe,
			 (double) rundb.B.StrobeDead,
			 (int) rundb.C.Prescaler, (int) rundb.C.ScalerSingle,
			 (int) rundb.C.ScalerDouble, (int) rundb.C.PreStrobe,
			 (double) rundb.C.StrobeDead, 
			 (int) rundb.kaos.ScalerSingle, 
			 (int) rundb.kaos.ScalerDouble);
      if (trippoffControl) Luminosity->useTrippoffControl();
    }
    
    // increase Luminosity
    Luminosity->handle(&abcn);
    
    // output of statusline
    if (!(++events % 1000)) {
      sprintf(status, "Events: %ld (int. Luminosity=%.1f (pbarn)^-1, "
	      "%4.1f%% dead time)", events, Luminosity->getIntegrated()/1e6,
	      Luminosity->getDeadtime());
      std::cerr << "\r" << status << " \r" << std::flush;
    }
  }

  // output of last statusline
  sprintf(status, "Events: %ld (int. Luminosity=%.1f (pbarn)^-1, "
	  "%4.1f%% dead time)", events, Luminosity->getIntegrated()/1e6,
	  Luminosity->getDeadtime());
  std::cerr << "\r" << status << " \r" << std::endl;

  // output of statistics
  Luminosity->print(lumi_out, lumi_trip);
  if (update) updateSQLdatabase(runname, Luminosity, update);

  return 0;
}
