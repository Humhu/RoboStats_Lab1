#CMake file for src folder

set( RSPF_particlefilter_SOURCES
     Dummy.cpp
     FilterVisualizer.cpp
     Map.cpp
     PoseSE2.cpp
     RobotLogReader.cpp )

add_library( RSPF_particlefilter SHARED ${RSPF_particlefilter_SOURCES} )

set_target_properties( RSPF_particlefilter PROPERTIES
    OUTPUT_NAME RSPF_particlefilter )

target_link_libraries( RSPF_particlefilter ${RSPF_LIBRARIES} )