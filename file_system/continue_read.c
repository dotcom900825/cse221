#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

const off_t BLOCKSIZE = 4*1024;
const off_t FILESIZE = 64*1024*1024;


void seqReadingFileProc(int fd, void* buf)
{
    while (1) {
        while (read(fd, buf, BLOCKSIZE)) {}
        lseek(fd, 0, SEEK_SET);
    }
}


void ranReadingFileProc(int fd, void* buf)
{
    srand((unsigned int)time(NULL));
    int num = FILESIZE / BLOCKSIZE;
    int i;
    while (1) {
        for (i = 0; i < num; i++) {
            int index = rand() % num;
            lseek(fd, index * BLOCKSIZE, SEEK_SET);
            read(fd, buf, BLOCKSIZE);
        }
    }
}


int main(int argc, char** argv)
{
    void *buf = malloc(BLOCKSIZE);
    int fd = open(argv[1], O_SYNC);
    if(fcntl(fd, F_NOCACHE, 1) == -1) {
        printf("Failed to disable cache.\n");
    }
    uint64_t st, ed;
    if (argv[2][0] == '0') {
        seqReadingFileProc(fd, buf);
    }
    else {
        ranReadingFileProc(fd, buf);
    }
}
