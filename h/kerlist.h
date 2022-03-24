/*
 * kerlist.h
 *
 *  Created on: Jan 14, 2020
 *      Author: OS1
 */

#ifndef KERLIST_H_
#define KERLIST_H_

#include "kersem.h"




struct Node{
		KernelSem *elem;
		Node *next;
		Node(KernelSem *el){elem=el; next=0;}

	};


class KerList {

public:
	KerList();
	~KerList();
	void insert(KernelSem *ker);
	void deleteKer(KernelSem *kers);
	int size();

private:

	int number;
	Node *first,*last;
	friend class KernelSem;
	friend class Kernel;


};

#endif /* KERLIST_H_ */
