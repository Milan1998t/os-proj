/*
 * main.cpp
 *
 *  Created on: Aug 8, 2020
 *      Author: OS1
 */


#include "kernel.h"
#include <stdio.h>

extern int userMain(int argc, char** argv);

int main(int argc, char** argv) {
	Kernel::start();
	userMain(argc, argv);
	Kernel::end();
	return 0;
}



/*
#include "kernel.h"
#include <DOS.H>
#include <STDIO.H>
#include <IOSTREAM.H>
#include <STDARG.H>

class TestThread : public Thread
{
public:

	TestThread(): Thread() {};
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};
int syncPrintf(const char *format, ...)
{
	int res;
	va_list args;
	asm cli;
		va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	asm sti;
		return res;
}

void TestThread::run()
{
	syncPrintf("Thread %d: loop1 starts\n", getId());

	for(int i=0;i<32000;i++)
	{
		for (int j = 0; j < 32000; j++);
	}

	syncPrintf("Thread %d: loop1 ends, dispatch\n",getId());

	dispatch();

	syncPrintf("Thread %d: loop2 starts\n",getId());

	for (int k = 0; k < 20000; k++);

	syncPrintf("Thread %d: loop2 ends\n",getId());

}



void tick(){}


int userMain(int argc, char** argv)
{
	syncPrintf("User main starts\n");
	TestThread t1,t2,t3,t4,t5;
	t1.start();
	t2.start();
	t3.start();
	t4.start();
	t5.start();

	syncPrintf("User main ends\n");
	return 16;
}
int main(int argc, char** argv){
		Kernel::inic();
		int c=userMain(argc, argv);
		cout<<c;
		Kernel::restore();
		return 0;
}

*/
/*
	Test: asinhrono preuzimanje
*/



