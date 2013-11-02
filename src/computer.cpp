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
		sleep(1);
	} while (hasJobs());
}

void Computer::loadJobs() {
	ProcessBuilder _instance = ProcessBuilder::getInstance();
	for (int i = 0; i < NUM_OF_PROCESSES; ++i) {
		process_t _proc = _instance.generateProcess();
		_proc._limit = (unsigned long) ((rand() % 120) + 4);
		_memManager.addToReadyQueue(_proc);
	}
}

bool Computer::hasJobs() {
	return true;
}

void Computer::cycle() {
	_memManager.executeCycle();
}

