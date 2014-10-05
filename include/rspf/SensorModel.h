#ifndef _SENSOR_MODEL_H_
#define _SENSOR_MODEL_H_

#include "rspf/Particle.h"
#include "rspf/RobotLogReader.h"
#include "rspf/Map.h"

#include <memory>

namespace rspf {

	class SensorModel {
	public:

		typedef std::shared_ptr<SensorModel> Ptr;
		
		SensorModel();
		
		virtual void weightParticle( Particle& particle, const SensorData& data ) = 0;

// 		std::vector< std::vector<double> > lastTraces; // HACK
		
	};
	
}

#endif