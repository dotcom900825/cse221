#ifndef _UTIL_H
#define _UTIL_H

#define TIMES 10000
#define CREAT_TIMES 500

static int fd[2];

static inline uint64_t rdtsc(void) {
	uint32_t lo, hi;
	__asm__ __volatile__("rdtsc;": "=a" (lo), "=d" (hi));
	return (((uint64_t)hi << 32) | lo);
}

static inline void test0(){}

static inline void test1(int a1){}

static inline void test2(int a1, int a2){}

static inline void test3(int a1, int a2, int a3){}

static inline void test4(int a1, int a2, int a3, int a4){}

static inline void test5(int a1, int a2, int a3, int a4, int a5){}

static inline void test6(int a1, int a2, int a3, int a4, int a5, int a6){}

static inline void test7(int a1, int a2, int a3, int a4, int a5, int a6, int a7){}

static inline void *foo(void *) {
	uint64_t t = rdtsc();

    write(fd[1], (void*)&t, sizeof(uint64_t));

    pthread_exit(NULL);
}

#endif