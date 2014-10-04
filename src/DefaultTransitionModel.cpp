#include "rspf/DefaultTransitionModel.h"

#include "rspf/RandomDistributions.h"
// #include <time.h>  


namespace rspf {

    DefaultTransitionModel::DefaultTransitionModel( const Map& _map) :
		map( _map ) {}
		
	void DefaultTransitionModel::transitionParticle( Particle& particle, 
													 const SensorData& data )
	{
			
		PoseSE2 myDisplacement = data.displacement; // get displacement
		
		particle.setPose( particle.getPose()*myDisplacement ); // apply displacement to particle		
		
		// apply noise
		double xCap = 1; //deca-meters
		double yCap = 1; //deca-meters
		double tCap = .1;
		UniformDistribution uniformX( -xCap, xCap );
		UniformDistribution uniformY( -yCap, yCap  );
		UniformDistribution uniformTheta( -tCap, tCap  );
		
		clock_t now = clock();
		uniformX.SetSeed( now );
		uniformY.SetSeed( now+10000 );
		uniformTheta.SetSeed( now+2000 );	
		
		PoseSE2 myPose = particle.getPose();

		double newX = myPose.getX()+uniformX.Sample();
		double newY = myPose.getY()+uniformY.Sample();
		double newTheta = myPose.getTheta()+uniformTheta.Sample();
		
		if(newX>map.GetXSize()){ newX = map.GetXSize(); }
		if(newX<0){ newX = 0; }
		if(newY>map.GetXSize()){ newY = map.GetYSize(); }
		if(newY<0){ newY = 0; }
		if(newTheta>2*M_PI){ newTheta = newTheta - 2*M_PI; }
		if(newTheta<0){ newTheta = 2*M_PI+newTheta; }	
		
		PoseSE2 newPose = PoseSE2( newX, newY, newTheta );
		particle.setPose( newPose );
    
	}

	
}