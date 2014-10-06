#ifndef PARTICLE_FILTER_
#define PARTICLE_FILTER_

#include "rspf/Particle.h" //quotes = local folder
#include "rspf/RobotLogReader.h"
#include "rspf/Map.h"
#include "rspf/Parameterized.h"

#include "rspf/TransitionModel.h"
#include "rspf/SensorModel.h"
#include "rspf/Resampler.h"

#include "rspf/WorkerPool.h"
#include "rspf/SynchronizationPrimitives.h"

#include <vector> //carats = system folder


namespace rspf {

    class ParticleFilter {
    public:

		ParticleFilter( const Map& _map, const PropertyTree& ptree );
		~ParticleFilter();
		
		void makeParticleSet();
		void weightParticleSet( SensorData x );
		std::vector<Particle> GetParticles();
		void make_a_transition(); // all the arguments this "function" would need are already in this workspace. 
					 			   // this is a function that is updating things that are already in this workspace
		void handleData( const SensorData& data );
// 
// 		std::vector< std::vector<double> > GetLastRaytraces();
		
    private:

		unsigned int numParticles;
		std::vector<Particle> particleSet;
		
		std::vector< TransitionModel::Ptr > transitionModel;
		std::vector< std::vector<SensorModel::Ptr> > sensorModels;
		Resampler::Ptr resampler;

		WorkerPool workers;
		Semaphore jobsPending;
		
		const Map& map;

		void Initialize( unsigned int numParticles );
		void handleDataSubset( const SensorData& data, unsigned int startIndex,
							   unsigned int endIndex, unsigned int instanceNum );
		
    }; // class
  
//  		std::vector<double> RayTrace( Particle particle, SensorData data, const Map& _map );

  
} 

#endif 