#ifndef PARTICLE_FILTER_
#define PARTICLE_FILTER_

#include "rspf/Particle.h" //quotes = local folder
#include "rspf/TransitionModel.h"
#include "rspf/Observation.h"
#include <vector> //carats = system folder

namespace rspf {

    class ParticleFilter {
    public:
		ParticleFilter(); 
		void makeParticleSet();
		void weightParticleSet( Observation x );
		std::vector<Particle> returnParticleSet();
		void make_a_transition(); // all the arguments this "function" would need are already in this workspace. this is a function that is updating things that are already in this workspace
		
    private: 
		std::vector<Particle> particleSet;
		TransitionModel* transitionModel;
		
    }; // class
    
}

#endif 