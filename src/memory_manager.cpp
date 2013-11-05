/*
 * MemoryManager.cpp
 *
 *  Created on: Nov 1, 2013
 *      Author: visitor15
 */

#include "memory_manager.h"

MemoryManager::MemoryManager() {
	// TODO Auto-generated constructor stub

}

MemoryManager::~MemoryManager() {
	// TODO Auto-generated destructor stub
}

void MemoryManager::init() {
	std::cout << "Memory booting..." << std::endl;
	std::fill(_mem_array, _mem_array + MEMORY_SIZE, ' ');
}

bool MemoryManager::swapIn(process_t process) {
	bool _process_swapped = false;
	std::pair<long, long> _index_pair;

//	std::cout << "Swapping with MEM STRATEGY: " << MEM_STRATEGY << std::endl;

	switch (MEM_STRATEGY) {
	case 0: {
		_index_pair = canFitFirstFit(process);
//		std::cout << "First POS: " << _index_pair.first << " Second POS: " << _index_pair.second << std::endl;
		if (_index_pair.first != -1 && _index_pair.second != -1) {
			process._base = _index_pair.first;
			process._limit = _index_pair.second;

			_process_swapped = true;
		} else {
			_process_swapped = false;
		}

		break;
	}
	case 1: {
		_index_pair = canFitBestFit(process);
		if (_index_pair.first != -1 && _index_pair.second != -1) {
			process._base = _index_pair.first;
			process._limit = _index_pair.second;

			_process_swapped = true;
		} else {
			_process_swapped = false;
		}

		break;
	}
	case 2: {
		_index_pair = canFitWorstFit(process);
		if (_index_pair.first != -1 && _index_pair.second != -1) {
			process._base = _index_pair.first;
			process._limit = _index_pair.second;

			_process_swapped = true;
		} else {
			_process_swapped = false;
		}

		break;
	}
	}

	if (_process_swapped) {
		std::cout << "Swapping in " << process._pid << std::endl;
		std::cout << "Size: " << (process._limit - process._base) << std::endl;
//		std::cout << "BASE: " << process._base << " LIMIT: " << process._limit
//				<< std::endl;
		for (unsigned long i = process._base; i < process._limit; ++i) {
//			std::cout << process._pid;
			_mem_array[i] = process._pid;
		}

		process._burst_time = (rand() % 100) + 10;

		process._can_swap_in = false;
		process._can_swap_out = false;

		_running_queue.push_back(process);

		printMemMap();
	}

	return _process_swapped;
}

bool MemoryManager::swapOut(const process_t process) {

	if (isKernelThread(process)) {
		return false;
	}

	std::cout << "Swapping out " << process._pid << std::endl;
	std::cout << "Freed: " << (process._limit - process._base) << std::endl;

	for (unsigned int i = process._base; i < process._limit; ++i) {
		_mem_array[i] = ' ';
	}

	printMemMap();

	return true;
}

std::pair<long, long> MemoryManager::canFitFirstFit(process_t process) {

	unsigned long _size = process.size();

	for (unsigned long i = 0; i < MEMORY_SIZE; ++i) {
		if (_mem_array[i] == ' ') {
//			std::cout << "MEM OBJ: " << _mem_array[i];
			for (unsigned long p = i; p < MEMORY_SIZE; ++p) {
				if (_mem_array[p] == ' ') {

					if (p == (MEMORY_SIZE - 1)) {
//						std::cout << "The end has been reached!" << std::endl;
						return std::make_pair<long, long>(-1L, -1L);
					} else if ((p - i) == _size) {
						return std::make_pair<long, long>(i, p);
					}
				}
			}
		}
	}
	return std::make_pair<long, long>(-1L, -1L);
}

std::pair<long, long> MemoryManager::canFitBestFit(process_t process) {
	std::pair<long, long> _pair = std::make_pair<long, long>(-1L, -1L);
	std::vector<std::pair<long, std::pair<long, long> > > _pos_list;

	int _index = 0;
	long _size = process._size;
	int _free_length = MEMORY_SIZE;
	int _length = 0;
	for (long i = 0; i < MEMORY_SIZE; i++) {
		if (_mem_array[i] == ' ') {
			for (long p = i; p < MEMORY_SIZE; p++) {

				if (_mem_array[p] == ' ') {
					if ((p - i) == _size) {
						_free_length = MEMORY_SIZE;
						_length = 0;
						for (long k = p; k < MEMORY_SIZE; k++) {
							if (_mem_array[k] == ' ') {
								++_length;
							}

							if (_mem_array[k] != ' '
									|| k == (MEMORY_SIZE - 1)) {
									_pos_list.push_back(std::make_pair<long, std::pair<long, long> >(_length, std::make_pair<long, long>(i, p + 1)));
								}
						}
					}
				} else {
					break;
				}
			}
		}
	}

	if(_pos_list.size() > 0) {
		for(int i = 1; i < _pos_list.size(); i++) {
			_index = ((_pos_list.at(i) < _pos_list.at(_index)) ? i : _index);
		}

		return _pos_list.at(_index).second;
	}

	return _pair;
}

