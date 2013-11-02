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
	bool _can_fit = false;

	switch (MEM_STRATEGY) {
	case 0: {
		std::pair<long, long> _index_pair = canFitFirstFit(process);
		if (_index_pair.first != -1 && _index_pair.second != -1) {
			process._base = _index_pair.first;
			process._limit = _index_pair.second;

			_can_fit = true;
		} else {
			_can_fit = false;
		}

		break;
	}
	case 1: {
		_can_fit = canFitBestFit(process);
		break;
	}
	case 3: {
		_can_fit = canFitWorstFit(process);
		break;
	}
	}

	if (_can_fit) {
		std::cout << "Swapping in " << process._pid << std::endl;
		std::cout << "Size: " << (process._limit - process._base) << std::endl;
//		std::cout << "BASE: " << process._base << " LIMIT: " << process._limit
//				<< std::endl;
		for (int i = process._base; i < process._limit; ++i) {
//			std::cout << process._pid;
			_mem_array[i] = process._pid;
		}

		_running_queue.push_back(process);

		return true;
	} else {
		for (int i = 0; i < _running_queue.size(); ++i) {
			if (_running_queue.at(i)._can_swap_out) {
				swapOut(_running_queue.at(i));
				_ready_queue.push_back(_running_queue.at(i));
				_running_queue.erase(_running_queue.begin() + i);

				return false;
			}
		}

	}

	return false;
}

bool MemoryManager::swapOut(const process_t process) {

	std::cout << "Swapping out " << process._pid << std::endl;
	std::cout << "Freed: " << (process._limit - process._base) << std::endl;

	for (unsigned int i = process._base; i < process._limit; ++i) {
		_mem_array[i] = ' ';
	}

	return true;
}

std::pair<long, long> MemoryManager::canFitFirstFit(process_t process) {

	long _size = process.size();

	for (int i = 0; i < MEMORY_SIZE; ++i) {
		if (_mem_array[i] == ' ') {
//			std::cout << "MEM OBJ: " << _mem_array[i];
			for (int p = i; p < MEMORY_SIZE; ++p) {
				if (_mem_array[p] == ' ') {

					if (p == (MEMORY_SIZE - 1)) {
//						std::cout << "The end has been reached!" << std::endl;
						return std::make_pair<long, long>(-1L, -1L);
					}

					if ((p - i) == _size) {
						process._base = i;
						process._limit = p;

						return std::make_pair<long, long>(i, p);
					}
				}
			}
		}
	}
	return std::make_pair<long, long>(-1L, -1L);
}

bool MemoryManager::canFitBestFit(process_t process) {

	return false;
}

bool MemoryManager::canFitWorstFit(process_t process) {

	return false;
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

void MemoryManager::executeCycle() {
//	std::cout << "Running Queue size: " << _running_queue.size() << std::endl;
	for (int i = 0; i < _running_queue.size(); ++i) {

		process_t& _proc = _running_queue[i];
		_proc._burst_time = (_proc._burst_time - 1);
		if (_proc._burst_time == 0) {
//			std::cout << "Process: " << _proc._pid << " Burst time is 0"
//					<< std::endl;
			_proc._can_swap_out = true;
		}
	}

	for (int i = 0; i < _ready_queue.size(); ++i) {
		if (swapIn(_ready_queue.at(i))) {
			_ready_queue.erase(_ready_queue.begin() + i);
			break;
		}
	}

}

