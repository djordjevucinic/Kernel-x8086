/*
 * queue.cpp
 *
 *  Created on: Jan 11, 2020
 *      Author: OS1
 */


#include "queue.h"
#include "lock.h"


Queue::Queue(){
	first=0;
	last=0;
	number=0;
	BigTime=0;
}


int Queue::size(){
	return number;
}



PCB* Queue::odblokiraj(Element *el){

	Element *temp=first;
	Element *prev=0;
	if (first==0)return 0;
	if (first==el){first=first->next; if(first==0) last=0; number--;return el->pcb;}
	temp=temp->next;
	while(temp!=el){
		prev=temp;
		temp=temp->next;
	}
	if (temp->next==0){prev->next=0; last=prev; return el->pcb;}
	prev->next=temp->next;
	number--;
	return el->pcb;

}

PCB* Queue::odblokirajBlockListu(){
	Element *temp=first;
	if (first==0)return 0;
		first=first->next;
		if(!first)last=0;
		number--;
		return temp->pcb;
}

PCB *Queue::odblokirajTimeListu(){

	Element *temp=first;
	Element *prev=0;
	Element *tmp,*prv;
	if(first==0) return 0;
	Time min =temp->time;
	PCB *minpcb=temp->pcb;
	prev=temp;
	temp=temp->next;
	if(temp==0) {first=last=0; return prev->pcb;}
	while (temp!=0){
		if(min>=(temp->time)){min=temp->time;minpcb=temp->pcb;tmp=temp;prv=prev;}
		prev=temp;
		temp=temp->next;
	}
	if(tmp->next==0){
		prv->next=0;
		last=prv;
		number--;
		return minpcb;
	}

	prv->next=tmp->next;
	number--;
	return minpcb;

}


Queue::~Queue() {

	Element* temp = first;
	while (temp != 0) {
		Element* temp2 = temp;
		temp = temp->next;
		delete temp2->pcb;
		delete temp2;
	}
	number = 0;
	first=last=0;
}

void Queue::insert(PCB *p){
	LOCK
	Element *elem = new Element(p,0);
	elem->next=0;
	if (first==0){

		first = elem;

	}else
		{
			last->next = elem;

		}
	last=elem;
	number++;
	UNLOCK

}


void Queue::tickunit(){
	if (first!=0) BigTime++;
	else BigTime=0;
}

void Queue::insert2(PCB *p,Time t){
	LOCK
	t+=BigTime;
	Element *elem = new Element(p,t);
	elem->next=0;
	if (first==0){

		first = elem;

	}else
		{
			last->next = elem;

		}
	last=elem;
	number++;
	UNLOCK

}

PCB* Queue::remove1(){

	if (first==0)return 0;
	LOCK
		PCB *x = first->pcb;
		Element *temp = first;
		first=first->next;
		if(first==0)last=0;
		delete temp;
		number--;
	UNLOCK
	return x;
}


PCB* Queue::removeID(ID id){

	if (number==0)return 0;
	LOCK
	PCB *x=0;
	Element *temp=first;
	Element *prevtemp=0;

	if(temp!=0 && temp->pcb->threadID()==id){
		number--;
		first=temp->next;
		x=temp->pcb;
		if (!first) last=0;
		delete temp;
		UNLOCK;

		return x;

	}

	while(temp!=0 && temp->pcb->threadID()!=id){
		prevtemp=temp;
		temp=temp->next;
	}
	if(!temp){UNLOCK;return 0; }
			if(temp->pcb->threadID()==id){
				x=temp->pcb;
				if (temp==last)
					last=prevtemp;
				else {
					prevtemp->next=temp->next;
					}
			}
			number--;

			delete temp;
			UNLOCK
			return x;

}

PCB* Queue::searchByID(ID id){
	LOCK
	if (number==0) return 0;
	Element *temp = first;
	while(temp!=0 || temp->pcb->threadID()!=id)
		temp=temp->next;

	UNLOCK
	if(temp->pcb->threadID()==id)
	return temp->pcb;
	return 0;
}


