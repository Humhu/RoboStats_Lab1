#ifndef _LASER_SENSOR_MODEL_H_
#define _LASER_SENSOR_MODEL_H_

#include "rspf/SensorModel.h"
#include "rspf/Map.h"
//#include "rspf/Particle.h"

namespace rspf {

	class LaserSensorModel : public SensorModel {
    public:
		LaserSensorModel( const Map& _map ); 
		virtual void weightParticle( Particle& particle, const SensorData& data );
		

    private: 
		const Map& map;		
		
		std::vector<double> RayTrace( Particle particle, SensorData data );	

    }; // class
		
}

#endif