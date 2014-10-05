#include "rspf/Map.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace rspf {

	Map::Map( const PropertyTree& ptree ) :
		scale( ptree.get<double>("scale") ) {
		scaleInv = 1.0/scale;
		Initialize( ptree.get<std::string>( "map_path" ) );
	}

	void Map::Initialize( const std::string& filename ) {
		
        // Attempt to open the file
        std::ifstream filestream( filename );
        if( !filestream.is_open() ) {
            throw std::runtime_error( "Could not open file: " + filename );
        }

        // Read the header first
        std::string line;
        unsigned int xdim = 0;
        unsigned int ydim = 0;
        std::vector<std::string> tokens;
        while( !filestream.eof() ) {
            std::getline( filestream, line );

            // This line contains the dimensions
            if( line.find( "global_map[0]" ) != std::string::npos ) {
                boost::split( tokens, line, boost::algorithm::is_any_of(" ") );
                xdim = boost::lexical_cast<unsigned int>( tokens[1] );
                ydim = boost::lexical_cast<unsigned int>( tokens[2] );
                std::cout << "Read map dimensions: " << xdim << " by " << ydim << std::endl;
                break;
            }
        }

        // CV_64F corresponds to double
        map = cv::Mat::zeros( xdim, ydim, CV_64F );
        
        // Now all following lines are data
        for( unsigned int j = 0; j < ydim; j++ ) {
            
            tokens.clear();
            std::getline( filestream, line );
            boost::split( tokens, line, boost::algorithm::is_any_of(" "), boost::token_compress_on );

            // Sometimes it has extra whitespace at the end
            if( tokens.size() < xdim ) {
                throw std::runtime_error( "Line had wrong number of elements." );
            }

            for( unsigned int i = 0; i < xdim; i++ ) {
				CellType value = boost::lexical_cast<CellType>( tokens[i] );
				if( value > 2.0 ) {
					std::cout << "Element " << tokens[i] << " converted to " << value << std::endl;
				}
				map.at<CellType>(xdim-i-1,j) = value; //<---------------------------------------------------
            }
        }

        xLim = (map.rows-1)*scale;
		yLim = (map.cols-1)*scale;
    }

    unsigned int Map::GetXSize() const {
        return xLim;
    }

    unsigned int Map::GetYSize() const {
        return yLim;
    }

    const Map::MapType& Map::GetMap() const {
        return map;
    }

    double Map::GetScale() const {
		return scale;
	}

    Map::CellType Map::GetValue( double x, double y ) const {
        x = std::round( x*scaleInv );
        y = std::round( y*scaleInv );

		if( x < 0 || x >= map.rows || y < 0 || y >= map.cols ) {
			std::stringstream ss;
			ss << "Position (" << x << ", " << y << ") exceeds map size!";
			throw std::out_of_range( ss.str() );
		}
		
		if( map.at<CellType>( x, y ) > 2 ) {
			std::cout << "Map at (" << x << ", " << y << ") has value " << map.at<CellType>(x,y) << std::endl;
		}
		return map.at<CellType>( x, y );
	}

}
