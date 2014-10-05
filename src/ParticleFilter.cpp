#include "rspf/ParticleFilter.h"

#include "rspf/StaticTransitionModel.h"
#include "rspf/DefaultTransitionModel.h"

#include "rspf/RandomDistributions.h"
#include <time.h>

#include "rspf/LaserSensorModel.h"
#include "rspf/WallSensorModel.h"

#include "rspf/LowVarianceResampler.h"

#include <boost/foreach.hpp>

namespace rspf {

	ParticleFilter::ParticleFilter( const Map& _map, const PropertyTree& ptree ) :
		map( _map ),
		workers( ptree.get_child("workers") ),
		numParticles( ptree.get<unsigned int>("num_particles") ),
		jobsPending( 0 )
	{
		
		Initialize( ptree.get<unsigned int>("init_particles") );

		unsigned int numWorkers = workers.size();
		for( unsigned int i = 0; i < numWorkers; i++ ) {
			TransitionModel::Ptr model = std::make_shared<DefaultTransitionModel>( map, ptree.get_child("transition_model") );
			transitionModel.push_back( model );
			usleep(1000); // NOTE To avoid correlating the RNGs too much
		}

		resampler = std::make_shared<LowVarianceResampler>( ptree.get_child("resampler") );
			

		BOOST_FOREACH( const PropertyTree::value_type& item, ptree ) {

			std::string identifier = item.first;
			PropertyTree subtree = item.second;
			if( identifier == "sensor_model" ) {

				std::string type = subtree.get<std::string>("type");

				std::vector<SensorModel::Ptr> models;
				if( type == "laser" ) {
					for( unsigned int i = 0; i < numWorkers; i++ ) {
						SensorModel::Ptr model = std::make_shared<LaserSensorModel>( map, ptree.get_child("sensor_model") );
						models.push_back( model );
					}
				}
				else if( type == "wall" ) {
					for( unsigned int i = 0; i < numWorkers; i++ ) {
						SensorModel::Ptr model = std::make_shared<WallSensorModel>( map );
						models.push_back( model );
					}
				}
				else {
					throw std::runtime_error( "Unknown sensor model of type: " + type );
				}
				sensorModels.push_back( models );
			}
		}
	}

	void ParticleFilter::Initialize( unsigned int n )
	{
		// UniformDistribution( double lower, double upper );
		UniformDistribution uniformX( 0, map.GetXSize() - 1E-3 );
		UniformDistribution uniformY( 0, map.GetYSize() - 1E-3 );
		UniformDistribution uniformTheta( 0, 2*M_PI );
		
		clock_t now = clock() >> 8;
		std::cout << "Seeding with " << now << std::endl;
		std::cout << "Map size: " << map.GetXSize() << std::endl;

		uniformX.SetSeed( now );
		uniformY.SetSeed( now+10000 );
		uniformTheta.SetSeed( now+2000 );
		
		particleSet = std::vector<Particle>(n);
		for( unsigned int i = 0; i < n; i++ )
		{
			while(true){
				
				// draw a pose  	// explicit PoseSE2( double x, double y, double theta );
				PoseSE2 myPose = PoseSE2( uniformX.Sample(), uniformY.Sample(), uniformTheta.Sample() );
				
				// check if pose is in map		//get value of map at particle x,y	
				//map is -1 for black regions, [0 1] for probability of fill elsewhere (0 = occupied, 1 = empty)
				double mapOccupancy = map.GetValue( myPose.getX(), myPose.getY() );

				// if x,y not object, we're ok, so break out of the while loop
				if( mapOccupancy > 0.9 ){

					// draw a particle
					// assign pose to Particle
						particleSet[i].setPose( myPose );
						particleSet[i].setW( 1.0 );
						
					break;
					
				}	//end if
			} // end while
		} // end for
	} //end initialize
	
	std::vector<Particle> ParticleFilter::GetParticles()
	{
		return particleSet;
	}

	void ParticleFilter::handleData( const SensorData& data )
	{
		// Have to chunk the indices up
		double numWorkers = workers.size();
		double chunkSize = particleSet.size() / numWorkers;

		for( unsigned int i = 0; i < numWorkers; i++ ) {
			unsigned int startIndex = std::floor( i*chunkSize );
			unsigned int endIndex = std::floor( (i+1)*chunkSize ) - 1;
			WorkerPool::Job job =
				boost::bind( &ParticleFilter::handleDataSubset, this, data, startIndex, endIndex, i );
			workers.EnqueueJob( job );
		}

		for( unsigned int i = 0; i < numWorkers; i++ ) {
			jobsPending.Decrement();
		}

		for( unsigned int i = 0; i < particleSet.size(); i++ ) {
			double w = particleSet[i].getW();
		}
		particleSet = resampler->resampleParticles( particleSet, numParticles );
	}

// 	std::vector< std::vector<double> > ParticleFilter::GetLastRaytraces() {
// 		return sensorModels[0][0]->lastTraces;
// 	}

	// NOTE Concurrent accesses to particleSet are probably OK
	void ParticleFilter::handleDataSubset( const SensorData& data, unsigned int startIndex,
										   unsigned int endIndex, unsigned int instanceNum ) {

		// First transition all the particles and reset their weights, apply sensor models
// 		sensorModels[0][instanceNum]->lastTraces.clear();

		for(int i = startIndex; i <= endIndex; i++)
		{
			transitionModel[instanceNum]->transitionParticle( particleSet[i], data );
			particleSet[i].setW( 1.0 );

			for( unsigned int j = 0; j < sensorModels.size(); j++ ) {
				sensorModels[j][instanceNum]->weightParticle( particleSet[i], data );
			}
		}
		jobsPending.Increment();
	}
	
}