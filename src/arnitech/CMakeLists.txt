CMAKE_MINIMUM_REQUIRED( VERSION 2.8.3 )
PROJECT( arnitech )

IF( UNIX )
    SET(CMAKE_CXX_FLAGS_DEBUG "-O0 --std=c++11 -g -pg -Wall -Wno-strict-aliasing")	
	SET(CMAKE_CXX_FLAGS_RELEASE "-O3 --std=c++11 -DNDEBUG -Wall -Wno-strict-aliasing")
	IF( ${CMAKE_SIZEOF_VOID_P} EQUAL 4)
        MESSAGE( FATAL_ERROR "Linux32 is Unsupported" )
	ENDIF( ${CMAKE_SIZEOF_VOID_P} EQUAL 4)
	SET(PLATFORM "linux64" )
ENDIF( UNIX )

IF( WIN32 )
	SET(CMAKE_CXX_FLAGS_DEBUG "/Od /openmp /W3 /Zi")	
	SET(CMAKE_CXX_FLAGS_RELEASE "/O2 /openmp /Oi /Ot /W3")	
	IF( ${CMAKE_SIZEOF_VOID_P} EQUAL 4)
		MESSAGE( FATAL_ERROR "Win32 is Unsupported" )
	ENDIF( ${CMAKE_SIZEOF_VOID_P} EQUAL 4)
	SET( PLATFORM "win64" )
ENDIF( WIN32 )

IF( NOT PLATFORM )
	MESSAGE(FATAL_ERROR "Unable to detect platform")
ENDIF( NOT PLATFORM )

ADD_DEFINITIONS(-DGLEW_STATIC
                -DFREEIMAGE_LIB
                -D_CRT_SECURE_NO_WARNINGS)

INCLUDE_DIRECTORIES( ./ )
INCLUDE_DIRECTORIES( ../ )
INCLUDE_DIRECTORIES( ${LIB_DIR}/jsoncpp/include )
INCLUDE_DIRECTORIES( ${LIB_DIR}/glew/include )
INCLUDE_DIRECTORIES( ${LIB_DIR}/freeimage/include )
INCLUDE_DIRECTORIES( ${LIB_DIR}/sdl2/include )
INCLUDE_DIRECTORIES( ${LIB_DIR}/sdl2/${PLATFORM}/include )
INCLUDE_DIRECTORIES( ${LIB_DIR}/box2d/include )
INCLUDE_DIRECTORIES( ${LIB_DIR}/jsoncpp/include )
INCLUDE_DIRECTORIES( ${LIB_DIR}/microprofile/include )


SET( SOURCE_FILES 
    ${LIB_DIR}/jsoncpp/jsoncpp.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/Buffer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/Effect.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/Effect.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/Globals.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/Globals.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/GLSL.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/GLSL.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/PixelShader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/PixelShader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/Renderer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/Renderer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/RendererConfig.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/RendererConfig.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/Sprite.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/Sprite.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/SpriteRenderer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/SpriteRenderer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/Texture.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/Texture.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/VertexShader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/renderer/VertexShader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/ResourceLoader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/resources/PathFinder.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/resources/PathFinder.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/Tiled.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/Tiled.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/Log.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/Log.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/system/Events.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/system/Config.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/system/Config.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/system/Context.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/system/Context.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/system/EventManager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/system/EventManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/Body.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/Body.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/Camera.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/Camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/StaticSprite.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/StaticSprite.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/Entity.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/Entity.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/Level.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/scene/Level.cpp
	)

FIND_PACKAGE(OpenGL REQUIRED)
LINK_DIRECTORIES( ${LIB_DIR}/freeimage/${PLATFORM} )
LINK_DIRECTORIES( ${LIB_DIR}/glew/${PLATFORM} )
LINK_DIRECTORIES( ${LIB_DIR}/box2d/${PLATFORM} )
LINK_DIRECTORIES( ${LIB_DIR}/sdl2/${PLATFORM} )

ADD_LIBRARY( ${PROJECT_NAME} ${SOURCE_FILES} )

SET( ARNITECH_INCLUDE_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR} 
    ${LIB_DIR}/freeimage/include
    ${LIB_DIR}/glew/include
    PARENT_SCOPE
)

