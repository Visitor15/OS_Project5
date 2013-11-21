/*
 * process_manager.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: visitor15
 */

#include "process_builder.h"

ProcessBuilder::ProcessBuilder() {
}
;
ProcessBuilder::ProcessBuilder(ProcessBuilder const&) {
}
;

ProcessBuilder* ProcessBuilder::getInstance() {

	if (m_pInstance == NULL) {
		static ProcessBuilder m_Self;
		m_pInstance = &m_Self;

		for (int i = 33; i < 127; i++) {
			if (i != 64 || i != 108 || i != 73) {
				m_pInstance->_pid_list.push_back(i);
			}
		}
	}

	return m_pInstance;
}

char ProcessBuilder::generate_PID() {
	unsigned int seed = static_cast<unsigned int>(time(0));
	int _index = ((rand_r(&seed) % _pid_list.size()));
	char _pid = (char) _pid_list.at(_index);
	m_pInstance->_pid_list.erase(_pid_list.begin() + _index);

	return _pid;
}

/*
 *	FUNCTION
 */
struct process_t ProcessBuilder::generateProcess() {
	process_t _p;

	char id[2];
	_p._pid = generate_PID();

	int i = 0;
	id[0] = _p._pid;
	id[1] = '0';
	_p._seg_code = segment_t(id, CODE_SEG_PAGE_SIZE, 0, 0);

	id[0] = _p._pid;
	id[1] = '1';
	_p._seg_stack = segment_t(id, STACK_SEG_PAGE_SIZE, 0, 0);

	id[0] = _p._pid;
	id[1] = '2';
	_p._seg_heap = segment_t(id, HEAP_SEG_PAGE_SIZE, 0, 0);


	unsigned int seed = static_cast<unsigned int>(time(0));
//	int _index = ((rand_r(&seed) % _pid_list.size()));
//	int routine_count = (rand_r(&seed) % NUM_OF_PROC_SUBROUTINES) + 1;
	int routine_count = 2;
	generateProcRoutines(_p._seg_routines, routine_count, _p._pid);
	_p._burst_time = (rand_r(&seed) % (BURST_RANGE - 100)) + 100;

	m_pInstance->_cached_history.push_back(_p);

//	std::cout << "GENERATED PROC: " + _p._pid << std::endl;

	return _p;
}

void ProcessBuilder::generateProcRoutines(std::vector<segment_t> &list, int count, const char pid) {
	mem_page_t page_list[SUB_ROUTINE_SEG_PAGE_SIZE];
	for (int i = 0; i < count; i++) {
		char generated_id[2] = { pid, (char)(((int)'0')+i) };

		for (i = 0; i < SUB_ROUTINE_SEG_PAGE_SIZE; i++) {
			page_list[i] = mem_page_t();
		}

		list.push_back(segment_t(generated_id, SUB_ROUTINE_SEG_PAGE_SIZE, 0, 0));
	}
}

struct process_t ProcessBuilder::generateKernelProcess() {
	mem_page_t page_list[KERNEL_SIZE_IN_FRAMES];
	process_t _k_proc;

	_k_proc._pid = '@';

	for (int i = 0; i < KERNEL_SIZE_IN_FRAMES; i++) {
		page_list[i] = mem_page_t();
	}

	char id[2];
	id[0] = _k_proc._pid;
	id[1] = '0';
	_k_proc._seg_code = segment_t(id, KERNEL_SIZE_IN_FRAMES, 0, 0);
	_k_proc._can_swap_out = false;
	_k_proc._can_swap_in = true;

	if (USE_MEM_STRATEGY_BLOCKS) {
		_k_proc._base = 0;
		_k_proc._limit = KERNEL_SIZE_IN_BLOCKS;
		_k_proc._size = KERNEL_SIZE_IN_BLOCKS;
	}

	return _k_proc;
}

