//Implementation of a lock-free stack using CAS
//	Uses the upper 16 bits as a counter

#ifndef _LOCKFREEBUFFER_HPP
#define _LOCKFREEBUFFER_HPP

#include "Buffer.hpp"
#include "CAS.h"
#include <assert.h>

namespace LockFree
{
	class Buffer : public Container::Buffer
	{
	public:
		Buffer() : Container::Buffer(), currentlyUsedIndexes(0) {
			//nothing
		}

		inline bool write(void* ptr) {
			assert(false);
		}

		inline void* read() {
			assert(false);
		}

		inline bool isEmpty() {
			assert(false);
		}
	private:
		uint32_t currentlyUsedIndexes;
	};
}

#endif