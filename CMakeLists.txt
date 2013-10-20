# setup the project
# =================

cmake_minimum_required(VERSION 2.6)
project(points2grid)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH} )

# set build type
# ==============

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING 
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel" FORCE)
endif()
message(STATUS "Setting ${CMAKE_PROJECT_NAME} build type - ${CMAKE_BUILD_TYPE}")

set (P2G_VERSION_MAJOR 1)
set (P2G_VERSION_MINOR 1)
set (P2G_VERSION_PATCH 0)

set (P2G_VERSION_STRING "${P2G_VERSION_MAJOR}.${P2G_VERSION_MINOR}.${P2G_VERSION_PATCH}")

# setup dependencies
# ==================

# Boost (required) 
# ----------------

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package( Boost 1.40 COMPONENTS iostreams program_options system filesystem REQUIRED )

if(Boost_FOUND
  AND Boost_IOSTREAMS_FOUND
  AND Boost_PROGRAM_OPTIONS_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

# make these available for the user to set.
mark_as_advanced(CLEAR Boost_INCLUDE_DIR) 
mark_as_advanced(CLEAR Boost_LIBRARY_DIRS) 
link_directories(${Boost_LIBRARY_DIRS}) 

set(Boost_SEMVER
  ${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}
  CACHE STRING "The Boost version you are linking against.")

#provide a fallback in case version detection fails
mark_as_advanced(Boost_SEMVER)

# message("Boost semantic version:" ${Boost_SEMVER})
if(Boost_SEMVER VERSION_LESS 1.44.0)
  message("Using old Boost.Iostreams")
  set(OLD_BOOST_IOSTREAMS TRUE)
endif()

# cURL (optional)
# ---------------

find_package(CURL)
include_directories(${CURL_INCLUDE_DIRS})



# bzip2
# -----

find_package(BZip2)
if(BZIP2_FOUND)
  include_directories(${BZIP2_INCLUDE_DIRS})
else()
  message(STATUS "BZip2 not found.  Builds using packaged Boost libraries may fail.")
endif()

# generate our configuration header
# =================================

configure_file(${PROJECT_SOURCE_DIR}/include/points2grid/config.h.in
  ${PROJECT_SOURCE_DIR}/include/points2grid/config.h)

# set flags for GCC (from original Makefile)
# ==========================================

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # implicit large file support
  execute_process(COMMAND getconf LFS_CFLAGS
    OUTPUT_VARIABLE LFS_CFLAGS
    ERROR_QUIET)

  add_definitions(
    -Wall
    -g
    -D_GNU_SOURCE
    -D_LARGEFILE_SOURCE
    ${LFS_CFLAGS})

  if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-m64)
  endif()
endif()

# and now the actual project that we'll build
# ===========================================

set(SRC_DIR src)
set (INCLUDE_DIR include/points2grid)
set (APPS_DIR apps)

include_directories(
    .
    include)

set(DEFAULT_INCLUDE_SUBDIR include)

set(DEFAULT_BIN_SUBDIR bin)
set(DEFAULT_LIB_SUBDIR lib)
  
set(LIBRARY_CPP
  ${SRC_DIR}/GridFile.cpp
  ${SRC_DIR}/GridMap.cpp
  ${SRC_DIR}/InCoreInterp.cpp
  ${SRC_DIR}/Interpolation.cpp
  ${SRC_DIR}/OutCoreInterp.cpp

)

set(POINTS2GRID_HPP
  ${INCLUDE_DIR}/config.h
  ${INCLUDE_DIR}/Interpolation.hpp
  ${INCLUDE_DIR}/OutCoreInterp.hpp
  ${INCLUDE_DIR}/CoreInterp.hpp
  ${INCLUDE_DIR}/Global.hpp
  ${INCLUDE_DIR}/GridFile.hpp
  ${INCLUDE_DIR}/GridMap.hpp
  ${INCLUDE_DIR}/GridPoint.hpp
  ${INCLUDE_DIR}/InCoreInterp.hpp    
)

# setup source groups
# -------------------

source_group("CMake Files" FILES CMakeLists.txt)
source_group("Header Files" FILES ${POINTS2GRID_HPP})
source_group("Source Files" FILES ${POINTS2GRID_CPP})


# build the actual executable
# ---------------------------

set(P2G_LIB_NAME pts2grd)
add_library(${P2G_LIB_NAME} STATIC ${LIBRARY_CPP} ${POINTS2GRID_HPP})
target_link_libraries(${P2G_LIB_NAME} ${Boost_LIBRARIES} ${CURL_LIBRARIES})

if (BZIP2_FOUND)
  target_link_libraries(${P2G_LIB_NAME} ${BZIP2_LIBRARIES})
endif()

if (APPLE)
  set_target_properties(
    ${P2G_LIB_NAME}
    PROPERTIES
    INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib")
endif()


add_executable(points2grid ${APPS_DIR}/points2grid.cpp)
target_link_libraries(points2grid ${P2G_LIB_NAME})

  
# install the executable
# ----------------------

install(TARGETS points2grid
  RUNTIME DESTINATION ${DEFAULT_BIN_SUBDIR}
  LIBRARY DESTINATION ${DEFAULT_LIB_SUBDIR}
  ARCHIVE DESTINATION ${DEFAULT_LIB_SUBDIR})

install(TARGETS ${P2G_LIB_NAME} 
  RUNTIME DESTINATION ${DEFAULT_BIN_SUBDIR}
  LIBRARY DESTINATION ${DEFAULT_LIB_SUBDIR}
  ARCHIVE DESTINATION ${DEFAULT_LIB_SUBDIR})

install(DIRECTORY ${INCLUDE_DIR}
  DESTINATION ${DEFAULT_INCLUDE_SUBDIR}
  FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
