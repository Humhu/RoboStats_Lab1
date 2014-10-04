#ifndef _FILTER_VISUALIZER_H_
#define _FILTER_VISUALIZER_H_

#include "rspf/Map.h"
#include "rspf/ParticleFilter.h"
#include "rspf/PoseSE2.h"
#include "rspf/Parameterized.h"

#include <opencv2/highgui/highgui.hpp>

namespace rspf {

    class FilterVisualizer {
    public:

        FilterVisualizer( ParticleFilter& filter, const Map& map, const std::string& windowName );
		FilterVisualizer( ParticleFilter& filter, const Map& map, const PropertyTree& ptree );
		
        void Update();
        
    protected:
        
        ParticleFilter& filter;
        const Map& map;
        const std::string windowName;
        
        double mapScale; // Scale from Map dimensions
        double robotSize; // Size in pixels
        double particleSubsample;
        
        cv::Mat mapImage;
        
        static bool windowThreadInitialized;

		void Initialize();
		
        // TODO Make vector of Particle instead
        void PlotRobotPoses( cv::Mat& img, std::vector<Particle>& poses );
    };
    
}

#endif