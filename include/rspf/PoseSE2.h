#ifndef _POSE_SE2_H_
#define _POSE_SE2_H_

#include <Eigen/Geometry>
#include <iostream>

namespace rspf {

    class PoseSE3;

    class PoseSE2 {
    public:

        typedef Eigen::Vector3d Vector;
        typedef Eigen::Transform<double, 2, Eigen::Isometry> Transform;
        typedef Eigen::Matrix<double, 3, 3> Matrix;
        typedef Eigen::Rotation2D<double> Rotation;
        typedef Eigen::Translation<double, 2> Translation;
        typedef Translation::VectorType TranslationVector;

        PoseSE2();
        explicit PoseSE2( double x, double y, double theta );
        explicit PoseSE2( const Transform& trans );
        explicit PoseSE2( const Matrix& m );
        explicit PoseSE2( const Rotation& r, const Translation& t );

        Matrix ToMatrix() const;
        Transform GetTransform() const;
        Vector ToVector() const;
        PoseSE2 Inverse() const;

        PoseSE2 operator*( const PoseSE2& other ) const;
        PoseSE2 operator/( const PoseSE2& other ) const;

        friend std::ostream& operator<<( std::ostream& os, const PoseSE2& se2 );

    protected:

        Transform trans;

        virtual void Print( std::ostream& os ) const;

    };

    std::ostream& operator<<(std::ostream& os, const PoseSE2& se2);

}

#endif

