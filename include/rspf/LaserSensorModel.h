#ifndef _LASER_SENSOR_MODEL_H_
#define _LASER_SENSOR_MODEL_H_

#include "rspf/SensorModel.h"
#include "rspf/Map.h"
#include "rspf/Parameterized.h"
#include "rspf/RandomDistributions.h"

namespace rspf {

	class LaserSensorModel : public SensorModel {
    public:

		typedef std::shared_ptr<LaserSensorModel> Ptr;
		
		LaserSensorModel( const Map& _map, const PropertyTree& ptree );
		
		virtual void weightParticle( Particle& particle, const SensorData& data );
		
    private:
		
		const Map& map;
		
		unsigned int laserSubsample;
		double raytraceStepsize;
		double raytraceThreshold;

		unsigned int numPoints;
		unsigned int numProbsToSkip;
		
		double gaussianWeight;
		double gaussianVarianceScale;
		double uniformWeight;
		double exponentialWeight;
		double maxRangeWeight;
		
		NormalDistribution gaussianComponent;
		UniformDistribution uniformComponent;
		double exponentialCoefficient;
		
		std::vector<double> RayTrace( Particle particle, SensorData data );

		double CalculateGaussian( double rEst, double rTrue );
		double CalculateUniform( double rTrue );
		double CalculateExponential( double rTrue );
		double CalculateMaxRange( double rTrue );

    }; // class
		
}

#endif