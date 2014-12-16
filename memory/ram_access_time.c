#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <mach/mach_time.h>

#define COUNT 10000
#define TIMES 1000
#define NANOS_PER_SECF 1000000000.0
//Size of memory region that we will accessed, ranging from 4KB to 512MB
int sizeList[18] = {4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288};
//The step length of KB
int strideList[7] = {4, 64, 128, 1024, 1048576, 4194304, 16777216};

static mach_timebase_info_data_t info;
static void __attribute__((constructor)) init_info() {
	mach_timebase_info(&info);
}
static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}

double monotonic_time();
double cacheAccessTime(int size, int strideLength);
void fixedStride(int strideIndex, FILE *file, int times);

int main(int argc, const char * argv[])
{
	FILE *file;

	file = fopen("prob_128_loop_special", "w");
    srand((unsigned int)time(0));
	fixedStride(2, file, TIMES);
    fclose(file);
    return 0;
}

double monotonic_time() {
  uint64_t time = mach_absolute_time();
  double dtime = (double) time;
  dtime *= (double) info.numer;
  dtime /= (double) info.denom;
  return dtime / NANOS_PER_SECF;
}

double cacheAccessTime(int size, int strideLength)
{
    int* A;
    A = (int *)malloc(size * 1024 / 4 * sizeof(int));
    int num, length;
    length = strideLength / 4;   
    num = size * 1024 / 4;
    int index;
    A[0] = 0;
    for (int i = 0; i < num; i++) {
        index = i + length;
        if (index >= num) {
            index %= num;
        }
        A[i] = index;
    }

    int x = 0;
    int64_t start;
    int64_t end;
    int64_t total_time = 0;
    int count = COUNT;

    start = rdtsc();
    for (int i = 0; i < count; i++) {
        x = A[x];
    }
    end = rdtsc();
    total_time = end - start;
    double ans = (total_time - 106) / count - 6;
    free(A);
    return ans;
}

void fixedStride(int strideIndex, FILE *file, int ts)
{
	for (int i = 0; i < ts; i++) {
		for (int j = 0; j < 18; i++) {
			double averageTime = cacheAccessTime(sizeList[j], strideList[strideIndex]);
			fprintf(file, "%lf ", averageTime);
		}
		fprintf(file, "\n");
	}
	return ;
}