/*
 * idle.cpp
 *
 *  Created on: Jan 14, 2020
 *      Author: OS1
 */

#include "idle.h"
Idle::Idle(): Thread(defaultStackSize,1){}



void Idle::run(){
	while(1){}
}


