#include "rspf/ParticleFilter.h"
#include "rspf/StaticTransitionModel.h"
#include "rspf/RandomDistributions.h"
#include <time.h>       /* clock_t, clock, CLOCKS_PER_SEC */

namespace rspf {
	
	ParticleFilter::ParticleFilter( const Map& _map, unsigned int numParticles ) : map(_map) //default arguments startign from the back. stupid.
	{
		Initialize( numParticles );
	}

	ParticleFilter::ParticleFilter( const Map& _map, const PropertyTree& ptree ) : map( _map )
	{
		Initialize( ptree.get<unsigned int>("num_particles") );
	}

	void ParticleFilter::Initialize( unsigned int numParticles ) {
		// UniformDistribution( double lower, double upper );
		UniformDistribution uniformX( 0, map.GetXSize() );
		UniformDistribution uniformY( 0, map.GetYSize() );
		UniformDistribution uniformTheta( 0, 2*M_PI );
		
		clock_t now = clock();
// 		std::cout << "Now: " << now << std::endl;
		uniformX.SetSeed( now );
		uniformY.SetSeed( now+10000 );
		uniformTheta.SetSeed( now+2000 );
		
		particleSet = std::vector<Particle>(numParticles);
		for( int i = 0; i < numParticles; i++ ) 
		{
			while(true){
				
			// draw a pose
				// explicit PoseSE2( double x, double y, double theta );
				PoseSE2 myPose = PoseSE2( uniformX.Sample(), uniformY.Sample(), uniformTheta.Sample() );
				
			// check if pose is in map				
				//get value of map at particle x,y				       Map::CellType GetValue( double x, double y ) const;
				//map is -1 for black regions, [0 1] for probability of fill elsewhere (0 = occupied, 1 = empty)
				double mapOccupancy = map.GetValue( myPose.getX(), myPose.getY() );

			// if x,y not object, we're ok, so break out of the while loop
				if( mapOccupancy != -1 ){
					// draw a particle
						particleSet[i] = Particle();
// 						std::cout <<myPose<< std::endl;
						
					// assign pose to Particle
						particleSet[i].setPose( myPose );  // 
						
					break;
					
				}	//end if
				
			} // end while

		} // end for
		
		StaticTransitionModel* model = new StaticTransitionModel();
		transitionModel = model;		
	}
	
	std::vector<Particle> ParticleFilter::GetParticles()
	{
		return particleSet;
	}
	

	void ParticleFilter::handleData ( SensorData data )
	{
		int numParticles = particleSet.size();
		
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		// apply position update to particles -- always happens
				
		PoseSE2 myDisplacement = data.displacement;
		// for all particles, displace them (in their own frames)
		
		for ( int i = 0; i < numParticles; i++ ) {
			particleSet[i].setPose( particleSet[i].getPose()*myDisplacement );
		} // end for

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		// "measurement data"
		double weightSum = 0;
		for ( int i = 0; i < numParticles; i++ ) {
			// for every particle, make a "measurement"--i.e., check the map to see the probability of being in a wall 
			PoseSE2 myPose = particleSet[i].getPose();
			double mapOccupancy = map.GetValue( myPose.getX(), myPose.getY() );	
						
			if( mapOccupancy == -1 ){
				particleSet[i].setW( 0 ); 
			}
			else{
				particleSet[i].setW( mapOccupancy ); 
			} //map is -1 for black regions, [0 1] for probability of fill elsewhere (0 = occupied, 1 = empty)		
			
// 			std::cout << "Particle [" << i << "] has weight " << particleSet[i].getW() << std::endl;
			weightSum += particleSet[i].getW();
			
		} //end for finding-map-weights-of-particles
		std::cout << "Weight sum after measurement update: " << weightSum << std::endl;
// 		std::cout << "Num particles: " << numParticles << std::endl;
		
		// now draw one random # per particle, where the random # is between 0 and max_val of number line, ie, sum of weights
		UniformDistribution uniformR( 0, weightSum );		
		uniformR.SetSeed( clock() );
		
		std::vector<double> randomNums(numParticles);	
		
		for( int i = 0; i < numParticles; i++ ){
			randomNums[i] = uniformR.Sample();
		} // end for-random#s
			
		// order the list of random numbers
		std::sort (randomNums.begin(), randomNums.end());  
		
		int n = 0; // this is the index of what old-particle(weight) we are on
		int i = 0; // this is the index of what random# (newParticle) we are on
		weightSum = 0; // reset weightSum
		std::vector<Particle> newParticleSet(numParticles);
		// for random#[i], see if it's greater than cumweight[n]
		while( i < numParticles ){
			
			// if current--SORTED--random# is under the weightSum, then make a newParticle corresponding to the particle-of-this-bin
			if(	randomNums[i] < weightSum ){
				newParticleSet[i] = particleSet[n-1];
// 				std::cout << "Assigned a new particle with pose " << particleSet[n-1].getPose() << std::endl;
				i += 1;
// 				std::cout <<"I increased i."<< std::endl;				
			}
			
			if( randomNums[i] >= weightSum ){
				weightSum += particleSet[n].getW();
				n += 1;
// 				std::cout <<"I increased n. randomNums[i] = " <<randomNums[i] << ", weight sum: " << weightSum << std::endl;				
			}
		} // end while
		
		particleSet = newParticleSet;
		for( int i = 0; i < numParticles; i++ ){
			// weights of all particles = 1/numParticles
			particleSet[i].setW( 1/numParticles );
		}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////				
		// if laser data, do stuff with that too.
		
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////				
		// apply noise
		double xCap = .1;
		double yCap = .1;
		double tCap = .02;
		UniformDistribution uniformX( -xCap, xCap );
		UniformDistribution uniformY( -yCap, yCap  );
		UniformDistribution uniformTheta( -tCap, tCap  );
		
		clock_t now = clock();
		uniformX.SetSeed( now );
		uniformY.SetSeed( now+10000 );
		uniformTheta.SetSeed( now+2000 );	
		
		for( int i = 0; i < numParticles; i++ ){
			PoseSE2 myPose = particleSet[i].getPose();

		double newX = myPose.getX()+uniformX.Sample();
		double newY = myPose.getY()+uniformY.Sample();
		double newTheta = myPose.getTheta()+uniformTheta.Sample();
		
		if(newX>map.GetXSize()){newX=map.GetXSize();};
		if(newX<0){newX=0;};
		if(newY>map.GetXSize()){newY=map.GetYSize();};
		if(newY<0){newY=0;};
		if(newTheta>2*M_PI){newTheta=newTheta-2*M_PI;};
		if(newTheta<0){newTheta=2*M_PI+newTheta;};		
		
		PoseSE2 newPose = PoseSE2( newX, newY, newTheta );
		particleSet[i].setPose( newPose );
		
		}

	}
	
	
	
	
	
	
	
	
	
}