/*
 * thread.cpp
 *
 *  Created on: Apr 22, 2020
 *      Author: OS1
 */
#include "thread.h"
//#include "list.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include<iostream.h>
#include "kernel.h"

PCBList pcb_list;
ID PCB::Id=0;


void PCB::wrapper(){
	lock
	if (PCB::running->myThread == Kernel::dummyThread)
			while (1) { }
	PCB::running->myThread->run();
	PCB* run=((PCB*)PCB::running);

	run->unlock_all();
	PCB::running->finish=1;
//	cout<<"ispo"<<endl;
	unlock
	dispatch();
}



/////////////////////////////////////////////

Thread* Thread::getThreadById(ID id){
	// obezbedi se
	PCBList::Node* tmp=pcb_list.getFirst();
	PCB* tmppcb=tmp->data;
	while(tmppcb->id!=id){
		tmp=tmp->next;
	}
	return tmppcb->myThread;
}

/////////////////////////////////////////////

ID Thread::getRunningId(){
	 return PCB::running->Id;
 }

/////////////////////////////////////////////

ID Thread::getId(){
	return myPCB->id;
}

/////////////////////////////////////////////

Thread::Thread(StackSize stackSize , Time timeSlice){
	//da li treba ovaj pcb ovako ili da se prosledi adresa od thsi
	//cout<<"napravio"<<endl;
	PCB* pcb=new PCB(this);
	PCB::createProcess(stackSize,timeSlice,pcb,PCB::wrapper);
	Kernel::threads->insert(pcb);
	//pcb_list.insert(pcb);
	myPCB=pcb;
}

/////////////////////////////////////////////

void Thread::start(){
	//ovo malo da se prekontrolise
	if(myPCB->ready==1 || myPCB->stack == 0)return;
	myPCB->ready=1;
	Scheduler::put(myPCB);
	//cout<<"ubacio"<<endl;
}

/////////////////////////////////////////////

Thread::~Thread(){
	//pcb_list.deletePCB(getId());
	//kaze milicev da virtual fja ne sme da se pozove iz jezgra
	//ziza kaze da se ne zove waittocomplete();
	//waitToComplete();
	//napravi kao sto ziza kaze
	//ovako bi trebalo
   //myPCB->semWaitingOn->myImpl->total.remove(myPCB);
	//System::threads->remove(myPCB);
	//delete myPCB;
	//cout<<"ovde"<<endl;
	//waitToComplete();
}

/////////////////////////////////////////////

void Thread::waitToComplete(){
	//cout<<"block"<<PCB::running->id<<endl;
	//cout<<"on"<<this->myPCB->id<<endl;
	//cout<<"dummy"<<Kernel::dummyThread->myPCB->id<<endl;
	lock
		if (myPCB->finish == 1 || myPCB->ready == 0 ||

				this == Kernel::dummyThread ||
				this == (PCB*) PCB::running->myThread) {
					unlock
					return;
		}
	//trebaju ovi uslovi da se vide

/*
	PCBList *tmp=this->myPCB->to_wait;
	tmp->insert((PCB*)PCB::running);
	*/
	PCB::running->blocked=1;
	this->myPCB->to_wait->insert((PCB*)PCB::running);
	//cout<<"wait"<<endl;
	unlock
	dispatch();

}

/////////////////////////////////////////////
void dispatch() {
	asm cli;
	Kernel::flipSwitch = 1;
	timer();
	asm sti;
}


