#include "rspf/RandomDistributions.h"

#include <sstream>

namespace rspf {

	UniformDistribution::UniformDistribution( double lower, double upper ) {
		SetBounds( lower, upper );
	}

	UniformDistribution::UniformDistribution( const PropertyTree& ptree ) {
		double lower = ptree.get<double>("lower_bound");
		double upper = ptree.get<double>("upper_bound");
		SetBounds( lower, upper );
	}

	void UniformDistribution::SetBounds( double lower, double upper ) {
		if( lower >= upper ) {
			std::stringstream ss;
			ss << "Lower bound " << lower << " is less than upper bound " << upper;
			throw std::logic_error( ss.str() );
		}

		lowerBound = lower;
		scale = upper - lower;
	}

	double UniformDistribution::GetLowerBound() const {
		return lowerBound;
	}

	double UniformDistribution::GetUpperBound() const {
		return lowerBound + scale;
	}

	double UniformDistribution::Sample() {
		ScalarType raw = SampleRaw();
		ScalarType transformed = lowerBound + raw*scale;
		return transformed;
	}

	NormalDistribution::NormalDistribution( double _mean, double variance ) {
		SetMean( _mean );
		SetVariance( variance );
	}

	NormalDistribution::NormalDistribution( const PropertyTree& ptree ) {
		SetMean( ptree.get<double>("mean") );
		SetVariance( ptree.get<double>("variance") );
	}

	void NormalDistribution::SetMean( double _mean ) {
		mean = _mean;
	}

	void NormalDistribution::SetVariance( double variance ) {
		if( variance < 0 ) {
			std::stringstream ss;
			ss << "Variance " << variance << " is negative." << std::endl;
			throw std::logic_error( ss.str() );
		}
		sigma = std::sqrt( variance );
	}

	double NormalDistribution::GetMean() const {
		return mean;
	}

	double NormalDistribution::GetVariance() const {
		return sigma*sigma;
	}

	double NormalDistribution::Sample() {
		ScalarType raw = SampleRaw();
		ScalarType transformed = sigma*raw + mean;
		return transformed;
	}
	
	double NormalDistribution::GetProb( double meas ) {
		double a = 1/(sigma*sqrt(2*M_PI));
		double b = mean;
		double c = sigma;
		double d = 0;
		double y = a * std::exp(-std::pow((meas-b),2) / (2*std::pow(c,2))) + d;
		return y;		
	}
	
}