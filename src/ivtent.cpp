/*
 * ivtent.cpp
 *
 *  Created on: Jan 15, 2020
 *      Author: OS1
 */
#include "ivtent.h"
#include "lock.h"


IVTEntry* IVTEntry::tabela[MAX_SIZE]={0};

IVTEntry::IVTEntry(IVTNumber n,pInterrupt routine){

	broj=n;
	oldRoutine=0;
	event=0;

#ifndef BCC_BLOCK_IGNORE
			asm cli;
			oldRoutine = getvect(broj);
			setvect(broj, routine);
			asm sti;
#endif
	tabela[broj]=this;
}

IVTEntry::~IVTEntry(){
	LOCK;
	tabela[broj]=0;
#ifndef BCC_BLOCK_IGNORE
			asm cli;
			setvect(broj, oldRoutine);
			asm sti;
#endif
	callOldInterrupt();
	UNLOCK;

}

void IVTEntry::setevent(KernelEv* eve){
	this->event=eve;

}

void IVTEntry::callOldInterrupt(){
	LOCK;
	if(oldRoutine)
		(*oldRoutine)();
	UNLOCK;

}

void IVTEntry::signal(){
	LOCK;
	if(event){event->signal();}
	UNLOCK;
}


