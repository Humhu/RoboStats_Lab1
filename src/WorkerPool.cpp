#include "rspf/WorkerPool.h"

namespace rspf {

	WorkerPool::WorkerPool( unsigned int numThreads ) :
		jobCounter( 0 ) {
		Initialize( numThreads );
	}

	WorkerPool::WorkerPool( const PropertyTree& ptree ) :
		jobCounter( 0 ) {
		Initialize( ptree.get<unsigned int>("num_threads") );
	}
		

	void WorkerPool::EnqueueJob( const Job& job ) {
		Lock lock( jobQueueMutex );

		jobQueue.push_back( job );
		jobCounter.Increment();
	}

	void WorkerPool::Initialize( unsigned int numThreads ) {
		for( unsigned int i = 0; i < numThreads; i++ ) {
			
			// NOTE Mixing std::bind and boost::threads
			workerThreads.create_thread( std::bind( &WorkerPool::WorkerLoop, this ) );
		}
	}
	
	void WorkerPool::WorkerLoop() {

		while( true ) {
			jobCounter.Decrement();
			Job job = jobQueue.front();
			jobQueue.pop_front();
			job();
		}
	}
	
}