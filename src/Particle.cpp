#include "rspf/Particle.h"

namespace rspf {

    Particle::Particle() {
		x = 0;
		y = 0;
		theta = 0;
		w = 0;
	}
	
	double Particle::getX() { return x; }
	double Particle::getY() { return y; }
	double Particle::getTheta() { return theta; }
	double Particle::getW() { return w; }
    
	void Particle::setX( double _xin ) { x = _xin; }
	void Particle::setY( double _xin ) { x = _xin; }
	void Particle::setTheta( double _xin ) { x = _xin; }
	void Particle::setW( double _xin ) { x = _xin; } 
    
}