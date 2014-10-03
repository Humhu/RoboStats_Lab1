#ifndef _PARAMETERIZED_H_
#define _PARAMETERIZED_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/exceptions.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <Eigen/Core>

namespace rspf {

	typedef boost::property_tree::ptree PropertyTree;

	/*! \brief Parse an XML file. */
	PropertyTree read_property_xml( const std::string& filepath );

	/*! \brief Parse a matrix from a string. */
	template< class C, unsigned int N, unsigned int M >
	typename Eigen::Matrix<C,N,M> parse_matrix( const std::string& str ) {
		
		std::string block( str );
		boost::erase_all( block, "\n" );
		std::vector<std::string> tokens;
		boost::split( tokens, block, boost::algorithm::is_any_of(" ,\t"), boost::token_compress_on );
		
		if( *tokens.begin() == "\0" ) { tokens.erase( tokens.begin() ); }
		if( *(tokens.end() - 1) == "\0" ) { tokens.erase( ( tokens.end() - 1) ); }
		
		if( tokens.size() != N*M ) {
			std::stringstream errmsg;
			errmsg << "MatrixParser: Received " << tokens.size() << " elements. Matrix should have " << N*M << " elements. Contents: "
			<< block;
			throw std::runtime_error( errmsg.str() );
		}
		
		typename Eigen::Matrix<C,N,M> mat;
		for( unsigned int i = 0; i < N*M; i++) {
			mat(i) = boost::lexical_cast<C>( tokens[i] );
		}
		return mat;
	}
}

#endif