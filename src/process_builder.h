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
#include "process_manager.h"

/*
 *	STRUCT
 */
struct process_t {
	char _pid;

	unsigned long _base;
	unsigned long _limit;
	long _size;

	unsigned int _burst_time;
	unsigned int _priority;

	bool _can_swap_out;
	bool _can_swap_in;

	P_STATE _state;

	long size() {
		return _size;
	}

	process_t() :
			_pid(' '), _base(0UL), _limit(0UL), _size(0), _burst_time(0UL), _priority(
					0U), _state(SLEEPING) {
		_can_swap_out = false;
		_can_swap_in = true;
	}
};

inline static bool isKernelProcess(const process_t proc) {
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

	struct process_t generateKernelProcess();

};

static ProcessBuilder* m_pInstance;

#endif /* PROCESS_BUILDER_H_ */
