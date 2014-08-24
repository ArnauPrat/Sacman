CMAKE_MINIMUM_REQUIRED( VERSION 2.8.3 )
PROJECT( sacman )

#detecting platform and setting variables
IF( UNIX )
	SET(CMAKE_CXX_FLAGS_DEBUG "-O0 --std=c++11 -g -pg -Wall")	
	SET(CMAKE_CXX_FLAGS_RELEASE "-O3 --std=c++11 -DNDEBUG -Wall")
	IF( ${CMAKE_SIZEOF_VOID_P} EQUAL 4)
        MESSAGE( FATAL_ERROR "Linux32 is Unsupported" )
	ENDIF( ${CMAKE_SIZEOF_VOID_P} EQUAL 4)
	SET(PLATFORM "linux64" )
ENDIF( UNIX )

IF( WIN32 )
	SET(CMAKE_CXX_FLAGS_DEBUG "/Od /openmp /W3")	
	SET(CMAKE_CXX_FLAGS_RELEASE "/O2 /openmp /Oi /Ot /Og /W3")	
	IF( ${CMAKE_SIZEOF_VOID_P} EQUAL 4)
		MESSAGE( FATAL_ERROR "Win32 is Unsupported" )
	ENDIF( ${CMAKE_SIZEOF_VOID_P} EQUAL 4)
	SET( PLATFORM "win64" )
ENDIF( WIN32 )

IF( NOT PLATFORM )
	MESSAGE(FATAL_ERROR "Unable to detect platform")
ENDIF( NOT PLATFORM )

SET(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release)
endif()


ADD_SUBDIRECTORY( ${CMAKE_CURRENT_SOURCE_DIR}/src/dali )


#library directories
INCLUDE_DIRECTORIES( src )
INCLUDE_DIRECTORIES( ${LIB_DIR}/sdl2/include )
INCLUDE_DIRECTORIES( ${LIB_DIR}/sdl2/${PLATFORM}/include )
INCLUDE_DIRECTORIES( ${LIB_DIR}/box2d/include )
INCLUDE_DIRECTORIES( ${LIB_DIR}/jsoncpp/include )
INCLUDE_DIRECTORIES( ${DALI_INCLUDE_DIRS} )


SET( SOURCE_FILES 

        ${LIB_DIR}/jsoncpp/jsoncpp.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/Config.cpp 
		${CMAKE_CURRENT_SOURCE_DIR}/src/Context.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/EventManager.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/Level.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/Tiled.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/objects/Background.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/objects/Character.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/objects/Entity.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/objects/Portal.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/objects/Body.cpp
 )


LINK_DIRECTORIES( ${LIB_DIR}/box2d/${PLATFORM} )
LINK_DIRECTORIES( ${LIB_DIR}/sdl2/${PLATFORM} )

SET( SDL2_LIBRARIES 
				    sdl2	
					sdl2main
)

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_FILES})	
TARGET_LINK_LIBRARIES(${PROJECT_NAME} dali box2d ${SDL2_LIBRARIES} )
