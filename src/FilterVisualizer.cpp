#include "rspf/FilterVisualizer.h"
#include "rspf/Timer.h"
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
		showScans( ptree.get<bool>("show_scans") ), 
		makeVideo( ptree.get<bool>("make_video") ) {

		if( makeVideo ) {
			videoFilename = ptree.get<std::string>("video_filename");
		}
		Initialize();
	}

// 	FilterVisualizer::~FilterVisualizer(){
// 		if( outputVideo.isOpened() ){
// 			outputVideo.close
// 		}
// 	}//de-c0nstruct-z0r
	
    void FilterVisualizer::Initialize() {

		// TODO Possible race condition here if parallelized construction
		if( !windowThreadInitialized ) {
			windowThreadInitialized = true;
			cv::startWindowThread();
		}
		
		// Initialize window
		cv::namedWindow( windowName, CV_WINDOW_AUTOSIZE );
		
		double mapWidth = map.GetXSize();
		double mapHeight = map.GetYSize();
		double imageWidth = std::round( mapWidth * mapScale );
		double imageHeight = std::round( mapHeight * mapScale );
		
		// Read map image
		mapImage = cv::Mat( imageWidth, imageHeight, CV_8UC3 );
		for( unsigned int x = 0; x < imageWidth; x++ ) {
			for( unsigned int y = 0; y < imageHeight; y++ ) {
				double mapVal = map.GetValue( x/mapScale, y/mapScale );
				
				if( mapVal == -1.0 ) {
					mapImage.at<cv::Vec3b>(x, y)[0] = 0;
					mapImage.at<cv::Vec3b>(x, y)[1] = 0;
					mapImage.at<cv::Vec3b>(x, y)[2] = 0;
				}
				else {
					mapImage.at<cv::Vec3b>(x, y)[0] = 255*mapVal;
					mapImage.at<cv::Vec3b>(x, y)[1] = 255*mapVal;
					mapImage.at<cv::Vec3b>(x, y)[2] = 255*mapVal;
				}
			}
		}
		
		if( makeVideo ) {
			// TODO open videoWriter
		
			std::stringstream ss;
			Timer myTimer;
			Time fooTime = myTimer.GetTime();

			ss << fooTime;
						
			const std::string myFile = "vid" + ss.str() + ".avi";
			cv::Size frameSize = cv::Size( imageWidth, imageHeight );
			double fps = 1.0;
			// VideoWriter::VideoWriter(const string& myFile, int fourcc, double 30, Size frameSize, bool isColor=true)
			outputVideo.open(videoFilename, CV_FOURCC('M','J','P','G'), fps, frameSize, true);
		}
		
	}

	void FilterVisualizer::Update() {
		currentImage = mapImage.clone();
		std::vector<Particle> particles = filter.GetParticles();
		PlotRobotPoses( currentImage, particles );
		cv::imshow( windowName, currentImage );
		
		if( showScans && lastData ) {
			PlotScans( currentImage, particles, *lastData );
		}
		
		if( makeVideo ) {
			outputVideo.write( currentImage );
		}
	}
 
    void FilterVisualizer::Update( const SensorData& data ) {

        currentImage = mapImage.clone();

        // Add things to the image here
        // TODO!
        std::vector<Particle> particles = filter.GetParticles();

// 		if( !filter.GetLastRaytraces().empty() ) {
// 			lastTraces = filter.GetLastRaytraces();
// 		}
		
		if( showScans ) {

// 			ShowRaytraces( lastTraces );
			
			if( data.hasScan ) {
				lastData = std::make_shared<SensorData>( data );
			}
			if( lastData ) {
				PlotScans( currentImage, particles, *lastData );
			}
			
		}
        PlotRobotPoses( currentImage, particles );

        cv::imshow( windowName, currentImage );
		
		if( makeVideo ) {
			// // TODO videoWriter::write
			// // outputVideo << res;  
			outputVideo.write( currentImage ); 
		}
    }
/*
// 	void FilterVisualizer::ShowRaytraces( const std::vector< std::vector<double> >& rays ) {
// 
// 		std::vector<Particle> particles = filter.GetParticles();
// 
// 		if( rays.size() == 0 ) { return; }
// 		
// 		unsigned int numPoses = particles.size();
// 		unsigned int numToPlot = std::ceil( numPoses/particleSubsample );
// 
// 		cv::Point points[numToPlot][SensorData::ScanSize + 1];
// 		const cv::Point* shapes[numToPlot];
// 		int numPoints[numToPlot];
// 
// 		for( unsigned int i = 0; i < numToPlot; i++ ) {
// 
// 			PoseSE2 pose = particles[i*particleSubsample].getPose();
// 			PoseSE2::Transform trans = pose.GetTransform();
// 			trans.translation() = trans.translation() * mapScale;
// 
// 			double theta = SensorData::StartAngle;
// 			for( unsigned int s = 0; s < SensorData::ScanSize; s++ ) {
// 
// 				Eigen::Vector2d scanPoint;
// 				double r = rays[i][s] * mapScale;
// 				scanPoint << r * std::cos( theta ),
// 							 r * std::sin( theta );
// 				theta += SensorData::ScanResolution;
// 
// 				Eigen::Vector2d transformed = trans*scanPoint.colwise().homogeneous();
// 				points[i][s] = cv::Point( transformed(1), transformed(0) );
// 			}
// 
// 			points[i][SensorData::ScanSize] = cv::Point( trans.translation().y(), trans.translation().x() );
// 			shapes[i] = points[i];
// 			numPoints[i] = SensorData::ScanSize + 1;
// 
// 		}
// 		cv::fillPoly( currentImage, shapes, numPoints, numToPlot, CV_RGB( 0, 255, 0 ), 8 );
// 		
// 	}
*/    
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
			double r = data.points[s] * mapScale;
			scanPoints[s](0) = r*std::cos(theta);
			scanPoints[s](1) = r*std::sin(theta);
			theta += SensorData::ScanResolution;
		}

		for( unsigned int i = 0; i < numToPlot; i++ ) {

			PoseSE2 pose = particles[i*particleSubsample].getPose() * data.laserOffset;
			PoseSE2::Transform trans = pose.GetTransform();
			trans.translation() = trans.translation() * mapScale;
			
			for( unsigned int s = 0; s < SensorData::ScanSize; s++ ) {
				
				Eigen::Vector2d transformed = trans*scanPoints[s].colwise().homogeneous();
				points[i][s] = cv::Point( transformed(1), transformed(0) );
			}
			
			points[i][SensorData::ScanSize] = cv::Point( trans.translation().y(), trans.translation().x() );
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
            trans.translation() = trans.translation() * mapScale;
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