//Implementation of a lock-free list, where single elements/lists can be pushed and the entire list removed

#ifndef _LOCKFREELIST_HPP
#define _LOCKFREELIST_HPP

#include "CAS.h"
#include "SinglyLinkedObject.hpp"

namespace LockFree {
	class List {
	public:
		//Default constructor
		List() : m_Head(0) {
			//nothing to do...
		}

		//Construct a list using a given list/element
		List(SinglyLinkedObject* head) : m_Head(head) {
			//nothing to do...
		}

		//Pop the list
		inline SinglyLinkedObject* popList() throw() {
			SinglyLinkedObject* curHead = m_Head;
			//CAS in an empty list with a null head
			while(!CASPtr((volatile void**)&m_Head, (void **)&curHead, 0));

			return curHead;
		}

		//Push a single element to the list
		inline void pushElement(SinglyLinkedObject* element) throw() {
			SinglyLinkedObject* curHead = m_Head;
			do {
				element->m_pNext = curHead;
			}
			//Push the element to the head
			while(!CASPtr((volatile void**)&m_Head, (void **)&curHead, element));
		}

		//Push a list to the list
		inline void pushList(SinglyLinkedObject* head, SinglyLinkedObject* tail) throw() {
			SinglyLinkedObject* curHead = m_Head;
			do {
				tail->m_pNext = curHead;
			}
			//Push the new head
			while(!CASPtr((volatile void**)&m_Head, (void **)&curHead, head));
		}

		//Check if empty
		inline bool empty() throw() {
			return !m_Head;
		}

	private:
		SinglyLinkedObject* m_Head;
	};
}

#endif