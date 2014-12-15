#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

const off_t BLOCKSIZE = 4*1024;

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}

int main(int argc, const char * argv[])
{
    void* buf = malloc(BLOCKSIZE);
	const off_t FILESIZE = atoll(argv[1]);
    int fd = open(argv[2], O_RDONLY | O_SYNC);
//    if(fcntl(fd, F_NOCACHE, 1) == -1) {
//        printf("Failed to disable cache.\n");
//    }
    lseek(fd, FILESIZE - 1, SEEK_SET);
    off_t totalBytes = 0;
    
    while(1){
        lseek(fd, -2*BLOCKSIZE, SEEK_CUR);
        ssize_t bytes = read(fd, buf, BLOCKSIZE);
        if (bytes <= 0 || totalBytes >= FILESIZE)
            break;
        totalBytes += bytes;
    }
    
    close(fd);
    fd = open(argv[2], O_RDONLY| O_SYNC);
    totalBytes = 0;
    lseek(fd, FILESIZE - 1, SEEK_SET);
//    if(fcntl(fd, F_NOCACHE, 1) == -1) {
//        printf("Failed to disable cache.\n");
//    }
    
    uint64_t st;
    uint64_t ed;
    uint64_t total_time = 0;
    while(1){
        lseek(fd, -2*BLOCKSIZE, SEEK_CUR);
        st = rdtsc();
        ssize_t bytes = read(fd, buf, BLOCKSIZE);
        ed = rdtsc();
        total_time += ed - st;
        if (bytes <= 0 || totalBytes >= FILESIZE)
            break;
        totalBytes += bytes;
    }
    
    close(fd);
    free(buf);
    double ans = (double)(total_time / (FILESIZE / BLOCKSIZE)) - 102;
    printf("%lf\n",ans);
    return 0;
}

