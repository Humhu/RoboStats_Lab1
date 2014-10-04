#CMake file for src folder

set( RSPF_particlefilter_SOURCES
     FilterVisualizer.cpp
     Map.cpp
     Parameterized.cpp
     Particle.cpp 
     ParticleFilter.cpp
	 PoseSE2.cpp
	 RandomDistributions.cpp
	 RobotLogReader.cpp 
	 StaticTransitionModel.cpp
	 SynchronizationPrimitives.cpp
	 Timer.cpp
     TransitionModel.cpp
     WorkerPool.cpp
     )


add_library( RSPF_particlefilter SHARED ${RSPF_particlefilter_SOURCES} )

set_target_properties( RSPF_particlefilter PROPERTIES
    OUTPUT_NAME RSPF_particlefilter )

target_link_libraries( RSPF_particlefilter ${RSPF_LIBRARIES} )
