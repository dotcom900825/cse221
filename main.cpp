/**************************
 * CSE 240 Course Project 
 *
 * Xia, Yu      A53041213
 * Xie, Chenfu  A53052342
 *
 * 10 / 19 / 2014
 **************************/

#include "CPU.h"
using namespace std;

int main() {
	class CPU newCPU;
	fstream file;

	cout << "Start testing..." << endl;

	newCPU.testReadOverhead(file);

	newCPU.testLoopOverhead(file);
	
	newCPU.testProcedureCallOverhead(file);
	
	newCPU.testSystemCallOverhead(file);
	
	newCPU.testThreadCreationOverhead(file);
	
	newCPU.testProcessCreationOverhead(file);
	
	newCPU.testContextSwitchOverhead(file);

	newCPU.testThreadSwitchOverhead(file);
	
	cout << "Complete...!" << endl;
	return 0;
}
//g++ -pthread -o main main.c -lm