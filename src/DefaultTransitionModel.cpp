#include "rspf/DefaultTransitionModel.h"

#include "rspf/RandomDistributions.h"
// #include <time.h>  


namespace rspf {

    DefaultTransitionModel::DefaultTransitionModel( const Map& _map, const PropertyTree& ptree ) :
		map( _map ),
		xNoise( ptree.get_child("x_noise") ),
		yNoise( ptree.get_child("y_noise") ),
		thNoise( ptree.get_child("th_noise") ) {

		SeedDistributions();
	}


	void DefaultTransitionModel::SeedDistributions() {
		clock_t now = clock();
		xNoise.SetSeed( now );
		yNoise.SetSeed( now + 10 );
		thNoise.SetSeed( now + 20 );
	}
	
	void DefaultTransitionModel::transitionParticle( Particle& particle, 
													 const SensorData& data )
	{

		PoseSE2 noise( xNoise.Sample(), yNoise.Sample(), thNoise.Sample() );
		PoseSE2 corruptedPose = particle.getPose() * noise * data.displacement;
		particle.setPose( corruptedPose );
		
// 		if(newX>map.GetXSize()){ newX = map.GetXSize(); }
// 		if(newX<0){ newX = 0; }
// 		if(newY>map.GetXSize()){ newY = map.GetYSize(); }
// 		if(newY<0){ newY = 0; }
// 		if(newTheta>2*M_PI){ newTheta = newTheta - 2*M_PI; }
// 		if(newTheta<0){ newTheta = 2*M_PI+newTheta; }	
// 		
// 		PoseSE2 newPose = PoseSE2( newX, newY, newTheta );
    
	}

	
}