/*
 * CS3242 Operating Systems
 * Fall 2013
 * Project 5: Swapping and Paging
 * Nick Champagne & John Mutabazi
 * Date: 11/22/2013
 * File: process_manager.h
 */

#ifndef PROCESS_MANAGER_H_
#define PROCESS_MANAGER_H_

static const long 	MEMORY_SIZE 				= 1040;
static const int	MEM_SIZE_IN_FRAMES 			= 280;
static const long 	FRAME_SIZE_IN_BYTES 		= 4096;
static const long	PAGE_SIZE_IN_BYTES 			= 4096;
static const int 	MAX_QUANTA 					= 100;
static const long 	BURST_RANGE 				= 5000;
static const int 	PRINT_INTERVAL 				= 2;
static const int 	NUM_OF_PROCESSES 			= 60;
static const int 	MAX_PROC_SIZE 				= 160;
static const double MAX_MEM_RATION 				= 0.65;
static const long	KERNEL_SIZE_IN_BLOCKS 		= 120;
static const long	KERNEL_SIZE_IN_FRAMES 		= 20;
static const int	NUM_OF_PROC_SUBROUTINES		= 5;
static const int	PROC_SUBROUTINE_NUM_OFFSET 	= 3;
static const int	BACKING_STORE_PAGE_COUNT 	= 720;
static const int	MEMORY_FRAME_COUNT 			= 280;
static const int	CODE_SEG_PAGE_SIZE 			= 2;
static const int	STACK_SEG_PAGE_SIZE 		= 3;
static const int	HEAP_SEG_PAGE_SIZE 			= 5;
static const int	SUB_ROUTINE_SEG_PAGE_SIZE 	= 2;
static const int	MAX_PROC_PAGE_SIZE 			= 20;
static const int	MAX_PROC_RUNTIME 			= 300;
static const int	SEG_TYPE_CODE 				= 0;
static const int	SEG_TYPE_STACK 				= 1;
static const int	SEG_TYPE_HEAP 				= 2;
static const int	SEG_TYPE_ROUTINE 			= 3;

enum P_STATE {
	SLEEPING 	= 0,
	RUNNING 	= 1,
	WAITING 	= 2,
	STOPPED 	= 3
};

enum P_PRIORITY {

};

#endif /* PROCESS_MANAGER_H_ */
