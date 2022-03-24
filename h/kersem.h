/*
 * kersem.h
 *
 *  Created on: Jan 14, 2020
 *      Author: OS1
 */

#ifndef KERSEM_H_
#define KERSEM_H_
#include "semaphor.h"
#include "kerlist.h"
#include "queue.h"

class KernelSem	{

public:
	KernelSem(int init,Semaphore *sema);
	~KernelSem();
	int wait(Time maxTimeToWait);
	int signal(int n);
	int val() const;


private:
	Semaphore *sem;
	int value;
	Queue *blockList;
	Queue *TimeBlockList;
	friend class PCB;
	friend class Kernel;
};


#endif /* KERSEM_H_ */
