#ifndef _SYNCHRONIZATION_PRIMITIVES_H_
#define _SYNCHRONIZATION_PRIMITIVES_H_

#include <boost/thread.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>

namespace rspf {

	/*! \brief Convenient typedefs for using locks. */
	typedef boost::mutex Mutex;
	typedef boost::unique_lock<Mutex> Lock;

	/*! \brief Basic condition variable wrapper. */
	class ConditionVariable {
	public:

		ConditionVariable( Mutex& m );
		~ConditionVariable();

		void NotifyAll();
		void Wait( Lock& lock ) const;

	protected:

		mutable boost::condition_variable cond_variable;
		Mutex& mutex;
	};

	/*! \brief Basic semaphore wrapper. */
	class Semaphore {
	public:
		
		Semaphore( int startCounter );

		void Increment();
		void Decrement();

	protected:

		mutable Mutex mutex;
		int counter;
		ConditionVariable hasCounters;
	};
	
}

#endif