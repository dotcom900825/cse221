#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include <math.h>
#include <mach/mach_time.h>

#define Offset 3*pow(2,10)
#define loops 10000
#define	counts 1000
#define Memory 26
#define NANOS_PER_SECF 1000000000.0

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}

static mach_timebase_info_data_t info;

static void __attribute__((constructor)) init_info() {
  mach_timebase_info(&info);
}

double monotonic_time();
double wirteTime(int *A, int counter);
double readTime(int *A, int counter);

int main(int argc, char const *argv[])
{
	int power = Memory;
	int counter = 1;
	for(int k = 0; k < power; k++) {
		counter = counter * 2;
	}
	int *A = (int *)malloc(counter * sizeof(int));
	for (int j = 0; j < counter; ++j)
	{
		A[j] = 0;
	}

	double wtTime;
	double rdTime;
	int rwTimes = counts;

	FILE *file1, *file2;
	file1 = fopen("RAM_wr_bandwidth","w+");
	file2 = fopen("RAM_rd_bandwidth","w+");
	for (int i = 0; i < rwTimes; ++i){
		wtTime = wirteTime(A, counter);
		rdTime = readTime(A, counter);
		fprintf(file1, "%f\n", wtTime);
		fprintf(file2, "%f\n", rdTime);
	}

	fclose(file1);
	fclose(file2);
	return 0;
}

double monotonic_time() {
  uint64_t time = mach_absolute_time();
  double dtime = (double) time;
  dtime *= (double) info.numer;
  dtime /= (double) info.denom;
  return dtime / NANOS_PER_SECF;
}

double wirteTime(int* A, int counter) {
	double start = 0;
	double end = 0;
	int i = 0;
	int j = 0;
	int ofset = Offset;

	double total_time = 0;
	start = monotonic_time();
	while(j < loops)
	{
		j++;
		for (int k = 0; k < counter; k++){
			A[k] = 0;
		}
		
	}
	end = monotonic_time();
	total_time = end - start;

	double cost = 4 * counter * loops;
	return (cost / total_time);
}

double readTime(int *A, int counter) {
	uint64_t start = 0;
	uint64_t end = 0;
	int a = 0;
	int i = 0, j = 0;
	int ofset = Offset;

	int total_time = 0;
	while (j < loops)
	{
		start = rdtsc();
		j++;
		i = i + ofset;
		i = i % counter;
		for (int k = 0; k < 1000; k++) {
			a = A[i+k];
		}
		end = rdtsc();
		total_time += end - start;
	}
	double timecost = total_time - (j+1)*29;
	double cost = 4 * 1000 * (j+1) / pow(2, 20);
	return (cost / (timecost * 0.56 * pow(10,-9)));
}