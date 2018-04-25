//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaJLab/rund.cc,v 1.2 2002-12-12 12:24:53 distler Exp $
//
// run server and storage program
//

#define _POSIX_C_SOURCE 2
#define _DEFAULT_SOURCE 1
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <stdio.h>
#include <iostream>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <unistd.h>
#include "Cindy/Cindy++.h"


struct header {
  unsigned short len;
  unsigned short null;
  unsigned short format;
  unsigned short flags;
  unsigned short events;
  unsigned short code;
};

const unsigned short EventFormat   = 30000;

const unsigned short EventType	   = 0xff00;
const unsigned short EventNormal   = 0x0000;
const unsigned short EventAdmin	   = 0x0100;
const unsigned short EventIdent	   = 0x0200;
const unsigned short EventConfig   = 0x0300;
const unsigned short EventSpecial  = 0x0400;
const unsigned short EventAny	   = 0x0500;
const unsigned short EventRuninfo  = 0x0600;
const unsigned short EventOldident = 0x0700;
const unsigned short EventBad	   = 0x0800;
const unsigned short EventRaw	   = 0x0900;
const unsigned short EventComment  = 0x0A00;
const unsigned short EventName	   = 0x0B00;
const unsigned short EventMsg	   = 0x0C00;

const unsigned short EventError	   = 0x0001;
const unsigned short EventNoswap   = 0x0002;
const unsigned short EventTar	   = 0x0004;
const unsigned short EventShar	   = 0x0008;


int gotSignal;

void
handleSignal(int sig)
{
    if ((sig == SIGINT) || (sig == SIGTERM)) gotSignal = 1;
}


//
// Function name : createSocket(int port)
//
// Description   : create socket for incoming connections
// Input         : port = port number
// Return        : file descriptor
//

