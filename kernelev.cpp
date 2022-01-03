/*
 * kernelev.cpp
 *
 *  Created on: Aug 25, 2020
 *      Author: OS1
 */


#include "kernelev.h"
#include "ivtentry.h"


void KernelEv::set(IVTNo ivt){
	entry=((IVTEntry*)IVTEntry::getIVTEntry(ivt));
	entry->setKernelev(this);
}


KernelEv::~KernelEv(){

			signal();
			IVTEntry::entry_addres[ivtNo]->setKernelev(0);

}
