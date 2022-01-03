/*
 * semlist.cpp
 *
 *  Created on: Aug 26, 2020
 *      Author: OS1
 */

#include "semlist.h"
//#include "kernel.h"


 void SemList::onTick(){
	 	//SemList*t=Kernel::sems;
		SemList::Node* p=Kernel::sems->first;
		//KernelSem* pom1=Kernel::sems->first->data;
		int cnt=0;
		while(p){
			cnt=p->data->spec->forInterruptFree();
			p->data->val+=cnt;
			p=p->next;
		}


	}
