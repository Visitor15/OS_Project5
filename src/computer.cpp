/*
 * CS3242 Operating Systems
 * Fall 2013
 * Project 5: Swapping and Paging
 * Nick Champagne & John Mutabazi
 * Date: 11/22/2013
 * File: computer.cpp
 */

#include "computer.h"

Computer::~Computer() {

}

void Computer::boot() {
	std::cout << "Computer booting..." << std::endl;
	init();
}

void Computer::init() {
	srand(time(0));
	_memManager.init();
	_memManager.loadKernelProcessInMemory(
			ProcessBuilder::getInstance()->generateKernelProcess());
	_memManager.touchSegment(&_memManager._running_queue[0]._seg_code, -1);
	beginExecution();
}

void Computer::beginExecution() {
	loadJobs();
	do {
		cycle();

		if (cycle_num % PRINT_INTERVAL == 0) {
			if(_memManager.MEM_STRATEGY != -1) {
			_memManager.printMemMap();
			} else {
				_memManager.printMemFrameMap();
			}
		}
	} while (hasJobs());

	std::cout << "FINISHED" << std::endl;
}

void Computer::cycle() {
	++cycle_num;

	if(_memManager.MEM_STRATEGY != -1) {
		_memManager.executeCycle();
	}
	else {
		_memManager.executeCycleNonContigious();
	}
}

void Computer::loadJobs() {
	_memManager.MEM_STRATEGY = -1;

	std::cout << "\nChoose a memory management strategy" << std::endl;
	std::cout << "1. First Fit" << std::endl;
	std::cout << "2. Best Fit" << std::endl;
	std::cout << "3. Worst Fit" << std::endl;
	std::cout << "\n4. Paging" << std::endl;
	std::cout << "\nChoice: ";
	std::cin >> _mem_strategy;
	for (int i = 0; i < NUM_OF_PROCESSES - 1; ++i) {
		process_t _proc = ProcessBuilder::getInstance()->generateProcess();

		if (_memManager.hasProcRegistered(_proc._pid)) {
			do {
				_proc = ProcessBuilder::getInstance()->generateProcess();
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
	} case 4: {
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

