/*
 * CS3242 Operating Systems
 * Fall 2013
 * Project 5: Swapping and Paging
 * Nick Champagne & John Mutabazi
 * Date: 11/22/2013
 * File: backing_store.h
 */
#ifndef BACKING_STORE_H_
#define BACKING_STORE_H_

#include <stdio.h>
#include <vector>
#include "process_builder.h"
#include "memory_manager.h"

/*
 *		CLASS		-Designed to be the manager of the backing_store array.
 */
class BackingStore {
private:
	mem_page_t _backing_store[BACKING_STORE_PAGE_COUNT];

public:
	BackingStore();
	mem_page_t* requestFreePage();
	mem_page_t* requestPageAt(const unsigned int index);

	void initMemPages();

	void clearPageAt(const unsigned int index);

	int getNumOfUniqueProcsInBackingStore();
	int getFreePageCount();
};

#endif /* BACKING_STORE_H_ */
