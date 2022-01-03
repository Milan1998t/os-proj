/*
 * list.h
 *
 *  Created on: Apr 23, 2020
 *      Author: OS1
 */

#ifndef LIST_H_
#define LIST_H_
#include "thread.h"
//#include "pcb.h"


class PCB;
class PCBList{
public:
	typedef struct Node{
	public:
	PCB* data;
	Node* next;
	Node(PCB* p){
		data=p;
		next=0;
	}
	};

	Node* first;
	Node* last;
	Node* curr;

public:

	PCBList(){
		first=last=0;
	}

	~PCBList(){
		Node*tmp=first;
		Node*prev=0;
		while(tmp!=0){
			prev=tmp;
			tmp=tmp->next;
			delete prev;
		}

	}
	void insert(PCB* p){
		Node* new_node= new Node(p);
		if(!first){
			first=last=new_node;
		}else{
		last->next=new_node;
		last=new_node;
		}
	}

	Node* getFirst(){
		return first;
	}

	PCB* takeFirst(){
		//kad se uzima mora da se ispita da li je 0
		if(!first) return 0;
		Node* pom=first;
		if(first->next!=0){
			first=first->next;
			return pom->data;
		}
		first=last=0;
		return pom->data;
	}

	void deletePCB(int pos){
		//nalazi se u all.h
	}
};



#endif /* LIST_H_ */
