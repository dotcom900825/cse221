#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <mach/mach_time.h>

static const int COUNT = 10000;
#define ExpTimes 1000

#define NANOS_PER_SECF 1000000000.0


static mach_timebase_info_data_t info;
static void __attribute__((constructor)) init_info() {
  mach_timebase_info(&info);
}

double monotonic_time() {
  uint64_t time = mach_absolute_time();
  double dtime = (double) time;
  dtime *= (double) info.numer;
  dtime /= (double) info.denom;
  return dtime / NANOS_PER_SECF;
}

//Size of memory region that we will accessed, ranging from 4KB to 512MB
int regionSizeList[18] = {4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288};

//The step length of KB
int stepLenList[7] = {4, 64, 128, 1024, 1048576, 4194304, 16777216};

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}

static inline double averageCacheAccessTime(int regionSize, int stepLen)
{
    int* integerArray;
    integerArray = (int *)malloc(regionSize * 1024 / 4 * sizeof(int));
    int intNum, intStepLen;
    intStepLen = stepLen / 4;   
    intNum = regionSize * 1024 / 4;
    int i,index;
    integerArray[0] = 0;
    for (i = 0; i < intNum; i++) {
        index = i + intStepLen;
        if (index >= intNum) {
            index %= intNum;
        }
        integerArray[i] = index;
    }
    for (i = 0; i < intNum; i++) {
        if(integerArray[i]) {
            
        }
    }
    int x = 0;
    int64_t start;
    int64_t end;
    int64_t total_time = 0;
    int count=COUNT;

    start = rdtsc();
    for (i = 0; i < count; i++) {
        x = integerArray[x];
    }
    end = rdtsc();
    total_time = end - start;
    double ans = (total_time - 106) / count - 6;
    free(integerArray);
    return ans;
}

void outputFixedStrideData(int strideIndex, FILE *file)
{

	int i;
    for (i = 0; i < 18; i++) {
        double averageTime = averageCacheAccessTime(regionSizeList[i], stepLenList[strideIndex]);
        //printf("%d KB stride acat of %d KB region size:%lf\n", stepLenList[strideIndex], regionSizeList[i], averageTime);
        fprintf(file, "%lf ", averageTime);
    }
    fprintf(file, "\n");
}


static inline double outputNoStrideData(int regionSize)
{
    int *integerArray = (int *)malloc(regionSize * 1024 * sizeof(char));
    int intNum = 1024 * regionSize / 4;
    int i;
    for (i = 0; i < intNum; i++) {
        integerArray[i] = i;
    }
    int index = rand() % intNum;
    int tmp;
    int64_t start;
    int64_t end;
    int64_t total_time = 0;
    int count=COUNT;
    for (i = 0; i < count; i++) {
        start = rdtsc();
        tmp = integerArray[index];
        end = rdtsc();
        total_time += end - start;
    }
    double ans = total_time / (double)COUNT;
    free(integerArray);
    return ans;
}

void NoStrideData(FILE *file)
{

    int i;
    for (i = 0; i < 18; i++) {
        double averageTime = outputNoStrideData(regionSizeList[i]);
        //printf("%d KB stride acat of %d KB region size:%lf\n", stepLenList[strideIndex], regionSizeList[i], averageTime);
        fprintf(file, "%lf ", averageTime);
    }
    fprintf(file, "\n");
}

int main(int argc, const char * argv[])
{
	FILE *file1,*file2;

	file1=fopen("prob_128_loop_special","w");
    // file2=fopen("prob1e","w+");
    srand((unsigned int)time(0));
    int exptime=ExpTimes;
    int i;
    for (i = 0; i < ExpTimes; ++i)
    {
	    outputFixedStrideData(2,file1);
    	//outputFixedByteStrideData(9);
    	// NoStrideData(file1);
    }
    fclose(file1);
    fclose(file2);
    return 0;
}

