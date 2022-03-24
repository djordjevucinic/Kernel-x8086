/*
 * kereve.cpp
 *
 *  Created on: Jan 15, 2020
 *      Author: OS1
 */
#include "kereve.h"
#include "lock.h"
#include "kernel.h"
#include "pcb.h"
#include "ivtent.h"


KernelEv::KernelEv(IVTNumber n){
	num=n;
	LOCK;
	allowed=(PCB*)Kernel::running;
	IVTEntry::tabela[n]->setevent(this);
	UNLOCK;
}

KernelEv::~KernelEv(){

	IVTEntry::tabela[num]->setevent(0);
	IVTEntry::tabela[num]=0;

}

void KernelEv::wait(){

	if (allowed==Kernel::running){
		LOCK;
		allowed->block();
		UNLOCK;
		Kernel::dispatch();
	}


}

void KernelEv::signal(){
	LOCK;
	allowed->ready();
	UNLOCK;
}
