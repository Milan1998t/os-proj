/*
 * semaphor.cpp
 *
 *  Created on: Aug 21, 2020
 *      Author: OS1
 */


#include"semaphor.h"
#include "krnlsem.h"
#include"semlist.h"
#include "kernel.h"


Semaphore::Semaphore(int init){
	myImpl=new KernelSem(init,this);
	//List_sem* s=List_sem::sem;
	//vidi da li je ovo dobro
	Kernel::sems->insert(myImpl);
}

Semaphore::~Semaphore(){
	delete myImpl;
}

int Semaphore::val()const{
	return myImpl->val;
}

int Semaphore::wait(Time maxTimeToWait){
	myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n){
	myImpl->signal(n);
}
