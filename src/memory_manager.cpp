/*
 * CS3242 Operating Systems
 * Fall 2013
 * Project 5: Swapping and Paging
 * Nick Champagne & John Mutabazi
 * Date: 11/22/2013
 * File: memory_manager.cpp
 */

#include "memory_manager.h"

MemoryManager::MemoryManager() {
	MEM_STRATEGY = 0;
	_m_cycle_num = 0;
}

MemoryManager::~MemoryManager() {
	// TODO Auto-generated destructor stub
}

void MemoryManager::init() {
	std::cout << "Memory booting..." << std::endl;

	f_table.initMemFrames();
	back_store.initMemPages();

	for (int i = 0; i < MEMORY_SIZE; i++) {
		_mem_array[i] = ' ';
	}
	std::fill(_mem_array, _mem_array + MEMORY_SIZE, ' ');
}

bool MemoryManager::swapIn(process_t process) {
	bool _process_swapped = false;
	std::pair<long, long> _index_pair;

	switch (MEM_STRATEGY) {
	case 0: {
		_index_pair = canFitFirstFit(process);
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
		for (long i = process._base; i < process._limit; ++i) {
			_mem_array[i] = process._pid;
		}

		process._burst_time = (rand() % (BURST_RANGE - 100)) + 100;

		process._can_swap_in = false;
		process._can_swap_out = false;

		_running_queue.push_back(process);
	}

	return _process_swapped;
}

bool MemoryManager::swapOut(const process_t process) {
	if (isKernelProcess(process)) {
		return false;
	}

	for (unsigned int i = process._base; i < process._limit; ++i) {
		_mem_array[i] = ' ';
	}

	return true;
}

std::pair<long, long> MemoryManager::canFitFirstFit(process_t process) {

	unsigned long _size = process._size;

	for (unsigned long i = 0; i < MEMORY_SIZE; ++i) {
		if (_mem_array[i] == ' ') {
			for (unsigned long p = i; p < MEMORY_SIZE; p++) {
				if (_mem_array[p] == ' ') {

					if (p == (MEMORY_SIZE - 1)) {
						return std::make_pair<long, long>(-1L, -1L);
					} else if ((p - i) == _size) {
						return std::make_pair<long, long>(i, p);
					}
				} else {
					break;
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
								_pos_list.push_back(
										std::make_pair<long,
												std::pair<long, long> >(_length,
												std::make_pair<long, long>(i,
														p + 1)));
							}
						}
					}
				} else {
					break;
				}
			}
		}
	}

	if (_pos_list.size() > 0) {
		for (unsigned int i = 1; i < _pos_list.size(); i++) {
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
								_pos_list.push_back(
										std::make_pair<long,
												std::pair<long, long> >(_length,
												std::make_pair<long, long>(i,
														p + 1)));
							}
						}
					}
				} else {
					break;
				}
			}
		}
	}

	if (_pos_list.size() > 0) {
		for (unsigned int i = 1; i < _pos_list.size(); i++) {
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

void MemoryManager::touchNextReadyProc() {
	if (_ready_queue.size() > 0) {
		if (touchProcess(&_ready_queue.at(0))) {
			_running_queue.push_back(_ready_queue.at(0));
			_ready_queue.erase(_ready_queue.begin());
		}
	}
}

bool MemoryManager::hasReadyProcess() {
	return _ready_queue.size() > 0;
}

void MemoryManager::executeCycle() {
	++_m_cycle_num;

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
					}
				}

				if (proc_swapped) {
					proc_swapped = false;
					break;
				}
			}
		}
	}

	double _ratio = getMemRatio();
	if (_ratio > MAX_MEM_RATION) {

		std::cout << "\tRUNNING COMPACTION - MAX RATIO HIT (" << _ratio << ")"
				<< std::endl;

		doCompaction();
		printMemMap();
	}

}

std::pair<int, int> MemoryManager::getMemFreeUsedPair() {
	std::pair<int, int> _pair;
	int _used = 0;
	int _holes = 0;
	for (int i = 0; i < MEMORY_SIZE; i++) {
		if (_mem_array[i] == ' ') {
			for (int p = i; p < MEMORY_SIZE; p++) {
				if (p != ' ') {
					++_holes;
					i = p;
					break;
				}
			}
		} else {
			for (int k = i; k < MEMORY_SIZE; k++) {
				if (k == ' ') {
					++_used;
					i = k;
					break;
				}
			}
		}
	}

	_pair = std::make_pair<int, int>(_holes, _used);

	return _pair;
}

