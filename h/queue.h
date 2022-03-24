/*
 * queue.h
 *
 *  Created on: Jan 7, 2020
 *      Author: OS1
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include "thread.h"
#include "pcb.h"
#include "lock.h"





struct Element{
		PCB *pcb;
		Time time;
		Element *next;
		Element(PCB *pc,Time t){pcb=pc; next=0;time=t; }

	};

class Queue {



public:
	Queue();
	~Queue();
	int size();
	void insert(PCB *p);
	void insert2(PCB *p,Time t);
	void tickunit();
	PCB* remove1();
	PCB* removeID(ID id);
	PCB *searchByID(ID id);
	PCB *odblokiraj(Element *el);
	PCB *odblokirajBlockListu();
	PCB *odblokirajTimeListu();
private:

	int number;
	Element *first,*last;
	Time BigTime;
	friend class KernelSem;
	friend class Kernel;

};



#endif /* QUEUE_H_ */
