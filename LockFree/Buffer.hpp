//Interface for buffers, both locking and lock-free

#ifndef _BUFFER_INTERFACE
#define _BUFFER_INTERFACE

#include <assert.h>

namespace Container
{
	class Buffer
	{
	public:
		//Constructor initializing the buffer
		Buffer() : currentSaveIndex(0), currentReadIndex(0) {
			//nothing to do...
		}

		virtual inline bool write(void* ptr) {
			assert(false);
		}
		virtual inline void* read() {
			assert(false);
		}
		virtual inline bool isEmpty() {
			assert(false);
		}

	protected:
		int currentSaveIndex;
		int currentReadIndex;
		void* slots[256];
	};
}

#endif