#ifndef _CPU_H
#define _CPU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/types.h>#include <mach/mach_time.h>
#include <mach/mach.h>#include <iostream>#include <fstream>#include <iomanip>#include <vector>#include "util.h"using namespace std;class CPU {private:	double getReadOverhead();	double getLoopOverhead();	void getProcedureOverhead(vector<double> &);	double sysCallOverhead();
	double threadCreationOverhead();	double processCreationOverhead();	uint64_t calculateSwitchTime(int *);	double* getContextSwitchTime();	uint64_t calculateKernelSwitch();	double* getKernelSwitchOverhead();public:	void testReadOverhead(fstream &);	void testLoopOverhead(fstream &);	void testProcedureCallOverhead(fstream &);	void testSystemCallOverhead(fstream &);	void testThreadCreationOverhead(fstream &);	void testProcessCreationOverhead(fstream &);	void testContextSwitchOverhead(fstream &);	void testThreadSwitchOverhead(fstream &);};#endif