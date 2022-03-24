/*
 * kernel.cpp
 *
 *  Created on: Jan 12, 2020
 *      Author: OS1
 */
#include "kernel.h"
#include "pcb.h"
#include "thread.h"
#include "idle.h"
#include <iostream.h>
extern int tick();




Queue* Kernel::proccessList = 0;
PCB* Kernel::running = 0;
Thread* Kernel::start=0;
Idle* Kernel::idle= 0;
KerList* Kernel::allKerSems= new KerList();
volatile int Kernel::zahtevana_promena_konteksta = 0;
volatile int Kernel::brojac=defaultTimeSlice;
pInterrupt Kernel::oldTimer = 0;



void Kernel::init(){
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	oldTimer = getvect(0x8);
	setvect(0x8, timer);
	setvect(0x60, oldTimer);
#endif
	Kernel::proccessList = new Queue();
	start= new Thread(maxStackSize,1);
	start->myPCB->status=PCB::READY;
	running = start->myPCB;
	idle=new Idle();
	idle->myPCB->status = PCB::IDLE;
	idle->start();
#ifndef BCC_BLOCK_IGNORE
	asm sti;
#endif

}

void Kernel::ticksem(){
	LOCK;

	Node *list=Kernel::allKerSems->first;
	KernelSem *sem=list->elem;
	for (int i=0; i<(Kernel::allKerSems->size()); i++){
		if (sem->TimeBlockList->size()>0)
		sem->TimeBlockList->tickunit();

		Element *temp=sem->TimeBlockList->first;
			for (int j=0; j<(sem->TimeBlockList->size());j++){

				if ((sem->TimeBlockList->BigTime)>=(temp->time)){
					if (sem->TimeBlockList->size()>0){
					PCB *odblokiran = sem->TimeBlockList->odblokiraj(temp);
					odblokiran->semsignal=1;
					odblokiran->ready();
					sem->value++;
					}
				}
				temp=temp->next;
			}

			list=list->next;
			sem=list->elem;
	}
UNLOCK;

}


void Kernel::restore(){
#ifndef BCC_BLOCK_IGNORE
	asm cli;
	setvect(0x8, oldTimer);
	asm sti;


#endif

	delete proccessList;
	delete allKerSems;



}

void Kernel::dispatch() {
	LOCK;
	zahtevana_promena_konteksta=1;
	timer();
	UNLOCK;
}

unsigned tsp;
unsigned tss;
unsigned tbp;


void interrupt Kernel::timer(...){	// prekidna rutina


	if (!zahtevana_promena_konteksta) {
		if(brojac>0)
		brojac--;
		tick();
		Kernel::ticksem();

	}
	if ((brojac == 0 && Kernel::running->timeSlice!=0)|| zahtevana_promena_konteksta) {
		zahtevana_promena_konteksta = 0;
#ifndef BCC_BLOCK_IGNORE
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp;
		}
#endif

		running->sp = tsp;
		running->ss = tss;
		running->bp= tbp;

		if(Kernel::running && Kernel::running->status!=PCB::FINISHED && Kernel::running->status!=PCB::BLOCKED && Kernel::running->threadID()!=1 && Kernel::running->threadID()!=0){
			Kernel::running->status=PCB::READY;
			Scheduler::put((PCB*)Kernel::running);

		}

		do{

		Kernel::running= Scheduler::get();



		if (Kernel::running==0){

			Kernel::running= idle->myPCB;
		}
		else
			Kernel::running->status=PCB::RUNNING;

		tsp = running->sp;
		tss = running->ss;
		tbp = running->bp;

		brojac = running->timeSlice;



#ifndef BCC_BLOCK_IGNORE
		asm {
			mov sp, tsp   // restore sp
			mov ss, tss
			mov bp, tbp;
		}
#endif

		}while(Kernel::running->status==PCB::FINISHED);

	//cout<<Kernel::running->threadID()<<endl;
}

	if(!zahtevana_promena_konteksta) {
#ifndef BCC_BLOCK_IGNORE
		asm int 60h;
#endif
	}
}
