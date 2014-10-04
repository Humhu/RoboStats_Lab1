#include "rspf/Timer.h"

namespace rspf {

	Timer::Timer() {
		startTime = GetTime();
	}

	Time Timer::GetTime() {
		return boost::posix_time::microsec_clock::universal_time();
	}

	TimeDuration Timer::GetDuration() {
		Time now = GetTime();
		return now - startTime;
	}

	void Timer::Reset() {
		startTime = GetTime();
	}
	
}