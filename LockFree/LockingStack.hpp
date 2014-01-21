//Implementation of a stack using a mutex for synchronization

#ifndef _LOCKINGSTACK_HPP
#define _LOCKINGSTACK_HPP

#include "Stack.hpp"
#include <mutex>

namespace Locking {
	typedef std::mutex Mutex;

	class Stack : public Container::Stack {
	public:
		//Constructor initializing the head to 0
		Stack() : Container::Stack() {
			//nothing to do...
		}

		//Constructor initializing the head to the specified object
		Stack(SinglyLinkedObject* head) : Container::Stack(head) {
			//nothing to do...
		}

		//Push to the stack
		inline bool push(SinglyLinkedObject* element) {
			//lock the stack
			m_StackMutex.lock();

			//push element onto the stack
			element->m_pNext = m_Head;
			m_Head = element;

			//unlock the stack
			m_StackMutex.unlock();
			return true;
		}

		//Pop from the stack
		inline SinglyLinkedObject* pop() {
			//lock the stack
			m_StackMutex.lock();

			//pop an element from the stack
			SinglyLinkedObject* element = m_Head;
			if(element)
				m_Head = element->m_pNext;

			//unlock the stack
			m_StackMutex.unlock();

			return element;
		}

	private:
		//Mutex for synchronization
		Mutex m_StackMutex;
	};
}

#endif