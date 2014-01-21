//Simple timer using time.h

#ifndef _SIMPLETIME_HPP
#define _SIMPLETIME_HPP

#include <time.h>
#include <stdint.h>

class SimpleTimer {
public:
	//Constructor initializing start and end times to 0, not started
	SimpleTimer() : m_Start(0), m_Stop(0), m_IsTiming(false), m_ElapsedMilliseconds(0) {
		//nothing to do
	}

	//Begin timing
	inline void start() throw() {
		m_IsTiming = true;
		m_Start = clock();
	}

	//Stop timing
	inline void stop() throw() {
		m_Stop = clock();
		m_IsTiming = false;
		m_ElapsedMilliseconds = (m_Stop - m_Start)/CLOCKS_PER_MILLI;
	}
	
	inline bool timing() const throw() {
		return m_IsTiming;
	}

	//Return the number of elapsed seconds
	inline size_t elapsedSeconds() throw() {
		if(m_IsTiming) {
			m_ElapsedMilliseconds = (clock() - m_Start)/CLOCKS_PER_MILLI;
		}
		return m_ElapsedMilliseconds / 1000;
	}

	//return the number of elapsed milliseconds
	inline size_t elapsedMilliseconds() throw() {
		if(m_IsTiming) {
			m_ElapsedMilliseconds = (clock() - m_Start)/CLOCKS_PER_MILLI;
		}
		return m_ElapsedMilliseconds;
	}

	//Reset the timer
	inline void reset() throw() {
		m_Start, m_Stop, m_ElapsedMilliseconds = 0;
		m_IsTiming = false;
	}

private:
	//constant for converting clock ticks to milliseconds
	static const size_t CLOCKS_PER_MILLI = CLOCKS_PER_SEC / 1000;

	//timer start, stop
	clock_t m_Start, m_Stop;

	//elapsed milliseconds
	size_t m_ElapsedMilliseconds;

	//flag for if the timer is currently running
	bool m_IsTiming;
};

#endif