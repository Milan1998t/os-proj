/*
 * kernelev.h
 *
 *  Created on: Aug 25, 2020
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_


#include "SCHEDULE.H"
//#include "ivtentry.h"
#include "event.h"
#include "pcb.h"
class IVTEntry;


class KernelEv{

public:

	int val;
	PCB* pcb;
	IVTEntry *entry;
	Event* event;


	IVTNo ivtNo;

	PCB* creator;




	KernelEv(IVTNo ivt,Event* e){
		event=e;
		ivtNo=ivt;
		creator=((PCB*)PCB::running);
		val=1;
		this->set(ivt);



	}

	~KernelEv();

	void set(IVTNo ivt);

	void signal(){
		/*if(pcb==0){
			val=1;
		}else{
			pcb->blocked=0;
			Scheduler::put((PCB*)PCB::running);
			val=0;
			pcb=0;
		}*/

		if(creator->blocked==1){

			creator->blocked=0;
			Scheduler::put(creator);
			val = 1;
			dispatch();
		} else val = 0;

	}


	void wait(){
		if(creator==PCB::running){
			if(val==1){
				val=0;
				pcb=((PCB*)PCB::running);
				pcb->blocked=1;
				dispatch();
			}else{
				val=0;
			}
		}
	}



	//pogledaj ovde za ove value u signal i wait i treba u signalu valjda prvo da se proveri da li je blokirana

};



#endif /* KERNELEV_H_ */
