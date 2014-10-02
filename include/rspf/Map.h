#ifndef _MAP_H_
#define _MAP_H_

#include <opencv2/core/core.hpp>

namespace rspf {

	/*! \class Map Map.h
	 * \brief An occupancy grid representation of a 2D world. */
    class Map {
    public:

        typedef cv::Mat MapType;
		typedef double CellType;

		static const CellType FilledValue;
		
        /*! \brief Reads a formatted map from a file. */
        Map( const std::string& filename );

        unsigned int GetXSize() const;
        unsigned int GetYSize() const;
        
        /*! \brief Returns the map value at (x,y). Double version rounds the indices. */
		Map::CellType GetValue( unsigned int x, unsigned int y ) const;
        Map::CellType GetValue( double x, double y ) const;

        const MapType& GetMap() const;
        
    protected:

        MapType map;
        
    };
    
}

#endif