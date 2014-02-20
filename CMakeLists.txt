cmake_minimum_required (VERSION 2.6) 
project (asearch)

message( STATUS "${PROJECT_SOURCE_DIR}: " ${PROJECT_SOURCE_DIR} )

include_directories ( ${PROJECT_SOURCE_DIR}/include )
link_directories    ( ${PROJECT_SOURCE_DIR}/lib )

add_executable (asearch 
	asearch.cpp 
	utils.cpp
	sigproc.cpp
	filters.cpp
    )

add_executable (amatch 
	amatch.cpp 
	)

target_link_libraries (asearch 
	#fftw3
    libfftw3-3
	libsndfile-1.lib 
	#samplerate 
	#armadillo 
	#sndfile 
	#boost_iostreams 
	#boost_system 
    )   

