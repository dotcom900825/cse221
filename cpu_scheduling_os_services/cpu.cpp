#include "CPU.h"
#include "util.h"

void* runThread(void *) {
  pthread_exit(NULL);
}

double CPU::getReadOverhead() {
  double sum = 0;
  uint64_t start, end;
  rdtsc();
  for (int i = 0; i < TIMES; i++) {
    start = rdtsc();
    end = rdtsc();
    sum = sum + (end - start);
  }

  return ((double)sum) / ((double)(TIMES));
}

double CPU::getLoopOverhead() {
  uint64_t start;
  uint64_t end;

  rdtsc();
  uint64_t total_time = 0;
  start = rdtsc();
  for (int i = 0; i < TIMES; i++) {
  }
  end = rdtsc();
  total_time += end - start;

  return (double)total_time / TIMES;
}

void CPU::getProcedureOverhead(vector<double> &ans) {
  uint64_t start;
  uint64_t end;
  uint64_t s = 0;
  uint64_t total_time = 0;
  rdtsc();

  for (int i = 0; i < TIMES; i++) {
      start = rdtsc();
      test0();
      end = rdtsc();

      total_time += (end - start);
  }
  ans[0] = (double)total_time / (double)TIMES;

  rdtsc();
  total_time = 0;
  for (int i = 0; i < TIMES; i++) {
      start = rdtsc();
      test1(1);
      end = rdtsc();

      total_time += end - start;
  }
  ans[1] = (double)total_time / (double)TIMES;

  rdtsc();
  total_time = 0;
  for (int i = 0; i < TIMES; i++) {
      start = rdtsc();
      test2(1, 2);
      end = rdtsc();

      total_time += end - start;
  }
  ans[2] = (double)total_time / (double)TIMES;

  rdtsc();
    total_time = 0;
    for (int i = 0; i < TIMES; i++) {
        start = rdtsc();
        test3(1, 2, 3);
        end = rdtsc();

        total_time += end - start;
    }
    ans[3] = (double)total_time / (double)TIMES;

    rdtsc();
    total_time = 0;
    for (int i = 0; i < TIMES; i++) {
        start = rdtsc();
        test4(1, 2, 3, 4);
        end = rdtsc();

        total_time += end - start;
    }
    ans[4] = (double)total_time / (double)TIMES;

    rdtsc();
    total_time = 0;
    for (int i = 0; i < TIMES; i++) {
        start = rdtsc();
        test5(1, 2, 3, 4, 5);
        end = rdtsc();

        total_time += end - start;
    }
    ans[5] = (double)total_time / (double)TIMES;

    rdtsc();
    total_time = 0;
    for (int i = 0; i < TIMES; i++) {
        start = rdtsc();
        test6(1, 2, 3, 4, 5, 6);
        end = rdtsc();

        total_time += end - start;
    }
    ans[6] = total_time / (double)TIMES;

    rdtsc();
    total_time = 0;
    for (int i = 0; i < TIMES; i++) {
        start = rdtsc();
        test7(1, 2, 3, 4, 5, 6, 7);
        end = rdtsc();

        total_time += end - start;
    }
    ans[7] = (double)total_time / (double)TIMES;

  return;
}

double CPU::sysCallOverhead() {
  uint64_t start;
  uint64_t end;
  uint64_t total_time = 0;
  
  for (int i = 0; i < TIMES; i++) {
    rdtsc();
    start = rdtsc();
    getppid();
    end = rdtsc();

        total_time += end - start;
    }
  
  return (double)total_time / (double)TIMES;
}

double CPU::threadCreationOverhead() {
  pthread_t td;
  uint64_t start, end, total_time;
  total_time = 0;
  
  for (int i = 0; i < CREAT_TIMES; i++) {
    rdtsc();
    start = rdtsc();

    pthread_create(&td, NULL, runThread, NULL);
    pthread_join(td, NULL);

        end = rdtsc();
    total_time += end - start;
     }
  
  return (double)total_time / (double)CREAT_TIMES;
}

double CPU::processCreationOverhead() {
  uint64_t start;
  uint64_t end;
  uint64_t total_time = 0;
  pid_t pid;
  
  for (int i = 0; i < CREAT_TIMES; i++) {
    rdtsc();
    start = rdtsc();
    pid = fork();
    
    if (pid == 0) {
      exit(1);
    }
    else {
      wait(NULL);
      end = rdtsc();
      total_time = total_time + end - start;
    }
  }
  
  return (double)total_time / (double)CREAT_TIMES;

}

uint64_t CPU::calculateSwitchTime(int *fd) {
  uint64_t start;
  uint64_t end;
  pid_t cpid;
  uint64_t result = 0;

  if ((cpid = fork()) != 0) {
    rdtsc();
    start = rdtsc();

    wait(NULL);
    read(fd[0], (void*)&end, sizeof(uint64_t));
  }
  else {
    end = rdtsc();

    write(fd[1], (void*)&end, sizeof(uint64_t));
    exit(1);
  }
  if(end > start){
    result = end - start;
  }
  return (result);
}

double* CPU::getContextSwitchTime() {
  int fd[2];
  pipe(fd);
  uint64_t s[CREAT_TIMES];
  double sum = 0;
  int i = 0;

  while(i < CREAT_TIMES) {
    uint64_t res = calculateSwitchTime(fd);
    if (res > 0) {
      s[i] = res;
      sum += s[i];
      ++i;
    }
  }

  static double res;
  res = (double)sum / (double)CREAT_TIMES;

  return res;
}

