#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#define BSIZ 65536
#define FNAM "mmapfile"

int
main()
{
    char buf[BSIZ];
    unsigned short *mm;
    int fd, i;

    if ((fd = creat(FNAM, 0644)) == -1) {
	perror("creat");
	return(0);
    }
    if (write(fd,buf,BSIZ) != BSIZ) {
	perror("write");
	return(0);
    }
    if (close(fd) == -1) {
	perror("close");
	return(0);
    }

    if ((fd = open(FNAM, O_RDWR)) == -1) {
	perror("open");
	return(0);
    }
    if ((mm = (unsigned short *) mmap(NULL, BSIZ, PROT_READ | PROT_WRITE,
			    MAP_FILE | MAP_SHARED, fd, 0))
	== (unsigned short *) -1) {
	perror("mmap");
	return(0);
    }

    while (1) {

	for (i=0;i<BSIZ/sizeof(unsigned short);i++) mm[i] = (~i & 0xffff);
	sleep(5);

	for (i=0;i<BSIZ/sizeof(unsigned short);i++) mm[i] = 0;
	sleep(5);

	for (i=0;i<BSIZ/sizeof(unsigned short);i++) mm[i] = (i & 0xffff);
	sleep(5);

	for (i=0;i<BSIZ/sizeof(unsigned short);i++) mm[i] = 0;
	sleep(5);
    }

    return(0);
}
