#include "rspf/ParticleFilter.h"

#include "rspf/StaticTransitionModel.h"
#include "rspf/DefaultTransitionModel.h"

#include "rspf/RandomDistributions.h"
#include <time.h>       

#include "rspf/LaserSensorModel.h"

namespace rspf {
	
	ParticleFilter::ParticleFilter( const Map& _map, unsigned int numParticles ) : map(_map) //default arguments starting from the back. stupid.
	{
		Initialize( numParticles );
	}

	ParticleFilter::ParticleFilter( const Map& _map, const PropertyTree& ptree ) : map( _map )
	{
		Initialize( ptree.get<unsigned int>("num_particles") );
	}

	void ParticleFilter::Initialize( unsigned int numParticles ) 
	{
		// UniformDistribution( double lower, double upper );
		UniformDistribution uniformX( 0, map.GetXSize() );
		UniformDistribution uniformY( 0, map.GetYSize() );
		UniformDistribution uniformTheta( 0, 2*M_PI );
		
		clock_t now = clock();

		uniformX.SetSeed( now );
		uniformY.SetSeed( now+10000 );
		uniformTheta.SetSeed( now+2000 );
		
		particleSet = std::vector<Particle>(numParticles);
		for( unsigned int i = 0; i < numParticles; i++ ) 
		{
			while(true){
				
				// draw a pose  	// explicit PoseSE2( double x, double y, double theta );
				PoseSE2 myPose = PoseSE2( uniformX.Sample(), uniformY.Sample(), uniformTheta.Sample() );
				
				// check if pose is in map		//get value of map at particle x,y	
				//map is -1 for black regions, [0 1] for probability of fill elsewhere (0 = occupied, 1 = empty)
				double mapOccupancy = map.GetValue( myPose.getX(), myPose.getY() );

				// if x,y not object, we're ok, so break out of the while loop
				if( mapOccupancy != -1 ){
					// draw a particle
						particleSet[i] = Particle();
						
					// assign pose to Particle
						particleSet[i].setPose( myPose );  
						
					break;
					
				}	//end if				
			} // end while
		} // end for
		
		// Create the transition model		
		DefaultTransitionModel* model = new DefaultTransitionModel( map );
		transitionModel = model;
		
		LaserSensorModel* mmodel = new LaserSensorModel( map );
		sensorModel = mmodel;
		
		
		
	} //end initialize
	
