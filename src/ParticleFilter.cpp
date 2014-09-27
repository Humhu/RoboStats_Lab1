#include "rspf/ParticleFilter.h"
#include "rspf/StaticTransitionModel.h"

namespace rspf {
	
	ParticleFilter::ParticleFilter( unsigned int numParticles ) 
	{
		particleSet = std::vector<Particle>(numParticles);
		for( int i = 0; i < numParticles; i++ ) 
		{
				particleSet[i] = Particle();
		}
		
		StaticTransitionModel* model = new StaticTransitionModel();
		transitionModel = model;		
	}
	
	std::vector<Particle> ParticleFilter::GetParticles()
	{
		return particleSet;
	}
}

//GetParticles