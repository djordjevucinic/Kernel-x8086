/*
 * kernel.h
 *
 *  Created on: Jan 11, 2020
 *      Author: OS1
 */

#ifndef KERNEL_H_
#define KERNEL_H_
#include "thread.h"
#include "queue.h"
#include "kersem.h"
#include "kerlist.h"
#include "idle.h"

typedef void interrupt (*pInterrupt)(...);

class Kernel{
	friend class Thread;
	friend class Queue;

public:

	static void init();
	static void restore();
	static void interrupt timer(...);
	static Queue *proccessList;
	static Thread *start;
	static Idle *idle;
	static  PCB *running;
	static pInterrupt oldTimer;
	static volatile int brojac;
	static volatile int zahtevana_promena_konteksta;
	static KerList *allKerSems;
	static void ticksem();
	static void dispatch();




private:





};

#endif /* KERNEL_H_ */
