#ifndef PARTICLE_FILTER_
#define PARTICLE_FILTER_

#include "rspf/Particle.h" //quotes = local folder
#include "rspf/TransitionModel.h"
#include "rspf/RobotLogReader.h"
#include "rspf/Map.h"
#include "rspf/Parameterized.h"

#include <vector> //carats = system folder


namespace rspf {

    class ParticleFilter {
    public:

		ParticleFilter( const Map& _map, unsigned int numParticles=10000 ); //default arguments startign from the back. stupid.
		ParticleFilter( const Map& _map, const PropertyTree& ptree );
		
		void makeParticleSet();
		void weightParticleSet( SensorData x );
		std::vector<Particle> GetParticles();
		void make_a_transition(); // all the arguments this "function" would need are already in this workspace. 
					 			   // this is a function that is updating things that are already in this workspace
		void handleData( SensorData data );
		
    private: 
		std::vector<Particle> particleSet;
		TransitionModel* transitionModel;
		const Map& map;

		void Initialize( unsigned int numParticles );
		
    }; // class
    
} 

#endif 