/*
 * pcb.cpp
 *
 *  Created on: Apr 26, 2020
 *      Author: OS1
 */
#include "pcb.h"
#include "kernel.h"
#include "iddle.h"


volatile PCB* PCB::running = 0;


PCB* PCB::getById(int i){
		PCBList::Node* tmp=Kernel::threads->first;
		if(tmp==0){
			cout<<"nije prazzna"<<endl;
		}
		while(tmp){
			if(tmp->data->id==i){
				//cout<<tmp->pcb->id<<endl;
				return tmp->data;
			}
			tmp=tmp->next;
		}
		return 0;
		//PCB* p=System::idleThread->myPCB;
	}

void PCB::setThreads(){
/*
		Kernel::firstThread= new Thread();
		Kernel::firstThread->myPCB->ready=1;
		Kernel::counter = Kernel::firstThread->myPCB->quant;
		PCB::running = Kernel::firstThread->myPCB;
		Kernel::threads->insert(Kernel::firstThread->myPCB);
*/

		Kernel::dummyThread=new Thread();

		Kernel::dummyThread->myPCB->ready=1;
		Kernel::threads->insert(Kernel::dummyThread->myPCB);

		Kernel::mainThread=new Thread();
		Kernel::mainThread->myPCB->quant=1;
		Kernel::mainThread->myPCB->ready=1;
		Kernel::threads->insert(Kernel::mainThread->myPCB);
		PCB::running=Kernel::mainThread->myPCB;


//		PCB::running=Kernel::dummyThread->myPCB;

}
