/*
 * CS3242 Operating Systems
 * Fall 2013
 * Project 5: Swapping and Paging
 * Nick Champagne & John Mutabazi
 * Date: 11/22/2013
 * File: page_fault_exception.h
 */

#ifndef PAGE_FAULT_EXCEPTION_H_
#define PAGE_FAULT_EXCEPTION_H_

#include <exception>

/*
 * 		CLASS		-Custom exception designed to deliver the
 * 					index of the faulty memory page.
 */
class PageFaultException: public std::exception {
public:
	int _index;
	PageFaultException(int index) {
		_index = index;
	}

	virtual const char* what() const throw () {
		return "Invalid memory access!";
	}
};

#endif /* PAGE_FAULT_EXCEPTION_H_ */
