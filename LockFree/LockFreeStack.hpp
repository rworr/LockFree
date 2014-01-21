//Implementation of a lock-free stack using CAS
//	Uses the upper 16 bits as a counter

#ifndef _LOCKFREESTACK_HPP
#define _LOCKFREESTACK_HPP

#include "Stack.hpp"
#include "CAS.h"
#include <assert.h>

namespace LockFree {
	class Stack : public Container::Stack {
	public:
		//Constructor initializing the head to 0
		Stack() : Container::Stack() {
			//nothing to do...
		}

		//Push to the stack
		inline bool push(SinglyLinkedObject* element) {
			if(!element||!validPointer(element)) {
				//ensure the element is valid and may be placed on the stack
				return false;
			}
			//create the new head for the stack
			SinglyLinkedObject* curHead = m_Head;
			SinglyLinkedObject* newHead = element;
			do {
				element->m_pNext = curHead;
				newHead = counterInc(curHead, element);
			}
			//atomic CAS to push to the head
			while(!CASPtr((volatile void**)&m_Head, (void**)&curHead, newHead));
			return true;
		}

		//Pop from the stack
		inline SinglyLinkedObject* pop() {
			SinglyLinkedObject* curHead = m_Head;
			SinglyLinkedObject* newHead;
			do {
				//return null if the stack is empty
				if(!pointer(curHead))
					return 0;
				newHead = pointer(curHead)->m_pNext;
				newHead = counterInc(curHead, newHead);
			}
			//CAS to update the head
			while(!CASPtr((volatile void**)&m_Head, (void**)&curHead, newHead));
			//read the pointer from the element
			curHead = pointer(curHead);
			curHead->m_pNext = 0;
			return curHead;
		}

		//Is the stack empty?
		inline bool empty() {
			return !pointer(m_Head);
		}

		//Check if a pointer can be used in the stack
		// i.e. counter == 0
		static bool validPointer(SinglyLinkedObject* ptr) {
			return !counter(ptr);
		}

	private:
#ifdef _WIN64
		static const size_t COUNTER_MASK = 0xFFFF000000000000;
		static const size_t COUNTER_DELTA = 0x0001000000000000;
#elif defined(_WIN32)
		static const int COUNTER_MASK = 0xFF000000;
		static const int COUNTER_DELTA = 0x01000000;
#else
#error System Not Implemented
#endif

		//Get counter from an element in the stack
		static uintptr_t counter(SinglyLinkedObject* element) {
			return (uintptr_t)element & COUNTER_MASK;
		}

		//Get pointer from an element in the stack
		static SinglyLinkedObject* pointer(SinglyLinkedObject* element) {
			return (SinglyLinkedObject*)((uintptr_t)element & ~COUNTER_MASK);
		}

		//Increment counter for two pointers
		static SinglyLinkedObject* counterInc(SinglyLinkedObject* head, SinglyLinkedObject* element) {
			uintptr_t counter = ((uintptr_t)head & COUNTER_MASK) + COUNTER_DELTA;
			return (SinglyLinkedObject*)((uintptr_t)pointer(element) | counter);
		}
	};
}

#endif