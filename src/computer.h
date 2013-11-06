/*
 * computer.h
 *
 *  Created on: Nov 1, 2013
 *      Author: visitor15
 */

#ifndef COMPUTER_H_
#define COMPUTER_H_

#include <unistd.h>
#include <iostream>
#include "memory_manager.h"
#include "process_manager.h"

static int _mem_strategy;
static MemoryManager _memManager;

class Computer {
public:
	static void powerOn() {
		static Computer _instance;
		_instance.boot();
	}

private:
	Computer() {
		cycle_num = 0;
		_mem_strategy = 0;
	}
	;
	Computer(Computer const&);
	void operator=(Computer const&);
	virtual ~Computer();

	void boot();
	void init();
	void beginExecution();
	void loadJobs();
	bool hasJobs();
	void cycle();

	int cycle_num;

};

#endif /* COMPUTER_H_ */
