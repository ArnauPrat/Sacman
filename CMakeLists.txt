CMAKE_MINIMUM_REQUIRED( VERSION 2.8.3 )
PROJECT( sacman )

#setting variables
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 --std=c++11 -g -pg -Wall")	
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 --std=c++11 -DNDEBUG -Wall")	
SET(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/modules")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release)
endif()

INCLUDE(ExternalProject)
ExternalProject_Add( SDL2 
    PREFIX ${LIB_DIR}/SDL2
    URL http://libsdl.org/release/SDL2-2.0.3.tar.gz
    CONFIGURE_COMMAND cmake -B.. -H.
    BUILD_IN_SOURCE 1
    BUILD_COMMAND make -C..
    INSTALL_COMMAND ""
    )
INCLUDE_DIRECTORIES( ${LIB_DIR}/SDL2/src/SDL2/include )

ExternalProject_Add( Box2D 
    PREFIX ${LIB_DIR}/Box2D
    SVN_REPOSITORY http://box2d.googlecode.com/svn/tags/v2.3.1
    CONFIGURE_COMMAND cmake -B. -H./Box2D -DBOX2D_BUILD_EXAMPLES=OFF
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND ""
    )
INCLUDE_DIRECTORIES(${LIB_DIR}/Box2D/src/Box2D/Box2D/ )

ADD_SUBDIRECTORY( ./src/dali )
INCLUDE_DIRECTORIES( ./src )

#ExternalProject_Add( jsoncpp 
#    PREFIX ${LIB_DIR}/jsoncpp
#    GIT_REPOSITORY  https://github.com/open-source-parsers/jsoncpp.git
#    CONFIGURE_COMMAND cmake . 
#    BUILD_IN_SOURCE 1
#    BUILD_COMMAND make 
#    INSTALL_COMMAND ""
#    )
INCLUDE_DIRECTORIES( ${LIB_DIR}/jsoncpp/src/jsoncpp/include )

INCLUDE_DIRECTORIES(${LIB_DIR}/glew/src/glew/include )

#FILE( GLOB_RECURSE SOURCE_FILES "src/*" )
SET( SOURCE_FILES ${SOURCE_FILES} "src/Config.cpp" )
SET( SOURCE_FILES ${SOURCE_FILES} "src/Context.cpp" )
SET( SOURCE_FILES ${SOURCE_FILES} "src/EventManager.cpp" )
SET( SOURCE_FILES ${SOURCE_FILES} "src/Level.cpp" )
SET( SOURCE_FILES ${SOURCE_FILES} "src/Tiled.cpp" )
SET( SOURCE_FILES ${SOURCE_FILES} "src/main.cpp" )
SET( SOURCE_FILES ${SOURCE_FILES} "src/objects/Background.cpp" )
SET( SOURCE_FILES ${SOURCE_FILES} "src/objects/Character.cpp" )
SET( SOURCE_FILES ${SOURCE_FILES} "src/objects/Entity.cpp" )
SET( SOURCE_FILES ${SOURCE_FILES} "src/objects/Portal.cpp" )
SET( SOURCE_FILES ${SOURCE_FILES} "src/objects/Body.cpp" )


LINK_DIRECTORIES(${LIB_DIR}/Box2D)

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_FILES})	
TARGET_LINK_LIBRARIES(${PROJECT_NAME} dali ${LIB_DIR}/SDL2/src/libSDL2.a ${LIB_DIR}/SDL2/src/libSDL2main.a ${LIB_DIR}/Box2D/src/Box2D/Box2D/libBox2D.a  )
