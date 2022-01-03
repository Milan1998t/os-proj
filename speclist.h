/*
 * speclist.h
 *
 *  Created on: Aug 25, 2020
 *      Author: OS1
 */

#ifndef SPECLIST_H_
#define SPECLIST_H_

#include "SCHEDULE.H"
#include "pcb.h"
#include "kernel.h"

class PCB;

class Listsem{

public:
	typedef struct Node{
	public:
	PCB* data;
	int sleep_time;
	Node* next;
	Node(PCB* p,int s){
		sleep_time=s;
		data=p;
		next=0;
	}
	};

	Node* first;
	Node* last;

//	friend class KernelSem;

public:

	Listsem(){
		first=0;
		last=0;
	}
	~Listsem(){
		Node*tmp=first;
		Node*prev=0;
		while(tmp!=0){
			prev=tmp;
			tmp=tmp->next;
			delete prev;
		}
	}
	void insert1(PCB* p,int s){
		Node* neew=new Node(p,s);
		if(!first){
			first=last=neew;
			return;
		}
		last->next=neew;
		last=neew;
		return;
	}

	void all(){
		Node* tmp=first;
		if(!first)return;
		while(tmp){

			if(tmp->data->id<10){
				//cout<<"tmp"<<tmp->data->id<<endl;
			Scheduler::put(tmp->data);
			}
			tmp=tmp->next;

		}
		first=last=0;
	}
	void insert(PCB* p,int s){
		if(!first){
			Node* new_node= new Node(p,s);
			first=last=new_node;
		}else{
			int tmp1=first->sleep_time;
			Node* tmp=first;
			if(first->sleep_time>=s){
				Node* new_node= new Node(p,s);
				new_node->next=first;
				first=new_node;
				tmp->sleep_time=tmp->sleep_time-s;
				return;
			}
			Node*tmp2=tmp;
			int tmp3=tmp1;
			while(tmp->next){
				if(s>(tmp1+tmp->next->sleep_time)){
					tmp=tmp->next;
					tmp1=tmp1+tmp->sleep_time;
					tmp2=tmp;
					tmp3=tmp1;
				}else{
					tmp=tmp->next;
					tmp1=tmp1+tmp->sleep_time;
				}
			}
			int c=s-tmp3;
			Node* new_node= new Node(p,c);
			new_node->next=tmp2->next;
			tmp2->next=new_node;
			if(tmp2==last){
				last=new_node;
				return;
			}
			tmp=new_node->next;
			Node*tmp4=first;
			int tmp5=0;
			while(tmp4!=new_node){
				tmp5=tmp5+tmp4->sleep_time;
				tmp4=tmp4->next;
			}
			tmp5=tmp5+new_node->next->sleep_time;
			if(tmp5==s){
				new_node->next->sleep_time=0;
				return;
			}


			tmp=new_node->next;
			tmp->sleep_time=tmp->sleep_time-c;

		}
	}

	Node* getFirst(){
		return first;
	}
	void print(){
		if(first){
			Node*pom=first;
			while(pom){

				//cout<<pom->data->id<<" broj sek "<<pom->sleep_time<<endl;

				pom=pom->next;
			}
		}
	}

	void printa(){
		//cout<<first->data->id<<" prvi broj sek "<<first->sleep_time<<endl;
	//	cout<<last->data->id<<" zadnji broj sek "<<last->sleep_time<<endl;

	}
	PCB* takeFirst(){

		/*if(tmp2->next!=0){
			if(tmp2->next->sleep_time==0){
				tmp2=tmp2->next;
				while(tmp2->sleep_time==0){
					tmp2=tmp2->next;
				}
				tmp2->sleep_time=tmp2->sleep_time+first->sleep_time;
			}else{
				tmp2=tmp2->next;
				tmp2->sleep_time=tmp2->sleep_time+first->sleep_time;
			}
			first=first->next;
			return tmp;
		}*/
		if(!first)return 0;
		PCB*tmp=first->data;
		Node*tmp2=first;
			if(tmp2->next!=0){
				tmp2=tmp2->next;
				tmp2->sleep_time=tmp2->sleep_time+first->sleep_time;
			}else{
				first=last=0;
			}
			first=first->next;
			return tmp;
		}

