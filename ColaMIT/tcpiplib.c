/*								        -*-c-*-
 * the guy to blame:
 *
 * Michael O. Distler                        mailto:distler@mitlns.mit.edu
 * MIT, Lab for Nuclear Science, 26-456      tel.   (617) 253-6997
 * Cambridge, MA 02139                       fax    (617) 258-5440
 *
 * $Header: /tmp/cvsroot/Cola/ColaMIT/tcpiplib.c,v 2.1 2002-03-05 03:49:54 distler Exp $
 *
 * TCP/IP library provides functions to access a unix server
 */

#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <time.h>
#include <stdlib.h>
#ifdef VAXC
#include <in.h>
#include <types.h>
#include <stat.h>
#include <socket.h>
#include <unixio.h>
#include <file.h>
#define MAXHOSTNAMELEN  64
#ifdef VAXCPREFIX
#define connect         vaxc$connect
#define gethostbyname   vaxc$gethostbyname
#define gethostname     vaxc$gethostname
#define htons           vaxc$htons
#define ntohl           vaxc$ntohl
#define socket          vaxc$socket
#endif
#define func(name) name
#else
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#if defined(__linux) || defined(__sparc)
#define func(name) name ## __
#else
#define func(name) name ## _
#endif
#endif
#include <errno.h>


/*
 * Function name : tcpip_open(const char * inetaddr,
 *                            const int * port,
 *                            long len)
 *
 * Description   : open a TCP/IP stream connection to the specified host/port
 *                 this function is called from a fortran program
 *                 some linker require the name to end with __
 *                 parameters are allways called by reference
 *                 for strings their length is also pushed to the parameter
 *                 stack and shows up as the last parameter
 * Input         : inetaddr = target internet address
 *                 port     = port number
 *                 len      = length of address string
 * Return        : socket file descriptor
 *
 * FORTRAN Example       INTEGER*4 FD
 *                       FD=TCPIP_OPEN('mod.mit.edu', 7999) 
 */

