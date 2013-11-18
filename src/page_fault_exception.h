/*
 * page_fault_exception.h
 *
 *  Created on: Nov 17, 2013
 *      Author: visitor15
 */

#ifndef PAGE_FAULT_EXCEPTION_H_
#define PAGE_FAULT_EXCEPTION_H_

#include <exception>

class PageFaultException: public std::exception {
public:
	struct mem_page_t* mem_page;
	PageFaultException(struct mem_page_t* page) {
		mem_page = page;
	}

	virtual const char* what() const throw () {
		return "Invalid memory access!";
	}
};

#endif /* PAGE_FAULT_EXCEPTION_H_ */
