#ifndef _RESAMPLER_H_
#define _RESAMPLER_H_

#include "rspf/Particle.h"
#include <memory>

namespace rspf {

	class Resampler {
	public:

		typedef std::shared_ptr<Resampler> Ptr;
		
		Resampler();

		virtual std::vector<Particle> resampleParticles( const std::vector<Particle>& particles, unsigned int numSamples ) = 0;
		
	};
	
}

#endif