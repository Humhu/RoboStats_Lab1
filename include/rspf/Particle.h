#ifndef PARTICLE_
#define PARTICLE_

#include "rspf/PoseSE2.h"

namespace rspf {

    class Particle {
    public:
		Particle(); //state x,y,theta
		PoseSE2 getPose() const;
// 		double getX();
// 		double getY();
// 		double getTheta();
		double getW() const;
//     
 		void setPose( PoseSE2 x );
// 		void setX( double x );
// 		void setY( double x );
// 		void setTheta( double x );
		void setW( double x );
		
    private: 
		PoseSE2 pose;
// 		double x;
// 		double y;
// 		double theta;
		double w; //weight
		
    }; // class
    
}

#endif 