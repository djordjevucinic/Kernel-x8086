/*
 * semaphor.cpp
 *
 *  Created on: Jan 14, 2020
 *      Author: OS1
 */
#include "semaphor.h"
#include "kersem.h"
#include "lock.h"


Semaphore::Semaphore(int init){
	this->myImpl = new KernelSem(init,this);
}


Semaphore::~Semaphore() {
    LOCK;
    delete myImpl;
    UNLOCK;
}

int Semaphore::wait(Time maxTimeToWait) {
    return this->myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n) {
    return this->myImpl->signal(n);
}

int Semaphore::val() const {
    return this->myImpl->val();
}