int
func(tcpip_open)
#ifdef VAXC
(const unsigned short * vaxstr, const int * port)
#else
(const char * inetaddr, const int * port, long len)
#endif
{
    struct sockaddr_in outsockaddr;
    struct hostent * targethost;
    char * target;
    int outsock;
#ifdef VAXC
    /* VAX FORTRAN passes strings in a very strange way */ 
    char * inetaddr = ((char **)vaxstr)[1];
    unsigned short len = *vaxstr;

    while ((len > 0) && (inetaddr[len-1] == ' ')) len--;
#endif /* VAXC */

    target = malloc(len+1);
    memcpy(target, inetaddr, len);
    target[len] = 0;

    if (NULL == (targethost = gethostbyname(target))) {
	free(target);
	perror("gethostbyname");
	return -1;
    } else free(target);

    /* create an endpoint for communication */
    if ((outsock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
	perror("socket");
	return -1;
    }

    /* initiate a connection on the socket */
    memset(outsockaddr.sin_zero, 0, 8);
    outsockaddr.sin_family = targethost->h_addrtype;
    outsockaddr.sin_port   = htons(*port);
    memcpy(&outsockaddr.sin_addr, targethost->h_addr, targethost->h_length);
    if (connect(outsock,(struct sockaddr *)&outsockaddr,sizeof(outsockaddr))) {
	perror("connect");
	return -1;
    }

    /* return socket file descriptor */
    return outsock;
} /* end of tcpip_open(const char *, const int *, long) */


/*
 * Function name : tcpip_write(const int * fd,
 *                             const char * buf,
 *                             const size_t * len)
 *
 * Description   : write to a file descriptor
 *                 the corresponding TCP/IP connection has to be
 *                 opened in a previous tcpip_open() call
 * Input         : fd  = file descriptor
 *                 buf = data buffer
 *                 len = number of bytes to transfer
 * Return        : On success, the number of bytes written are returned.
 *                 On error, -1 is returned.
 */

size_t
func(tcpip_write)(const int * fd, const char * buf, const size_t * len)
{
    size_t total, written, count;

    if (*fd < 0) return -1;

    total = 0;
    count = *len;
    while (count > 0) {
	written = write(*fd, buf, count);
	if (written < 0) {
	    perror("write");
	    return -1;
	}
	total += written;
	buf   += written;
	count -= written;
    }

    return total;
} /* end of tcpip_write(const int *, const char *, const size_t *) */


/*
 * Function name : tcpip_close(const int * fd)
 *
 * Description   : close the tcpip connection
 * Input         : fd = file descriptor
 * Return        : zero on success, or -1 if an error occurred
 */

int
func(tcpip_close)(const int * fd)
{
    if (*fd < 0) return -1;

    return close(*fd);
} /* end of tcpip_close(const int *) */


/*
 * Function name : fill_header(unsigned short * shead,
 *                             unsigned short * type, unsigned short * run)
 *
 * Description   : fill the fields of a Q data record
 *                 according to the 'Q - programmer's information manual'
 *                 Appendix B the header of a Q data record has the following
 *                 format:
 *                 .WORD record number on this tape (not changed)
 *                 .WORD date: bits 15-11=day, 10-7=month, 6-0=year-1900
 *                 .BYTE hours
 *                 .BYTE minutes
 *                 .BYTE seconds
 *                 .BYTE ticks
 *                 .BYTE record type
 *                 .BYTE tape format identification
 *                 .WORD run number
 *                 ..... (not changed)
 * Input         : shead = pointer to a Q record header (INTEGER*2 ARRAY)
 * Return        : -1 on error
 */

int
func(fill_header)(unsigned short * shead,
		unsigned short * type, unsigned short * run)
{
  time_t now;
  struct tm * tmptr;
  unsigned char  * chead = (unsigned char  *)shead;

  time(&now);
  tmptr = localtime(&now);

  shead[1] = (tmptr->tm_mday << 11)
    + ((tmptr->tm_mon+1) << 7) + tmptr->tm_year;
  chead[4] = tmptr->tm_hour;
  chead[5] = tmptr->tm_min;
  chead[6] = tmptr->tm_sec;
  chead[7] = 0;
  chead[8] = *type;
  chead[9] = 1;
  shead[5] = *run;

  return 0;
} /* end of fill_header(unsigned short *,unsigned short *,unsigned short *) */


#ifdef LIB_TEST

/*
 * Function name : main(int argc, char *argv[])
 *
 * Description   : main function to test functions
 * Input         : argc =
 *                 argv =
 * Return        : zero on success, or -1 if an error occurred
 */

#define BUFLEN 4096

int
main(int argc, char *argv[])
{
    size_t len;
    int sfd, fd, port = 7999;
    char buffer[BUFLEN];
#ifdef VAXC
    /* VAX FORTRAN passes strings in a very strange way */ 
    unsigned short target[4];
    ((size_t *)target)[0] = strlen(argv[1]);
    ((char **)target)[1] = argv[1];
#endif

    if (argc != 3) {
	fprintf(stderr, "usage : %s <target> <file>\n", *argv);
	return -1;
    }

#ifdef VAXC
    if ((sfd = func(tcpip_open)(target, &port)) < 0) {
#else
    if ((sfd = func(tcpip_open)(argv[1], &port, strlen(argv[1]))) < 0) {
#endif
	return -1;
    }

    fd = -1;
    if (!strcmp(argv[2], "-")) {
      fd = 0;
    } else if ((fd = open(argv[2], O_RDONLY, 00400)) < 0) {
	perror(*argv);
	return -1;
    }

    while ( 1 ) {
	len = read(fd, buffer, BUFLEN);
	if (len <= 0) break;
	if (0 > func(tcpip_write)(&sfd, buffer, &len)) break;
    }

    func(tcpip_close)(&sfd);
    if (fd > 0) close(fd);

    return 0;
} /* end of main(int, char **) */

#endif
