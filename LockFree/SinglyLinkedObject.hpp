//Objects with a single next pointer, used to create singly-linked objects (stacks)

#ifndef _SINGLYLINKEDOBJECT_HPP
#define _SINGLYLINKEDOBJECT_HPP

class SinglyLinkedObject {
public:
	//Constructor initializing the next pointer to NULL
	SinglyLinkedObject() : m_pNext(0) {
		//nothing to do...
	}

	//Constructor initializing the next pointer to the given object
	SinglyLinkedObject(SinglyLinkedObject* next) : m_pNext(next) {
		//nothing to do...
	}

	//Pointer to the next object
	SinglyLinkedObject* m_pNext;
};

#endif