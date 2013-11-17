#ifndef PROCESS_MANAGER_H_
#define PROCESS_MANAGER_H_

static const long MEMORY_SIZE = 1040;
static const int MAX_QUANTA = 50000;
static const long BURST_RANGE = 5000;
static const int PRINT_INTERVAL = 5000;
static const int NUM_OF_PROCESSES = 60;
static const int MAX_PROC_SIZE = 160;
static const double MAX_MEM_RATION = 0.65;

enum P_STATE {
	SLEEPING = 0, RUNNING = 1, WAITING = 2, STOPPED = 3
};

enum P_PRIORITY {

};

#endif /* PROCESS_MANAGER_H_ */
