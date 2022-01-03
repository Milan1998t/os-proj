/*
 * pcb.h
 *
 *  Created on: Apr 23, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_
#include <dos.h>
#include"pcblist.h"
#include <iostream.h>
#include "SCHEDULE.H"
#include "kernel.h"

class PCB{
public:
	static volatile PCB* running;
	static ID Id;
	unsigned* stack;
	unsigned bp;
	unsigned sp;
	unsigned ss;
	unsigned finish;
	int ready;
	int quant;
	Thread* myThread;
	ID id;
	int blocked;
	int unlimited;
	int with_wait;
	PCBList* to_wait;

	PCB(Thread* t){
		//pogledaj posle lepo
		myThread=t;
		id=Id++;
	}

	~PCB(){
		delete to_wait;
		delete[] stack;
	}

	static void wrapper();

	void sleep(int t){
		blocked=1;
	}

	static void createProcess(unsigned stacksize,unsigned time,PCB *newPCB, void (*body)()){
		//mozda ovo treba u konstruktoru threada
		if(stacksize<256) stacksize=256;
		if(stacksize>defaultStackSize)stacksize=defaultStackSize;
		unsigned* st1 = new unsigned[stacksize];

		//moram po velicini steka ovo da pravim u zagradama kod st1
		st1[stacksize-1] =0x200;//setovan I fleg u  pocetnom PSW-u za nit

		st1[stacksize-2] = FP_SEG(body);
		st1[stacksize-3] = FP_OFF(body);

		//nisam siguran da treba -12 i da li treba PCB::wraper na adrese da se stavi
		newPCB->sp = FP_OFF(st1+stacksize-12); 	 //svi sacuvani registri pri ulasku u interrupt rutinu
		newPCB->ss = FP_SEG(st1+stacksize-12);
		newPCB->bp = FP_OFF(st1+stacksize-12);

		newPCB->stack = st1;
		newPCB->finish = 0;
		newPCB->ready=0;
		newPCB->quant=time;
		newPCB->blocked=0;
		newPCB->to_wait=new PCBList();
		newPCB->with_wait=-1;
		if(time==0){
			newPCB->unlimited=1;
		}else{
			newPCB->unlimited=0;
		}
		//cout<<"napravio"<<endl;
	}



	void unlock_all(){

		//cout<<this->id<<endl;
		PCBList::Node* tmp=to_wait->first;

		//cout<<"doso"<<endl;
			while(tmp){
				//tmp->data->blocked=0;
				PCB* s=tmp->data;
				/*
				if(s->myThread==Kernel::firstThread){
					cout<<"odbl"<<endl;
				}
				if(Kernel::firstThread->myPCB->blocked==1){
					cout<<"blokirana"<<endl;
				}
				*/
				s->blocked=0;
				Scheduler::put(s);
				tmp=tmp->next;
			}


		to_wait=0;
	}
	friend class KernelSem;
	friend class Listsem;

	//////////////////

	static PCB* getById(ID i);

	static void setThreads();
};

//PCB* running;


#endif /* PCB_H_ */
