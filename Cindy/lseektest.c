#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BSIZ 10240

int
main(int argc,char *argv[])
{
    char buf[BSIZ];
    int fd, ls;

    if (argc < 2) {
	fprintf(stderr, "usage: %s filename\n", argv[0]);
	return(0);
    }

    if ((fd = open(argv[1], O_RDONLY)) == -1) {
	perror("open");
	return(0);
    }

    if (read(fd,buf,BSIZ) != BSIZ) {
	perror("read");
	return(0);
    } 
    printf("%s\n",buf);

for (ls=0;ls<1000;ls++)
    if (read(fd,buf,BSIZ) != BSIZ) {
	perror("read");
	return(0);
    }

    if ((ls=lseek(fd,0,SEEK_SET)) == -1) {
	perror("lseek");
	return(0);
    }

    printf("%s: lseek returned %d\n", argv[0], ls);

    if (read(fd,buf,BSIZ) != BSIZ) {
	perror("read");
	return(0);
    } 
    printf("%s\n",buf);

    if (close(fd) == -1) {
	perror("close");
	return(0);
    }

    return(0);
}
