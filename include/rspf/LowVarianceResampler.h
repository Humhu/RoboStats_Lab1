#ifndef _LOW_VARIANCE_RESAMPLER_H_
#define _LOW_VARIANCE_RESAMPLER_H_

#include "rspf/Resampler.h"
#include "rspf/RandomDistributions.h"
#include "rspf/Parameterized.h"

namespace rspf {

	class LowVarianceResampler : public Resampler {
	public:

		typedef std::shared_ptr<LowVarianceResampler> Ptr;
		LowVarianceResampler( const PropertyTree& ptree );

		virtual std::vector<Particle> resampleParticles( const std::vector<Particle>& particles, unsigned int numSamples );

	private:

		UniformDistribution uniformDist;
		NormalDistribution xNoise;
		NormalDistribution yNoise;
		NormalDistribution thNoise;

		void SeedDistributions();
		PoseSE2 SamplePose();
	};
	
}

#endif