/*
 * kerlist.cpp
 *
 *  Created on: Jan 14, 2020
 *      Author: OS1
 */

#include "kerlist.h"
#include "lock.h"




int KerList::size(){
	return number;
}


KerList::KerList(){
	first=0;
	last=0;
	number=0;
}

KerList::~KerList(){
	Node *temp=first,*temp2;
	if (first==0)return;
	while(temp!=0){
	temp2=temp;
	temp=temp->next;
	delete temp2->elem;
	delete temp2;
	}
	first=last=0;

}

void KerList::insert(KernelSem *ker){
		LOCK;
		Node *element = new Node(ker);
		element->next=0;
		if (first==0){
			first = element;

		}else
			{
				last->next = element;

			}
		last=element;
		number++;
		UNLOCK;
}


void KerList::deleteKer(KernelSem *kers){
	if (number==0)return ;
		LOCK;
		Node *temp=first;
		Node *prevtemp=0;

		if(temp!=0 && ((temp->elem)==kers)){
			number--;
			first=temp->next;
			if (!first) last=0;
			delete temp;
			UNLOCK;
			return;
		}
		while(temp!=0 && temp->elem!=kers){
			prevtemp=temp;
			temp=temp->next;
		}
		if(!temp){UNLOCK;return; }
		if(temp->elem==kers){
			if (temp==last)
				last=prevtemp;
			else {
				prevtemp->next=temp->next;
				}
		}
		number--;

		delete temp;
		UNLOCK
		return;
}



