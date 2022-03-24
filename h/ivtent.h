/*
 * ivtent.h
 *
 *  Created on: Jan 15, 2020
 *      Author: OS1
 */

#ifndef IVTENT_H_
#define IVTENT_H_
#include "kereve.h"
#define MAX_SIZE 255


typedef unsigned char IVTNumber;
class KernelEv;

typedef void interrupt (*pInterrupt)(...);

#define PREPAREENTRY(NUM,value)\
	void interrupt inter##NUM(...);\
	IVTEntry ent##NUM = IVTEntry(NUM,inter##NUM);\
	void interrupt inter##NUM(...){\
		ent##NUM.signal();\
		if (value == 1)\
		ent##NUM.callOldInterrupt();\
	}



class IVTEntry {

public:

	IVTEntry(IVTNumber n,pInterrupt routine);
	~IVTEntry();
	static IVTEntry *tabela[MAX_SIZE];
	void setevent(KernelEv *eve);
	void signal();
	void callOldInterrupt();

private:
	pInterrupt oldRoutine;
	KernelEv *event;
	IVTNumber broj;

};





#endif /* IVTENT_H_ */
