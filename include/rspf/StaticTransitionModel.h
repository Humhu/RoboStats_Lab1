#ifndef STATIC_TRANSITION_MODEL_
#define STATIC_TRANSITION_MODEL_

#include "rspf/Particle.h" //quotes = local folder
#include "rspf/TransitionModel.h"

namespace rspf {

    class StaticTransitionModel : public TransitionModel {
    public:
		StaticTransitionModel(); 
		virtual void transitionParticle( Particle x );

    private: 
 		
    }; // class
    
}

#endif 