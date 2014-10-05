#ifndef _NAIVE_RESAMPLER_H_
#define _NAIVE_RESAMPLER_H_

#include "rspf/Resampler.h"
#include "rspf/RandomDistributions.h"

namespace rspf {

	class NaiveResampler {
	public:

		NaiveResampler();

		std::vector<Particle> resampleParticles( const std::vector<Particle>& particles, unsigned int numSamples );
	};
	
}

#endif