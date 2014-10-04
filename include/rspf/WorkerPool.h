#ifndef _WORKER_POOL_H_
#define _WORKER_POOL_H_

#include "rspf/SynchronizationPrimitives.h"
#include "rspf/Parameterized.h"

#include <boost/thread/thread.hpp>

namespace rspf {

	class WorkerPool {
	public:

		typedef std::function<void()> Job;
		
		WorkerPool( unsigned int numThreads );
		WorkerPool( const PropertyTree& ptree );

		void EnqueueJob( const Job& job );
		
	protected:

		Mutex jobQueueMutex;
		Semaphore jobCounter;
		std::deque<Job> jobQueue;
		
		boost::thread_group workerThreads;

		void WorkerLoop();
		void Initialize( unsigned int numThreads );
		
	};
	
}

#endif