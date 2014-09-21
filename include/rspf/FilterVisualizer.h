#ifndef _FILTER_VISUALIZER_H_
#define _FILTER_VISUALIZER_H_

#include "rspf/Map.h"
//#include "rspf/ParticleFilter.h"

#include <opencv2/highgui/highgui.hpp>

namespace rspf {

    // TODO Remove once ParticleFilter is in the master branch
    class ParticleFilter;
    
    class FilterVisualizer {
    public:

        FilterVisualizer( const ParticleFilter& filter, const Map& map, const std::string& windowName );

        void Update();
        
    protected:

        const ParticleFilter& filter;
        const Map& map;
        const std::string windowName;
        
        static bool windowThreadInitialized;
    };
    
}

#endif