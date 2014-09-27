#ifndef _FILTER_VISUALIZER_H_
#define _FILTER_VISUALIZER_H_

#include "rspf/Map.h"
#include "rspf/ParticleFilter.h"
#include "rspf/PoseSE2.h"

#include <opencv2/highgui/highgui.hpp>

namespace rspf {

    // TODO Remove once ParticleFilter is in the master branch
    class ParticleFilter;
    
    class FilterVisualizer {
    public:

        FilterVisualizer( ParticleFilter& filter, const Map& map, const std::string& windowName );

        void Update();
        
    protected:
        
        ParticleFilter& filter;
        const Map& map;
        const std::string windowName;
        
        double mapScale; // Scale from Map dimensions
        double robotSize; // Size in pixels
        
        cv::Mat mapImage;
        
        static bool windowThreadInitialized;

        // TODO Make vector of Particle instead
        void PlotRobotPoses( cv::Mat& img, std::vector<Particle>& poses );
    };
    
}

#endif