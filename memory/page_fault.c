#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}

int main()
{
    int fileDescriptor = open("random", O_RDWR);
    if (fileDescriptor < 0) {
        printf("Open failed\n");
        return -1;
    }
    unsigned int offset = 16777216; //16 MB
    unsigned int FILESIZE = 3435973836; //2.8 GB
    char* map =(char*) mmap(NULL, FILESIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
    
    // Measure start time here
    char c;
    unsigned int i;
    uint64_t start;
    uint64_t end;
    uint64_t total_time = 0;

    start = rdtsc();
    for (i = 0; i < 100; i++)
    {   
        c = map[(((i+1) * offset) % (FILESIZE - 1))]; 
    }
    end = rdtsc();
    total_time = end - start;
    
    double averageTime = (double)(total_time - 106) / 100 - 6;
    printf("AvgTime: %lf\n", averageTime);
    munmap(map, FILESIZE);
    close(fileDescriptor);
    return 0;
}
