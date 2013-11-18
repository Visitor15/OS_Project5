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
#include "process_manager.h"
#include "page_fault_exception.h"

/*
 * 	STRUCT
 */
typedef struct mem_frame_t {
private:
	unsigned long _size;

public:
//	process_t* proc;

	char _id;
	unsigned long _base;
	unsigned long _limit;

	bool _active;

	mem_frame_t() {
//		proc = 0;
		_id = ' ';
		_base = 0;
		_limit = 0;
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
	int _index;

	bool _is_loaded;

	mem_frame_t* p_frame;

	mem_page_t() {
		_index = -1;
		_size = (sizeof(char) * FRAME_SIZE_IN_BYTES);
		_is_loaded = false;
		p_frame = 0;
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

	mem_page_t* p_pages;

	bool _valid;

	segment_t() {
		_id[0] = ' ';
		_id[1] = ' ';
		_base = 0;
		_limit = 0;
		_num_pages = 0;
		p_pages = 0;
		_valid = true;
	}

	segment_t(char ID[], int NUM_PAGES, mem_page_t PAGES[], long REG_BASE,
			long REG_LIMIT) {
		_id[0] = ID[0];
		_id[1] = ID[1];
		_num_pages = NUM_PAGES;
		p_pages = PAGES;
		_base = REG_BASE;
		_limit = REG_LIMIT;
		_valid = false;
	}

	bool touch() {
		for (int i = 0; i < _num_pages; i++) {
			if (!p_pages[i]._is_loaded) {
				throw PageFaultException(&p_pages[i]);
			}
		}

		return true;
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
	segment_t* _segs_routines;

	unsigned int _num_routines;
	unsigned int _burst_time;
	unsigned int _priority;

	unsigned long _size;
	unsigned long _base;
	unsigned long _limit;

	bool _can_swap_out;
	bool _can_swap_in;

	P_STATE _state;

	long size() {
		return _size;
	}

	process_t() {
		_pid = ' ';
		_segs_routines = 0;
		_num_routines = 0;
		_burst_time = 0;
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

	segment_t* generateProcRoutines(const char pid);

	struct process_t generateKernelProcess();

};

static ProcessBuilder* m_pInstance;

#endif /* PROCESS_BUILDER_H_ */
