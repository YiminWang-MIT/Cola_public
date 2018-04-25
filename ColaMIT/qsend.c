/*								        -*-c-*-
 * the guy to blame:
 *
 * Michael O. Distler                        mailto:distler@mit.edu
 * MIT, Lab for Nuclear Science, 26-402B     tel.   (617) 253-6997
 * Cambridge, MA 02139                       fax    (617) 258-5440
 *
 * $Header: /tmp/cvsroot/Cola/ColaMIT/qsend.c,v 2.1 2002-03-05 03:49:50 distler Exp $
 *
 * send Q files from VAX to PC
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef VAXC
#include <stat.h>
#define W_OK       2
#define R_OK       4
#define O_RDONLY   00
#define O_WRONLY   01
#define S_IWUSR    00200
#define S_IRUSR    00400
#define func(name) name
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#if defined(__linux) || defined(__sparc)
#define func(name) name ## __
#else
#define func(name) name ## _
#endif
#endif

int func(tcpip_open)();
int func(tcpip_write)();
int func(tcpip_close)();

#define BUFLEN (32*1024)

int
closeAll(int *tcpfd, int mt1, int mt2)
{
  if (*tcpfd > 0) func(tcpip_close)(tcpfd);
  if (mt1 > 0) close(mt1);
  if (mt2 > 0) close(mt2);
  return 0;
}

int
main(int argc, char *argv[])
{
  int port, fd, tcpfd, mt1, mt2, i, towrite;
  char *host, *colon, buffer[BUFLEN];
  struct stat status;

#ifdef VAXC
  unsigned short target[4];
#endif

  if (argc < 3) {
    fprintf(stderr, "Usage: %s filename [host:port] [device:]\n", argv[0]);
    return 1;
  }

  if (access(argv[1], R_OK) || (0 > (fd=open(argv[1],O_RDONLY,S_IRUSR)))) {
    fprintf(stderr, "ERROR: can't read '%s'\n", argv[1]);
    return 2;
  }

  if (fstat(fd, &status)) {
    fprintf(stderr, "ERROR: can't get file status of '%s'\n", argv[1]);
    return 3;
  }

  if (status.st_size < 18) {
    fprintf(stderr, "ERROR: file size (%ld) to small - '%s' NOT sent\n",
	    status.st_size, argv[1]);
    return 4;
  }

  tcpfd = mt1 = mt2 = -1;
  for (i=2; i<argc; i++) {
    if (!(colon = strchr(argv[i], ':'))) {
      fprintf(stderr, "Usage: %s filename [host:port] [device:]\n", argv[0]);
      return 5;
    }

    if (colon[1] == 0) { /* open tape device */
      int mt;

      if (access(argv[i], W_OK) || (0 > (mt=open(argv[i],O_WRONLY,S_IWUSR)))) {
	fprintf(stderr, "ERROR: can't open magtape '%s'\n", argv[i]);
	return 6;
      }

      if (mt1 == -1) {
	mt1 = mt;
      } else {
	if (mt2 == -1) {
	  mt2 = mt;
	}
      }

    } else { /* open tcpip connection */

      host = malloc(colon-argv[2]+1);
      strncpy(host, argv[2], colon-argv[2]);
      host[colon-argv[2]] = 0;
      port = atoi(colon+1);

#ifdef VAXC
      ((size_t *)target)[0] = strlen(host);
      ((char **)target)[1] = host;
      tcpfd = func(tcpip_open)(target, &port);
#else
      tcpfd = func(tcpip_open)(host, &port, strlen(host));
#endif

      if (tcpfd < 0) {
	fprintf(stderr, "ERROR: can't open tcpip connection\n");
	return 7;
      }
    }
  }

  do {
    towrite = read(fd, buffer, BUFLEN);
    if (tcpfd > 0) {
      if (0 > func(tcpip_write)(&tcpfd, buffer, &towrite)) {
	fprintf(stderr, "ERROR: tcpip write failed\n");
	closeAll(&tcpfd, mt1, mt2);
	return 8;
      }
    }
    if (mt1 > 0) {
      if (towrite != write(mt1, buffer, towrite)) {
	fprintf(stderr, "ERROR: magtape 1 write failed\n");
	closeAll(&tcpfd, mt1, mt2);
	return 9;
      }
    }
    if (mt2 > 0) {
      if (towrite != write(mt2, buffer, towrite)) {
	fprintf(stderr, "ERROR: magtape 2 write failed\n");
	closeAll(&tcpfd, mt1, mt2);
	return 10;
      }
    }

  } while (towrite > 0);

  closeAll(&tcpfd, mt1, mt2);

  return 0;
}