std::pair<long, long> MemoryManager::canFitWorstFit(process_t process) {
	std::pair<long, long> _pair = std::make_pair<long, long>(-1L, -1L);
	std::vector<std::pair<long, std::pair<long, long> > > _pos_list;

	int _index = 0;
	long _size = process._size;
	int _free_length = MEMORY_SIZE;
	int _length = 0;
	for (long i = 0; i < MEMORY_SIZE; i++) {
		if (_mem_array[i] == ' ') {
			for (long p = i; p < MEMORY_SIZE; p++) {

				if (_mem_array[p] == ' ') {
					if ((p - i) == _size) {
						_free_length = MEMORY_SIZE;
						_length = 0;
						for (long k = p; k < MEMORY_SIZE; k++) {
							if (_mem_array[k] == ' ') {
								++_length;
							}

							if (_mem_array[k] != ' '
									|| k == (MEMORY_SIZE - 1)) {
									_pos_list.push_back(std::make_pair<long, std::pair<long, long> >(_length, std::make_pair<long, long>(i, p + 1)));
								}
						}
					}
				} else {
					break;
				}
			}
		}
	}

	if(_pos_list.size() > 0) {
		for(int i = 1; i < _pos_list.size(); i++) {
			_index = ((_pos_list.at(i) > _pos_list.at(_index)) ? i : _index);
		}

		return _pos_list.at(_index).second;
	}

	return _pair;
}

void MemoryManager::addToBackingStore(process_t process) {
	_back_store.push_back(process);
}

struct process_t MemoryManager::pullNextFromBackStore() {
	return _back_store.at(0);
}

void MemoryManager::addToReadyQueue(const process_t process) {
	_ready_queue.push_back(process);
}

struct process_t MemoryManager::pullNextFromReadyQueue() {
	return _ready_queue.at(0);
}

bool MemoryManager::hasReadyProcess() {
	return _ready_queue.size() > 0;
}

void MemoryManager::executeCycle() {
//	std::cout << "RUNNING QUEUE SIZE: " << _running_queue.size() << std::endl;
//	std::cout << "READY QUEUE SIZE: " << _ready_queue.size() << std::endl;

	for (unsigned long i = 0; i < _running_queue.size(); ++i) {

		process_t& _proc = _running_queue[i];
		_proc._burst_time = (_proc._burst_time - 1);
		if (_proc._burst_time == 0) {
			_proc._can_swap_out = true;
		}
	}

	if (hasReadyProcess()) {
		if (swapIn(pullNextFromReadyQueue())) {
			_ready_queue.erase(_ready_queue.begin());
		} else {

			std::cout << "Could not fit " << pullNextFromReadyQueue()._pid << std::endl;
			std::cout << "Size: " << pullNextFromReadyQueue()._size;

			bool proc_swapped = false;
			for (unsigned long i = 0; i < _running_queue.size(); i++) {
				process_t& _proc = _running_queue[i];
				if (_proc._can_swap_out) {
					if (swapOut(_proc)) {
						_proc._can_swap_out = false;
						_proc._can_swap_in = true;
						_ready_queue.push_back(_running_queue.at(i));
						_running_queue.erase(_running_queue.begin() + i);
						proc_swapped = true;
//						std::cout << "Swapped out: " << i << std::endl;
					}
				}

				if(proc_swapped) {
					proc_swapped = false;
					break;
				}
			}
		}
	}

}

bool MemoryManager::hasProcRegistered(char _pid) {
	bool _inRunQueue = false;
	bool _inReadyQueue = false;

//	std::cout << "CHECKING PID: " << _pid << std::endl;

	unsigned long i = 0;
	for (i = 0; i < _running_queue.size(); i++) {
		if (_pid == _running_queue.at(i)._pid) {
//			std::cout << _pid << " && " << _running_queue.at(i)._pid << " match!" << std::endl;
			_inRunQueue = true;
			break;
		}
	}

	for (i = 0; i < _ready_queue.size(); i++) {
		if (_pid == _ready_queue.at(i)._pid) {
//			std::cout << "2: " << _pid << " && " << _ready_queue.at(i)._pid << " match!" << std::endl;
			_inReadyQueue = true;
			break;
		}
	}

	return (_inRunQueue || _inReadyQueue) ? true : false;
}

int i = 0;
int _print_state = 0;
void MemoryManager::printMemMap() {
	std::cout << "\n\n===================================" << std::endl;
	std::cout << "\tMEMORY MAP" << std::endl;
	std::cout << "===================================" << std::endl;

	i = 0;
	for (; i <= MEMORY_SIZE; i++) {
		switch (_print_state) {
		case 0: {
//			if ((i > 0) && (i % 10 == 0)) {
//				std::cout << i;
//			}
//			else if ((i < 100) && (i % 8 != 0)) {
//				std::cout << " ";
//			} else if ((i < 1000) && (i % 7 != 0)) {
//				std::cout << " ";
//			} else if ((i < 10000) && (i % 6 != 0)) {
//				std::cout << " ";
//			}

			if ((i > 0) && (i % 80 == 0)) {
				_print_state += 1;
				std::cout << std::endl;
				i = (i - 80);
			}

			continue;
		}
		case 1: {

			if (i % 5 == 0) {
				if (i % 10 == 0) {
					std::cout << "|";
				} else {
					std::cout << "+";
				}
			} else {
				std::cout << "-";
			}

			if ((i > 0) && (i % 80 == 0)) {
				_print_state += 1;
				std::cout << std::endl;
				i = (i - 80);
			}

			continue;
		}
		case 2: {

			std::cout << _mem_array[i];

			if ((i > 0) && (i % 80 == 0)) {
				_print_state = 0;
				std::cout << std::endl;
			}

			continue;
		}
		}
	}
}

