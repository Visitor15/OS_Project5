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
#include "process_manager.h"

static const unsigned long MEMORY_SIZE = 1040UL;

static int MEM_STRATEGY = 0;

class MemoryManager {
public:
	MemoryManager();
	virtual ~MemoryManager();
	virtual void init();
	virtual void executeCycle();
	virtual bool swapIn(const process_t process);
	virtual bool swapOut(const process_t process);
	virtual void addToBackingStore(const process_t process);
	virtual struct process_t pullNextFromBackStore();
	virtual void addToReadyQueue(const process_t process);
	virtual struct process_t pullNextFromReadyQueue();
	virtual std::pair<long, long> canFitFirstFit( process_t process);
	virtual bool canFitBestFit( process_t process);
	virtual bool canFitWorstFit( process_t process);
	virtual bool hasProcRegistered(char _pid);
	virtual void printMemMap();

private:
	char _mem_array[MEMORY_SIZE];

	std::vector<struct process_t> _running_queue;
	std::vector<struct process_t> _ready_queue;
	std::vector<struct process_t> _back_store;
};

#endif /* MEMORYMANAGER_H_ */
