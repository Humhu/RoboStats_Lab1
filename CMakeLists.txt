# CMake root for RoboStats particle filter lab
# Ellen Cappo, Humphrey Hu, Siddarth Swaminathan

# RoboStats Particle Filter
project( RSPF )

cmake_minimum_required( VERSION 2.8 )

if( NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "" )
  message( STATUS "No build type selected, default to Release" )
  set( CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE )
endif()
message( STATUS "Build configuration: ${CMAKE_BUILD_TYPE}" )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set( LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/lib" CACHE PATH
    "Target directory for all libraries." )
set( EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/bin" CACHE PATH
    "Target directory for all executables." )

add_definitions( -Wall -std=c++0x )

# Find packages we will be using

# Eigen for linear algebra and matrices
find_package( Eigen REQUIRED )
message( STATUS "Eigen found at: ${Eigen_INCLUDE_DIRS}" )
include_directories( ${Eigen_INCLUDE_DIRS} )

# Threading and Boost utilities
find_package( Threads REQUIRED )
list( APPEND RSPF_LIBRARIES ${CMAKE_THREAD_LIBS_INIT} )

find_package( Boost REQUIRED )
include_directories( ${Boost_INCLUDE_DIRS} )
list( APPEND RSPF_LIBRARIES ${Boost_LIBRARIES} )

set( RSPF_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/include" )
include_directories( ${RSPF_INCLUDE_DIRECTORIES} )

add_subdirectory( src )