int
createSocket(int port)
{
  char nodename[MAXHOSTNAMELEN];
  struct hostent * localhost;
  int insock;
  struct sockaddr_in insockaddr;

  if (gethostname(nodename, MAXHOSTNAMELEN) < 0) {
    perror("gethostname");
    exit(-1);
  }

  if (NULL == (localhost = gethostbyname(nodename))) {
    perror("gethostbyname");
    exit(-1);
  }

  if ((insock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(-1);
  }

  memset(insockaddr.sin_zero, 0, 8);
  insockaddr.sin_family = localhost->h_addrtype;
  insockaddr.sin_port   = htons(port);
  memcpy(&insockaddr.sin_addr, localhost->h_addr, localhost->h_length);
  if (bind(insock, (struct sockaddr *)&insockaddr, sizeof(insockaddr))) {
    perror("bind");
    exit(-1);
  }

  if (listen(insock, 1)) {
    perror("listen");
    exit(-1);
  }

  return insock;
} // end of createSocket(int)


//
// Function name : forkGzip(const char * filename)
//
// Description   : fork the gzip program
// Input         : filename = output file name for gzip
// Return        : parent file descriptor
//

int
forkGzip(const char * filename)
{
  int pid, fd;
  struct {
    int child;
    int parent;
  } pipes;

  if (pipe(&pipes.child)) exit(errno);

  if ((pid = fork()) < 0) exit(errno);

  if (!pid) { /* child */

    close(pipes.parent);
    if (0 != dup2(pipes.child, 0)) exit (errno);

    fd = creat(filename, S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
    if (1 != dup2(fd, 1)) exit (errno);

    char * args[2] = { GZIP, NULL };
    if (execvp(args[0], args)) exit (errno);

  } else { /* parent */

    close(pipes.child);

  }

  return pipes.parent;
} // end of forkGzip(const char *)


//
// Function name : printHelp(char * progname)
//
// Description   : print online help information
// Input         : progname = name of the program
// Return        : 
//

int
printHelp(char * progname)
{
  cerr << endl << progname << ':' << endl;
  cerr << '\t' << "run server and storage program" << endl << endl;
  cerr << " -f <file>\t" << "input  file. default: STDIN" << endl;
  cerr << " -p <port>\t" << "port number. default: 8000" << endl;
  cerr << " -v\t\t"      << "verbose flag." << endl;
  cerr << " -z\t\t"      << "gzip file output." << endl << endl;

  return 0;
} // end of printHelp(char *)


//
// Function name : Write(int fd, char * buf, int count)
//
// Description   : write buffer to file
// Input         : fd    = file descriptor
//                 buf   = data buffer
//                 count = number of chars to write
// Return        : number of chars written or -1 on error
//

int
Write(int fd, char * buf, int count)
{
  int havewritten = 0;

  if (fd < 0) return 0;

  while (count > 0) {
    int w = ::write(fd, buf, count);
    if (w < 0) return w;
    buf += w;
    count -= w;
    havewritten += w;
  }

  return havewritten;
} // end of Write(int, char *, int)


//
// Function name : ReadPipe(int fd, char * buf, int count)
//
// Description   : read pipe data to buffer
// Input         : fd    = file descriptor
//                 buf   = data buffer
//                 count = number of chars to write
// Return        : number of chars read or -1 on error
//

int
ReadPipe(int fd, char * buf, int count)
{
  int haveread = 0;

  while (count > 0) {
    int r = ::read(fd, buf, count);
    if (r < 0) return r;
    if ((r == 0) && !haveread) return haveread;
    buf += r;
    count -= r;
    haveread += r;
  }

  return haveread;
} // end of ReadPipe(int, char *, int)


//
// Function name : main(int argc, char *argv[])
//
// Description   : main function - setup file and network I/O
// Input         : argc = number of command line parameters
//                 argv = array of command line parameter strings
// Return        : return value != 0 indicates error
//

int
main(int argc, char *argv[])
{
  int opt, flags = 0;
  int port = 8000;
  char * input = NULL;
  char * buffer = NULL;

  // parse command line options
  while (EOF != (opt = getopt(argc, argv, "f:hp:vz?"))) {
    switch (opt) {
    case 'f':
      input = optarg;
      break;
    case 'p':
      port = atoi(optarg);
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

  // parse input parameter (if any)
  int sockfd, fd = 0;
  if (input) {
    // have to open pipes non blocking
    if (0 > (fd = open(input, O_RDONLY|O_NONBLOCK))) {
      perror(argv[0]);
      return errno;
    }
    // set blocking mode again
    int fdflag = fcntl(fd, F_GETFL);
    if (0 > fcntl(fd, F_SETFL, fdflag & ~O_NONBLOCK)) {
      perror(argv[0]);
      return errno;
    }
  }

  sockfd = createSocket(port);

  if ((fd < 0) || (sockfd < 0)) {
    perror(argv[0]);
    return errno;
  }

  buffer = new char[131084];
  if (!buffer) {
    perror(argv[0]);
    return errno;
  }


  //
  // MAIN LOOP : read run and distribute data
  //
  int ofd = -1;
  fd_set fdsOut;
  FD_ZERO(&fdsOut);
  gotSignal = 0;
  signal(SIGINT,  handleSignal);
  signal(SIGTERM, handleSignal);
  signal(SIGPIPE, SIG_IGN);
  while ( !gotSignal ) {
    fd_set fdsIn;
    struct timeval tv;
    int rval;

    FD_ZERO(&fdsIn);
    FD_SET(fd, &fdsIn);
    FD_SET(sockfd, &fdsIn);
    tv.tv_sec  = 1;
    tv.tv_usec = 0;

    rval = select(32, (FDSETPTR)&fdsIn, NULL, NULL, &tv);
    if (rval < 0) {
      if (errno == EINTR) continue;
      perror(*argv);
      return errno;
    }
    if (rval == 0) continue;

    if (FD_ISSET(sockfd, &fdsIn)) {
      struct sockaddr_in nsdesc;
      int sdesc;
      unsigned int ssiz = sizeof(nsdesc);
      while ((sdesc = accept(sockfd, (struct sockaddr *)&nsdesc, &ssiz)) < 0);
      FD_SET(sdesc, &fdsOut);
      fprintf(stderr, "accept : %d (%d)\n", sockfd, sdesc);
    }

    if (FD_ISSET(fd, &fdsIn)) {
      int r, havetoread;
      int w, havetowrite;
      struct header * head = (struct header *)buffer;

      havetoread = sizeof(struct header);
      if (havetoread != (r = ReadPipe(fd, (char *)head, havetoread))) {
	if (r == 0) {
	  if (ofd >= 0) { close(ofd); ofd = -1; }
	  if ((fd > 0) && input) {
	    int status;
	    close(fd);
	    wait(&status);
	    if ((fd = open(input, O_RDONLY|O_NONBLOCK)) < 0) {
	      perror(argv[0]);
	      return errno;
	    }
	    int fdflag = fcntl(fd, F_GETFL);
	    if (0 > fcntl(fd, F_SETFL, fdflag & ~O_NONBLOCK)) {
	      perror(argv[0]);
	      return errno;
	    }
	    continue;
	  }
	} else {
	  if (errno == EINTR) continue;
	  perror(*argv);
	  return errno;
	}
      }
      havetowrite = r;

      havetoread = 2*(int)(head->len);
      if (havetoread != (r = ReadPipe(fd, (char *)(head+1), havetoread))) {
	if (r == 0) {
	  if (ofd >= 0) { close(ofd); ofd = -1; }
	  if ((fd > 0) && input) {
	    int status;
	    close(fd);
	    wait(&status);
	    if ((fd = open(input, O_RDONLY|O_NONBLOCK)) < 0) {
	      perror(argv[0]);
	      return errno;
	    }
	    int fdflag = fcntl(fd, F_GETFL);
	    if (0 > fcntl(fd, F_SETFL, fdflag & ~O_NONBLOCK)) {
	      perror(argv[0]);
	      return errno;
	    }
	    continue;
	  }
	} else {
	  if (errno == EINTR) continue;
	  perror(*argv);
	  return errno;
	}
      }
      havetowrite += r;

      if ((head->format == EventFormat) &&
	  ((head->flags & EventName) == EventName)) {
	if (ofd >= 0) { close(ofd); ofd = -1; }
	char filename[32];
	if (flags & zipFlag) {
	  sprintf(filename, "run_%s.gz", (char *)(head+1));
	  ofd = forkGzip(filename);
	} else {
	  sprintf(filename, "run_%s", (char *)(head+1));
	  ofd = creat(filename, S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
	}
      }

      if (ofd >= 0)
	if (havetowrite != (w = Write(ofd, (char *)head, havetowrite))) {
	  fprintf(stderr, "write data : %d (%d)\n", w, havetowrite);
	  close(ofd); ofd = -1; // error
      }

      tv.tv_sec  = 0;
      tv.tv_usec = 0;
      fd_set fdsO;
      memcpy(&fdsO, &fdsOut, sizeof(fd_set));
      rval = select(32, NULL, (FDSETPTR)&fdsO, NULL, &tv);
      if (rval < 0) {
	perror(*argv);
	return errno;
      }

      int outfd = 3;
      while (rval) {
	if (FD_ISSET(outfd, &fdsO)) {
	  rval--;
	  if (havetowrite != (w = Write(outfd, (char *)head, havetowrite))) {
	    fprintf(stderr, "write socket : %d (%d)\n", w, havetowrite);
	    shutdown(outfd, 2);
	    close(outfd);
	    FD_CLR(outfd, &fdsOut);
	  }
	}
	outfd++;
      } // end of while (rval)

    } // end of if (FD_ISSET(fd, &fdsIn))

  } // end of while ( !gotSignal )

  if (fd >= 0) close(fd);
  if (ofd >= 0) close(ofd);
  if (sockfd >= 0) close(sockfd);

  delete buffer;
  return 0;
} // end of main()
