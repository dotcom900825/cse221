#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

const off_t BLOCKSIZE = 4*1024;
off_t FILESIZE;

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}

double avg_seq_time(char *file, void* buf);

double avg_random_time(char *file, void* buf);

int main(int argc, const char *argv[]) //input the filesize and name of file
{
    FILESIZE = atoll(argv[1]);
    srand((unsigned int)time(NULL));
    void *buf = malloc(BLOCKSIZE);
    double seq_ans = avg_seq_time(argv[2], buf);
    double ran_ans = avg_random_time(argv[2], buf);
    free(buf);
    printf("%.2lf %.2lf\n", seq_ans, ran_ans); // output 21 bits after point
    return 0;
}

double avg_seq_time(char *file, void* buf)
{
    int fd = open(file, O_SYNC);
    if(fcntl(fd, F_NOCACHE, 1) == -1) {
        printf("Failed.\n"); //cannot disable cache
    }
    uint64_t st;
    uint64_t ed;
    uint64_t total_time;
    int count = 0;

    while (1) {
        st = rdtsc();
        ssize_t bytes = read(fd, buf, BLOCKSIZE); // return #byte when read successfully
        if (bytes <= 0) {
            break;
        }
        ed = rdtsc();
        total_time += ed - st;
    }
    close(fd);
    double num = FILESIZE / BLOCKSIZE;
    return total_time / num / 2.6 / 1000;
}

double avg_random_time(char *file, void* buf)
{
    int i = 0;
    int fd = open(file, O_SYNC); //open synchronously
    if(fcntl(fd, F_NOCACHE, 1) == -1) //disable cache
    {
        printf("Failed.\n"); //cannot disable cache
    }
    off_t num = FILESIZE / BLOCKSIZE;
   
    uint64_t st;
    uint64_t ed;
    uint64_t total_time = 0;
    int count = 0;

    for (i = 0; i < num; i++) {
        off_t k = rand() % num;
        st = rdtsc();
        lseek(fd, k * BLOCKSIZE, SEEK_SET); // offset
        read(fd, buf, BLOCKSIZE);
        ed = rdtsc();
        total_time += ed - st;
    }
    close(fd);
    return total_time / (double)num / 2.6 / 1000;
}