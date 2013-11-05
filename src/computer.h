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

static const int MAX_QUANTA = 50000;
static const int PRINT_INTERVAL = 5000;
static const int NUM_OF_PROCESSES = 60;
static int _mem_strategy = 0;
static MemoryManager _memManager;

class Computer {
public:
	static void powerOn() {
		static Computer _instance;
		_instance.boot();
	}

private:
	Computer() {};
	Computer(Computer const&);
	void operator=(Computer const&);
	virtual ~Computer();

	void boot();
	void init();
	void beginExecution();
	void loadJobs();
	bool hasJobs();
	void cycle();

	int cycle_num = 0;

};

#endif /* COMPUTER_H_ */
