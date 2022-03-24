/*
 * main.cpp
 *
 *  Created on: Jan 14, 2020
 *      Author: OS1
 */


#include <iostream.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "thread.h"
#include "pcb.h"
#include "kernel.h"
#include "lock.h"


extern int userMain(int argc, char* argv[]);


int main(int argc, char* argv[]){

			Kernel::init();
			int ret = userMain(argc, argv);
			LOCK;
			printf("User main return value = %d\n", ret);
			UNLOCK;
			Kernel::restore();

		return ret;
}

