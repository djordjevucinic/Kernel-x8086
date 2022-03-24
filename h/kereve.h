/*
 * kereve.h
 *
 *  Created on: Jan 15, 2020
 *      Author: OS1
 */

#ifndef KEREVE_H_
#define KEREVE_H_
#include "event.h"
#include "pcb.h"

typedef unsigned char IVTNumber;

class KernelEv {

public:

	KernelEv(IVTNumber n);
	~KernelEv();
	void wait();
	void signal();


private:

	IVTNumber num;
	PCB *allowed;
	friend class PCB;


};


#endif /* KEREVE_H_ */
