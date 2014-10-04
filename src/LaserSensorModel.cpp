#include "rspf/LaserSensorModel.h"
#include "rspf/RandomDistributions.h"
// #include <time.h>  


namespace rspf {

    LaserSensorModel::LaserSensorModel( const Map& _map) :
		map( _map ) {}
		
	void LaserSensorModel::weightParticle( Particle& particle, 
											const SensorData& data )
	{
		if(!data.hasScan){
			return;
		}
		
//		double weightSum = 0; // reset weightSum, because we're about to re-weight things
			
		// get the laser ranges we expect to see for this particle
		std::vector<double> zhat = RayTrace( particle, data );
		
		// If the ray tracing is infeasible (signalled by empty return), we set the particle weight to zero
		if( zhat.empty() ) {
			particle.setW( 0 );
			return;
		}
		
		double cumProb = 1; // initialize the cumulative probability of all the laser probabilities for this particle
					
		// find out what the probability of seeing the laser from this position is
		for( unsigned int s = 0; s < data.ScanSize; s++ ) {
			NormalDistribution laserPDF( zhat[s], 10 ); // make a laser pdf centered at zhat
			// find the probability of the real laser measurement under this distribution, and mulitply it into the cumulative
			double indivProb = laserPDF.GetProb( data.points[s] ) + 1;
			cumProb = cumProb * indivProb;
			
		} // end find-cumlative-probability-of-every-laser-beam-for-this-particle
	
		particle.setW( cumProb * particle.getW() ); // update the particle weight
				
	}// end LaserSensorModel::weightParticle
	
	std::vector<double> LaserSensorModel::RayTrace( Particle particle, SensorData data )
	{
		std::vector<double> zhat(data.ScanSize); // initialize vector of probabilities-per-laser-beam

		PoseSE2 laserH = particle.getPose()*data.laserOffset; // find laser position in world
		
		double laserAngle = laserH.getTheta(); // get the angle of the laser in the world
		double scanAngle = data.StartAngle + laserAngle;  // get the starting scan angle (relative to the laser angle)
							
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

				double mapVal = map.GetValue( xL, yL );

				prob = prob*mapVal; // check the map, & update the probability that the laser hit something

			} // end while-probability-of-ray-hasn't-hit-a-wall
			
			xL = xL-laserH.getX();
			yL = yL-laserH.getY();
			zhat[s] = std::sqrt( xL*xL + yL*yL ); // store the distance that the laser went on this scan before it hit something
			scanAngle = scanAngle + data.ScanResolution; // update the scan angle
			
		} // end for-every-scan
		
		return zhat;
		
	} // end RayTrace method	
	
}	
	