	int reduce(){
		if(!first) return -1;
		if(first->sleep_time==0){
			return 1;
		}
		first->sleep_time=first->sleep_time-1;
		if(first->sleep_time==0){
			return 1;
		}
		return 0;
	}

	//nisam siguran da bas dobro radi zato sto sta ako je prva 0 a drugi 1 i onda znas sta se desi
	int forInterruptFree(){
		if(!first) return 0;
		//ako imamo neku koja je stavljena sa wait(0)
		lock
		int cnt=0;
		first->sleep_time--;
		if(first->sleep_time==0){
			Node*tmp=first;
			while((tmp!=0)&&(tmp->sleep_time==0)){
				tmp->data->blocked=0;
				tmp->data->with_wait=1;
				PCB* t=tmp->data;
				//cout<<"deblock"<<tmp->data->id<<endl;
				Scheduler::put(t);
				//cout<<"in"<<tmp->data->id<<endl;
				tmp=tmp->next;
				cnt++;
			}
			if(tmp!=0){
			first=tmp;
			//first->sleep_time=first->sleep_time-1;
			}else{
				first=last=0;
			}
			unlock
			return cnt;
		}
	}

	int forInterruptFree2(){
		/*
			if(!first) return -1;
			//ako imamo neku koja je stavljena sa wait(0)
			lock
			if(first->sleep_time==0){
				Node*tmp=first;
				while(tmp->sleep_time==0){
					tmp->data->blocked=0;
					tmp->data->with_wait=1;
					PCB* t=tmp->data;
					Scheduler::put(t);
					//cout<<"in"<<tmp->data->id<<endl;
					tmp=tmp->next;
				}
				if(tmp!=0){
				first=tmp;
				first->sleep_time=first->sleep_time-1;
				}else{
					first=last=0;
				}
				unlock
				return 1;
			}else{
				first->sleep_time=first->sleep_time-1;
			}
			//a ovo je za one koje nisu sa wait(0)
			first->sleep_time=first->sleep_time-1;
			if(first->sleep_time==0){
				Node*tmp=first;
				while(tmp->sleep_time==0){
					tmp->data->blocked=0;
					tmp->data->with_wait=1;
					PCB* t=tmp->data;
					Scheduler::put(t);
					//cout<<"in"<<tmp->sleep_time<<endl;
					tmp=tmp->next;
				}
				if(tmp!=0){
					//cout<<"nije zadnji"<<tmp->data->id<<endl;
					first=tmp;
					first->sleep_time=first->sleep_time-1;
				}else{
					first=last=0;
				}
				unlock
				return 1;
			}
			if(first->sleep_time<=0){
				cout<<"greska"<<endl;
			}
			unlock
			return 2;
			*/
		}

	void update(PCB* p){
		Node* tmp=first;
		while(tmp->data!=p){
			tmp=tmp->next;
		}
		while(tmp){

		}

	}

	void deletePCB(int pos){
		if(first){
			Node* tmp=first;
			ID t=tmp->data->id;
			ID t2=first->data->id;
			if(t==t2){
				if(first==last){
					first=last=0;
					tmp=0;
				}else{
				first=tmp->next;
				tmp=0;
				}
			}else{
				while(tmp->next->data->id!=pos){
					tmp=tmp->next;
				}
				Node* tmp2=tmp->next;
				if(tmp2==last){
					last=tmp;
					tmp2=0;
				}else{
					tmp->next=tmp2->next;
					tmp2=0;
				}
			}
		}
	}
};




#endif /* SPECLIST_H_ */
