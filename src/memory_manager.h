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
#include <cstring>
#include "process_builder.h"
#include "backing_store.h"
#include "frame_table.h"

class MemoryManager {
public:
	MemoryManager();
	~MemoryManager();


	std::vector<struct process_t> _running_queue;

	int MEM_STRATEGY;

	void init();
	void executeCycle();
	bool swapIn(const process_t process);
	bool swapOut(const process_t process);
	void addToBackingStore(const process_t process);
	long getNumberOfFreeBlocks();
	std::pair<int, int> getMemFreeUsedPair();
	double getMemRatio();
	process_t getLargestProcess();
	process_t getSmallestProcess();
	void formatDetails();
	struct process_t pullNextFromBackStore();
	void addToReadyQueue(const process_t process);
	struct process_t pullNextFromReadyQueue();
	std::pair<long, long> canFitFirstFit(process_t process);
	std::pair<long, long> canFitBestFit(process_t process);
	std::pair<long, long> canFitWorstFit(process_t process);
	bool doCompaction();
	bool hasProcRegistered(char _pid);
	bool hasReadyProcess();
	void printMemMap();
	void loadSegmentInMemory(struct segment_t seg);
	bool loadPage(struct mem_page_t* page);
	bool touchProcess(struct process_t* proc);
	bool touchSegment(struct segment_t* seg);
	bool loadKernelProcessInMemory(struct process_t proc);
	void executeCycleNonContigious();

private:
	BackingStore 	back_store;
	FrameTable		f_table;

	long _m_cycle_num;
	char _mem_array[MEMORY_SIZE];

	mem_frame_t _MAIN_MEMORY[MEM_SIZE_IN_FRAMES];


	std::vector<struct process_t> _ready_queue;
	std::vector<struct process_t> _back_store;
};

#endif /* MEMORYMANAGER_H_ */