uint64_t CPU::calculateKernelSwitch() {
  uint64_t threadSt;
  uint64_t threadEd;

  pthread_t thread;
  pipe(fd);
  pthread_create(&thread, NULL, foo, NULL);

    rdtsc();

    threadSt = rdtsc();
  pthread_join(thread, NULL);
  read(fd[0], (void*)&threadEd, sizeof(uint64_t));

    return threadEd - threadSt;
}

double* CPU::getKernelSwitchOverhead() {
  uint64_t s[CREAT_TIMES];
  double sum = 0.0;
  double tmp = 0.0;

  int i = 0;
    while (i < CREAT_TIMES) {
    uint64_t res = calculateKernelSwitch();

        if (res > 0) {
      s[i] = res;
      sum += s[i];
      i++;
    }
  }

    static double res[2];
  res[0] = sum / (double)CREAT_TIMES;

    for(i = 0; i < CREAT_TIMES; ++i) {
    tmp += (double)(s[i] - res[0]) * (double)(s[i] - res[0]);
  }

    res[1] = sqrt(tmp / (double)(CREAT_TIMES - 1));
  return res;
}

void CPU::testReadOverhead(fstream &file) {
  cout << "Getting Read Overhead..." << endl;

  file.open("Read_Overhead.txt", ios::out);
    if (file == NULL) {
    cout << "File open failed!" << endl;
    return;
  }
  for (int i = 0; i < 10; i++) {
    file << setiosflags(ios::fixed) << getReadOverhead() << "\n";
  }
  file.close();
}

void CPU::testLoopOverhead(fstream &file) {
  cout << "Getting Loop Overhead..." << endl;

    file.open("Loop_Overhead.txt", ios::out);
  if (file == NULL) {
    cout << "File open failed!" << endl;
    return;
  }
  for (int i = 0; i < 10; i++) {
    file << setiosflags(ios::fixed) << getLoopOverhead() << "\n";
  }
  file.close();
}

void CPU::testProcedureCallOverhead(fstream &file) {
  cout << "Getting Procedure Call Overhead..." << endl;
  file.open("Procedure_Call_Overhead.txt", ios::out);
  if (file == NULL) {
    cout << "File open failed!" << endl;
    return;
  }
  for (int i = 0; i < 10; i++) {
    vector<double> result(8, 0.0);
    getProcedureOverhead(result);

    file << setiosflags(ios::fixed)
       << result[0] << " "
         << result[1] << " "
       << result[2] << " "
       << result[3] << " "
       << result[4] << " "
       << result[5] << " "
       << result[6] << " "
       << result[7] << "\n";
    double increment = (result[7] + result[6] + result[5] + result[4] - result[3] - result[2] - result[1] - result[0]) / 16;
        file << setiosflags(ios::fixed) << increment << "\n";
  }
  file.close();
}

void CPU::testSystemCallOverhead(fstream &file) {
  cout << "Getting System Call Overhead..." << endl;
    file.open("System_Call_Overhead.txt", ios::out);
  if (file == NULL) {
    cout << "File open failed!" << endl;
    return;
  }
    for (int i = 0; i < 10; i++) {
    file << setiosflags(ios::fixed) << sysCallOverhead() << "\n";
  }
  file.close();
}

void CPU::testThreadCreationOverhead(fstream &file) {
  cout << "Getting Thread Creation Overhead..." << endl;
  file.open("Thread_Creation_Overhead.txt", ios::out);
  if (file == NULL) {
    cout << "File open failed!" << endl;
    return;
  }
  for (int i = 0; i < 10; i++) {
    file << setiosflags(ios::fixed) << threadCreationOverhead() << "\n";
  }
  file.close();
}

void CPU::testProcessCreationOverhead(fstream &file) {
  cout << "Getting Process Creation Overhead..." << endl;
  file.open("Process_Creation_Overhead.txt", ios::out);
  if (file == NULL) {
    cout << "File open failed!" << endl;
    return;
  }
  for (int i = 0; i < 10; i++) {
    file << setiosflags(ios::fixed) << processCreationOverhead() << "\n";
  }
  file.close();
}

void CPU::testContextSwitchOverhead(fstream &file) {
  cout << "Getting Context Switch Overhead..." << endl;
    file.open("Context_Switch_Overhead.txt", ios::out);
  if (file == NULL) {
    cout << "File open failed!" << endl;
    return;
  }
  for (int i = 0; i < 10; i++) {
    double contextSwitchAvg = getContextSwitchTime();

    file << setiosflags(ios::fixed)
       << contextSwitchAvg[0] << "\n";
  }
  file.close();
}

void CPU::testThreadSwitchOverhead(fstream &file) {
  cout << "Getting Thread Switch Overhead..." << endl;
    file.open("Kernel_Switch_Overhead.txt", ios::out);
  if (file == NULL) {
    cout << "File open failed!" << endl;
    return;
  }
  for (int i = 0; i < 10; i++) {
    double* kernelSwitchAvg = getKernelSwitchOverhead();

    file <<  setiosflags(ios::fixed) 
       << kernelSwitchAvg[0] << " " 
       << kernelSwitchAvg[1] << "\n";
  }
  file.close();
}