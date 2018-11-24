CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

if(MSVC)
	if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")
	else()
		set(CMAKE_BUILD_TYPE "Release")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  /fp:fast /Gy /Oi /Oy /O2 /Ot /Zi /EHsc ")
	endif()
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
else()
	if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall -Wno-unused-variable")
	else( CMAKE_BUILD_TYPE STREQUAL "Debug" )
			set(CMAKE_BUILD_TYPE "Release")
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
	endif( CMAKE_BUILD_TYPE STREQUAL "Debug" )
endif()

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/gles/include/
	${CMAKE_CURRENT_SOURCE_DIR}
)

set( GLES_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/gles )

find_library( EGL libEGL "libEGL.lib" ${GLES_LIB_DIR} )
find_library( GLESv2 libGLESv2 "libGLESv2.lib" ${GLES_LIB_DIR} )
find_library( MaliEmulator libMaliEmulator "libMaliEmulator.lib" ${GLES_LIB_DIR} )

add_subdirectory( samples )
