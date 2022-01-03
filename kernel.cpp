

#include "kernel.h"
#include "thread.h"
#include "pcb.h"
#include <dos.h>
#include <stdio.h>
#include "semlist.h"
#include <iostream.h>


PCBList* Kernel::threads = new PCBList();
SemList* Kernel::sems = new SemList();

volatile unsigned Kernel::lockFlag = 1;

volatile int Kernel::quant = 0;
volatile int Kernel::flipSwitch = 0;

Thread  *Kernel::dummyThread = 0;
Thread *Kernel::mainThread=0;

pInterrupt Kernel::oldRoutine = 0;

volatile unsigned tsp = 0, tss = 0, tbp = 0;


void Kernel::start() {
	Kernel::setTimerRoutine();

	PCB::setThreads();

}

void Kernel::end() {
//mozda ovaj asm ne treba ovako
	asm { pushf; cli; }
		setvect(oldrut, Kernel::oldRoutine);
		asm { popf; }

	PCB::running=0;
	delete Kernel::mainThread;
	delete Kernel::dummyThread;
}

void Kernel::setTimerRoutine(){
	asm {
		pushf;
		cli;
	}
	Kernel::oldRoutine = getvect(oldrut);
	setvect(oldrut, timer);
	asm {
		popf;
	}

}


void interrupt timer(...) {
	/*
	int cnt1=0;
	while(cnt1<5){
	cout<<PCB::running->id<<endl;
	cnt1++;
	}*/


	//cout<<"runn"<<PCB::running->id<<endl;
	//cout<<"dum"<<Kernel::dummyThread->myPCB->id<<endl;

	if ((!Kernel::flipSwitch)&&(PCB::running->unlimited==0)) {
		tick();
		Kernel::quant--;
		//za sad je ovde problem
		Kernel::sems->onTick();


		(*Kernel::oldRoutine)();
	}
	if ((Kernel::quant <= 0 && (PCB::running->unlimited==0)) || Kernel::flipSwitch) {
		if (1) {

			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

			/*
			Kernel::firstThread->myPCB->blocked=0;
			if(Kernel::firstThread->myPCB->finish==1){
				cout<<"prva zavrsila"<<endl;
				PCB::running=0;
				Kernel::context_on_demand=0;
			}
*/
			if (PCB::running->finish!=1 && PCB::running->blocked==0){
				if( PCB::running->myThread != Kernel::dummyThread) {
					if(PCB::running->id==3){
						//cout<<"opet usos"<<endl;
					}
					Scheduler::put((PCB*) PCB::running);
			}}


			PCB::running = Scheduler::get();
			if (!(PCB::running)){
				//cout<<"greska"<<endl;
			PCB* ft=PCB::getById(Kernel::dummyThread->getId());
			PCB::running=PCB::getById(Kernel::dummyThread->getId());
			//PCB::running = Kernel::dummyThread->myPCB;
			}

			/*
			do {
				PCB::running = Scheduler::get();
				if (!(PCB::running)){
					//cout<<"greska"<<endl;
				PCB* ft=PCB::getById(Kernel::dummyThread->getId());
				PCB::running = Kernel::dummyThread->myPCB;
				}
			} while(!PCB::running->stack);
*/


			/*
			int cnt=1;
			while(cnt!=6){
				PCB* ft2=PCB::getById(cnt);
				if(ft2==0){
					cout<<"nasap"<<endl;
				}
				ft2->blocked=1;
				cnt++;
			}

			*/

			Kernel::quant = PCB::running->quant;



			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;


			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
			Kernel::flipSwitch = 0;
		}
		else {
			//Kernel::flipSwitch = 1;
		}
	}
}
