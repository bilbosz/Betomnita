cmake_minimum_required( VERSION 3.12 )

option( Betomnita_DEBUG "Debug mode" ON )
option( Betomnita_FULLSCREEN "Fullscreen mode" ON )
option( Betomnita_CONSOLE "Show console" OFF )
option( Betomnita_CONSOLE_STOP "Waits for key pressed after program execution" ON )
option( Betomnita_REDIRECT_CONSOLE_OUT "Redirects console out and error stream to stdout.log and stderr.log files" ON )

set( SFML_REQUIRED_VERSION 2.4.2 )
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


set( SFML_STATIC_LIBRARIES TRUE )
set( SFML_REQUIRED_MODULES system window graphics audio network )
if( ${CMAKE_SYSTEM_NAME} STREQUAL Windows )
    set( SFML_REQUIRED_MODULES ${SFML_REQUIRED_MODULES} main )
endif()
find_package( SFML ${SFML_REQUIRED_VERSION} REQUIRED ${SFML_REQUIRED_MODULES} )
if ( SFML_FOUND )
    include_directories( ${SFML_INCLUDE_DIR} )
    target_link_libraries( ${MAIN_TARGET} ${SFML_DEPENDENCIES} ${SFML_LIBRARIES} )
endif()

set( BOOST_ROOT ${Betomnita_Boost_ROOT} )
find_package( Boost ${Boost_REQUIRED_VERSION} REQUIRED )
if( Boost_FOUND )
    include_directories( ${Boost_INCLUDE_DIRS} )
    target_link_libraries( ${MAIN_TARGET} ${Boost_LIBRARIES} )
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

