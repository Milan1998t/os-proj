/*
 * kernel.h
 *
 *  Created on: Aug 16, 2020
 *      Author: OS1
 */

#ifndef KERNEL_H_
#define KERNEL_H_

#include "pcblist.h"
//#include "semlist.h"
#define oldrut 0x08
#define newrut 0x60

typedef void interrupt (*pInterrupt)(...);
void interrupt timer(...);

extern void tick();

#define lock Kernel::lockFlag = 0;
#define unlock { Kernel::lockFlag = 1; if (Kernel::flipSwitch) dispatch(); }
class SemList;

class Kernel {

public:
	static volatile int quant;
	static volatile unsigned lockFlag; // Unlocked

	static PCBList *threads;
	static SemList*sems;

	static Thread  *dummyThread,*mainThread;

	static pInterrupt oldRoutine;


	static volatile int flipSwitch;

	static void start();
	static void end();
	static void setTimerRoutine();

};



#endif /* KERNEL_H_ */