double MemoryManager::getMemRatio() {
	std::pair<int, int> freeUsedPair = getMemFreeUsedPair();
	double n1 = (double) freeUsedPair.first;
	double n2 = (double) freeUsedPair.second;

	double gdc;
	while (n2 != 0) {
		int mod = (int) n1 % (int) n2;
		n1 = n2;
		n2 = mod;
	}
	gdc = n1;
	double _free = (double) freeUsedPair.first / gdc;
	double _used = (double) freeUsedPair.second / gdc;

	return _used / _free;
}

bool MemoryManager::hasProcRegistered(char _pid) {
	bool _inRunQueue = false;
	bool _inReadyQueue = false;

	unsigned long i = 0;
	for (i = 0; i < _running_queue.size(); i++) {
		if (_pid == _running_queue.at(i)._pid) {
			_inRunQueue = true;
			break;
		}
	}

	for (i = 0; i < _ready_queue.size(); i++) {
		if (_pid == _ready_queue.at(i)._pid) {
			_inReadyQueue = true;
			break;
		}
	}

	return (_inRunQueue || _inReadyQueue) ? true : false;
}

process_t MemoryManager::getLargestProcess() {
	process_t proc;
	proc._size = -1L;
	int i;
	if (_running_queue.size() > 0) {
		proc = _running_queue.at(0);
		for (i = 0; i < _running_queue.size(); i++) {
			proc = (_running_queue.at(i)._size > proc._size) ?
					(_running_queue.at(i)) : proc;
		}
	}

	return proc;
}

process_t MemoryManager::getSmallestProcess() {
	process_t proc;
	proc._size = -1L;
	int i;
	if (_running_queue.size() > 0) {
		proc = _running_queue.at(0);
		for (i = 0; i < _running_queue.size(); i++) {
			proc = (proc._size > _running_queue.at(i)._size) ?
					(_running_queue.at(i)) : proc;
		}
	}

	return proc;
}

long MemoryManager::getNumberOfFreeBlocks() {
	long val = 0L;

	for (int i = 0; i < MEMORY_SIZE; i++) {
		if (_mem_array[i] == ' ') {
			++val;
		}
	}

	return val;
}

bool MemoryManager::doCompaction() {
	std::vector<process_t> tmpList;
	bool result = false;
	process_t proc;

	unsigned int i = 0;
	for (i = 0; i < _running_queue.size(); i++) {
		proc = _running_queue.at(i);
		if (isKernelProcess(proc)) {
			continue;
		}

		swapOut(proc);
	}

	for (i = 0; i < _running_queue.size(); i++) {
		proc = _running_queue.at(i);
		if (isKernelProcess(proc)) {
			continue;
		}

		for (int k = 0; k < MEMORY_SIZE; k++) {
			if (_mem_array[k] == ' ') {
				for (int p = 0; p < proc._size; p++) {
					_mem_array[k] = proc._pid;
					k++;
				}
				break;
			}
		}
	}

	return result;
}

int i = 0;
int _print_state = 0;
void MemoryManager::printMemMap() {
	formatDetails();

	i = 0;
	for (; i <= MEMORY_SIZE; i++) {
		switch (_print_state) {
		case 0: {

			if (i > 0) {
				if (i % 5 == 0) {
					if (i % 10 == 0) {
						if (i < 100) {
							std::cout << "        " << i;
						} else if (i < 1000) {
							std::cout << "       " << i;
						} else {
							std::cout << "      " << i;
						}
					}
				}
			}

			if ((i > 0) && (i % 80 == 0)) {
				_print_state += 1;
				std::cout << "\n";
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
				std::cout << "" << std::endl;
			}

			continue;
		}
		}
	}

	std::cout
			<< "================================================================================"
			<< std::endl;
}

void MemoryManager::printMemFrameMap() {
	i = 0;
	_print_state = 0;
	formatDetails();

	i = 0;
	for (; i <= MEM_SIZE_IN_FRAMES; i++) {
		switch (_print_state) {
		case 0: {

			if (i > 0) {
				if (i % 5 == 0) {
					if (i % 10 == 0) {
						if (i < 100) {
							std::cout << "         " << i;
						} else if (i < 1000) {
							std::cout << "        " << i;
						} else {
							std::cout << "       " << i;
						}
					}
				}
			}

			if ((i > 0) && (i % 49 == 0)) {
				_print_state += 1;
				std::cout << "\n";
				i = (i - 49);
			}

			continue;
		}
		case 1: {

			if (i % 5 == 0) {
				if (i % 10 == 0) {
					std::cout << "||";
				} else {
					std::cout << "++";
				}
			} else {
				std::cout << "--";
			}

			if ((i > 0) && (i % 49 == 0)) {
				_print_state += 1;
				std::cout << std::endl;
				i = (i - 49);
			}

			continue;
		}
		case 2: {

			std::cout << f_table.requestFrameAt(i)->_id;

			if ((i > 0) && (i % 49 == 0)) {
				_print_state = 0;
				std::cout << "" << std::endl;
			}

			continue;
		}
		}
	}

	std::cout
			<< "================================================================================"
			<< std::endl;
}

