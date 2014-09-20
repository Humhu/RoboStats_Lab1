#include "rspf/ParticleFilter.h"
#include "rspf/StaticTransitionModel.h"

namespace rspf {
	
	ParticleFilter::ParticleFilter() {
		int defaultN = 100;
		particleSet = std::vector<Particle>(defaultN);
		for( int i = 0; i<N; i++ ) {
				particleSet[i] = Particle();
		}
		
		StaticTransitionModel* model = new StaticTransitionModel();
		transitionModel = model;
		
	}
}