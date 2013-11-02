

#ifndef PROCESS_BUILDER_H_
#define PROCESS_BUILDER_H_

#include <stdlib.h>
#include <time.h>
#include <vector>

inline static char generate_PID() {
	return (char)( (rand() % 52) + 65);
}

static const long BURST_RANGE = 5000;

enum P_STATE {
	SLEEPING = 0,
	RUNNING = 1,
	WAITING = 2,
	STOPPED = 3
};

enum P_PRIORITY {

};

/*
 *	STRUCT
 */
struct process_t {
	unsigned char _pid;

	unsigned long _base;
	unsigned long _limit;

	unsigned int _burst_time;
	unsigned int _priority;

	bool _can_swap_out;
	bool _can_swap_in;

	P_STATE _state;

	long size() {
		return (_limit - _base);
	}

	process_t() :
		_pid(generate_PID()),
		_base(0UL),
		_limit(0UL),
		_burst_time(0UL),
		_priority(0U),
		_state(SLEEPING) {
		_can_swap_out = false;
		_can_swap_in = true;
	}
};

inline static bool isKernelThread(const process_t proc) {
	return (proc._pid == '@');
}

/*
 *	CLASS				ProcessBuilder
 *
 *	Implemented as a singleton builder to generate base processes.
 */
static std::vector<process_t> _cached_history;
class ProcessBuilder {

private:
	ProcessBuilder() {};

public:
	ProcessBuilder(ProcessBuilder const&) {};
	static ProcessBuilder& getInstance() {
		static ProcessBuilder _instance;
		return _instance;
	}

	/*
	 *	FUNCTION
	 */
	struct process_t generateProcess() {
		process_t _p;

		_p._burst_time = (rand() % 5) + 1;

		_cached_history.push_back(_p);

		return _p;
	}

	struct process_t generateKernelProcess() {
		process_t _k_proc;

		_k_proc._pid = '@';
		_k_proc._base = 0;
		_k_proc._limit= 120;
		_k_proc._can_swap_out = false;
		_k_proc._can_swap_in = true;

		return _k_proc;
	}

};

#endif /* PROCESS_BUILDER_H_ */
