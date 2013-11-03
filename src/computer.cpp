/*
 * computer.cpp
 *
 *  Created on: Nov 1, 2013
 *      Author: visitor15
 */

#include "computer.h"

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
		if (cycle_num % PRINT_INTERVAL == 0) {
			_memManager.printMemMap();
		}
	} while (hasJobs());
}

void Computer::cycle() {
	++cycle_num;
	_memManager.executeCycle();
}

void Computer::loadJobs() {
	srand(time(0));

	ProcessBuilder _instance = ProcessBuilder::getInstance();
	for (int i = 0; i < NUM_OF_PROCESSES; ++i) {
		process_t _proc = _instance.generateProcess();

		if (_memManager.hasProcRegistered(_proc._pid)) {
			do {
				_proc._pid = generate_PID();
			} while (_memManager.hasProcRegistered(_proc._pid));
		} else {
			_proc._limit = (unsigned long) ((rand() % 120) + 4);
			_proc._size = _proc._limit;

			std::cout << "Process: " << _proc._pid << " Generated size: "
					<< _proc._size << std::endl;

			_memManager.addToReadyQueue(_proc);
		}
	}
}

bool Computer::hasJobs() {
	return (cycle_num != MAX_QUANTA);
}

