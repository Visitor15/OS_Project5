/*
 * MemoryManager.h
 *
 *  Created on: Nov 1, 2013
 *      Author: visitor15
 */

#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include "process_manager.h"

static const long MEMORY_SIZE = 1040;

class MemoryManager {
public:
	MemoryManager();
	~MemoryManager();

	int MEM_STRATEGY;

	void init();
	void executeCycle();
	bool swapIn(const process_t process);
	bool swapOut(const process_t process);
	void addToBackingStore(const process_t process);
	long getNumberOfFreeBlocks();
	process_t getLargestProcess();
	process_t getSmallestProcess();
	void formatDetails();
	struct process_t pullNextFromBackStore();
	void addToReadyQueue(const process_t process);
	struct process_t pullNextFromReadyQueue();
	std::pair<long, long> canFitFirstFit(process_t process);
	std::pair<long, long> canFitBestFit(process_t process);
	std::pair<long, long> canFitWorstFit(process_t process);
	bool hasProcRegistered(char _pid);
	bool hasReadyProcess();
	void printMemMap();

private:
	long _m_cycle_num;

	char _mem_array[MEMORY_SIZE];

	std::vector<struct process_t> _running_queue;
	std::vector<struct process_t> _ready_queue;
	std::vector<struct process_t> _back_store;
};

#endif /* MEMORYMANAGER_H_ */
