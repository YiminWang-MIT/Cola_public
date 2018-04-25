//                                                                    -*-c++-*-
// the guy to blame:
//
// Michael O. Distler                        mailto:distler@mitlns.mit.edu
// MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
// Cambridge, MA 02139                       fax    (617) 258-5440
//
// $Header: /tmp/cvsroot/Cola/ColaJLab/Coda2Cindy.cc,v 1.2 2002-12-12 12:24:53 distler Exp $
//
// convert Coda data to Cindy format
//

#define _POSIX_C_SOURCE 2
#define _DEFAULT_SOURCE    1
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "HallAfile.h"
#include <iostream>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <signal.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/socket.h>
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include "hrs.h"

///This structure represents the maximal event
struct hrs hrs;

/**The object ct is initialized. It knows about the maximal event.
 * Methods to decode the data are available.
 */
CindyTree *ct = cindyTreeAlloc();

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

/**This function will daemonize the process.
 * Stop signal will be ignored, open file descriptors will be closed,
 * the process will be forked and put in the background.
 */
int
detachProcess(const char * progname)
{
  int childpid;
  long fd;

  // Ignore the terminal stop signals (BSD)
  signal(SIGTTOU, SIG_IGN);
  signal(SIGTTIN, SIG_IGN);
  signal(SIGTSTP, SIG_IGN);

  // fork and let the parent exit
  if ( (childpid = fork()) < 0) {
    perror(progname);
    return errno;
  } else if (childpid > 0) exit(0); // parent

  // create a session and set the process group ID
  if (setsid() == -1) perror(progname);

  if ( (childpid = fork()) < 0) {
    perror(progname);
    return errno;
  } else if (childpid > 0) exit(0); // first child

  // Close any open files descriptors
  for (fd = sysconf(_SC_OPEN_MAX)-1; fd >= 0; fd--) close(fd);
  errno = 0; // probably got set to EBADF from a close

  // stdin, stdout and stderr -> /dev/null
  fd = open("/dev/null", O_RDWR);
  dup(fd);
  dup(fd);

  return 0;
} // end of detachProcess(const char *)


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
  cerr << '\t' << "Coda to Cindy convertion program" << endl << endl;
  cerr << " -C <dir>\t"    << "data directory. default: cwd" << endl;
  cerr << " -R\t\t"        << "receive mode." << endl;
  cerr << " -d\t\t"        << "debug flag." << endl;
  cerr << " -f <file>\t"   << "input  file. default: STDIN" << endl;
  cerr << " -n\t\t"        << "don't fork processes" << endl;
  cerr << " -o <file>\t"   << "output file. default: STDOUT" << endl;
  cerr << " -v\t\t"        << "verbose flag." << endl;
  cerr << " -z\t\t"        << "gzip output." << endl << endl;

  return 0;
} // end of printHelp(char *)


//
// Function name : forkRunDaemon(const char * fifo, int port, int useGzip)
//
// Description   : fort run daemon
// Input         : fifo    = named pipe
//                 port    = port number for data distribution
//                 useGzip = use gzip to compress data ?
// Return        : pid of child process
//

int
forkRunDaemon(const char * fifo, int port, int useGzip)
{
  int pid;

  if ((pid = fork()) < 0) return -1;

  if (!pid) { /* child */
    char * args[7];
    int i;
    size_t slen = 8;

    if (strlen(fifo) >= slen) slen = strlen(fifo)+1;
    for (i=0; i<6; i++) args[i] = new char[slen];

    strcpy(args[0], "rund");
    strcpy(args[1], "-f");
    strcpy(args[2], fifo);
    strcpy(args[3], "-p");
    sprintf(args[4], "%d", port);
    strcpy(args[5], "-z");
    if (!useGzip) args[5] = NULL;
    args[6] = NULL;
    if (execvp(args[0], args)) exit (errno);
  }

  return pid;
} // end of forkRunDaemon(const char *, int, int)


