#include "rspf/Map.h"
#include "rspf/RobotLogReader.h"
#include "rspf/FilterVisualizer.h"
#include "rspf/Parameterized.h"

#include <iostream>

using namespace rspf;

int main( int argc, char* argv[] ) {

    if( argc < 2 ) {
        std::cout << "Please specify config file." << std::endl;
        return -1;
    }

    std::string configFilename( argv[1] );
	PropertyTree ptree = read_property_xml( configFilename );

	std::cout << "Initializing map..." << std::endl;
    Map map( ptree.get_child("map") );

	std::cout << "Initializing particle filter nyah..." << std::endl;
    ParticleFilter pf( map, ptree.get_child("particle_filter") );

	std::cout << "Initializing filter visualizer..." << std::endl;
    FilterVisualizer vis( pf, map, ptree.get_child("filter_visualizer") );
	    
	std::cout << "Initializing log reader..." << std::endl;
    RobotLogReader log( ptree.get_child("log_reader") );

	unsigned int lineNumber = 0;
    while( log.HasData() ) {

		SensorData data = log.GetNextData();

// 		std::cout << "Read line " << lineNumber << std::endl;
		lineNumber++;
				
		// apply update from data to particles in the pf
		pf.handleData(data);

		vis.Update(data);



    }
    
    return 0;
    
}