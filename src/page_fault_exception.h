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
	int _index = -1;
	PageFaultException(int index) {
		_index = index;
	}

	virtual const char* what() const throw () {
		return "Invalid memory access!";
	}
};

#endif /* PAGE_FAULT_EXCEPTION_H_ */
