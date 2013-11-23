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
#include <stdlib.h>
#include "process_builder.h"
#include "backing_store.h"
#include "frame_table.h"

class MemoryManager {
public:
	MemoryManager();
	~MemoryManager();

	std::vector<struct process_t> _running_queue;
	std::vector<struct process_t> _ready_queue;

	int MEM_STRATEGY = -1;

	void init();
	void executeCycle();
	void executeCycleNonContigious();
	void addToBackingStore(const process_t process);
	void formatDetails();
	void addToReadyQueue(const process_t process);
	void touchNextReadyProc();
	void printMemMap();
	void printMemFrameMap();
	void loadSegmentInMemory(struct segment_t seg);

	bool swapIn(const process_t process);
	bool swapOut(const process_t process);
	bool doCompaction();
	bool hasProcRegistered(char _pid);
	bool hasReadyProcess();
	bool loadPage(struct mem_page_t* page);
	bool touchProcess(struct process_t* proc);
	bool touchSegment(struct segment_t* seg, int opt_index);
	bool loadKernelProcessInMemory(struct process_t proc);

	double getMemRatio();
	long getNumberOfFreeBlocks();
	std::pair<int, int> getMemFreeUsedPair();

	process_t* getProcessByPID(char pid);
	process_t getLargestProcess();
	process_t getSmallestProcess();

	struct process_t pullNextFromBackStore();
	struct process_t pullNextFromReadyQueue();

	std::pair<long, long> canFitFirstFit(process_t process);
	std::pair<long, long> canFitBestFit(process_t process);
	std::pair<long, long> canFitWorstFit(process_t process);

private:
	BackingStore back_store;
	FrameTable f_table;

	long _m_cycle_num;
	char _mem_array[MEMORY_SIZE];

	mem_frame_t _MAIN_MEMORY[MEM_SIZE_IN_FRAMES];

	std::vector<struct process_t> _back_store;
};

#endif /* MEMORYMANAGER_H_ */
