//Interface for stacks, both locking and lock-free

#ifndef _STACK_INTERFACE
#define _STACK_INTERFACE

#include "SinglyLinkedObject.hpp"
#include <assert.h>

namespace Container {
	class Stack {
	public:
		//Constructor initializing the head to 0
		Stack() : m_Head(0) {
			//nothing to do...
		}

		//Constructor initializing the head to the specified object
		Stack(SinglyLinkedObject* head) : m_Head(head) {
			//nothing to do...
		}

		//virtual push
	    //returns true if the element was successfully pushed to the stack
		virtual inline bool push(SinglyLinkedObject* element)  {
			//this code MUST never be called
			assert(false);
			return false;
		}

		//virtual pop
		virtual inline SinglyLinkedObject* pop() {
			//this code MUST never be called
			assert(false);
			return 0;
		}

		//isEmpty
		virtual inline bool empty() {
			return !m_Head;
		}

	protected:
		//Head of the stack
		SinglyLinkedObject* m_Head;
	};
}

#endif