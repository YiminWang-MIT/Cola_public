//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/qread.cc,v 2.3 2002-12-12 23:52:12 distler Exp $
//
// test program for class Qtape
//

#define _POSIX_C_SOURCE 2
#include "magTape.h"
#include "qtape.h"
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <utime.h>
#include <sys/wait.h>


int
printHelp(char * progname)
{
  std::cerr << std::endl << progname << ':' << std::endl;
  std::cerr << "\tread Q tape and compress data" << std::endl << std::endl;
  std::cerr << " -f <dev>\t" << "tape device" << std::endl;
  std::cerr << " -r\t\t"     << "rewind tape first" << std::endl;
  std::cerr << " -v\t\t"     << "verbose flag." << std::endl << std::endl;

  return 0;
} // end of printHelp(char *)


int
main(int argc, char *argv[])
{
  char * tape = NULL;
  int opt;
  int rewind = 0;
  int verbose = 0;

  while (EOF != (opt = getopt(argc, argv, "f:hrv?"))) {
    switch (opt) {
    case 'f':
      tape = optarg;
      break;
    case 'r':
      rewind++;
      break;
    case 'v':
      verbose++;
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

  if (!tape) {
    printHelp(*argv);
    return -1;
  }

  magTape mt(tape);
  if (rewind && mt.rewind()) {
    perror(*argv);
    return errno;
  }
  if (mt.setblk(0)) {
    perror(*argv);
    return errno;
  }

  int endOfTape = 0;
  while (!endOfTape) {

    Qtape qt(mt.open());
    struct {
      int child;
      int parent;
    } pipes;
    int endOfData = 0;
    while (!endOfData) {

      QbeginningOfRunRecord * brec;
      QendOfRunRecord * erec;
      char runname[128];
      char datetime[128];
      struct tm * tmptr;
      struct utimbuf tbuf;
      time_t t;
      int pid;
      switch (qt.nextRecord()) {
      case QtapeBase::BoR :
	brec = (QbeginningOfRunRecord *) qt.getRecord();
	sprintf(runname, "run%d.dat.gz", brec->runNumber());
	t = brec->dateTime();
	tmptr = localtime(&t);
	sprintf(datetime, "run_%02d%02d%02d%02d%02d%02d.gz",
		tmptr->tm_year, tmptr->tm_mon+1, tmptr->tm_mday,
		tmptr->tm_hour, tmptr->tm_min, tmptr->tm_sec);
	if (verbose) std::cout << "name : " << runname << std::endl;
	if (verbose) std::cout << "link : " << datetime << std::endl;
	if (pipe(&pipes.child)) {
	  perror(*argv);
	  return errno;
	}
	if ((pid = fork()) < 0) {
	  perror(*argv);
	  return errno;
	}
	if (!pid) { /* child */
	  int ofd;
	  mt.close();
	  close(pipes.parent);
	  unlink(datetime);
	  if (0 > (ofd = creat(runname, S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH))) {
	    perror(*argv);
	    return errno;
	  }
	  if (link(runname, datetime)) {
	    perror(*argv);
	    return errno;
	  }
	  if (0 != dup2(pipes.child, 0)) {
	    perror(*argv);
	    return errno;
	  }
	  if (1 != dup2(ofd, 1)) {
	    perror(*argv);
	    return errno;
	  }
	  close(2);

	  char * args[2] = { GZIP, NULL };
	  if (execvp(args[0], args)) {
	    perror(*argv);
	    return errno;
	  }
	} else { /* parent */
	  close(pipes.child);
	  qt.putRecord(pipes.parent);
	}
	break;
      case QtapeBase::EoR :
	erec = (QendOfRunRecord *) qt.getRecord();
	tbuf.actime = tbuf.modtime = erec->dateTime();
	if (verbose)
	  std::cout << "time : " << ctime(&tbuf.modtime) << std::endl;
	close(pipes.parent);
	wait(NULL);
	utime(runname, &tbuf);
	// if (verbose) mt.printStatus();
	break;
      case QtapeBase::None :
	//std::cerr << "*** corrupt data ***" << std::endl;
	endOfTape++;
      case QtapeBase::EoF :
	mt.close();
	endOfData++;
      default:
	break;
      } // switch (qt.nextRecord())
    } // while (!endOfData)

    // if (verbose) mt.printStatus();
    if (mt.isEOT()) endOfTape++;

  } // while (!endOfTape)

  if (mt.offline()) {
    perror(*argv);
    return errno;
  }

  return 0;
} // end of main()
