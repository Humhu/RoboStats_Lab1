#include "rspf/RobotLogReader.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace rspf {

    SensorData::SensorData( const PoseSE2& disp, double time ) :
        displacement( disp ),
        timestamp( time ),
        hasScan( false ),
        points() {}

    SensorData::SensorData( const PoseSE2& disp, double time,
                            const std::array<double, ScanSize>& pts ) :
        displacement( disp ),
        timestamp( time ),
        hasScan( true ),
        points( pts ) {}

    std::ostream& operator<<( std::ostream& os, const SensorData& data ) {
        os << "Displacement: " << data.displacement << " t: " << data.timestamp;
        if( data.hasScan ) {
            os << " Scan:";
            for( unsigned int i = 0; i < SensorData::ScanSize; i++ ) {
                os << " " << data.points[i];
            }
        }
        return os;
    }
        
    
    RobotLogReader::RobotLogReader( const std::string& filename ) :
        positionInitialized( false ), logFile( filename ) {

        if( !logFile.is_open() ) {
            throw std::runtime_error( "Could not open log: " + filename );
        }

        getline( logFile, currentLine );
    }

    bool RobotLogReader::HasData() const {
        return currentLine.find_first_not_of(' ') != std::string::npos;
    }
    
    SensorData RobotLogReader::GetNextData() {
        
        std::vector<std::string> tokens;
        boost::split( tokens, currentLine, boost::algorithm::is_any_of(" ") );

        // After we've split we can get the next line
        getline( logFile, currentLine );
        
        double x = boost::lexical_cast<double>( tokens[1] );
        double y = boost::lexical_cast<double>( tokens[2] );
        double th = boost::lexical_cast<double>( tokens[3] );
        PoseSE2 pos( x, y, th );

        if( !positionInitialized ) {
            positionInitialized = true;
            lastPosition = pos;
        }

        // Perform differencing to get robot frame displacements
        PoseSE2 displacement = lastPosition.Inverse() * pos;
        lastPosition = pos;
        
        if( tokens[0] == "O" ) {
            
            double time = boost::lexical_cast<double>( tokens[4] );
            
            SensorData data( displacement, time );
            return data;
        }
        else if( tokens[0] == "L" ) {

            double time = boost::lexical_cast<double>( tokens[187] );
            SensorData::Scan points;
            for( unsigned int i = 0; i < SensorData::ScanSize; i++ ) {
                points[i] = boost::lexical_cast<double>( tokens[7 + i] );
            }

            SensorData data( displacement, time, points );
            return data;
        }
        else {
            throw std::runtime_error("Read invalid line type of: " + tokens[0] );
        }
    }
    
}