void MemoryManager::formatDetails() {
	std::string val;
	std::string tmp;
	std::stringstream _stream;
	process_t proc;

	std::cout
			<< "\n================================================================================"
			<< std::endl;
	std::cout << "MEMORY MAP" << std::endl;
	std::cout
			<< "================================================================================"
			<< std::endl;

	val += "QUANTA ELAPSED:\t";

	_stream << _m_cycle_num;
	_stream >> tmp;
	val.append(tmp);

	val.append("\n\n");

	val.append("MEMORY:\t\t");
	_stream.clear();
	_stream << MEMORY_SIZE;
	tmp.clear();
	_stream >> tmp;
	val.append(tmp);

	val.append("\t\tUSED:\t");
	_stream.clear();
	_stream << (MEMORY_SIZE - getNumberOfFreeBlocks());
	tmp.clear();
	_stream >> tmp;
	val.append(tmp);

	val.append("\t\tFREE:\t\t");
	_stream.clear();
	_stream << getNumberOfFreeBlocks();
	tmp.clear();
	_stream >> tmp;
	val.append(tmp);

	val.append("\n");

	val.append("PROCESSES:\t");
	_stream.clear();
	_stream << (_running_queue.size() + _ready_queue.size());
	tmp.clear();
	_stream >> tmp;
	val.append(tmp);

	val.append("\t\tLOADED:\t");
	_stream.clear();
	_stream << _running_queue.size();
	tmp.clear();
	_stream >> tmp;
	val.append(tmp);

	val.append("\t\tUNLOADED:\t");
	_stream.clear();
	_stream << _ready_queue.size();
	tmp.clear();
	_stream >> tmp;
	val.append(tmp);

	val.append("\n");

	val.append("FREE BLOCKS:\t");
	_stream.clear();
	_stream << getNumberOfFreeBlocks();
	tmp.clear();
	_stream >> tmp;
	val.append(tmp);

	proc = getLargestProcess();
	val.append("\t\tLARGEST:");
	_stream.clear();
	_stream << proc._size;
	tmp.clear();
	_stream >> tmp;
	tmp.append(" (");
	tmp += proc._pid;
	tmp.append(")");
	val.append(tmp);

	proc = getSmallestProcess();
	val.append("\t\tSMALLEST:\t");
	_stream.clear();
	_stream << proc._size;
	tmp.clear();
	_stream >> tmp;
	tmp.append(" (");
	tmp += proc._pid;
	tmp.append(")");
	val.append(tmp);

	val.append("\tBL/PROCS RATIO:");
	_stream.clear();
	_stream << getMemRatio();
	tmp.clear();
	_stream >> tmp;
	val.append(tmp);

	std::cout << val << std::endl;

	std::cout
			<< "================================================================================"
			<< std::endl;
}

void MemoryManager::executeCycleNonContigious() {
	if (_running_queue.size() > 0) {
		this->touchNextReadyProc();

//		touchProcess(&(_running_queue.at((rand() % _running_queue.size()))));
	} else {
		std::cout << "No processes found in running queue!" << std::endl;
	}
}

bool MemoryManager::loadKernelProcessInMemory(struct process_t proc) {

	_running_queue.push_back(proc);
	touchSegment(&proc._seg_code, -1);

	return false;
}

bool MemoryManager::loadPage(struct mem_page_t* page) {

	mem_frame_t* frame = f_table.requestFreeFrame();

	std::memcpy(page->p_frame._id, page->_id, 2);
	std::memcpy(frame->_id, page->_id, 2);
	(*page)._frame_index = (*page).p_frame._index;

	return true;
}

bool MemoryManager::touchProcess(struct process_t* proc) {

	touchSegment(&proc->_seg_code, -1);
	touchSegment(&proc->_seg_heap, -1);
	touchSegment(&proc->_seg_stack, -1);

	if (proc->_num_routines > 0) {
		int index = rand() % proc->_num_routines;

		std::cout << "TOUCHING SUBROUTINE SEGMENT: "
				<< *proc->_seg_routines.at(index)._id << " index: " << index
				<< std::endl;

		touchSegment(&proc->_seg_routines.at(index), index);
	}

	return true;
}

