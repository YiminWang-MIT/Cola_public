//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaMIT/qhead.cc,v 2.1 2002-03-05 03:49:49 distler Exp $
//
// test program for class Qtape
//

#include "qtape.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <utime.h>

int
main(int argc, char *argv[])
{
  if (argc != 2) {
    printf("usage: %s <run>\n", argv[0]);
  }

  int fd;
  if (0 > (fd = open(argv[1], O_RDONLY))) {
    perror(argv[0]);
    return errno;
  }

  Qtape qt(fd);
  int finish = 0;
  char runname[128];
  char datetime[128];

  strcpy(runname, argv[1]);
  while (!finish) {
    QbeginningOfRunRecord * brec;
    QendOfRunRecord * erec;
    struct tm * tmptr;
    struct utimbuf tbuf;
    time_t t;
    switch (qt.nextRecord()) {
    case QtapeBase::BoR :
      brec = (QbeginningOfRunRecord *) qt.getRecord();
      sprintf(runname, "run%d.dat", brec->runNumber());
      rename(argv[1], runname);
      t = brec->dateTime();
      tmptr = localtime(&t);
      sprintf(datetime, "run_%02d%02d%02d%02d%02d%02d",
	      tmptr->tm_year, tmptr->tm_mon+1, tmptr->tm_mday,
	      tmptr->tm_hour, tmptr->tm_min, tmptr->tm_sec);
      break;
    case QtapeBase::EoR :
      erec = (QendOfRunRecord *) qt.getRecord();
      tbuf.actime = tbuf.modtime = erec->dateTime();
      utime(runname, &tbuf);
      break;
    case QtapeBase::None :
    case QtapeBase::EoF :
      finish = 1;
      break;
    default:
      break;
    }
  }

  printf("%s %s\n", runname, datetime);

  return 0;
} // end of main()

