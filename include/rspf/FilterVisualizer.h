#ifndef _FILTER_VISUALIZER_H_
#define _FILTER_VISUALIZER_H_

#include "rspf/Map.h"
#include "rspf/ParticleFilter.h"
#include "rspf/PoseSE2.h"
#include "rspf/Parameterized.h"
#include "rspf/RobotLogReader.h"

#include <opencv2/highgui/highgui.hpp>

namespace rspf {

    class FilterVisualizer {
    public:

		FilterVisualizer( ParticleFilter& filter, const Map& map, const PropertyTree& ptree );

		void ShowRaytraces( const std::vector< std::vector<double> >& rays );

		void Update();
        void Update( const SensorData& data );
        
    protected:
        
        ParticleFilter& filter;
        const Map& map;
        const std::string windowName;
		
		cv::VideoWriter outputVideo;

		bool showScans;
		bool makeVideo;
        double mapScale; // Scale from Map dimensions
        double robotSize; // Size in pixels
        double particleSubsample;

		std::vector< std::vector<double> > lastTraces;
		std::shared_ptr<SensorData> lastData; // Caching for smoothing in between laser scans
        cv::Mat mapImage;
		cv::Mat currentImage;
        
        static bool windowThreadInitialized;

		void Initialize();
		
        // TODO Make vector of Particle instead
        void PlotRobotPoses( cv::Mat& img, const std::vector<Particle>& poses );
		void PlotScans( cv::Mat& img, const std::vector<Particle>& poses, const SensorData& data );
    };
    
}

#endif