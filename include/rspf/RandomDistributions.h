#ifndef _UNIFORM_DISTRIBUTION_H_
#define _UNIFORM_DISTRIBUTION_H_

#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/uniform_01.hpp>

#include "rspf/Parameterized.h"

namespace rspf {

	template < class Generator, class S = double >
	class DistributionBase {
	public:

		DistributionBase() :
			adapter( engine, generator ) {}

		void SetSeed( unsigned int seed ) {
			engine.seed( seed );
		}
		
	protected:

		typedef S ScalarType;
		typedef boost::mt19937 RandEngine;
		typedef Generator GeneratorType;
		typedef boost::variate_generator<RandEngine&, GeneratorType> RandAdapter;

		RandEngine engine;
		GeneratorType generator;
		RandAdapter adapter;

		DistributionBase::ScalarType SampleRaw() {
			return adapter();
		}
	};

	/*! \brief A continuous uniform random number generator. */
	class UniformDistribution: public DistributionBase< boost::uniform_01<double> > {
	public:

		UniformDistribution( double lower, double upper );
		UniformDistribution( const PropertyTree& ptree );

		void SetBounds( double lower, double upper );
		double GetLowerBound() const;
		double GetUpperBound() const;

		double Sample();

	protected:

		boost::uniform_01<double> generator;
		
		double lowerBound;
		double scale; // Difference of upper and lower

	};

	/*! \brief A univariate Gaussian random number generator. */
	class NormalDistribution : public DistributionBase< boost::normal_distribution<double> > {
	public:

		NormalDistribution( double _mean, double variance );
		NormalDistribution( const PropertyTree& ptree );

		void SetMean( double _mean );
		void SetVariance( double variance );

		double GetMean() const;
		double GetVariance() const;

		double Sample();

	protected:

		double mean;
		double sigma; // Square root of variance (std dev)
		
	};
	
}

#endif