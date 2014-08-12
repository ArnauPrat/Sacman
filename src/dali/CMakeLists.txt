CMAKE_MINIMUM_REQUIRED( VERSION 2.8.3 )
PROJECT( dali )

INCLUDE(FindPkgConfig)
##PKG_SEARCH_MODULE(GLEW REQUIRED glew)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)

INCLUDE_DIRECTORIES(./)
INCLUDE_DIRECTORIES(../)
INCLUDE_DIRECTORIES(${LIB_DIR}/soil/include)
INCLUDE_DIRECTORIES(${LIB_DIR}/jsoncpp)

INCLUDE(ExternalProject)
ExternalProject_Add( glew
    PREFIX ${LIB_DIR}/glew
    ##    GIT_REPOSITORY git://git.code.sf.net/p/glew/code 
    URL http://downloads.sourceforge.net/project/glew/glew/1.11.0/glew-1.11.0.tgz
    CONFIGURE_COMMAND make -C <SOURCE_DIR>/auto 
    BUILD_IN_SOURCE 1
    BUILD_COMMAND make
    INSTALL_COMMAND ""
    )
INCLUDE_DIRECTORIES(${LIB_DIR}/glew/src/glew/include )

ExternalProject_Add( freeimage 
    PREFIX ${LIB_DIR}/freeimage
    URL http://sourceforge.net/projects/freeimage/files/Source%20Distribution/3.16.0/FreeImage3160.zip/download 
    CONFIGURE_COMMAND ""
    BUILD_IN_SOURCE 1
    BUILD_COMMAND make
    INSTALL_COMMAND ""
    )
INCLUDE_DIRECTORIES(${LIB_DIR}/freeimage/src/freeimage/Source )

ExternalProject_Add( jsoncpp 
    PREFIX ${LIB_DIR}/jsoncpp
    GIT_REPOSITORY  https://github.com/open-source-parsers/jsoncpp.git
    CONFIGURE_COMMAND cmake . 
    BUILD_IN_SOURCE 1
    BUILD_COMMAND make 
    INSTALL_COMMAND ""
    )
INCLUDE_DIRECTORIES( ${LIB_DIR}/jsoncpp/src/jsoncpp/include )

FILE( GLOB_RECURSE SOURCE_FILES "./*" )
ADD_LIBRARY( ${PROJECT_NAME} ${SOURCE_FILES} )
add_dependencies( dali glew freeimage jsoncpp )
TARGET_LINK_LIBRARIES( ${PROJECT_NAME} ${OPENGL_LIBRARIES} ${LIB_DIR}/glew/src/glew/lib/libGLEW.a ${LIB_DIR}/freeimage/src/freeimage/Dist/libfreeimage.a ${LIB_DIR}/jsoncpp/src/jsoncpp/lib/libjsoncpp.a )
