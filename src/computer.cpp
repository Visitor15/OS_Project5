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

	std::cout << "\nChoose a memory management strategy" << std::endl;
	std::cout << "1. First Fit" << std::endl;
	std::cout << "2. Best Fit" << std::endl;
	std::cout << "3. Worst Fit" << std::endl;
	std::cout << "\nChoice: ";
	std::cin >> _mem_strategy;

	ProcessBuilder _instance = ProcessBuilder::getInstance();
	for (int i = 0; i < NUM_OF_PROCESSES - 1; ++i) {
		process_t _proc = _instance.generateProcess();

		if (_memManager.hasProcRegistered(_proc._pid)) {
			do {
				_proc = _instance.generateProcess();
			} while (_memManager.hasProcRegistered(_proc._pid));
		}

		_proc._limit = (unsigned long) ((rand() % (MAX_PROC_SIZE - 4)) + 4);
		_proc._size = _proc._limit;

		std::cout << "Process: " << _proc._pid << " Generated size: "
				<< _proc._size << std::endl;

		_memManager.addToReadyQueue(_proc);
	}

	switch (_mem_strategy) {
	case 1: {
		_memManager.MEM_STRATEGY = 0;
		break;
	}
	case 2: {
		_memManager.MEM_STRATEGY = 1;
		break;
	}
	case 3: {
		_memManager.MEM_STRATEGY = 2;
		break;
	}
	default: {
		std::cout << "\nBad choice!\n" << std::endl;
		loadJobs();
		break;
	}
	}

	std::cout << "Running strategy: " << _memManager.MEM_STRATEGY << std::endl;
}

bool Computer::hasJobs() {
	return (cycle_num != MAX_QUANTA);
}

