#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>

#include <mach/mach_time.h>



#define Offset 3*pow(2,10)
#define LoopTimes 10000
#define	rwCount 1000
#define MemRegion 26
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

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}

static inline double getWtTime(int* array, int counter)
{
	double stTime = 0;
	double edTime = 0;
	int i = 0;
	int j = 0;
	int rpTime = Offset;

	double total_time = 0;
	stTime = monotonic_time();
	while(j < LoopTimes)
	{
		j++;
		int k;
		for(k=0; k<counter; k++){
			array[k] = 0;
		}
		
	}
	edTime = monotonic_time();
	total_time = edTime - stTime;

	double megacost = 4 * counter * LoopTimes;
	return (megacost / total_time);
}

static inline double getRdTime(int *array, int counter)
{
	uint64_t stTime = 0;
	uint64_t edTime = 0;
	int a = 0;
	int i = 0, j = 0;
	int rpTime = Offset;

	int total_time = 0;
	while (j < LoopTimes)
	{
		stTime = rdtsc();
		j++;
		i = i + rpTime;
		i = i % counter;
		int k;
		for(k=0; k<1000; k++)
			a = array[i+k];
		edTime = rdtsc();
		total_time += edTime - stTime;
	}
	double timecost = total_time - (j+1)*29;
	double megacost = 4 * 1000 * (j+1)/ pow(2, 20);
	return (megacost / (timecost * 0.56 * pow(10,-9)));
}

int main(int argc, char const *argv[])
{
	int power = MemRegion;
	int k;
	int counter = 1;
	for(k=0; k<power; k++)
		counter = counter * 2;
	int *array = (int *)malloc(counter*sizeof(int));
	/*intialize*/
  int j;
	for (j = 0; j < counter; ++j)
	{
		array[j] = 0;
	}

	double wtTime;
	double rdTime;
	int rwTimes = rwCount;

	FILE *f1, *f2;
	f1=fopen("RAM_wr_bandwidth","w+");
	f2=fopen("RAM_rd_bandwidth","w+");
	int i;
	for (i = 0; i < rwTimes; ++i){
		wtTime=getWtTime(array,counter);
		printf("%f\n", wtTime);
	}
	for (i = 0; i < rwTimes; ++i){
		rdTime=getRdTime(array,counter);
		fprintf(f2, "%f\n",rdTime);
	}
	fclose(f1);
	fclose(f2);
	return 0;
}
