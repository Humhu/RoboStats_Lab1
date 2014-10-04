#ifndef PARTICLE_FILTER_
#define PARTICLE_FILTER_

#include "rspf/Particle.h" //quotes = local folder
#include "rspf/TransitionModel.h"
#include "rspf/RobotLogReader.h"
#include <vector> //carats = system folder
#include "rspf/Map.h"

namespace rspf {

    class ParticleFilter {
    public:
		ParticleFilter( Map _map, unsigned int numParticles=10000 ); //default arguments startign from the back. stupid.
		void makeParticleSet();
		void weightParticleSet( SensorData x );
		std::vector<Particle> GetParticles();
		void make_a_transition(); // all the arguments this "function" would need are already in this workspace. 
					 			   // this is a function that is updating things that are already in this workspace
		void handleData( SensorData data );
		
    private: 
		std::vector<Particle> particleSet;
		TransitionModel* transitionModel;
		Map map;
		
    }; // class
    
} 

#endif 