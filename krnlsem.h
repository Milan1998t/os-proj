/*
 * krnlsem.h
 *
 *  Created on: Aug 22, 2020
 *      Author: OS1
 */

#ifndef KRNLSEM_H_
#define KRNLSEM_H_
#include "speclist.h"
#include "semaphor.h"
class KernelSem{
public:

	Semaphore* mySem;
	PCBList* normal;
	Listsem* spec;
	int val;

	KernelSem(int i,Semaphore* s){
		val=i;
		mySem=s;
		normal=new PCBList();
		spec=new Listsem(); //ili treba new Spec();
	}

	~KernelSem(){
		delete normal;
		delete spec;
	}

	void pom(){
		PCBList::Node* p=normal->first;
		while(p){
			cout<<p->data->id<<endl;
			p=p->next;
		}
		cout<<"++++++"<<val<<endl;
	}

	int wait(Time maxTimeToWait){
		val--;
		//cout<<val<<endl;

		//pom();
		//proveri da li je manje i jednako ili samo manje
		if(maxTimeToWait>0){
			spec->insert((PCB*)PCB::running,maxTimeToWait);
			PCB* p=(PCB*)PCB::running;
			//cout<<"specblok"<<p->id<<endl;
			p->blocked=1;
			dispatch();
		}
		if(val<0){
			if(maxTimeToWait==0){
				normal->insert((PCB*)PCB::running);
				PCB* p=(PCB*)PCB::running;
				p->blocked=1;
				//PCB::running->blocked=1;
				//cout<<"blocked"<<p->id<<endl;
				dispatch();
			}else{
/*
				spec->insert((PCB*)PCB::running,maxTimeToWait);
				PCB* p=(PCB*)PCB::running;
				cout<<"specblok"<<p->id<<endl;
				p->blocked=1;
				dispatch();
				*/
			}
			if(((PCB*)PCB::running)->with_wait==1){
				//cout<<"vrati 0"<<endl;
				return 0;
			}
			return 1;
		}

		return 0;

	}

	int signal(int n){
		//pom();
		if(n<0){
			return n;
		}
		int cnt=0;
		if(val<=0){
			if(n==0){
				val++;
				if(normal->first!=0){
					//cout<<"pokuso"<<endl;
					PCB* tmp=normal->takeFirst();
					tmp->blocked=0;
					//cout<<"wake"<<tmp->id<<endl;
					Scheduler::put(tmp);

					return 0;
				}else if(spec->first!=0){
					cout<<"specijalac"<<endl;
					PCB* tmp=spec->takeFirst();
					tmp->blocked=0;
					Scheduler::put(tmp);
					// da li treba plus ili minus

					return 0;
				}
			}else{
				while(cnt!=n){
					if(normal->first!=0){
						PCB* tmp=normal->takeFirst();
						tmp->blocked=0;
						Scheduler::put(tmp);
					}else{
						PCB* tmp=spec->takeFirst();
						if(tmp==0){
							return cnt;
						}
						tmp->blocked=0;
						Scheduler::put(tmp);
					}
					cnt++;
					val++;

				}
				return cnt;
			}
		}else{
			if(n==0)val++;
			val+=n;
			return 0;
		}

	}

};


#endif /* KRNLSEM_H_ */
