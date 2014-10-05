#include "rspf/WorkerPool.h"

namespace rspf {

	WorkerPool::WorkerPool( unsigned int numThreads ) :
		hasJobs( jobQueueMutex ) {
		Initialize( numThreads );
	}

	WorkerPool::WorkerPool( const PropertyTree& ptree ) :
		hasJobs( jobQueueMutex ) {
		Initialize( ptree.get<unsigned int>("num_threads") );
	}
		

	WorkerPool::~WorkerPool() {
		workerThreads.interrupt_all();
	}
		
	void WorkerPool::EnqueueJob( const Job& job ) {
		Lock lock( jobQueueMutex );

		jobQueue.push_back( job );
		hasJobs.NotifyAll();
	}

	void WorkerPool::Initialize( unsigned int numThreads ) {
		for( unsigned int i = 0; i < numThreads; i++ ) {
			
			// NOTE Mixing std::bind and boost::threads
			workerThreads.create_thread( std::bind( &WorkerPool::WorkerLoop, this ) );
		}
	}

	unsigned int WorkerPool::size() const {
		return workerThreads.size();
	}
	
	void WorkerPool::WorkerLoop() {

		while( true ) {
			Job job = WaitOnJob();
			job();
		}
	}

	WorkerPool::Job WorkerPool::WaitOnJob() {
		Lock lock( jobQueueMutex );
		while( jobQueue.empty() ) {
			hasJobs.Wait( lock );
		}
		Job job = jobQueue.front();
		jobQueue.pop_front();
		return job;
	}
	
}