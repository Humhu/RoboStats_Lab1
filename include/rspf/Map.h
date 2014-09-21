#ifndef _MAP_H_
#define _MAP_H_

#include <opencv2/core/core.hpp>

namespace rspf {

    class Map {
    public:

        /*! \brief Reads a formatted map from a file. */
        Map( const std::string& filename );

        unsigned int GetXSize() const;
        unsigned int GetYSize() const;
        
        /*! \brief Returns the map value at (x,y). Rounds indices. */
        double GetValue( double x, double y ) const;

    protected:

        cv::Mat map;
        
    };
    
}

#endif