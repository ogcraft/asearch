cmake_minimum_required (VERSION 2.6) 
project (amatch)

message( STATUS "${PROJECT_SOURCE_DIR}: " ${PROJECT_SOURCE_DIR} )

#include_directories ( ${PROJECT_SOURCE_DIR}/include ) # win32
link_directories    ( ${PROJECT_SOURCE_DIR}/lib )

add_executable (generate_pfkeys 
	asearch.cpp 
	utils.cpp
	sigproc.cpp
	filters.cpp
    )

add_executable (amatch 
	amatch.cpp 
	)

target_link_libraries ( generate_pfkeys
 	fftw3
	sndfile 
	#libfftw3-3 # win32
	#libsndfile-1.lib # win32 
	#samplerate 
	#armadillo 
	#boost_iostreams 
	#boost_system 
    )   

