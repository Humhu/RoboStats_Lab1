#ifndef STATIC_TRANSITION_MODEL_
#define STATIC_TRANSITION_MODEL_

#include "rspf/TransitionModel.h"

namespace rspf {

    class StaticTransitionModel : public TransitionModel {
    public:
		
		StaticTransitionModel(); 

		virtual void transitionParticle( Particle& particle, const SensorData& data );

    private: 
 		
    }; // class
    
}

#endif 