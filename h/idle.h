/*
 * idle.h
 *
 *  Created on: Jan 14, 2020
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_

#include "thread.h"

class Idle: public Thread {
public:
	Idle();

	void run();

};



#endif /* IDLE_H_ */
