//Implementation of tests designed to test the stack implementations

#ifndef STACKTEST
#define STACKTEST

#include "SimpleTimer.hpp"
#include "Locking.h"
#include "LockFree.h"

#include <iostream>
#include <thread>
#include <assert.h>
#include <fstream>

#define OUTPUT_FILE "timing_analysis.csv"
#define OUTER_ITERATIONS 1024
#define MAX_INNER_ITERATIONS 1024

static Container::Stack* stack = 0;
static int iterations = 1;

//Push to the static stack
void StackTThread()
{
	SinglyLinkedObject* obj;
	for(int i = 0; i < OUTER_ITERATIONS; ++i)
	{
		for(int j = 0; j < iterations; ++j)
		{
			obj = (SinglyLinkedObject*)malloc(sizeof(void*));
			while(LockFree::Stack::GetCounter(obj)) {
				free(obj);
				obj = (SinglyLinkedObject*)malloc(sizeof(void*));
				std::cout << "retry alloc" << std::endl;
			}
			stack->Push(obj);
		}
		for(int j = 0; j < iterations; ++j)
		{
			obj = stack->Pop();
			free(obj);
		}
	}
}

//Push and then pop on the static stack
void StackSThread()
{
	for(int i = 0; i < OUTER_ITERATIONS; ++i)
	{
		for(int j = 0; j < 1024; ++j)
		{
			SinglyLinkedObject* obj = (SinglyLinkedObject*)malloc(sizeof(void*));
			while(LockFree::Stack::GetCounter(obj)) {
				free(obj);
				obj = (SinglyLinkedObject*)malloc(sizeof(void*));
				std::cout << "retry alloc" << std::endl;
			}
			stack->Push(obj);
			obj = stack->Pop();
			assert(obj);
			free(obj);
		}
	}
}

void doTest(const int numThreads, std::thread* threads, const bool stest, const bool locking)
{
	iterations = 1;
	do
	{
		SimpleTimer timer;
		timer.Start();
		//initialize testing threads, then wait for all threads to finish
		if(stest)
		{
			for(int i = 0; i < numThreads; ++i)
			{
				threads[i] = std::thread(StackSThread);
			}
		}
		else 
		{
			for(int i = 0; i < numThreads; ++i)
			{
				threads[i] = std::thread(StackTThread);
			}
		}
		for(int i = 0; i < numThreads; ++i)
		{
			threads[i].join();
		}
		timer.Stop();
		while(!stack->IsEmpty())
		{
			delete (stack->Pop());
		}
		std::cout << (locking ? "Locking " : "LockFree ") << (stest ? "STest " : "TTest ") <<
		    iterations << " inner iterations with " <<
			numThreads << " threads: " << timer.ElapsedMilliseconds() << std::endl;

		std::ofstream outfile;
		outfile.open(OUTPUT_FILE, std::ios_base::app);
		outfile << (locking ? "L," : "F,") << (stest ? "S," : "T,") << iterations << "," << numThreads << "," << timer.ElapsedMilliseconds() << std::endl;
		outfile.close();

		iterations = iterations << 1;
	}
	while(iterations < MAX_INNER_ITERATIONS);
}

void LockingStackTest()
{
	for(int i = 0; i < 4; ++i)
	{
		//Tests the locking implementation of the stack
		stack = new Locking::Stack();
		std::thread threads1[1];
		doTest(1, threads1, true, true);
		doTest(1, threads1, false, true);
		std::thread threads4[4];
		doTest(4, threads4, true, true);
		doTest(4, threads4, false, true);
		delete stack;
	}
}

void LockFreeStackTest()
{
	for(int i = 0; i < 4; ++i)
	{
		//Tests the lockfree implementation of the stack
		stack = new LockFree::Stack();
		std::thread threads1[1];
		doTest(1, threads1, true, false);
		doTest(1, threads1, false, false);
		std::thread threads4[4];
		doTest(4, threads4, true, false);
		doTest(4, threads4, false, false);
		delete stack;
	}
}

#endif