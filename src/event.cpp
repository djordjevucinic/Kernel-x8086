/*
 * event.cpp
 *
 *  Created on: Jan 15, 2020
 *      Author: OS1
 */
#include "event.h"
#include "lock.h"
#include "kereve.h"

//#include "kerevent.h"

Event::Event(IVTNo ivtNo) {
	LOCK;
    myImpl = new KernelEv(ivtNo);
    UNLOCK;
}

Event::~Event() {
    LOCK;
    delete myImpl;
    myImpl = 0;
    UNLOCK;

}

void Event::wait() {
    myImpl->wait();
}

void Event::signal() {
    myImpl->signal();
}



