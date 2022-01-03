/*
 * ivtentry.cpp
 *
 *  Created on: Aug 25, 2020
 *      Author: OS1
 */



#include "ivtentry.h"

IVTEntry* IVTEntry::entry_addres[256] = {0};

void IVTEntry::setKernelev(KernelEv* k){
		kernel=k;
	}

/*
IVTEntry::~IVTEntry(){

			signal();
			IVTEntry::entry_addres[ivtNo]->setKernelev(0);

}
*/
