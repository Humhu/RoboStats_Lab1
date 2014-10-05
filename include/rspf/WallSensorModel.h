#ifndef _WALL_SENSOR_MODEL_H_
#define _WALL_SENSOR_MODEL_H_

#include "rspf/SensorModel.h"
#include "rspf/Map.h"

namespace rspf {

	class WallSensorModel : public SensorModel {
	public:

		typedef std::shared_ptr<WallSensorModel> Ptr;
		
		WallSensorModel( const Map& _map );

		virtual void weightParticle( Particle& particle, const SensorData& data );
		
	private:

		const Map& map;
		double xMin;
		double xMax;
		double yMin;
		double yMax;
		
	};
	
}

#endif