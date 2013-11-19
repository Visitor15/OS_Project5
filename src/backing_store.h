/*
 * backing_store.h
 *
 *  Created on: Nov 17, 2013
 *      Author: visitor15
 */

#ifndef BACKING_STORE_H_
#define BACKING_STORE_H_

#include <stdio.h>
#include <vector>
#include "process_builder.h"

class BackingStore {
private:
	mem_page_t _backing_store[BACKING_STORE_PAGE_COUNT];

public:
	BackingStore();
	mem_page_t* requestFreePage();
	mem_page_t* requestPageAt(const unsigned int index);

	void initMemFrames();

	void clearPageAt(const unsigned int index);

	int getNumOfUniqueProcsInBackingStore();
	int getFreePageCount();
};

#endif /* BACKING_STORE_H_ */