	std::vector<Particle> ParticleFilter::GetParticles()
	{
		return particleSet;
	}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
// ray trace the way to freedom
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
/*	
	std::vector<double> ParticleFilter::RayTrace( Particle particle, SensorData data, const Map& map )
	{
		std::vector<double> zhat(data.ScanSize); // initialize vector of probabilities-per-laser-beam

		PoseSE2 laserH = particle.getPose()*data.laserOffset; // find laser position in world
		
		double laserAngle = laserH.getTheta(); // get the angle of the laser in the world
		double scanAngle = data.StartAngle + laserAngle;  // get the starting scan angle (relative to the laser angle)
				
// 		std::cout << "Particle pose: " << particle.getPose() << std::endl;
// 		std::cout << "Laser offset: " << data.laserOffset << std::endl;
// 		std::cout << "Laser H: " << laserH << std::endl;
				
		for( unsigned int s = 0; s < data.ScanSize; s++ ) { //data.ScanSize
			
			double prob = 1; // this is the starting probability that the laser beam passes through a cell
			double step = 1; // this is the step distance at which we will re-check the laser probability
			double threshold = 0; // this is the threshold for when we decide the laser has hit something
			
			double xL = laserH.getX(); // this is the x-starting position of the laser beam
			double yL = laserH.getY(); // this is the y-starting position of the laser beam
			double cosS = std::cos( scanAngle );
			double sinS = std::cos( scanAngle );
			
			if( xL < 0 || xL > map.GetXSize() || yL < 0 || yL > map.GetYSize() ) {
				return std::vector<double>() ;
			}
			
			while( prob > threshold ){
				
				xL = xL + step * cosS; // step along the laser ray
				yL = yL + step * sinS;
				
				if( xL < 0 ) {
					xL = 0;
				}
				else if( xL > map.GetXSize() ) {
					xL = map.GetXSize();
				}
				
				if( yL < 0 ) {
					yL = 0;
				}
				else if( yL > map.GetYSize() ) {
					yL = map.GetYSize();
				}
// 				std::cout << xL << " " << yL << " " << scanAngle << std::endl;			
				double mapVal = map.GetValue( xL, yL );
// 				std::cout << mapVal << std::endl;							
				prob = prob*mapVal; // check the map, & update the probability that the laser hit something
// 				std::cout << prob << std::endl;
				
			} // end while-probability-of-ray-hasn't-hit-a-wall
			
			xL = xL-laserH.getX();
			yL = yL-laserH.getY();
			zhat[s] = std::sqrt( xL*xL + yL*yL ); // store the distance that the laser went on this scan before it hit something
			scanAngle = scanAngle + data.ScanResolution; // update the scan angle
			
		} // end for-every-scan
		
		return zhat;
		
	} // end RayTrace method
	*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		

	

	void ParticleFilter::handleData ( SensorData data )
	{
		int numParticles = particleSet.size();
		
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
// 		// apply position update to particles -- always happens
		for(int i = 0; i<numParticles; i++) 
		{		
			transitionModel->transitionParticle( particleSet[i], data );
		}
/* 				
// 		PoseSE2 myDisplacement = data.displacement;
// 		// for all particles, displace them (in their own frames)
// 		
// 		for ( int i = 0; i < numParticles; i++ ) {
// 			particleSet[i].setPose( particleSet[i].getPose()*myDisplacement );
// 		} // end for
*/ 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		// update weights based on "measurement data" from the wall
		double weightSum = 0;
		for ( int i = 0; i < numParticles; i++ ) 
		{
			// for every particle, make a "measurement"--i.e., check the map to see the probability of being in a wall 
			PoseSE2 myPose = particleSet[i].getPose();
			double mapOccupancy = map.GetValue( myPose.getX(), myPose.getY() );	
						
			if( mapOccupancy == -1 ){
				particleSet[i].setW( 0 ); 
			}
			else{
				particleSet[i].setW( mapOccupancy ); 
			} //map is -1 for black regions, [0 1] for probability of fill elsewhere (0 = occupied, 1 = empty)		
			
			weightSum += particleSet[i].getW();
			
		} //end for finding-map-weights-of-particles

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////				
// major lazer		
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////				
		// if we have laser data, weight the particles by that too				
		
			weightSum = 0;
			
