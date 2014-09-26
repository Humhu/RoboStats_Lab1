#ifndef TRANSITION_MODEL_
#define TRANSITION_MODEL_

#include "rspf/Particle.h" //quotes = local folder
// #include <vector> //carats = system folder

namespace rspf {

    class TransitionModel {
    public:
		TransitionModel(); 
		virtual void transitionParticle( Particle x ) = 0;


    private: 

    }; // class
    
}

#endif 