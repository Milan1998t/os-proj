/*
 * ivtentry.h
 *
 *  Created on: Aug 25, 2020
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_
#include "kernelev.h"

#define PREPAREENTRY(ivtNo,old)\
void interrupt intr##ivtNo(...);\
IVTEntry entry##ivtNo(ivtNo,intr##ivtNo);\
void interrupt intr##ivtNo(...) {\
	if (old) entry##ivtNo.callOld();\
	entry##ivtNo.signal();\
}

typedef void interrupt (*pInterrupt)(...);

//class KernelEv;

class IVTEntry{

public:
	static IVTEntry *entry_addres[256];
	pInterrupt oldRoutine, newRoutine;
	KernelEv* kernel;
	int Num;


	IVTEntry(int no,pInterrupt new_interrupt){
		Num=no;
		oldRoutine = getvect(no);
		kernel=0;
		//newRoutine=setvect(no, new_interrupt);

		setvect(no, new_interrupt);
		entry_addres[no]=this;


	}

	~IVTEntry(){
		//koji treba ispod drugog
		setvect(Num,oldRoutine);
		entry_addres[Num]=0;

		callOld();
	}

	void IVTEntry::callOld() {
		(*oldRoutine)();
	}

	static IVTEntry* getIVTEntry(IVTNo num)	{
		return entry_addres[num];
	}


	void setKernelev(KernelEv* k);

	void signal(){
		if(!kernel) return;
		kernel->signal();
	}



};




#endif /* IVTENTRY_H_ */
