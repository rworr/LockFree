//Program entry point

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
#define MULTITHREADS 4 

static Container::Stack* stack = 0;
static int iterations = 1;

//helper function to push to the stack
//handling allocations and deallocations
void stackPush() {
	SinglyLinkedObject* obj = 0;
	//push the object to the stack
	//if a push fails, retry with a different pointer
	do { 
		if(obj) free(obj);
		obj = (SinglyLinkedObject*)malloc(sizeof(SinglyLinkedObject*));
	} while(!stack->push(obj));
}

//helper function to pop from the stack
//waiting for a successful pop and deallocating
void stackPop() {
	SinglyLinkedObject* obj = 0;
	//loop until an object is successfully pulled from the stack
	do {
		obj = stack->pop();
	} while(!obj);
	free(obj);
}

//producer thread for pushing onto the stack
void produce() {
	for(int i = 0; i < OUTER_ITERATIONS; ++i) {
		for(int j = 0; j < iterations; ++j) {
			stackPush();
		}
	}
}

//consumer thread to pop from the stack
void consume() {
	for(int i = 0; i < OUTER_ITERATIONS; ++i) {
		for(int j = 0; j < iterations; ++j) {
			stackPop();
		}
	}
}

//Push multiple pointers to the static stack,
//then pop multiple and free
void multipleTest() {
	for(int i = 0; i < OUTER_ITERATIONS; ++i) {
		for(int j = 0; j < iterations; ++j) {
			stackPush();
		}
		for(int j = 0; j < iterations; ++j) {
			stackPop();
		}
	}
}

//Push and then pop single pointers on the static stack
void singleTest() {
	for(int i = 0; i < OUTER_ITERATIONS; ++i) {
		for(int j = 0; j < iterations; ++j) {
			stackPush();
			stackPop();
		}
	}
}

void doTest(const int numThreads, std::thread* threads, const bool stest, const bool locking) {
	iterations = 1;
	do {
		SimpleTimer timer;
		timer.start();
		//initialize testing threads, then wait for all threads to finish
		if(stest) {
			for(int i = 0; i < numThreads; ++i) {
				threads[i] = std::thread(singleTest);
			}
		}
		else {
			for(int i = 0; i < numThreads; ++i) {
				threads[i] = std::thread(multipleTest);
			}
		}
		for(int i = 0; i < numThreads; ++i) {
			threads[i].join();
		}
		timer.stop();
		
		assert(stack->empty());

		std::cout << (locking ? "Locking " : "LockFree ") << (stest ? "S " : "M ") <<
		    iterations << " iterations " <<
			numThreads << " threads: " << timer.elapsedMilliseconds() << std::endl;

		std::ofstream outfile;
		outfile.open(OUTPUT_FILE, std::ios_base::app);
		outfile << (locking ? "L," : "F,") << (stest ? "S," : "M,") << iterations << "," << numThreads << "," << timer.elapsedMilliseconds() << std::endl;
		outfile.close();

		iterations = iterations << 1;
	}
	while(iterations < MAX_INNER_ITERATIONS);
}

void pcTest(bool locking) {
	int iterations = 1;
	do {
		SimpleTimer timer;
		std::thread threads[4];
		timer.start();
		for(int i = 0; i < 2; ++i) {
			threads[i] = std::thread(produce);
		}
		for(int i = 2; i < 4; ++i) {
			threads[i] = std::thread(consume);
		}
		for(int i = 0; i < 4; ++i) {
			threads[i].join();
		}
		timer.stop();
		assert(stack->empty());
		std::cout << (locking ? "Locking " : "LockFree ") << "PC " << iterations << " iterations: " << timer.elapsedMilliseconds() << std::endl;

		std::ofstream outfile;
		outfile.open(OUTPUT_FILE, std::ios_base::app);
		outfile << (locking ? "L," : "F,") << "PC" << iterations << "," << timer.elapsedMilliseconds() << std::endl;
		outfile.close();
			
		iterations = iterations << 1;
	} while(iterations < MAX_INNER_ITERATIONS);
}

void lockingStackTest() {
	for(int i = 0; i < 1; ++i) {
		//Tests the locking implementation of the stack
		stack = new Locking::Stack();
		std::thread threads1[1];
		doTest(1, threads1, true, true);
		doTest(1, threads1, false, true);
		std::thread threads4[4];
		doTest(4, threads4, true, true);
		doTest(4, threads4, false, true);
		pcTest(true);
		delete stack;
	}
}

void lockFreeStackTest() {
	for(int i = 0; i < 1; ++i) {
		//Tests the lockfree implementation of the stack
		stack = new LockFree::Stack();
		std::thread threads1[1];
		doTest(1, threads1, true, false);
		doTest(1, threads1, false, false);
		std::thread threads4[4];
		doTest(4, threads4, true, false);
		doTest(4, threads4, false, false);
		pcTest(false);
		delete stack;
	}
}

//Simplified tests for hotspot analysis and timing using VTune
void hotspotTesting() {	
	iterations = MAX_INNER_ITERATIONS;
	for(int i = 0; i < 4; ++i) {
		//single alloc/dealloc test
		{
			std::thread threads[4];
			for(int i = 0; i < 4; ++i) {
				threads[i] = std::thread(singleTest);
			}
			for(int i = 0; i < 4; ++i) {
				threads[i].join();
			}
		}

		//multiple alloc/dealloc test
		{
			std::thread threads[4];
			for(int i = 0; i < 4; ++i) {
				threads[i] = std::thread(multipleTest);
			}
			for(int i = 0; i < 4; ++i) {
				threads[i].join();
			}
		}

		//2-producer, 2-consumer test
		{
			std::thread threads[4];
			for(int i = 0; i < 2; ++i) {
				threads[i] = std::thread(produce);
			}
			for(int i = 2; i < 4; ++i) {
				threads[i] = std::thread(consume);
			}
			for(int i = 0; i < 4; ++i) {
				threads[i].join();
			}
		}
	}
}

int main() {
	//Test the stack implementations
	//stack = new Locking::Stack();
	//hotspotTesting();
	//delete stack;

	//stack = new LockFree::Stack();
	//hotspotTesting();
	//delete stack;

	lockingStackTest();
	lockFreeStackTest();

	std::cout << "Done!" << std::endl;
}