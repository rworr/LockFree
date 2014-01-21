//Implementation of a Compare and Swap for 32-bit and 64-bit
#include <stdint.h>

#ifndef _HAS_CAS
#define _HAS_CAS

#ifdef _WIN32
//Windows version - use windows API
#include <Windows.h>
	//Compare-And-Swap for 32-bit values
	inline bool CAS32(volatile uint32_t* cell, uint32_t* expected, uint32_t exchange) {
		uint32_t expectedValue = *expected;
		uint32_t initialVal = InterlockedCompareExchange((LONG*)cell, (LONG)exchange, (LONG)expectedValue);
		if(initialVal == expectedValue) {
			//successful exchange
			return true;
		}
		//unsuccessful, change the expected old value to the actual destination value
		*expected = *cell;
		return false;
	}

	//Compare-And-Swap for 64-bit values
	inline bool CAS64(volatile uint64_t* cell, uint64_t* expected, uint64_t exchange) {
		uint64_t expectedValue = *expected;
		uint64_t initialVal = InterlockedCompareExchange64((LONGLONG*)cell, (LONGLONG)exchange, (LONGLONG)expectedValue);
		if(initialVal == expectedValue) {
			//successful exchange
			return true;
		}
		//unsuccessful, change the expected old value to the actual destination value
		*expected = *cell;
		return false;
	}

	//Compare-And-Swap for pointers
	inline bool CASPtr(volatile void** cell, void** expected, void* exchange) {
		void* expectedPtr = *expected;
		void* initialPtr = InterlockedCompareExchangePointer((PVOID*)cell, (PVOID)exchange, (PVOID)expectedPtr);
		if(initialPtr == expectedPtr) {
			//successful exchange
			return true;
		}
		//unsuccessful, change the expected old value to the actual destination value
		//temporary conversion to uintptr_t
		uintptr_t tmp = (uintptr_t)*cell;
		*expected = (void*)tmp;
		return false;
	}
#else
#error System Not Implemented
#endif

#endif