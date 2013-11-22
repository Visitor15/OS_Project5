/*
 * process_builder.h
 *
 *  Created on: Nov 17, 2013
 *      Author: visitor15
 */

#ifndef PROCESS_BUILDER_H_
#define PROCESS_BUILDER_H_

#include <stdlib.h>
#include <time.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <memory.h>
#include "process_manager.h"
#include "page_fault_exception.h"
#include "memory_full_exception.h"

/*
 * 	STRUCT
 */
typedef struct mem_frame_t {
private:
	unsigned long _size;

public:

	char _id[2];
	unsigned long _base;
	unsigned long _limit;

	int _index;

	bool _active;

	mem_frame_t() {
		_id[0] = ' ';
		_id[1] = ' ';
		_base = 0;
		_limit = 0;
		_index = -1;
		_size = (sizeof(char) * PAGE_SIZE_IN_BYTES);
		_active = false;
	}

	mem_frame_t(int index) {
		_id[0] = ' ';
		_id[1] = ' ';
		_base = 0;
		_limit = 0;
		_index = index;
		_size = (sizeof(char) * PAGE_SIZE_IN_BYTES);
		_active = false;
	}

} _frame;

/*
 * 	STRUCT
 */
typedef struct mem_page_t {
private:
	unsigned long _size;

public:
	char _id[2];
	int _index;

	bool _is_active;

	mem_frame_t p_frame;

	mem_page_t() {
		_index = -1;
		_is_active = false;
		_size = (sizeof(char) * FRAME_SIZE_IN_BYTES);
		p_frame = mem_frame_t();
	}

	mem_page_t(int index) {
		_index = index;
		_is_active = false;
		_size = (sizeof(char) * FRAME_SIZE_IN_BYTES);
		p_frame = mem_frame_t();
	}

} _page;

/*
 *	STRUCT
 */
typedef struct segment_t {
	char _id[2];
	unsigned long _base;
	unsigned long _limit;
	int _num_pages;

	std::vector<mem_page_t> seg_pages;

	bool _valid;

	segment_t() {
		_id[0] = ' ';
		_id[1] = ' ';
		_base = 0;
		_limit = 0;
		_num_pages = 0;
		_valid = false;
	}

	segment_t(char ID[], int NUM_PAGES, long REG_BASE, long REG_LIMIT) {
		_id[0] = ID[0];
		_id[1] = ID[1];
		_num_pages = NUM_PAGES;

		seg_pages.clear();
		for (int i = 0; i < NUM_PAGES; i++) {
			seg_pages.push_back(mem_page_t(i));
		}

		_base = REG_BASE;
		_limit = REG_LIMIT;
		_valid = false;
	}

	bool touch() {
		for (int i = 0; i < seg_pages.size(); i++) {
			if (seg_pages.at(i)._index != seg_pages.at(i).p_frame._index) {
				throw PageFaultException(i);
			}
		}

		_valid = true;

		std::cout << "TOUCHED " << *this->_id << std::endl;

		return _valid;
	}
} _segment;

/*
 *	STRUCT
 */
typedef struct process_t {
	char _pid;

	segment_t _seg_code;
	segment_t _seg_stack;
	segment_t _seg_heap;
	std::vector<struct segment_t> _seg_routines;

	int _num_routines;
	int _burst_time;
	int _proc_runtime;
	int _priority;

	long _size;
	long _base;
	long _limit;

	bool _can_swap_out;
	bool _can_swap_in;

	P_STATE _state;

	long size() {
		return _size;
	}

	process_t() {
		_pid = ' ';
		_num_routines = 0;
		_burst_time = 0;
		_proc_runtime = 0;
		_priority = 0;
		_size = 0;
		_base = 0;
		_limit = 0;
		_state = SLEEPING;
		_can_swap_out = false;
		_can_swap_in = true;
	}
} _process;

static bool isKernelProcess(const process_t proc) {
	return (proc._pid == '@');
}

/*
 *	CLASS				ProcessBuilder
 *
 *	Implemented as a singleton builder to generate base processes.
 */

class ProcessBuilder {

private:
	std::vector<int> _pid_list;
	std::vector<process_t> _cached_history;

	ProcessBuilder();
	ProcessBuilder(ProcessBuilder const&);

public:

	static ProcessBuilder* getInstance();

	char generate_PID();

	process_t generateProcess();

	void generateProcRoutines(std::vector<segment_t> &list, int length, const char pid);

	struct process_t generateKernelProcess();

};

static ProcessBuilder* m_pInstance;

#endif /* PROCESS_BUILDER_H_ */
