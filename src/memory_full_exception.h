/*
 * memory_full_exception.h
 *
 *  Created on: Nov 18, 2013
 *      Author: visitor15
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
