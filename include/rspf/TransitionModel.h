#ifndef TRANSITION_MODEL_
#define TRANSITION_MODEL_

#include "rspf/Particle.h" //quotes = local folder
#include "rspf/RobotLogReader.h"

// #include <vector> //carats = system folder

namespace rspf {

    class TransitionModel {
    public:
		TransitionModel(); 
		virtual void transitionParticle( Particle& particle, const SensorData& data  ) = 0;


    private: 

    }; // class
    
}

#endif 