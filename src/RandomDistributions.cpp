#include "rspf/RandomDistributions.h"

#include <sstream>

namespace rspf {

	UniformDistribution::UniformDistribution( double lower, double upper ) {
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
	
}