/*
 * process_manager.cpp
 *
 *  Created on: Nov 16, 2013
 *      Author: visitor15
 */

#include "process_builder.h"

ProcessBuilder::ProcessBuilder() {};
ProcessBuilder::ProcessBuilder(ProcessBuilder const&) {};

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
	int _index = ((rand() % _pid_list.size()));
	char _pid = (char) _pid_list.at(_index);
	m_pInstance->_pid_list.erase(_pid_list.begin() + _index);

	return _pid;
}

/*
 *	FUNCTION
 */
struct process_t ProcessBuilder::generateProcess() {
	process_t _p;

	_p._pid = generate_PID();
	_p._burst_time = (rand() % (BURST_RANGE - 100)) + 100;

	m_pInstance->_cached_history.push_back(_p);

	return _p;
}

struct process_t ProcessBuilder::generateKernelProcess() {
	process_t _k_proc;

	_k_proc._pid = '@';
	_k_proc._base = 0;
	_k_proc._limit = 120;
	_k_proc._size = 120;
	_k_proc._can_swap_out = false;
	_k_proc._can_swap_in = true;

	return _k_proc;
}