			for ( int i = 0; i < numParticles; i++ ) 
			{
				//SensorModel->weightParticle( particleSet[i], data );		
				sensorModel->weightParticle( particleSet[i], data );	
				weightSum = weightSum + particleSet[i].getW();
			}
/*			
			weightSum = 0; // reset weightSum, because we're about to re-weight things

			for( int i = 0; i < numParticles; i++ ) {
				
				// get the laser ranges we expect to see for this particle
				std::vector<double> zhat = RayTrace( particleSet[i], data, map );
				
				// If the ray tracing is infeasible (signalled by empty return), we set the particle weight to zero
				if( zhat.empty() ) {
					particleSet[i].setW( 0 );
					continue;
				}
				
				double cumProb = 1; // initialize the cumulative probability of all the laser probabilities for this particle
				
// 				std::cout << "Laser probs: ";
				
				// find out what the probability of seeing the laser from this position is
				for( unsigned int s = 0; s < data.ScanSize; s++ ) {
					NormalDistribution laserPDF( zhat[s], 10 ); // make a laser pdf centered at zhat
					// find the probability of the real laser measurement under this distribution, and mulitply it into the cumulative
					double indivProb = laserPDF.GetProb( data.points[s] ) + 1;
					cumProb = cumProb * indivProb;
					
// 					std::cout << "(" << data.points[s] << ", " << zhat[s] << "), ";
				} // end find-cumlative-probability-of-every-laser-beam-for-this-particle
// 				std::cout << std::endl;
			
// 				std::cout << "Particle " << i << " has laser prob: " << cumProb << std::endl;
				particleSet[i].setW( cumProb * particleSet[i].getW() ); // update the particle weight
				weightSum += particleSet[i].getW(); // update the sum of weights
				
			} // end for
*/			
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 		// resample 
/* 		
// 		// now draw one random # per particle, where the random # is between 0 and max_val of number line, ie, sum of weights
// 		UniformDistribution uniformR( 0, weightSum );		
// 		uniformR.SetSeed( clock() );
// 		
// 		std::vector<double> randomNums(numParticles);	
// 		
// 		for( int i = 0; i < numParticles; i++ ){
// 			randomNums[i] = uniformR.Sample();
// 		} // end for-random#s
// 			
// 		// order the list of random numbers
// 		std::sort (randomNums.begin(), randomNums.end()); 
// 
// 		int n = 0; // this is the index of what old-particle(weight) we are on
// 		int i = 0; // this is the index of what random# (newParticle) we are on
// 		weightSum = 0; // reset weightSum
// 		std::vector<Particle> newParticleSet(numParticles);
// 
// 		while( i < numParticles ){		// for random#[i], see if it's greater than cumweight[n]
// 			
// 			// if current--SORTED--random# is under the weightSum, then make a newParticle corresponding to the particle-of-this-bin
// 			if(	randomNums[i] < weightSum ){
// 				newParticleSet[i] = particleSet[n-1];
// // 				std::cout << "Assigned a new particle with pose " << particleSet[n-1].getPose() << std::endl;
// 				i += 1;
// // 				std::cout <<"I increased i."<< std::endl;				
// 			}
// 			
// 			if( randomNums[i] >= weightSum ){
// 				weightSum += particleSet[n].getW();
// 				n += 1;
// // 				std::cout <<"I increased n. randomNums[i] = " <<randomNums[i] << ", weight sum: " << weightSum << std::endl;				
// 			}
// 		} // end while
*/		
		double combtooth = weightSum/(numParticles+1);
		UniformDistribution uniformR( 0, combtooth );		
		uniformR.SetSeed( clock() );		
		double randomNum = uniformR.Sample();
 		//randomNums[1:end] = 1:numParticles*weightSum/numParticles + randomNums[0]
		
		int n = 0; // this is the index of what old-particle(weight) we are on
		int i = 0; // this is the index of what random# (newParticle) we are on
		weightSum = 0; // reset weightSum
		std::vector<Particle> newParticleSet(numParticles);

		while( i < numParticles ){		// for random#[i], see if it's greater than cumweight[n]
			
			// if current--SORTED--random# is under the weightSum, then make a newParticle corresponding to the particle-of-this-bin
			if(	randomNum < weightSum ){
				newParticleSet[i] = particleSet[n-1];
// 				std::cout << "Assigned a new particle with pose " << particleSet[n-1].getPose() << std::endl;
				i += 1;
				randomNum += combtooth;
// 				std::cout <<"I increased i."<< std::endl;				
			}
			
			if( randomNum >= weightSum ){
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
/* 		
// 		// apply noise
// 		double xCap = 1; //deca-meters
// 		double yCap = 1; //deca-meters
// 		double tCap = .1;
// 		UniformDistribution uniformX( -xCap, xCap );
// 		UniformDistribution uniformY( -yCap, yCap  );
// 		UniformDistribution uniformTheta( -tCap, tCap  );
// 		
// 		clock_t now = clock();
// 		uniformX.SetSeed( now );
// 		uniformY.SetSeed( now+10000 );
// 		uniformTheta.SetSeed( now+2000 );	
// 		
// 		for( int i = 0; i < numParticles; i++ ){
// 			PoseSE2 myPose = particleSet[i].getPose();
// 
// 		double newX = myPose.getX()+uniformX.Sample();
// 		double newY = myPose.getY()+uniformY.Sample();
// 		double newTheta = myPose.getTheta()+uniformTheta.Sample();
// 		
// 		if(newX>map.GetXSize()){newX=map.GetXSize();};
// 		if(newX<0){newX=0;};
// 		if(newY>map.GetXSize()){newY=map.GetYSize();};
// 		if(newY<0){newY=0;};
// 		if(newTheta>2*M_PI){newTheta=newTheta-2*M_PI;};
// 		if(newTheta<0){newTheta=2*M_PI+newTheta;};		
// 		
// 		PoseSE2 newPose = PoseSE2( newX, newY, newTheta );
// 		particleSet[i].setPose( newPose );
*/ 		
		}
}