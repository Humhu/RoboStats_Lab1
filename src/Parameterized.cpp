#include "rspf/Parameterized.h"

#include <boost/property_tree/xml_parser.hpp>

namespace rspf {

	PropertyTree read_property_xml( const std::string& filepath ) {

		PropertyTree ptree;

		try {
			boost::property_tree::read_xml( filepath, ptree );
		}
		catch( std::exception e ) {
			std::stringstream ss;
			ss << "Error reading file: " << filepath;
			throw std::runtime_error( ss.str() );
		}
		
		return ptree;
	}
	
}