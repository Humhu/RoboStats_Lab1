#CMake file for src folder

set( RSPF_particlefilter_SOURCES
	 DefaultTransitionModel.cpp
	 FilterVisualizer.cpp
	 LaserSensorModel.cpp
     Map.cpp
     Parameterized.cpp
     Particle.cpp 
     ParticleFilter.cpp
	 PoseSE2.cpp
	 RandomDistributions.cpp
	 RobotLogReader.cpp 
	 SensorModel.cpp
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
