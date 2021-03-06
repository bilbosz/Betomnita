﻿cmake_minimum_required( VERSION 3.12 )

option( Betomnita_DEBUG "Debug mode" ON )
option( Betomnita_FULLSCREEN "Fullscreen mode" OFF )
option( Betomnita_CONSOLE "Show console" ON )
option( Betomnita_CONSOLE_STOP "Waits for key pressed after program execution" OFF )
option( Betomnita_REDIRECT_CONSOLE_OUT "Redirects console out and error stream to stdout.log and stderr.log files" OFF )
option( Betomnita_FIX_POLYGON "Fixes polygons to be parsed by Betomnita without point changes" ON )
option( Betomnita_SAVE_POLYGON_TRIANGULATION "Saves triangulation in SVG files. Improve performance on big number vertices polygons. Implies FIX_POLYGON" OFF )

set(SFML_REQUIRED_VERSION 2.5.0)
set( Boost_REQUIRED_VERSION 1.66 )

set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH} )

set( CMAKE_BUILD_TYPE )
if ( Betomnita_DEBUG )
    set( CMAKE_BUILD_TYPE Debug )
else()
    set( CMAKE_BUILD_TYPE Release )
endif()

set( Betomnita_PROJECT Betomnita )
project( "${Betomnita_PROJECT}" )

set( Betomnita_VERSION_MAJOR 0 )
set( Betomnita_VERSION_MINOR 0 )
set( Betomnita_VERSION_PATCH 0 )

set( MAIN_TARGET "Betomnita" )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

add_compile_definitions( BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE )
add_compile_definitions( _CRT_SECURE_NO_WARNINGS )
if ( Betomnita_DEBUG )
    add_compile_definitions( DEBUG )
endif()

if( ${CMAKE_SIZEOF_VOID_P} STREQUAL "8" )
    set( ARCH "x86_64" )
else()
    set( ARCH "x86" )
endif()

set_property( GLOBAL PROPERTY USE_FOLDERS ON )
set_property( GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake" )
set_property( DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${MAIN_TARGET}" )

configure_file(
    "${CMAKE_SOURCE_DIR}/src/project/Config.hpp.in"
    "${CMAKE_BINARY_DIR}/include/project/Config.hpp"
)
include_directories( "${CMAKE_BINARY_DIR}/include" )

set( RES "${CMAKE_SOURCE_DIR}/res" )
file( COPY "${CMAKE_SOURCE_DIR}/res" DESTINATION "${CMAKE_BINARY_DIR}" )
message( STATUS "Resources ${CMAKE_SOURCE_DIR}/res copied into ${CMAKE_BINARY_DIR}" )

set( SRC )
file(
    GLOB_RECURSE SRC
    LIST_DICTIONARIES false
    "${CMAKE_SOURCE_DIR}/src/*.hpp"
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)
include_directories( "${CMAKE_SOURCE_DIR}/src" )

if( ${CMAKE_SYSTEM_NAME} STREQUAL Windows AND NOT Betomnita_CONSOLE )
	add_executable(
		${MAIN_TARGET}
		WIN32
		${SRC}
	)
else()
    add_executable(
        ${MAIN_TARGET}
        ${SRC}
    )
endif()

if (${CMAKE_SYSTEM_NAME} STREQUAL Windows)
    set(SFML_STATIC_LIBRARIES TRUE)
else ()
    set(SFML_STATIC_LIBRARIES FALSE)
endif ()
set(SFML_REQUIRED_MODULES system window graphics audio network)
find_package(SFML ${SFML_REQUIRED_VERSION} COMPONENTS ${SFML_REQUIRED_MODULES} REQUIRED)
target_link_libraries(${MAIN_TARGET} sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

set( BOOST_ROOT ${Betomnita_Boost_ROOT} )
find_package( Boost ${Boost_REQUIRED_VERSION} REQUIRED )
if( Boost_FOUND )
    include_directories( ${Boost_INCLUDE_DIRS} )
    target_link_libraries( ${MAIN_TARGET} ${Boost_LIBRARIES} )
endif()

if( DEFINED ENV{BOX2D_ROOT} )
    message( STATUS "Found Box2d in $ENV{BOX2D_ROOT}" )
    include_directories( $ENV{BOX2D_ROOT} )
    if (${CMAKE_SYSTEM_NAME} STREQUAL Windows)
        set(BOX2D_LIB "$ENV{BOX2D_ROOT}/Build/bin/${ARCH}/${CMAKE_BUILD_TYPE}/Box2D.lib")
    else ()
        set(BOX2D_LIB "$ENV{BOX2D_ROOT}/Build/bin/${ARCH}/${CMAKE_BUILD_TYPE}/libBox2D.a")
    endif ()
    if( EXISTS ${BOX2D_LIB} )
        message( STATUS "Found Box2d library in ${BOX2D_LIB}" )
        target_link_libraries( ${MAIN_TARGET} ${BOX2D_LIB} )
    else()
        message( FATAL_ERROR "Box2d library not found" )
    endif()
else()
    message(FATAL_ERROR "Box2d not found")
endif()

if( DEFINED ENV{PUGIXML_SRC} )
    message( STATUS "Found pugixml in $ENV{PUGIXML_SRC}" )
    include_directories($ENV{PUGIXML_SRC})
    find_library(PUGIXML_LIB
            NAMES pugixml
            HINTS "/usr/lib/x86_64-linux-gnu"
            )
    target_link_libraries(${MAIN_TARGET} "/usr/lib/x86_64-linux-gnu/libpugixml.so")
else()
    message(FATAL_ERROR "pugixml not found")
endif()

add_custom_target( "CMAKE_RELOAD"
    COMMAND cmake .
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DEPENDS "${CMAKE_SOURCE_DIR}/CMakeLists.txt"
)

add_custom_target( "CMAKE_GUI"
    COMMAND cmake-gui ${CMAKE_SOURCE_DIR}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target( "OPEN_EXPLORER"
	COMMAND explorer .
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

install( TARGETS "${MAIN_TARGET}" DESTINATION "." )
install( DIRECTORY "${RES}" DESTINATION "." )

set( CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt" )
set( CPACK_PACKAGE_VERSION_MAJOR "${Betomnita_VERSION_MAJOR}" )
set( CPACK_PACKAGE_VERSION_MINOR "${Betomnita_VERSION_MINOR}" )
set( CPACK_PACKAGE_VERSION_PATCH "${Betomnita_VERSION_PATCH}" )
set( CPACK_MONOLITHIC_INSTALL 1 )
include( CPack )