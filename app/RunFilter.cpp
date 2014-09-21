#include "rspf/Map.h"
#include "rspf/RobotLogReader.h"

#include <iostream>

int main( int argc, char* argv[] ) {

    if( argc < 3 ) {
        std::cout << "Please specify map and data filenames." << std::endl;
        return -1;
    }

    std::string mapFilename( argv[1] );
    std::cout << "Reading map at: " << mapFilename << std::endl;

    rspf::Map map( mapFilename );

    std::string logFilename( argv[2] );
    std::cout << "Reading log at: " << logFilename << std::endl;

    rspf::RobotLogReader log( logFilename );
    while( log.HasData() ) {
        rspf::SensorData data = log.GetNextData();
        std::cout << "Read: " << data << std::endl;
    }
    
    return 0;
    
}