#ifndef _SENSOR_MODEL_H_
#define _SENSOR_MODEL_H_

#include "rspf/Particle.h"
#include "rspf/RobotLogReader.h"
#include "rspf/Map.h"

namespace rspf {

	class SensorModel {
	public:
		
		SensorModel();
		
		virtual void weightParticle( Particle& particle, const SensorData& data ) = 0;
		
	};
	
}

#endif