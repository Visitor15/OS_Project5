/*
 * CS3242 Operating Systems
 * Fall 2013
 * Project 5: Swapping and Paging
 * Nick Champagne & John Mutabazi
 * Date: 11/22/2013
 * File: frame_table.h
 */

#ifndef FRAME_TABLE_H_
#define FRAME_TABLE_H_

#include "process_builder.h"

class FrameTable {
private:
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
