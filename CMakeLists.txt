# setup the project
# =================

cmake_minimum_required(VERSION 2.6)
project(points2grid CXX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

# set build type
# ==============

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING 
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel" FORCE)
endif()
message(STATUS "Setting ${CMAKE_PROJECT_NAME} build type - ${CMAKE_BUILD_TYPE}")

# setup dependencies
# ==================

# Boost (required) 
# ----------------

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package( Boost 1.40 COMPONENTS program_options system REQUIRED )

if(Boost_FOUND AND Boost_PROGRAM_OPTIONS_FOUND)
   include_directories(${Boost_INCLUDE_DIRS})
endif()

# make these available for the user to set.
mark_as_advanced(CLEAR Boost_INCLUDE_DIR) 
mark_as_advanced(CLEAR Boost_LIBRARY_DIRS) 
link_directories(${Boost_LIBRARY_DIRS}) 

set(Boost_SEMVER
  ${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION})
message("Boost semantic version:" ${Boost_SEMVER})
if(Boost_SEMVER VERSION_LESS 1.44.0)
  message("Using old Boost.Iostreams")
  set(OLD_BOOST_IOSTREAMS TRUE)
  set(Boost_IOSTREAMS_SOURCE_DIR ${PROJECT_SOURCE_DIR}/boost/1_40_0/iostreams)
else()
  set(Boost_IOSTREAMS_SOURCE_DIR ${PROJECT_SOURCE_DIR}/boost/1_44_0/iostreams)
endif()

add_definitions(-DBOOST_IOSTREAMS_NO_LIB)

# cURL (optional)
# ---------------

find_package(CURL)
include_directories(${CURL_INCLUDE_DIRS})

# libLAS
# ------

find_package(LibLAS REQUIRED)
include_directories(${LIBLAS_INCLUDE_DIR})

# generate our configuration header
# =================================

configure_file(${PROJECT_SOURCE_DIR}/config.h.in
  ${PROJECT_SOURCE_DIR}/config.h)

# set flags for GCC (from original Makefile)
# ==========================================

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  add_definitions(
    -O3 -Wall -g
    -D_GNU_SOURCE
    -D_FILE_OFFSET_BITS=64
    -D_LARGEFILE_SOURCE
    -D_USE_LARGEFILE64)

  if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-m64)
  endif()
endif()

# and now the actual project that we'll build
# ===========================================

set(POINTS2GRID_CPP
  GridFile.cpp
  GridMap.cpp
  InCoreInterp.cpp
  Interpolation.cpp
  main.cpp
  OutCoreInterp.cpp
  ${Boost_IOSTREAMS_SOURCE_DIR}/mapped_file.cpp
)

set(POINTS2GRID_HPP
  config.h
  CoreInterp.h
  Global.h
  GridFile.h
  GridMap.h
  GridPoint.h
  InCoreInterp.h
  Interpolation.h
  OutCoreInterp.h
)

# setup source groups
# -------------------

source_group("CMake Files" FILES CMakeLists.txt)
source_group("Header Files" FILES ${POINTS2GRID_HPP})
source_group("Source Files" FILES ${POINTS2GRID_CPP})


# ensure the compiler knows we're using C++
# -----------------------------------------

set_source_files_properties(${POINTS2GRID_HPP} PROPERTIES LANGUAGE CXX)

# build the actual executable
# ---------------------------

add_executable(points2grid ${POINTS2GRID_HPP} ${POINTS2GRID_CPP})
target_link_libraries(points2grid ${Boost_LIBRARIES} ${CURL_LIBRARIES}
  ${LIBLAS_LIBRARY})
