#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mtio.h>
#include <fcntl.h>

int
main(int argc,char *argv[])
{
    struct mtget mt_status;
    struct stat sbuf;
    int mtfd;

    if (argc < 2) {
	fprintf(stderr, "usage: %s filename\n", argv[0]);
	return(0);
    }

    if (stat(argv[1], &sbuf) == -1) {
	perror("stat");
	return(0);
    }

    if (S_ISREG(sbuf.st_mode)) {
	printf("%s is regular file\n", argv[1]);
	return(0);
    }

    if (S_ISCHR(sbuf.st_mode)) {
	printf("%s is character device\n", argv[1]);
	if ((mtfd = open(argv[1], O_RDONLY)) == -1) {
	    perror("open");
	    return(0);
	}
	if (ioctl(mtfd, MTIOCGET, (char *)&mt_status) == -1) {
	    perror("ioctl");
	    return(0);
	}
	if (close(mtfd) == -1) {
	    perror("close");
	    return(0);
	}
    }

    return(0);
}
