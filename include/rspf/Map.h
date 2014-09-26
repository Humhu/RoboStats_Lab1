#ifndef _MAP_H_
#define _MAP_H_

#include <opencv2/core/core.hpp>

namespace rspf {

    class Map {
    public:

        typedef cv::Mat MapType;
        
        /*! \brief Reads a formatted map from a file. */
        Map( const std::string& filename );

        unsigned int GetXSize() const;
        unsigned int GetYSize() const;
        
        /*! \brief Returns the map value at (x,y). Rounds indices. */
        double GetValue( double x, double y ) const;

        const MapType& GetMap() const;
        
    protected:

        MapType map;
        
    };
    
}

#endif