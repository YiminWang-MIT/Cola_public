//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaIASA/EPICS2Cindy.cc,v 1.4 1998-10-07 15:29:42 distler Exp $
//
// convert Q data to Cindy format
//

#define _POSIX_C_SOURCE 2
#define _DEFAULT_SOURCE    1
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "Cindy/Cindy++.h"
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <unistd.h>
#include <fstream.h>
extern "C" {
#include "getdate.h"
#include <stdio.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
}
#include "iasa0.h"

///This structure represents the maximal event
struct iasa0 iasa0;

/**The object ct is initialized. It knows about the maximal event.
 * Methods to decode the data are available.
 */
CindyTree *ct = cindyTreeAlloc();

class EPICSitem {
  ///the first item
  static EPICSitem * first;
  ///the last item
  static EPICSitem * last;
  ///time structure - used for time stamp information
  static struct tm * tms;
  ///last time stamp
  static int lastTimeStamp;
  ///time (UTC) when to write next ident
  static time_t nextIdent;
  ///elapsed time (in days) since begin time
  static int numOfDays;
  ///next item
  EPICSitem * next;
  ///address of the control item in the Cindy data structure
  caddr_t addr;
public:
  ///Constructor
  EPICSitem(const char *inam);
  ///read next input line and create Cindy event
  static int handle(ifstream & inp);
  ///initialize time information
  static int initTime(int days=0);
  ///return last time stamp (UTC)
  static time_t currentTime();
  ///begin time in seconds since 00:00:00 on January 1, 1970, (UTC).
  static time_t beginTime;
};

EPICSitem * EPICSitem::first = NULL;
EPICSitem * EPICSitem::last  = NULL;
struct tm * EPICSitem::tms   = NULL;
time_t EPICSitem::beginTime  = 0;
time_t EPICSitem::nextIdent  = 0;
int EPICSitem::lastTimeStamp = 0;
int EPICSitem::numOfDays     = 0;

/**Global variable that shows the status of the program.
 * Is changed to 1 if a signal is received.
 */
int gotSignal;

/**Signal service routine. If SIGINT or SIGTERM is received
 * the global variable gotSignal is changed to 1.
 */
void
handleSignal(int sig)
{
    if ((sig == SIGINT) || (sig == SIGTERM)) gotSignal = 1;
}

//
// Function name : printHelp(char * progname)
//
// Description   : print online help information
// Input         : progname = name of the program
//

int
printHelp(char * progname)
{
  cerr << endl << progname << ':' << endl;
  cerr << '\t' << "Q to Cindy convertion program" << endl << endl;
  cerr << " -d\t\t"      << "debug flag." << endl;
  cerr << " -f <file>\t" << "input  file. default: STDIN" << endl;
  cerr << " -o <file>\t" << "output file. default: STDOUT" << endl;
  cerr << " -v\t\t"      << "verbose flag." << endl;
  cerr << " -z\t\t"      << "gzip output." << endl;

  return 0;
} // end of printHelp(char *)


//
// Function name : main(int argc, char *argv[])
//
// Description   : main function
// Input         : argc = number of command line parameters
//                 argv = array of command line parameter strings
// Return        : return value != 0 indicates error
//

int
main(int argc, char *argv[])
{
  int opt, flags = 0;
  char * input  = NULL;
  char * output = "-";

  // parse command line options
  while (EOF != (opt = getopt(argc, argv, "df:ho:vz?"))) {
    switch (opt) {
    case 'd':
      flags |= debugFlag;
      break;
    case 'f':
      input  = optarg;
      break;
    case 'o':
      output = optarg;
      flags |= outputFlag;
      break;
    case 'v':
      flags |= verboseFlag;
      break;
    case 'z':
      flags |= zipFlag;
      break;
    case 'h':
    case '?':
      printHelp(*argv);
      return 0;
    default:
      printHelp(*argv);
      return -1;
    }
  }

  gotSignal = 0;
  signal(SIGINT,  handleSignal);
  signal(SIGTERM, handleSignal);
  putenv("TZ=EET"); tzset(); // Timezone Eastern Europe

  // parse input parameter (if any)
  int fd = 0;
  if (input) {
    switch (*input) {
    case '-': // use STDIN
      fd = 0;
      break;
    default:  // use FILE
      if ((fd = open(input, O_RDONLY)) < 0) {
	perror(argv[0]);
	return errno;
      }
      break;
    }
  }

  // set option flags and output stream in CindyTree object
  if (ct) {
    ct->putOpt(flags, NULL, output);
  } else {
    perror(argv[0]);
    return errno;
  }

  //
  // MAIN LOOP : read all Q records and convert data
  //
  ifstream inp(fd);
  const int llen = 1024;
  char line[llen];

  // read begin time and write configuration
  // and initialize EPICSitem time structure
  inp.get(line, llen, '\n');
  ct->putIdent(EPICSitem::beginTime=get_date(line+6, NULL), 1);
  EPICSitem::initTime();
  inp.get(line[0]);

  // read 2nd line and write as comment
  memset(line, 0, llen);
  inp.get(line, llen-1, '\n');
  inp.get(line[strlen(line)]);
  ct->putComment(line);

  // read number of items
  int i, inum;
  inp >> inum;

  // create EPICSitem objects
  for (i=0; i<inum; i++) {
    inp.get(line[0]); // skip newline
    inp.get(line, llen, '\n');
    if (new EPICSitem(line)); // keep compiler happy
  }
  inp.get(line[0]); // skip newline

  // read data
  while (!gotSignal && !EPICSitem::handle(inp)) { ; }
  ct->putIdent(EPICSitem::currentTime());

  // clean up
  if (input && (*input != '-')) close(fd);
  if (ct) ct->closeIO();

  return 0;
} // end of main()

EPICSitem::EPICSitem(const char *inam)
{
  addr = ct->addrOf(inam);
  next = NULL;

  if (addr) {
    if (first == NULL) first = this;
    if (last != NULL) last->next = this;
    last = this;
  }
}

int
EPICSitem::handle(ifstream & inp)
{
  int timeStamp;
  inp >> timeStamp;
  if (inp.eof()) return -1;

  if (timeStamp < lastTimeStamp) initTime(++numOfDays);
  lastTimeStamp = timeStamp;
  tms->tm_hour =  timeStamp / 10000;
  tms->tm_min  = (timeStamp / 100) % 100;
  tms->tm_sec  =  timeStamp % 100;

  // write ident every 300 sec = 5 min
  time_t now = mktime(tms);
  int *tsptr = (int *)ct->addrOf(".timestamp");
  *tsptr = now - beginTime;
  if (now > nextIdent) {
    nextIdent = (nextIdent>0 ? nextIdent : beginTime) + 300;
    ct->putIdent(now);
  }
  ct->packEventData(tsptr);

  EPICSitem * item = first;
  while (item) {
    float val;
    inp >> val;
    if (inp.eof()) return -1;
    *(float *)(item->addr) = val;
    ct->packEventData(item->addr);
    item = item->next;
  }
  ct->putPackedData();
  inp.get(); // skip newline
  if (inp.eof()) return -1;

  return 0;
}

int
EPICSitem::initTime(int days)
{
  time_t start = beginTime + days*24*60*60;
  if (tms == NULL) tms = new tm;
  memcpy(tms, localtime(&start), sizeof(struct tm));

  return 0;
}

time_t
EPICSitem::currentTime()
{
  return (tms ? mktime(tms) : 0);
}
