#include "rspf/FilterVisualizer.h"

#include <opencv2/imgproc/imgproc.hpp>

namespace rspf {

    bool FilterVisualizer::windowThreadInitialized = false;

    FilterVisualizer::FilterVisualizer( ParticleFilter& _filter, const Map& _map,
                                        const std::string& _windowName ) :
        filter( _filter ),
        map( _map ),
        windowName( _windowName ),
        mapScale( 1.0 ), // TODO Take as argument
        robotSize( 10.0 ) {

        if( !windowThreadInitialized ) {
            windowThreadInitialized = true;
            cv::startWindowThread();
        }

        // Initialize window
        cv::namedWindow( windowName, CV_WINDOW_AUTOSIZE );

        // Read map image
		 cv::Mat temp = cv::Mat( map.GetXSize(), map.GetYSize(), CV_8UC3 );
        for( unsigned int x = 0; x < map.GetXSize(); x++ ) {
            for( unsigned int y = 0; y < map.GetYSize(); y++ ) {
                double mapVal = map.GetValue( x, y );

                if( mapVal == -1.0 ) {
                    temp.at<cv::Vec3b>(x, y)[0] = 0;
					temp.at<cv::Vec3b>(x, y)[1] = 0;
					temp.at<cv::Vec3b>(x, y)[2] = 0;
                }
                else {
                    temp.at<cv::Vec3b>(x, y)[0] = 255*mapVal;
					temp.at<cv::Vec3b>(x, y)[1] = 255*mapVal;
					temp.at<cv::Vec3b>(x, y)[2] = 255*mapVal;
                }
            }
        }
        
        cv::Size scaledSize( std::round( map.GetXSize()*mapScale ),
                             std::round( map.GetYSize()*mapScale ) );
		cv::Mat resizeTemp = cv::Mat( scaledSize.width, scaledSize.height, CV_8UC3 );
		resize( temp, resizeTemp, scaledSize );
		
  		mapImage = resizeTemp.clone();
        
    }
 
    void FilterVisualizer::Update() {

        cv::Mat image = mapImage.clone();

        // Add things to the image here
        // TODO!
        std::vector<Particle> particles = filter.GetParticles();
		
        PlotRobotPoses( image, particles );
        
        cv::imshow( windowName, image );
    }

    void FilterVisualizer::PlotRobotPoses( cv::Mat& img, std::vector<Particle>& particles ) {

        unsigned int numPoses = particles.size();

        // Untransformed triangle points
        Eigen::Vector2d tip;
        tip << robotSize*2.0/3, 0;
        Eigen::Vector2d left;
        left << -robotSize/3.0, robotSize/4.0;
        Eigen::Vector2d right;
        right << -robotSize/3.0, -robotSize/4.0;
        
        // Each pose is plotted as a triangle
        cv::Point points[numPoses][3];
        const cv::Point* shapes[numPoses];
        int numPoints[numPoses];
        
        for( unsigned int i = 0; i < numPoses; i++ ) {

            // Transform the points using the poses
            PoseSE2::Transform trans = particles[i].getPose().GetTransform();
            trans.translation() = mapScale*trans.translation();
            Eigen::Vector2d tipTrans = trans*tip.colwise().homogeneous();
            Eigen::Vector2d leftTrans = trans*left.colwise().homogeneous();
            Eigen::Vector2d rightTrans = trans*right.colwise().homogeneous();

            points[i][0] = cv::Point( tipTrans(0), tipTrans(1) );
            points[i][1] = cv::Point( leftTrans(0), leftTrans(1) );
            points[i][2] = cv::Point( rightTrans(0), rightTrans(1) );
            shapes[i] = points[i];
            numPoints[i] = 3;
        }

        cv::fillPoly( img, shapes, numPoints, 3, CV_RGB( 255, 0, 0 ), 8 );
    }

    
    
}   