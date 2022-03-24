/*
 * kersem.cpp
 *
 *  Created on: Jan 14, 2020
 *      Author: OS1
 */

#include "kersem.h"
#include "lock.h"
#include "pcb.h"
#include "kernel.h"
#include "kerlist.h"
#include <iostream.h>


KernelSem::KernelSem(int init,Semaphore *sema){
	if (init <0) init=0;
	value=init;
	sem=sema;
	Kernel::allKerSems->insert(this);
	blockList=new Queue();
	TimeBlockList= new Queue();

}

KernelSem::~KernelSem() {
 /*   for (int i=0; i<TimeBlockList->size(); i++){
    	PCB	*odblokirana = TimeBlockList->odblokirajTimeListu();
    	odblokirana->ready();
    }
    delete TimeBlockList;

    for (int j=0; j<blockList->size(); j++){
        PCB	*odblokirana = blockList->odblokirajBlockListu();
        odblokirana->ready();
       }
    delete blockList;*/
    Kernel::allKerSems->deleteKer(this);


}




int KernelSem::val() const {
	return this->value;
}


int KernelSem::wait(Time maxTimeToWait){
	LOCK;
	int ret=1;
	if (--value>=0){
		ret=1;
	}else{

		Kernel::running->block();

		if (maxTimeToWait==0)
			blockList->insert((PCB*)Kernel::running);
		else
			TimeBlockList->insert2((PCB*)Kernel::running,maxTimeToWait);

		Kernel::dispatch();

		if (Kernel::running->semsignal==1){
			Kernel::running->semsignal=0;
			ret = 0;
		}


	}
	UNLOCK;
	return ret;


}

int KernelSem::signal(int n) {

	int num;

	LOCK;
	if (n<0) {UNLOCK; return n;}
	if (n==0) num=1;
	else num=n;
	int povratna=0;

	Element *elem=blockList->first;
	if(elem!=0){
		for (int i=0; i<(blockList->size());i++){
			if (num<=0)break;
			if ((blockList->size())>0){
			PCB	*odblokirana = blockList->odblokirajBlockListu();
			odblokirana->ready();
			num--;
			povratna++;
			}
		}

	}
	Element *elem2=TimeBlockList->first;
	if(elem2!=0){
			for (int i=0; i<(TimeBlockList->size());i++){
				if (num<=0)break;
				if((TimeBlockList->size())>0){
				PCB	*odblokirana = TimeBlockList->odblokirajTimeListu();
				odblokirana->ready();
				num--;
				povratna++;
			}

		}
	}
	if (n==0){value=value+1;povratna=0;}
	else value=value+n;
	UNLOCK;
	return povratna;


}