bool MemoryManager::touchSegment(struct segment_t* seg, int opt_index) {

	bool page_fault;

	do {
		try {
			page_fault = false;
			seg->touch();
		} catch (PageFaultException &e) {
			page_fault = true;
			try {
				mem_page_t& page = *back_store.requestFreePage();
				page._seg_type = seg->_seg_type;
				page._index = e._index;
				page._seg_list_index = e._index;
				seg->seg_pages.at(e._index) = page;
			} catch (MemoryFullException &error) {
				int _index = rand() % BACKING_STORE_PAGE_COUNT;

				mem_page_t m_page = *back_store.requestPageAt(_index);
				process_t* proc = getProcessByPID(seg->_id[0]);

				if (m_page._index < 0) {
					m_page._index = 0;
				}
				if (m_page._seg_list_index < 0) {
					m_page._seg_list_index = 0;
				}

				if (proc) {
					mem_page_t new_page = mem_page_t();
					new_page._index = m_page._index;
					new_page._seg_type = m_page._seg_type;
					new_page._seg_list_index = m_page._seg_list_index;
					switch (seg->_seg_type) {
					case SEG_TYPE_CODE: {
						(*proc)._seg_code.seg_pages.at(e._index) = new_page;
						break;
					}
					case SEG_TYPE_STACK: {
						(*proc)._seg_stack.seg_pages.at(e._index) = new_page;
						break;
					}
					case SEG_TYPE_HEAP: {
						(*proc)._seg_heap.seg_pages.at(e._index) = new_page;
						break;
					}
					case SEG_TYPE_ROUTINE: {
						(*proc)._seg_routines.at(opt_index).seg_pages.at(
								e._index) = new_page;
						break;
					}
					default: {
						// Nothing
					}
					}
				}
			}

			std::memcpy(seg->seg_pages[e._index]._id, seg->_id, 2);

			loadPage(&seg->seg_pages.at(e._index));

		}
	} while (page_fault);

	return true;
}

struct process_t* MemoryManager::getProcessByPID(char pid) {
	unsigned int i = 0;
	for (i = 0; i < _ready_queue.size(); i++) {
		if (_ready_queue.at(i)._pid == pid) {
			return &_ready_queue.at(i);
		}
	}

	for (i = 0; i < _running_queue.size(); i++) {
		if (_running_queue.at(i)._pid == pid) {
			return &_running_queue.at(i);
		}
	}

	return 0;
}

/*************************************************
 *	CLASS	BackingStore
 *************************************************/
BackingStore::BackingStore() {

}

void BackingStore::initMemPages() {
	for (int i = 0; i < BACKING_STORE_PAGE_COUNT; i++) {
		BackingStore::_backing_store[i] = mem_page_t(i);
	}
}

struct mem_page_t* BackingStore::requestFreePage() {

	for (int i = 0; i < BACKING_STORE_PAGE_COUNT; i++) {
		if (!_backing_store[i]._is_active) {
			_backing_store[i]._is_active = true;
			return &_backing_store[i];
		}
	}
	std::cout << "THROWING Out of memory!" << std::endl;
	throw MemoryFullException();
}

struct mem_page_t* BackingStore::requestPageAt(const unsigned int index) {
	return &_backing_store[index];
}

void BackingStore::clearPageAt(const unsigned int index) {
	_backing_store[index] = mem_page_t();
}

int BackingStore::getNumOfUniqueProcsInBackingStore() {
	return BACKING_STORE_PAGE_COUNT;
}

int BackingStore::getFreePageCount() {
	return BACKING_STORE_PAGE_COUNT;
}

/*************************************************
 *	CLASS	FrameTable
 *************************************************/
FrameTable::FrameTable() {

}

void FrameTable::initMemFrames() {
	for (int i = 0; i < MEMORY_FRAME_COUNT; i++) {
		FrameTable::_frame_table[i] = mem_frame_t(i);
	}
}

struct mem_frame_t* FrameTable::requestFreeFrame() {

	for (int i = 0; i < MEMORY_FRAME_COUNT; i++) {
		if (!_frame_table[i]._active) {
			FrameTable::_frame_table[i]._active = true;
			return &FrameTable::_frame_table[i];
		}
	}

	int _index;
	do {
		_index = (rand() % MEMORY_FRAME_COUNT);
	} while (std::strstr((char*) _frame_table[_index]._id, "@") != NULL);

	return &FrameTable::_frame_table[_index];
}

struct mem_frame_t* FrameTable::requestFrameAt(const unsigned int index) {
	return &FrameTable::_frame_table[index];
}

