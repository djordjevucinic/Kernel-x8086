/*
 * pcb.h
 *
 *  Created on: Jan 7, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_


#include "thread.h"
#include "lock.h"
#include "queue.h"
#include "SCHEDULE.H"
#include <dos.h>


class Queue;



class PCB {
public:
	PCB(Thread *Thr, StackSize sz, Time time_Slice);
	enum State { NEW, READY, RUNNING, BLOCKED, FINISHED,IDLE };
	ID threadID();
	Thread *getThread();

	~PCB();
	void start();
	void createStack();
	void waitToComplete();
	static void wrapper();
	void block();
	void ready();
	int semsignal;
	void setsemsignal();



private:
	Thread *thread;
	Queue *waitingList;
	StackSize stacksize;
	Time timeSlice;
	static int id;


	ID thrID;


	unsigned *myStack;
	unsigned ss;
	unsigned sp;
	unsigned bp;
	volatile State status;



	friend class Thread;
	friend class Kernel;
	friend class Queue;
};


#endif /* PCB_H_ */
