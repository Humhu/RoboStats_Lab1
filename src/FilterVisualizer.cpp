#include "rspf/FilterVisualizer.h"

#include <opencv2/imgproc/imgproc.hpp>

namespace rspf {

    bool FilterVisualizer::windowThreadInitialized = false;

    FilterVisualizer::FilterVisualizer( ParticleFilter& _filter, const Map& _map,
										const PropertyTree& ptree ) :
		filter( _filter ),
		map( _map ),
		windowName( ptree.get<std::string>("window_name") ),
		mapScale( ptree.get<double>("map_scale") ),
		robotSize( ptree.get<double>("robot_size") ),
		particleSubsample( ptree.get<double>("particle_subsample") ),
		showScans( ptree.get<bool>("show_scans") ) {

		Initialize();
	}

    void FilterVisualizer::Initialize() {

		// TODO Possible race condition here if parallelized construction
		if( !windowThreadInitialized ) {
			windowThreadInitialized = true;
			cv::startWindowThread();
		}
		
		// Initialize window
		cv::namedWindow( windowName, CV_WINDOW_AUTOSIZE );
		
// 		double width = map.GetXSize();
// 		double height = map.GetYSize();
		double width = map.GetXSize();
		double height = map.GetYSize();
		
		// Read map image
// 		cv::Mat temp = cv::Mat( map.GetXSize(), map.GetYSize(), CV_8UC3 );
// 		for( unsigned int x = 0; x < map.GetXSize(); x++ ) {
// 			for( unsigned int y = 0; y < map.GetYSize(); y++ ) {
		cv::Mat temp = cv::Mat( width, height, CV_8UC3 );
		for( unsigned int x = 0; x < width; x++ ) {
			for( unsigned int y = 0; y < height; y++ ) {	
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
		
// 		cv::Size scaledSize( std::round( width*mapScale ),
// 							 std::round( height*mapScale ) );
// 		cv::Mat resizeTemp = cv::Mat( scaledSize.width, scaledSize.height, CV_8UC3 );
		cv::Size scaledSize( std::round( height*mapScale ),
							  std::round( width*mapScale ) );
		cv::Mat resizeTemp = cv::Mat( scaledSize.width, scaledSize.height, CV_8UC3 );		
		resize( temp, resizeTemp, scaledSize );
		
		mapImage = resizeTemp.clone();
	}
 
    void FilterVisualizer::Update( const SensorData& data ) {

        currentImage = mapImage.clone();

        // Add things to the image here
        // TODO!
        std::vector<Particle> particles = filter.GetParticles();

		if( showScans ) {
			
			if( data.hasScan ) {
				lastData = std::make_shared<SensorData>( data );
			}
			if( lastData ) {
				PlotScans( currentImage, particles, *lastData );
			}
			
		}
        PlotRobotPoses( currentImage, particles );

        cv::imshow( windowName, currentImage );
    }

    void FilterVisualizer::PlotScans( cv::Mat& img, const std::vector<Particle>& particles, const SensorData& data ) {

		unsigned int numPoses = particles.size();
		unsigned int numToPlot = std::ceil( numPoses/particleSubsample );
		
		cv::Point points[numToPlot][SensorData::ScanSize + 1];
		const cv::Point* shapes[numToPlot];
		int numPoints[numToPlot];

		// First get scan points
		Eigen::Vector2d scanPoints[SensorData::ScanSize];
		double theta = SensorData::StartAngle;
		for( unsigned int s = 0; s < SensorData::ScanSize; s++ ) {
			double r = data.points[s];
			scanPoints[s](0) = r*std::cos(theta);
			scanPoints[s](1) = r*std::sin(theta);
			theta += SensorData::ScanResolution;
		}

		for( unsigned int i = 0; i < numToPlot; i++ ) {

			PoseSE2 pose = particles[i*particleSubsample].getPose() * data.laserOffset;
			PoseSE2::Transform trans = pose.GetTransform();
			
			for( unsigned int s = 0; s < SensorData::ScanSize; s++ ) {
				
				Eigen::Vector2d transformed = trans*scanPoints[s].colwise().homogeneous();
				points[i][s] = cv::Point( transformed(1), transformed(0) );
			}
			
			points[i][SensorData::ScanSize] = cv::Point( pose.getY(), pose.getX() );
			shapes[i] = points[i];
			numPoints[i] = SensorData::ScanSize + 1;

		}
			cv::fillPoly( img, shapes, numPoints, numToPlot, CV_RGB( 255, 255, 0 ), 8 );
			
	}

    void FilterVisualizer::PlotRobotPoses( cv::Mat& img, const std::vector<Particle>& particles ) {

        unsigned int numPoses = particles.size();
		unsigned int numToPlot = std::ceil( numPoses/particleSubsample );

        // Untransformed triangle points
        Eigen::Vector2d tip;
        tip << robotSize*2.0/3, 0;
        Eigen::Vector2d left;
        left << -robotSize/3.0, robotSize/4.0;
        Eigen::Vector2d right;
        right << -robotSize/3.0, -robotSize/4.0;
        
        // Each pose is plotted as a triangle
		cv::Point points[numToPlot][3];
		const cv::Point* shapes[numToPlot];
		int numPoints[numToPlot];
        
		for( unsigned int i = 0; i < numToPlot; i++ ) {

            // Transform the points using the poses
            PoseSE2::Transform trans = particles[i*particleSubsample].getPose().GetTransform();
            trans.translation() = mapScale*trans.translation();
            Eigen::Vector2d tipTrans = trans*tip.colwise().homogeneous();
            Eigen::Vector2d leftTrans = trans*left.colwise().homogeneous();
            Eigen::Vector2d rightTrans = trans*right.colwise().homogeneous();

            points[i][0] = cv::Point( tipTrans(1), tipTrans(0) );
            points[i][1] = cv::Point( leftTrans(1), leftTrans(0) );
            points[i][2] = cv::Point( rightTrans(1), rightTrans(0) );
            shapes[i] = points[i];
            numPoints[i] = 3;
        }

        cv::fillPoly( img, shapes, numPoints, numToPlot, CV_RGB( 255, 0, 0 ), 8 );
    }

    
    
}   