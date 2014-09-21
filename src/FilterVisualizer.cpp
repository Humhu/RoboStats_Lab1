#include "rspf/FilterVisualizer.h"

namespace rspf {

    bool FilterVisualizer::windowThreadInitialized = false;

    FilterVisualizer::FilterVisualizer( const ParticleFilter& _filter, const Map& _map,
                                        const std::string& _windowName ) :
        filter( _filter ),
        map( _map ),
        windowName( _windowName ) {

        if( !windowThreadInitialized ) {
            windowThreadInitialized = true;
            cv::startWindowThread();
        }

        cv::namedWindow( windowName, CV_WINDOW_AUTOSIZE );
        
    }

    void FilterVisualizer::Update() {

        Map::MapType mapCopy = map.GetMap().clone();

        cv::imshow( windowName, mapCopy );
    }
    
}