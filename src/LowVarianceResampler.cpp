#include "rspf/LowVarianceResampler.h"

namespace rspf {

	LowVarianceResampler::LowVarianceResampler( const PropertyTree& ptree ) :
		uniformDist( 0, 1 ),
		xNoise( ptree.get_child("x_noise") ),
		yNoise( ptree.get_child("y_noise") ),
		thNoise( ptree.get_child("th_noise") ) {

		SeedDistributions();
	}

	void LowVarianceResampler::SeedDistributions() {
		uniformDist.SetSeed( clock() );
		xNoise.SetSeed( clock() + 2000 );
		yNoise.SetSeed( clock() + 2142 );
		thNoise.SetSeed( clock() + 3420 ); // HACK
	}

	std::vector<Particle> LowVarianceResampler::resampleParticles( const std::vector<Particle>& particles, unsigned numSamples ) {

		unsigned int numParticles = particles.size();

		double weightSum = 0;
		for( unsigned int i = 0; i < numParticles; i++ ) {
			weightSum += particles[i].getW();
			if( particles[i].getW() > 2 ) {
				std::cout << "Added weight: " << particles[i].getW() << " for particle at "
					<< particles[i].getPose() << std::endl;
			}
				
		}

		std::cout << "Weight sum: " << weightSum << std::endl;
		
		double combtooth = weightSum / ( numSamples );
		uniformDist.SetBounds( 0, combtooth );
		double randomNum = uniformDist.Sample();
		//randomNums[1:end] = 1:numParticles*weightSum/numParticles + randomNums[0]
		
		int n = 0; // this is the index of what old-particle(weight) we are on
		int i = 0; // this is the index of what random# (newParticle) we are on
		weightSum = 0; // reset weightSum
		std::vector<Particle> newParticleSet(numSamples);

		unsigned int numSampledOfCurrent = 0;
		
		while( i < numSamples ){		// for random#[i], see if it's greater than cumweight[n]

			// if current--SORTED--random# is under the weightSum, then make a newParticle corresponding to the particle-of-this-bin
			if(	randomNum < weightSum ){

				PoseSE2 pose = particles[n-1].getPose();
				if( numSampledOfCurrent > 0 ) {
					pose = pose * SamplePose();
				}
				newParticleSet[i].setPose( pose );
				
				i += 1;
				randomNum += combtooth;
				numSampledOfCurrent++;
			}
			
			if( randomNum >= weightSum ){
				weightSum += particles[n].getW();
				n += 1;
				numSampledOfCurrent = 0;
			}
		} // end while

		return newParticleSet;
	}

	PoseSE2 LowVarianceResampler::SamplePose() {
		return PoseSE2( xNoise.Sample(), yNoise.Sample(), thNoise.Sample() );
	}
	
}