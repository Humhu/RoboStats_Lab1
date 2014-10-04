#ifndef DEFAULT_TRANSITION_MODEL_
#define DEFAULT_TRANSITION_MODEL_

#include "rspf/Particle.h" //quotes = local folder
#include "rspf/TransitionModel.h"
#include "rspf/Map.h"

namespace rspf {

    class DefaultTransitionModel : public TransitionModel {
    public:
		
		DefaultTransitionModel( const Map& _map ); 
		virtual void transitionParticle( Particle& particle, const SensorData& data );

	private: 
		
		const Map& map;		
 		
    }; // class
    
}

#endif 