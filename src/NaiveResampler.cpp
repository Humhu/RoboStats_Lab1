#include "rspf/NaiveResampler.h"

namespace rspf {

	NaiveResampler::NaiveResampler() {}

	std::vector<Particle> NaiveResampler::resampleParticles( const std::vector<Particle>& particles, unsigned int numSamples ) {

		unsigned int numParticles = particles.size();
		double weightSum = 0;
		for( unsigned int i = 0; i < numParticles; i++ ) {
			weightSum += particles[i].getW();
		}
		
		// resample
		// now draw one random # per particle, where the random # is between 0 and max_val of number line, ie, sum of weights
		UniformDistribution uniformR( 0, weightSum );
		uniformR.SetSeed( clock() );

		std::vector<double> randomNums(numSamples);

		for( int i = 0; i < numSamples; i++ ){
			randomNums[i] = uniformR.Sample();
		} // end for-random#s

		// order the list of random numbers
		std::sort (randomNums.begin(), randomNums.end());

		int n = 0; // this is the index of what old-particle(weight) we are on
		int i = 0; // this is the index of what random# (newParticle) we are on
		weightSum = 0; // reset weightSum
		std::vector<Particle> newParticleSet(numSamples);

		while( i < numSamples ) {		// for random#[i], see if it's greater than cumweight[n]

			// if current--SORTED--random# is under the weightSum, then make a newParticle corresponding to the particle-of-this-bin
			if(	randomNums[i] < weightSum ){
				newParticleSet[i] = particles[n-1];
				// 				std::cout << "Assigned a new particle with pose " << particleSet[n-1].getPose() << std::endl;
				i += 1;
				// 				std::cout <<"I increased i."<< std::endl;
			}

			if( randomNums[i] >= weightSum ){
				weightSum += particles[n].getW();
				n += 1;
				// 				std::cout <<"I increased n. randomNums[i] = " <<randomNums[i] << ", weight sum: " << weightSum << std::endl;
			}
		} // end while

		return newParticleSet;
	}


}