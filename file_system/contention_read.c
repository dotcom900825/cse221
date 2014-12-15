#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

off_t FILESIZE;

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__("xor %%eax, %%eax;" "cpuid;" "rdtsc;": "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo);
}

void readFile(char *file, int seq){
  void *buf = malloc(4096);

  char *files[] = {"size64k_0", "size64k_1", "size64k_2", "size64k_3", "size64k_4", "size64k_5", "size64k_6", "size64k_7", "size64k_8", "size64k_9"};

  int fd = open(files[seq], O_RDONLY);

  if(fcntl(fd, F_NOCACHE, 1) == -1) {
     printf("Failed.\n"); //cannot disable cache
  }

  uint64_t st;
  uint64_t ed;
  uint64_t total_time;
  while(1){
    st = rdtsc();
    ssize_t bytes = read(fd, buf, 4096);
    if(bytes <= 0){
      break;
    }
    ed = rdtsc();
    total_time += ed - st;
  }
  close(fd);
  double num = FILESIZE / 4096;
  printf("%.2lf\n", total_time / num / 2.6 / 1000); // output 21 bits after point

}

void readFileRandomly(char *file, int seq){
  void *buf = malloc(4096);
  char *files[] = {"size64k_0", "size64k_1", "size64k_2", "size64k_3", "size64k_4", "size64k_5", "size64k_6", "size64k_7", "size64k_8", "size64k_9"};
  int fd = open(files[seq], O_RDONLY);

  if(fcntl(fd, F_NOCACHE, 1) == -1) {
     printf("Failed.\n"); //cannot disable cache
  }

  off_t num = FILESIZE / 4096;
  off_t *access_list = (off_t*)malloc(num * sizeof(off_t));
  int i;
  for (i = 0; i < num; i++) 
  {
     access_list[i] = rand() % num;
  }

  double st;
  double ed;
  double total_time;

  for (i = 0; i < num; i++) {
    st = rdtsc();
    lseek(fd, access_list[i] * 4096, SEEK_SET); // offset
    read(fd, buf, 4096);
    ed = rdtsc();
    total_time += ed - st;
  }

  close(fd);
  printf("%.2lf\n", total_time / (double)num / 2.6 / 1000); // output 21 bits after point

}

int main(int argc, char const *argv[])
{
  /* code */
  FILESIZE = atoll(argv[1]);
  srand((unsigned int)time(NULL));

  pid_t pids[10];
  int i;
  int n = atoll(argv[3]);
  printf("\n");
  /* Start children. */
  for (i = 0; i < n; ++i) {
    if ((pids[i] = fork()) < 0) {
      perror("fork");
      abort();
    } else if (pids[i] == 0) {
      readFileRandomly(argv[2], i);
      exit(0);
    }else{
      //readFile(argv[2]);
    }
  }

  return 0;
}

