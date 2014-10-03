#include "rspf/RobotLogReader.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

namespace rspf {

    SensorData::SensorData( const PoseSE2& disp, double time ) :
        displacement( disp ),
        timestamp( time ),
        hasScan( false ),
        points() {}

    SensorData::SensorData( const PoseSE2& disp, double time, const std::array<double, ScanSize>& pts,
                            const PoseSE2& laserOff ) :
        displacement( disp ),
        timestamp( time ),
        hasScan( true ),
        points( pts ),
        laserOffset( laserOff ) {}

    std::ostream& operator<<( std::ostream& os, const SensorData& data ) {
        os << "Displacement: " << data.displacement << " t: " << data.timestamp;
        if( data.hasScan ) {
            os << "Laser Offset: " << data.laserOffset
               << " Scan:";
            for( unsigned int i = 0; i < SensorData::ScanSize; i++ ) {
                os << " " << data.points[i];
            }
        }
        return os;
    }
        
    
    RobotLogReader::RobotLogReader( const std::string& filename ) :
        positionInitialized( false ), logPath( filename ) {

        Initialize();
    }

	RobotLogReader::RobotLogReader( const PropertyTree& ptree ) :
		positionInitialized( false ),
		logPath( ptree.get_child("log_reader").get<std::string>("log_path") ) {

		Initialize();
	}

	void RobotLogReader::Initialize() {
		logFile.open( logPath );
		if( !logFile.is_open() ) {
			throw std::runtime_error( "Could not open log: " + logPath );
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

            double lx = boost::lexical_cast<double>( tokens[4] );
            double ly = boost::lexical_cast<double>( tokens[5] );
            double lth = boost::lexical_cast<double>( tokens[6] );
            PoseSE2 laserPose( lx, ly, lth );
            PoseSE2 laserOffset = pos.Inverse() * laserPose;
            
            SensorData::Scan points;
            for( unsigned int i = 0; i < SensorData::ScanSize; i++ ) {
                points[i] = boost::lexical_cast<double>( tokens[7 + i] );
            }

            SensorData data( displacement, time, points, laserOffset );
            return data;
        }
        else {
            throw std::runtime_error("Read invalid line type of: " + tokens[0] );
        }
    }
    
}