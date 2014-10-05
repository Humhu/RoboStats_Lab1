#ifndef DEFAULT_TRANSITION_MODEL_
#define DEFAULT_TRANSITION_MODEL_

#include "rspf/Particle.h" //quotes = local folder
#include "rspf/TransitionModel.h"
#include "rspf/Map.h"
#include "rspf/RandomDistributions.h"
#include "rspf/Parameterized.h"

namespace rspf {

    class DefaultTransitionModel : public TransitionModel {
    public:

		typedef std::shared_ptr<DefaultTransitionModel> Ptr;
		
		DefaultTransitionModel( const Map& _map, const PropertyTree& ptree );

		virtual void transitionParticle( Particle& particle, const SensorData& data );

	private: 
		
		const Map& map;

		NormalDistribution xNoise;
		NormalDistribution yNoise;
		NormalDistribution thNoise;

		void SeedDistributions();
		
 		
    }; // class
    
}

#endif 