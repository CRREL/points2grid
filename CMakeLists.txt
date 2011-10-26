cmake_minimum_required(VERSION 2.8)

set(POINTS2GRID_CPP
  GridFile.cpp
  GridMap.cpp
  InCoreInterp.cpp
  Interpolation.cpp
  main.cpp
  OutCoreInterp.cpp
)

set(POINTS2GRID_HPP
  CoreInterp.h
  Global.h
  GridFile.h
  GridMap.h
  GridPoint.h
  InCoreInterp.h
  Interpolation.h
  OutCoreInterp.h
)

source_group("CMake Files" FILES CMakeLists.txt)
source_group("Header Files" FILES ${POINTS2GRID_HPP})
source_group("Source Files" FILES ${POINTS2GRID_CPP})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(curl REQUIRED)
include_directories(${CURL_INCLUDE_DIRS})

find_package(LibLAS REQUIRED)
include_directories(${LIBLAS_INCLUDE_DIR})

add_executable(points2grid ${POINTS2GRID_HPP} ${POINTS2GRID_CPP})
target_link_libraries(points2grid ${CURL_LIBRARIES} ${LIBLAS_LIBRARIES})
