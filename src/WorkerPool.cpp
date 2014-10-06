#include "rspf/WorkerPool.h"

namespace rspf {

	WorkerPool::WorkerPool( unsigned int numThreads ) {
		Initialize( numThreads );
	}

	WorkerPool::WorkerPool( const PropertyTree& ptree ) {
		Initialize( ptree.get<unsigned int>("num_threads") );
	}
		

	WorkerPool::~WorkerPool() {

		Lock lock( jobQueueMutex );
		threadsRunning = false;
		lock.unlock();
		
		hasJobs.notify_all();
//  		workerThreads.interrupt_all();
		workerThreads.join_all();
	}
		
	void WorkerPool::EnqueueJob( const Job& job ) {
		Lock lock( jobQueueMutex );

		jobQueue.push_back( job );
		hasJobs.notify_all();
	}

	void WorkerPool::Initialize( unsigned int numThreads ) {
		threadsRunning = true;
		for( unsigned int i = 0; i < numThreads; i++ ) {
			
			// NOTE Mixing std::bind and boost::threads
			workerThreads.create_thread( std::bind( &WorkerPool::WorkerLoop, this ) );
		}
	}

	unsigned int WorkerPool::size() const {
		return workerThreads.size();
	}
	
	void WorkerPool::WorkerLoop() {

		try {
			while( true ) {
				Lock lock( jobQueueMutex );

				while( threadsRunning && jobQueue.empty() ) {
					hasJobs.wait( lock );
				}

				if( !threadsRunning ) {
// 					std::cout << "Thread no longer running. Bailing out!" << std::endl;
					return;
				}
				
				Job job = jobQueue.front();
				jobQueue.pop_front();
				job();
			}
		} catch( std::exception e ) {}
	}	
}