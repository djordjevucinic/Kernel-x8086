/*
 * pcb.cpp
 *
 *  Created on: Jan 7, 2020
 *      Author: OS1
 */
#include "pcb.h"
#include "thread.h"
#include "kernel.h"
#include <iostream.h>



int PCB::id = 0;

PCB::PCB(Thread *Thr, StackSize sz, Time time_Slice) : thread(Thr) ,timeSlice(time_Slice){

	LOCK;
	bp=sp=ss=0;
	thrID=id++;
	myStack=0;
	semsignal=0;
	waitingList = new Queue();
	if (sz>maxStackSize) stacksize = maxStackSize/sizeof(unsigned);
	else stacksize = sz/sizeof(unsigned);
	status=NEW;
	Kernel::proccessList->insert(this);
	UNLOCK;

}

void PCB::setsemsignal(){
	semsignal=1;
}

ID PCB::threadID(){
	return this->thrID;
}


Thread * PCB::getThread(){
	return thread;
}

void PCB::createStack(){
	LOCK;
	myStack = new unsigned[stacksize];

	#ifndef BCC_BLOCK_IGNORE
		myStack[stacksize - 1] = 0x200; //PSW
		myStack[stacksize - 2] = FP_SEG(PCB::wrapper);
		myStack[stacksize - 3] = FP_OFF(PCB::wrapper);
		sp = FP_OFF(myStack + stacksize - 12);
		ss = FP_SEG(myStack + stacksize - 12);
		bp = FP_OFF(myStack + stacksize - 12);

	#endif
	UNLOCK;
}


void PCB::start() {
	if (status==NEW){
		this->ready();
	}
}

void PCB::block(){
	LOCK;
	status=BLOCKED;
	UNLOCK;

}


PCB::~PCB(){
	if (myStack) delete myStack;
	myStack=0;

}

void PCB::ready(){
	LOCK
	this->status=READY;
	Scheduler::put(this);
	UNLOCK
}

void PCB::waitToComplete(){

	if (this->status!=FINISHED){
	LOCK;
	Kernel::running->block();
	waitingList->insert((PCB*)Kernel::running);

	Kernel::dispatch();
	UNLOCK;
	return;
	}


}



void PCB::wrapper() {
	Kernel::running->thread->run();
	LOCK;
	while(Kernel::running->waitingList->size()>0){

		PCB*novi = Kernel::running->waitingList->remove1();
		novi->status=READY;
		Scheduler::put(novi);

	}
	Kernel::running->status=FINISHED;
	Kernel::dispatch();
	UNLOCK;

}
