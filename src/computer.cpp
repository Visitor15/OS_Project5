/*
 * computer.cpp
 *
 *  Created on: Nov 1, 2013
 *      Author: visitor15
 */

#include "computer.h"

int cycle_num = 0;

Computer::~Computer() {

}

void Computer::boot() {
	std::cout << "Computer booting..." << std::endl;
	init();
}

void Computer::init() {
	_memManager.init();
	_memManager.swapIn(ProcessBuilder::getInstance().generateKernelProcess());
	beginExecution();
}

void Computer::beginExecution() {

	loadJobs();
	do {
		cycle();
		_memManager.printMemMap();
	} while (hasJobs());

}

void Computer::loadJobs() {
	cycle_num = 0;
	ProcessBuilder _instance = ProcessBuilder::getInstance();
	for (int i = 0; i < NUM_OF_PROCESSES; ++i) {
		process_t _proc = _instance.generateProcess();

		if (_memManager.hasProcRegistered(_proc._pid)) {
			_proc._pid = generate_PID();
		}

		_proc._limit = (unsigned long) ((rand() % 120) + 4);
		_memManager.addToReadyQueue(_proc);
	}
}

bool Computer::hasJobs() {
	return (cycle_num != 100);
}

void Computer::cycle() {
	++cycle_num;
	_memManager.executeCycle();
}

