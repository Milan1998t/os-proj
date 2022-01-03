/*
 * semlist.h
 *
 *  Created on: Aug 22, 2020
 *      Author: OS1
 */

#ifndef SEMLIST_H_
#define SEMLIST_H_
#include "krnlsem.h"
class SemList{
public:
	typedef struct Node{
	public:
	KernelSem* data;
	Node* next;
	Node(KernelSem* p){
		data=p;
		next=0;
	}
	};
	int num;
	Node* first;
	Node* last;
	Node* curr;

public:
	SemList(){
		first=last=0;
		num=0;
	}

	~SemList(){
		Node *tmp;
		while (first) {
			tmp = first;
			first = first->next;
			delete tmp;
			num--;
		}
	}

	void insert(KernelSem* s){
		Node* new_node= new Node(s);
		if(!first){
			first=last=new_node;
		}else{
			last->next=new_node;
			last=new_node;
		}
		num++;
	}

	KernelSem* getFirst(){
		return first->data;
	}

	KernelSem* takeFirst(){
		//kad se uzima mora da se ispita da li je 0
		if(!first) return 0;
		Node* pom=first;
		if(first->next!=0){
			first=first->next;
			return pom->data;
		}
		first=last=0;
		num--;
		return pom->data;

	}



	void onTick();
};


#endif /* SEMLIST_H_ */
