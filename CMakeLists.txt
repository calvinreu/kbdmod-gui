#set minimum cmake version
cmake_minimum_required( VERSION 3.25 )

#load version from command line
if ( NOT VERSION )
    message( FATAL_ERROR "No version specified" )
else()
    message( STATUS "Version: ${VERSION}" )
endif()

#debug
if ( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    message( STATUS "Debug build" )
    add_compile_definitions( DEBUG )
    add_compile_options( -g -O0 )
endif()

project( "kbdmod-gui" VERSION ${VERSION} LANGUAGES CXX )
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

#set version in main.cpp by #define
add_compile_definitions(VERSION="${VERSION}")

#link gtk4
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

#set include directories for gtk4
include_directories( "/usr/include/gtk-4.0" )
include_directories( "/usr/include/glib-2.0" )
include_directories( "/usr/lib/glib-2.0/include" )
include_directories( "/usr/include/cairo" )
include_directories( "/usr/include/pango-1.0" )
include_directories( "/usr/include/harfbuzz" )
#gdk
include_directories( "/usr/include/gdk-pixbuf-2.0" )
#graphene
include_directories( "/usr/include/graphene-1.0" )
include_directories( "/usr/lib/graphene-1.0/include" )

#get cpp files from src
file(GLOB_RECURSE SOURCES "src/*.cpp")

#link gtk4
link_directories( "/usr/bin" )

#build executable
add_executable( ${PROJECT_NAME} ${SOURCES} )

#link gtk4
target_link_libraries( ${PROJECT_NAME} ${GTK4_LIBRARIES} )