//
// Function name : main(int argc, char *argv[])
//
// Description   : main function - create HallAfile object
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
  char * Cdir = NULL;
  int rundpid = -1;

  // parse command line options
  while (EOF != (opt = getopt(argc, argv, "C:Rdf:hno:vz?"))) {
    switch (opt) {
    case 'C':
      Cdir   = optarg;
      break;
    case 'R':
      flags |= specialFlag;
      break;
    case 'd':
      flags |= debugFlag;
      break;
    case 'f':
      input  = optarg;
      break;
    case 'n':
      flags |= noForkFlag;
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

  if (input && (*input == ':')) {
    // check if Q directory exists and is writable, then change dir
    if (!Cdir) {
      Cdir = new char[MAXPATHLEN];
      getcwd(Cdir, MAXPATHLEN);
    }
    if (access(Cdir, R_OK|W_OK|X_OK)) {
      perror(argv[0]);
      return errno;
    }
    if (chdir(Cdir)) {
      perror(argv[0]);
      return errno;
    }

    if (flags & specialFlag) output = NULL;

    if (!(flags & noForkFlag)) {
      detachProcess(*argv);
      if (output && *output != '=') output = "=cindy";
    }

  }

  int ctflags = flags;
  if (!(flags & noForkFlag) && !(flags & specialFlag) && (*output == '=')) {
    // create named pipe (fifo)
    remove(output);
    if (mkfifo(output, S_IWUSR|S_IRUSR)) {
      perror(argv[0]);
      return errno;
    }
    sleep(1);
    rundpid = forkRunDaemon(output,
			    (input && (*input==':')) ? atoi(input+1)+1 : 8000,
			    ctflags & zipFlag);
    ctflags &= ~zipFlag;
  }

  // parse input parameter (if any)
  int fd = 0;
  if (input) {
    switch (*input) {
    case '-': // use STDIN
      fd = 0;
      break;
    case ':': // use TCP/IP
      fd = createSocket(atoi(input+1));

      while ( !gotSignal ) {
	fd_set fds;
	struct timeval tv;
	int rval;

	FD_ZERO(&fds);
	FD_SET(fd, &fds);
	tv.tv_sec  = 1;
	tv.tv_usec = 0;

	rval = select(32, (FDSETPTR)&fds, NULL, NULL, &tv);
	if (rval < 0) {
	  if (errno == EINTR) continue;
	  perror(*argv);
	  return errno;
	}
	if (rval == 0) continue;

	int childpid = 0;
	if (!(flags & noForkFlag) && ((childpid = fork()) < 0)) {
	  perror(*argv);
	  return errno;
	}
	if (childpid) {
	  int status;
	  wait(&status);
	} else {
	  struct sockaddr_in nsdesc;
	  int sdesc;
	  unsigned int ssiz = sizeof(nsdesc);
	  while ((sdesc = accept(fd, (struct sockaddr *)&nsdesc, &ssiz)) < 0);
	  fd = sdesc;
	  rundpid = -1;
	  break;
	}
      } // while ( !gotSignal )
      if (gotSignal && (rundpid > 0)) {
	kill(rundpid, SIGTERM);
	remove(output);
      }
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
    ct->putOpt(ctflags, NULL, output);
  } else {
    perror(argv[0]);
    return errno;
  }

  //
  // MAIN LOOP : read all Coda records and convert data
  //
  int copyrun = donotCopyRun;
  if (input && (*input == ':')) {
    copyrun = (flags & zipFlag) ?
      ((flags & specialFlag) ? linkRunCompressed   : copyRunCompressed) :
      ((flags & specialFlag) ? linkRunUncompressed : copyRunUncompressed) ;
  }

  HallAfile hallAfile(ct, fd, copyrun);
  while (!gotSignal && (!hallAfile.nextRecord())) {
    if (!(flags & specialFlag)) hallAfile.handle();
  }

  // clean up
  if (input && (*input != '-')) close(fd);
  if (ct) ct->closeIO();
  sleep(1);
  if (rundpid > 0) {
    kill(rundpid, SIGTERM);
    remove(output);
  }

  return 0;
} // end of main()
