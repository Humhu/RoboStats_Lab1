#include "rspf/Particle.h"

namespace rspf {

    Particle::Particle() {
// 		x = 0;
// 		y = 0;
// 		theta = 0;
		w = 0;
		
	}
	
// 	double Particle::getX() { return x; }
// 	double Particle::getY() { return y; }
// 	double Particle::getTheta() { return theta; }
	double Particle::getW() const { return w; }
	PoseSE2 Particle::getPose() const { return pose; }
    
// 	void Particle::setX( double _xin ) { x = _xin; }
// 	void Particle::setY( double _xin ) { y = _xin; }
// 	void Particle::setTheta( double _xin ) { theta = _xin; }
	void Particle::setW( double _xin ) { w = _xin; } 
	void Particle::setPose( PoseSE2 _posein ) { pose = _posein; }
    
}