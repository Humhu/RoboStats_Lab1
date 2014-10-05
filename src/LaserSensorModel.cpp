#include "rspf/LaserSensorModel.h"
#include "rspf/RandomDistributions.h"
// #include <time.h>  

namespace rspf {

    LaserSensorModel::LaserSensorModel( const Map& _map, const PropertyTree& ptree ) :
		map( _map ),
		laserSubsample( ptree.get<unsigned int>("laser_subsample") ),
		gaussianWeight( ptree.get<double>("gaussian_weight") ),
		uniformWeight( ptree.get<double>("uniform_weight") ),
		exponentialWeight( ptree.get<double>("exponential_weight") ),
		maxRangeWeight( ptree.get<double>("max_range_weight") ),
		gaussianComponent( ptree.get_child("gaussian_component") ),
		uniformComponent( ptree.get_child("uniform_component") ),
		exponentialCoefficient( ptree.get<double>("exponential_coeff") ),
		raytraceStepsize( ptree.get<double>("raytrace_stepsize") ),
		raytraceThreshold( ptree.get<double>("raytrace_threshold") ) {}
		
	void LaserSensorModel::weightParticle( Particle& particle, 
											const SensorData& data )
	{
		if(!data.hasScan){
			return;
		}
		
		// get the laser ranges we expect to see for this particle
		std::vector<double> zhat = RayTrace( particle, data );
		
		// If the ray tracing is infeasible (signalled by empty return), we set the particle weight to zero
		if( zhat.empty() ) {
			particle.setW( 0 );
			return;
		}

// 		double cumProb = 0;
		double cumProb = 1; // initialize the cumulative probability of all the laser probabilities for this particle
					
		// find out what the probability of seeing the laser from this position is
		for( unsigned int s = 0; s < data.ScanSize; s+= laserSubsample ) {

			double rTrue = data.points[s];
			double rEst = zhat[s];
			
			double indivProb = CalculateGaussian( rEst, rTrue )
							+ CalculateUniform( rTrue )
							+ CalculateExponential( rTrue )
							+ CalculateMaxRange( rTrue );
			
			// find the probability of the real laser measurement under this distribution, and mulitply it into the cumulative
			cumProb = cumProb * indivProb;
// 			cumProb += indivProb;

// 			std::cout << "rEst: " << rEst << ", rTrue: " << rTrue << std::endl;
// 			std::cout << "indivProb: " << indivProb << ", cumProb: " << cumProb << std::endl;
			
		} // end find-cumlative-probability-of-every-laser-beam-for-this-particle
	
		particle.setW( cumProb * particle.getW() ); // update the particle weight
				
	}// end LaserSensorModel::weightParticle
	
	std::vector<double> LaserSensorModel::RayTrace( Particle particle, SensorData data )
	{
		std::vector<double> zhat(data.ScanSize); // initialize vector of probabilities-per-laser-beam

		PoseSE2 laserH = particle.getPose()*data.laserOffset; // find laser position in world
		
		double laserAngle = laserH.getTheta(); // get the angle of the laser in the world
		double scanAngle = data.StartAngle + laserAngle;  // get the starting scan angle (relative to the laser angle)
							
		for( unsigned int s = 0; s < data.ScanSize; s += laserSubsample ) { //data.ScanSize
			
			double prob = 1; // this is the starting probability that the laser beam passes through a cell
			
			double xL = laserH.getX(); // this is the x-starting position of the laser beam
			double yL = laserH.getY(); // this is the y-starting position of the laser beam

			double dX = raytraceStepsize * std::cos( scanAngle );
			double dY = raytraceStepsize * std::sin( scanAngle );
			double r = 0;
			
			if( xL < 0 || xL >= map.GetXSize() - 1 || yL < 0 || yL >= map.GetYSize() - 1 ) {
				return std::vector<double>() ;
			}
			
			while( prob > raytraceThreshold ){
				
				xL += dX; // step along the laser ray
				yL += dY;
				r += raytraceStepsize;
				
				if( xL < 0 ) {
					xL = 0;
				}
				else if( xL >= map.GetXSize() - 1 ) {
					xL = map.GetXSize() - 1;
				}
				
				if( yL < 0 ) {
					yL = 0;
				}
				else if( yL >= map.GetYSize() - 1 ) {
					yL = map.GetYSize() - 1;
				}

				double mapVal = map.GetValue( xL, yL );

				prob = prob*mapVal; // check the map, & update the probability that the laser hit something

			} // end while-probability-of-ray-hasn't-hit-a-wall
			
			xL = xL-laserH.getX();
			yL = yL-laserH.getY();
// 			zhat[s] = std::sqrt( xL*xL + yL*yL ); // store the distance that the laser went on this scan before it hit something
			zhat[s] = 0.1*r;
			scanAngle += data.ScanResolution; // update the scan angle
			
		} // end for-every-scan
		
		return zhat;
		
	} // end RayTrace method	

	double LaserSensorModel::CalculateGaussian( double rEst, double rTrue ) {
		return gaussianWeight * gaussianComponent.GetProb( rEst - rTrue );
	}

	double LaserSensorModel::CalculateUniform( double rTrue ) {
		return uniformWeight * uniformComponent.GetProb( rTrue );
	}

	double LaserSensorModel::CalculateExponential( double rTrue ) {
		return exponentialWeight * std::exp( exponentialCoefficient * rTrue );
	}

	double LaserSensorModel::CalculateMaxRange( double rTrue ) {
		// There is some roundoff error
		if( std::abs( rTrue - SensorData::MaxRange ) < 10E-6 ) {
			return maxRangeWeight;
		}
		return 0.0;
	}
	
}	
	