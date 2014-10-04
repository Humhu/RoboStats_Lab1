#include "rspf/PoseSE2.h"
#include <cmath>

namespace rspf {

    PoseSE2::PoseSE2() :
        trans( Transform::Identity() ) {}

    PoseSE2::PoseSE2(const double x, const double y, const double theta) {
        Rotation rot( theta );
        Translation zero( 0, 0 );
        Translation lin( x, y );

        // Need this step for the transform composition to be valid
        Transform rTrans = rot*zero;
        
        trans = lin*rTrans;
    }

    PoseSE2::PoseSE2( const Matrix& m ) :
        trans( m ) {}
        

    // TODO Remove this because of inaccuracy!
    PoseSE2::PoseSE2( const Transform &t ) :
        trans( t ) {}

    PoseSE2::Matrix PoseSE2::ToMatrix() const {
        return trans.matrix();
    }

    PoseSE2::Transform PoseSE2::GetTransform() const {
        return trans;
    }

    PoseSE2::Vector PoseSE2::ToVector() const {
        Transform::ConstTranslationPart lin = trans.translation();
        Rotation rot(0);
        rot.fromRotationMatrix( trans.linear() );

        Vector ret;
        ret << lin(0), lin(1), rot.angle();
        return ret;
    }
    
    
   /*		double getX() const;
		double getY() const;
		double getTheta() const;*/
	double PoseSE2::getX() const {
		Transform::ConstTranslationPart lin = trans.translation();
		return lin(0);
	}
	double PoseSE2::getY() const {
		Transform::ConstTranslationPart lin = trans.translation();
		return lin(1);
	}   
	double PoseSE2::getTheta() const {
        Rotation rot(0);
        rot.fromRotationMatrix( trans.linear() );
		return rot.angle();
	}   
   
   
    PoseSE2 PoseSE2::Inverse() const {
        return PoseSE2( trans.inverse() );
    }

    PoseSE2 PoseSE2::operator*(const PoseSE2& other) const {
        return PoseSE2( trans*other.trans );
    }

    PoseSE2 PoseSE2::operator/(const PoseSE2& other) const {
        return PoseSE2( trans*other.trans.inverse() );
    }

    void PoseSE2::Print(std::ostream& os) const {
        Vector vec = ToVector();
        os << vec(0) << " " << vec(1) << " " << vec(2);
    }

    std::ostream& operator<<(std::ostream& os, const PoseSE2& se2) {
        se2.Print(os);
        return os;
    }

}