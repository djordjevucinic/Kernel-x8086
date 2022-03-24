/*
 * thread.cpp
 *
 *  Created on: Jan 7, 2020
 *      Author: OS1
 */

#include "thread.h"
#include "pcb.h"
#include "lock.h"
#include "queue.h"
#include "SCHEDULE.H"
#include "kernel.h"
#include <iostream.h>




Thread::Thread(StackSize stackSize, Time timeSlice){
	LOCK
	myPCB = new PCB(this,stackSize,timeSlice);
	myPCB->createStack();
	UNLOCK
}

Thread::~Thread(){

	LOCK
	waitToComplete();
	delete myPCB;
	UNLOCK

}

ID Thread::getId(){
	return this->myPCB->threadID();
}


ID Thread::getRunningId(){
	return ((PCB*)Kernel::running)->threadID();
}


Thread* Thread::getThreadById(ID id){
	Queue *temp=Kernel::proccessList;
	return temp->removeID(id)->getThread();

}


void Thread::waitToComplete(){

	if (myPCB==0) return;
		myPCB->waitToComplete();

}

void Thread::start(){
	myPCB->start();
}



void Thread::dispatch() {
	LOCK;
	Kernel::dispatch();
	UNLOCK;
}





