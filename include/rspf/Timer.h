#ifndef _TIMER_H_
#define _TIMER_H_

#include <boost/date_time/posix_time/posix_time.hpp>

namespace rspf {

	typedef boost::posix_time::ptime Time;
	typedef boost::posix_time::time_duration TimeDuration;
	
	class Timer {
	public:
		
		Timer();

		/*! \brief Reset the timer start time to now. */
		void Reset();

		/*! \brief Get the system time. */
		Time GetTime();

		/*! \brief Get the duration since the start time. */
		TimeDuration GetDuration();

	private:

		Time startTime;
	};
	
}

#endif