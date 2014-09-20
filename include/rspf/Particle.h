#ifndef PARTICLE_
#define PARTICLE_

namespace rspf {

    class Particle {
    public:
		Particle(); //state x,y,theta
		double getX();
		double getY();
		double getTheta();
		double getW();
    
		void setX( double x );
		void setY( double x );
		void setTheta( double x );
		void setW( double x );
		
    private: 
		double x;
		double y;
		double theta;
		double w; //weight
		
    }; // class
    
}

#endif 