/*
 * CS3242 Operating Systems
 * Fall 2013
 * Project 5: Swapping and Paging
 * Nick Champagne & John Mutabazi
 * Date: 11/22/2013
 * File: memory_full_exception.h
 */

#ifndef MEMORY_FULL_EXCEPTION_H_
#define MEMORY_FULL_EXCEPTION_H_


#include <exception>

class MemoryFullException: public std::exception {
public:
	virtual const char* what() const throw () {
		return "System memory FULL!";
	}
};


#endif /* MEMORY_FULL_EXCEPTION_H_ */
