#include "rspf/SynchronizationPrimitives.h"

namespace rspf {

	ConditionVariable::ConditionVariable( Mutex& mu ) :
		mutex( mu ) {}

	ConditionVariable::~ConditionVariable() {
		cond_variable.notify_all();
	}
		
	void ConditionVariable::NotifyAll() {
		cond_variable.notify_all();
	}

	void ConditionVariable::Wait( Lock& lock ) const {
		if( !lock.owns_lock() && lock.mutex() != &mutex ) {
			throw std::logic_error( "Condition variable failed lock check." );
		}

		cond_variable.wait( lock );
	}

	Semaphore::Semaphore( int startCounter ) :
		counter( startCounter ),
		hasCounters( mutex ) {}

	void Semaphore::Increment() {
		Lock lock( mutex );
		
		counter++;
		hasCounters.NotifyAll();
	}

	void Semaphore::Decrement() {
		Lock lock( mutex );
		
		while( counter <= 0 ) {
			hasCounters.Wait( lock );
		}

		counter--;
	}

	
	
}