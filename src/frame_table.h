/*
 * running_processes.h
 *
 *  Created on: Nov 17, 2013
 *      Author: visitor15
 */

#ifndef FRAME_TABLE_H_
#define FRAME_TABLE_H_

#include "process_builder.h"

class FrameTable {
private:
//	static std::vector<struct process_t> _running_queue(MEMORY_PAGE_COUNT);

	mem_frame_t _frame_table[MEMORY_FRAME_COUNT];

public:
	FrameTable();

	void initMemFrames();

	mem_frame_t* requestFreeFrame();
	mem_frame_t* requestFrameAt(const unsigned int index);

	bool writeDataToFrame(const char* data);
	bool clearFrameAt(const unsigned int index);
};

#endif /* FRAME_TABLE_H_ */
